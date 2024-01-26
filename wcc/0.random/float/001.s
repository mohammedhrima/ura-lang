.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl main

FLT0: .long	1073741824 	/*  2.0 */
FLT1: .long	-1082130432 /* -1.0 */
FLT2: .long	1084227584  /*  5.0 */
STR0: .string "%f + %f = %f\n"

main:
	push	rbp
	mov		rbp, rsp

	/* x */
	movss	xmm0, DWORD PTR FLT0[rip]
	movss	DWORD PTR -4[rbp], xmm0
	/* y */
	movss	xmm0, DWORD PTR FLT1[rip]
	movss	DWORD PTR -8[rbp], xmm0
	
	/* x + y */
	movss	xmm0, DWORD PTR -4[rbp]
	movss	xmm1, DWORD PTR -8[rbp]
	addss	xmm0, xmm1

	/* z = x + y + 5.0 */
	movss	xmm1, DWORD PTR FLT2[rip]
	addss	xmm0, xmm1
	movss	DWORD PTR -12[rbp], xmm0


	mov		rax, QWORD PTR FLT0[rip]
	movq	xmm2, rax
	
	mov		rax, QWORD PTR FLT1[rip]
	movq	xmm1, rax
	
	mov		rax, QWORD PTR FLT2[rip]
	movq	xmm0, rax

	mov		eax, 3
	lea		rdi, STR0[rip]
	
	call	printf@PLT

	
	leave
	ret

