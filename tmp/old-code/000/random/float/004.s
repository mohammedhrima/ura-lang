.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

/* 10 */
.LC0:
	.long	1092616192
/* 13 */
.LC1:
	.long	1090519040
/* 8 */
.LC2:
	.long	1095761920
.LC3:
	.string	"%f\n"

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16

	movss	xmm0, DWORD PTR .LC0[rip]
	movss	DWORD PTR -4[rbp], xmm0

	movss	xmm0, DWORD PTR -4[rbp]
	movss	xmm1, DWORD PTR .LC1[rip]
	addss	xmm0, xmm1

	movss	xmm1, DWORD PTR .LC2[rip]
	mulss	xmm0, xmm1

	movss	DWORD PTR -8[rbp], xmm0

	pxor	xmm2, xmm2
	cvtss2sd	xmm2, DWORD PTR -8[rbp]
	movq	rax, xmm2
	movq	xmm0, rax
	lea	rax, .LC3[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	
	mov	eax, 0
	leave
	ret
