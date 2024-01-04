main:
	ret

sum:
	movl	$0, %eax
	jmp	.L3
.L4:
	addq	(%rdi), %rax
	addq	$8, %rdi
	subq	$1, %rsi
.L3:
	testq	%rsi, %rsi
	jne	.L4
	ret

a:
	.quad	1
	.quad	2
	.quad	3
	.quad	4