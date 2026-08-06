#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "lib/lidrm/lidrm.h"

static volatile bool running = true;

static void handle_signal(int sig) {
    (void)sig;
    running = false;
}

// Helper: Fill a rectangle on the framebuffer with clipping
static void draw_rect(lidrm_t *drm, int rx, int ry, int rw, int rh, uint32_t color) {
    int x1 = rx < 0 ? 0 : rx;
    int y1 = ry < 0 ? 0 : ry;
    int x2 = (rx + rw > (int)drm->width) ? (int)drm->width : (rx + rw);
    int y2 = (ry + rh > (int)drm->height) ? (int)drm->height : (ry + rh);

    for (int y = y1; y < y2; y++) {
        uint32_t *row = drm->pixels + (y * (drm->pitch / 4));
        for (int x = x1; x < x2; x++) {
            row[x] = color;
        }
    }
}

// Helper: Render a simulated window with title bar and border
static void draw_window(lidrm_t *drm, int x, int y, int w, int h, const char *title, uint32_t title_color) {
    (void)title;
    // Window Border
    draw_rect(drm, x - 2, y - 2, w + 4, h + 4, 0xFF555555);
    // Window Titlebar
    draw_rect(drm, x, y, w, 24, title_color);
    // Window Client Area
    draw_rect(drm, x, y + 24, w, h - 24, 0xFF1E1E1E);
}

int main(int argc, char *argv[]) {
    const char *dev_path = (argc > 1) ? argv[1] : "/dev/dri/card0";
    lidrm_t drm = {0};

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("[waxl-compositor] Initializing DRM on %s...\n", dev_path);
    if (!lidrm_init(&drm, dev_path)) {
        fprintf(stderr, "[waxl-compositor] Failed to initialize DRM!\n");
        return EXIT_FAILURE;
    }

    printf("[waxl-compositor] DRM Mode set: %ux%u\n", drm.width, drm.height);

    // Initial display mode setup
    if (!lidrm_set_mode(&drm)) {
        fprintf(stderr, "[waxl-compositor] Failed to apply CRTC mode!\n");
        lidrm_cleanup(&drm);
        return EXIT_FAILURE;
    }

    // Compositor state: Simulated window bounds & velocity
    int win_w = 480;
    int win_h = 320;
    int win_x = (drm.width - win_w) / 2;
    int win_y = (drm.height - win_h) / 2;
    int dx = 4;
    int dy = 3;

    struct timespec target_time;
    clock_gettime(CLOCK_MONOTONIC, &target_time);

    printf("[waxl-compositor] Compositor running. Press Ctrl+C to exit.\n");

    while (running) {
        // 1. Clear background (WaxL Dark Charcoal)
        lidrm_clear(&drm, 0xFF121212);

        // 2. Draw Status Bar (Top panel)
        draw_rect(&drm, 0, 0, drm.width, 32, 0xFF282828);
        draw_rect(&drm, 0, 31, drm.width, 1, 0xFF3C3C3C); // Divider line

        // 3. Move and bounce window
        win_x += dx;
        win_y += dy;

        if (win_x <= 0 || (win_x + win_w) >= (int)drm.width) {
            dx = -dx;
            win_x += dx;
        }
        if (win_y <= 32 || (win_y + win_h) >= (int)drm.height) {
            dy = -dy;
            win_y += dy;
        }

        // 4. Render active window
        draw_window(&drm, win_x, win_y, win_w, win_h, "WaxL App Terminal", 0xFF0055AA);

        // 5. Commit frame to CRTC
        lidrm_set_mode(&drm);

        // 6. Frame pacing (~60 FPS / 16.6ms target delay)
        target_time.tv_nsec += 16666666;
        if (target_time.tv_nsec >= 1000000000) {
            target_time.tv_sec += 1;
            target_time.tv_nsec -= 1000000000;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &target_time, NULL);
    }

    printf("\n[waxl-compositor] Restoring original CRTC state and exiting...\n");
    lidrm_cleanup(&drm);
    return EXIT_SUCCESS;
}
