	.file	"test.c"
	.text
	.globl	sum_rows1
	.type	sum_rows1, @function
sum_rows1:
.LFB0:
	.cfi_startproc
	movl	$0, %r9d
	jmp	.L2
.L4:
	movq	%r9, %rcx
	imulq	%rdx, %rcx
	addq	%rax, %rcx
	movsd	(%rdi,%rcx,8), %xmm0
	addsd	(%r8), %xmm0
	movsd	%xmm0, (%r8)
	addq	$1, %rax
.L3:
	cmpq	%rdx, %rax
	jl	.L4
	addq	$1, %r9
.L2:
	cmpq	%rdx, %r9
	jge	.L6
	leaq	(%rsi,%r9,8), %r8
	movq	$0x000000000, (%r8)
	movl	$0, %eax
	jmp	.L3
.L6:
	ret
	.cfi_endproc
.LFE0:
	.size	sum_rows1, .-sum_rows1
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
