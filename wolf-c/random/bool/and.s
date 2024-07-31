	.file	"and.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp

	mov		DWORD PTR -4[rbp], 0
	mov		DWORD PTR -8[rbp], 10

	cmp		DWORD PTR -4[rbp], 0
	sete	al
	mov		BYTE PTR -9[rbp], al

	cmp		DWORD PTR -8[rbp], 0
	sete	al
	mov		BYTE PTR -10[rbp], al

	cmp		BYTE PTR -9[rbp], 0
	je		.L2
	cmp		BYTE PTR -10[rbp], 0
	je		.L2
	mov		eax, 1
	jmp		.L3
.L2:
	mov		eax, 0
.L3:
	mov		BYTE PTR -11[rbp], al
	and		BYTE PTR -11[rbp], 1
	mov		eax, 0
	pop		rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
