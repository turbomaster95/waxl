savedcmd_modules.builtin.modinfo := objcopy  -j .modinfo -O binary vmlinux.unstripped modules.builtin.modinfo; sed -i 's/\x00\+$$/\x00/g' modules.builtin.modinfo; chmod -x modules.builtin.modinfo
