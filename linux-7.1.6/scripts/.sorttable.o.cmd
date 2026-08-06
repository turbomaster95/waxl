savedcmd_scripts/sorttable.o := gcc -Wp,-MMD,scripts/.sorttable.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include  -I./tools/include -c -o scripts/sorttable.o scripts/sorttable.c

source_scripts/sorttable.o := scripts/sorttable.c

deps_scripts/sorttable.o := \
  scripts/elf-parse.h \
  tools/include/tools/be_byteshift.h \
  tools/include/tools/le_byteshift.h \

scripts/sorttable.o: $(deps_scripts/sorttable.o)

$(deps_scripts/sorttable.o):
