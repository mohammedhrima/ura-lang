	.file	"001.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"> %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	movabs	rax, 1811111999111944441
	mov	QWORD PTR -8[rbp], rax
	mov	rcx, QWORD PTR -8[rbp]
	movabs	rdx, 1054099661354831521
	mov	rax, rcx
	imul	rdx
	mov	rax, rdx
	sar	rax, 2
	sar	rcx, 63
	mov	rdx, rcx
	sub	rax, rdx
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
