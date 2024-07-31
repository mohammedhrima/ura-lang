	.file	"int3.c"
	.intel_syntax noprefix
	.text
	.globl	putnbr
	.type	putnbr, @function
putnbr:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	ret
	.size	putnbr, .-putnbr
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 48

	mov		QWORD PTR -48[rbp], 11
	mov		QWORD PTR -40[rbp], 22
	mov		QWORD PTR -32[rbp], 33

	lea		rax, -48[rbp]			
	mov		QWORD PTR -8[rbp], rax  /* int *arr1 = &arr[0] */

	mov		QWORD PTR -16[rbp], 0	/* int i = 0 */
	mov		rax, QWORD PTR -16[rbp] /* mov i to rax*/
	lea		rdx, 0[0+rax*8]			/* add i*size + 0*/
	mov		rax, QWORD PTR -8[rbp]	/* add it ot the pointer*/
	add		rax, rdx
	mov		rax, QWORD PTR [rax]
	mov		rdi, rax
	call	putnbr
	mov		eax, 0
	leave
	ret

	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
