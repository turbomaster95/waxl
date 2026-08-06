#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <lidrm.h>

#define WAXL_SOCKET_PATH "/tmp/waxl-comp.sock"
#define MAX_CLIENTS 16
#define MAX_SURFACES 32
#define TITLE_BAR_HEIGHT 24

typedef enum {
    WAXL_CMD_CREATE_SURFACE = 1,
    WAXL_CMD_COMMIT         = 2,
    WAXL_CMD_MOVE           = 3,
    WAXL_CMD_DESTROY        = 4
} waxl_cmd_type_t;

typedef struct {
    uint32_t type;        // waxl_cmd_type_t
    uint32_t surface_id;
    int32_t  x, y;
    uint32_t width, height;
} waxl_msg_t;

typedef struct surface {
    uint32_t id;
    int client_fd;
    int32_t x, y;
    uint32_t width, height;
    int shm_fd;
    uint32_t *pixels;
    size_t shm_size;
    bool active;
    bool needs_redraw;
    struct surface *next;
} surface_t;

static volatile bool running = true;
static surface_t *surface_list = NULL;
static uint32_t next_surface_id = 1;

static void handle_signal(int sig) {
    (void)sig;
    running = false;
}

static int send_fd(int socket_fd, int fd_to_send, uint32_t surface_id) {
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];
    memset(buf, 0, sizeof(buf));

    struct iovec io = { .iov_base = &surface_id, .iov_len = sizeof(surface_id) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(int));

    return (sendmsg(socket_fd, &msg, 0) < 0) ? -1 : 0;
}

static surface_t *create_surface(int client_fd, uint32_t width, uint32_t height, int32_t x, int32_t y) {
    surface_t *surf = calloc(1, sizeof(surface_t));
    if (!surf) return NULL;

    surf->id = next_surface_id++;
    surf->client_fd = client_fd;
    surf->width = width;
    surf->height = height;
    surf->x = x;
    surf->y = y;
    surf->shm_size = width * height * sizeof(uint32_t);

    char memfd_name[32];
    snprintf(memfd_name, sizeof(memfd_name), "waxl-surf-%u", surf->id);
    surf->shm_fd = memfd_create(memfd_name, MFD_CLOEXEC);
    if (surf->shm_fd < 0) {
        perror("[waxl-comp] memfd_create failed");
        free(surf);
        return NULL;
    }

    if (ftruncate(surf->shm_fd, surf->shm_size) < 0) {
        perror("[waxl-comp] ftruncate failed");
        close(surf->shm_fd);
        free(surf);
        return NULL;
    }

    surf->pixels = mmap(NULL, surf->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, surf->shm_fd, 0);
    if (surf->pixels == MAP_FAILED) {
        perror("[waxl-comp] mmap shm failed");
        close(surf->shm_fd);
        free(surf);
        return NULL;
    }

    memset(surf->pixels, 0x00, surf->shm_size); // Default transparent
    surf->active = true;

    surf->next = surface_list;
    surface_list = surf;

    if (send_fd(client_fd, surf->shm_fd, surf->id) < 0) {
        perror("[waxl-comp] Failed to pass SHM FD to client");
    }

    printf("[waxl-comp] Created Surface ID %u (%ux%u) for client FD %d\n", surf->id, width, height, client_fd);
    return surf;
}

static void destroy_surface(uint32_t surface_id) {
    surface_t **curr = &surface_list;
    while (*curr) {
        surface_t *entry = *curr;
        if (entry->id == surface_id) {
            *curr = entry->next;
            if (entry->pixels && entry->pixels != MAP_FAILED) {
                munmap(entry->pixels, entry->shm_size);
            }
            if (entry->shm_fd >= 0) close(entry->shm_fd);
            printf("[waxl-comp] Destroyed Surface ID %u\n", surface_id);
            free(entry);
            return;
        }
        curr = &entry->next;
    }
}

static void remove_client_surfaces(int client_fd) {
    surface_t **curr = &surface_list;
    while (*curr) {
        surface_t *entry = *curr;
        if (entry->client_fd == client_fd) {
            *curr = entry->next;
            if (entry->pixels && entry->pixels != MAP_FAILED) {
                munmap(entry->pixels, entry->shm_size);
            }
            if (entry->shm_fd >= 0) close(entry->shm_fd);
            printf("[waxl-comp] Cleaned up Surface ID %u for disconnected client FD %d\n", entry->id, client_fd);
            free(entry);
        } else {
            curr = &entry->next;
        }
    }
}

static inline uint32_t blend_pixel(uint32_t src, uint32_t dst) {
    uint32_t a = (src >> 24) & 0xFF;
    if (a == 255) return src;
    if (a == 0)   return dst;

    uint32_t sr = (src >> 16) & 0xFF, sg = (src >> 8) & 0xFF, sb = src & 0xFF;
    uint32_t dr = (dst >> 16) & 0xFF, dg = (dst >> 8) & 0xFF, db = dst & 0xFF;

    uint32_t r = (sr * a + dr * (255 - a)) / 255;
    uint32_t g = (sg * a + dg * (255 - a)) / 255;
    uint32_t b = (sb * a + db * (255 - a)) / 255;

    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

static void draw_rect(lidrm_t *drm, int rx, int ry, int rw, int rh, uint32_t color) {
    int x1 = rx < 0 ? 0 : rx;
    int y1 = ry < 0 ? 0 : ry;
    int x2 = (rx + rw > (int)drm->width) ? (int)drm->width : (rx + rw);
    int y2 = (ry + rh > (int)drm->height) ? (int)drm->height : (ry + rh);

    for (int y = y1; y < y2; y++) {
        uint32_t *row = drm->pixels + (y * (drm->pitch / 4));
        for (int x = x1; x < x2; x++) {
            row[x] = color;
        }
    }
}

static void composite_surface(lidrm_t *drm, surface_t *surf) {
    if (!surf->active || !surf->pixels) return;

    int win_x = surf->x;
    int win_y = surf->y;
    int win_w = surf->width;
    int win_h = surf->height;

    draw_rect(drm, win_x - 2, win_y - TITLE_BAR_HEIGHT - 2, win_w + 4, win_h + TITLE_BAR_HEIGHT + 4, 0xFF3C3C3C);
    draw_rect(drm, win_x, win_y - TITLE_BAR_HEIGHT, win_w, TITLE_BAR_HEIGHT, 0xFF0055AA);

    int start_x = win_x < 0 ? 0 : win_x;
    int start_y = win_y < 0 ? 0 : win_y;
    int end_x = (win_x + win_w > (int)drm->width) ? (int)drm->width : (win_x + win_w);
    int end_y = (win_y + win_h > (int)drm->height) ? (int)drm->height : (win_y + win_h);

    for (int y = start_y; y < end_y; y++) {
        uint32_t *dst_row = drm->pixels + (y * (drm->pitch / 4));
        uint32_t *src_row = surf->pixels + ((y - win_y) * surf->width);

        for (int x = start_x; x < end_x; x++) {
            uint32_t src_pixel = src_row[x - win_x];
            dst_row[x] = blend_pixel(src_pixel, dst_row[x]);
        }
    }
}

int main(int argc, char *argv[]) {
    const char *dev_path = (argc > 1) ? argv[1] : "/dev/dri/card0";
    lidrm_t drm = {0};

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("[waxl-comp] Initializing DRM on %s...\n", dev_path);
    if (!lidrm_init(&drm, dev_path)) {
        fprintf(stderr, "[waxl-comp] Failed to initialize DRM!\n");
        return EXIT_FAILURE;
    }

    if (!lidrm_set_mode(&drm)) {
        fprintf(stderr, "[waxl-comp] Failed to apply CRTC mode!\n");
        lidrm_cleanup(&drm);
        return EXIT_FAILURE;
    }

    unlink(WAXL_SOCKET_PATH);
    int server_fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server_fd < 0) {
        perror("[waxl-comp] Socket creation failed");
        lidrm_cleanup(&drm);
        return EXIT_FAILURE;
    }

    struct sockaddr_un sun = { .sun_family = AF_UNIX };
    strncpy(sun.sun_path, WAXL_SOCKET_PATH, sizeof(sun.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&sun, sizeof(sun)) < 0) {
        perror("[waxl-comp] Socket bind failed");
        close(server_fd);
        lidrm_cleanup(&drm);
        return EXIT_FAILURE;
    }

    listen(server_fd, MAX_CLIENTS);
    printf("[waxl-comp] SurfaceFlinger listening on %s\n", WAXL_SOCKET_PATH);

    struct pollf_fds {
        struct pollfd fds[MAX_CLIENTS + 1];
        int count;
    } pfds = {0};

    pfds.fds[0].fd = server_fd;
    pfds.fds[0].events = POLLIN;
    pfds.count = 1;

    struct timespec target_time;
    clock_gettime(CLOCK_MONOTONIC, &target_time);

    while (running) {
        int poll_res = poll(pfds.fds, pfds.count, 0);
        if (poll_res > 0) {
            if (pfds.fds[0].revents & POLLIN) {
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd >= 0 && pfds.count < MAX_CLIENTS + 1) {
                    fcntl(client_fd, F_SETFL, O_NONBLOCK);
                    pfds.fds[pfds.count].fd = client_fd;
                    pfds.fds[pfds.count].events = POLLIN | POLLHUP;
                    pfds.count++;
                    printf("[waxl-comp] Client connected on FD %d\n", client_fd);
                }
            }

            for (int i = 1; i < pfds.count; i++) {
                if (pfds.fds[i].revents & (POLLHUP | POLLERR)) {
                    printf("[waxl-comp] Client FD %d disconnected\n", pfds.fds[i].fd);
                    remove_client_surfaces(pfds.fds[i].fd);
                    close(pfds.fds[i].fd);
                    pfds.fds[i] = pfds.fds[pfds.count - 1];
                    pfds.count--;
                    i--;
                    continue;
                }

                if (pfds.fds[i].revents & POLLIN) {
                    waxl_msg_t msg;
                    ssize_t bytes = read(pfds.fds[i].fd, &msg, sizeof(msg));
                    if (bytes == sizeof(msg)) {
                        switch (msg.type) {
                            case WAXL_CMD_CREATE_SURFACE:
                                create_surface(pfds.fds[i].fd, msg.width, msg.height, msg.x, msg.y);
                                break;
                            case WAXL_CMD_MOVE: {
                                surface_t *s = surface_list;
                                while (s) {
                                    if (s->id == msg.surface_id) {
                                        s->x = msg.x;
                                        s->y = msg.y;
                                        break;
                                    }
                                    s = s->next;
                                }
                                break;
                            }
                            case WAXL_CMD_DESTROY:
                                destroy_surface(msg.surface_id);
                                break;
                            case WAXL_CMD_COMMIT:
                                break;
                        }
                    }
                }
            }
        }

        lidrm_clear(&drm, 0xFF121212); // Background

        draw_rect(&drm, 0, 0, drm.width, 32, 0xFF282828);
        draw_rect(&drm, 0, 31, drm.width, 1, 0xFF3C3C3C);

        surface_t *curr = surface_list;
        while (curr) {
            composite_surface(&drm, curr);
            curr = curr->next;
        }

        lidrm_set_mode(&drm);

        target_time.tv_nsec += 16666666;
        if (target_time.tv_nsec >= 1000000000) {
            target_time.tv_sec += 1;
            target_time.tv_nsec -= 1000000000;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &target_time, NULL);
    }

    printf("\n[waxl-comp] Shutting down compositor...\n");
    for (int i = 1; i < pfds.count; i++) close(pfds.fds[i].fd);
    close(server_fd);
    unlink(WAXL_SOCKET_PATH);
    lidrm_cleanup(&drm);
    return EXIT_SUCCESS;
}
