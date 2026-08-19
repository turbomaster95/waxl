#include <stdio.h>
#include <string.h>
#include "libinder.h"

#define CMD_PING 101
#define CMD_ECHO 102

int main(void) {
    libinder_t *b = libinder_open("/dev/binderfs/stdbinder", 0);
    if (!b) {
        perror("[CLIENT] Failed to open binder device");
        return 1;
    }

    const char ping_msg[] = "PING from Binder Client!";
    char resp[128] = {0};
    size_t resp_len = sizeof(resp);

    printf("[CLIENT] Sending PING code %d with msg: \"%s\"\n", CMD_PING, ping_msg);

    int res = libinder_call(b, 0, CMD_PING, ping_msg, sizeof(ping_msg), resp, &resp_len);
    if (res == 0) {
        printf("[CLIENT] PING Success! Received response: \"%s\" (%zu bytes)\n", resp, resp_len);
    } else {
        printf("[CLIENT] PING Call failed!\n");
    }

    const char echo_msg[] = "Hello Echo Server!";
    memset(resp, 0, sizeof(resp));
    resp_len = sizeof(resp);

    printf("[CLIENT] Sending ECHO code %d with msg: \"%s\"\n", CMD_ECHO, echo_msg);

    res = libinder_call(b, 0, CMD_ECHO, echo_msg, sizeof(echo_msg), resp, &resp_len);
    if (res == 0) {
        printf("[CLIENT] ECHO Success! Received echo: \"%s\" (%zu bytes)\n", resp, resp_len);
    } else {
        printf("[CLIENT] ECHO Call failed!\n");
    }

    libinder_close(b);
    return 0;
}
