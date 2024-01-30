	.file	"or_and.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	BYTE PTR -1[rbp], 0
	mov	BYTE PTR -2[rbp], 0
	mov	BYTE PTR -3[rbp], 0
	mov	BYTE PTR -4[rbp], 1
	mov	BYTE PTR -5[rbp], 1
	mov	BYTE PTR -6[rbp], 0
	
	cmp	BYTE PTR -1[rbp], 0
	je	.L2
	cmp	BYTE PTR -2[rbp], 0
	je	.L2
	cmp	BYTE PTR -3[rbp], 0
	je	.L2
	cmp	BYTE PTR -4[rbp], 0
	jne	.L3
.L2:
	cmp	BYTE PTR -4[rbp], 0
	je	.L4
	cmp	BYTE PTR -5[rbp], 0
	je	.L4
	cmp	BYTE PTR -6[rbp], 0
	je	.L4
.L3:
	mov	eax, 1
	jmp	.L5
.L4:
	mov	eax, 0
.L5:
	mov	BYTE PTR -7[rbp], al
	and	BYTE PTR -7[rbp], 1
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
