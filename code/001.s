	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	hello
	.type	hello, @function
hello:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	mov	eax, 1
	pop	rbp
	ret
	.size	hello, .-hello
	.section	.rodata
.LC0:
	.string	"abc"
.LC1:
	.string	"efg"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rdi, .LC0[rip]
	call	hello
	lea	rax, .LC1[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
