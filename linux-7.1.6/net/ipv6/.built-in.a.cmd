savedcmd_net/ipv6/built-in.a := rm -f net/ipv6/built-in.a;  printf "net/ipv6/%s " addrconf_core.o exthdrs_core.o ip6_checksum.o ip6_icmp.o | xargs ar cDPrST net/ipv6/built-in.a
