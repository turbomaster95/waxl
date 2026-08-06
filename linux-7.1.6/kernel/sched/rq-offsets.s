	.file	"rq-offsets.c"
# GNU C11 (Alpine 15.2.0) version 15.2.0 (x86_64-alpine-linux-musl)
#	compiled by GNU C version 15.2.0, GMP version 6.3.0, MPFR version 4.2.2, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-sse4a -m64 -mno-80387 -mno-fp-ret-in-387 -mpreferred-stack-boundary=3 -mskip-rax-setup -march=x86-64 -mtune=generic -mno-red-zone -mcmodel=kernel -Os -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -fms-extensions -fcf-protection=branch -fno-jump-tables -falign-jumps=1 -falign-loops=1 -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -fno-allow-store-data-races -fno-stack-protector -fomit-frame-pointer -ftrivial-auto-var-init=zero -fzero-init-padding-bits=all -fno-stack-clash-protection -fmin-function-alignment=16 -fstrict-flex-arrays=3 -fno-strict-overflow -fstack-check=no -fconserve-stack -fno-builtin-wcslen
	.text
	.section	.text.startup,"ax",@progbits
	.align 16
	.globl	main
	.type	main, @function
main:
	endbr64	
# kernel/sched/rq-offsets.c:9: 	DEFINE(RQ_nr_pinned, offsetof(struct rq, nr_pinned));
#APP
# 9 "kernel/sched/rq-offsets.c" 1
	
.ascii "->RQ_nr_pinned $2856 offsetof(struct rq, nr_pinned)"	#
# 0 "" 2
# kernel/sched/rq-offsets.c:12: }
#NO_APP
	xorl	%eax, %eax	#
	ret	
	.size	main, .-main
	.ident	"GCC: (Alpine 15.2.0) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x1
3:
	.align 8
4:
