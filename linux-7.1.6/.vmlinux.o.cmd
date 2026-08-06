savedcmd_vmlinux.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments -r -o vmlinux.o   --whole-archive vmlinux.a --no-whole-archive --start-group  --end-group  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --static-call --uaccess --link vmlinux.o

vmlinux.o: $(wildcard ./tools/objtool/objtool)
