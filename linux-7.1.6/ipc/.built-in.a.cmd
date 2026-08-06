savedcmd_ipc/built-in.a := rm -f ipc/built-in.a;  printf "ipc/%s " util.o msgutil.o msg.o sem.o shm.o syscall.o ipc_sysctl.o namespace.o | xargs ar cDPrST ipc/built-in.a
