.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main
.type	main, @function

main:
	push	rbp
	mov	rbp, rsp

	mov	QWORD PTR -8[rbp], 12
	
	mov	rax, QWORD PTR -8[rbp]
	add	rax, 17
	mov	QWORD PTR -16[rbp], rax

	mov	rax, QWORD PTR -16[rbp]
	cdq

	idiv	QWORD PTR -8[rbp]
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	pop	rbp
	ret
