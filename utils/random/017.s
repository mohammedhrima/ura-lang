	.file	"017.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	""
.LC1:
	.string	"a"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -16[rbp], 0
	mov	QWORD PTR -8[rbp], 0
	lea	rax, .LC0[rip]
	mov	QWORD PTR -16[rbp], rax
	mov	QWORD PTR -32[rbp], 0
	mov	QWORD PTR -24[rbp], 0
	lea	rax, .LC1[rip]
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14+deb12u1) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
