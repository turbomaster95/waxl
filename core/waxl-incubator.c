#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <nu.h>
#include <dlfcn.h>

#define INCUBATOR_SOCKET "/run/incubator.sock"

#define BINFMT_REGISTER "/proc/sys/fs/binfmt_misc/register"
#define BINFMT_WAX_RULE "/proc/sys/fs/binfmt_misc/wax"
#define WAX_BINFMT_RULE ":wax:E::wax::/bin/waxl-exec:P"

#define POLL_INTERVAL_US 100000

typedef int (*app_entry_fn)(int argc, char **argv);

static void sigchld_handler(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void register_wax_rule(void) {
    int fd = open(BINFMT_REGISTER, O_WRONLY);
    if (fd < 0) return;

    ssize_t len = strlen(WAX_BINFMT_RULE);
    if (write(fd, WAX_BINFMT_RULE, len) == len) {
        printf("[incubator] Registered .wax binfmt_misc rule.\n");
    }

    close(fd);
}

void *binfmt_poll_thread(void *arg) {
    (void)arg;
    while (1) {
        if (access(BINFMT_WAX_RULE, F_OK) != 0) {
            register_wax_rule();
        }
        usleep(POLL_INTERVAL_US);
    }
    return NULL;
}

static int receive_fd(int sock, char *arg_buffer, size_t buffer_len) {
    struct msghdr msg = {0};
    struct iovec iov[1];
    iov[0].iov_base = arg_buffer;
    iov[0].iov_len = buffer_len;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    char cmsgbuf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);

    if (recvmsg(sock, &msg, 0) <= 0) {
        return -1;
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        int received_fd;
        memcpy(&received_fd, CMSG_DATA(cmsg), sizeof(int));
        return received_fd;
    }

    return -1;
}

static void preload_resources(void) {
    NU_INFO("Preloading Waxl core runtime and memory pools...");

    void *nu_handle = dlopen("libnu.so", RTLD_NOW | RTLD_GLOBAL);
    if (!nu_handle) {
        NU_ERROR("Failed to preload libnu.so: %s", dlerror());
        exit(EXIT_FAILURE);
    }

    NU_INFO("Successfully preloaded libnu.so at %p", nu_handle);
}

static void accept_and_process_connection(int server_fd) {
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) return;

    char arg_buf[4096] = {0};
    int passed_fd = receive_fd(client_fd, arg_buf, sizeof(arg_buf));

    if (passed_fd < 0) {
        NU_ERROR("Failed to receive memfd over socket");
        close(client_fd);
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        NU_ERROR("Fork failed for incoming application payload");
        close(passed_fd);
        close(client_fd);
        return;
    }

    if (pid == 0) {
        dup2(client_fd, STDOUT_FILENO);
        dup2(client_fd, STDERR_FILENO);
        close(client_fd);

        char memfd_path[64];
        snprintf(memfd_path, sizeof(memfd_path), "/proc/self/fd/%d", passed_fd);

        void *app_handle = dlopen(memfd_path, RTLD_NOW | RTLD_LOCAL);
        if (!app_handle) {
            NU_ERROR("Incubator failed to load memfd library: %s", dlerror());
            _exit(EXIT_FAILURE);
        }

        dlerror();
        app_entry_fn entry = (app_entry_fn)dlsym(app_handle, "main");
        const char *dlsym_err = dlerror();

        if (dlsym_err || !entry) {
            dlerror();
            entry = (app_entry_fn)dlsym(app_handle, "app_main");
            dlsym_err = dlerror();
        }

        if (dlsym_err || !entry) {
            NU_ERROR("Incubator failed to locate entry point ('main' or 'app_main')");
            dlclose(app_handle);
            _exit(EXIT_FAILURE);
        }

        char *argv[64];
        int argc = 0;
        argv[argc++] = "wax_app";

        char *ptr = arg_buf;
        while (*ptr && argc < 63) {
            argv[argc++] = ptr;
            ptr += strlen(ptr) + 1;
        }
        argv[argc] = NULL;

        NU_INFO("Incubator Launching target in-memory...");
        int exit_code = entry(argc, argv);

	fflush(stdout);
        fflush(stderr);

        dlclose(app_handle);
        close(passed_fd);
        _exit(exit_code);
    } else {
        close(passed_fd);
        close(client_fd);
        NU_INFO("Spawned Incubator child (PID: %d)", pid);
    }
}

int main(void) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigchld_handler);

    NU_INFO("WaxL Incubator Starting..");

    preload_resources();

    pthread_t binfmt_thread;
    if (pthread_create(&binfmt_thread, NULL, binfmt_poll_thread, NULL) == 0) {
        pthread_detach(binfmt_thread);
        NU_INFO("Started binfmt_misc rule monitor thread");
    } else {
        NU_WARN("Failed to start binfmt_misc monitor thread");
    }

    unlink(INCUBATOR_SOCKET);

    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, INCUBATOR_SOCKET, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    chmod(INCUBATOR_SOCKET, 0666);

    if (listen(listen_fd, 16) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    NU_INFO("Listening on socket: %s", INCUBATOR_SOCKET);

    while (1) {
        accept_and_process_connection(listen_fd);
    }

    close(listen_fd);
    return EXIT_SUCCESS;
}
