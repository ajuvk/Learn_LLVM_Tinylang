	.file	"Gcd.mod"
	.text
	.globl	_t3Gcd3GCD
	.p2align	4
	.type	_t3Gcd3GCD,@function
_t3Gcd3GCD:
	.cfi_startproc
	movq	%rdi, %rax
	.p2align	4
.LBB0_1:
	testq	%rsi, %rsi
	jne	.LBB0_1
	retq
.Lfunc_end0:
	.size	_t3Gcd3GCD, .Lfunc_end0-_t3Gcd3GCD
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
