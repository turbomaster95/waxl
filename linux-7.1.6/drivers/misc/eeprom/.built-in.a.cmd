savedcmd_drivers/misc/eeprom/built-in.a := rm -f drivers/misc/eeprom/built-in.a;  printf "drivers/misc/eeprom/%s " eeprom_93cx6.o | xargs ar cDPrST drivers/misc/eeprom/built-in.a
