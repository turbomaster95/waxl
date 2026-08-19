savedcmd_mod.o := gcc -Wp,-MMD,./.mod.o.d -nostdinc -I/home/deva/linux-7.1.6/arch/x86/include -I/home/deva/linux-7.1.6/arch/x86/include/generated -I/home/deva/linux-7.1.6/include -I/home/deva/linux-7.1.6/include -I/home/deva/linux-7.1.6/arch/x86/include/uapi -I/home/deva/linux-7.1.6/arch/x86/include/generated/uapi -I/home/deva/linux-7.1.6/include/uapi -I/home/deva/linux-7.1.6/include/generated/uapi -include /home/deva/linux-7.1.6/include/linux/compiler-version.h -include /home/deva/linux-7.1.6/include/linux/kconfig.h -include /home/deva/linux-7.1.6/include/linux/compiler_types.h -D__KERNEL__ -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -std=gnu11 -fms-extensions -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-sse4a -fcf-protection=branch -fno-jump-tables -m64 -falign-jumps=1 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mpreferred-stack-boundary=3 -mskip-rax-setup -march=x86-64 -mtune=generic -mno-red-zone -mcmodel=kernel -Wno-sign-compare -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -Os -fno-allow-store-data-races -fno-stack-protector -fomit-frame-pointer -ftrivial-auto-var-init=zero -fzero-init-padding-bits=all -fno-stack-clash-protection -fmin-function-alignment=16 -fstrict-flex-arrays=3 -fno-strict-overflow -fno-stack-check -fconserve-stack -fno-builtin-wcslen -Wall -Wextra -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=1280 -Wno-main -Wno-type-limits -Wno-dangling-pointer -Wvla-larger-than=1 -Wno-pointer-sign -Wcast-function-type -Wno-unterminated-string-initialization -Wno-array-bounds -Wno-stringop-overflow -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wenum-conversion -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-packed-not-aligned -Wno-format-overflow -Wno-format-truncation -Wno-stringop-truncation -Wno-override-init -Wno-missing-field-initializers -Wno-shift-negative-value -Wno-maybe-uninitialized -Wno-sign-compare -Wno-unused-parameter  -DMODULE  -DKBUILD_BASENAME='"mod"' -DKBUILD_MODNAME='"mod"' -D__KBUILD_MODNAME=mod -c -o mod.o mod.c   ; /home/deva/linux-7.1.6/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --static-call --uaccess  --link  --module mod.o

source_mod.o := mod.c

deps_mod.o := \
  /home/deva/linux-7.1.6/include/linux/compiler-version.h \
    $(wildcard include/config/CC_VERSION_TEXT) \
  /home/deva/linux-7.1.6/include/linux/kconfig.h \
    $(wildcard include/config/CPU_BIG_ENDIAN) \
    $(wildcard include/config/BOOGER) \
    $(wildcard include/config/FOO) \
  /home/deva/linux-7.1.6/include/linux/compiler_types.h \
    $(wildcard include/config/DEBUG_INFO_BTF) \
    $(wildcard include/config/PAHOLE_HAS_BTF_TAG) \
    $(wildcard include/config/FUNCTION_ALIGNMENT) \
    $(wildcard include/config/CC_HAS_SANE_FUNCTION_ALIGNMENT) \
    $(wildcard include/config/X86_64) \
    $(wildcard include/config/ARM64) \
    $(wildcard include/config/LD_DEAD_CODE_DATA_ELIMINATION) \
    $(wildcard include/config/LTO_CLANG) \
    $(wildcard include/config/HAVE_ARCH_COMPILER_H) \
    $(wildcard include/config/KCSAN) \
    $(wildcard include/config/CC_HAS_ASSUME) \
    $(wildcard include/config/CC_HAS_COUNTED_BY) \
    $(wildcard include/config/FORTIFY_SOURCE) \
    $(wildcard include/config/UBSAN_BOUNDS) \
    $(wildcard include/config/CC_HAS_COUNTED_BY_PTR) \
    $(wildcard include/config/CC_HAS_MULTIDIMENSIONAL_NONSTRING) \
    $(wildcard include/config/CFI) \
    $(wildcard include/config/ARCH_USES_CFI_GENERIC_LLVM_PASS) \
    $(wildcard include/config/CC_HAS_BROKEN_COUNTED_BY_REF) \
    $(wildcard include/config/CC_HAS_ASM_INLINE) \
  /home/deva/linux-7.1.6/include/linux/compiler-context-analysis.h \
  /home/deva/linux-7.1.6/include/linux/compiler_attributes.h \
  /home/deva/linux-7.1.6/include/linux/compiler-gcc.h \
    $(wildcard include/config/ARCH_USE_BUILTIN_BSWAP) \
    $(wildcard include/config/SHADOW_CALL_STACK) \
    $(wildcard include/config/KCOV) \
    $(wildcard include/config/CC_HAS_TYPEOF_UNQUAL) \
  /home/deva/linux-7.1.6/include/linux/init.h \
    $(wildcard include/config/MEMORY_HOTPLUG) \
    $(wildcard include/config/HAVE_ARCH_PREL32_RELOCATIONS) \
  /home/deva/linux-7.1.6/include/linux/build_bug.h \
  /home/deva/linux-7.1.6/include/linux/compiler.h \
    $(wildcard include/config/TRACE_BRANCH_PROFILING) \
    $(wildcard include/config/PROFILE_ALL_BRANCHES) \
    $(wildcard include/config/OBJTOOL) \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/rwonce.h \
  /home/deva/linux-7.1.6/include/asm-generic/rwonce.h \
  /home/deva/linux-7.1.6/include/linux/kasan-checks.h \
    $(wildcard include/config/KASAN_GENERIC) \
    $(wildcard include/config/KASAN_SW_TAGS) \
  /home/deva/linux-7.1.6/include/linux/types.h \
    $(wildcard include/config/HAVE_UID16) \
    $(wildcard include/config/UID16) \
    $(wildcard include/config/ARCH_DMA_ADDR_T_64BIT) \
    $(wildcard include/config/PHYS_ADDR_T_64BIT) \
    $(wildcard include/config/64BIT) \
    $(wildcard include/config/ARCH_32BIT_USTAT_F_TINODE) \
  /home/deva/linux-7.1.6/include/uapi/linux/types.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/types.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/types.h \
  /home/deva/linux-7.1.6/include/asm-generic/int-ll64.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/int-ll64.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/bitsperlong.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitsperlong.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/bitsperlong.h \
  /home/deva/linux-7.1.6/include/uapi/linux/posix_types.h \
  /home/deva/linux-7.1.6/include/linux/stddef.h \
  /home/deva/linux-7.1.6/include/uapi/linux/stddef.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/X86_32) \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/posix_types_64.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/posix_types.h \
  /home/deva/linux-7.1.6/include/linux/kcsan-checks.h \
    $(wildcard include/config/KCSAN_WEAK_MEMORY) \
    $(wildcard include/config/KCSAN_IGNORE_ATOMICS) \
  /home/deva/linux-7.1.6/include/linux/stringify.h \
  /home/deva/linux-7.1.6/include/linux/module.h \
    $(wildcard include/config/MODULES) \
    $(wildcard include/config/SYSFS) \
    $(wildcard include/config/MODULES_TREE_LOOKUP) \
    $(wildcard include/config/LIVEPATCH) \
    $(wildcard include/config/STACKTRACE_BUILD_ID) \
    $(wildcard include/config/ARCH_USES_CFI_TRAPS) \
    $(wildcard include/config/MODULE_SIG) \
    $(wildcard include/config/GENERIC_BUG) \
    $(wildcard include/config/KALLSYMS) \
    $(wildcard include/config/SMP) \
    $(wildcard include/config/TRACEPOINTS) \
    $(wildcard include/config/TREE_SRCU) \
    $(wildcard include/config/BPF_EVENTS) \
    $(wildcard include/config/DEBUG_INFO_BTF_MODULES) \
    $(wildcard include/config/JUMP_LABEL) \
    $(wildcard include/config/TRACING) \
    $(wildcard include/config/EVENT_TRACING) \
    $(wildcard include/config/DYNAMIC_FTRACE) \
    $(wildcard include/config/KPROBES) \
    $(wildcard include/config/HAVE_STATIC_CALL_INLINE) \
    $(wildcard include/config/KUNIT) \
    $(wildcard include/config/PRINTK_INDEX) \
    $(wildcard include/config/MODULE_UNLOAD) \
    $(wildcard include/config/CONSTRUCTORS) \
    $(wildcard include/config/FUNCTION_ERROR_INJECTION) \
    $(wildcard include/config/DYNAMIC_DEBUG_CORE) \
    $(wildcard include/config/MITIGATION_RETPOLINE) \
  /home/deva/linux-7.1.6/include/linux/list.h \
    $(wildcard include/config/LIST_HARDENED) \
    $(wildcard include/config/DEBUG_LIST) \
  /home/deva/linux-7.1.6/include/linux/container_of.h \
  /home/deva/linux-7.1.6/include/linux/poison.h \
    $(wildcard include/config/ILLEGAL_POINTER_VALUE) \
  /home/deva/linux-7.1.6/include/linux/const.h \
  /home/deva/linux-7.1.6/include/vdso/const.h \
  /home/deva/linux-7.1.6/include/uapi/linux/const.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/barrier.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/alternative.h \
    $(wildcard include/config/CALL_THUNKS) \
    $(wildcard include/config/MITIGATION_ITS) \
    $(wildcard include/config/MITIGATION_RETHUNK) \
  /home/deva/linux-7.1.6/include/linux/objtool.h \
    $(wildcard include/config/FRAME_POINTER) \
    $(wildcard include/config/NOINSTR_VALIDATION) \
    $(wildcard include/config/MITIGATION_UNRET_ENTRY) \
    $(wildcard include/config/MITIGATION_SRSO) \
  /home/deva/linux-7.1.6/include/linux/objtool_types.h \
  /home/deva/linux-7.1.6/include/linux/annotate.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/asm.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/asm-offsets.h \
  /home/deva/linux-7.1.6/include/generated/asm-offsets.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/extable_fixup_types.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/bug.h \
    $(wildcard include/config/DEBUG_BUGVERBOSE) \
    $(wildcard include/config/DEBUG_BUGVERBOSE_DETAILED) \
  /home/deva/linux-7.1.6/include/linux/instrumentation.h \
  /home/deva/linux-7.1.6/include/asm-generic/bug.h \
    $(wildcard include/config/BUG) \
    $(wildcard include/config/GENERIC_BUG_RELATIVE_POINTERS) \
  /home/deva/linux-7.1.6/include/linux/once_lite.h \
  /home/deva/linux-7.1.6/include/linux/panic.h \
    $(wildcard include/config/PANIC_TIMEOUT) \
  /home/deva/linux-7.1.6/include/linux/stdarg.h \
  /home/deva/linux-7.1.6/include/linux/printk.h \
    $(wildcard include/config/MESSAGE_LOGLEVEL_DEFAULT) \
    $(wildcard include/config/CONSOLE_LOGLEVEL_DEFAULT) \
    $(wildcard include/config/CONSOLE_LOGLEVEL_QUIET) \
    $(wildcard include/config/EARLY_PRINTK) \
    $(wildcard include/config/PRINTK) \
    $(wildcard include/config/DYNAMIC_DEBUG) \
  /home/deva/linux-7.1.6/include/linux/kern_levels.h \
  /home/deva/linux-7.1.6/include/linux/linkage.h \
    $(wildcard include/config/ARCH_USE_SYM_ANNOTATIONS) \
  /home/deva/linux-7.1.6/include/linux/export.h \
    $(wildcard include/config/MODVERSIONS) \
    $(wildcard include/config/GENDWARFKSYMS) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/CALL_PADDING) \
    $(wildcard include/config/MITIGATION_SLS) \
    $(wildcard include/config/FUNCTION_PADDING_BYTES) \
    $(wildcard include/config/UML) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/ibt.h \
    $(wildcard include/config/X86_KERNEL_IBT) \
  /home/deva/linux-7.1.6/include/linux/ratelimit_types.h \
  /home/deva/linux-7.1.6/include/linux/bits.h \
  /home/deva/linux-7.1.6/include/vdso/bits.h \
  /home/deva/linux-7.1.6/include/uapi/linux/bits.h \
  /home/deva/linux-7.1.6/include/linux/overflow.h \
  /home/deva/linux-7.1.6/include/linux/limits.h \
  /home/deva/linux-7.1.6/include/uapi/linux/limits.h \
  /home/deva/linux-7.1.6/include/vdso/limits.h \
  /home/deva/linux-7.1.6/include/uapi/linux/param.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/param.h \
  /home/deva/linux-7.1.6/include/asm-generic/param.h \
    $(wildcard include/config/HZ) \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/param.h \
  /home/deva/linux-7.1.6/include/linux/spinlock_types_raw.h \
    $(wildcard include/config/DEBUG_SPINLOCK) \
    $(wildcard include/config/DEBUG_LOCK_ALLOC) \
  /home/deva/linux-7.1.6/include/linux/spinlock_types_up.h \
  /home/deva/linux-7.1.6/include/linux/lockdep_types.h \
    $(wildcard include/config/PROVE_RAW_LOCK_NESTING) \
    $(wildcard include/config/LOCKDEP) \
    $(wildcard include/config/LOCK_STAT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/nops.h \
  /home/deva/linux-7.1.6/include/asm-generic/barrier.h \
  /home/deva/linux-7.1.6/include/linux/stat.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/stat.h \
  /home/deva/linux-7.1.6/include/uapi/linux/stat.h \
  /home/deva/linux-7.1.6/include/linux/time.h \
    $(wildcard include/config/POSIX_TIMERS) \
  /home/deva/linux-7.1.6/include/linux/cache.h \
    $(wildcard include/config/ARCH_HAS_CACHE_LINE_SIZE) \
  /home/deva/linux-7.1.6/include/uapi/linux/kernel.h \
  /home/deva/linux-7.1.6/include/uapi/linux/sysinfo.h \
  /home/deva/linux-7.1.6/include/vdso/cache.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cache.h \
    $(wildcard include/config/X86_L1_CACHE_SHIFT) \
    $(wildcard include/config/X86_INTERNODE_CACHE_SHIFT) \
    $(wildcard include/config/X86_VSMP) \
  /home/deva/linux-7.1.6/include/linux/math64.h \
    $(wildcard include/config/ARCH_SUPPORTS_INT128) \
  /home/deva/linux-7.1.6/include/linux/math.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/div64.h \
  /home/deva/linux-7.1.6/include/asm-generic/div64.h \
    $(wildcard include/config/CC_OPTIMIZE_FOR_PERFORMANCE) \
  /home/deva/linux-7.1.6/include/vdso/math64.h \
  /home/deva/linux-7.1.6/include/linux/time64.h \
  /home/deva/linux-7.1.6/include/vdso/time64.h \
  /home/deva/linux-7.1.6/include/uapi/linux/time.h \
  /home/deva/linux-7.1.6/include/uapi/linux/time_types.h \
  /home/deva/linux-7.1.6/include/linux/time32.h \
  /home/deva/linux-7.1.6/include/linux/timex.h \
  /home/deva/linux-7.1.6/include/uapi/linux/timex.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/timex.h \
    $(wildcard include/config/X86_TSC) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/processor.h \
    $(wildcard include/config/X86_VMX_FEATURE_NAMES) \
    $(wildcard include/config/X86_IOPL_IOPERM) \
    $(wildcard include/config/VM86) \
    $(wildcard include/config/X86_USER_SHADOW_STACK) \
    $(wildcard include/config/X86_DEBUG_FPU) \
    $(wildcard include/config/USE_X86_SEG_SUPPORT) \
    $(wildcard include/config/PARAVIRT_XXL) \
    $(wildcard include/config/CPU_SUP_AMD) \
    $(wildcard include/config/XEN) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/MITIGATION_PAGE_TABLE_ISOLATION) \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/processor-flags.h \
  /home/deva/linux-7.1.6/include/linux/mem_encrypt.h \
    $(wildcard include/config/ARCH_HAS_MEM_ENCRYPT) \
    $(wildcard include/config/AMD_MEM_ENCRYPT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/mem_encrypt.h \
    $(wildcard include/config/X86_MEM_ENCRYPT) \
  /home/deva/linux-7.1.6/include/linux/cc_platform.h \
    $(wildcard include/config/ARCH_HAS_CC_PLATFORM) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/math_emu.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/ptrace.h \
    $(wildcard include/config/PARAVIRT) \
    $(wildcard include/config/IA32_EMULATION) \
    $(wildcard include/config/X86_DEBUGCTLMSR) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/segment.h \
    $(wildcard include/config/XEN_PV) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/page_types.h \
    $(wildcard include/config/PHYSICAL_START) \
    $(wildcard include/config/PHYSICAL_ALIGN) \
    $(wildcard include/config/DYNAMIC_PHYSICAL_MASK) \
  /home/deva/linux-7.1.6/include/vdso/page.h \
    $(wildcard include/config/PAGE_SHIFT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/page_64_types.h \
    $(wildcard include/config/KASAN) \
    $(wildcard include/config/RANDOMIZE_BASE) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/kaslr.h \
    $(wildcard include/config/RANDOMIZE_MEMORY) \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/ptrace.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/ptrace-abi.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/proto.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/ldt.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/sigcontext.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/current.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/CC_HAS_NAMED_AS) \
  /home/deva/linux-7.1.6/include/linux/args.h \
  /home/deva/linux-7.1.6/include/asm-generic/percpu.h \
    $(wildcard include/config/DEBUG_PREEMPT) \
    $(wildcard include/config/HAVE_SETUP_PER_CPU_AREA) \
  /home/deva/linux-7.1.6/include/linux/threads.h \
    $(wildcard include/config/NR_CPUS) \
    $(wildcard include/config/BASE_SMALL) \
  /home/deva/linux-7.1.6/include/linux/percpu-defs.h \
    $(wildcard include/config/ARCH_MODULE_NEEDS_WEAK_PER_CPU) \
    $(wildcard include/config/DEBUG_FORCE_WEAK_PER_CPU) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cpufeatures.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cpuid/api.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cpuid/types.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/string.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/string_64.h \
    $(wildcard include/config/KMSAN) \
    $(wildcard include/config/ARCH_HAS_UACCESS_FLUSHCACHE) \
  /home/deva/linux-7.1.6/include/linux/jump_label.h \
    $(wildcard include/config/HAVE_ARCH_JUMP_LABEL_RELATIVE) \
  /home/deva/linux-7.1.6/include/linux/cleanup.h \
  /home/deva/linux-7.1.6/include/linux/err.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/errno.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/errno.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/errno-base.h \
  /home/deva/linux-7.1.6/include/linux/atomic.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/atomic.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cmpxchg.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cmpxchg_64.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/rmwcc.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/atomic64_64.h \
  /home/deva/linux-7.1.6/include/linux/atomic/atomic-arch-fallback.h \
    $(wildcard include/config/GENERIC_ATOMIC64) \
  /home/deva/linux-7.1.6/include/linux/atomic/atomic-long.h \
  /home/deva/linux-7.1.6/include/linux/atomic/atomic-instrumented.h \
  /home/deva/linux-7.1.6/include/linux/instrumented.h \
    $(wildcard include/config/DEBUG_ATOMIC) \
    $(wildcard include/config/DEBUG_ATOMIC_LARGEST_ALIGN) \
  /home/deva/linux-7.1.6/include/linux/bug.h \
    $(wildcard include/config/BUG_ON_DATA_CORRUPTION) \
  /home/deva/linux-7.1.6/include/linux/kmsan-checks.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/page.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/page_64.h \
    $(wildcard include/config/DEBUG_VIRTUAL) \
    $(wildcard include/config/X86_VSYSCALL_EMULATION) \
  /home/deva/linux-7.1.6/include/linux/mmdebug.h \
    $(wildcard include/config/DEBUG_VM) \
    $(wildcard include/config/DEBUG_VM_IRQSOFF) \
    $(wildcard include/config/DEBUG_VM_PGFLAGS) \
  /home/deva/linux-7.1.6/include/linux/range.h \
  /home/deva/linux-7.1.6/include/asm-generic/memory_model.h \
    $(wildcard include/config/FLATMEM) \
    $(wildcard include/config/SPARSEMEM_VMEMMAP) \
    $(wildcard include/config/SPARSEMEM) \
  /home/deva/linux-7.1.6/include/linux/pfn.h \
  /home/deva/linux-7.1.6/include/asm-generic/getorder.h \
  /home/deva/linux-7.1.6/include/linux/log2.h \
    $(wildcard include/config/ARCH_HAS_ILOG2_U32) \
    $(wildcard include/config/ARCH_HAS_ILOG2_U64) \
  /home/deva/linux-7.1.6/include/linux/bitops.h \
  /home/deva/linux-7.1.6/include/linux/typecheck.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/generic-non-atomic.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/X86_CMOV) \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/sched.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/arch_hweight.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/const_hweight.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/instrumented-atomic.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/instrumented-non-atomic.h \
    $(wildcard include/config/KCSAN_ASSUME_PLAIN_WRITES_ATOMIC) \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/instrumented-lock.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/le.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/byteorder.h \
  /home/deva/linux-7.1.6/include/linux/byteorder/little_endian.h \
  /home/deva/linux-7.1.6/include/uapi/linux/byteorder/little_endian.h \
  /home/deva/linux-7.1.6/include/linux/swab.h \
  /home/deva/linux-7.1.6/include/uapi/linux/swab.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/swab.h \
  /home/deva/linux-7.1.6/include/linux/byteorder/generic.h \
  /home/deva/linux-7.1.6/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/X86_INTEL_MEMORY_PROTECTION_KEYS) \
    $(wildcard include/config/X86_PAE) \
    $(wildcard include/config/MEM_SOFT_DIRTY) \
    $(wildcard include/config/HAVE_ARCH_USERFAULTFD_WP) \
    $(wildcard include/config/PGTABLE_LEVELS) \
    $(wildcard include/config/PROC_FS) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pgtable_64_types.h \
    $(wildcard include/config/DEBUG_KMAP_LOCAL_FORCE_MAP) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/sparsemem.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/desc_defs.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/special_insns.h \
  /home/deva/linux-7.1.6/include/linux/errno.h \
  /home/deva/linux-7.1.6/include/uapi/linux/errno.h \
  /home/deva/linux-7.1.6/include/linux/irqflags.h \
    $(wildcard include/config/PROVE_LOCKING) \
    $(wildcard include/config/TRACE_IRQFLAGS) \
    $(wildcard include/config/PREEMPT_RT) \
    $(wildcard include/config/IRQSOFF_TRACER) \
    $(wildcard include/config/PREEMPT_TRACER) \
    $(wildcard include/config/DEBUG_IRQFLAGS) \
    $(wildcard include/config/TRACE_IRQFLAGS_SUPPORT) \
  /home/deva/linux-7.1.6/include/linux/irqflags_types.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/irqflags.h \
    $(wildcard include/config/DEBUG_ENTRY) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/nospec-branch.h \
    $(wildcard include/config/CALL_THUNKS_DEBUG) \
    $(wildcard include/config/MITIGATION_CALL_DEPTH_TRACKING) \
    $(wildcard include/config/MITIGATION_IBPB_ENTRY) \
    $(wildcard include/config/BPF_JIT) \
  /home/deva/linux-7.1.6/include/linux/static_key.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/msr-index.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/unwind_hints.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/orc_types.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/fpu/types.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/vmxfeatures.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/vdso/processor.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/shstk.h \
  /home/deva/linux-7.1.6/include/linux/personality.h \
  /home/deva/linux-7.1.6/include/uapi/linux/personality.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/tsc.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cpufeature.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/cpufeaturemasks.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/msr.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cpumask.h \
  /home/deva/linux-7.1.6/include/linux/cpumask.h \
    $(wildcard include/config/FORCE_NR_CPUS) \
    $(wildcard include/config/HOTPLUG_CPU) \
    $(wildcard include/config/DEBUG_PER_CPU_MAPS) \
    $(wildcard include/config/CPUMASK_OFFSTACK) \
  /home/deva/linux-7.1.6/include/linux/bitmap.h \
  /home/deva/linux-7.1.6/include/linux/align.h \
  /home/deva/linux-7.1.6/include/vdso/align.h \
  /home/deva/linux-7.1.6/include/linux/find.h \
  /home/deva/linux-7.1.6/include/linux/string.h \
    $(wildcard include/config/BINARY_PRINTF) \
  /home/deva/linux-7.1.6/include/linux/array_size.h \
  /home/deva/linux-7.1.6/include/uapi/linux/string.h \
  /home/deva/linux-7.1.6/include/linux/bitmap-str.h \
  /home/deva/linux-7.1.6/include/linux/cpumask_types.h \
  /home/deva/linux-7.1.6/include/linux/gfp_types.h \
    $(wildcard include/config/KASAN_HW_TAGS) \
  /home/deva/linux-7.1.6/include/linux/numa.h \
    $(wildcard include/config/NUMA_KEEP_MEMINFO) \
    $(wildcard include/config/NUMA) \
    $(wildcard include/config/HAVE_ARCH_NODE_DEV_GROUP) \
  /home/deva/linux-7.1.6/include/linux/nodemask.h \
    $(wildcard include/config/HIGHMEM) \
  /home/deva/linux-7.1.6/include/linux/minmax.h \
  /home/deva/linux-7.1.6/include/linux/nodemask_types.h \
    $(wildcard include/config/NODES_SHIFT) \
  /home/deva/linux-7.1.6/include/linux/random.h \
    $(wildcard include/config/VMGENID) \
  /home/deva/linux-7.1.6/include/linux/kernel.h \
    $(wildcard include/config/PREEMPT_VOLUNTARY_BUILD) \
    $(wildcard include/config/PREEMPT_DYNAMIC) \
    $(wildcard include/config/HAVE_PREEMPT_DYNAMIC_CALL) \
    $(wildcard include/config/HAVE_PREEMPT_DYNAMIC_KEY) \
    $(wildcard include/config/PREEMPT_) \
    $(wildcard include/config/DEBUG_ATOMIC_SLEEP) \
    $(wildcard include/config/MMU) \
  /home/deva/linux-7.1.6/include/linux/kstrtox.h \
  /home/deva/linux-7.1.6/include/linux/sprintf.h \
  /home/deva/linux-7.1.6/include/linux/static_call_types.h \
    $(wildcard include/config/HAVE_STATIC_CALL) \
  /home/deva/linux-7.1.6/include/linux/trace_printk.h \
  /home/deva/linux-7.1.6/include/linux/instruction_pointer.h \
  /home/deva/linux-7.1.6/include/linux/util_macros.h \
    $(wildcard include/config/FOO_SUSPEND) \
  /home/deva/linux-7.1.6/include/linux/wordpart.h \
  /home/deva/linux-7.1.6/include/uapi/linux/random.h \
  /home/deva/linux-7.1.6/include/uapi/linux/ioctl.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/ioctl.h \
  /home/deva/linux-7.1.6/include/asm-generic/ioctl.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/ioctl.h \
  /home/deva/linux-7.1.6/include/linux/irqnr.h \
  /home/deva/linux-7.1.6/include/uapi/linux/irqnr.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/msr.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/shared/msr.h \
  /home/deva/linux-7.1.6/include/linux/percpu.h \
    $(wildcard include/config/RANDOM_KMALLOC_CACHES) \
    $(wildcard include/config/PAGE_SIZE_4KB) \
    $(wildcard include/config/NEED_PER_CPU_PAGE_FIRST_CHUNK) \
  /home/deva/linux-7.1.6/include/linux/alloc_tag.h \
    $(wildcard include/config/MEM_ALLOC_PROFILING_DEBUG) \
    $(wildcard include/config/MEM_ALLOC_PROFILING) \
    $(wildcard include/config/MEM_ALLOC_PROFILING_ENABLED_BY_DEFAULT) \
  /home/deva/linux-7.1.6/include/linux/codetag.h \
    $(wildcard include/config/CODE_TAGGING) \
  /home/deva/linux-7.1.6/include/linux/preempt.h \
    $(wildcard include/config/PREEMPT_COUNT) \
    $(wildcard include/config/TRACE_PREEMPT_TOGGLE) \
    $(wildcard include/config/PREEMPTION) \
    $(wildcard include/config/PREEMPT_NOTIFIERS) \
    $(wildcard include/config/PREEMPT_NONE) \
    $(wildcard include/config/PREEMPT_VOLUNTARY) \
    $(wildcard include/config/PREEMPT) \
    $(wildcard include/config/PREEMPT_LAZY) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/preempt.h \
  /home/deva/linux-7.1.6/include/linux/smp.h \
    $(wildcard include/config/UP_LATE_INIT) \
    $(wildcard include/config/CSD_LOCK_WAIT_DEBUG) \
  /home/deva/linux-7.1.6/include/linux/smp_types.h \
  /home/deva/linux-7.1.6/include/linux/llist.h \
    $(wildcard include/config/ARCH_HAVE_NMI_SAFE_CMPXCHG) \
  /home/deva/linux-7.1.6/include/linux/sched.h \
    $(wildcard include/config/VIRT_CPU_ACCOUNTING_NATIVE) \
    $(wildcard include/config/SCHED_INFO) \
    $(wildcard include/config/SCHEDSTATS) \
    $(wildcard include/config/SCHED_CORE) \
    $(wildcard include/config/FAIR_GROUP_SCHED) \
    $(wildcard include/config/RT_GROUP_SCHED) \
    $(wildcard include/config/RT_MUTEXES) \
    $(wildcard include/config/UCLAMP_TASK) \
    $(wildcard include/config/UCLAMP_BUCKETS_COUNT) \
    $(wildcard include/config/KMAP_LOCAL) \
    $(wildcard include/config/THREAD_INFO_IN_TASK) \
    $(wildcard include/config/SCHED_CLASS_EXT) \
    $(wildcard include/config/CGROUP_SCHED) \
    $(wildcard include/config/CFS_BANDWIDTH) \
    $(wildcard include/config/BLK_DEV_IO_TRACE) \
    $(wildcard include/config/PREEMPT_RCU) \
    $(wildcard include/config/TASKS_RCU) \
    $(wildcard include/config/TASKS_TRACE_RCU) \
    $(wildcard include/config/TRIVIAL_PREEMPT_RCU) \
    $(wildcard include/config/MEMCG_V1) \
    $(wildcard include/config/LRU_GEN) \
    $(wildcard include/config/COMPAT_BRK) \
    $(wildcard include/config/CGROUPS) \
    $(wildcard include/config/BLK_CGROUP) \
    $(wildcard include/config/PSI) \
    $(wildcard include/config/PAGE_OWNER) \
    $(wildcard include/config/EVENTFD) \
    $(wildcard include/config/ARCH_HAS_CPU_PASID) \
    $(wildcard include/config/X86_BUS_LOCK_DETECT) \
    $(wildcard include/config/TASK_DELAY_ACCT) \
    $(wildcard include/config/STACKPROTECTOR) \
    $(wildcard include/config/ARCH_HAS_SCALED_CPUTIME) \
    $(wildcard include/config/VIRT_CPU_ACCOUNTING_GEN) \
    $(wildcard include/config/NO_HZ_FULL) \
    $(wildcard include/config/POSIX_CPUTIMERS) \
    $(wildcard include/config/POSIX_CPU_TIMERS_TASK_WORK) \
    $(wildcard include/config/KEYS) \
    $(wildcard include/config/SYSVIPC) \
    $(wildcard include/config/DETECT_HUNG_TASK) \
    $(wildcard include/config/IO_URING) \
    $(wildcard include/config/AUDIT) \
    $(wildcard include/config/AUDITSYSCALL) \
    $(wildcard include/config/DETECT_HUNG_TASK_BLOCKER) \
    $(wildcard include/config/UBSAN) \
    $(wildcard include/config/UBSAN_TRAP) \
    $(wildcard include/config/COMPACTION) \
    $(wildcard include/config/TASK_XACCT) \
    $(wildcard include/config/CPUSETS) \
    $(wildcard include/config/X86_CPU_RESCTRL) \
    $(wildcard include/config/FUTEX) \
    $(wildcard include/config/COMPAT) \
    $(wildcard include/config/PERF_EVENTS) \
    $(wildcard include/config/NUMA_BALANCING) \
    $(wildcard include/config/ARCH_HAS_LAZY_MMU_MODE) \
    $(wildcard include/config/FAULT_INJECTION) \
    $(wildcard include/config/LATENCYTOP) \
    $(wildcard include/config/FUNCTION_GRAPH_TRACER) \
    $(wildcard include/config/MEMCG) \
    $(wildcard include/config/UPROBES) \
    $(wildcard include/config/BCACHE) \
    $(wildcard include/config/VMAP_STACK) \
    $(wildcard include/config/SECURITY) \
    $(wildcard include/config/BPF_SYSCALL) \
    $(wildcard include/config/KSTACK_ERASE) \
    $(wildcard include/config/KSTACK_ERASE_METRICS) \
    $(wildcard include/config/X86_MCE) \
    $(wildcard include/config/KRETPROBES) \
    $(wildcard include/config/RETHOOK) \
    $(wildcard include/config/ARCH_HAS_PARANOID_L1D_FLUSH) \
    $(wildcard include/config/RV) \
    $(wildcard include/config/RV_PER_TASK_MONITORS) \
    $(wildcard include/config/USER_EVENTS) \
    $(wildcard include/config/UNWIND_USER) \
    $(wildcard include/config/SCHED_PROXY_EXEC) \
    $(wildcard include/config/SCHED_MM_CID) \
  /home/deva/linux-7.1.6/include/uapi/linux/sched.h \
  /home/deva/linux-7.1.6/include/linux/thread_info.h \
    $(wildcard include/config/GENERIC_ENTRY) \
    $(wildcard include/config/ARCH_HAS_PREEMPT_LAZY) \
    $(wildcard include/config/HAVE_ARCH_WITHIN_STACK_FRAMES) \
    $(wildcard include/config/SH) \
  /home/deva/linux-7.1.6/include/linux/restart_block.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/X86_FRED) \
  /home/deva/linux-7.1.6/include/asm-generic/thread_info_tif.h \
  /home/deva/linux-7.1.6/include/linux/pid_types.h \
  /home/deva/linux-7.1.6/include/linux/sem_types.h \
  /home/deva/linux-7.1.6/include/linux/shm.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/shmparam.h \
  /home/deva/linux-7.1.6/include/linux/kmsan_types.h \
  /home/deva/linux-7.1.6/include/linux/mutex_types.h \
    $(wildcard include/config/MUTEX_SPIN_ON_OWNER) \
    $(wildcard include/config/DEBUG_MUTEXES) \
  /home/deva/linux-7.1.6/include/linux/osq_lock.h \
  /home/deva/linux-7.1.6/include/linux/spinlock_types.h \
  /home/deva/linux-7.1.6/include/linux/rwlock_types.h \
  /home/deva/linux-7.1.6/include/linux/plist_types.h \
  /home/deva/linux-7.1.6/include/linux/hrtimer_types.h \
  /home/deva/linux-7.1.6/include/linux/timerqueue_types.h \
  /home/deva/linux-7.1.6/include/linux/rbtree_types.h \
  /home/deva/linux-7.1.6/include/linux/timer_types.h \
  /home/deva/linux-7.1.6/include/linux/seccomp_types.h \
    $(wildcard include/config/SECCOMP) \
  /home/deva/linux-7.1.6/include/linux/refcount_types.h \
  /home/deva/linux-7.1.6/include/linux/resource.h \
  /home/deva/linux-7.1.6/include/uapi/linux/resource.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/resource.h \
  /home/deva/linux-7.1.6/include/asm-generic/resource.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/resource.h \
  /home/deva/linux-7.1.6/include/linux/latencytop.h \
  /home/deva/linux-7.1.6/include/linux/sched/prio.h \
  /home/deva/linux-7.1.6/include/linux/sched/types.h \
  /home/deva/linux-7.1.6/include/linux/signal_types.h \
    $(wildcard include/config/OLD_SIGACTION) \
  /home/deva/linux-7.1.6/include/uapi/linux/signal.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/signal.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/signal.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/signal-defs.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/siginfo.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/siginfo.h \
  /home/deva/linux-7.1.6/include/linux/spinlock.h \
  /home/deva/linux-7.1.6/include/linux/bottom_half.h \
  /home/deva/linux-7.1.6/include/linux/lockdep.h \
    $(wildcard include/config/DEBUG_LOCKING_API_SELFTESTS) \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/mmiowb.h \
  /home/deva/linux-7.1.6/include/asm-generic/mmiowb.h \
    $(wildcard include/config/MMIOWB) \
  /home/deva/linux-7.1.6/include/linux/spinlock_up.h \
  /home/deva/linux-7.1.6/include/linux/rwlock.h \
  /home/deva/linux-7.1.6/include/linux/spinlock_api_up.h \
  /home/deva/linux-7.1.6/include/linux/syscall_user_dispatch_types.h \
  /home/deva/linux-7.1.6/include/linux/mm_types_task.h \
    $(wildcard include/config/ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/tlbbatch.h \
  /home/deva/linux-7.1.6/include/linux/netdevice_xmit.h \
    $(wildcard include/config/NET_ACT_MIRRED) \
    $(wildcard include/config/NET_EGRESS) \
    $(wildcard include/config/NF_DUP_NETDEV) \
  /home/deva/linux-7.1.6/include/linux/task_io_accounting.h \
    $(wildcard include/config/TASK_IO_ACCOUNTING) \
  /home/deva/linux-7.1.6/include/linux/posix-timers_types.h \
  /home/deva/linux-7.1.6/include/linux/rseq_types.h \
    $(wildcard include/config/RSEQ) \
    $(wildcard include/config/RSEQ_SLICE_EXTENSION) \
  /home/deva/linux-7.1.6/include/linux/irq_work_types.h \
  /home/deva/linux-7.1.6/include/linux/workqueue_types.h \
  /home/deva/linux-7.1.6/include/linux/seqlock_types.h \
  /home/deva/linux-7.1.6/include/linux/kcsan.h \
  /home/deva/linux-7.1.6/include/linux/rv.h \
    $(wildcard include/config/RV_LTL_MONITOR) \
    $(wildcard include/config/RV_HA_MONITOR) \
    $(wildcard include/config/RV_REACTORS) \
  /home/deva/linux-7.1.6/include/linux/uidgid_types.h \
  /home/deva/linux-7.1.6/include/linux/tracepoint-defs.h \
  /home/deva/linux-7.1.6/include/linux/unwind_deferred_types.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/kmap_size.h \
  /home/deva/linux-7.1.6/include/asm-generic/kmap_size.h \
    $(wildcard include/config/DEBUG_KMAP_LOCAL) \
  /home/deva/linux-7.1.6/include/generated/rq-offsets.h \
  /home/deva/linux-7.1.6/include/linux/sched/ext.h \
    $(wildcard include/config/EXT_GROUP_SCHED) \
  /home/deva/linux-7.1.6/include/vdso/time32.h \
  /home/deva/linux-7.1.6/include/vdso/time.h \
  /home/deva/linux-7.1.6/include/linux/uidgid.h \
    $(wildcard include/config/MULTIUSER) \
    $(wildcard include/config/USER_NS) \
  /home/deva/linux-7.1.6/include/linux/highuid.h \
  /home/deva/linux-7.1.6/include/linux/buildid.h \
    $(wildcard include/config/VMCORE_INFO) \
  /home/deva/linux-7.1.6/include/linux/kmod.h \
  /home/deva/linux-7.1.6/include/linux/umh.h \
  /home/deva/linux-7.1.6/include/linux/gfp.h \
    $(wildcard include/config/ZONE_DMA) \
    $(wildcard include/config/ZONE_DMA32) \
    $(wildcard include/config/ZONE_DEVICE) \
    $(wildcard include/config/CONTIG_ALLOC) \
  /home/deva/linux-7.1.6/include/linux/mmzone.h \
    $(wildcard include/config/ARCH_FORCE_MAX_ORDER) \
    $(wildcard include/config/PAGE_BLOCK_MAX_ORDER) \
    $(wildcard include/config/HAVE_GIGANTIC_FOLIOS) \
    $(wildcard include/config/HUGETLB_PAGE) \
    $(wildcard include/config/HUGETLB_PAGE_OPTIMIZE_VMEMMAP) \
    $(wildcard include/config/CMA) \
    $(wildcard include/config/MEMORY_ISOLATION) \
    $(wildcard include/config/ZSMALLOC) \
    $(wildcard include/config/UNACCEPTED_MEMORY) \
    $(wildcard include/config/IOMMU_SUPPORT) \
    $(wildcard include/config/SWAP) \
    $(wildcard include/config/TRANSPARENT_HUGEPAGE) \
    $(wildcard include/config/LRU_GEN_STATS) \
    $(wildcard include/config/LRU_GEN_WALKS_MMU) \
    $(wildcard include/config/MEMORY_FAILURE) \
    $(wildcard include/config/PAGE_EXTENSION) \
    $(wildcard include/config/DEFERRED_STRUCT_PAGE_INIT) \
    $(wildcard include/config/HAVE_MEMORYLESS_NODES) \
    $(wildcard include/config/SPARSEMEM_EXTREME) \
    $(wildcard include/config/SPARSEMEM_VMEMMAP_PREINIT) \
    $(wildcard include/config/HAVE_ARCH_PFN_VALID) \
  /home/deva/linux-7.1.6/include/linux/list_nulls.h \
  /home/deva/linux-7.1.6/include/linux/wait.h \
  /home/deva/linux-7.1.6/include/linux/seqlock.h \
    $(wildcard include/config/CC_IS_GCC) \
    $(wildcard include/config/GCC_VERSION) \
    $(wildcard include/config/UBSAN_ALIGNMENT) \
  /home/deva/linux-7.1.6/include/linux/mutex.h \
  /home/deva/linux-7.1.6/include/linux/debug_locks.h \
  /home/deva/linux-7.1.6/include/linux/pageblock-flags.h \
    $(wildcard include/config/HUGETLB_PAGE_SIZE_VARIABLE) \
  /home/deva/linux-7.1.6/include/linux/page-flags-layout.h \
  /home/deva/linux-7.1.6/include/generated/bounds.h \
  /home/deva/linux-7.1.6/include/linux/mm_types.h \
    $(wildcard include/config/HAVE_ALIGNED_STRUCT_PAGE) \
    $(wildcard include/config/SLAB_OBJ_EXT) \
    $(wildcard include/config/HUGETLB_PMD_PAGE_TABLE_SHARING) \
    $(wildcard include/config/SLAB_FREELIST_HARDENED) \
    $(wildcard include/config/USERFAULTFD) \
    $(wildcard include/config/ANON_VMA_NAME) \
    $(wildcard include/config/PER_VMA_LOCK) \
    $(wildcard include/config/HAVE_ARCH_COMPAT_MMAP_BASES) \
    $(wildcard include/config/MEMBARRIER) \
    $(wildcard include/config/FUTEX_PRIVATE_HASH) \
    $(wildcard include/config/ARCH_HAS_ELF_CORE_EFLAGS) \
    $(wildcard include/config/AIO) \
    $(wildcard include/config/MMU_NOTIFIER) \
    $(wildcard include/config/SPLIT_PMD_PTLOCKS) \
    $(wildcard include/config/IOMMU_MM_DATA) \
    $(wildcard include/config/KSM) \
    $(wildcard include/config/MM_ID) \
    $(wildcard include/config/CORE_DUMP_DEFAULT_ELF_HEADERS) \
  /home/deva/linux-7.1.6/include/linux/auxvec.h \
  /home/deva/linux-7.1.6/include/uapi/linux/auxvec.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/auxvec.h \
  /home/deva/linux-7.1.6/include/linux/kref.h \
  /home/deva/linux-7.1.6/include/linux/refcount.h \
  /home/deva/linux-7.1.6/include/linux/rbtree.h \
  /home/deva/linux-7.1.6/include/linux/rcupdate.h \
    $(wildcard include/config/TINY_RCU) \
    $(wildcard include/config/RCU_STRICT_GRACE_PERIOD) \
    $(wildcard include/config/RCU_LAZY) \
    $(wildcard include/config/RCU_STALL_COMMON) \
    $(wildcard include/config/VIRT_XFER_TO_GUEST_WORK) \
    $(wildcard include/config/RCU_NOCB_CPU) \
    $(wildcard include/config/TASKS_RCU_GENERIC) \
    $(wildcard include/config/TASKS_RUDE_RCU) \
    $(wildcard include/config/TREE_RCU) \
    $(wildcard include/config/DEBUG_OBJECTS_RCU_HEAD) \
    $(wildcard include/config/PROVE_RCU) \
    $(wildcard include/config/ARCH_WEAK_RELEASE_ACQUIRE) \
  /home/deva/linux-7.1.6/include/linux/context_tracking_irq.h \
    $(wildcard include/config/CONTEXT_TRACKING_IDLE) \
  /home/deva/linux-7.1.6/include/linux/rcutree.h \
  /home/deva/linux-7.1.6/include/linux/maple_tree.h \
    $(wildcard include/config/MAPLE_RCU_DISABLED) \
    $(wildcard include/config/DEBUG_MAPLE_TREE) \
  /home/deva/linux-7.1.6/include/linux/rwsem.h \
    $(wildcard include/config/RWSEM_SPIN_ON_OWNER) \
    $(wildcard include/config/DEBUG_RWSEMS) \
  /home/deva/linux-7.1.6/include/linux/completion.h \
  /home/deva/linux-7.1.6/include/linux/swait.h \
  /home/deva/linux-7.1.6/include/linux/uprobes.h \
  /home/deva/linux-7.1.6/include/linux/timer.h \
    $(wildcard include/config/DEBUG_OBJECTS_TIMERS) \
    $(wildcard include/config/NO_HZ_COMMON) \
  /home/deva/linux-7.1.6/include/linux/ktime.h \
  /home/deva/linux-7.1.6/include/linux/jiffies.h \
  /home/deva/linux-7.1.6/include/vdso/jiffies.h \
  /home/deva/linux-7.1.6/include/generated/timeconst.h \
  /home/deva/linux-7.1.6/include/vdso/ktime.h \
  /home/deva/linux-7.1.6/include/linux/timekeeping.h \
    $(wildcard include/config/POSIX_AUX_CLOCKS) \
    $(wildcard include/config/GENERIC_CMOS_UPDATE) \
  /home/deva/linux-7.1.6/include/linux/clocksource_ids.h \
  /home/deva/linux-7.1.6/include/linux/debugobjects.h \
    $(wildcard include/config/DEBUG_OBJECTS) \
    $(wildcard include/config/DEBUG_OBJECTS_FREE) \
  /home/deva/linux-7.1.6/include/linux/workqueue.h \
    $(wildcard include/config/DEBUG_OBJECTS_WORK) \
    $(wildcard include/config/FREEZER) \
    $(wildcard include/config/WQ_WATCHDOG) \
  /home/deva/linux-7.1.6/include/linux/percpu_counter.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/mmu.h \
    $(wildcard include/config/MODIFY_LDT_SYSCALL) \
    $(wildcard include/config/ADDRESS_MASKING) \
    $(wildcard include/config/BROADCAST_TLB_FLUSH) \
  /home/deva/linux-7.1.6/include/linux/page-flags.h \
    $(wildcard include/config/PAGE_IDLE_FLAG) \
    $(wildcard include/config/ARCH_USES_PG_ARCH_2) \
    $(wildcard include/config/ARCH_USES_PG_ARCH_3) \
    $(wildcard include/config/MIGRATION) \
  /home/deva/linux-7.1.6/include/linux/local_lock.h \
  /home/deva/linux-7.1.6/include/linux/local_lock_internal.h \
  /home/deva/linux-7.1.6/include/linux/zswap.h \
    $(wildcard include/config/ZSWAP) \
  /home/deva/linux-7.1.6/include/linux/sizes.h \
  /home/deva/linux-7.1.6/include/linux/memory_hotplug.h \
    $(wildcard include/config/ARCH_HAS_ADD_PAGES) \
    $(wildcard include/config/MEMORY_HOTREMOVE) \
  /home/deva/linux-7.1.6/include/linux/notifier.h \
  /home/deva/linux-7.1.6/include/linux/srcu.h \
    $(wildcard include/config/TINY_SRCU) \
    $(wildcard include/config/NEED_SRCU_NMI_SAFE) \
  /home/deva/linux-7.1.6/include/linux/rcu_segcblist.h \
  /home/deva/linux-7.1.6/include/linux/srcutree.h \
  /home/deva/linux-7.1.6/include/linux/rcu_node_tree.h \
    $(wildcard include/config/RCU_FANOUT) \
    $(wildcard include/config/RCU_FANOUT_LEAF) \
  /home/deva/linux-7.1.6/include/linux/topology.h \
    $(wildcard include/config/USE_PERCPU_NUMA_NODE_ID) \
    $(wildcard include/config/SCHED_SMT) \
    $(wildcard include/config/GENERIC_ARCH_TOPOLOGY) \
  /home/deva/linux-7.1.6/include/linux/arch_topology.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/topology.h \
    $(wildcard include/config/X86_LOCAL_APIC) \
    $(wildcard include/config/SCHED_MC_PRIO) \
  /home/deva/linux-7.1.6/include/asm-generic/topology.h \
  /home/deva/linux-7.1.6/include/linux/sysctl.h \
    $(wildcard include/config/SYSCTL) \
  /home/deva/linux-7.1.6/include/uapi/linux/sysctl.h \
  /home/deva/linux-7.1.6/include/linux/elf.h \
    $(wildcard include/config/ARCH_HAVE_EXTRA_ELF_NOTES) \
    $(wildcard include/config/ARCH_USE_GNU_PROPERTY) \
    $(wildcard include/config/ARCH_HAVE_ELF_PROT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/elf.h \
    $(wildcard include/config/X86_X32_ABI) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/ia32.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/user.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/user_64.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/fsgsbase.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/vdso.h \
  /home/deva/linux-7.1.6/include/uapi/linux/elf.h \
  /home/deva/linux-7.1.6/include/uapi/linux/elf-em.h \
  /home/deva/linux-7.1.6/include/linux/kobject.h \
    $(wildcard include/config/UEVENT_HELPER) \
    $(wildcard include/config/DEBUG_KOBJECT_RELEASE) \
  /home/deva/linux-7.1.6/include/linux/sysfs.h \
  /home/deva/linux-7.1.6/include/linux/kernfs.h \
    $(wildcard include/config/KERNFS) \
  /home/deva/linux-7.1.6/include/linux/idr.h \
  /home/deva/linux-7.1.6/include/linux/radix-tree.h \
  /home/deva/linux-7.1.6/include/linux/xarray.h \
    $(wildcard include/config/XARRAY_MULTI) \
  /home/deva/linux-7.1.6/include/linux/sched/mm.h \
    $(wildcard include/config/MMU_LAZY_TLB_REFCOUNT) \
    $(wildcard include/config/ARCH_HAS_MEMBARRIER_CALLBACKS) \
    $(wildcard include/config/ARCH_HAS_SYNC_CORE_BEFORE_USERMODE) \
  /home/deva/linux-7.1.6/include/linux/sync_core.h \
    $(wildcard include/config/ARCH_HAS_PREPARE_SYNC_CORE_CMD) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/sync_core.h \
  /home/deva/linux-7.1.6/include/linux/sched/coredump.h \
  /home/deva/linux-7.1.6/include/linux/kobject_ns.h \
  /home/deva/linux-7.1.6/include/linux/moduleparam.h \
    $(wildcard include/config/ALPHA) \
    $(wildcard include/config/PPC64) \
  /home/deva/linux-7.1.6/include/linux/rbtree_latch.h \
  /home/deva/linux-7.1.6/include/linux/error-injection.h \
  /home/deva/linux-7.1.6/include/asm-generic/error-injection.h \
  /home/deva/linux-7.1.6/include/linux/dynamic_debug.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/module.h \
    $(wildcard include/config/UNWINDER_ORC) \
  /home/deva/linux-7.1.6/include/asm-generic/module.h \
    $(wildcard include/config/HAVE_MOD_ARCH_SPECIFIC) \
  /home/deva/linux-7.1.6/include/linux/fs.h \
    $(wildcard include/config/FANOTIFY_ACCESS_PERMISSIONS) \
    $(wildcard include/config/READ_ONLY_THP_FOR_FS) \
    $(wildcard include/config/FS_POSIX_ACL) \
    $(wildcard include/config/CGROUP_WRITEBACK) \
    $(wildcard include/config/IMA) \
    $(wildcard include/config/FILE_LOCKING) \
    $(wildcard include/config/FSNOTIFY) \
    $(wildcard include/config/EPOLL) \
    $(wildcard include/config/FS_DAX) \
    $(wildcard include/config/BLOCK) \
    $(wildcard include/config/UNICODE) \
  /home/deva/linux-7.1.6/include/linux/fs/super.h \
  /home/deva/linux-7.1.6/include/linux/fs/super_types.h \
    $(wildcard include/config/QUOTA) \
    $(wildcard include/config/FS_ENCRYPTION) \
    $(wildcard include/config/FS_VERITY) \
  /home/deva/linux-7.1.6/include/linux/fs_dirent.h \
  /home/deva/linux-7.1.6/include/linux/errseq.h \
  /home/deva/linux-7.1.6/include/linux/list_lru.h \
  /home/deva/linux-7.1.6/include/linux/shrinker.h \
    $(wildcard include/config/SHRINKER_DEBUG) \
  /home/deva/linux-7.1.6/include/linux/list_bl.h \
  /home/deva/linux-7.1.6/include/linux/bit_spinlock.h \
  /home/deva/linux-7.1.6/include/linux/uuid.h \
  /home/deva/linux-7.1.6/include/linux/percpu-rwsem.h \
  /home/deva/linux-7.1.6/include/linux/rcuwait.h \
  /home/deva/linux-7.1.6/include/linux/sched/signal.h \
    $(wildcard include/config/SCHED_AUTOGROUP) \
    $(wildcard include/config/BSD_PROCESS_ACCT) \
    $(wildcard include/config/TASKSTATS) \
    $(wildcard include/config/STACK_GROWSUP) \
  /home/deva/linux-7.1.6/include/linux/rculist.h \
    $(wildcard include/config/PROVE_RCU_LIST) \
  /home/deva/linux-7.1.6/include/linux/signal.h \
    $(wildcard include/config/DYNAMIC_SIGFRAME) \
  /home/deva/linux-7.1.6/include/linux/sched/jobctl.h \
  /home/deva/linux-7.1.6/include/linux/sched/task.h \
    $(wildcard include/config/HAVE_EXIT_THREAD) \
    $(wildcard include/config/ARCH_WANTS_DYNAMIC_TASK_STRUCT) \
    $(wildcard include/config/HAVE_ARCH_THREAD_STRUCT_WHITELIST) \
  /home/deva/linux-7.1.6/include/linux/uaccess.h \
    $(wildcard include/config/ARCH_HAS_SUBPAGE_FAULTS) \
    $(wildcard include/config/HARDENED_USERCOPY) \
  /home/deva/linux-7.1.6/include/linux/fault-inject-usercopy.h \
    $(wildcard include/config/FAULT_INJECTION_USERCOPY) \
  /home/deva/linux-7.1.6/include/linux/nospec.h \
  /home/deva/linux-7.1.6/include/linux/ucopysize.h \
    $(wildcard include/config/HARDENED_USERCOPY_DEFAULT_ON) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/uaccess.h \
    $(wildcard include/config/CC_HAS_ASM_GOTO_OUTPUT) \
    $(wildcard include/config/CC_HAS_ASM_GOTO_TIED_OUTPUT) \
    $(wildcard include/config/ARCH_HAS_COPY_MC) \
    $(wildcard include/config/X86_INTEL_USERCOPY) \
  /home/deva/linux-7.1.6/include/linux/mmap_lock.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/smap.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/extable.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/tlbflush.h \
  /home/deva/linux-7.1.6/include/linux/mmu_notifier.h \
  /home/deva/linux-7.1.6/include/linux/interval_tree.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/smp.h \
    $(wildcard include/config/DEBUG_NMI_SELFTEST) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/invpcid.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pti.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pgtable.h \
    $(wildcard include/config/DEBUG_WX) \
    $(wildcard include/config/HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD) \
    $(wildcard include/config/ARCH_SUPPORTS_PMD_PFNMAP) \
    $(wildcard include/config/ARCH_SUPPORTS_PUD_PFNMAP) \
    $(wildcard include/config/HAVE_ARCH_SOFT_DIRTY) \
    $(wildcard include/config/ARCH_ENABLE_THP_MIGRATION) \
    $(wildcard include/config/PAGE_TABLE_CHECK) \
    $(wildcard include/config/X86_SGX) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/x86_init.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pkru.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/fpu/api.h \
    $(wildcard include/config/MATH_EMULATION) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/coco.h \
  /home/deva/linux-7.1.6/include/asm-generic/pgtable_uffd.h \
    $(wildcard include/config/PTE_MARKER_UFFD_WP) \
  /home/deva/linux-7.1.6/include/linux/page_table_check.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pgtable_64.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/fixmap.h \
    $(wildcard include/config/PROVIDE_OHCI1394_DMA_INIT) \
    $(wildcard include/config/X86_IO_APIC) \
    $(wildcard include/config/PCI_MMCONFIG) \
    $(wildcard include/config/ACPI_APEI_GHES) \
    $(wildcard include/config/INTEL_TXT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/apicdef.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/vsyscall.h \
  /home/deva/linux-7.1.6/include/asm-generic/fixmap.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/pgtable-invert.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/uaccess_64.h \
  /home/deva/linux-7.1.6/include/asm-generic/access_ok.h \
    $(wildcard include/config/ALTERNATE_USER_ADDRESS_SPACE) \
  /home/deva/linux-7.1.6/include/linux/cred.h \
  /home/deva/linux-7.1.6/include/linux/capability.h \
  /home/deva/linux-7.1.6/include/uapi/linux/capability.h \
  /home/deva/linux-7.1.6/include/linux/key.h \
    $(wildcard include/config/KEY_NOTIFICATIONS) \
    $(wildcard include/config/NET) \
  /home/deva/linux-7.1.6/include/linux/assoc_array.h \
    $(wildcard include/config/ASSOCIATIVE_ARRAY) \
  /home/deva/linux-7.1.6/include/linux/sched/user.h \
    $(wildcard include/config/VFIO_PCI_ZDEV_KVM) \
    $(wildcard include/config/IOMMUFD) \
    $(wildcard include/config/WATCH_QUEUE) \
  /home/deva/linux-7.1.6/include/linux/ratelimit.h \
  /home/deva/linux-7.1.6/include/linux/pid.h \
  /home/deva/linux-7.1.6/include/linux/rhashtable-types.h \
  /home/deva/linux-7.1.6/include/linux/posix-timers.h \
  /home/deva/linux-7.1.6/include/linux/alarmtimer.h \
    $(wildcard include/config/RTC_CLASS) \
  /home/deva/linux-7.1.6/include/linux/hrtimer.h \
    $(wildcard include/config/HIGH_RES_TIMERS) \
    $(wildcard include/config/TIME_LOW_RES) \
    $(wildcard include/config/TIMERFD) \
  /home/deva/linux-7.1.6/include/linux/hrtimer_defs.h \
  /home/deva/linux-7.1.6/include/linux/timerqueue.h \
  /home/deva/linux-7.1.6/include/linux/hrtimer_rearm.h \
    $(wildcard include/config/HRTIMER_REARM_DEFERRED) \
  /home/deva/linux-7.1.6/include/linux/rcuref.h \
  /home/deva/linux-7.1.6/include/linux/rcu_sync.h \
  /home/deva/linux-7.1.6/include/linux/quota.h \
    $(wildcard include/config/QUOTA_NETLINK_INTERFACE) \
  /home/deva/linux-7.1.6/include/uapi/linux/dqblk_xfs.h \
  /home/deva/linux-7.1.6/include/linux/dqblk_v1.h \
  /home/deva/linux-7.1.6/include/linux/dqblk_v2.h \
  /home/deva/linux-7.1.6/include/linux/dqblk_qtree.h \
  /home/deva/linux-7.1.6/include/linux/projid.h \
  /home/deva/linux-7.1.6/include/uapi/linux/quota.h \
  /home/deva/linux-7.1.6/include/linux/unicode.h \
  /home/deva/linux-7.1.6/include/linux/dcache.h \
  /home/deva/linux-7.1.6/include/linux/rculist_bl.h \
  /home/deva/linux-7.1.6/include/linux/lockref.h \
    $(wildcard include/config/ARCH_USE_CMPXCHG_LOCKREF) \
  /home/deva/linux-7.1.6/include/linux/stringhash.h \
    $(wildcard include/config/DCACHE_WORD_ACCESS) \
  /home/deva/linux-7.1.6/include/linux/hash.h \
    $(wildcard include/config/HAVE_ARCH_HASH) \
  /home/deva/linux-7.1.6/include/linux/vfsdebug.h \
    $(wildcard include/config/DEBUG_VFS) \
  /home/deva/linux-7.1.6/include/linux/wait_bit.h \
  /home/deva/linux-7.1.6/include/linux/kdev_t.h \
  /home/deva/linux-7.1.6/include/uapi/linux/kdev_t.h \
  /home/deva/linux-7.1.6/include/linux/path.h \
  /home/deva/linux-7.1.6/include/linux/semaphore.h \
  /home/deva/linux-7.1.6/include/linux/fcntl.h \
    $(wildcard include/config/ARCH_32BIT_OFF_T) \
  /home/deva/linux-7.1.6/include/uapi/linux/fcntl.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/fcntl.h \
  /home/deva/linux-7.1.6/include/uapi/asm-generic/fcntl.h \
  /home/deva/linux-7.1.6/include/uapi/linux/openat2.h \
  /home/deva/linux-7.1.6/include/linux/migrate_mode.h \
  /home/deva/linux-7.1.6/include/linux/delayed_call.h \
  /home/deva/linux-7.1.6/include/linux/ioprio.h \
  /home/deva/linux-7.1.6/include/linux/sched/rt.h \
  /home/deva/linux-7.1.6/include/linux/iocontext.h \
    $(wildcard include/config/BLK_ICQ) \
  /home/deva/linux-7.1.6/include/uapi/linux/ioprio.h \
  /home/deva/linux-7.1.6/include/linux/mount.h \
  /home/deva/linux-7.1.6/include/linux/mnt_idmapping.h \
  /home/deva/linux-7.1.6/include/linux/slab.h \
    $(wildcard include/config/FAILSLAB) \
    $(wildcard include/config/KFENCE) \
    $(wildcard include/config/SLUB_TINY) \
    $(wildcard include/config/SLUB_DEBUG) \
    $(wildcard include/config/SLAB_BUCKETS) \
    $(wildcard include/config/KVFREE_RCU_BATCHED) \
  /home/deva/linux-7.1.6/include/linux/percpu-refcount.h \
  /home/deva/linux-7.1.6/include/linux/kasan.h \
    $(wildcard include/config/KASAN_STACK) \
    $(wildcard include/config/KASAN_VMALLOC) \
  /home/deva/linux-7.1.6/include/linux/kasan-enabled.h \
    $(wildcard include/config/ARCH_DEFER_KASAN) \
  /home/deva/linux-7.1.6/include/linux/kasan-tags.h \
  /home/deva/linux-7.1.6/include/linux/rw_hint.h \
  /home/deva/linux-7.1.6/include/linux/file_ref.h \
  /home/deva/linux-7.1.6/include/uapi/linux/fs.h \
  /home/deva/linux-7.1.6/include/linux/fs_context.h \
  /home/deva/linux-7.1.6/include/linux/security.h \
    $(wildcard include/config/SECURITY_NETWORK) \
    $(wildcard include/config/SECURITY_PATH) \
    $(wildcard include/config/SECURITY_INFINIBAND) \
    $(wildcard include/config/SECURITY_NETWORK_XFRM) \
    $(wildcard include/config/SECURITYFS) \
  /home/deva/linux-7.1.6/include/linux/kernel_read_file.h \
  /home/deva/linux-7.1.6/include/linux/file.h \
  /home/deva/linux-7.1.6/include/linux/mm.h \
    $(wildcard include/config/HAVE_ARCH_MMAP_RND_BITS) \
    $(wildcard include/config/HAVE_ARCH_MMAP_RND_COMPAT_BITS) \
    $(wildcard include/config/PPC32) \
    $(wildcard include/config/RISCV_USER_CFI) \
    $(wildcard include/config/ARM64_GCS) \
    $(wildcard include/config/ARCH_HAS_PKEYS) \
    $(wildcard include/config/ARCH_PKEY_BITS) \
    $(wildcard include/config/PARISC) \
    $(wildcard include/config/SPARC64) \
    $(wildcard include/config/ARM64_MTE) \
    $(wildcard include/config/HAVE_ARCH_USERFAULTFD_MINOR) \
    $(wildcard include/config/MSEAL_SYSTEM_MAPPINGS) \
    $(wildcard include/config/FIND_NORMAL_PAGE) \
    $(wildcard include/config/SHMEM) \
    $(wildcard include/config/ARCH_HAS_PTE_SPECIAL) \
    $(wildcard include/config/ASYNC_KERNEL_PGTABLE_FREE) \
    $(wildcard include/config/SPLIT_PTE_PTLOCKS) \
    $(wildcard include/config/HIGHPTE) \
    $(wildcard include/config/DEBUG_VM_RB) \
    $(wildcard include/config/PAGE_POISONING) \
    $(wildcard include/config/INIT_ON_ALLOC_DEFAULT_ON) \
    $(wildcard include/config/INIT_ON_FREE_DEFAULT_ON) \
    $(wildcard include/config/DEBUG_PAGEALLOC) \
    $(wildcard include/config/ARCH_WANT_OPTIMIZE_DAX_VMEMMAP) \
    $(wildcard include/config/HUGETLBFS) \
    $(wildcard include/config/MAPPING_DIRTY_HELPERS) \
    $(wildcard include/config/PAGE_POOL) \
  /home/deva/linux-7.1.6/include/linux/pgalloc_tag.h \
  /home/deva/linux-7.1.6/include/linux/page_ext.h \
  /home/deva/linux-7.1.6/include/linux/stacktrace.h \
    $(wildcard include/config/ARCH_STACKWALK) \
    $(wildcard include/config/STACKTRACE) \
    $(wildcard include/config/HAVE_RELIABLE_STACKTRACE) \
  /home/deva/linux-7.1.6/include/linux/page_ref.h \
    $(wildcard include/config/DEBUG_PAGE_REF) \
  /home/deva/linux-7.1.6/include/linux/pgtable.h \
    $(wildcard include/config/ARCH_HAS_NONLEAF_PMD_YOUNG) \
    $(wildcard include/config/ARCH_HAS_HW_PTE_YOUNG) \
    $(wildcard include/config/GUP_GET_PXX_LOW_HIGH) \
    $(wildcard include/config/ARCH_WANT_PMD_MKWRITE) \
    $(wildcard include/config/HAVE_ARCH_HUGE_VMAP) \
    $(wildcard include/config/X86_ESPFIX64) \
  /home/deva/linux-7.1.6/include/linux/memremap.h \
    $(wildcard include/config/DEVICE_PRIVATE) \
    $(wildcard include/config/PCI_P2PDMA) \
  /home/deva/linux-7.1.6/include/linux/ioport.h \
  /home/deva/linux-7.1.6/include/linux/cacheinfo.h \
    $(wildcard include/config/ACPI_PPTT) \
    $(wildcard include/config/ARM) \
    $(wildcard include/config/ARCH_HAS_CPU_CACHE_ALIASING) \
  /home/deva/linux-7.1.6/include/linux/cpuhplock.h \
  /home/deva/linux-7.1.6/include/linux/iommu-debug-pagealloc.h \
    $(wildcard include/config/IOMMU_DEBUG_PAGEALLOC) \
  /home/deva/linux-7.1.6/include/linux/huge_mm.h \
    $(wildcard include/config/PGTABLE_HAS_HUGE_LEAVES) \
    $(wildcard include/config/PERSISTENT_HUGE_ZERO_FOLIO) \
  /home/deva/linux-7.1.6/include/linux/vmstat.h \
    $(wildcard include/config/VM_EVENT_COUNTERS) \
    $(wildcard include/config/DEBUG_TLBFLUSH) \
    $(wildcard include/config/PER_VMA_LOCK_STATS) \
  /home/deva/linux-7.1.6/include/linux/vm_event_item.h \
    $(wildcard include/config/BALLOON) \
    $(wildcard include/config/BALLOON_MIGRATION) \
    $(wildcard include/config/X86) \
    $(wildcard include/config/DEBUG_STACK_USAGE) \
  /home/deva/linux-7.1.6/include/linux/sockptr.h \
  /home/deva/linux-7.1.6/include/linux/bpf.h \
    $(wildcard include/config/CGROUP_BPF) \
    $(wildcard include/config/DEBUG_KERNEL) \
    $(wildcard include/config/DYNAMIC_FTRACE_WITH_JMP) \
    $(wildcard include/config/FINEIBT) \
    $(wildcard include/config/BPF_LSM) \
    $(wildcard include/config/BPF_JIT_ALWAYS_ON) \
    $(wildcard include/config/INET) \
  /home/deva/linux-7.1.6/include/uapi/linux/bpf.h \
    $(wildcard include/config/BPF_LIRC_MODE2) \
    $(wildcard include/config/EFFICIENT_UNALIGNED_ACCESS) \
    $(wildcard include/config/CGROUP_NET_CLASSID) \
    $(wildcard include/config/IP_ROUTE_CLASSID) \
    $(wildcard include/config/BPF_KPROBE_OVERRIDE) \
    $(wildcard include/config/XFRM) \
    $(wildcard include/config/SOCK_CGROUP_DATA) \
    $(wildcard include/config/IPV6) \
  /home/deva/linux-7.1.6/include/uapi/linux/bpf_common.h \
  /home/deva/linux-7.1.6/include/uapi/linux/filter.h \
  /home/deva/linux-7.1.6/include/crypto/sha2.h \
  /home/deva/linux-7.1.6/include/linux/kallsyms.h \
    $(wildcard include/config/KALLSYMS_ALL) \
    $(wildcard include/config/HAVE_FUNCTION_DESCRIPTORS) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/sections.h \
  /home/deva/linux-7.1.6/include/asm-generic/sections.h \
  /home/deva/linux-7.1.6/include/linux/bpfptr.h \
  /home/deva/linux-7.1.6/include/linux/btf.h \
  /home/deva/linux-7.1.6/include/linux/bsearch.h \
  /home/deva/linux-7.1.6/include/linux/btf_ids.h \
  /home/deva/linux-7.1.6/include/uapi/linux/btf.h \
  /home/deva/linux-7.1.6/include/linux/rcupdate_trace.h \
    $(wildcard include/config/TASKS_TRACE_RCU_NO_MB) \
  /home/deva/linux-7.1.6/include/linux/static_call.h \
  /home/deva/linux-7.1.6/include/linux/cpu.h \
    $(wildcard include/config/GENERIC_CPU_DEVICES) \
    $(wildcard include/config/PM_SLEEP_SMP) \
    $(wildcard include/config/PM_SLEEP_SMP_NONZERO_CPU) \
    $(wildcard include/config/ARCH_HAS_CPU_FINALIZE_INIT) \
    $(wildcard include/config/CPU_MITIGATIONS) \
  /home/deva/linux-7.1.6/include/linux/node.h \
    $(wildcard include/config/HMEM_REPORTING) \
  /home/deva/linux-7.1.6/include/linux/device.h \
    $(wildcard include/config/GENERIC_MSI_IRQ) \
    $(wildcard include/config/ENERGY_MODEL) \
    $(wildcard include/config/PINCTRL) \
    $(wildcard include/config/ARCH_HAS_DMA_OPS) \
    $(wildcard include/config/DMA_DECLARE_COHERENT) \
    $(wildcard include/config/DMA_CMA) \
    $(wildcard include/config/SWIOTLB) \
    $(wildcard include/config/SWIOTLB_DYNAMIC) \
    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_DEVICE) \
    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_CPU) \
    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_CPU_ALL) \
    $(wildcard include/config/DMA_OPS_BYPASS) \
    $(wildcard include/config/DMA_NEED_SYNC) \
    $(wildcard include/config/IOMMU_DMA) \
    $(wildcard include/config/PM) \
    $(wildcard include/config/PM_SLEEP) \
    $(wildcard include/config/OF) \
    $(wildcard include/config/DEVTMPFS) \
  /home/deva/linux-7.1.6/include/linux/dev_printk.h \
  /home/deva/linux-7.1.6/include/linux/energy_model.h \
  /home/deva/linux-7.1.6/include/linux/sched/cpufreq.h \
    $(wildcard include/config/CPU_FREQ) \
  /home/deva/linux-7.1.6/include/linux/sched/topology.h \
    $(wildcard include/config/SCHED_CLUSTER) \
    $(wildcard include/config/SCHED_MC) \
    $(wildcard include/config/CPU_FREQ_GOV_SCHEDUTIL) \
  /home/deva/linux-7.1.6/include/linux/sched/idle.h \
  /home/deva/linux-7.1.6/include/linux/sched/sd_flags.h \
  /home/deva/linux-7.1.6/include/linux/klist.h \
  /home/deva/linux-7.1.6/include/linux/pm.h \
    $(wildcard include/config/VT_CONSOLE_SLEEP) \
    $(wildcard include/config/CXL_SUSPEND) \
    $(wildcard include/config/PM_CLK) \
    $(wildcard include/config/PM_GENERIC_DOMAINS) \
  /home/deva/linux-7.1.6/include/linux/device/bus.h \
    $(wildcard include/config/ACPI) \
  /home/deva/linux-7.1.6/include/linux/device/class.h \
  /home/deva/linux-7.1.6/include/linux/device/devres.h \
    $(wildcard include/config/HAS_IOMEM) \
  /home/deva/linux-7.1.6/include/linux/device/driver.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/device.h \
  /home/deva/linux-7.1.6/include/linux/pm_wakeup.h \
  /home/deva/linux-7.1.6/include/linux/cpuhotplug.h \
    $(wildcard include/config/HOTPLUG_CORE_SYNC_DEAD) \
  /home/deva/linux-7.1.6/include/linux/cpu_smt.h \
    $(wildcard include/config/HOTPLUG_SMT) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/static_call.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/text-patching.h \
    $(wildcard include/config/UML_X86) \
  /home/deva/linux-7.1.6/include/linux/memcontrol.h \
    $(wildcard include/config/MEMCG_NMI_SAFETY_REQUIRES_ATOMIC) \
  /home/deva/linux-7.1.6/include/linux/cgroup.h \
    $(wildcard include/config/DEBUG_CGROUP_REF) \
    $(wildcard include/config/CGROUP_CPUACCT) \
    $(wildcard include/config/CGROUP_DATA) \
  /home/deva/linux-7.1.6/include/uapi/linux/cgroupstats.h \
  /home/deva/linux-7.1.6/include/uapi/linux/taskstats.h \
  /home/deva/linux-7.1.6/include/linux/seq_file.h \
  /home/deva/linux-7.1.6/include/linux/string_helpers.h \
  /home/deva/linux-7.1.6/include/linux/ctype.h \
  /home/deva/linux-7.1.6/include/linux/string_choices.h \
  /home/deva/linux-7.1.6/include/linux/ns_common.h \
  /home/deva/linux-7.1.6/include/linux/ns/ns_common_types.h \
    $(wildcard include/config/IPC_NS) \
    $(wildcard include/config/NET_NS) \
    $(wildcard include/config/PID_NS) \
    $(wildcard include/config/TIME_NS) \
    $(wildcard include/config/UTS_NS) \
  /home/deva/linux-7.1.6/include/linux/ns/nstree_types.h \
  /home/deva/linux-7.1.6/include/uapi/linux/nsfs.h \
  /home/deva/linux-7.1.6/include/linux/nsproxy.h \
  /home/deva/linux-7.1.6/include/linux/user_namespace.h \
    $(wildcard include/config/INOTIFY_USER) \
    $(wildcard include/config/FANOTIFY) \
    $(wildcard include/config/BINFMT_MISC) \
    $(wildcard include/config/PERSISTENT_KEYRINGS) \
  /home/deva/linux-7.1.6/include/linux/rculist_nulls.h \
  /home/deva/linux-7.1.6/include/linux/kernel_stat.h \
    $(wildcard include/config/GENERIC_IRQ_STAT_SNAPSHOT) \
  /home/deva/linux-7.1.6/include/linux/interrupt.h \
    $(wildcard include/config/IRQ_FORCED_THREADING) \
    $(wildcard include/config/GENERIC_IRQ_PROBE) \
  /home/deva/linux-7.1.6/include/linux/irqreturn.h \
  /home/deva/linux-7.1.6/include/linux/hardirq.h \
  /home/deva/linux-7.1.6/include/linux/context_tracking_state.h \
    $(wildcard include/config/CONTEXT_TRACKING_USER) \
    $(wildcard include/config/CONTEXT_TRACKING) \
    $(wildcard include/config/RCU_DYNTICKS_TORTURE) \
  /home/deva/linux-7.1.6/include/linux/ftrace_irq.h \
    $(wildcard include/config/HWLAT_TRACER) \
    $(wildcard include/config/OSNOISE_TRACER) \
  /home/deva/linux-7.1.6/include/linux/vtime.h \
    $(wildcard include/config/VIRT_CPU_ACCOUNTING) \
    $(wildcard include/config/IRQ_TIME_ACCOUNTING) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/hardirq.h \
    $(wildcard include/config/KVM_INTEL) \
    $(wildcard include/config/KVM) \
    $(wildcard include/config/GUEST_PERF_EVENTS) \
    $(wildcard include/config/X86_THERMAL_VECTOR) \
    $(wildcard include/config/X86_MCE_THRESHOLD) \
    $(wildcard include/config/X86_MCE_AMD) \
    $(wildcard include/config/X86_HV_CALLBACK_VECTOR) \
    $(wildcard include/config/HYPERV) \
    $(wildcard include/config/X86_POSTED_MSI) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/irq.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/irq_vectors.h \
    $(wildcard include/config/PCI_MSI) \
  /home/deva/linux-7.1.6/include/linux/cgroup-defs.h \
    $(wildcard include/config/EXT_SUB_SCHED) \
    $(wildcard include/config/CGROUP_NET_PRIO) \
  /home/deva/linux-7.1.6/include/linux/u64_stats_sync.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/local64.h \
  /home/deva/linux-7.1.6/include/asm-generic/local64.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/local.h \
  /home/deva/linux-7.1.6/include/linux/bpf-cgroup-defs.h \
  /home/deva/linux-7.1.6/include/linux/psi_types.h \
  /home/deva/linux-7.1.6/include/linux/kthread.h \
  /home/deva/linux-7.1.6/include/linux/cgroup_subsys.h \
    $(wildcard include/config/CGROUP_DEVICE) \
    $(wildcard include/config/CGROUP_FREEZER) \
    $(wildcard include/config/CGROUP_PERF) \
    $(wildcard include/config/CGROUP_HUGETLB) \
    $(wildcard include/config/CGROUP_PIDS) \
    $(wildcard include/config/CGROUP_RDMA) \
    $(wildcard include/config/CGROUP_MISC) \
    $(wildcard include/config/CGROUP_DMEM) \
    $(wildcard include/config/CGROUP_DEBUG) \
  /home/deva/linux-7.1.6/include/linux/cgroup_namespace.h \
  /home/deva/linux-7.1.6/include/linux/cgroup_refcnt.h \
  /home/deva/linux-7.1.6/include/linux/page_counter.h \
  /home/deva/linux-7.1.6/include/linux/vmpressure.h \
  /home/deva/linux-7.1.6/include/linux/eventfd.h \
  /home/deva/linux-7.1.6/include/uapi/linux/eventfd.h \
  /home/deva/linux-7.1.6/include/linux/writeback.h \
  /home/deva/linux-7.1.6/include/linux/flex_proportions.h \
  /home/deva/linux-7.1.6/include/linux/backing-dev-defs.h \
    $(wildcard include/config/DEBUG_FS) \
  /home/deva/linux-7.1.6/include/linux/blk_types.h \
    $(wildcard include/config/FAIL_MAKE_REQUEST) \
    $(wildcard include/config/BLK_CGROUP_IOCOST) \
    $(wildcard include/config/BLK_INLINE_ENCRYPTION) \
    $(wildcard include/config/BLK_DEV_INTEGRITY) \
  /home/deva/linux-7.1.6/include/linux/bvec.h \
  /home/deva/linux-7.1.6/include/linux/highmem.h \
  /home/deva/linux-7.1.6/include/linux/cacheflush.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cacheflush.h \
  /home/deva/linux-7.1.6/include/asm-generic/cacheflush.h \
  /home/deva/linux-7.1.6/include/linux/kmsan.h \
  /home/deva/linux-7.1.6/include/linux/dma-direction.h \
  /home/deva/linux-7.1.6/include/linux/highmem-internal.h \
  /home/deva/linux-7.1.6/include/linux/folio_batch.h \
  /home/deva/linux-7.1.6/include/linux/cfi.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/cfi.h \
    $(wildcard include/config/FINEIBT_BHI) \
    $(wildcard include/config/FUNCTION_PADDING_CFI) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/rqspinlock.h \
    $(wildcard include/config/QUEUED_SPINLOCKS) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/paravirt.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/paravirt-base.h \
    $(wildcard include/config/PARAVIRT_SPINLOCKS) \
  /home/deva/linux-7.1.6/arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/ZERO_CALL_USED_REGS) \
  /home/deva/linux-7.1.6/include/asm-generic/rqspinlock.h \
  /home/deva/linux-7.1.6/include/linux/bpf_types.h \
    $(wildcard include/config/NETFILTER_BPF_LINK) \
    $(wildcard include/config/XDP_SOCKETS) \
  /home/deva/linux-7.1.6/include/uapi/linux/lsm.h \
  /home/deva/linux-7.1.6/include/uapi/linux/unistd.h \
  /home/deva/linux-7.1.6/arch/x86/include/asm/unistd.h \
  /home/deva/linux-7.1.6/arch/x86/include/uapi/asm/unistd.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/uapi/asm/unistd_64.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/unistd_64_x32.h \
  /home/deva/linux-7.1.6/arch/x86/include/generated/asm/unistd_32_ia32.h \
  /home/deva/linux-7.1.6/include/linux/lsm/selinux.h \
    $(wildcard include/config/SECURITY_SELINUX) \
  /home/deva/linux-7.1.6/include/linux/lsm/smack.h \
    $(wildcard include/config/SECURITY_SMACK) \
  /home/deva/linux-7.1.6/include/linux/lsm/apparmor.h \
    $(wildcard include/config/SECURITY_APPARMOR) \
  /home/deva/linux-7.1.6/include/linux/lsm/bpf.h \
  /home/deva/linux-7.1.6/include/linux/pagemap.h \
  /home/deva/linux-7.1.6/include/linux/hugetlb_inline.h \

mod.o: $(deps_mod.o)

$(deps_mod.o):

mod.o: $(wildcard /home/deva/linux-7.1.6/tools/objtool/objtool)
