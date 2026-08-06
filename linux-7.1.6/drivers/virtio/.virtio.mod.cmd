savedcmd_drivers/virtio/virtio.mod := printf '%s\n'   virtio.o | awk '!x[$$0]++ { print("drivers/virtio/"$$0) }' > drivers/virtio/virtio.mod
