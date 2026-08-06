savedcmd_security/built-in.a := rm -f security/built-in.a;  printf "security/%s " commoncap.o min_addr.o device_cgroup.o | xargs ar cDPrST security/built-in.a
