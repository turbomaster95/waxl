#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libinder.h>
#include <common.h>

typedef int (*transaction_cb)(const void *in_data, size_t in_len, void *out_buf, size_t *out_len);

typedef struct {
    uint32_t code;
    const char *name;
    transaction_cb callback;
} route_t;

static int handle_ping(const void *in, size_t in_len, void *out, size_t *out_len) {
    const char *reply = "PONG_ACK";
    if (*out_len < strlen(reply) + 1) return -ENOMEM;
    strcpy(out, reply);
    *out_len = strlen(reply) + 1;
    return 0;
}

static int handle_echo(const void *in, size_t in_len, void *out, size_t *out_len) {
    if (*out_len < in_len) return -ENOMEM;
    memcpy(out, in, in_len);
    *out_len = in_len;
    return 0;
}

static const route_t ROUTES[] = {
    { 101, "PING", handle_ping },
    { 102, "ECHO", handle_echo },
    // { 103, "CUSTOM_CMD", handle_custom },
};

static int universal_handler(uint32_t code, const void *in_data, size_t in_len,
                             void *out_buf, size_t *out_len, void *user_data) {
    const char *node_name = (const char *)user_data;

    for (size_t i = 0; i < sizeof(ROUTES) / sizeof(ROUTES[0]); i++) {
        if (ROUTES[i].code == code) {
            NL_INFO("[%s] Executing route: %s (Code: %u)", node_name, ROUTES[i].name, code);
            return ROUTES[i].callback(in_data, in_len, out_buf, out_len);
        }
    }

    NL_INFO("[%s] Unknown transaction code: %u", node_name, code);
    return -EINVAL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        NL_ERROR("Usage: %s <path_to_binder_node> (e.g. /dev/binderfs/stdbinder)", argv[0]);
        return 1;
    }

    const char *node_path = argv[1];

    libinder_t *binder = libinder_open(node_path, 0);
    if (!binder) {
        NL_ERROR("libinder_open failed");
        return 1;
    }

    if (libinder_set_context_manager(binder) < 0) {
        NL_ERROR("Failed to set context manager");
        libinder_close(binder);
        return 1;
    }

    NL_INFO("Binder Orchestrator active on %s.", node_path);
    int ret = libinder_loop(binder, universal_handler, (void *)node_path);

    libinder_close(binder);
    return ret;
}
