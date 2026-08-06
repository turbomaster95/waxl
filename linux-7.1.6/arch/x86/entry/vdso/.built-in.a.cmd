savedcmd_arch/x86/entry/vdso/built-in.a := rm -f arch/x86/entry/vdso/built-in.a;  printf "arch/x86/entry/vdso/%s " vma.o extable.o vdso64/built-in.a | xargs ar cDPrST arch/x86/entry/vdso/built-in.a
