#ifndef WAXLB_H
#define WAXLB_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Waxl Binder IPC Protocol v2
 * 
 * Handle 0 = waxl-comp (context manager)
 * Handle 1 = waxl-wm (well-known, first non-CM client)
 */

// Comp transactions (handle 0)
#define WAXL_COMP_TXN_CREATE_SURFACE    1
#define WAXL_COMP_TXN_DESTROY_SURFACE   2
#define WAXL_COMP_TXN_ATTACH_BUFFER     3
#define WAXL_COMP_TXN_COMMIT_SURFACE    4
#define WAXL_COMP_TXN_SET_MODE          5   // x,y,w,h for a surface
#define WAXL_COMP_TXN_GET_DISPLAY_INFO  6
#define WAXL_COMP_TXN_REGISTER_WM       7   // wm -> comp: "I'm here"

// WM transactions (handle 1)  
#define WAXL_WM_TXN_CREATE_WINDOW       10
#define WAXL_WM_TXN_DESTROY_WINDOW      11
#define WAXL_WM_TXN_GET_BUFFER          12
#define WAXL_WM_TXN_COMMIT_BUFFER       13
#define WAXL_WM_TXN_SET_TITLE           14
#define WAXL_WM_TXN_SET_SIZE            15
#define WAXL_WM_TXN_RAISE_WINDOW        16
#define WAXL_WM_TXN_GET_EVENT           17
#define WAXL_WM_TXN_ACK_CLOSE           18
#define WAXL_WM_TXN_INPUT_EVENT         19   // comp -> wm forward

#define WAXL_MAX_TITLE_LEN      128
#define WAXL_MAX_SHM_NAME       64

typedef uint32_t waxl_id_t;
#define WAXL_INVALID_ID 0

#define WAXL_FORMAT_XRGB8888 0

// Create surface/window
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t flags;
    char title[WAXL_MAX_TITLE_LEN];
} waxl_create_req_t;

// Create response (includes pre-allocated shm)
typedef struct {
    waxl_id_t id;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t format;
    char shm_name[WAXL_MAX_SHM_NAME];
    uint32_t shm_size;
} waxl_create_resp_t;

// Set surface position/size (comp-side)
typedef struct {
    waxl_id_t surface_id;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
} waxl_set_mode_t;

// Display info
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t refresh_hz;
} waxl_display_info_t;

// Input event (comp -> wm -> app)
typedef enum {
    WAXL_EV_NONE = 0,
    WAXL_EV_POINTER_ENTER,
    WAXL_EV_POINTER_LEAVE, 
    WAXL_EV_POINTER_MOTION,
    WAXL_EV_POINTER_BUTTON,
    WAXL_EV_KEYBOARD_ENTER,
    WAXL_EV_KEYBOARD_LEAVE,
    WAXL_EV_KEYBOARD_KEY,
    WAXL_EV_CONFIGURE,
    WAXL_EV_CLOSE_REQUEST,
    WAXL_EV_PRESENT,
} waxl_event_type_t;

typedef struct {
    uint32_t type;
    waxl_id_t window_id;
    union {
        struct { int32_t x, y; } pointer;
        struct { uint32_t button; uint32_t state; int32_t x, y; } button;
        struct { uint32_t key; uint32_t state; } key;
        struct { uint32_t width, height; } configure;
    } data;
} waxl_app_event_t;

// WM -> comp: input forwarding
typedef struct {
    uint32_t ev_type;
    waxl_id_t surface_id;
    int32_t x, y;
    uint32_t button;
    uint32_t state;
    uint32_t key;
} waxl_input_forward_t;

// Config
typedef struct {
    char key[64];
    char value[192];
} waxl_config_entry_t;

#define WAXL_CONFIG_FILE "/.waxlc"
#define WAXL_CFG_AUTOSTART "autostart"

#ifdef __cplusplus
}
#endif

#endif
