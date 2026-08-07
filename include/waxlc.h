#ifndef WAXLC_H
#define WAXLC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct waxl_app waxl_app_t;

// Create/destroy
waxl_app_t *waxl_app_create(uint32_t width, uint32_t height, const char *title);
void waxl_app_destroy(waxl_app_t *app);
bool waxl_app_running(waxl_app_t *app);

// Event polling (call every frame)
bool waxl_app_poll_events(waxl_app_t *app);

// Access pixel buffer (ARGB8888 format, row-major)
// Dimensions: app->width x app->height
// Stride: app->width * 4 bytes
uint32_t *waxl_app_pixels(waxl_app_t *app);
uint32_t waxl_app_width(waxl_app_t *app);
uint32_t waxl_app_height(waxl_app_t *app);

// Present frame to screen
void waxl_app_present(waxl_app_t *app);

void waxl_app_clear(waxl_app_t *app, uint32_t color);
void waxl_app_pixel(waxl_app_t *app, int x, int y, uint32_t color);
void waxl_app_rect(waxl_app_t *app, int x, int y, int w, int h, uint32_t color);
void waxl_app_rect_fill(waxl_app_t *app, int x, int y, int w, int h, uint32_t color);
void waxl_app_line(waxl_app_t *app, int x0, int y0, int x1, int y1, uint32_t color);
void waxl_app_circle(waxl_app_t *app, int cx, int cy, int r, uint32_t color);
void waxl_app_circle_fill(waxl_app_t *app, int cx, int cy, int r, uint32_t color);
void waxl_app_text(waxl_app_t *app, int x, int y, const char *text, uint32_t color);
void waxl_app_text_scaled(waxl_app_t *app, int x, int y, const char *text, uint32_t color, int scale);

bool waxl_app_key_down(waxl_app_t *app, int keycode);
bool waxl_app_key_pressed(waxl_app_t *app, int keycode);  // true once per press
bool waxl_app_mouse_down(waxl_app_t *app, int button);     // 0=left, 1=right, 2=middle
int waxl_app_mouse_x(waxl_app_t *app);
int waxl_app_mouse_y(waxl_app_t *app);
int waxl_app_mouse_dx(waxl_app_t *app);  // delta since last poll
int waxl_app_mouse_dy(waxl_app_t *app);

// Standard keycodes (linux/input-event-codes.h subset)
#define WAXL_KEY_ESC        1
#define WAXL_KEY_1          2
#define WAXL_KEY_2          3
#define WAXL_KEY_3          4
#define WAXL_KEY_4          5
#define WAXL_KEY_5          6
#define WAXL_KEY_6          7
#define WAXL_KEY_7          8
#define WAXL_KEY_8          9
#define WAXL_KEY_9          10
#define WAXL_KEY_0          11
#define WAXL_KEY_MINUS      12
#define WAXL_KEY_EQUAL      13
#define WAXL_KEY_BACKSPACE  14
#define WAXL_KEY_TAB        15
#define WAXL_KEY_Q          16
#define WAXL_KEY_W          17
#define WAXL_KEY_E          18
#define WAXL_KEY_R          19
#define WAXL_KEY_T          20
#define WAXL_KEY_Y          21
#define WAXL_KEY_U          22
#define WAXL_KEY_I          23
#define WAXL_KEY_O          24
#define WAXL_KEY_P          25
#define WAXL_KEY_LEFTBRACE  26
#define WAXL_KEY_RIGHTBRACE 27
#define WAXL_KEY_ENTER      28
#define WAXL_KEY_LEFTCTRL   29
#define WAXL_KEY_A          30
#define WAXL_KEY_S          31
#define WAXL_KEY_D          32
#define WAXL_KEY_F          33
#define WAXL_KEY_G          34
#define WAXL_KEY_H          35
#define WAXL_KEY_J          36
#define WAXL_KEY_K          37
#define WAXL_KEY_L          38
#define WAXL_KEY_SEMICOLON  39
#define WAXL_KEY_APOSTROPHE 40
#define WAXL_KEY_GRAVE      41
#define WAXL_KEY_LEFTSHIFT  42
#define WAXL_KEY_BACKSLASH  43
#define WAXL_KEY_Z          44
#define WAXL_KEY_X          45
#define WAXL_KEY_C          46
#define WAXL_KEY_V          47
#define WAXL_KEY_B          48
#define WAXL_KEY_N          49
#define WAXL_KEY_M          50
#define WAXL_KEY_COMMA      51
#define WAXL_KEY_DOT        52
#define WAXL_KEY_SLASH      53
#define WAXL_KEY_RIGHTSHIFT 54
#define WAXL_KEY_KPASTERISK 55
#define WAXL_KEY_LEFTALT    56
#define WAXL_KEY_SPACE      57
#define WAXL_KEY_CAPSLOCK   58
#define WAXL_KEY_F1         59
#define WAXL_KEY_F2         60
#define WAXL_KEY_F3         61
#define WAXL_KEY_F4         62
#define WAXL_KEY_F5         63
#define WAXL_KEY_F6         64
#define WAXL_KEY_F7         65
#define WAXL_KEY_F8         66
#define WAXL_KEY_F9         67
#define WAXL_KEY_F10        68
#define WAXL_KEY_F11        87
#define WAXL_KEY_F12        88
#define WAXL_KEY_RIGHTCTRL  97
#define WAXL_KEY_RIGHTALT   100
#define WAXL_KEY_HOME       102
#define WAXL_KEY_UP         103
#define WAXL_KEY_PAGEUP     104
#define WAXL_KEY_LEFT       105
#define WAXL_KEY_RIGHT      106
#define WAXL_KEY_END        107
#define WAXL_KEY_DOWN       108
#define WAXL_KEY_PAGEDOWN   109
#define WAXL_KEY_INSERT     110
#define WAXL_KEY_DELETE     111
#define WAXL_KEY_LEFTMETA   125
#define WAXL_KEY_RIGHTMETA  126

// Mouse buttons
#define WAXL_MOUSE_LEFT     0x110
#define WAXL_MOUSE_RIGHT    0x111
#define WAXL_MOUSE_MIDDLE   0x112

// Color constructor
#define WAXL_RGB(r, g, b) ((uint32_t)((r) << 16 | (g) << 8 | (b)))

// Catppuccin Mocha palette
#define WAXL_CTP_RED      0xF38BA8
#define WAXL_CTP_TEXT     0xCDD6F4
#define WAXL_CTP_TEAL     0x94E2D5
#define WAXL_CTP_SURFACE2 0x585B70
#define WAXL_CTP_SUBTEXT1 0xBAC2DE
#define WAXL_CTP_YELLOW   0xF9E2AF
#define WAXL_CTP_LAVENDER 0xB4BEFE
#define WAXL_CTP_BLUE     0x89B4FA

#ifdef __cplusplus
}
#endif

#endif // WAXL_APP_H

#ifdef WAXL_APP_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>

#include <libinder.h>
#include <waxlb.h>

// 5x7 font data (ASCII 32-126)
static const uint8_t WAXL_FONT_5X7[][5] = {
    {0x00,0x00,0x00,0x00,0x00},{0x00,0x00,0x5F,0x00,0x00},{0x00,0x07,0x00,0x07,0x00},
    {0x14,0x7F,0x14,0x7F,0x14},{0x24,0x2A,0x7F,0x2A,0x12},{0x23,0x13,0x08,0x64,0x62},
    {0x36,0x49,0x55,0x22,0x50},{0x00,0x05,0x03,0x00,0x00},{0x00,0x1C,0x22,0x41,0x00},
    {0x00,0x41,0x22,0x1C,0x00},{0x08,0x2A,0x1C,0x2A,0x08},{0x08,0x08,0x3E,0x08,0x08},
    {0x00,0x50,0x30,0x00,0x00},{0x08,0x08,0x08,0x08,0x08},{0x00,0x60,0x60,0x00,0x00},
    {0x20,0x10,0x08,0x04,0x02},{0x3E,0x51,0x49,0x45,0x3E},{0x00,0x42,0x7F,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4B,0x31},{0x18,0x14,0x12,0x7F,0x10},
    {0x27,0x45,0x45,0x45,0x39},{0x3C,0x4A,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1E},{0x00,0x36,0x36,0x00,0x00},
    {0x00,0x56,0x36,0x00,0x00},{0x00,0x08,0x14,0x22,0x41},{0x14,0x14,0x14,0x14,0x14},
    {0x41,0x22,0x14,0x08,0x00},{0x02,0x01,0x51,0x09,0x06},{0x32,0x49,0x79,0x41,0x3E},
    {0x7E,0x11,0x11,0x11,0x7E},{0x7F,0x49,0x49,0x49,0x36},{0x3E,0x41,0x41,0x41,0x22},
    {0x7F,0x41,0x41,0x22,0x1C},{0x7F,0x49,0x49,0x49,0x41},{0x7F,0x09,0x09,0x01,0x01},
    {0x3E,0x41,0x41,0x51,0x32},{0x7F,0x08,0x08,0x08,0x7F},{0x00,0x41,0x7F,0x41,0x00},
    {0x20,0x40,0x41,0x3F,0x01},{0x7F,0x08,0x14,0x22,0x41},{0x7F,0x40,0x40,0x40,0x40},
    {0x7F,0x02,0x04,0x02,0x7F},{0x7F,0x04,0x08,0x10,0x7F},{0x3E,0x41,0x41,0x41,0x3E},
    {0x7F,0x09,0x09,0x09,0x06},{0x3E,0x41,0x51,0x21,0x5E},{0x7F,0x09,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31},{0x01,0x01,0x7F,0x01,0x01},{0x3F,0x40,0x40,0x40,0x3F},
    {0x1F,0x20,0x40,0x20,0x1F},{0x7F,0x20,0x18,0x20,0x7F},{0x63,0x14,0x08,0x14,0x63},
    {0x03,0x04,0x78,0x04,0x03},{0x61,0x51,0x49,0x45,0x43},{0x00,0x00,0x7F,0x41,0x41},
    {0x02,0x04,0x08,0x10,0x20},{0x41,0x41,0x7F,0x00,0x00},{0x04,0x02,0x01,0x02,0x04},
    {0x40,0x40,0x40,0x40,0x40},{0x00,0x01,0x02,0x04,0x00},{0x20,0x54,0x54,0x54,0x78},
    {0x7F,0x48,0x44,0x44,0x38},{0x38,0x44,0x44,0x44,0x20},{0x38,0x44,0x44,0x48,0x7F},
    {0x38,0x54,0x54,0x54,0x18},{0x08,0x7E,0x09,0x01,0x02},{0x08,0x14,0x54,0x54,0x3C},
    {0x7F,0x08,0x04,0x04,0x78},{0x00,0x44,0x7D,0x40,0x00},{0x20,0x40,0x44,0x3D,0x00},
    {0x00,0x7F,0x10,0x28,0x44},{0x00,0x41,0x7F,0x40,0x00},{0x7C,0x04,0x18,0x04,0x78},
    {0x7C,0x08,0x04,0x04,0x78},{0x38,0x44,0x44,0x44,0x38},{0x7C,0x14,0x14,0x14,0x08},
    {0x08,0x14,0x14,0x18,0x7C},{0x7C,0x08,0x04,0x04,0x08},{0x48,0x54,0x54,0x54,0x20},
    {0x04,0x3F,0x44,0x40,0x20},{0x3C,0x40,0x40,0x20,0x7C},{0x1C,0x20,0x40,0x20,0x1C},
    {0x3C,0x40,0x30,0x40,0x3C},{0x44,0x28,0x10,0x28,0x44},{0x0C,0x50,0x50,0x50,0x3C},
    {0x44,0x64,0x54,0x4C,0x44},{0x00,0x08,0x36,0x41,0x00},{0x00,0x00,0x7F,0x00,0x00},
    {0x00,0x41,0x36,0x08,0x00},{0x08,0x08,0x2A,0x1C,0x08}
};

struct waxl_app {
    // Binder connection to waxl-wm
    libinder_t *binder;
    uint32_t wm_handle;

    // Window
    waxl_id_t window_id;
    uint32_t width, height;

    // Buffer
    char shm_name[WAXL_MAX_SHM_NAME];
    int shm_fd;
    uint32_t *pixels;
    size_t shm_size;
    bool buffer_ready;

    // State
    bool running;
    bool needs_present;

    // Input
    bool keys[256];
    bool keys_pressed[256];
    bool mouse_buttons[3];
    int mouse_x, mouse_y;
    int mouse_dx, mouse_dy;
    int last_mouse_x, last_mouse_y;
};

static bool wm_call(waxl_app_t *app, uint32_t code, const void *in, size_t in_len,
                     void *out, size_t out_cap, size_t *out_len) {
    if (!app || !app->binder) return false;
    size_t ol = out_cap;
    int ret = libinder_call(app->binder, app->wm_handle, code, in, in_len, out, &ol);
    if (out_len) *out_len = ol;
    return ret == 0;
}

waxl_app_t *waxl_app_create(uint32_t width, uint32_t height, const char *title) {
    waxl_app_t *app = calloc(1, sizeof(waxl_app_t));
    if (!app) return NULL;

    app->width = width ? width : 400;
    app->height = height ? height : 300;
    app->running = true;
    app->wm_handle = 1;  // waxl-wm service handle

    // Connect to binder
    app->binder = libinder_open(NULL, 0);
    if (!app->binder) {
        fprintf(stderr, "[waxl_app] Failed to connect to binder\n");
        free(app);
        return NULL;
    }

    // Create window via waxl-wm
    waxl_create_req_t req = {
        .width = app->width,
        .height = app->height,
        .flags = 0
    };
    strncpy(req.title, title ? title : "Waxl App", WAXL_MAX_TITLE_LEN - 1);

    waxl_create_resp_t resp = {0};
    size_t out_len = sizeof(resp);
    if (!wm_call(app, WAXL_WM_TXN_CREATE_WINDOW, &req, sizeof(req), &resp, sizeof(resp), &out_len)) {
        fprintf(stderr, "[waxl_app] Failed to create window\n");
        libinder_close(app->binder);
        free(app);
        return NULL;
    }

    app->window_id = resp.id;
    app->width = resp.width;
    app->height = resp.height;

    // Get buffer from wm
    waxl_id_t wid = app->window_id;
    out_len = sizeof(resp);
    if (!wm_call(app, WAXL_WM_TXN_GET_BUFFER, &wid, sizeof(wid), &resp, sizeof(resp), &out_len)) {
        fprintf(stderr, "[waxl_app] Failed to get buffer\n");
        // Destroy window
        wm_call(app, WAXL_WM_TXN_DESTROY_WINDOW, &wid, sizeof(wid), NULL, 0, NULL);
        libinder_close(app->binder);
        free(app);
        return NULL;
    }

    strncpy(app->shm_name, resp.shm_name, WAXL_MAX_SHM_NAME - 1);
    app->shm_size = resp.shm_size;

    // Open and map shm
    app->shm_fd = shm_open(app->shm_name, O_RDWR, 0);
    if (app->shm_fd < 0) {
        perror("[waxl_app] shm_open");
        wm_call(app, WAXL_WM_TXN_DESTROY_WINDOW, &wid, sizeof(wid), NULL, 0, NULL);
        libinder_close(app->binder);
        free(app);
        return NULL;
    }

    app->pixels = mmap(NULL, app->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, app->shm_fd, 0);
    if (app->pixels == MAP_FAILED) {
        perror("[waxl_app] mmap");
        close(app->shm_fd);
        wm_call(app, WAXL_WM_TXN_DESTROY_WINDOW, &wid, sizeof(wid), NULL, 0, NULL);
        libinder_close(app->binder);
        free(app);
        return NULL;
    }

    app->buffer_ready = true;
    printf("[waxl_app] Window %u created (%ux%u) buffer: %s\n", 
           app->window_id, app->width, app->height, app->shm_name);

    return app;
}

void waxl_app_destroy(waxl_app_t *app) {
    if (!app) return;
    if (app->pixels) munmap(app->pixels, app->shm_size);
    if (app->shm_fd >= 0) close(app->shm_fd);
    if (app->window_id) {
        waxl_id_t wid = app->window_id;
        wm_call(app, WAXL_WM_TXN_DESTROY_WINDOW, &wid, sizeof(wid), NULL, 0, NULL);
    }
    if (app->binder) libinder_close(app->binder);
    free(app);
}

bool waxl_app_running(waxl_app_t *app) {
    return app && app->running;
}

bool waxl_app_poll_events(waxl_app_t *app) {
    if (!app) return false;

    // Reset per-frame state
    app->mouse_dx = app->mouse_x - app->last_mouse_x;
    app->mouse_dy = app->mouse_y - app->last_mouse_y;
    app->last_mouse_x = app->mouse_x;
    app->last_mouse_y = app->mouse_y;
    memset(app->keys_pressed, 0, sizeof(app->keys_pressed));

    // Poll for events from wm (non-blocking)
    waxl_app_event_t ev;
    size_t out_len = sizeof(ev);
    while (wm_call(app, WAXL_WM_TXN_GET_EVENT, &app->window_id, sizeof(app->window_id),
                    &ev, sizeof(ev), &out_len)) {
        if (ev.type == WAXL_EV_NONE) break;

        switch (ev.type) {
            case WAXL_EV_POINTER_MOTION:
                app->mouse_x = ev.data.pointer.x;
                app->mouse_y = ev.data.pointer.y;
                break;
            case WAXL_EV_POINTER_BUTTON:
                if (ev.data.button.button == WAXL_MOUSE_LEFT) app->mouse_buttons[0] = ev.data.button.state;
                if (ev.data.button.button == WAXL_MOUSE_RIGHT) app->mouse_buttons[1] = ev.data.button.state;
                if (ev.data.button.button == WAXL_MOUSE_MIDDLE) app->mouse_buttons[2] = ev.data.button.state;
                break;
            case WAXL_EV_KEYBOARD_KEY:
                if (ev.data.key.key < 256) {
                    app->keys[ev.data.key.key] = ev.data.key.state;
                    if (ev.data.key.state) app->keys_pressed[ev.data.key.key] = true;
                }
                if (ev.data.key.key == WAXL_KEY_ESC && !ev.data.key.state) {
                    app->running = false;
                }
                break;
            case WAXL_EV_CLOSE_REQUEST:
                app->running = false;
                break;
            case WAXL_EV_CONFIGURE:
                app->width = ev.data.configure.width;
                app->height = ev.data.configure.height;
                break;
            default:
                break;
        }
        out_len = sizeof(ev);
    }

    return true;
}

uint32_t *waxl_app_pixels(waxl_app_t *app) {
    return app ? app->pixels : NULL;
}

uint32_t waxl_app_width(waxl_app_t *app) {
    return app ? app->width : 0;
}

uint32_t waxl_app_height(waxl_app_t *app) {
    return app ? app->height : 0;
}

void waxl_app_present(waxl_app_t *app) {
    if (!app || !app->buffer_ready) return;

    // Commit buffer to wm
    waxl_id_t wid = app->window_id;
    wm_call(app, WAXL_WM_TXN_COMMIT_BUFFER, &wid, sizeof(wid), NULL, 0, NULL);
    app->needs_present = false;
}

void waxl_app_clear(waxl_app_t *app, uint32_t color) {
    if (!app || !app->pixels) return;
    size_t count = (size_t)app->width * app->height;
    for (size_t i = 0; i < count; i++) app->pixels[i] = color;
}

void waxl_app_pixel(waxl_app_t *app, int x, int y, uint32_t color) {
    if (!app || x < 0 || y < 0 || x >= (int)app->width || y >= (int)app->height) return;
    app->pixels[y * app->width + x] = color;
}

void waxl_app_rect(waxl_app_t *app, int x, int y, int w, int h, uint32_t color) {
    if (!app) return;
    // Outline only
    for (int i = 0; i < w; i++) { waxl_app_pixel(app, x + i, y, color); waxl_app_pixel(app, x + i, y + h - 1, color); }
    for (int i = 0; i < h; i++) { waxl_app_pixel(app, x, y + i, color); waxl_app_pixel(app, x + w - 1, y + i, color); }
}

void waxl_app_rect_fill(waxl_app_t *app, int x, int y, int w, int h, uint32_t color) {
    if (!app) return;
    int x0 = x < 0 ? 0 : x, y0 = y < 0 ? 0 : y;
    int x1 = x + w > (int)app->width ? (int)app->width : x + w;
    int y1 = y + h > (int)app->height ? (int)app->height : y + h;
    for (int py = y0; py < y1; py++)
        for (int px = x0; px < x1; px++)
            app->pixels[py * app->width + px] = color;
}

void waxl_app_line(waxl_app_t *app, int x0, int y0, int x1, int y1, uint32_t color) {
    if (!app) return;
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        waxl_app_pixel(app, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void waxl_app_circle(waxl_app_t *app, int cx, int cy, int r, uint32_t color) {
    if (!app) return;
    int x = r, y = 0, err = 0;
    while (x >= y) {
        waxl_app_pixel(app, cx + x, cy + y, color);
        waxl_app_pixel(app, cx + y, cy + x, color);
        waxl_app_pixel(app, cx - y, cy + x, color);
        waxl_app_pixel(app, cx - x, cy + y, color);
        waxl_app_pixel(app, cx - x, cy - y, color);
        waxl_app_pixel(app, cx - y, cy - x, color);
        waxl_app_pixel(app, cx + y, cy - x, color);
        waxl_app_pixel(app, cx + x, cy - y, color);
        if (err <= 0) { y += 1; err += 2*y + 1; }
        if (err > 0) { x -= 1; err -= 2*x + 1; }
    }
}

void waxl_app_circle_fill(waxl_app_t *app, int cx, int cy, int r, uint32_t color) {
    if (!app) return;
    for (int dy = -r; dy <= r; dy++)
        for (int dx = -r; dx <= r; dx++)
            if (dx*dx + dy*dy <= r*r)
                waxl_app_pixel(app, cx + dx, cy + dy, color);
}

void waxl_app_text(waxl_app_t *app, int x, int y, const char *text, uint32_t color) {
    waxl_app_text_scaled(app, x, y, text, color, 1);
}

void waxl_app_text_scaled(waxl_app_t *app, int x, int y, const char *text, uint32_t color, int scale) {
    if (!app || scale < 1) return;
    int cx = x;
    while (*text) {
        char c = *text;
        if (c < 32 || c > 126) c = '?';
        const uint8_t *g = WAXL_FONT_5X7[c - 32];
        for (int col = 0; col < 5; col++) {
            for (int row = 0; row < 7; row++) {
                if (g[col] & (1 << row)) {
                    for (int sy = 0; sy < scale; sy++)
                        for (int sx = 0; sx < scale; sx++)
                            waxl_app_pixel(app, cx + col * scale + sx, y + row * scale + sy, color);
                }
            }
        }
        cx += 6 * scale;
        text++;
    }
}

bool waxl_app_key_down(waxl_app_t *app, int keycode) {
    return app && keycode >= 0 && keycode < 256 ? app->keys[keycode] : false;
}

bool waxl_app_key_pressed(waxl_app_t *app, int keycode) {
    return app && keycode >= 0 && keycode < 256 ? app->keys_pressed[keycode] : false;
}

bool waxl_app_mouse_down(waxl_app_t *app, int button) {
    return app && button >= 0 && button < 3 ? app->mouse_buttons[button] : false;
}

int waxl_app_mouse_x(waxl_app_t *app) {
    return app ? app->mouse_x : 0;
}

int waxl_app_mouse_y(waxl_app_t *app) {
    return app ? app->mouse_y : 0;
}

int waxl_app_mouse_dx(waxl_app_t *app) {
    return app ? app->mouse_dx : 0;
}

int waxl_app_mouse_dy(waxl_app_t *app) {
    return app ? app->mouse_dy : 0;
}

#endif // WAXL_APP_IMPL
