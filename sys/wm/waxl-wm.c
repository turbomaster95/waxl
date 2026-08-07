/*
 * waxl-wm: Simple Window Manager
 *
 * Connects to waxl-comp via Unix socket.
 * Manages window tree, focus, and basic decorations.
 * Uses libinder for IPC with clients (or direct socket for test-wm).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "libinder.h"

#define WAXL_COMP_SOCK  "/tmp/waxl-comp.sock"
#define WAXL_WM_SOCK    "/tmp/waxl-wm.sock"
#define MAX_WINDOWS     64
#define MAX_CLIENTS     16
#define MAX_EVENTS      64

// ---- Protocol with compositor ----

typedef enum {
    WAXL_MSG_CREATE_SURFACE = 1,
    WAXL_MSG_DESTROY_SURFACE,
    WAXL_MSG_UPDATE_SURFACE,
    WAXL_MSG_MOVE_SURFACE,
    WAXL_MSG_SHOW_SURFACE,
    WAXL_MSG_HIDE_SURFACE,
    WAXL_MSG_SET_BG,
    WAXL_MSG_COMPOSITE,
    WAXL_MSG_QUIT,
} waxl_msg_type_t;

typedef struct {
    uint32_t type;
    uint32_t surface_id;
    int32_t x, y;
    int32_t width, height;
    uint32_t color;
} waxl_msg_t;

// ---- Window State ----

typedef struct {
    int id;
    int surface_id;       // compositor surface ID
    int x, y;
    int width, height;
    char title[256];
    bool visible;
    bool focused;
    bool decorated;
    uint32_t border_color;
    uint32_t titlebar_color;
    int client_fd;        // connected client
} window_t;

typedef struct {
    int comp_fd;          // connection to compositor
    int wm_fd;            // server socket for clients
    int epoll_fd;

    window_t windows[MAX_WINDOWS];
    int window_count;
    int next_win_id;

    int focused_id;
    int screen_w, screen_h;

    bool running;
} wm_state_t;

static wm_state_t g_wm = {0};

// ---- Compositor Communication ----

static int connect_to_comp(void) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_COMP_SOCK, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

static int comp_send(wm_state_t *wm, waxl_msg_t *msg) {
    if (wm->comp_fd < 0) return -1;
    ssize_t n = send(wm->comp_fd, msg, sizeof(*msg), 0);
    return (n == sizeof(*msg)) ? 0 : -1;
}

static int comp_recv_int(wm_state_t *wm) {
    int val;
    ssize_t n = recv(wm->comp_fd, &val, sizeof(val), 0);
    return (n == sizeof(val)) ? val : -1;
}

static int comp_create_surface(wm_state_t *wm, int x, int y, int w, int h) {
    waxl_msg_t msg = {
        .type = WAXL_MSG_CREATE_SURFACE,
        .x = x, .y = y,
        .width = w, .height = h
    };
    if (comp_send(wm, &msg) < 0) return -1;
    return comp_recv_int(wm);
}

static int comp_destroy_surface(wm_state_t *wm, int surface_id) {
    waxl_msg_t msg = {
        .type = WAXL_MSG_DESTROY_SURFACE,
        .surface_id = surface_id
    };
    return comp_send(wm, &msg);
}

static int comp_update_surface(wm_state_t *wm, int surface_id, uint32_t *pixels, size_t size) {
    waxl_msg_t msg = {
        .type = WAXL_MSG_UPDATE_SURFACE,
        .surface_id = surface_id
    };
    if (comp_send(wm, &msg) < 0) return -1;

    size_t total = 0;
    while (total < size) {
        ssize_t n = send(wm->comp_fd, (char*)pixels + total, size - total, 0);
        if (n < 0) {
            if (errno == EAGAIN || errno == EINTR) continue;
            return -1;
        }
        total += n;
    }
    return 0;
}

static int comp_move_surface(wm_state_t *wm, int surface_id, int x, int y) {
    waxl_msg_t msg = {
        .type = WAXL_MSG_MOVE_SURFACE,
        .surface_id = surface_id,
        .x = x, .y = y
    };
    return comp_send(wm, &msg);
}

static int comp_show_surface(wm_state_t *wm, int surface_id) {
    waxl_msg_t msg = {.type = WAXL_MSG_SHOW_SURFACE, .surface_id = surface_id};
    return comp_send(wm, &msg);
}

static int comp_hide_surface(wm_state_t *wm, int surface_id) {
    waxl_msg_t msg = {.type = WAXL_MSG_HIDE_SURFACE, .surface_id = surface_id};
    return comp_send(wm, &msg);
}

static int comp_composite(wm_state_t *wm) {
    waxl_msg_t msg = {.type = WAXL_MSG_COMPOSITE};
    return comp_send(wm, &msg);
}

static int comp_set_bg(wm_state_t *wm, uint32_t color) {
    waxl_msg_t msg = {.type = WAXL_MSG_SET_BG, .color = color};
    return comp_send(wm, &msg);
}

// ---- Window Management ----

static window_t *find_window(wm_state_t *wm, int id) {
    for (int i = 0; i < wm->window_count; i++) {
        if (wm->windows[i].id == id)
            return &wm->windows[i];
    }
    return NULL;
}

static window_t *find_window_by_surface(wm_state_t *wm, int surface_id) {
    for (int i = 0; i < wm->window_count; i++) {
        if (wm->windows[i].surface_id == surface_id)
            return &wm->windows[i];
    }
    return NULL;
}

static window_t *alloc_window(wm_state_t *wm) {
    if (wm->window_count >= MAX_WINDOWS) return NULL;
    window_t *w = &wm->windows[wm->window_count++];
    memset(w, 0, sizeof(*w));
    w->id = wm->next_win_id++;
    w->decorated = true;
    w->border_color = 0xFF4a90d9;
    w->titlebar_color = 0xFF2c3e50;
    w->client_fd = -1;
    return w;
}

static void remove_window(wm_state_t *wm, int id) {
    for (int i = 0; i < wm->window_count; i++) {
        if (wm->windows[i].id == id) {
            comp_destroy_surface(wm, wm->windows[i].surface_id);
            if (wm->windows[i].client_fd >= 0)
                close(wm->windows[i].client_fd);

            // Shift remaining
            for (int j = i; j < wm->window_count - 1; j++) {
                wm->windows[j] = wm->windows[j + 1];
            }
            wm->window_count--;

            if (wm->focused_id == id) {
                wm->focused_id = (wm->window_count > 0) ? wm->windows[wm->window_count - 1].id : -1;
            }
            break;
        }
    }
}

static void focus_window(wm_state_t *wm, int id) {
    for (int i = 0; i < wm->window_count; i++) {
        wm->windows[i].focused = (wm->windows[i].id == id);
        if (wm->windows[i].focused) {
            wm->windows[i].border_color = 0xFF4a90d9; // Blue focus
        } else {
            wm->windows[i].border_color = 0xFF555555; // Gray unfocus
        }
    }
    wm->focused_id = id;
}

// Draw window decorations (title bar, border) into pixel buffer
static void render_decorations(window_t *win, uint32_t *pixels, int w, int h) {
    int title_h = 24;
    int border = 2;

    // Fill content area with light gray
    for (int y = title_h; y < h - border; y++) {
        for (int x = border; x < w - border; x++) {
            pixels[y * w + x] = 0xFFeeeeee;
        }
    }

    // Title bar
    for (int y = 0; y < title_h; y++) {
        for (int x = 0; x < w; x++) {
            pixels[y * w + x] = win->focused ? 0xFF4a90d9 : 0xFF2c3e50;
        }
    }

    // Border
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < border; x++) {
            pixels[y * w + x] = win->border_color;
            pixels[y * w + (w - 1 - x)] = win->border_color;
        }
    }
    for (int y = h - border; y < h; y++) {
        for (int x = 0; x < w; x++) {
            pixels[y * w + x] = win->border_color;
        }
    }

    // Simple "X" close button
    int bx = w - 20, by = 4;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if ((i == j) || (i == 15 - j)) {
                if (by + i < h && bx + j < w)
                    pixels[(by + i) * w + (bx + j)] = 0xFFffffff;
            }
        }
    }
}

static void update_window_surface(wm_state_t *wm, window_t *win) {
    if (!win->visible) return;

    uint32_t *pixels = calloc(win->width * win->height, sizeof(uint32_t));
    if (!pixels) return;

    if (win->decorated) {
        render_decorations(win, pixels, win->width, win->height);
    }

    comp_update_surface(wm, win->surface_id, pixels, 
                        win->width * win->height * sizeof(uint32_t));
    free(pixels);
}

// ---- Client Protocol ----

typedef enum {
    WM_CMD_CREATE_WINDOW = 1,
    WM_CMD_DESTROY_WINDOW,
    WM_CMD_MOVE_WINDOW,
    WM_CMD_RESIZE_WINDOW,
    WM_CMD_SET_TITLE,
    WM_CMD_SHOW_WINDOW,
    WM_CMD_HIDE_WINDOW,
    WM_CMD_UPDATE_CONTENT,
    WM_CMD_GET_EVENT,
    WM_CMD_QUIT,
} wm_cmd_t;

typedef struct {
    uint32_t cmd;
    int32_t win_id;
    int32_t x, y;
    int32_t w, h;
    char title[256];
} wm_cmd_msg_t;

typedef struct {
    int32_t status;
    int32_t win_id;
} wm_reply_t;

static int handle_wm_client(wm_state_t *wm, int client_fd) {
    wm_cmd_msg_t cmd;
    ssize_t n = recv(client_fd, &cmd, sizeof(cmd), 0);
    if (n <= 0) return -1;
    if (n != sizeof(cmd)) return 0;

    wm_reply_t reply = {.status = 0, .win_id = -1};

    switch (cmd.cmd) {
        case WM_CMD_CREATE_WINDOW: {
            window_t *win = alloc_window(wm);
            if (!win) {
                reply.status = -1;
                send(client_fd, &reply, sizeof(reply), 0);
                break;
            }

            win->x = cmd.x;
            win->y = cmd.y;
            win->width = cmd.w > 0 ? cmd.w : 400;
            win->height = cmd.h > 0 ? cmd.h : 300;
            strncpy(win->title, cmd.title, sizeof(win->title) - 1);
            win->visible = true;
            win->client_fd = client_fd;

            // Create compositor surface
            int surf = comp_create_surface(wm, win->x, win->y, win->width, win->height);
            if (surf < 0) {
                reply.status = -1;
                send(client_fd, &reply, sizeof(reply), 0);
                wm->window_count--;
                break;
            }

            win->surface_id = surf;
            update_window_surface(wm, win);
            focus_window(wm, win->id);
            comp_composite(wm);

            reply.status = 0;
            reply.win_id = win->id;
            send(client_fd, &reply, sizeof(reply), 0);

            printf("[wm] Created window %d (surface %d)\n", win->id, surf);
            break;
        }

        case WM_CMD_DESTROY_WINDOW: {
            remove_window(wm, cmd.win_id);
            comp_composite(wm);
            reply.status = 0;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_MOVE_WINDOW: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win) {
                win->x = cmd.x;
                win->y = cmd.y;
                comp_move_surface(wm, win->surface_id, win->x, win->y);
                comp_composite(wm);
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_RESIZE_WINDOW: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win) {
                // Destroy old surface, create new one
                comp_destroy_surface(wm, win->surface_id);
                win->width = cmd.w;
                win->height = cmd.h;
                int surf = comp_create_surface(wm, win->x, win->y, win->width, win->height);
                if (surf >= 0) {
                    win->surface_id = surf;
                    update_window_surface(wm, win);
                    comp_composite(wm);
                }
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_SET_TITLE: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win) {
                strncpy(win->title, cmd.title, sizeof(win->title) - 1);
                update_window_surface(wm, win);
                comp_composite(wm);
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_SHOW_WINDOW: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win) {
                win->visible = true;
                comp_show_surface(wm, win->surface_id);
                comp_composite(wm);
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_HIDE_WINDOW: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win) {
                win->visible = false;
                comp_hide_surface(wm, win->surface_id);
                comp_composite(wm);
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_UPDATE_CONTENT: {
            window_t *win = find_window(wm, cmd.win_id);
            if (win && win->visible) {
                size_t size = win->width * win->height * sizeof(uint32_t);
                uint32_t *pixels = calloc(win->width * win->height, sizeof(uint32_t));
                if (pixels) {
                    // Read pixel data from client
                    size_t total = 0;
                    while (total < size) {
                        ssize_t r = recv(client_fd, (char*)pixels + total, size - total, 0);
                        if (r <= 0) { free(pixels); return -1; }
                        total += r;
                    }
                    comp_update_surface(wm, win->surface_id, pixels, size);
                    free(pixels);
                    comp_composite(wm);
                }
            }
            reply.status = win ? 0 : -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        case WM_CMD_QUIT: {
            wm->running = false;
            reply.status = 0;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
        }

        default:
            reply.status = -1;
            send(client_fd, &reply, sizeof(reply), 0);
            break;
    }

    return 0;
}

// ---- Server Setup ----

static int setup_wm_server(void) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    unlink(WAXL_WM_SOCK);

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_WM_SOCK, sizeof(addr.sun_path) - 1);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, MAX_CLIENTS) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}

// ---- Main ----

static void sigint_handler(int sig) {
    (void)sig;
    g_wm.running = false;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    printf("[waxl-wm] Starting window manager...\n");

    // Connect to compositor
    g_wm.comp_fd = connect_to_comp();
    if (g_wm.comp_fd < 0) {
        fprintf(stderr, "Failed to connect to compositor at %s\n", WAXL_COMP_SOCK);
        fprintf(stderr, "Make sure waxl-comp is running first.\n");
        return 1;
    }

    printf("[waxl-wm] Connected to compositor\n");

    // Setup WM server for clients
    g_wm.wm_fd = setup_wm_server();
    if (g_wm.wm_fd < 0) {
        close(g_wm.comp_fd);
        return 1;
    }

    g_wm.epoll_fd = epoll_create1(0);
    if (g_wm.epoll_fd < 0) {
        perror("epoll_create1");
        close(g_wm.comp_fd);
        close(g_wm.wm_fd);
        return 1;
    }

    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = g_wm.wm_fd;
    epoll_ctl(g_wm.epoll_fd, EPOLL_CTL_ADD, g_wm.wm_fd, &ev);

    g_wm.running = true;
    g_wm.focused_id = -1;
    g_wm.next_win_id = 1;

    // Set nice background
    comp_set_bg(&g_wm, 0xFF1a1a2e);
    comp_composite(&g_wm);

    printf("[waxl-wm] Ready. Listening on %s\n", WAXL_WM_SOCK);

    struct epoll_event events[MAX_EVENTS];
    while (g_wm.running) {
        int nfds = epoll_wait(g_wm.epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == g_wm.wm_fd) {
                int client = accept(g_wm.wm_fd, NULL, NULL);
                if (client >= 0) {
                    int flags = fcntl(client, F_GETFL, 0);
                    fcntl(client, F_SETFL, flags | O_NONBLOCK);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = client;
                    epoll_ctl(g_wm.epoll_fd, EPOLL_CTL_ADD, client, &ev);
                    printf("[wm] Client connected (fd=%d)\n", client);
                }
            } else {
                if (handle_wm_client(&g_wm, events[i].data.fd) < 0) {
                    printf("[wm] Client disconnected (fd=%d)\n", events[i].data.fd);
                    epoll_ctl(g_wm.epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);

                    // Mark any windows owned by this client
                    for (int j = 0; j < g_wm.window_count; j++) {
                        if (g_wm.windows[j].client_fd == events[i].data.fd) {
                            g_wm.windows[j].client_fd = -1;
                        }
                    }
                }
            }
        }
    }

    printf("[waxl-wm] Shutting down...\n");

    // Cleanup all windows
    for (int i = 0; i < g_wm.window_count; i++) {
        comp_destroy_surface(&g_wm, g_wm.windows[i].surface_id);
    }

    // Tell compositor to quit
    waxl_msg_t quit_msg = {.type = WAXL_MSG_QUIT};
    comp_send(&g_wm, &quit_msg);

    close(g_wm.epoll_fd);
    close(g_wm.wm_fd);
    close(g_wm.comp_fd);
    unlink(WAXL_WM_SOCK);

    return 0;
}
