savedcmd_arch/x86/boot/bzImage := (dd if=arch/x86/boot/setup.bin bs=4k conv=sync status=none; cat arch/x86/boot/vmlinux.bin) >arch/x86/boot/bzImage
