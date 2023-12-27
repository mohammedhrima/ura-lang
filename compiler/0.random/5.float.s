.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

.LC0:
	.long	0
	.long	1076953088
	.align 8
.LC1:
	.long	0
	.long	1075576832
	.align 8
.LC2:
	.long	0
	.long	1076101120
.LC3:
	.string	"%f + %f = %f\n"
.LC4:
	.string	"%lf + %f = %f\n"

main:
	sub		rsp, 8

	/* a = 10 */
	mov		rax, QWORD PTR .LC0[rip]
	lea		rdi, .LC3[rip]
	movq	xmm2, rax
	/* b = 7 */
	mov		rax, QWORD PTR .LC1[rip]
	movq	xmm1, rax
	
	mov		rax, QWORD PTR .LC2[rip]
	movq	xmm0, rax
	mov		eax, 3
	
	call	printf@PLT

	mov		rax, QWORD PTR .LC0[rip]
	lea		rdi, .LC4[rip]
	movq	xmm2, rax
	
	mov		rax, QWORD PTR .LC1[rip]
	movq	xmm1, rax
	
	mov		rax, QWORD PTR .LC2[rip]
	movq	xmm0, rax
	mov		eax, 3
	
	call	printf@PLT
	
	xor		eax, eax
	add		rsp, 8
	ret

