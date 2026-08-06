savedcmd_drivers/gpu/drm/virtio/virtio-gpu.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o drivers/gpu/drm/virtio/virtio-gpu.o @drivers/gpu/drm/virtio/virtio-gpu.mod  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --static-call --uaccess  --link  --module drivers/gpu/drm/virtio/virtio-gpu.o

drivers/gpu/drm/virtio/virtio-gpu.o: $(wildcard ./tools/objtool/objtool)
