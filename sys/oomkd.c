#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <common.h>

#ifndef MCL_CURRENT
#define MCL_CURRENT 1
#endif
#ifndef MCL_FUTURE
#define MCL_FUTURE 2
#endif

#ifndef __NR_pidfd_open
#define __NR_pidfd_open 434
#endif
#ifndef __NR_pidfd_send_signal
#define __NR_pidfd_send_signal 424
#endif

#define PSI_PATH "/proc/pressure/memory"
#define MAX_EVENTS 4

/* PSI: 5ms of memory stall within a 50ms window */
#define PSI_TRIGGER_SOME "some 5000 50000"
#define NORMAL_POLL_MS 10
#define FAST_POLL_MS   1
#define MIN_AVAILABLE_KB 12288 // 12MB threshold

static int is_numeric_dir(const char *name) {
    while (*name) {
        if (!isdigit((unsigned char)*name)) return 0;
        name++;
    }
    return 1;
}

static int read_proc_int(const char *path, int default_val) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return default_val;
    char buf[32];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) return default_val;
    buf[n] = '\0';
    return atoi(buf);
}

static int get_task_metrics(pid_t pid, long *out_rss_anon, int *out_is_kthread) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;

    char buf[4096];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (n <= 0) return -1;
    buf[n] = '\0';

    // Check if it's a kernel thread
    *out_is_kthread = (strstr(buf, "Kthread:\t1") != NULL);

    char *rss_line = strstr(buf, "RssAnon:");
    if (rss_line) {
        if (sscanf(rss_line, "RssAnon: %ld", out_rss_anon) != 1) {
            *out_rss_anon = 0;
        }
    } else {
        *out_rss_anon = 0;
    }

    return 0;
}

static int check_meminfo_pressure(int *out_avail_kb) {
    int fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0) return 0;

    char buf[1024];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (n <= 0) return 0;
    buf[n] = '\0';

    char *line = strstr(buf, "MemAvailable:");
    if (!line) line = strstr(buf, "MemFree:");

    if (line) {
        int avail_kb = 0;
        if (sscanf(line, "%*s %d", &avail_kb) == 1) {
            if (out_avail_kb) *out_avail_kb = avail_kb;
            if (avail_kb < MIN_AVAILABLE_KB) return 1;
        }
    }
    return 0;
}

void lock_daemon_resources(void) {
    if (mlockall(MCL_CURRENT | MCL_FUTURE) < 0) {
        perror("mlockall failed");
    }

    // Protect oomkd from the kernel's oom-killer
    int fd = open("/proc/self/oom_score_adj", O_WRONLY);
    if (fd >= 0) {
        write(fd, "-1000", 5);
        close(fd);
    }

    struct sched_param param = { .sched_priority = 99 };
    if (sched_setscheduler(0, SCHED_FIFO, &param) < 0) {
        setpriority(PRIO_PROCESS, 0, -20);
    }
}

static pid_t find_expendable_target(long *out_target_rss) {
    DIR *dir = opendir("/proc");
    if (!dir) return -1;

    struct dirent *entry;
    pid_t target_pid = -1;
    long max_rss_anon = 0;
    pid_t self_pid = getpid();

    while ((entry = readdir(dir)) != NULL) {
        if (!is_numeric_dir(entry->d_name)) continue;

        pid_t pid = (pid_t)atoi(entry->d_name);
        if (pid <= 2 || pid == self_pid) continue;

        char score_path[64];
        snprintf(score_path, sizeof(score_path), "/proc/%d/oom_score_adj", pid);
        if (read_proc_int(score_path, 0) <= -1000) continue;

        long rss_anon = 0;
        int is_kthread = 0;
        if (get_task_metrics(pid, &rss_anon, &is_kthread) < 0) continue;

        if (is_kthread) continue;

        if (rss_anon > max_rss_anon) {
            max_rss_anon = rss_anon;
            target_pid = pid;
        }
    }

    closedir(dir);
    *out_target_rss = max_rss_anon;
    return target_pid; // Can return -1 if nothing is found
}

static void execute_kill(const char *reason) {
    long target_rss_anon = 0;
    pid_t target = find_expendable_target(&target_rss_anon);

    if (target > 0 && target_rss_anon > 0) {
        NL_INFO("[%s] Hit! Target PID %d (RssAnon: %ld kB)",
               reason, target, target_rss_anon);
	NL_INFO("Target Down!");

        int pidfd = syscall(__NR_pidfd_open, target, 0);
        if (pidfd >= 0) {
            if (syscall(__NR_pidfd_send_signal, pidfd, SIGKILL, NULL, 0) == 0) {
                NL_INFO("Successfully sent SIGKILL via pidfd to PID %d", target);
            }
            close(pidfd);
        } else {
            if (kill(target, SIGKILL) == 0) {
                NL_INFO("Successfully sent SIGKILL via kill() to PID %d", target);
            }
        }
    }
}

int main(void) {
    NL_INFO("Starting oomkd...");

    int psi_fd = open(PSI_PATH, O_RDWR | O_NONBLOCK);
    if (psi_fd >= 0) {
        if (write(psi_fd, PSI_TRIGGER_SOME, strlen(PSI_TRIGGER_SOME)) < 0) {
            NL_ERROR("PSI trigger write failed, using fallback..");
            close(psi_fd);
            psi_fd = -1;
        }
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) return EXIT_FAILURE;

    lock_daemon_resources();

    if (psi_fd >= 0) {
        struct epoll_event ev;
        ev.events = EPOLLPRI;
        ev.data.fd = psi_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, psi_fd, &ev);
    }

    struct epoll_event events[MAX_EVENTS];
    int timeout_ms = NORMAL_POLL_MS;

    NL_INFO("oomkd active: watching PSI right now :3");

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout_ms);

        if (nfds < 0) {
            if (errno == EINTR) continue;
            break;
        }

        int triggered = 0;

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == psi_fd && (events[i].events & EPOLLPRI)) {
                // MUST READ FROM PSI FD TO CLEAR THE EPOLL EVENT
                char dummy_buf[64];
                read(psi_fd, dummy_buf, sizeof(dummy_buf));
                
                execute_kill("oomkd");
                triggered = 1;
            }
        }

        int avail_kb = 0;
        int is_low = check_meminfo_pressure(&avail_kb);

        if (!triggered && is_low) {
            execute_kill("oomkd");
        }

        // Drop to hyper-polling only when within 2x of the critical threshold
        timeout_ms = (avail_kb < (MIN_AVAILABLE_KB * 2)) ? FAST_POLL_MS : NORMAL_POLL_MS;
    }

    if (psi_fd >= 0) close(psi_fd);
    close(epoll_fd);
    return EXIT_SUCCESS;
}
