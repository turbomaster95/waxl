#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <logd.h>
#include <common.h>

#define DEV_NODE_PATH "/dev/waxl_log"
#define RING_BUF_CAP 1024
#define MAX_CLIENTS 16

typedef struct {
    log_entry_t entries[RING_BUF_CAP];
    size_t head;   // Write index
    size_t count;  // Current entry count
    pthread_mutex_t lock;
} ring_buffer_t;

static ring_buffer_t g_ring = {
    .head = 0,
    .count = 0,
    .lock = PTHREAD_MUTEX_INITIALIZER
};

static void ring_buffer_push(const log_entry_t *entry) {
    pthread_mutex_lock(&g_ring.lock);
    g_ring.entries[g_ring.head] = *entry;
    g_ring.head = (g_ring.head + 1) % RING_BUF_CAP;
    if (g_ring.count < RING_BUF_CAP) {
        g_ring.count++;
    }
    pthread_mutex_unlock(&g_ring.lock);
}

static int g_clients[MAX_CLIENTS];
static pthread_mutex_t g_clients_lock = PTHREAD_MUTEX_INITIALIZER;

static void init_clients(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        g_clients[i] = -1;
    }
}

static void broadcast_log(const log_entry_t *entry) {
    pthread_mutex_lock(&g_clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_clients[i] != -1) {
            ssize_t ret = write(g_clients[i], entry, sizeof(log_entry_t));
            if (ret <= 0) {
                close(g_clients[i]);
                g_clients[i] = -1;
            }
        }
    }
    pthread_mutex_unlock(&g_clients_lock);
}

static void *dev_node_worker(void *arg) {
    (void)arg;
    unlink(DEV_NODE_PATH);

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        NL_ERROR("logd: failed to create socket for " DEV_NODE_PATH);
        return NULL;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, DEV_NODE_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        NL_ERROR("logd: failed to bind " DEV_NODE_PATH);
        close(server_fd);
        return NULL;
    }

    chmod(DEV_NODE_PATH, 0666);
    listen(server_fd, 8);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;

        pthread_mutex_lock(&g_ring.lock);
        size_t start = (g_ring.count < RING_BUF_CAP) ? 0 : g_ring.head;
        for (size_t i = 0; i < g_ring.count; i++) {
            size_t idx = (start + i) % RING_BUF_CAP;
            if (write(client_fd, &g_ring.entries[idx], sizeof(log_entry_t)) <= 0) {
                break;
            }
        }
        pthread_mutex_unlock(&g_ring.lock);

        pthread_mutex_lock(&g_clients_lock);
        bool added = false;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (g_clients[i] == -1) {
                g_clients[i] = client_fd;
                added = true;
                break;
            }
        }
        pthread_mutex_unlock(&g_clients_lock);

        if (!added) {
            close(client_fd); // Client pool full
        }
    }

    close(server_fd);
    unlink(DEV_NODE_PATH);
    return NULL;
}

static mqd_t mq_fd = (mqd_t)-1;
static volatile bool running = true;

static void cleanup_and_exit(int sig) {
    (void)sig;
    running = false;
    unlink(DEV_NODE_PATH);
    if (mq_fd != (mqd_t)-1) {
        mq_close(mq_fd);
        mq_unlink(MQ_NAME);
    }
    _exit(0);
}

static void *kmsg_worker(void *arg) {
    (void)arg;
	    int kfd = open("/dev/kmsg", O_RDONLY | O_NONBLOCK);
    if (kfd < 0) return NULL;

    char kbuf[512];
    mqd_t write_mq = mq_open(MQ_NAME, O_WRONLY | O_NONBLOCK);
    if (write_mq == (mqd_t)-1) {
        close(kfd);
        return NULL;
    }

    while (running) {
        ssize_t n = read(kfd, kbuf, sizeof(kbuf) - 1);
        if (n <= 0) {
            usleep(100000);
            continue;
        }
        kbuf[n] = '\0';

        int raw_prio = LOGD_INFO;
        sscanf(kbuf, "%d,", &raw_prio);

        char *msg_ptr = strchr(kbuf, ';');
        if (!msg_ptr) continue;
        msg_ptr++;

        size_t len = strlen(msg_ptr);
        if (len > 0 && msg_ptr[len - 1] == '\n') {
            msg_ptr[len - 1] = '\0';
        }

        log_entry_t entry;
        memset(&entry, 0, sizeof(entry));
        entry.priority = raw_prio & 7;
        strncpy(entry.tag, "KERN", MAX_TAG_LEN - 1);
        strncpy(entry.message, msg_ptr, MAX_MSG_LEN - 1);

        mq_send(write_mq, (const char *)&entry, sizeof(entry), 0);
    }

    mq_close(write_mq);
    close(kfd);
    return NULL;
}

int main(void) {
    signal(SIGINT, cleanup_and_exit);
    signal(SIGTERM, cleanup_and_exit);

    init_clients();

    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 256,
        .mq_msgsize = sizeof(log_entry_t),
        .mq_curmsgs = 0
    };

    mq_unlink(MQ_NAME);
    mq_fd = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq_fd == (mqd_t)-1) {
        NL_ERROR("logd: mq_open failed");
        return EXIT_FAILURE;
    }

    pthread_t dev_thread;
    if (pthread_create(&dev_thread, NULL, dev_node_worker, NULL) == 0) {
        pthread_detach(dev_thread);
    }

    pthread_t kthread;
    if (pthread_create(&kthread, NULL, kmsg_worker, NULL) == 0) {
        pthread_detach(kthread);
    }

    log_entry_t entry;
    while (running) {
        ssize_t bytes_read = mq_receive(mq_fd, (char *)&entry, sizeof(entry), NULL);
        if (bytes_read < 0) {
            if (errno == EINTR) continue;
            break;
        }

        ring_buffer_push(&entry);

        broadcast_log(&entry);

        if (strcmp(entry.tag, "KERN") == 0) {
            continue;
        }

        const char *level_str = "INF";
        const char *color_code = "\033[32m";

        switch (entry.priority) {
            case LOGD_EMERG:   level_str = "EMG"; color_code = "\033[1;31m"; break;
            case LOGD_ALERT:   level_str = "ALT"; color_code = "\033[1;35m"; break;
            case LOGD_CRIT:    level_str = "CRT"; color_code = "\033[1;31m"; break;
            case LOGD_ERR:     level_str = "ERR"; color_code = "\033[31m";   break;
            case LOGD_WARNING: level_str = "WRN"; color_code = "\033[33m";   break;
            case LOGD_NOTICE:  level_str = "NTC"; color_code = "\033[36m";   break;
            case LOGD_INFO:    level_str = "INF"; color_code = "\033[32m";   break;
            case LOGD_DEBUG:   level_str = "DBG"; color_code = "\033[90m";   break;
        }

	time_t raw;
        time(&raw);
        struct tm *timeinfo = localtime(&raw);
        char t_str[16];
        strftime(t_str, sizeof(t_str), "%H:%M:%S", timeinfo);

        printf("%s %s[%s]\033[0m \033[1m%s:\033[0m %s\n",
               t_str, color_code, level_str, entry.tag, entry.message);
        fflush(stdout);
    }

    cleanup_and_exit(0);
    return EXIT_SUCCESS;
}
