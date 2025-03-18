	.file	"003.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -12[rbp], 1
	lea	rax, -12[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	eax, DWORD PTR -12[rbp]
	lea	edx, 2000000[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], edx
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
