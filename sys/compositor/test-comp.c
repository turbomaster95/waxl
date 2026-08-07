/*
 * test-wm: Test Window Manager Client
 *
 * Connects to waxl-wm via Unix socket and creates 4 test windows.
 * Demonstrates the window creation, content update, and movement APIs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define WAXL_WM_SOCK "/tmp/waxl-wm.sock"

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

typedef struct {
    int wm_fd;
    int win_id;
    int width;
    int height;
    int x;
    int y;
} test_win_t;

static volatile int g_running = 1;

static void sigint_handler(int sig) {
    (void)sig;
    g_running = 0;
}

static int connect_to_wm(void) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_WM_SOCK, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(fd);
        return -1;
    }

    return fd;
}

static int wm_send_cmd(int fd, wm_cmd_msg_t *cmd, wm_reply_t *reply) {
    ssize_t n = send(fd, cmd, sizeof(*cmd), 0);
    if (n != sizeof(*cmd)) return -1;

    n = recv(fd, reply, sizeof(*reply), 0);
    if (n != sizeof(*reply)) return -1;

    return reply->status;
}

static int create_window(int fd, int x, int y, int w, int h, const char *title) {
    wm_cmd_msg_t cmd = {
        .cmd = WM_CMD_CREATE_WINDOW,
        .x = x, .y = y,
        .w = w, .h = h,
    };
    strncpy(cmd.title, title, sizeof(cmd.title) - 1);

    wm_reply_t reply;
    if (wm_send_cmd(fd, &cmd, &reply) < 0) {
        fprintf(stderr, "Failed to create window\n");
        return -1;
    }

    printf("[test-wm] Created window %d ('%s')\n", reply.win_id, title);
    return reply.win_id;
}

static int move_window(int fd, int win_id, int x, int y) {
    wm_cmd_msg_t cmd = {
        .cmd = WM_CMD_MOVE_WINDOW,
        .win_id = win_id,
        .x = x, .y = y,
    };
    wm_reply_t reply;
    return wm_send_cmd(fd, &cmd, &reply);
}

static int update_content(int fd, int win_id, uint32_t *pixels, int w, int h) {
    wm_cmd_msg_t cmd = {
        .cmd = WM_CMD_UPDATE_CONTENT,
        .win_id = win_id,
    };

    ssize_t n = send(fd, &cmd, sizeof(cmd), 0);
    if (n != sizeof(cmd)) return -1;

    size_t size = w * h * sizeof(uint32_t);
    size_t total = 0;
    while (total < size) {
        n = send(fd, (char*)pixels + total, size - total, 0);
        if (n < 0) {
            if (errno == EAGAIN || errno == EINTR) continue;
            return -1;
        }
        total += n;
    }

    wm_reply_t reply;
    n = recv(fd, &reply, sizeof(reply), 0);
    if (n != sizeof(reply)) return -1;

    return reply.status;
}

static int destroy_window(int fd, int win_id) {
    wm_cmd_msg_t cmd = {
        .cmd = WM_CMD_DESTROY_WINDOW,
        .win_id = win_id,
    };
    wm_reply_t reply;
    return wm_send_cmd(fd, &cmd, &reply);
}

// ---- Drawing Helpers ----

static inline void set_pixel(uint32_t *pixels, int w, int h, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    pixels[y * w + x] = color;
}

static void fill_rect(uint32_t *pixels, int w, int h, int x, int y, int rw, int rh, uint32_t color) {
    for (int row = y; row < y + rh && row < h; row++) {
        for (int col = x; col < x + rw && col < w; col++) {
            if (row >= 0 && col >= 0)
                pixels[row * w + col] = color;
        }
    }
}

static void draw_gradient(uint32_t *pixels, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint8_t r = (x * 255) / w;
            uint8_t g = (y * 255) / h;
            uint8_t b = 128;
            pixels[y * w + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

static void draw_checkerboard(uint32_t *pixels, int w, int h, int size) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            bool dark = ((x / size) + (y / size)) % 2 == 0;
            uint8_t c = dark ? 0x44 : 0x88;
            pixels[y * w + x] = (0xFF << 24) | (c << 16) | (c << 8) | c;
        }
    }
}

static void draw_circle(uint32_t *pixels, int w, int h, int cx, int cy, int r, uint32_t color) {
    for (int y = cy - r; y <= cy + r; y++) {
        for (int x = cx - r; x <= cx + r; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx*dx + dy*dy <= r*r) {
                set_pixel(pixels, w, h, x, y, color);
            }
        }
    }
}

// ---- Demo Render Functions ----

static void demo_gradient(test_win_t *win) {
    uint32_t *pixels = calloc(win->width * win->height, sizeof(uint32_t));
    if (!pixels) return;

    draw_gradient(pixels, win->width, win->height);
    draw_circle(pixels, win->width, win->height, win->width/2, win->height/2, 60, 0xFFFF0000);
    draw_circle(pixels, win->width, win->height, win->width/2 - 40, win->height/2 - 30, 20, 0xFF00FF00);
    draw_circle(pixels, win->width, win->height, win->width/2 + 40, win->height/2 - 30, 20, 0xFF00FF00);
    draw_circle(pixels, win->width, win->height, win->width/2, win->height/2 + 20, 25, 0xFF0000FF);

    update_content(win->wm_fd, win->win_id, pixels, win->width, win->height);
    free(pixels);
}

static void demo_checkerboard(test_win_t *win) {
    uint32_t *pixels = calloc(win->width * win->height, sizeof(uint32_t));
    if (!pixels) return;

    draw_checkerboard(pixels, win->width, win->height, 40);
    update_content(win->wm_fd, win->win_id, pixels, win->width, win->height);
    free(pixels);
}

static void demo_animated(test_win_t *win, int frame) {
    uint32_t *pixels = calloc(win->width * win->height, sizeof(uint32_t));
    if (!pixels) return;

    fill_rect(pixels, win->width, win->height, 0, 0, win->width, win->height, 0xFF111111);

    float t = frame * 0.05f;
    int bx = (int)(win->width/2 + (win->width/3) * sinf(t));
    int by = (int)(win->height/2 + (win->height/3) * cosf(t * 1.3f));

    draw_circle(pixels, win->width, win->height, bx, by, 30, 0xFFFF4444);
    draw_circle(pixels, win->width, win->height,
                (int)(win->width/2 + (win->width/4) * cosf(t * 0.7f)),
                (int)(win->height/2 + (win->height/4) * sinf(t * 0.9f)),
                20, 0xFF44FF44);

    update_content(win->wm_fd, win->win_id, pixels, win->width, win->height);
    free(pixels);
}

// ---- Main ----

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    signal(SIGINT, sigint_handler);

    int wm_fd = connect_to_wm();
    if (wm_fd < 0) {
        fprintf(stderr, "Failed to connect to window manager at %s\n", WAXL_WM_SOCK);
        fprintf(stderr, "Make sure waxl-wm is running first.\n");
        return 1;
    }

    test_win_t wins[4] = {
        { .wm_fd = wm_fd, .x = 50,  .y = 50,  .width = 400, .height = 300 }, // Gradient
        { .wm_fd = wm_fd, .x = 480, .y = 50,  .width = 400, .height = 300 }, // Checkerboard
        { .wm_fd = wm_fd, .x = 50,  .y = 380, .width = 400, .height = 300 }, // Bouncing Ball Animation
        { .wm_fd = wm_fd, .x = 480, .y = 380, .width = 400, .height = 300 }  // Moving Window
    };

    wins[0].win_id = create_window(wm_fd, wins[0].x, wins[0].y, wins[0].width, wins[0].height, "Gradient Demo");
    wins[1].win_id = create_window(wm_fd, wins[1].x, wins[1].y, wins[1].width, wins[1].height, "Checkerboard Demo");
    wins[2].win_id = create_window(wm_fd, wins[2].x, wins[2].y, wins[2].width, wins[2].height, "Animation Demo");
    wins[3].win_id = create_window(wm_fd, wins[3].x, wins[3].y, wins[3].width, wins[3].height, "Move Demo");

    for (int i = 0; i < 4; i++) {
        if (wins[i].win_id < 0) {
            close(wm_fd);
            return 1;
        }
    }

    // Static contents render once
    demo_gradient(&wins[0]);
    demo_checkerboard(&wins[1]);
    demo_gradient(&wins[3]);

    printf("[test-wm] Created 4 demo windows. Running render loop. Press Ctrl+C to exit.\n");

    int frame = 0;
    int move_dx = 3, move_dy = 2;
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 16666666}; // ~60 FPS

    while (g_running) {
        // Update bouncing animation in Window 3
        demo_animated(&wins[2], frame++);

        // Move Window 4 position
        wins[3].x += move_dx;
        wins[3].y += move_dy;
        if (wins[3].x > 800 || wins[3].x < 480) move_dx = -move_dx;
        if (wins[3].y > 600 || wins[3].y < 380) move_dy = -move_dy;
        move_window(wm_fd, wins[3].win_id, wins[3].x, wins[3].y);

        nanosleep(&ts, NULL);
    }

    printf("[test-wm] Cleaning up...\n");
    for (int i = 0; i < 4; i++) {
        destroy_window(wm_fd, wins[i].win_id);
    }
    close(wm_fd);

    return 0;
}
