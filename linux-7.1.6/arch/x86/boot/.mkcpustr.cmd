savedcmd_arch/x86/boot/mkcpustr := gcc -Wp,-MMD,arch/x86/boot/.mkcpustr.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ./scripts/include -I./tools/include -include include/generated/autoconf.h -D__EXPORTED_HEADERS__    -o arch/x86/boot/mkcpustr arch/x86/boot/mkcpustr.c   

source_arch/x86/boot/mkcpustr := arch/x86/boot/mkcpustr.c

deps_arch/x86/boot/mkcpustr := \
  arch/x86/boot/../include/asm/cpufeatures.h \
    $(wildcard include/config/X86_32) \
  arch/x86/boot/../include/asm/vmxfeatures.h \
  arch/x86/boot/../kernel/cpu/capflags.c \
    $(wildcard include/config/X86_VMX_FEATURE_NAMES) \

arch/x86/boot/mkcpustr: $(deps_arch/x86/boot/mkcpustr)

$(deps_arch/x86/boot/mkcpustr):
