savedcmd_drivers/firmware/efi/built-in.a := rm -f drivers/firmware/efi/built-in.a;  printf "drivers/firmware/efi/%s " sysfb_efi.o | xargs ar cDPrST drivers/firmware/efi/built-in.a
