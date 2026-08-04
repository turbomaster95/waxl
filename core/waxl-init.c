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

#define LOG_PREFIX "[waxl-init] "

typedef struct {
    const char *source;
    const char *target;
    const char *type;
    unsigned long flags;
    const char *data;
} mount_point_t;

static const mount_point_t vfs_mounts[] = {
    { "proc",     "/proc",     "proc",     MS_NODEV | MS_NOEXEC | MS_NOSUID, NULL },
    { "sysfs",    "/sys",      "sysfs",    MS_NODEV | MS_NOEXEC | MS_NOSUID, NULL },
    { "devtmpfs", "/dev",      "devtmpfs", MS_NOSUID,                        "mode=0755" },
    { "devpts",   "/dev/pts",  "devpts",   MS_NOEXEC | MS_NOSUID,            "gid=5,mode=0620" },
    { "tmpfs",    "/dev/shm",  "tmpfs",    MS_NODEV | MS_NOEXEC | MS_NOSUID, "mode=1777,size=64M" },
    { "tmpfs",    "/tmp",      "tmpfs",    MS_NODEV | MS_NOSUID,             "mode=1777" },
    { "tmpfs",    "/run",      "tmpfs",    MS_NODEV | MS_NOSUID,             "mode=0755" }
};

static void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(LOG_PREFIX);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, LOG_PREFIX "ERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

static void make_dir(const char *path) {
    if (mkdir(path, 0755) < 0 && errno != EEXIST) {
        log_error("What?? Failed to mkdir %s: %s", path, strerror(errno));
    }
}

static bool setup_vfs(void) {
    log_info("Mounting virtual filesystems...");
    size_t count = sizeof(vfs_mounts) / sizeof(vfs_mounts[0]);

    for (size_t i = 0; i < count; i++) {
        const mount_point_t *m = &vfs_mounts[i];
        make_dir(m->target);

        if (mount(m->source, m->target, m->type, m->flags, m->data) < 0) {
            if (errno == EBUSY) {
                continue;
            }
            log_error("Oh noes!! Failed to mount %s on %s: %s", m->source, m->target, strerror(errno));
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
            log_info("Shutdown signal received, syncing filesystems...");
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

    // Ignore SIGPIPE so broken sockets/pipes don't crash init
    signal(SIGPIPE, SIG_IGN);
}

static pid_t spawn_daemon(const char *path, char *const argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        log_error(":( Failed to fork daemon %s: %s", path, strerror(errno));
        return -1;
    }

    if (pid == 0) {
        // Child Process
        execv(path, argv);
        log_error(":( Failed to exec %s: %s", path, strerror(errno));
        _exit(127);
    }

    log_info("Spawned %s (PID: %d)", path, pid);
    return pid;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Verify we are *actually* running as PID 1 and not as some random pid number
    if (getpid() != 1) {
        log_error("waxl-init must be executed as PID 1!");
        return EXIT_FAILURE;
    }

    log_info("init waxl");

    if (!setup_vfs()) {
        log_error("Oh noes!! VFS setup failed. Halting.");
        while (1) pause();
    }

    setup_signals();

    char *incubator_argv[] = { "/bin/waxl-incubator", NULL };
    pid_t incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv);

    if (incubator_pid < 0) {
        // Fallback search path if /bin/ isn't used
        incubator_argv[0] = "./waxl-incubator";
        incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv);
    }

    log_info("boot done");

    while (1) {
        int status;
        pid_t exited_pid = wait(&status);

        if (exited_pid < 0) {
            if (errno == EINTR) continue;
            log_error("oh noes!! wait error: %s", strerror(errno));
            sleep(1);
            continue;
        }

        // If the main incubator crashes, restart it automatically
        if (exited_pid == incubator_pid) {
            log_error("waxl-incubator unexpectedly died! I'll restart it in 1 second...");
            sleep(1);
            incubator_pid = spawn_daemon(incubator_argv[0], incubator_argv);
        }
    }

    return EXIT_SUCCESS;
}
