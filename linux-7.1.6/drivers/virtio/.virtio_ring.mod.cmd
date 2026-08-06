savedcmd_drivers/virtio/virtio_ring.mod := printf '%s\n'   virtio_ring.o | awk '!x[$$0]++ { print("drivers/virtio/"$$0) }' > drivers/virtio/virtio_ring.mod
