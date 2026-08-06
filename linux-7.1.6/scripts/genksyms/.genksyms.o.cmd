savedcmd_scripts/genksyms/genksyms.o := gcc -Wp,-MMD,scripts/genksyms/.genksyms.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include   -c -o scripts/genksyms/genksyms.o scripts/genksyms/genksyms.c

source_scripts/genksyms/genksyms.o := scripts/genksyms/genksyms.c

deps_scripts/genksyms/genksyms.o := \
  scripts/include/hashtable.h \
  scripts/include/array_size.h \
  scripts/include/list.h \
  scripts/include/list_types.h \
  scripts/genksyms/genksyms.h \

scripts/genksyms/genksyms.o: $(deps_scripts/genksyms/genksyms.o)

$(deps_scripts/genksyms/genksyms.o):
