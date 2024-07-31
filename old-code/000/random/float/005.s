	.file	"5.float.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC3:
	.string	"%f\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	movss	xmm0, DWORD PTR .LC0[rip]
	movss	DWORD PTR -4[rbp], xmm0
	movss	xmm1, DWORD PTR -4[rbp]
	movss	xmm0, DWORD PTR .LC1[rip]
	addss	xmm1, xmm0
	movss	xmm0, DWORD PTR .LC2[rip]
	divss	xmm0, xmm1
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
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1092616192
	.align 4
.LC1:
	.long	1090519040
	.align 4
.LC2:
	.long	1095761920
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
