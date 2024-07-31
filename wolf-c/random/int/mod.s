	.file	"mod.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -4[rbp], 10
	mov	eax, DWORD PTR -4[rbp]
	lea	edx, 7[rax]
	mov	eax, DWORD PTR -4[rbp]
	imul	eax, edx
	mov	DWORD PTR -8[rbp], eax
	mov	edx, DWORD PTR -4[rbp]
	mov	eax, edx
	sar	eax, 31
	shr	eax, 31
	add	edx, eax
	and	edx, 1
	sub	edx, eax
	mov	DWORD PTR -12[rbp], edx
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
