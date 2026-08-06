savedcmd_drivers/rtc/built-in.a := rm -f drivers/rtc/built-in.a;  printf "drivers/rtc/%s " lib.o rtc-mc146818-lib.o | xargs ar cDPrST drivers/rtc/built-in.a
