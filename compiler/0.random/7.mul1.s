.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

main:
	push	rbp
	mov	rbp, rsp

	mov	QWORD PTR -4[rbp], 215754

	mov	rax, 1239999945
	sub	rax, QWORD PTR -4[rbp]
	mov	QWORD PTR -8[rbp], rax
	
	mov	rax, QWORD PTR -4[rbp]
	cdq
	mov rbx, 8
	idiv	rbx

	mov	QWORD PTR -4[rbp], rax
	mov	rax, QWORD PTR -4[rbp]
	
	pop	rbp
	ret
