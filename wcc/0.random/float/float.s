	.file	"float.c"
	.intel_syntax noprefix
	.text
	.globl	_putfloat
	.type	_putfloat, @function
_putfloat:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	movss	DWORD PTR -20[rbp], xmm0
	pxor	xmm0, xmm0
	comiss	xmm0, DWORD PTR -20[rbp]
	jbe	.L2
	mov	edi, 45
	mov	eax, 0
	call	_putchar@PLT
	movss	xmm0, DWORD PTR -20[rbp]
	movss	xmm1, DWORD PTR .LC1[rip]
	xorps	xmm0, xmm1
	movss	DWORD PTR -20[rbp], xmm0
.L2:
	movss	xmm0, DWORD PTR -20[rbp]
	cvttss2si	eax, xmm0
	mov	DWORD PTR -12[rbp], eax
	mov	eax, DWORD PTR -12[rbp]
	mov	edi, eax
	mov	eax, 0
	call	_putnbr@PLT
	mov	edi, 46
	mov	eax, 0
	call	_putchar@PLT
	pxor	xmm1, xmm1
	cvtsi2ss	xmm1, DWORD PTR -12[rbp]
	movss	xmm0, DWORD PTR -20[rbp]
	subss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	mov	DWORD PTR -8[rbp], 0
	jmp	.L4
.L5:
	movss	xmm1, DWORD PTR -4[rbp]
	movss	xmm0, DWORD PTR .LC2[rip]
	mulss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	movss	xmm0, DWORD PTR -4[rbp]
	cvttss2si	eax, xmm0
	mov	edi, eax
	mov	eax, 0
	call	_putnbr@PLT
	movss	xmm0, DWORD PTR -4[rbp]
	cvttss2si	eax, xmm0
	pxor	xmm1, xmm1
	cvtsi2ss	xmm1, eax
	movss	xmm0, DWORD PTR -4[rbp]
	subss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	add	DWORD PTR -8[rbp], 1
.L4:
	cmp	DWORD PTR -8[rbp], 5
	jle	.L5
	nop
	nop
	leave
	ret
	.size	_putfloat, .-_putfloat
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	movss	xmm0, DWORD PTR .LC3[rip]
	movss	DWORD PTR -4[rbp], xmm0
	pxor	xmm1, xmm1
	cvtss2sd	xmm1, DWORD PTR -4[rbp]
	movq	rax, xmm1
	movq	xmm0, rax
	mov	eax, 1
	call	ft_putfloat@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.section	.rodata
	.align 16
.LC1:
	.long	-2147483648
	.long	0
	.long	0
	.long	0
	.align 4
.LC2:
	.long	1092616192
	.align 4
.LC3:
	.long	1106247680
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
