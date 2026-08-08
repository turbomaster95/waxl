savedcmd_drivers/block/built-in.a := rm -f drivers/block/built-in.a;  printf "drivers/block/%s " virtio_blk.o | xargs ar cDPrST drivers/block/built-in.a
