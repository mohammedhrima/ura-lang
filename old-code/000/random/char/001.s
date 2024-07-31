	.file	"001.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%s \n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	movabs	rax, 7089059874119836257
	movabs	rdx, 7233733596956419171
	mov	QWORD PTR -64[rbp], rax
	mov	QWORD PTR -56[rbp], rdx
	movabs	rax, 7378413942531516005
	movabs	rdx, 7089059874119836257
	mov	QWORD PTR -48[rbp], rax
	mov	QWORD PTR -40[rbp], rdx
	movabs	rax, 7233733596956419171
	movabs	rdx, 7378413942531516005
	mov	QWORD PTR -32[rbp], rax
	mov	QWORD PTR -24[rbp], rdx
	mov	BYTE PTR -16[rbp], 0
	mov	BYTE PTR -64[rbp], 113
	lea	rax, -64[rbp]
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
