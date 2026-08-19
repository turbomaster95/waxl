#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <common.h>

#define MAX_ARGS 32
#define MAX_SERVICES 32
#define MAX_ACTIONS 64
#define MAX_CMDS_PER_ACTION 32
#define MAX_PROPS 128
#define MAX_LINE 512
#define MAX_ENV 16

typedef struct {
    const char *source;
    const char *target;
    const char *type;
    unsigned long flags;
    const char *data;
} mount_point_t;

static const mount_point_t vfs_mounts[] = {
    { "proc",         "/proc",         "proc",        MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "sysfs",        "/sys",          "sysfs",       MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "devtmpfs",     "/dev",          "devtmpfs",    MS_NOSUID,                         "mode=0755" },
    { "devpts",       "/dev/pts",      "devpts",      MS_NOEXEC | MS_NOSUID,             "gid=5,mode=0620" },
    { "tmpfs",        "/dev/shm",      "tmpfs",       MS_NODEV  | MS_NOEXEC | MS_NOSUID, "mode=1777,size=64M" },
    { "tmpfs",        "/tmp",          "tmpfs",       MS_NODEV  | MS_NOSUID,             "mode=1777" },
    { "tmpfs",        "/run",          "tmpfs",       MS_NODEV  | MS_NOSUID,             "mode=0755" },
    { "binder",       "/dev/binderfs", "binder",      MS_NOEXEC | MS_NOSUID, NULL },
    { "binfmt_misc",  "/proc/sys/fs/binfmt_misc", "binfmt_misc", MS_NODEV | MS_NOEXEC | MS_NOSUID, NULL },
    { "mqueue",       "/dev/mqueue",   "mqueue",      MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "cgroup2",      "/sys/fs/cgroup", "cgroup2",     MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "configfs",     "/sys/kernel/config",    "configfs",    MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
    { "bpf",          "/sys/fs/bpf",   "bpf",         MS_NODEV  | MS_NOEXEC | MS_NOSUID, NULL },
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

typedef struct {
    char key[64];
    char value[128];
} property_t;

static property_t properties[MAX_PROPS];
static size_t property_count = 0;

typedef struct {
    int argc;
    char *argv[MAX_ARGS];
} command_t;

typedef struct {
    char trigger[64];
    command_t cmds[MAX_CMDS_PER_ACTION];
    size_t cmd_count;
} action_t;

static action_t actions[MAX_ACTIONS];
static size_t action_count = 0;

typedef struct {
    char name[64];
    char class_name[32];
    char path[256];
    char *argv[MAX_ARGS];
    int argc;
    bool is_oneshot;
    bool is_critical;
    bool is_disabled;
    bool has_uid;
    bool has_gid;
    uid_t uid;
    gid_t gid;
    char *envp[MAX_ENV];
    int env_count;
    pid_t pid;
} service_t;

static service_t services[MAX_SERVICES];
static size_t service_count = 0;

static void trigger_event(const char *trigger_name);
static void parse_init_rc(const char *filepath);

static const char *get_property(const char *key) {
    for (size_t i = 0; i < property_count; i++) {
        if (strcmp(properties[i].key, key) == 0) {
            return properties[i].value;
        }
    }
    return NULL;
}

static void set_property(const char *key, const char *value) {
    for (size_t i = 0; i < property_count; i++) {
        if (strcmp(properties[i].key, key) == 0) {
            strncpy(properties[i].value, value, sizeof(properties[i].value) - 1);
            NL_INFO("Property set: %s = %s", key, value);

            char prop_trigger[128];
            snprintf(prop_trigger, sizeof(prop_trigger), "property:%s=%s", key, value);
            trigger_event(prop_trigger);
            return;
        }
    }

    if (property_count < MAX_PROPS) {
        strncpy(properties[property_count].key, key, sizeof(properties[property_count].key) - 1);
        strncpy(properties[property_count].value, value, sizeof(properties[property_count].value) - 1);
        property_count++;
        NL_INFO("Property set: %s = %s", key, value);

        char prop_trigger[128];
        snprintf(prop_trigger, sizeof(prop_trigger), "property:%s=%s", key, value);
        trigger_event(prop_trigger);
    }
}

static service_t *find_service(const char *name) {
    for (size_t i = 0; i < service_count; i++) {
        if (strcmp(services[i].name, name) == 0) {
            return &services[i];
        }
    }
    return NULL;
}

static pid_t start_service(service_t *s) {
    if (!s) return -1;

    /* If a disabled service is manually started, clear the disabled 
       flag so the wait loop will respawn it if it crashes. */
    s->is_disabled = false;

    pid_t pid = fork();
    if (pid < 0) {
        NL_ERROR("Failed to fork service %s: %s", s->name, strerror(errno));
        return -1;
    }

    if (pid == 0) {
        /* Reset signal handlers to default before executing the daemon */
        signal(SIGTERM, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);

        setsid();
        ioctl(STDIN_FILENO, TIOCSCTTY, 1);

        if (s->has_gid && setgid(s->gid) < 0) {
            NL_ERROR("Failed to setgid %d for %s", s->gid, s->name);
            _exit(127);
        }
        if (s->has_uid && setuid(s->uid) < 0) {
            NL_ERROR("Failed to setuid %d for %s", s->uid, s->name);
            _exit(127);
        }

        for (int i = 0; i < s->env_count; i++) {
            putenv(s->envp[i]);
        }

        execv(s->path, s->argv);
        NL_ERROR("Failed to exec service %s (%s): %s", s->name, s->path, strerror(errno));
        _exit(127);
    }

    s->pid = pid;
    NL_INFO("Spawned %s (PID: %d)", s->name, pid);
    return pid;
}

static void stop_service(service_t *s) {
    if (s && s->pid > 0) {
        kill(s->pid, SIGTERM);
        NL_INFO("Stopped service %s (PID: %d)", s->name, s->pid);
        s->pid = 0;
    }
}

static void class_start(const char *class_name) {
    for (size_t i = 0; i < service_count; i++) {
        if (strcmp(services[i].class_name, class_name) == 0) {
            if (!services[i].is_disabled && services[i].pid <= 0) {
                start_service(&services[i]);
            }
        }
    }
}

static void class_stop(const char *class_name) {
    for (size_t i = 0; i < service_count; i++) {
        if (strcmp(services[i].class_name, class_name) == 0) {
            stop_service(&services[i]);
        }
    }
}

static void class_reset(const char *class_name) {
    class_stop(class_name);
    class_start(class_name);
}

static void execute_command(int argc, char **argv) {
    if (argc == 0) return;

    if (strcmp(argv[0], "mkdir") == 0 && argc >= 2) {
        mode_t mode = (argc >= 3) ? strtoul(argv[2], NULL, 8) : 0755;
        if (mkdir(argv[1], mode) < 0 && errno != EEXIST) {
            NL_ERROR("mkdir %s failed: %s", argv[1], strerror(errno));
        }
    } else if (strcmp(argv[0], "write") == 0 && argc >= 3) {
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            write(fd, argv[2], strlen(argv[2]));
            close(fd);
        }
    } else if (strcmp(argv[0], "chmod") == 0 && argc >= 3) {
        mode_t mode = strtoul(argv[1], NULL, 8);
        chmod(argv[2], mode);
    } else if (strcmp(argv[0], "chown") == 0 && argc >= 4) {
        uid_t uid = atoi(argv[1]);
        gid_t gid = atoi(argv[2]);
        chown(argv[3], uid, gid);
    } else if (strcmp(argv[0], "symlink") == 0 && argc >= 3) {
        symlink(argv[1], argv[2]);
    } else if (strcmp(argv[0], "rm") == 0 && argc >= 2) {
        unlink(argv[1]);
    } else if (strcmp(argv[0], "setprop") == 0 && argc >= 3) {
        set_property(argv[1], argv[2]);
    } else if (strcmp(argv[0], "trigger") == 0 && argc >= 2) {
        trigger_event(argv[1]);
    } else if (strcmp(argv[0], "import") == 0 && argc >= 2) {
        parse_init_rc(argv[1]);
    } else if (strcmp(argv[0], "start") == 0 && argc >= 2) {
        start_service(find_service(argv[1]));
    } else if (strcmp(argv[0], "stop") == 0 && argc >= 2) {
        stop_service(find_service(argv[1]));
    } else if (strcmp(argv[0], "class_start") == 0 && argc >= 2) {
        class_start(argv[1]);
    } else if (strcmp(argv[0], "class_stop") == 0 && argc >= 2) {
        class_stop(argv[1]);
    } else if (strcmp(argv[0], "class_reset") == 0 && argc >= 2) {
        class_reset(argv[1]);
    } else if (strcmp(argv[0], "restart") == 0 && argc >= 2) {
        service_t *s = find_service(argv[1]);
        stop_service(s);
        start_service(s);
    } else if (strcmp(argv[0], "exec") == 0 && argc >= 2) {
        pid_t pid = fork();
        if (pid == 0) {
            setsid();
            ioctl(STDIN_FILENO, TIOCSCTTY, 1);
            execv(argv[1], &argv[1]);
            _exit(127);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

static void trigger_event(const char *trigger_name) {
    NL_INFO("Triggering event: [%s]", trigger_name);
    for (size_t i = 0; i < action_count; i++) {
        if (strcmp(actions[i].trigger, trigger_name) == 0) {
            for (size_t j = 0; j < actions[i].cmd_count; j++) {
                execute_command(actions[i].cmds[j].argc, actions[i].cmds[j].argv);
            }
        }
    }
}

static void parse_init_rc(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        NL_ERROR("Failed to open init script: %s", filepath);
        return;
    }

    char line[MAX_LINE];
    action_t *current_action = NULL;
    service_t *current_svc = NULL;

    while (fgets(line, sizeof(line), fp)) {
        char *comment = strchr(line, '#');
        if (comment) *comment = '\0';

        char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;

        char *argv[MAX_ARGS];
        int argc = 0;
        char *token = strtok(p, " \t\r\n");
        while (token && argc < MAX_ARGS - 1) {
            argv[argc++] = token;
            token = strtok(NULL, " \t\r\n");
        }
        argv[argc] = NULL;
        if (argc == 0) continue;

        if (strcmp(argv[0], "on") == 0 && argc >= 2) {
            current_svc = NULL;
            if (action_count < MAX_ACTIONS) {
                current_action = &actions[action_count++];
                memset(current_action, 0, sizeof(action_t));
                strncpy(current_action->trigger, argv[1], sizeof(current_action->trigger) - 1);
            }
            continue;
        }

        if (strcmp(argv[0], "service") == 0 && argc >= 3) {
            current_action = NULL;
            current_svc = find_service(argv[1]);
            if (!current_svc && service_count < MAX_SERVICES) {
                current_svc = &services[service_count++];
                memset(current_svc, 0, sizeof(service_t));
                strncpy(current_svc->name, argv[1], sizeof(current_svc->name) - 1);
                strncpy(current_svc->path, argv[2], sizeof(current_svc->path) - 1);
		strncpy(current_svc->class_name, "default", sizeof(current_svc->class_name) - 1);

                for (int i = 2; i < argc; i++) {
                    current_svc->argv[current_svc->argc++] = strdup(argv[i]);
                }
                current_svc->argv[current_svc->argc] = NULL;
            }
            continue;
        }

        if (current_action && current_action->cmd_count < MAX_CMDS_PER_ACTION) {
            command_t *cmd = &current_action->cmds[current_action->cmd_count++];
            cmd->argc = argc;
            for (int i = 0; i < argc; i++) {
                cmd->argv[i] = strdup(argv[i]);
            }
            cmd->argv[argc] = NULL;
        } else if (current_svc) {
            if (strcmp(argv[0], "oneshot") == 0) {
                current_svc->is_oneshot = true;
            } else if (strcmp(argv[0], "critical") == 0) {
                current_svc->is_critical = true;
            } else if (strcmp(argv[0], "disabled") == 0) {
                current_svc->is_disabled = true;
            } else if (strcmp(argv[0], "user") == 0 && argc >= 2) {
                current_svc->uid = atoi(argv[1]);
                current_svc->has_uid = true;
            } else if (strcmp(argv[0], "group") == 0 && argc >= 2) {
                current_svc->gid = atoi(argv[1]);
                current_svc->has_gid = true;
            } else if (strcmp(argv[0], "class") == 0 && argc >= 2) {
	        strncpy(current_svc->class_name, argv[1], sizeof(current_svc->class_name) - 1);
	    } else if (strcmp(argv[0], "setenv") == 0 && argc >= 3) {
                if (current_svc->env_count < MAX_ENV) {
                    char envbuf[256];
                    snprintf(envbuf, sizeof(envbuf), "%s=%s", argv[1], argv[2]);
                    current_svc->envp[current_svc->env_count++] = strdup(envbuf);
                }
            }
        }
    }

    fclose(fp);
}

static pid_t spawn_fallback_shell(void) {
    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid == 0) {
        setsid();
        int fd = open("/dev/console", O_RDWR);
        if (fd >= 0) {
            dup2(fd, STDIN_FILENO);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            if (fd > STDERR_FILENO) close(fd);
        }
        ioctl(STDIN_FILENO, TIOCSCTTY, 1);

        char *args[] = { "-sh", NULL };
        execv("/bin/sh", args);
        _exit(127);
    }
    return pid;
}

static void handle_signal(int sig) {
    switch (sig) {
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
    sa.sa_flags = SA_RESTART;

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

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

    parse_init_rc("/etc/init.rc");

    trigger_event("early-init");
    trigger_event("init");
    trigger_event("boot");

    class_start("default");

    NL_INFO("boot done");
    sleep(2);
    trigger_event("post-boot");

    spawn_fallback_shell();

    while (1) {
        int status;
        pid_t exited_pid = wait(&status);

        if (exited_pid < 0) {
            if (errno == EINTR) continue;
            if (errno == ECHILD) { pause(); continue; }
            sleep(1);
            continue;
        }

        for (size_t i = 0; i < service_count; i++) {
            if (services[i].pid == exited_pid) {
                NL_WARN("Service '%s' (PID: %d) exited", services[i].name, exited_pid);
                services[i].pid = 0;

                if (!services[i].is_oneshot && !services[i].is_disabled) {
                    if (!services[i].is_critical) {
                        sleep(1);
                    }
                    start_service(&services[i]);
                }
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
