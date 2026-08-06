savedcmd_drivers/firmware/cirrus/built-in.a := rm -f drivers/firmware/cirrus/built-in.a;  printf "drivers/firmware/cirrus/%s " test/built-in.a | xargs ar cDPrST drivers/firmware/cirrus/built-in.a
