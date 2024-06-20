	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	processIntegers
	.type	processIntegers, @function
processIntegers:
	push	rbp
	mov		rbp, rsp
	
	mov		rax, QWORD PTR 56[rbp]
	add		rdx, rax
	mov		rax, QWORD PTR 64[rbp]
	add		rdx, rax
	mov		rax, QWORD PTR 72[rbp]
	add		rax, rdx
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -8[rbp]
	pop		rbp
	ret
	
.globl	main
main:
	push	rbp
	mov	rbp, rsp
	push	1
	push	2
	push	3

	call	processIntegers
	add	rsp, 64
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
