#include <waxlc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>

#define WAXL_SOCKET_PATH  "/tmp/waxl-comp.sock"
#define WAXL_MAX_CLIENTS  64
#define WAXL_MAX_WORKSPACES 9

/* Protocol: compositor <-> WM communication */
typedef enum {
    WAXL_MSG_CREATE_WINDOW = 1,
    WAXL_MSG_DESTROY_WINDOW,
    WAXL_MSG_MOVE_WINDOW,
    WAXL_MSG_RESIZE_WINDOW,
    WAXL_MSG_RAISE_WINDOW,
    WAXL_MSG_GET_FB,
    WAXL_MSG_PRESENT,
    WAXL_MSG_INPUT,
    WAXL_MSG_SET_TITLE,
    WAXL_MSG_QUIT,
} waxl_msg_type_t;

typedef struct {
    uint32_t type;
    uint32_t win_id;
    int32_t  x, y;
    uint32_t w, h;
    uint32_t color;
    char     title[64];
} waxl_msg_t;

typedef enum {
    WAXL_LAYOUT_FLOATING = 0,
    WAXL_LAYOUT_TILING,
    WAXL_LAYOUT_FULLSCREEN,
} waxl_layout_t;

typedef struct {
    uint32_t id;
    int32_t  x, y;
    uint32_t w, h;
    uint32_t color;
    bool     visible;
    bool     floating;
    bool     fullscreen;
    char     title[64];
    uint32_t workspace;
} waxl_wm_client_t;

typedef struct {
    uint32_t id;
    waxl_wm_client_t *clients[WAXL_MAX_CLIENTS];
    uint32_t client_count;
    waxl_layout_t layout;
    bool     active;
} waxl_workspace_t;

typedef struct {
    int      sock_fd;
    int      running;
    uint32_t next_win_id;
    uint32_t screen_w, screen_h;

    waxl_workspace_t workspaces[WAXL_MAX_WORKSPACES];
    uint32_t active_workspace;
    waxl_wm_client_t clients[WAXL_MAX_CLIENTS];
    uint32_t client_count;
    uint32_t focused_client;

    bool mod_pressed;
    pthread_mutex_t lock;
} waxl_wm_t;

static waxl_wm_t g_wm = {0};

/* === Compositor communication === */

static int waxl_connect_comp(void) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static bool waxl_send_msg(waxl_wm_t *wm, waxl_msg_t *msg) {
    if (wm->sock_fd < 0) return false;
    ssize_t n = send(wm->sock_fd, msg, sizeof(*msg), 0);
    return n == sizeof(*msg);
}

static bool waxl_send_create(waxl_wm_t *wm, uint32_t id, int x, int y, uint32_t w, uint32_t h, uint32_t color, const char *title) {
    waxl_msg_t msg = {
        .type = WAXL_MSG_CREATE_WINDOW,
        .win_id = id,
        .x = x, .y = y,
        .w = w, .h = h,
        .color = color,
    };
    strncpy(msg.title, title ? title : "Untitled", sizeof(msg.title) - 1);
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_destroy(waxl_wm_t *wm, uint32_t id) {
    waxl_msg_t msg = { .type = WAXL_MSG_DESTROY_WINDOW, .win_id = id };
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_move(waxl_wm_t *wm, uint32_t id, int x, int y) {
    waxl_msg_t msg = { .type = WAXL_MSG_MOVE_WINDOW, .win_id = id, .x = x, .y = y };
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_resize(waxl_wm_t *wm, uint32_t id, uint32_t w, uint32_t h) {
    waxl_msg_t msg = { .type = WAXL_MSG_RESIZE_WINDOW, .win_id = id, .w = w, .h = h };
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_raise(waxl_wm_t *wm, uint32_t id) {
    waxl_msg_t msg = { .type = WAXL_MSG_RAISE_WINDOW, .win_id = id };
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_title(waxl_wm_t *wm, uint32_t id, const char *title) {
    waxl_msg_t msg = { .type = WAXL_MSG_SET_TITLE, .win_id = id };
    strncpy(msg.title, title, sizeof(msg.title) - 1);
    return waxl_send_msg(wm, &msg);
}

static bool waxl_send_quit(waxl_wm_t *wm) {
    waxl_msg_t msg = { .type = WAXL_MSG_QUIT };
    return waxl_send_msg(wm, &msg);
}

/* === Client management === */

static waxl_wm_client_t* waxl_find_client(waxl_wm_t *wm, uint32_t id) {
    for (uint32_t i = 0; i < wm->client_count; i++) {
        if (wm->clients[i].id == id) return &wm->clients[i];
    }
    return NULL;
}

static waxl_wm_client_t* waxl_new_client(waxl_wm_t *wm, const char *title, uint32_t w, uint32_t h, uint32_t color) {
    if (wm->client_count >= WAXL_MAX_CLIENTS) return NULL;
    waxl_wm_client_t *c = &wm->clients[wm->client_count++];
    memset(c, 0, sizeof(*c));
    c->id = wm->next_win_id++;
    c->w = w;
    c->h = h;
    c->color = color;
    c->visible = true;
    c->workspace = wm->active_workspace;
    snprintf(c->title, sizeof(c->title), "%s", title ? title : "Window");

    waxl_workspace_t *ws = &wm->workspaces[wm->active_workspace];
    if (ws->client_count < WAXL_MAX_CLIENTS) {
        ws->clients[ws->client_count++] = c;
    }

    wm->focused_client = c->id;
    return c;
}

static void waxl_remove_client(waxl_wm_t *wm, uint32_t id) {
    waxl_wm_client_t *c = waxl_find_client(wm, id);
    if (!c) return;

    waxl_workspace_t *ws = &wm->workspaces[c->workspace];
    for (uint32_t i = 0; i < ws->client_count; i++) {
        if (ws->clients[i] == c) {
            memmove(&ws->clients[i], &ws->clients[i+1], (ws->client_count - i - 1) * sizeof(void*));
            ws->client_count--;
            break;
        }
    }

    for (uint32_t i = 0; i < wm->client_count; i++) {
        if (wm->clients[i].id == id) {
            memmove(&wm->clients[i], &wm->clients[i+1], (wm->client_count - i - 1) * sizeof(waxl_wm_client_t));
            wm->client_count--;
            break;
        }
    }

    if (wm->focused_client == id) {
        wm->focused_client = wm->client_count > 0 ? wm->clients[wm->client_count - 1].id : 0;
    }
}

/* === Layout engines === */

static void waxl_apply_tiling(waxl_wm_t *wm, waxl_workspace_t *ws) {
    if (ws->client_count == 0) return;

    uint32_t mw = wm->screen_w / 2;
    uint32_t nmaster = 1;
    uint32_t n = ws->client_count;

    for (uint32_t i = 0; i < n; i++) {
        waxl_wm_client_t *c = ws->clients[i];
        if (!c || c->floating || c->fullscreen) continue;

        if (i < nmaster) {
            c->x = 0;
            c->y = 0;
            c->w = mw;
            c->h = wm->screen_h;
        } else {
            uint32_t stack_count = n - nmaster;
            uint32_t sh = wm->screen_h / stack_count;
            c->x = mw;
            c->y = (i - nmaster) * sh;
            c->w = wm->screen_w - mw;
            c->h = sh;
        }
        waxl_send_move(wm, c->id, c->x, c->y);
        waxl_send_resize(wm, c->id, c->w, c->h);
    }
}

static void waxl_arrange(waxl_wm_t *wm, waxl_workspace_t *ws) {
    switch (ws->layout) {
        case WAXL_LAYOUT_TILING:
            waxl_apply_tiling(wm, ws);
            break;
        case WAXL_LAYOUT_FULLSCREEN:
            for (uint32_t i = 0; i < ws->client_count; i++) {
                waxl_wm_client_t *c = ws->clients[i];
                if (!c || c->floating) continue;
                c->x = 0; c->y = 0;
                c->w = wm->screen_w;
                c->h = wm->screen_h;
                waxl_send_move(wm, c->id, 0, 0);
                waxl_send_resize(wm, c->id, wm->screen_w, wm->screen_h);
            }
            break;
        case WAXL_LAYOUT_FLOATING:
        default:
            break;
    }
}

/* === Workspace management === */

static void waxl_switch_workspace(waxl_wm_t *wm, uint32_t ws_id) {
    if (ws_id >= WAXL_MAX_WORKSPACES) return;

    waxl_workspace_t *old = &wm->workspaces[wm->active_workspace];
    for (uint32_t i = 0; i < old->client_count; i++) {
        waxl_wm_client_t *c = old->clients[i];
        if (c) {
            c->visible = false;
            waxl_send_move(wm, c->id, -9999, -9999);
        }
    }

    waxl_workspace_t *new_ws = &wm->workspaces[ws_id];
    for (uint32_t i = 0; i < new_ws->client_count; i++) {
        waxl_wm_client_t *c = new_ws->clients[i];
        if (c) {
            c->visible = true;
            waxl_send_move(wm, c->id, c->x, c->y);
            waxl_send_resize(wm, c->id, c->w, c->h);
        }
    }

    wm->active_workspace = ws_id;
    waxl_arrange(wm, new_ws);
}

/* === Key handling === */

static void waxl_handle_key(waxl_wm_t *wm, int keycode, int pressed) {
    #define KEY_LEFTMETA 125
    #define KEY_RIGHTMETA 126
    #define KEY_LEFTALT 56
    #define KEY_RIGHTALT 100
    #define KEY_Q 16
    #define KEY_T 20
    #define KEY_F 33
    #define KEY_H 35
    #define KEY_J 36
    #define KEY_K 37
    #define KEY_L 38
    #define KEY_1 2
    #define KEY_2 3
    #define KEY_3 4
    #define KEY_4 5
    #define KEY_5 6
    #define KEY_6 7
    #define KEY_7 8
    #define KEY_8 9
    #define KEY_9 10
    #define KEY_ENTER 28
    #define KEY_ESC 1
    #define KEY_SPACE 57

    if (keycode == KEY_LEFTMETA || keycode == KEY_RIGHTMETA ||
        keycode == KEY_LEFTALT || keycode == KEY_RIGHTALT) {
        wm->mod_pressed = pressed;
        return;
    }

    if (!pressed) return;

    if (wm->mod_pressed) {
        switch (keycode) {
            case KEY_Q:
                if (wm->focused_client) {
                    waxl_send_destroy(wm, wm->focused_client);
                    waxl_remove_client(wm, wm->focused_client);
                }
                break;
            case KEY_T:
                wm->workspaces[wm->active_workspace].layout = WAXL_LAYOUT_TILING;
                waxl_arrange(wm, &wm->workspaces[wm->active_workspace]);
                break;
            case KEY_F:
                wm->workspaces[wm->active_workspace].layout = WAXL_LAYOUT_FULLSCREEN;
                waxl_arrange(wm, &wm->workspaces[wm->active_workspace]);
                break;
            case KEY_SPACE:
                wm->workspaces[wm->active_workspace].layout = WAXL_LAYOUT_FLOATING;
                waxl_arrange(wm, &wm->workspaces[wm->active_workspace]);
                break;
            case KEY_J: {
                waxl_workspace_t *ws = &wm->workspaces[wm->active_workspace];
                for (uint32_t i = 0; i < ws->client_count; i++) {
                    if (ws->clients[i]->id == wm->focused_client && i + 1 < ws->client_count) {
                        wm->focused_client = ws->clients[i + 1]->id;
                        waxl_send_raise(wm, wm->focused_client);
                        break;
                    }
                }
                break;
            }
            case KEY_K: {
                waxl_workspace_t *ws = &wm->workspaces[wm->active_workspace];
                for (uint32_t i = 0; i < ws->client_count; i++) {
                    if (ws->clients[i]->id == wm->focused_client && i > 0) {
                        wm->focused_client = ws->clients[i - 1]->id;
                        waxl_send_raise(wm, wm->focused_client);
                        break;
                    }
                }
                break;
            }
            case KEY_1: case KEY_2: case KEY_3:
            case KEY_4: case KEY_5: case KEY_6:
            case KEY_7: case KEY_8: case KEY_9:
                waxl_switch_workspace(wm, keycode - KEY_1);
                break;
            case KEY_ENTER: {
                if (fork() == 0) {
                    execlp("./test-wm", "test-wm", NULL);
                    exit(1);
                }
                break;
            }
        }
    }
}

/* === Main === */

static volatile int g_running = 1;
static void sigint_handler(int sig) { g_running = 0; }

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    memset(&g_wm, 0, sizeof(g_wm));
    pthread_mutex_init(&g_wm.lock, NULL);
    g_wm.running = 1;
    g_wm.next_win_id = 1000;
    g_wm.screen_w = 1024;
    g_wm.screen_h = 768;
    g_wm.active_workspace = 0;

    for (int i = 0; i < WAXL_MAX_WORKSPACES; i++) {
        g_wm.workspaces[i].id = i;
        g_wm.workspaces[i].layout = WAXL_LAYOUT_FLOATING;
    }
    g_wm.workspaces[0].active = true;

    int retries = 50;
    while (retries-- > 0) {
        g_wm.sock_fd = waxl_connect_comp();
        if (g_wm.sock_fd >= 0) break;
        usleep(100000);
    }

    if (g_wm.sock_fd < 0) {
        fprintf(stderr, "[waxl-wm] Could not connect to compositor at %s\n", WAXL_SOCKET_PATH);
        pthread_mutex_destroy(&g_wm.lock);
        return 1;
    }

    printf("[waxl-wm] Connected to compositor\n");
    printf("[waxl-wm] Key bindings:\n");
    printf("  Super+Enter  : spawn test-wm app\n");
    printf("  Super+Q      : close focused window\n");
    printf("  Super+T      : tiling layout\n");
    printf("  Super+F      : fullscreen layout\n");
    printf("  Super+Space  : floating layout\n");
    printf("  Super+J/K    : focus next/prev window\n");
    printf("  Super+1..9   : switch workspace\n");

    struct pollfd pfd = { .fd = g_wm.sock_fd, .events = POLLIN };
    while (g_running && g_wm.running) {
        int ret = poll(&pfd, 1, 16);
        if (ret > 0 && (pfd.revents & POLLIN)) {
            waxl_msg_t msg;
            ssize_t n = recv(g_wm.sock_fd, &msg, sizeof(msg), 0);
            if (n == sizeof(msg) && msg.type == WAXL_MSG_INPUT) {
                waxl_handle_key(&g_wm, msg.x, msg.y);
            }
        }
    }

    printf("[waxl-wm] Shutting down...\n");

    while (g_wm.client_count > 0) {
        waxl_remove_client(&g_wm, g_wm.clients[0].id);
    }

    close(g_wm.sock_fd);
    pthread_mutex_destroy(&g_wm.lock);
    return 0;
}
