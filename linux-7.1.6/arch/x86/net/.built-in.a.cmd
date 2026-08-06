savedcmd_arch/x86/net/built-in.a := rm -f arch/x86/net/built-in.a;  printf "arch/x86/net/%s " bpf_jit_comp.o bpf_timed_may_goto.o | xargs ar cDPrST arch/x86/net/built-in.a
