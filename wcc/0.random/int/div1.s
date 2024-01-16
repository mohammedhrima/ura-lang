.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
	push	rbp
	mov	rbp, rsp
	
	mov	rax, 12	
	mov	QWORD PTR -4[rbp], 6
    /*convert double word to quad*/
	cdq

	idiv	QWORD PTR -4[rbp]
	
	pop	rbp
	ret
