.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
	push	rbp
	mov		rbp, rsp

	mov		QWORD PTR -4[rbp], 10
	
	mov		rax, QWORD PTR -4[rbp]
	add		rax, 8
	mov		QWORD PTR -8[rbp], rax

	mov		rax, QWORD PTR -8[rbp]
	add		rax, QWORD PTR -4[rbp]
	leave
	ret
