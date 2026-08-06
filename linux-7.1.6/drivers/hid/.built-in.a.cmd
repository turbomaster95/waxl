savedcmd_drivers/hid/built-in.a := rm -f drivers/hid/built-in.a;  printf "drivers/hid/%s " hid-core.o hid-input.o hid-quirks.o hid-generic.o | xargs ar cDPrST drivers/hid/built-in.a
