savedcmd_lib/crc/built-in.a := rm -f lib/crc/built-in.a;  printf "lib/crc/%s " crc16.o crc32-main.o tests/built-in.a | xargs ar cDPrST lib/crc/built-in.a
