	.file	"010.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	// 0 -> 16
	mov	DWORD PTR -16[rbp], 1
	mov	BYTE PTR -12[rbp], 2
	mov	DWORD PTR -8[rbp], 3
	mov	DWORD PTR -4[rbp], 4

	// 20 -> 28
	mov	BYTE PTR -28[rbp], 1
	mov	BYTE PTR -27[rbp], 2
	mov	DWORD PTR -24[rbp], 3
	mov	BYTE PTR -20[rbp], 4
	
	// 36 -> 48
	mov	QWORD PTR -48[rbp], 1
	mov	BYTE PTR -40[rbp], 2
	mov	BYTE PTR -39[rbp], 3
	mov	DWORD PTR -36[rbp], 4
	
	// 
	mov	QWORD PTR -64[rbp], 1
	mov	DWORD PTR -56[rbp], 2
	
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
