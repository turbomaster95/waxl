savedcmd_drivers/virtio/virtio_pci.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o drivers/virtio/virtio_pci.o @drivers/virtio/virtio_pci.mod  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --static-call --uaccess  --link  --module drivers/virtio/virtio_pci.o

drivers/virtio/virtio_pci.o: $(wildcard ./tools/objtool/objtool)
