savedcmd_drivers/virt/built-in.a := rm -f drivers/virt/built-in.a;  printf "drivers/virt/%s " vboxguest/built-in.a coco/built-in.a | xargs ar cDPrST drivers/virt/built-in.a
