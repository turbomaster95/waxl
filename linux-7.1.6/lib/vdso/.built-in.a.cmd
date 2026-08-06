savedcmd_lib/vdso/built-in.a := rm -f lib/vdso/built-in.a;  printf "lib/vdso/%s " datastore.o | xargs ar cDPrST lib/vdso/built-in.a
