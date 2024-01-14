	.file	"int2.c"
	.intel_syntax noprefix
	.text
	.globl	putnbr
	.type	putnbr, @function
putnbr:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	ret
	.size	putnbr, .-putnbr
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	QWORD PTR -48[rbp], 11
	mov	QWORD PTR -40[rbp], 12
	mov	QWORD PTR -32[rbp], 13
	mov	QWORD PTR -24[rbp], 14
	lea	rax, -48[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	DWORD PTR -12[rbp], 2
	mov	eax, DWORD PTR -12[rbp]
	cdqe
	lea	rdx, 0[0+rax*8]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	rdi, rax
	call	putnbr
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
