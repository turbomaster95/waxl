savedcmd_vmlinux.unstripped := scripts/link-vmlinux.sh "ld" "-m elf_x86_64 -z noexecstack --no-warn-rwx-segments" "-z max-page-size=0x200000 --build-id=sha1 --orphan-handling=warn" "vmlinux.unstripped";  true

source_vmlinux.unstripped := scripts/link-vmlinux.sh

deps_vmlinux.unstripped := \
    $(wildcard include/config/LTO_CLANG) \
    $(wildcard include/config/X86_KERNEL_IBT) \
    $(wildcard include/config/KLP_BUILD) \
    $(wildcard include/config/GENERIC_BUILTIN_DTB) \
    $(wildcard include/config/KALLSYMS_ALL) \
    $(wildcard include/config/64BIT) \
    $(wildcard include/config/RELOCATABLE) \
    $(wildcard include/config/ARCH_WANTS_PRE_LINK_VMLINUX) \
    $(wildcard include/config/KALLSYMS) \
    $(wildcard include/config/DEBUG_INFO_BTF) \
    $(wildcard include/config/SHELL) \
    $(wildcard include/config/VMLINUX_MAP) \
    $(wildcard include/config/BUILDTIME_TABLE_SORT) \

vmlinux.unstripped: $(deps_vmlinux.unstripped)

$(deps_vmlinux.unstripped):
