#include <lidrm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/types.h>

#include <vendor/linux/drm/drm.h>
#include <vendor/linux/drm/drm_mode.h>
  
#define DRM_MODE_CONNECTED 1

bool lidrm_init(lidrm_t *drm, const char *device_path) {
    if (!drm) return false;
    memset(drm, 0, sizeof(lidrm_t));

    const char *path = device_path ? device_path : "/dev/dri/card0";
    drm->fd = open(path, O_RDWR | O_CLOEXEC);
    if (drm->fd < 0) {
        perror("[lidrm] Failed to open DRM device");
        return false;
    }

    ioctl(drm->fd, DRM_IOCTL_SET_MASTER, 0);

    struct drm_mode_card_res res = {0};
    if (ioctl(drm->fd, DRM_IOCTL_MODE_GETRESOURCES, &res) < 0) {
        perror("[lidrm] GETRESOURCES failed");
        close(drm->fd);
        return false;
    }

    uint32_t *conn_ids = calloc(res.count_connectors, sizeof(uint32_t));
    uint32_t *crtc_ids = calloc(res.count_crtcs, sizeof(uint32_t));

    res.connector_id_ptr = (uint64_t)(uintptr_t)conn_ids;
    res.crtc_id_ptr = (uint64_t)(uintptr_t)crtc_ids;

    if (ioctl(drm->fd, DRM_IOCTL_MODE_GETRESOURCES, &res) < 0) {
        perror("[lidrm] GETRESOURCES populated query failed");
        free(conn_ids); free(crtc_ids);
        close(drm->fd);
        return false;
    }

    struct drm_mode_modeinfo selected_mode = {0};
    bool found_connector = false;

    for (uint32_t i = 0; i < res.count_connectors; i++) {
        struct drm_mode_get_connector conn = {
            .connector_id = conn_ids[i]
        };

        if (ioctl(drm->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn) < 0) continue;

        if (conn.connection == DRM_MODE_CONNECTED && conn.count_modes > 0) {
            struct drm_mode_modeinfo *modes = calloc(conn.count_modes, sizeof(struct drm_mode_modeinfo));
            conn.modes_ptr = (uint64_t)(uintptr_t)modes;

            if (ioctl(drm->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn) == 0) {
                selected_mode = modes[0];
                drm->connector_id = conn.connector_id;
                drm->width = selected_mode.hdisplay;
                drm->height = selected_mode.vdisplay;
                found_connector = true;
            }
            free(modes);
            if (found_connector) break;
        }
    }

    if (found_connector && res.count_crtcs > 0) {
        drm->crtc_id = crtc_ids[0];
    }

    free(conn_ids);
    free(crtc_ids);

    if (!found_connector) {
        fprintf(stderr, "[lidrm] No active connected display found\n");
        close(drm->fd);
        return false;
    }

    struct drm_mode_crtc saved_crtc = { .crtc_id = drm->crtc_id };
    if (ioctl(drm->fd, DRM_IOCTL_MODE_GETCRTC, &saved_crtc) == 0) {
        drm->saved_fb_id = saved_crtc.fb_id;
        drm->saved_x = saved_crtc.x;
        drm->saved_y = saved_crtc.y;
        drm->saved_mode_valid = saved_crtc.mode_valid;
        if (saved_crtc.mode_valid) {
            memcpy(drm->saved_mode_blob, &saved_crtc.mode, sizeof(struct drm_mode_modeinfo));
        }
    }

    struct drm_mode_create_dumb creq = {
        .width = drm->width,
        .height = drm->height,
        .bpp = 32
    };

    if (ioctl(drm->fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
        perror("[lidrm] CREATE_DUMB failed");
        close(drm->fd);
        return false;
    }

    drm->handle = creq.handle;
    drm->pitch = creq.pitch;
    drm->size = creq.size;

    struct drm_mode_fb_cmd cmd = {
        .width = drm->width,
        .height = drm->height,
        .pitch = drm->pitch,
        .bpp = 32,
        .depth = 24,
        .handle = drm->handle
    };

    if (ioctl(drm->fd, DRM_IOCTL_MODE_ADDFB, &cmd) < 0) {
        perror("[lidrm] ADDFB failed");
        goto err_destroy_dumb;
    }
    drm->fb_id = cmd.fb_id;

    struct drm_mode_map_dumb mreq = { .handle = drm->handle };
    if (ioctl(drm->fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq) < 0) {
        perror("[lidrm] MAP_DUMB failed");
        goto err_rm_fb;
    }

    drm->pixels = (uint32_t *)mmap(0, drm->size, PROT_READ | PROT_WRITE, MAP_SHARED, drm->fd, mreq.offset);
    if (drm->pixels == MAP_FAILED) {
        perror("[lidrm] mmap failed");
        drm->pixels = NULL;
        goto err_rm_fb;
    }

    struct drm_mode_crtc crtc = {
        .crtc_id = drm->crtc_id,
        .fb_id = drm->fb_id,
        .set_connectors_ptr = (uint64_t)(uintptr_t)&drm->connector_id,
        .count_connectors = 1,
        .mode = selected_mode,
        .mode_valid = 1
    };

    if (ioctl(drm->fd, DRM_IOCTL_MODE_SETCRTC, &crtc) < 0) {
        perror("[lidrm] SETCRTC failed");
        goto err_unmap;
    }

    printf("[lidrm] Initialized display %ux%u on CRTC %u\n", drm->width, drm->height, drm->crtc_id);
    return true;

err_unmap:
    munmap(drm->pixels, drm->size);
err_rm_fb:
    ioctl(drm->fd, DRM_IOCTL_MODE_RMFB, &drm->fb_id);
err_destroy_dumb:
    {
        struct drm_mode_destroy_dumb dreq = { .handle = drm->handle };
        ioctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
    }
    close(drm->fd);
    return false;
}

void lidrm_clear(lidrm_t *drm, uint32_t color) {
    if (!drm || !drm->pixels) return;
    for (size_t i = 0; i < drm->size / sizeof(uint32_t); i++) {
        drm->pixels[i] = color;
    }
}

void lidrm_cleanup(lidrm_t *drm) {
    if (!drm || drm->fd <= 0) return;

    if (drm->saved_mode_valid) {
        struct drm_mode_crtc crtc = {
            .crtc_id = drm->crtc_id,
            .fb_id = drm->saved_fb_id,
            .x = drm->saved_x,
            .y = drm->saved_y,
            .set_connectors_ptr = (uint64_t)(uintptr_t)&drm->connector_id,
            .count_connectors = 1,
            .mode_valid = 1
        };
        memcpy(&crtc.mode, drm->saved_mode_blob, sizeof(struct drm_mode_modeinfo));
        ioctl(drm->fd, DRM_IOCTL_MODE_SETCRTC, &crtc);
    }

    if (drm->pixels) {
        munmap(drm->pixels, drm->size);
    }
    if (drm->fb_id) {
        ioctl(drm->fd, DRM_IOCTL_MODE_RMFB, &drm->fb_id);
    }
    if (drm->handle) {
        struct drm_mode_destroy_dumb dreq = { .handle = drm->handle };
        ioctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
    }

    ioctl(drm->fd, DRM_IOCTL_DROP_MASTER, 0);
    close(drm->fd);
    memset(drm, 0, sizeof(lidrm_t));
}
