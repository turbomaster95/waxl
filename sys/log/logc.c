#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <logd.h>
#include <common.h>

#define DEV_NODE_PATH "/dev/waxl_log"

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-p priority] [-t tag] [-h]\n", prog);
    fprintf(stderr, "  -p <priority>  Max priority level to show (0: EMG to 7: DBG, default: 7)\n");
    fprintf(stderr, "  -t <tag>       Filter logs by specific tag (e.g., KERN, worch)\n");
    fprintf(stderr, "  -h             Show this help message\n");
}

static const char *get_level_str(int priority) {
    switch (priority) {
        case LOGD_EMERG:   return "EMG";
        case LOGD_ALERT:   return "ALT";
        case LOGD_CRIT:    return "CRT";
        case LOGD_ERR:     return "ERR";
        case LOGD_WARNING: return "WRN";
        case LOGD_NOTICE:  return "NTC";
        case LOGD_INFO:    return "INF";
        case LOGD_DEBUG:   return "DBG";
        default:           return "UNK";
    }
}

static const char *get_level_color(int priority) {
    switch (priority) {
        case LOGD_EMERG:   return "\033[1;31m"; // Bold Red
        case LOGD_ALERT:   return "\033[1;35m"; // Bold Magenta
        case LOGD_CRIT:    return "\033[1;31m"; // Bold Red
        case LOGD_ERR:     return "\033[31m";   // Red
        case LOGD_WARNING: return "\033[33m";   // Yellow
        case LOGD_NOTICE:  return "\033[36m";   // Cyan
        case LOGD_INFO:    return "\033[32m";   // Green
        case LOGD_DEBUG:   return "\033[90m";   // Gray
        default:           return "\033[0m";
    }
}

int main(int argc, char *argv[]) {
    int max_priority = LOGD_DEBUG;
    char filter_tag[MAX_TAG_LEN] = {0};

    int opt;
    while ((opt = getopt(argc, argv, "p:t:h")) != -1) {
        switch (opt) {
            case 'p':
                max_priority = atoi(optarg);
                if (max_priority < 0 || max_priority > 7) {
                    max_priority = LOGD_DEBUG;
                }
                break;
            case 't':
                strncpy(filter_tag, optarg, MAX_TAG_LEN - 1);
                filter_tag[MAX_TAG_LEN - 1] = '\0';
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("logc: socket creation failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, DEV_NODE_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("logc: connect to " DEV_NODE_PATH " failed");
        close(fd);
        return EXIT_FAILURE;
    }

    log_entry_t entry;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, &entry, sizeof(entry))) > 0) {
        if (bytes_read < (ssize_t)sizeof(entry)) {
            continue;
        }

        if (entry.priority > max_priority) {
            continue;
        }

        if (filter_tag[0] != '\0' && strcmp(entry.tag, filter_tag) != 0) {
            continue;
        }

        const char *color = get_level_color(entry.priority);
        const char *level = get_level_str(entry.priority);

        printf("%s[%s]\033[0m \033[1m%s:\033[0m %s\n",
               color, level, entry.tag, entry.message);
        fflush(stdout);
    }

    close(fd);
    return EXIT_SUCCESS;
}
