savedcmd_arch/x86/entry/vdso/vdso64/note.o := gcc -Wp,-MMD,arch/x86/entry/vdso/vdso64/.note.o.d -nostdinc -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/compiler-version.h -include ./include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__ -m64 -DBUILD_VDSO64 -m64 -mcmodel=small -D__DISABLE_EXPORTS -DDISABLE_BRANCH_PROFILING -DBUILD_VDSO -I./arch/x86/entry/vdso/vdso64/.. -I. -O2 -fpic -fno-stack-protector -fno-omit-frame-pointer -foptimize-sibling-calls -fasynchronous-unwind-tables -fcf-protection=none    -DKBUILD_MODFILE='"arch/x86/entry/vdso/vdso64/note"' -DKBUILD_MODNAME='"note"' -D__KBUILD_MODNAME=note -c -o arch/x86/entry/vdso/vdso64/note.o arch/x86/entry/vdso/vdso64/note.S 

source_arch/x86/entry/vdso/vdso64/note.o := arch/x86/entry/vdso/vdso64/note.S

deps_arch/x86/entry/vdso/vdso64/note.o := \
  include/linux/compiler-version.h \
    $(wildcard include/config/CC_VERSION_TEXT) \
  include/linux/kconfig.h \
    $(wildcard include/config/CPU_BIG_ENDIAN) \
    $(wildcard include/config/BOOGER) \
    $(wildcard include/config/FOO) \
  arch/x86/entry/vdso/vdso64/../common/note.S \
  include/linux/build-salt.h \
    $(wildcard include/config/BUILD_SALT) \
  include/linux/elfnote.h \
  include/generated/uapi/linux/version.h \

arch/x86/entry/vdso/vdso64/note.o: $(deps_arch/x86/entry/vdso/vdso64/note.o)

$(deps_arch/x86/entry/vdso/vdso64/note.o):
