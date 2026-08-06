#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>

#define WAXL_SOCKET_PATH "/tmp/waxl-comp.sock"

typedef struct {
    uint32_t type;
    uint32_t surface_id;
    int32_t  x, y;
    uint32_t width, height;
} waxl_msg_t;

int receive_fd(int socket_fd, uint32_t *surface_id) {
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];

    struct iovec io = { .iov_base = surface_id, .iov_len = sizeof(*surface_id) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg(socket_fd, &msg, 0) < 0) return -1;

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (!cmsg || cmsg->cmsg_type != SCM_RIGHTS) return -1;

    int fd;
    memcpy(&fd, CMSG_DATA(cmsg), sizeof(int));
    return fd;
}

int main(void) {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un sun = { .sun_family = AF_UNIX };
    strncpy(sun.sun_path, WAXL_SOCKET_PATH, sizeof(sun.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&sun, sizeof(sun)) < 0) {
        perror("Connect failed");
        return 1;
    }

    uint32_t w = 320, h = 240;
    waxl_msg_t req = { .type = 1, .x = 100, .y = 100, .width = w, .height = h };
    write(sock, &req, sizeof(req));

    uint32_t surf_id = 0;
    int shm_fd = receive_fd(sock, &surf_id);
    printf("Acquired SHM FD %d for Surface ID %u\n", shm_fd, surf_id);

    uint32_t *buf = mmap(NULL, w * h * 4, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    for (uint32_t i = 0; i < w * h; i++) {
        buf[i] = 0xFF00FF00; // Semi-transparent / opaque green
    }

    waxl_msg_t commit = { .type = 2, .surface_id = surf_id };
    write(sock, &commit, sizeof(commit));

    sleep(5);
    close(sock);
    return 0;
}
