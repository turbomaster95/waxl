savedcmd_drivers/leds/built-in.a := rm -f drivers/leds/built-in.a;  printf "drivers/leds/%s " blink/built-in.a simatic/built-in.a | xargs ar cDPrST drivers/leds/built-in.a
