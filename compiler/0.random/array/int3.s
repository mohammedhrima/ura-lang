	.file	"int3.c"
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
	sub	rsp, 64
	mov	QWORD PTR -64[rbp], 11
	mov	QWORD PTR -56[rbp], 12
	mov	QWORD PTR -48[rbp], 13
	mov	QWORD PTR -40[rbp], 14
	lea	rax, -64[rbp]
	mov	QWORD PTR -32[rbp], rax
	lea	rax, -32[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	DWORD PTR -20[rbp], 2
	mov	eax, DWORD PTR -20[rbp]
	cdqe
	lea	rdx, 0[0+rax*8]
	mov	rax, QWORD PTR -16[rbp]
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
