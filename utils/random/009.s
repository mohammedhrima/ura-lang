	.file	"009.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -20[rbp], 1
	mov	BYTE PTR -16[rbp], 2
	mov	DWORD PTR -12[rbp], 3
	lea	rax, -20[rbp]
	mov	QWORD PTR -8[rbp], rax
	
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], 11
	mov	rax, QWORD PTR -8[rbp]
	mov	BYTE PTR 4[rax], 22
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 8[rax], 33
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
