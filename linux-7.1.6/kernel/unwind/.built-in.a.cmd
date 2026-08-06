savedcmd_kernel/unwind/built-in.a := rm -f kernel/unwind/built-in.a;  printf "kernel/unwind/%s " user.o deferred.o | xargs ar cDPrST kernel/unwind/built-in.a
