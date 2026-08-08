savedcmd_kernel/power/built-in.a := rm -f kernel/power/built-in.a;  printf "kernel/power/%s " qos.o main.o | xargs ar cDPrST kernel/power/built-in.a
