#include <libinder.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/android/binder.h>

struct libinder {
    int fd;
    void *mapped;
    size_t mapped_size;
};

libinder_t *libinder_open(const char *devpath, size_t map_size) {
    if (map_size == 0) {
        map_size = 128 * 1024; // Default 128 KB
    }

    int fd = -1;
    if (devpath) {
        fd = open(devpath, O_RDWR | O_CLOEXEC);
    } else {
        fd = open("/dev/binder", O_RDWR | O_CLOEXEC);
        if (fd < 0) {
            fd = open("/dev/binderfs/binder", O_RDWR | O_CLOEXEC);
        }
    }

    if (fd < 0) return NULL;

    void *mapped = mmap(NULL, map_size, PROT_READ, MAP_PRIVATE | MAP_NORESERVE, fd, 0);
    if (mapped == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    libinder_t *b = calloc(1, sizeof(libinder_t));
    if (!b) {
        munmap(mapped, map_size);
        close(fd);
        return NULL;
    }

    b->fd = fd;
    b->mapped = mapped;
    b->mapped_size = map_size;

    return b;
}

int libinder_set_context_manager(libinder_t *b) {
    if (!b || b->fd < 0) return -1;
    int dummy = 0;
    return ioctl(b->fd, BINDER_SET_CONTEXT_MGR, &dummy);
}

int libinder_get_fd(const libinder_t *b)
{
    return b ? b->fd : -1;
}

int libinder_call(libinder_t *b, uint32_t handle, uint32_t code, 
                const void *in_data, size_t in_len, 
                void *out_data, size_t *out_len) {
    if (!b || b->fd < 0) return -1;

struct {
        uint32_t cmd;
        struct binder_transaction_data txn;
    } __attribute__((packed)) write_buf;

    memset(&write_buf, 0, sizeof(write_buf));
    write_buf.cmd = BC_TRANSACTION;
    write_buf.txn.target.handle = handle;
    write_buf.txn.code = code;
    write_buf.txn.flags = TF_ACCEPT_FDS;
    write_buf.txn.data_size = in_len;
    write_buf.txn.data.ptr.buffer = (binder_uintptr_t)in_data;

    uint8_t read_buf[512];
    struct binder_write_read bwr;
    memset(&bwr, 0, sizeof(bwr));

    bwr.write_buffer = (binder_uintptr_t)&write_buf;
    bwr.write_size = sizeof(write_buf);

    while (1) {
        bwr.read_buffer = (binder_uintptr_t)read_buf;
        bwr.read_size = sizeof(read_buf);
        bwr.read_consumed = 0;

        if (ioctl(b->fd, BINDER_WRITE_READ, &bwr) < 0) {
            if (errno == EINTR) continue;
            return -1;
        }

        bwr.write_size = 0;

        uint8_t *ptr = read_buf;
        uint8_t *end = read_buf + bwr.read_consumed;

        while (ptr < end) {
            uint32_t cmd = *(uint32_t *)ptr;
            ptr += sizeof(uint32_t);

            if (cmd == BR_REPLY) {
                struct binder_transaction_data *tr = (struct binder_transaction_data *)ptr;
                
                if (out_data && out_len) {
                    size_t copy_len = tr->data_size < *out_len ? tr->data_size : *out_len;
                    memcpy(out_data, (void *)tr->data.ptr.buffer, copy_len);
                    *out_len = copy_len;
                }

                struct {
                    uint32_t cmd;
                    binder_uintptr_t buffer;
                } __attribute__((packed)) free_pkt = {
                    .cmd = BC_FREE_BUFFER,
                    .buffer = tr->data.ptr.buffer
                };

                struct binder_write_read free_bwr = {0};
                free_bwr.write_buffer = (binder_uintptr_t)&free_pkt;
                free_bwr.write_size = sizeof(free_pkt);
                ioctl(b->fd, BINDER_WRITE_READ, &free_bwr);

                return 0;
            } else if (cmd == BR_TRANSACTION_COMPLETE || cmd == BR_OK || cmd == BR_NOOP) {
                continue;
            } else if (cmd == BR_ERROR || cmd == BR_DEAD_REPLY) {
                return -1;
            }
        }
    }
    return -1;
}

int libinder_loop(libinder_t *b, binder_handler_fn handler, void *user_data) {
    if (!b || b->fd < 0 || !handler) return -1;

    uint32_t enter_cmd = BC_ENTER_LOOPER;
    struct binder_write_read bwr = {0};
    bwr.write_buffer = (binder_uintptr_t)&enter_cmd;
    bwr.write_size = sizeof(enter_cmd);
    ioctl(b->fd, BINDER_WRITE_READ, &bwr);

    uint8_t read_buf[1024];

    while (1) {
        memset(&bwr, 0, sizeof(bwr));
        bwr.read_buffer = (binder_uintptr_t)read_buf;
        bwr.read_size = sizeof(read_buf);

        if (ioctl(b->fd, BINDER_WRITE_READ, &bwr) < 0) {
            if (errno == EINTR) continue;
            break;
        }

        uint8_t *ptr = read_buf;
        uint8_t *end = read_buf + bwr.read_consumed;

        while (ptr < end) {
            uint32_t br_cmd = *(uint32_t *)ptr;
            ptr += sizeof(uint32_t);

            if (br_cmd == BR_TRANSACTION) {
                struct binder_transaction_data *tr = (struct binder_transaction_data *)ptr;
                ptr += sizeof(struct binder_transaction_data);

                uint8_t resp_data[512];
                size_t resp_len = sizeof(resp_data);

                int status = handler(tr->code, (void *)tr->data.ptr.buffer, tr->data_size,
                                     resp_data, &resp_len, user_data);

                struct {
                    uint32_t free_cmd;
                    binder_uintptr_t free_buf;
                    uint32_t reply_cmd;
                    struct binder_transaction_data reply_tr;
                } __attribute__((packed)) reply_pkt;

                memset(&reply_pkt, 0, sizeof(reply_pkt));
                reply_pkt.free_cmd = BC_FREE_BUFFER;
                reply_pkt.free_buf = tr->data.ptr.buffer;

                reply_pkt.reply_cmd = BC_REPLY;
                reply_pkt.reply_tr.target.ptr = tr->target.ptr;
                reply_pkt.reply_tr.cookie = tr->cookie;
                reply_pkt.reply_tr.code = status;
                reply_pkt.reply_tr.data_size = resp_len;
                reply_pkt.reply_tr.data.ptr.buffer = (binder_uintptr_t)resp_data;

                struct binder_write_read reply_bwr = {0};
                reply_bwr.write_buffer = (binder_uintptr_t)&reply_pkt;
                reply_bwr.write_size = sizeof(reply_pkt);
                ioctl(b->fd, BINDER_WRITE_READ, &reply_bwr);
            } else if (br_cmd == BR_NOOP || br_cmd == BR_OK || br_cmd == BR_TRANSACTION_COMPLETE) {
                continue;
            } else {
                break; // Unknown or unhandled event payload
            }
        }
    }

    return 0;
}

void libinder_close(libinder_t *b) {
    if (!b) return;
    if (b->mapped && b->mapped_size > 0) {
        munmap(b->mapped, b->mapped_size);
    }
    if (b->fd >= 0) {
        close(b->fd);
    }
    free(b);
}
