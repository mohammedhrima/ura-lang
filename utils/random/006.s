	.file	"006.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -16[rbp], 0
	mov	QWORD PTR -8[rbp], 0
	mov	QWORD PTR -16[rbp], 125
	mov	DWORD PTR -8[rbp], 12
	mov	BYTE PTR -4[rbp], 13
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits


