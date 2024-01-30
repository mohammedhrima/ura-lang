	.file	"int4.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], 4
	mov		rax, QWORD PTR -8[rbp]
	add		rax, 1
	mov		esi, 8
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR -16[rbp]
	mov		QWORD PTR [rax], 0
	mov		rax, QWORD PTR -16[rbp]
	add		rax, 8
	mov		QWORD PTR [rax], 1
	mov		rax, QWORD PTR -16[rbp]
	add		rax, 16
	mov		QWORD PTR [rax], 2
	mov		rax, QWORD PTR -16[rbp]
	add		rax, 24
	mov		QWORD PTR [rax], 3
	mov		eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
