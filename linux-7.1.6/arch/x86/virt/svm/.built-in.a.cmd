savedcmd_arch/x86/virt/svm/built-in.a := rm -f arch/x86/virt/svm/built-in.a;  printf "arch/x86/virt/svm/%s " cmdline.o | xargs ar cDPrST arch/x86/virt/svm/built-in.a
