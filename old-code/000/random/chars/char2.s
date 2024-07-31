	.file	"char2.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"abc"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	
	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax

	mov	DWORD PTR -12[rbp], 6710628
	
	mov	QWORD PTR -22[rbp], 6710628
	mov	WORD PTR -14[rbp], 0

	mov	WORD PTR -25[rbp], 26727
	
	mov	BYTE PTR -23[rbp], 105
	
	mov	QWORD PTR -48[rbp], 7105386
	mov	QWORD PTR -40[rbp], 0
	mov	DWORD PTR -32[rbp], 0
	
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
