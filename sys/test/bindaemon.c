#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libinder.h>

#define CMD_PING 0x100

static int daemon_handler(uint32_t code, const void *in_data, size_t in_len,
                          void *out_buf, size_t *out_len, void *user_data) {
    (void)user_data;

    printf("[DAEMON] Received transaction code: 0x%X\n", code);

    if (code == CMD_PING) {
        printf("[DAEMON] Client message: \"%s\"\n", (const char *)in_data);

        const char reply[] = "PONG from Binder Daemon!";
        size_t reply_len = sizeof(reply);

        if (*out_len >= reply_len) {
            memcpy(out_buf, reply, reply_len);
            *out_len = reply_len;
            printf("[DAEMON] Replied with: \"%s\"\n", reply);
            return 0;
        } else {
            printf("[DAEMON] Output buffer too small!\n");
            return -1;
        }
    }

    printf("[DAEMON] Unknown command code!\n");
    return -1;
}

int main(void) {
    // Open binder node (passing NULL defaults to /dev/binder or /dev/binderfs/binder)
    libinder_t *b = libinder_open("/dev/binderfs/binder", 0);
    if (!b) {
        perror("[DAEMON] Failed to open binder device");
        return 1;
    }

    // Become context manager (handle 0)
    if (libinder_set_context_manager(b) < 0) {
        perror("[DAEMON] Failed to set context manager");
        libinder_close(b);
        return 1;
    }

    printf("[DAEMON] Registered as Context Manager (Handle 0). Listening...\n");

    // Enter processing loop
    libinder_loop(b, daemon_handler, NULL);

    libinder_close(b);
    return 0;
}
