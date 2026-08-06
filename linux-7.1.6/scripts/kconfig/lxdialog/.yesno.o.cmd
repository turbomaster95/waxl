savedcmd_scripts/kconfig/lxdialog/yesno.o := gcc -Wp,-MMD,scripts/kconfig/lxdialog/.yesno.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include  -D_BSD_SOURCE -DNCURSES_WIDECHAR -c -o scripts/kconfig/lxdialog/yesno.o scripts/kconfig/lxdialog/yesno.c

source_scripts/kconfig/lxdialog/yesno.o := scripts/kconfig/lxdialog/yesno.c

deps_scripts/kconfig/lxdialog/yesno.o := \
  scripts/kconfig/lxdialog/dialog.h \

scripts/kconfig/lxdialog/yesno.o: $(deps_scripts/kconfig/lxdialog/yesno.o)

$(deps_scripts/kconfig/lxdialog/yesno.o):
