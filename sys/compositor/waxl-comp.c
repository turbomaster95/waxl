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
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <fcntl.h>
#include <pthread.h>

#include <lidrm.h>

#define WAXL_SOCKET_PATH  "/tmp/waxl-comp.sock"
#define WAXL_MAX_WINDOWS  32
#define WAXL_MAX_EVENTS   64
#define WAXL_TITLEBAR_H   24
#define WAXL_BORDER_W     2
#define WAXL_EPOLL_MAX    128

/* Protocol: compositor <-> WM communication */
typedef enum {
    WAXL_MSG_CREATE_WINDOW = 1,
    WAXL_MSG_DESTROY_WINDOW,
    WAXL_MSG_MOVE_WINDOW,
    WAXL_MSG_RESIZE_WINDOW,
    WAXL_MSG_RAISE_WINDOW,
    WAXL_MSG_GET_FB,          /* Client sends shm name for framebuffer */
    WAXL_MSG_PRESENT,         /* Client tells compositor to update */
    WAXL_MSG_INPUT,           /* Compositor -> WM: keyboard/mouse event */
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
    int32_t  x, y;
    uint32_t w, h;
    uint32_t color;
    bool     visible;
    bool     focused;
    char     title[64];
    uint32_t *pixels;     /* Client-provided pixel buffer (shared memory) */
    size_t   pix_size;
    int      shm_fd;      /* Shared memory fd for client content */
} waxl_window_t;

typedef struct {
    lidrm_t drm;
    waxl_window_t windows[WAXL_MAX_WINDOWS];
    uint32_t window_count;
    uint32_t focused_id;
    int      sock_fd;
    int      epoll_fd;
    int      running;
    uint32_t bg_color;
    int      mouse_x, mouse_y;
    bool     mouse_down;
    uint32_t drag_win;      /* Window being dragged */
    int      drag_off_x, drag_off_y;
    pthread_mutex_t lock;
} waxl_comp_t;

static waxl_comp_t g_comp = {0};

/* === Drawing helpers === */

static inline uint32_t waxl_color(uint8_t r, uint8_t g, uint8_t b) {
    return 0xFF000000 | ((b << 16) | (g << 8) | r); /* XRGB8888 */
}

static void waxl_put_pixel(waxl_comp_t *c, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || (uint32_t)x >= c->drm.width || (uint32_t)y >= c->drm.height)
        return;
    c->drm.pixels[y * (c->drm.pitch / 4) + x] = color;
}

static void waxl_fill_rect(waxl_comp_t *c, int x, int y, int w, int h, uint32_t color) {
    int x0 = x < 0 ? 0 : x;
    int y0 = y < 0 ? 0 : y;
    int x1 = x + w > (int)c->drm.width ? (int)c->drm.width : x + w;
    int y1 = y + h > (int)c->drm.height ? (int)c->drm.height : y + h;
    if (x1 <= x0 || y1 <= y0) return;
    uint32_t pitch_pixels = c->drm.pitch / 4;
    for (int py = y0; py < y1; py++) {
        for (int px = x0; px < x1; px++) {
            c->drm.pixels[py * pitch_pixels + px] = color;
        }
    }
}

static void waxl_draw_rect_outline(waxl_comp_t *c, int x, int y, int w, int h, uint32_t color) {
    waxl_fill_rect(c, x, y, w, 1, color);
    waxl_fill_rect(c, x, y + h - 1, w, 1, color);
    waxl_fill_rect(c, x, y, 1, h, color);
    waxl_fill_rect(c, x + w - 1, y, 1, h, color);
}

/* 8x8 font bitmap for ASCII 32-127.
 * Each byte is a row. Bit 0 (LSB) is the LEFT-most pixel.
 * This font is stored LSB-first per row. */
static const uint8_t font8x8[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00},
    {0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x36,0x36,0x7F,0x36,0x7F,0x36,0x36,0x00},
    {0x0C,0x3E,0x03,0x1E,0x30,0x1F,0x0C,0x00},
    {0x00,0x63,0x33,0x18,0x0C,0x66,0x63,0x00},
    {0x1C,0x36,0x1C,0x6E,0x3B,0x33,0x6E,0x00},
    {0x06,0x06,0x03,0x00,0x00,0x00,0x00,0x00},
    {0x18,0x0C,0x06,0x06,0x06,0x0C,0x18,0x00},
    {0x06,0x0C,0x18,0x18,0x18,0x0C,0x06,0x00},
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00},
    {0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x06},
    {0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00},
    {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00},
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00},
    {0x0C,0x0E,0x0C,0x0C,0x0C,0x0C,0x3F,0x00},
    {0x1E,0x33,0x30,0x1C,0x06,0x33,0x3F,0x00},
    {0x1E,0x33,0x30,0x1C,0x30,0x33,0x1E,0x00},
    {0x38,0x3C,0x36,0x33,0x7F,0x30,0x78,0x00},
    {0x3F,0x03,0x1F,0x30,0x30,0x33,0x1E,0x00},
    {0x1C,0x06,0x03,0x1F,0x33,0x33,0x1E,0x00},
    {0x3F,0x33,0x30,0x18,0x0C,0x0C,0x0C,0x00},
    {0x1E,0x33,0x33,0x1E,0x33,0x33,0x1E,0x00},
    {0x1E,0x33,0x33,0x3E,0x30,0x18,0x0E,0x00},
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x00},
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x06},
    {0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x00},
    {0x00,0x00,0x3F,0x00,0x00,0x3F,0x00,0x00},
    {0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00},
    {0x1E,0x33,0x30,0x18,0x0C,0x00,0x0C,0x00},
    {0x3E,0x63,0x7B,0x7B,0x7B,0x03,0x1E,0x00},
    {0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00},
    {0x3F,0x66,0x66,0x3E,0x66,0x66,0x3F,0x00},
    {0x3C,0x66,0x03,0x03,0x03,0x66,0x3C,0x00},
    {0x1F,0x36,0x66,0x66,0x66,0x36,0x1F,0x00},
    {0x7F,0x46,0x16,0x1E,0x16,0x46,0x7F,0x00},
    {0x7F,0x46,0x16,0x1E,0x16,0x06,0x0F,0x00},
    {0x3C,0x66,0x03,0x03,0x73,0x66,0x7C,0x00},
    {0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00},
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00},
    {0x78,0x30,0x30,0x30,0x33,0x33,0x1E,0x00},
    {0x67,0x66,0x36,0x1E,0x36,0x66,0x67,0x00},
    {0x0F,0x06,0x06,0x06,0x46,0x66,0x7F,0x00},
    {0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00},
    {0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x00},
    {0x1C,0x36,0x63,0x63,0x63,0x36,0x1C,0x00},
    {0x3F,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00},
    {0x1E,0x33,0x33,0x33,0x3B,0x1E,0x38,0x00},
    {0x3F,0x66,0x66,0x3E,0x36,0x66,0x67,0x00},
    {0x1E,0x33,0x07,0x0E,0x38,0x33,0x1E,0x00},
    {0x3F,0x2D,0x0C,0x0C,0x0C,0x0C,0x1E,0x00},
    {0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x00},
    {0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00},
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
    {0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00},
    {0x33,0x33,0x33,0x1E,0x0C,0x0C,0x1E,0x00},
    {0x7F,0x63,0x31,0x18,0x4C,0x66,0x7F,0x00},
    {0x1E,0x06,0x06,0x06,0x06,0x06,0x1E,0x00},
    {0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00},
    {0x1E,0x18,0x18,0x18,0x18,0x18,0x1E,0x00},
    {0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},
    {0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x1E,0x30,0x3E,0x33,0x6E,0x00},
    {0x07,0x06,0x06,0x3E,0x66,0x66,0x3B,0x00},
    {0x00,0x00,0x1E,0x33,0x03,0x33,0x1E,0x00},
    {0x38,0x30,0x30,0x3e,0x33,0x33,0x6E,0x00},
    {0x00,0x00,0x1E,0x33,0x3f,0x03,0x1E,0x00},
    {0x1C,0x36,0x06,0x0f,0x06,0x06,0x0F,0x00},
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x1F},
    {0x07,0x06,0x36,0x6E,0x66,0x66,0x67,0x00},
    {0x0C,0x00,0x0E,0x0C,0x0C,0x0C,0x1E,0x00},
    {0x30,0x00,0x30,0x30,0x30,0x33,0x33,0x1E},
    {0x07,0x06,0x66,0x36,0x1E,0x36,0x67,0x00},
    {0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00},
    {0x00,0x00,0x33,0x7F,0x7F,0x6B,0x63,0x00},
    {0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x00},
    {0x00,0x00,0x1E,0x33,0x33,0x33,0x1E,0x00},
    {0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F},
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78},
    {0x00,0x00,0x3B,0x6E,0x66,0x06,0x0F,0x00},
    {0x00,0x00,0x3E,0x03,0x1E,0x30,0x1F,0x00},
    {0x08,0x0C,0x3E,0x0C,0x0C,0x2C,0x18,0x00},
    {0x00,0x00,0x33,0x33,0x33,0x33,0x6E,0x00},
    {0x00,0x00,0x33,0x33,0x33,0x1E,0x0C,0x00},
    {0x00,0x00,0x63,0x6B,0x7F,0x36,0x36,0x00},
    {0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00},
    {0x00,0x00,0x33,0x33,0x33,0x3E,0x30,0x1F},
    {0x00,0x00,0x3F,0x19,0x0C,0x26,0x3F,0x00},
};

static void waxl_draw_char(waxl_comp_t *c, int x, int y, char ch, uint32_t color) {
    if (ch < 32 || ch > 127) ch = '?';
    const uint8_t *bmp = font8x8[ch - 32];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            /* Bit 0 (LSB) is the LEFT-most pixel in this font */
            if (bmp[row] & (1 << col)) {
                waxl_put_pixel(c, x + col, y + row, color);
            }
        }
    }
}

static void waxl_draw_string(waxl_comp_t *c, int x, int y, const char *str, uint32_t color) {
    while (*str) {
        waxl_draw_char(c, x, y, *str, color);
        x += 8;
        str++;
    }
}

static void waxl_draw_line(waxl_comp_t *c, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        waxl_put_pixel(c, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/* === Window management === */

static waxl_window_t* waxl_find_window(waxl_comp_t *c, uint32_t id) {
    for (uint32_t i = 0; i < c->window_count; i++) {
        if (c->windows[i].id == id) return &c->windows[i];
    }
    return NULL;
}

static waxl_window_t* waxl_create_window(waxl_comp_t *c, uint32_t id, int x, int y, uint32_t w, uint32_t h, uint32_t color) {
    if (c->window_count >= WAXL_MAX_WINDOWS) return NULL;
    waxl_window_t *win = &c->windows[c->window_count++];
    memset(win, 0, sizeof(*win));
    win->id = id;
    win->x = x;
    win->y = y;
    win->w = w;
    win->h = h;
    win->color = color;
    win->visible = true;
    snprintf(win->title, sizeof(win->title), "Window %u", id);
    return win;
}

static void waxl_destroy_window(waxl_comp_t *c, uint32_t id) {
    for (uint32_t i = 0; i < c->window_count; i++) {
        if (c->windows[i].id == id) {
            if (c->windows[i].pixels) {
                munmap(c->windows[i].pixels, c->windows[i].pix_size);
                c->windows[i].pixels = NULL;
            }
            if (c->windows[i].shm_fd >= 0) {
                close(c->windows[i].shm_fd);
                c->windows[i].shm_fd = -1;
            }
            memmove(&c->windows[i], &c->windows[i+1], (c->window_count - i - 1) * sizeof(waxl_window_t));
            c->window_count--;
            return;
        }
    }
}

static void waxl_raise_window(waxl_comp_t *c, uint32_t id) {
    /* Find the window's index in the array */
    int idx = -1;
    for (uint32_t i = 0; i < c->window_count; i++) {
        if (c->windows[i].id == id) { idx = (int)i; break; }
    }
    if (idx < 0) return;

    waxl_window_t tmp = c->windows[idx];

    if ((uint32_t)idx < c->window_count - 1) {
        memmove(&c->windows[idx], &c->windows[idx + 1],
                (c->window_count - idx - 1) * sizeof(waxl_window_t));
    }

    c->window_count--;
    c->windows[c->window_count] = tmp;
    c->window_count++;
    c->focused_id = id;
}

static void waxl_compose(waxl_comp_t *c) {
    /* Clear background */
    lidrm_clear(&c->drm, c->bg_color);

    /* Draw all windows bottom-to-top */
    for (uint32_t i = 0; i < c->window_count; i++) {
        waxl_window_t *win = &c->windows[i];
        if (!win->visible) continue;

        int wx = win->x;
        int wy = win->y;
        int ww = win->w;
        int wh = win->h;

        /* Window shadow */
        waxl_fill_rect(c, wx + 4, wy + 4, ww, wh, waxl_color(0, 0, 0));

        /* Window border */
        uint32_t border_color = (win->id == c->focused_id)
            ? waxl_color(0x4A, 0x90, 0xD9)
            : waxl_color(0x66, 0x66, 0x66);
        waxl_fill_rect(c, wx, wy, ww, wh, border_color);

        /* Title bar */
        uint32_t title_bg = (win->id == c->focused_id)
            ? waxl_color(0x2D, 0x2D, 0x2D)
            : waxl_color(0x1A, 0x1A, 0x1A);
        waxl_fill_rect(c, wx + WAXL_BORDER_W, wy + WAXL_BORDER_W,
                       ww - WAXL_BORDER_W * 2, WAXL_TITLEBAR_H, title_bg);

        /* Title text */
        waxl_draw_string(c, wx + 8, wy + 8, win->title, waxl_color(0xCC, 0xCC, 0xCC));

        /* Close button */
        uint32_t close_color = waxl_color(0xE8, 0x1E, 0x1E);
        int cx = wx + ww - 20;
        int cy = wy + 8;
        waxl_draw_line(c, cx, cy, cx + 8, cy + 8, close_color);
        waxl_draw_line(c, cx + 8, cy, cx, cy + 8, close_color);

        /* Client area background */
        int client_x = wx + WAXL_BORDER_W;
        int client_y = wy + WAXL_BORDER_W + WAXL_TITLEBAR_H;
        int client_w = ww - WAXL_BORDER_W * 2;
        int client_h = wh - WAXL_BORDER_W * 2 - WAXL_TITLEBAR_H;
        waxl_fill_rect(c, client_x, client_y, client_w, client_h, win->color);

        /* If window has client pixel buffer, blit it */
        if (win->pixels && win->pix_size > 0) {
            uint32_t screen_pitch = c->drm.pitch / 4;
            for (int py = 0; py < client_h; py++) {
                for (int px = 0; px < client_w; px++) {
                    int sx = client_x + px;
                    int sy = client_y + py;
                    if (sx >= 0 && sy >= 0 && (uint32_t)sx < c->drm.width && (uint32_t)sy < c->drm.height) {
                        c->drm.pixels[sy * screen_pitch + sx] = win->pixels[py * client_w + px];
                    }
                }
            }
        }
    }

    /* Mouse cursor */
    int mx = c->mouse_x;
    int my = c->mouse_y;
    uint32_t cursor_color = waxl_color(0xFF, 0xFF, 0xFF);
    /* Simple arrow cursor */
    waxl_fill_rect(c, mx, my, 2, 14, cursor_color);
    waxl_fill_rect(c, mx, my, 10, 2, cursor_color);
    waxl_draw_line(c, mx, my, mx + 8, my + 12, cursor_color);
    waxl_draw_line(c, mx + 2, my + 10, mx + 6, my + 14, cursor_color);
}

/* === Input handling === */

static int waxl_open_evdev(const char *name_hint) {
    char path[64];
    for (int i = 0; i < 32; i++) {
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        int fd = open(path, O_RDWR | O_NONBLOCK);
        if (fd < 0) continue;
        char evname[256] = {0};
        ioctl(fd, EVIOCGNAME(sizeof(evname)), evname);
        if (name_hint == NULL || strstr(evname, name_hint) != NULL) {
            return fd;
        }
        close(fd);
    }
    return -1;
}

static void waxl_handle_mouse(waxl_comp_t *c, int dx, int dy, int btn, int pressed) {
    /* Update position from relative motion */
    if (dx != 0 || dy != 0) {
        c->mouse_x += dx;
        c->mouse_y += dy;
        if (c->mouse_x < 0) c->mouse_x = 0;
        if (c->mouse_y < 0) c->mouse_y = 0;
        if ((uint32_t)c->mouse_x >= c->drm.width) c->mouse_x = c->drm.width - 1;
        if ((uint32_t)c->mouse_y >= c->drm.height) c->mouse_y = c->drm.height - 1;
    }

    /* Handle button events */
    if (btn == 0) { /* left button */
        if (pressed && !c->mouse_down) {
            c->mouse_down = true;
            /* Find topmost window under cursor */
            waxl_window_t *hit = NULL;
            for (int i = (int)c->window_count - 1; i >= 0; i--) {
                waxl_window_t *w = &c->windows[i];
                if (!w->visible) continue;
                if (c->mouse_x >= w->x && c->mouse_x < (int)(w->x + w->w) &&
                    c->mouse_y >= w->y && c->mouse_y < (int)(w->y + w->h)) {
                    hit = w;
                    break;
                }
            }
            if (hit) {
                c->focused_id = hit->id;
                waxl_raise_window(c, hit->id);
                /* Check close button */
                int cx = hit->x + hit->w - 20;
                int cy = hit->y + 8;
                if (c->mouse_x >= cx && c->mouse_x < cx + 12 &&
                    c->mouse_y >= cy && c->mouse_y < cy + 12) {
                    waxl_destroy_window(c, hit->id);
                    if (c->window_count > 0)
                        c->focused_id = c->windows[c->window_count - 1].id;
                    else
                        c->focused_id = 0;
                } else if (c->mouse_y < hit->y + WAXL_TITLEBAR_H + WAXL_BORDER_W) {
                    /* Drag from title bar */
                    c->drag_win = hit->id;
                    c->drag_off_x = c->mouse_x - hit->x;
                    c->drag_off_y = c->mouse_y - hit->y;
                }
            }
        } else if (!pressed && c->mouse_down) {
            c->mouse_down = false;
            c->drag_win = 0;
        }
    }

    /* Dragging */
    if (c->mouse_down && c->drag_win != 0) {
        waxl_window_t *w = waxl_find_window(c, c->drag_win);
        if (w) {
            w->x = c->mouse_x - c->drag_off_x;
            w->y = c->mouse_y - c->drag_off_y;
        }
    }
}

/* === Socket / Protocol === */

static int waxl_setup_socket(waxl_comp_t *c) {
    unlink(WAXL_SOCKET_PATH);
    c->sock_fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (c->sock_fd < 0) return -1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, WAXL_SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(c->sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(c->sock_fd);
        return -1;
    }
    listen(c->sock_fd, 8);

    c->epoll_fd = epoll_create1(0);
    struct epoll_event ev = { .events = EPOLLIN, .data.fd = c->sock_fd };
    epoll_ctl(c->epoll_fd, EPOLL_CTL_ADD, c->sock_fd, &ev);

    return 0;
}

static void waxl_handle_wm_msg(waxl_comp_t *c, int fd) {
    waxl_msg_t msg;
    ssize_t n = recv(fd, &msg, sizeof(msg), 0);
    if (n <= 0) {
        epoll_ctl(c->epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        return;
    }

    pthread_mutex_lock(&c->lock);
    switch (msg.type) {
        case WAXL_MSG_CREATE_WINDOW: {
            waxl_window_t *w = waxl_create_window(c, msg.win_id, msg.x, msg.y, msg.w, msg.h, msg.color);
            if (w) {
                strncpy(w->title, msg.title, sizeof(w->title) - 1);
                c->focused_id = msg.win_id;
            }
            break;
        }
        case WAXL_MSG_DESTROY_WINDOW:
            waxl_destroy_window(c, msg.win_id);
            break;
        case WAXL_MSG_MOVE_WINDOW: {
            waxl_window_t *w = waxl_find_window(c, msg.win_id);
            if (w) { w->x = msg.x; w->y = msg.y; }
            break;
        }
        case WAXL_MSG_RESIZE_WINDOW: {
            waxl_window_t *w = waxl_find_window(c, msg.win_id);
            if (w) { w->w = msg.w; w->h = msg.h; }
            break;
        }
        case WAXL_MSG_RAISE_WINDOW:
            waxl_raise_window(c, msg.win_id);
            break;
        case WAXL_MSG_SET_TITLE: {
            waxl_window_t *w = waxl_find_window(c, msg.win_id);
            if (w) strncpy(w->title, msg.title, sizeof(w->title) - 1);
            break;
        }
        case WAXL_MSG_GET_FB: {
            /* Client sends shm name after the msg */
            char shm_name[64] = {0};
            ssize_t name_len = recv(fd, shm_name, sizeof(shm_name) - 1, 0);
            if (name_len > 0) {
                waxl_window_t *w = waxl_find_window(c, msg.win_id);
                if (w) {
                    /* Clean up old fb if any */
                    if (w->pixels && w->pixels != MAP_FAILED) {
                        munmap(w->pixels, w->pix_size);
                        w->pixels = NULL;
                    }
                    if (w->shm_fd >= 0) {
                        close(w->shm_fd);
                        w->shm_fd = -1;
                    }
                    /* Open client's shm */
                    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
                    if (shm_fd >= 0) {
                        uint32_t cw = w->w > 4 ? w->w - 4 : 1;
                        uint32_t ch = w->h > 26 ? w->h - 26 : 1;
                        size_t size = cw * ch * sizeof(uint32_t);
                        w->shm_fd = shm_fd;
                        w->pix_size = size;
                        w->pixels = mmap(NULL, size, PROT_READ, MAP_SHARED, shm_fd, 0);
                        if (w->pixels == MAP_FAILED) {
                            w->pixels = NULL;
                            close(shm_fd);
                            w->shm_fd = -1;
                        }
                    }
                }
            }
            break;
        }
        case WAXL_MSG_PRESENT: {
            /* Nothing special needed for dumb buffer - next compose will pick up pixels */
            break;
        }
        case WAXL_MSG_QUIT:
            c->running = 0;
            break;
    }
    pthread_mutex_unlock(&c->lock);
}

static void waxl_poll_wm(waxl_comp_t *c) {
    struct epoll_event events[WAXL_EPOLL_MAX];
    int nfds = epoll_wait(c->epoll_fd, events, WAXL_EPOLL_MAX, 0);
    for (int i = 0; i < nfds; i++) {
        if (events[i].data.fd == c->sock_fd) {
            int client = accept4(c->sock_fd, NULL, NULL, SOCK_NONBLOCK);
            if (client >= 0) {
                struct epoll_event ev = { .events = EPOLLIN, .data.fd = client };
                epoll_ctl(c->epoll_fd, EPOLL_CTL_ADD, client, &ev);
            }
        } else {
            waxl_handle_wm_msg(c, events[i].data.fd);
        }
    }
}

/* === Main loop === */

static volatile int g_running = 1;
static void sigint_handler(int sig) { (void)sig; g_running = 0; }

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    memset(&g_comp, 0, sizeof(g_comp));
    pthread_mutex_init(&g_comp.lock, NULL);
    g_comp.bg_color = waxl_color(0x1E, 0x1E, 0x1E);
    g_comp.running = 1;

    const char *device = getenv("WAXL_DRM_DEVICE");
    if (!lidrm_init(&g_comp.drm, device)) {
        fprintf(stderr, "[waxl-comp] Failed to initialize DRM\n");
        return 1;
    }

    /* Initialize mouse to center of screen */
    g_comp.mouse_x = g_comp.drm.width / 2;
    g_comp.mouse_y = g_comp.drm.height / 2;

    if (waxl_setup_socket(&g_comp) < 0) {
        fprintf(stderr, "[waxl-comp] Failed to setup socket\n");
        lidrm_cleanup(&g_comp.drm);
        return 1;
    }

    /* Open input devices - do NOT add to epoll, read directly */
    int mouse_fd = waxl_open_evdev("Mouse");
    if (mouse_fd < 0) mouse_fd = waxl_open_evdev(NULL);
    int kbd_fd = waxl_open_evdev("Keyboard");
    if (kbd_fd < 0) kbd_fd = waxl_open_evdev(NULL);

    printf("[waxl-comp] Running on %ux%u\n", g_comp.drm.width, g_comp.drm.height);
    printf("[waxl-comp] Socket: %s\n", WAXL_SOCKET_PATH);
    printf("[waxl-comp] Mouse: %s, Keyboard: %s\n",
           mouse_fd >= 0 ? "OK" : "FAIL",
           kbd_fd >= 0 ? "OK" : "FAIL");

    struct input_event ev;
    while (g_running && g_comp.running) {
        waxl_poll_wm(&g_comp);

        /* Poll mouse input directly (not via epoll) */
        if (mouse_fd >= 0) {
            while (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
                if (ev.type == EV_REL) {
                    if (ev.code == REL_X) waxl_handle_mouse(&g_comp, ev.value, 0, -1, 0);
                    if (ev.code == REL_Y) waxl_handle_mouse(&g_comp, 0, ev.value, -1, 0);
                } else if (ev.type == EV_KEY) {
                    if (ev.code == BTN_LEFT) waxl_handle_mouse(&g_comp, 0, 0, 0, ev.value);
                }
            }
        }

        /* Poll keyboard input directly */
        if (kbd_fd >= 0) {
            while (read(kbd_fd, &ev, sizeof(ev)) == sizeof(ev)) {
                if (ev.type == EV_KEY && (ev.value == 1 || ev.value == 2)) {
                    waxl_msg_t msg = {
                        .type = WAXL_MSG_INPUT,
                        .win_id = g_comp.focused_id,
                        .x = ev.code,
                        .y = ev.value,
                    };
                }
            }
        }

        pthread_mutex_lock(&g_comp.lock);
        waxl_compose(&g_comp);
        pthread_mutex_unlock(&g_comp.lock);

        lidrm_flush(&g_comp.drm);
        usleep(16666); /* ~60fps */
    }

    printf("[waxl-comp] Shutting down...\n");

    if (mouse_fd >= 0) close(mouse_fd);
    if (kbd_fd >= 0) close(kbd_fd);
    close(g_comp.epoll_fd);
    close(g_comp.sock_fd);
    unlink(WAXL_SOCKET_PATH);
    lidrm_cleanup(&g_comp.drm);
    pthread_mutex_destroy(&g_comp.lock);

    return 0;
}
