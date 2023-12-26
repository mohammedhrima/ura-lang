	.file	"5.float.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC2:
	.string	"%f + %f = %f\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	movss	xmm0, DWORD PTR .LC0[rip]
	movss	DWORD PTR -4[rbp], xmm0
	movss	xmm0, DWORD PTR .LC1[rip]
	movss	DWORD PTR -8[rbp], xmm0
	movss	xmm0, DWORD PTR -4[rbp]
	addss	xmm0, DWORD PTR -8[rbp]
	movss	DWORD PTR -12[rbp], xmm0
	pxor	xmm1, xmm1
	cvtss2sd	xmm1, DWORD PTR -12[rbp]
	pxor	xmm0, xmm0
	cvtss2sd	xmm0, DWORD PTR -8[rbp]
	pxor	xmm3, xmm3
	cvtss2sd	xmm3, DWORD PTR -4[rbp]
	movq	rax, xmm3
	movapd	xmm2, xmm1
	movapd	xmm1, xmm0
	movq	xmm0, rax
	lea	rax, .LC2[rip]
	mov	rdi, rax
	mov	eax, 3
	call	printf@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1092616192
	.align 4
.LC1:
	.long	1088421888
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
