savedcmd_mod.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T /home/deva/linux-7.1.6/scripts/module.lds -o mod.ko mod.o mod.mod.o .module-common.o
