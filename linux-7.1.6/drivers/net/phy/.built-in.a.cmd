savedcmd_drivers/net/phy/built-in.a := rm -f drivers/net/phy/built-in.a;  printf "drivers/net/phy/%s " mediatek/built-in.a qcom/built-in.a | xargs ar cDPrST drivers/net/phy/built-in.a
