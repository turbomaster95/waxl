/*
 * waxl-comp: Dumb DRM Compositor
 * 
 * A simple software-rendered compositor that:
 * - Opens DRM via lidrm
 * - Receives surface buffers from waxl-wm via shared memory / unix sockets
 * - Composites windows onto the framebuffer
 * - Handles basic input via evdev (optional)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <linux/input.h>
#include <fcntl.h>
#include <poll.h>

#include "lidrm.h"

#define WAXL_SOCK_PATH  "/tmp/waxl-comp.sock"
#define MAX_SURFACES    32
#define MAX_EVENTS      64

typedef struct {
    int id;
    int x, y;
    int width, height;
    uint32_t *pixels;     // ARGB8888 pixel data
    size_t pixels_size;
    bool visible;
    bool dirty;
} surface_t;

typedef struct {
    lidrm_t drm;
    surface_t surfaces[MAX_SURFACES];
    int surface_count;
    int sock_fd;
    int epoll_fd;
    bool running;
    uint32_t bg_color;
} compositor_t;

static compositor_t g_comp = {0};

// ---- Pixel Drawing ----

static inline void put_pixel(compositor_t *c, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= (int)c->drm.width || y >= (int)c->drm.height)
        return;
    c->drm.pixels[y * (c->drm.pitch / 4) + x] = color;
}

static inline uint32_t blend_pixel(uint32_t dst, uint32_t src) {
    uint8_t sa = (src >> 24) & 0xFF;
    if (sa == 0) return dst;
    if (sa == 255) return src;

    uint8_t sr = (src >> 16) & 0xFF;
    uint8_t sg = (src >> 8) & 0xFF;
    uint8_t sb = src & 0xFF;

    uint8_t dr = (dst >> 16) & 0xFF;
    uint8_t dg = (dst >> 8) & 0xFF;
    uint8_t db = dst & 0xFF;

    uint8_t r = (sr * sa + dr * (255 - sa)) / 255;
    uint8_t g = (sg * sa + dg * (255 - sa)) / 255;
    uint8_t b = (sb * sa + db * (255 - sa)) / 255;

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

static void draw_rect(compositor_t *c, int x, int y, int w, int h, uint32_t color) {
    for (int row = y; row < y + h && row < (int)c->drm.height; row++) {
        for (int col = x; col < x + w && col < (int)c->drm.width; col++) {
            if (row >= 0 && col >= 0)
                put_pixel(c, col, row, color);
        }
    }
}

static void draw_surface(compositor_t *c, surface_t *surf) {
    if (!surf->visible || !surf->pixels) return;

    for (int row = 0; row < surf->height; row++) {
        int dy = surf->y + row;
        if (dy < 0 || dy >= (int)c->drm.height) continue;

        for (int col = 0; col < surf->width; col++) {
            int dx = surf->x + col;
            if (dx < 0 || dx >= (int)c->drm.width) continue;

            uint32_t src = surf->pixels[row * surf->width + col];
            uint32_t *dst = &c->drm.pixels[dy * (c->drm.pitch / 4) + dx];
            *dst = blend_pixel(*dst, src);
        }
    }
}

static void composite(compositor_t *c) {
    // Clear background
    lidrm_clear(&c->drm, c->bg_color);

    // Draw all visible surfaces (simple painter's algorithm)
    for (int i = 0; i < c->surface_count; i++) {
        if (c->surfaces[i].visible) {
            draw_surface(c, &c->surfaces[i]);
        }
    }

    lidrm_flush(&c->drm);
}

// ---- Surface Management ----

static surface_t *find_surface(compositor_t *c, int id) {
    for (int i = 0; i < c->surface_count; i++) {
        if (c->surfaces[i].id == id)
            return &c->surfaces[i];
    }
    return NULL;
}

static surface_t *alloc_surface(compositor_t *c) {
    if (c->surface_count >= MAX_SURFACES) return NULL;
    surface_t *s = &c->surfaces[c->surface_count++];
    memset(s, 0, sizeof(*s));
    s->id = c->surface_count; // simple ID assignment
    return s;
}

static void free_surface(surface_t *s) {
    if (s->pixels) {
        free(s->pixels);
        s->pixels = NULL;
    }
    s->visible = false;
}

// ---- Protocol ----

typedef enum {
    WAXL_MSG_CREATE_SURFACE = 1,
    WAXL_MSG_DESTROY_SURFACE,
    WAXL_MSG_UPDATE_SURFACE,
    WAXL_MSG_MOVE_SURFACE,
    WAXL_MSG_SHOW_SURFACE,
    WAXL_MSG_HIDE_SURFACE,
    WAXL_MSG_SET_BG,
    WAXL_MSG_COMPOSITE,   // trigger composite
    WAXL_MSG_QUIT,
} waxl_msg_type_t;

typedef struct {
    uint32_t type;
    uint32_t surface_id;
    int32_t x, y;
    int32_t width, height;
    uint32_t color;
} waxl_msg_t;

static int handle_client(compositor_t *c, int client_fd) {
    waxl_msg_t msg;
    ssize_t n = recv(client_fd, &msg, sizeof(msg), 0);
    if (n <= 0) return -1;
    if (n != sizeof(msg)) return 0;

    switch (msg.type) {
        case WAXL_MSG_CREATE_SURFACE: {
            surface_t *s = alloc_surface(c);
            if (!s) {
                int err = -1;
                send(client_fd, &err, sizeof(err), 0);
                break;
            }
            s->x = msg.x;
            s->y = msg.y;
            s->width = msg.width;
            s->height = msg.height;
            s->visible = true;
            s->pixels_size = msg.width * msg.height * sizeof(uint32_t);
            s->pixels = calloc(1, s->pixels_size);
            send(client_fd, &s->id, sizeof(s->id), 0);
            printf("[comp] Created surface %d (%dx%d @ %d,%d)\n", 
                   s->id, s->width, s->height, s->x, s->y);
            break;
        }

        case WAXL_MSG_DESTROY_SURFACE: {
            surface_t *s = find_surface(c, msg.surface_id);
            if (s) free_surface(s);
            break;
        }

        case WAXL_MSG_UPDATE_SURFACE: {
            surface_t *s = find_surface(c, msg.surface_id);
            if (s && s->pixels) {
                size_t to_read = s->width * s->height * sizeof(uint32_t);
                size_t total = 0;
                while (total < to_read) {
                    ssize_t r = recv(client_fd, (char*)s->pixels + total, to_read - total, 0);
                    if (r <= 0) return -1;
                    total += r;
                }
                s->dirty = true;
            }
            break;
        }

        case WAXL_MSG_MOVE_SURFACE: {
            surface_t *s = find_surface(c, msg.surface_id);
            if (s) {
                s->x = msg.x;
                s->y = msg.y;
            }
            break;
        }

        case WAXL_MSG_SHOW_SURFACE: {
            surface_t *s = find_surface(c, msg.surface_id);
            if (s) s->visible = true;
            break;
        }

        case WAXL_MSG_HIDE_SURFACE: {
            surface_t *s = find_surface(c, msg.surface_id);
            if (s) s->visible = false;
            break;
        }

        case WAXL_MSG_SET_BG: {
            c->bg_color = msg.color;
            break;
        }

        case WAXL_MSG_COMPOSITE: {
            composite(c);
            break;
        }

        case WAXL_MSG_QUIT: {
            c->running = false;
            break;
        }
    }

    return 0;
}

// ---- Server Setup ----

static int setup_server(void) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    unlink(WAXL_SOCK_PATH);

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, 4) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}

// ---- Main ----

static void sigint_handler(int sig) {
    (void)sig;
    g_comp.running = false;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    printf("[waxl-comp] Starting compositor...\n");

    // Init DRM
    const char *drm_path = getenv("WAXL_DRM_DEVICE");
    if (!lidrm_init(&g_comp.drm, drm_path)) {
        fprintf(stderr, "Failed to init DRM\n");
        return 1;
    }

    if (!lidrm_set_mode(&g_comp.drm)) {
        fprintf(stderr, "Failed to set DRM mode\n");
        lidrm_cleanup(&g_comp.drm);
        return 1;
    }

    printf("[waxl-comp] DRM: %dx%d\n", g_comp.drm.width, g_comp.drm.height);

    // Init server socket
    g_comp.sock_fd = setup_server();
    if (g_comp.sock_fd < 0) {
        lidrm_cleanup(&g_comp.drm);
        return 1;
    }

    g_comp.epoll_fd = epoll_create1(0);
    if (g_comp.epoll_fd < 0) {
        perror("epoll_create1");
        close(g_comp.sock_fd);
        lidrm_cleanup(&g_comp.drm);
        return 1;
    }

    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = g_comp.sock_fd;
    epoll_ctl(g_comp.epoll_fd, EPOLL_CTL_ADD, g_comp.sock_fd, &ev);

    g_comp.running = true;
    g_comp.bg_color = 0xFF1a1a2e; // Dark blue background

    // Initial clear
    lidrm_clear(&g_comp.drm, g_comp.bg_color);
    lidrm_flush(&g_comp.drm);

    printf("[waxl-comp] Ready. Listening on %s\n", WAXL_SOCK_PATH);

    struct epoll_event events[MAX_EVENTS];
    while (g_comp.running) {
        int nfds = epoll_wait(g_comp.epoll_fd, events, MAX_EVENTS, 16); // 16ms ~ 60fps

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == g_comp.sock_fd) {
                int client = accept(g_comp.sock_fd, NULL, NULL);
                if (client >= 0) {
                    int flags = fcntl(client, F_GETFL, 0);
                    fcntl(client, F_SETFL, flags | O_NONBLOCK);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = client;
                    epoll_ctl(g_comp.epoll_fd, EPOLL_CTL_ADD, client, &ev);
                }
            } else {
                if (handle_client(&g_comp, events[i].data.fd) < 0) {
                    epoll_ctl(g_comp.epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
            }
        }

        // Periodic composite for animations (optional)
        // composite(&g_comp);
    }

    printf("[waxl-comp] Shutting down...\n");

    close(g_comp.epoll_fd);
    close(g_comp.sock_fd);
    unlink(WAXL_SOCK_PATH);

    for (int i = 0; i < g_comp.surface_count; i++) {
        free_surface(&g_comp.surfaces[i]);
    }

    lidrm_cleanup(&g_comp.drm);
    return 0;
}
