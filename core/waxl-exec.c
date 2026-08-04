#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/run/incubator.sock"
#define BUFFER_SIZE 4096
#define TARGET_SO "lib/app.so"

#pragma pack(push, 1)
typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
} tar_header_t;
#pragma pack(pop)

static uint64_t octal_to_uint(const char *str, size_t len) {
    uint64_t result = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '7') continue;
        result = (result << 3) + (str[i] - '0');
    }
    return result;
}

static int extract_so_to_memfd(const char *tar_path) {
    int fd = open(tar_path, O_RDONLY);
    if (fd < 0) {
        perror("[waxl-exec] Failed to open .wax package");
        return -1;
    }

    tar_header_t header;
    int memfd = -1;

    while (read(fd, &header, sizeof(tar_header_t)) == sizeof(tar_header_t)) {
        if (header.name[0] == '\0') break;

        uint64_t file_size = octal_to_uint(header.size, sizeof(header.size));

        if (strcmp(header.name, TARGET_SO) == 0 || strcmp(header.name, "./" TARGET_SO) == 0) {
            memfd = memfd_create("wax_app_so", MFD_CLOEXEC);
            if (memfd < 0) {
                perror("[waxl-exec] memfd_create failed");
                close(fd);
                return -1;
            }

            char chunk[8192];
            uint64_t bytes_left = file_size;

            while (bytes_left > 0) {
                uint64_t to_read = (bytes_left < sizeof(chunk)) ? bytes_left : sizeof(chunk);
                ssize_t nread = read(fd, chunk, to_read);
                if (nread <= 0) break;

                if (write(memfd, chunk, nread) != nread) {
                    perror("[waxl-exec] Failed writing to memfd");
                    close(memfd);
                    close(fd);
                    return -1;
                }
                bytes_left -= nread;
            }
            break;
        } else {
            uint64_t padded_size = (file_size + 511) & ~511;
            lseek(fd, padded_size, SEEK_CUR);
        }
    }

    close(fd);
    return memfd;
}

static int send_fd_and_args(int sock, int fd_to_send, int argc, char **argv) {
    char payload[BUFFER_SIZE] = {0};
    int len = 0;

    for (int i = 2; i < argc; i++) {
        int written = snprintf(payload + len, BUFFER_SIZE - len, "%s", argv[i]);
        if (written < 0 || len + written >= BUFFER_SIZE - 1) break;
        len += written;
        payload[len++] = '\0';
    }

    struct msghdr msg = {0};
    struct iovec iov[1];

    iov[0].iov_base = payload;
    iov[0].iov_len = (len > 0) ? len : 1;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    char cmsgbuf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(int));

    if (sendmsg(sock, &msg, 0) < 0) {
        perror("[waxl-exec] sendmsg failed");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: waxl-exec <path-to-wax-file> [args...]\n");
        return 1;
    }

    int memfd = extract_so_to_memfd(argv[1]);
    if (memfd < 0) {
        fprintf(stderr, "[waxl-exec] Failed to extract target library from %s\n", argv[1]);
        return 1;
    }

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[waxl-exec] Socket creation failed");
        close(memfd);
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("[waxl-exec] Failed to connect to incubator daemon");
        close(sock);
        close(memfd);
        return 1;
    }

    if (send_fd_and_args(sock, memfd, argc, argv) < 0) {
        close(sock);
        close(memfd);
        return 1;
    }

    char relay_buf[1024];
    ssize_t nread;
    while ((nread = read(sock, relay_buf, sizeof(relay_buf))) > 0) {
        write(STDOUT_FILENO, relay_buf, nread);
    }

    close(sock);
    close(memfd);
    return 0;
}
