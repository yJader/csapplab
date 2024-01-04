
rsum:
	testq	%rsi, %rsi ; if (count <= 0)
	jle	.L3

	pushq	%rbx
	movq	(%rdi), %rbx ; rbx = *a
	subq	$1, %rsi
	addq	$8, %rdi
	call	rsum
	addq	%rbx, %rax
	popq	%rbx
	ret
.L3:
	movl	$0, %eax
	ret

main:
	subq	$8, %rsp
	movl	$4, %esi
	leaq	a(%rip), %rdi
	call	rsum
	addq	$8, %rsp
	ret

a:
	.quad	2684395520
	.quad	184552192
	.quad	12583104
	.quad	851981

