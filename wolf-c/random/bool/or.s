	.file	"or.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -4[rbp], 1
	mov	DWORD PTR -8[rbp], 2
	mov	DWORD PTR -12[rbp], 3
	mov	DWORD PTR -16[rbp], 4
	mov	DWORD PTR -20[rbp], 5
	cmp	DWORD PTR -4[rbp], 5
	je	.L2
	cmp	DWORD PTR -8[rbp], 5
	je	.L2
	cmp	DWORD PTR -12[rbp], 5
	je	.L2
	cmp	DWORD PTR -16[rbp], 5
	je	.L2
	cmp	DWORD PTR -20[rbp], 5
	jne	.L3
.L2:
	mov	eax, 1
	jmp	.L4
.L3:
	mov	eax, 0
.L4:
	mov	BYTE PTR -21[rbp], al
	and	BYTE PTR -21[rbp], 1
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
