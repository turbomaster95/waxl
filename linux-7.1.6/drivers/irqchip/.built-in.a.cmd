savedcmd_drivers/irqchip/built-in.a := rm -f drivers/irqchip/built-in.a;  printf "drivers/irqchip/%s " irqchip.o | xargs ar cDPrST drivers/irqchip/built-in.a
