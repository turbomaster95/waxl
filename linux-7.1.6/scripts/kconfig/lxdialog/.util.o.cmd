savedcmd_scripts/kconfig/lxdialog/util.o := gcc -Wp,-MMD,scripts/kconfig/lxdialog/.util.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include  -D_BSD_SOURCE -DNCURSES_WIDECHAR -c -o scripts/kconfig/lxdialog/util.o scripts/kconfig/lxdialog/util.c

source_scripts/kconfig/lxdialog/util.o := scripts/kconfig/lxdialog/util.c

deps_scripts/kconfig/lxdialog/util.o := \
  scripts/kconfig/lxdialog/dialog.h \

scripts/kconfig/lxdialog/util.o: $(deps_scripts/kconfig/lxdialog/util.o)

$(deps_scripts/kconfig/lxdialog/util.o):
