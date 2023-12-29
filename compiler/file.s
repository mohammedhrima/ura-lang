.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

.LC0: .long	  1076953088
.LC1: .long	  1075576832
.LC2: .long	  1076101120
.LC3: .string "%f + %f = %f\n"

main:
   push     rbp
	mov	rbp, rsp

	/* a */
	mov		rax, QWORD PTR .LC0[rip]
	movq     xmm2, rax
   
	/* b */
	mov		rax, QWORD PTR .LC1[rip]
	movq	   xmm1, rax

    /* c */
	mov		rax, QWORD PTR .LC2[rip]
	movq	   xmm0, rax
	
	mov		eax, 1
	call	   printf@PLT
	xor		eax, eax
   
	leave
	ret

