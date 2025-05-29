	.file	"015.c"
	.intel_syntax noprefix
	.text
	.globl	m
	.type	m, @function
m:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	ret
	.size	m, .-m
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], 0
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	m
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14+deb12u1) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
