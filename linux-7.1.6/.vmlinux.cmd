savedcmd_vmlinux := objcopy --set-section-flags .modinfo=noload vmlinux.unstripped vmlinux; objcopy --remove-section=.modinfo -w --strip-unneeded-symbol='__mod_device_table__*' vmlinux
