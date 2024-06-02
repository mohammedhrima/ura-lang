	.file	"7.mul.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	movss	xmm0, DWORD PTR .LC0[rip]
	movss	DWORD PTR -4[rbp], xmm0
	movss	xmm1, DWORD PTR -4[rbp]
	movss	xmm0, DWORD PTR .LC1[rip]
	addss	xmm0, xmm1
	movss	xmm1, DWORD PTR .LC2[rip]
	divss	xmm0, xmm1
	movss	DWORD PTR -8[rbp], xmm0
	movss	xmm0, DWORD PTR -8[rbp]
	cvttss2si	eax, xmm0
	pop	rbp
	ret
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1185446912
	.align 4
.LC1:
	.long	1065353216
	.align 4
.LC2:
	.long	1150001152
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
