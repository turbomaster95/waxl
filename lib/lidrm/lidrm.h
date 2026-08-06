#ifndef LIDRM_H
#define LIDRM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <drm/drm_mode.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int fd;
    uint32_t connector_id;
    uint32_t crtc_id;
    uint32_t width;
    uint32_t height;

    // Active mode timings
    struct drm_mode_modeinfo mode;

    // Framebuffer state
    uint32_t fb_id;
    uint32_t handle;
    uint32_t pitch;
    uint64_t size;
    uint32_t *pixels; // ARGB8888 / XRGB8888 mapped buffer

    uint32_t saved_fb_id;
    uint32_t saved_x, saved_y;
    uint32_t saved_mode_valid;
    uint8_t saved_mode_blob[512];
} lidrm_t;

bool lidrm_init(lidrm_t *drm, const char *device_path);
bool lidrm_set_mode(lidrm_t *drm);
void lidrm_clear(lidrm_t *drm, uint32_t color);
void lidrm_cleanup(lidrm_t *drm);

#ifdef __cplusplus
}
#endif

#endif // LIDRM_H
