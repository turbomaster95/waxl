savedcmd_arch/x86/mm/pat/built-in.a := rm -f arch/x86/mm/pat/built-in.a;  printf "arch/x86/mm/pat/%s " set_memory.o memtype.o | xargs ar cDPrST arch/x86/mm/pat/built-in.a
