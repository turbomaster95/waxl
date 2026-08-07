#include <stdio.h>
#include <string.h>
#include <libinder.h>

#define CMD_PING 0x100

int main(void) {
    libinder_t *b = libinder_open("/dev/binderfs/binder", 0);
    if (!b) {
        perror("[CLIENT] Failed to open binder device");
        return 1;
    }

    const char msg[] = "PING from Binder Client!";
    char resp[128] = {0};
    size_t resp_len = sizeof(resp);

    printf("[CLIENT] Sending message: \"%s\"\n", msg);

    // Call handle 0 (Daemon)
    int res = libinder_call(b, 0, CMD_PING, msg, sizeof(msg), resp, &resp_len);
    if (res == 0) {
        printf("[CLIENT] Success! Received response: \"%s\" (%zu bytes)\n", resp, resp_len);
    } else {
        printf("[CLIENT] Call failed!\n");
    }

    libinder_close(b);
    return 0;
}
