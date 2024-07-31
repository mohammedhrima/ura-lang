.intel_syntax noprefix

.globl	main

processIntegers:
	push	rbp
	mov		rbp, rsp

	mov		rax, QWORD PTR 16[rbp]
	mov		QWORD PTR [rax], 77
	

	/* printf */
	mov 	rsi, [rax]
	lea		rax, .LC0[rip]
	mov		rdi, rax
	mov		eax, 0
	call	printf@PLT
	pop		rbp
	ret
	
.globl	main
.LC0:
	.string	">>>> %ld\n"
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 100

	mov		QWORD PTR -8[rbp], 1

	lea		rax, -8[rbp]
	push	rax
	call 	processIntegers


	/* printf */
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rax
	lea		rax, .LC0[rip]
	mov		rdi, rax
	mov		eax, 0
	call	printf@PLT
	mov		eax, 0
	
	
	leave
	ret
.section	.note.GNU-stack,"",@progbits
