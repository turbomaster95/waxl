savedcmd_kernel/cgroup/built-in.a := rm -f kernel/cgroup/built-in.a;  printf "kernel/cgroup/%s " cgroup.o rstat.o namespace.o cgroup-v1.o freezer.o pids.o | xargs ar cDPrST kernel/cgroup/built-in.a
