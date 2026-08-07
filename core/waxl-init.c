#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reboot.h>
#include <fcntl.h>
#include <common.h>

#define READY_FD 3

typedef struct {
    const char *source;
    const char *target;
    const char *type;
    unsigned long flags;
    const char *data;
} mount_point_t;

static const mount_point_t vfs_mounts[] = {
    { "proc",          "/proc",         "proc",       MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "sysfs",         "/sys",          "sysfs",      MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "devtmpfs",      "/dev",          "devtmpfs",   MS_NOSUID,                        "mode=0755" },
    { "devpts",        "/dev/pts",      "devpts",     MS_NOEXEC | MS_NOSUID,            "gid=5,mode=0620" },
    { "tmpfs",         "/dev/shm",      "tmpfs",      MS_NODEV  | MS_NOEXEC | MS_NOSUID, "mode=1777,size=64M" },
    { "tmpfs",         "/tmp",          "tmpfs",      MS_NODEV  | MS_NOSUID,             "mode=1777" },
    { "tmpfs",         "/run",          "tmpfs",      MS_NODEV  | MS_NOSUID,             "mode=0755" },
    { "binder",        "/dev/binderfs", "binder",     MS_NOEXEC | MS_NOSUID, NULL },
    { "binfmt_misc",   "/proc/sys/fs/binfmt_misc", "binfmt_misc", MS_NODEV | MS_NOEXEC | MS_NOSUID, NULL },
};

static void make_dir(const char *path) {
    if (mkdir(path, 0755) < 0 && errno != EEXIST) {
        NL_ERROR("Failed to mkdir %s: %s", path, strerror(errno));
    }
}

static bool setup_vfs(void) {
    NL_INFO("Mounting virtual filesystems...");
    size_t count = sizeof(vfs_mounts) / sizeof(vfs_mounts[0]);

    for (size_t i = 0; i < count; i++) {
        const mount_point_t *m = &vfs_mounts[i];
        make_dir(m->target);

        if (mount(m->source, m->target, m->type, m->flags, m->data) < 0) {
            if (errno == EBUSY) {
                continue;
            }
            NL_ERROR("Failed to mount %s on %s: %s", m->source, m->target, strerror(errno));
            return false;
        }
    }

    // Ensure /dev/console and standard stdio descriptors exist
    int console_fd = open("/dev/console", O_RDWR);
    if (console_fd >= 0) {
        dup2(console_fd, STDIN_FILENO);
        dup2(console_fd, STDOUT_FILENO);
        dup2(console_fd, STDERR_FILENO);
        if (console_fd > STDERR_FILENO) {
            close(console_fd);
        }
    }

    return true;
}

static void handle_signal(int sig) {
    switch (sig) {
        case SIGCHLD:
            // Non-blocking reap of all dead child processes
            while (waitpid(-1, NULL, WNOHANG) > 0);
            break;
        case SIGTERM:
        case SIGINT:
            NL_INFO("Shutdown signal received, syncing filesystems...");
            sync();
            reboot(RB_POWER_OFF);
            break;
        default:
            break;
    }
}

static void setup_signals(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    signal(SIGPIPE, SIG_IGN);
}

static pid_t spawn_daemon(const char *path, char *const argv[], int ready_pipe_write) {
    pid_t pid = fork();
    if (pid < 0) {
        NL_ERROR("Failed to fork daemon %s: %s", path, strerror(errno));
        return -1;
    }

    if (pid == 0) {
        if (ready_pipe_write >= 0) {
            if (ready_pipe_write != READY_FD) {
                dup2(ready_pipe_write, READY_FD);
                close(ready_pipe_write);
            }
        }
        execv(path, argv);
        NL_ERROR("Failed to exec %s: %s", path, strerror(errno));
        _exit(127);
    }

    NL_INFO("Spawned %s (PID: %d)", path, pid);
    return pid;
}

static pid_t spawn_shell(void) {
    pid_t pid = fork();
    if (pid < 0) {
        NL_ERROR("Failed to fork shell: %s", strerror(errno));
        return -1;
    }

    if (pid == 0) {
        setsid();
        ioctl(STDIN_FILENO, TIOCSCTTY, 1);

        char *shell_argv[] = { "-sh", NULL };
        execv("/bin/sh", shell_argv);

        shell_argv[0] = "-sh";
        execv("/sh", shell_argv);

        NL_ERROR("Failed to exec shell: %s", strerror(errno));
        _exit(127);
    }

    NL_INFO("Spawned /bin/sh (PID: %d)", pid);
    return pid;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    if (getpid() != 1) {
        NL_ERROR("waxl-init must be executed as PID 1!");
        return EXIT_FAILURE;
    }

    NL_INFO("init waxl");

    if (!setup_vfs()) {
        NL_ERROR("VFS setup failed. Halting.");
        while (1) pause();
    }

    setup_signals();

    int ready_pipe[2] = { -1, -1 };
    if (pipe(ready_pipe) < 0) {
        NL_ERROR("Failed to create readiness pipe: %s", strerror(errno));
    }

    char *incubator_argv[] = { "/bin/waxl-incubator", NULL };
    pid_t incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv, ready_pipe[1]);

    if (incubator_pid < 0) {
        incubator_argv[0] = "./waxl-incubator";
        incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv, ready_pipe[1]);
    }

    if (ready_pipe[1] >= 0) {
        close(ready_pipe[1]);
    }

    if (ready_pipe[0] >= 0) {
        char buf;
        while (read(ready_pipe[0], &buf, 1) > 0);
        close(ready_pipe[0]);
    }

    NL_INFO("boot done");

    sleep(2);
    printf("\n\n");

    pid_t shell_pid = spawn_shell();

    while (1) {
        int status;
        pid_t exited_pid = wait(&status);

        if (exited_pid < 0) {
            if (errno == EINTR) continue;
            NL_ERROR("wait error: %s", strerror(errno));
            sleep(1);
            continue;
        }

        if (exited_pid == incubator_pid) {
            NL_ERROR("waxl-incubator unexpectedly died! Restarting in 1 second...");
            sleep(1);
            incubator_argv[0] = "/bin/waxl-incubator";
            incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv, -1);
            if (incubator_pid < 0) {
                incubator_argv[0] = "./waxl-incubator";
                incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv, -1);
            }
        } else if (exited_pid == shell_pid) {
            NL_INFO("Shell exited! Respawning in 1 second...");
            sleep(1);
            shell_pid = spawn_shell();
        }
    }

    return EXIT_SUCCESS;
}
