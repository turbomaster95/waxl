savedcmd_scripts/kconfig/lxdialog/checklist.o := gcc -Wp,-MMD,scripts/kconfig/lxdialog/.checklist.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include  -D_BSD_SOURCE -DNCURSES_WIDECHAR -c -o scripts/kconfig/lxdialog/checklist.o scripts/kconfig/lxdialog/checklist.c

source_scripts/kconfig/lxdialog/checklist.o := scripts/kconfig/lxdialog/checklist.c

deps_scripts/kconfig/lxdialog/checklist.o := \
  scripts/kconfig/lxdialog/dialog.h \

scripts/kconfig/lxdialog/checklist.o: $(deps_scripts/kconfig/lxdialog/checklist.o)

$(deps_scripts/kconfig/lxdialog/checklist.o):
