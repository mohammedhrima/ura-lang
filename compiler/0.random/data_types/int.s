	.file	"int.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -4[rbp], 10
	mov	eax, DWORD PTR -4[rbp]
	add	eax, 8
	mov	DWORD PTR -8[rbp], eax
	mov	eax, DWORD PTR -8[rbp]
	add	DWORD PTR -4[rbp], eax
	mov	eax, DWORD PTR -4[rbp]
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
