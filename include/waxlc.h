#ifndef WAXL_CLIENT_H
#define WAXL_CLIENT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new window. Returns window ID or 0 on failure.
 * title: window title
 * w, h:  window dimensions (including decorations)
 * color: default background color (ARGB) */
uint32_t waxl_wm_create_window(const char *title, uint32_t w, uint32_t h, uint32_t color);

/* Destroy a window and free its resources. */
void waxl_wm_destroy_window(uint32_t win_id);

/* Move window to (x, y). */
void waxl_wm_move_window(uint32_t win_id, int x, int y);

/* Resize window. */
void waxl_wm_resize_window(uint32_t win_id, uint32_t w, uint32_t h);

/* Set window title. */
void waxl_wm_set_title(uint32_t win_id, const char *title);

/* Raise window to top. */
void waxl_wm_raise_window(uint32_t win_id);

/* Get the framebuffer for drawing.
 * Returns a pointer to an ARGB8888 buffer of size (*out_w) * (*out_h).
 * The buffer is backed by shared memory with the compositor.
 * Call waxl_wm_present() after drawing to show changes.
 * Returns NULL on failure. */
uint32_t* waxl_wm_get_framebuffer(uint32_t win_id, uint32_t *out_w, uint32_t *out_h);

/* Present window contents to the screen. */
void waxl_wm_present(uint32_t win_id);

/* Quit the entire session (sends quit to compositor). */
void waxl_wm_quit(void);

/* Color helper: pack R,G,B into ARGB8888 */
static inline uint32_t waxl_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return 0xFF000000 | ((b << 16) | (g << 8) | r);
}

#ifdef __cplusplus
}
#endif

#endif /* WAXL_CLIENT_H */
