savedcmd_drivers/input/modules.order := {   cat drivers/input/keyboard/modules.order;   cat drivers/input/mouse/modules.order;   echo drivers/input/mousedev.o; :; } > drivers/input/modules.order
