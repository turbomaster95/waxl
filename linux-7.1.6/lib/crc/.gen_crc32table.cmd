savedcmd_lib/crc/gen_crc32table := gcc -Wp,-MMD,lib/crc/.gen_crc32table.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include     -o lib/crc/gen_crc32table lib/crc/gen_crc32table.c   

source_lib/crc/gen_crc32table := lib/crc/gen_crc32table.c

deps_lib/crc/gen_crc32table := \
  lib/crc/../../include/linux/crc32poly.h \

lib/crc/gen_crc32table: $(deps_lib/crc/gen_crc32table)

$(deps_lib/crc/gen_crc32table):
