savedcmd_drivers/virtio/built-in.a := rm -f drivers/virtio/built-in.a;  printf "drivers/virtio/%s " virtio_anchor.o | xargs ar cDPrST drivers/virtio/built-in.a
