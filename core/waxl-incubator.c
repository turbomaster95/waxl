#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <nu.h>
#include <dlfcn.h>

#define INCUBATOR_SOCKET "/tmp/waxl-incubator.sock"

static nu_mm_t *g_mm = NULL;

#define MM_SIZE (1024 * 1024 * 2)

static void sigchld_event_cb(int fd, void *data) {
    (void)fd;
    (void)data;
    while (waitpid(-1, NULL, WNOHANG) > 0);
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

static void handle_ipc_spawn(const char *msg, int client_fd, void *data) {
    (void)data;
    if (!msg || *msg == '\0') {
        nu_fd_write(client_fd, "ERR_EMPTY_COMMAND\n", 18);
        return;
    }

    char cmd_buf[512];
    nu_snprintf(cmd_buf, sizeof(cmd_buf), "%s", msg);
    char *trimmed_cmd = nu_str_trim(cmd_buf);

    int argc = 0;
    char **argv = nu_str_split(trimmed_cmd, " ", &argc);
    if (!argv || argc == 0) {
        nu_fd_write(client_fd, "ERR_INVALID_ARGS\n", 17);
        if (argv) nu_str_free_list(argv, argc);
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        NU_ERROR("Fork failed for command: %s", argv[0]);
        nu_fd_write(client_fd, "ERR_FORK_FAILED\n", 16);
        nu_str_free_list(argv, argc);
        return;
    }

    if (pid == 0) {
        close(client_fd);

        execvp(argv[0], argv);

        NU_ERROR("Failed to exec: %s", argv[0]);
        _exit(EXIT_FAILURE);
    } else {
        char response[64];
        int len = nu_snprintf(response, sizeof(response), "PID %d\n", pid);
        nu_fd_write(client_fd, response, len);

        NU_INFO("Spawned process '%s' (PID: %d)", argv[0], pid);
        nu_str_free_list(argv, argc);
    }
}

int main(void) {
    NU_INFO("WaxL Incubator Starting..");

    preload_resources();

    g_mm = nu_mm_create(NU_MM_BUDDY, NULL, MM_SIZE);
    if (!g_mm) {
        NU_ERROR("Failed to allocate incubator memory manager");
        exit(EXIT_FAILURE);
    }

    nu_loop_t *loop = nu_loop_create(g_mm);
    if (!loop) {
        NU_ERROR("Failed to create event loop");
        return EXIT_FAILURE;
    }

    if (!nu_loop_add_signal(loop, g_mm, SIGCHLD, sigchld_event_cb, NULL)) {
        NU_WARN("Failed to register SIGCHLD loop handler, falling back to signal()");
        signal(SIGCHLD, SIG_IGN);
    }

    NU_INFO("Listening on socket: %s", INCUBATOR_SOCKET);
    nu_ipc_listen(g_mm, INCUBATOR_SOCKET, handle_ipc_spawn, NULL);

    nu_loop_destroy(loop, g_mm);
    nu_mm_destroy(g_mm);

    return EXIT_SUCCESS;
}
