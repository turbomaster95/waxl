savedcmd_drivers/clocksource/built-in.a := rm -f drivers/clocksource/built-in.a;  printf "drivers/clocksource/%s " i8253.o | xargs ar cDPrST drivers/clocksource/built-in.a
