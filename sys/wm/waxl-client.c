#include <waxlc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define WAXL_SOCKET_PATH  "/tmp/waxl-comp.sock"

/* Protocol: compositor <-> WM/client communication */
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

typedef struct {
    uint32_t id;
    uint32_t w, h;
    uint32_t *pixels;
    size_t   pix_size;
    int      shm_fd;
    char     shm_name[64];
} waxl_client_window_t;

typedef struct {
    int      sock_fd;
    pthread_mutex_t lock;
    waxl_client_window_t windows[32];
    uint32_t window_count;
} waxl_client_ctx_t;

static waxl_client_ctx_t g_ctx = { .sock_fd = -1 };
static pthread_once_t g_init_once = PTHREAD_ONCE_INIT;

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

static void waxl_client_init_once(void) {
    pthread_mutex_init(&g_ctx.lock, NULL);
    int retries = 100;
    while (retries-- > 0) {
        g_ctx.sock_fd = waxl_connect_comp();
        if (g_ctx.sock_fd >= 0) break;
        usleep(50000); /* 50ms */
    }
    if (g_ctx.sock_fd < 0) {
        fprintf(stderr, "[waxl-client] Failed to connect to compositor at %s\n", WAXL_SOCKET_PATH);
    }
}

static void waxl_ensure_init(void) {
    pthread_once(&g_init_once, waxl_client_init_once);
}

static bool waxl_send_msg(waxl_msg_t *msg) {
    waxl_ensure_init();
    if (g_ctx.sock_fd < 0) return false;
    pthread_mutex_lock(&g_ctx.lock);
    ssize_t n = send(g_ctx.sock_fd, msg, sizeof(*msg), 0);
    pthread_mutex_unlock(&g_ctx.lock);
    return n == sizeof(*msg);
}

static waxl_client_window_t* waxl_find_win(uint32_t id) {
    for (uint32_t i = 0; i < g_ctx.window_count; i++) {
        if (g_ctx.windows[i].id == id) return &g_ctx.windows[i];
    }
    return NULL;
}

static waxl_client_window_t* waxl_add_win(uint32_t id, uint32_t w, uint32_t h) {
    if (g_ctx.window_count >= 32) return NULL;
    waxl_client_window_t *win = &g_ctx.windows[g_ctx.window_count++];
    memset(win, 0, sizeof(*win));
    win->id = id;
    win->w = w;
    win->h = h;
    win->shm_fd = -1;
    return win;
}

static void waxl_remove_win(uint32_t id) {
    for (uint32_t i = 0; i < g_ctx.window_count; i++) {
        if (g_ctx.windows[i].id == id) {
            waxl_client_window_t *w = &g_ctx.windows[i];
            if (w->pixels && w->pixels != MAP_FAILED) {
                munmap(w->pixels, w->pix_size);
                w->pixels = NULL;
            }
            if (w->shm_fd >= 0) {
                close(w->shm_fd);
                w->shm_fd = -1;
            }
            if (w->shm_name[0]) {
                shm_unlink(w->shm_name);
                w->shm_name[0] = '\0';
            }
            memmove(&g_ctx.windows[i], &g_ctx.windows[i+1],
                    (g_ctx.window_count - i - 1) * sizeof(waxl_client_window_t));
            g_ctx.window_count--;
            return;
        }
    }
}

/* === Public API === */

uint32_t waxl_wm_create_window(const char *title, uint32_t w, uint32_t h, uint32_t color) {
    static uint32_t next_id = 10000;
    uint32_t id = next_id++;

    waxl_msg_t msg = {
        .type = WAXL_MSG_CREATE_WINDOW,
        .win_id = id,
        .x = 100, .y = 100,
        .w = w, .h = h,
        .color = color,
    };
    strncpy(msg.title, title ? title : "Untitled", sizeof(msg.title) - 1);

    if (!waxl_send_msg(&msg)) {
        fprintf(stderr, "[waxl-client] Failed to send CREATE_WINDOW\n");
        return 0;
    }

    waxl_add_win(id, w, h);
    return id;
}

void waxl_wm_destroy_window(uint32_t win_id) {
    waxl_msg_t msg = { .type = WAXL_MSG_DESTROY_WINDOW, .win_id = win_id };
    waxl_send_msg(&msg);
    waxl_remove_win(win_id);
}

void waxl_wm_move_window(uint32_t win_id, int x, int y) {
    waxl_msg_t msg = { .type = WAXL_MSG_MOVE_WINDOW, .win_id = win_id, .x = x, .y = y };
    waxl_send_msg(&msg);
}

void waxl_wm_resize_window(uint32_t win_id, uint32_t w, uint32_t h) {
    waxl_msg_t msg = { .type = WAXL_MSG_RESIZE_WINDOW, .win_id = win_id, .w = w, .h = h };
    waxl_send_msg(&msg);
    waxl_client_window_t *win = waxl_find_win(win_id);
    if (win) { win->w = w; win->h = h; }
}

void waxl_wm_set_title(uint32_t win_id, const char *title) {
    waxl_msg_t msg = { .type = WAXL_MSG_SET_TITLE, .win_id = win_id };
    strncpy(msg.title, title, sizeof(msg.title) - 1);
    waxl_send_msg(&msg);
}

void waxl_wm_raise_window(uint32_t win_id) {
    waxl_msg_t msg = { .type = WAXL_MSG_RAISE_WINDOW, .win_id = win_id };
    waxl_send_msg(&msg);
}

uint32_t* waxl_wm_get_framebuffer(uint32_t win_id, uint32_t *out_w, uint32_t *out_h) {
    waxl_ensure_init();
    waxl_client_window_t *win = waxl_find_win(win_id);
    if (!win) {
        fprintf(stderr, "[waxl-client] Window %u not found\n", win_id);
        return NULL;
    }

    uint32_t cw = win->w > 4 ? win->w - 4 : 1;
    uint32_t ch = win->h > 26 ? win->h - 26 : 1;

    if (win->pixels && win->pix_size == cw * ch * sizeof(uint32_t)) {
        if (out_w) *out_w = cw;
        if (out_h) *out_h = ch;
        return win->pixels;
    }

    if (win->pixels && win->pixels != MAP_FAILED) {
        munmap(win->pixels, win->pix_size);
        win->pixels = NULL;
    }
    if (win->shm_fd >= 0) {
        close(win->shm_fd);
        win->shm_fd = -1;
    }
    if (win->shm_name[0]) {
        shm_unlink(win->shm_name);
    }

    snprintf(win->shm_name, sizeof(win->shm_name), "/waxl_fb_%u", win_id);
    shm_unlink(win->shm_name);

    int fd = shm_open(win->shm_name, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("[waxl-client] shm_open failed");
        win->shm_name[0] = '\0';
        return NULL;
    }

    size_t size = cw * ch * sizeof(uint32_t);
    if (ftruncate(fd, size) < 0) {
        perror("[waxl-client] ftruncate failed");
        close(fd);
        shm_unlink(win->shm_name);
        win->shm_name[0] = '\0';
        return NULL;
    }

    win->shm_fd = fd;
    win->pix_size = size;
    win->pixels = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (win->pixels == MAP_FAILED) {
        perror("[waxl-client] mmap failed");
        close(fd);
        shm_unlink(win->shm_name);
        win->shm_fd = -1;
        win->pixels = NULL;
        win->shm_name[0] = '\0';
        return NULL;
    }

    waxl_msg_t msg = { .type = WAXL_MSG_GET_FB, .win_id = win_id };
    pthread_mutex_lock(&g_ctx.lock);
    send(g_ctx.sock_fd, &msg, sizeof(msg), 0);
    send(g_ctx.sock_fd, win->shm_name, strlen(win->shm_name) + 1, 0);
    pthread_mutex_unlock(&g_ctx.lock);

    if (out_w) *out_w = cw;
    if (out_h) *out_h = ch;
    return win->pixels;
}

void waxl_wm_present(uint32_t win_id) {
    waxl_msg_t msg = { .type = WAXL_MSG_PRESENT, .win_id = win_id };
    waxl_send_msg(&msg);
}

void waxl_wm_quit(void) {
    waxl_msg_t msg = { .type = WAXL_MSG_QUIT };
    waxl_send_msg(&msg);
}
