	.file	"float.c"
	.intel_syntax noprefix
	.text
	.globl	_putchar
	.type	_putchar, @function
_putchar:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	eax, edi
	mov	BYTE PTR -4[rbp], al
	lea	rax, -4[rbp]
	mov	edx, 1
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	nop
	leave
	ret
	.size	_putchar, .-_putchar
	.globl	_putnbr
	.type	_putnbr, @function
_putnbr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], rdi
	cmp	QWORD PTR -8[rbp], 0
	jns	.L3
	neg	QWORD PTR -8[rbp]
	mov	edi, 45
	call	_putchar
.L3:
	cmp	QWORD PTR -8[rbp], 9
	jle	.L4
	mov	rcx, QWORD PTR -8[rbp]
	movabs	rdx, 7378697629483820647
	mov	rax, rcx
	imul	rdx
	mov	rax, rdx
	sar	rax, 2
	sar	rcx, 63
	mov	rdx, rcx
	sub	rax, rdx
	mov	rdi, rax
	call	_putnbr
	mov	rcx, QWORD PTR -8[rbp]
	movabs	rdx, 7378697629483820647
	mov	rax, rcx
	imul	rdx
	sar	rdx, 2
	mov	rax, rcx
	sar	rax, 63
	sub	rdx, rax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	add	rax, rax
	sub	rcx, rax
	mov	rdx, rcx
	mov	eax, edx
	add	eax, 48
	movsx	eax, al
	mov	edi, eax
	call	_putchar
	jmp	.L6
.L4:
	mov	rax, QWORD PTR -8[rbp]
	add	eax, 48
	movsx	eax, al
	mov	edi, eax
	call	_putchar
.L6:
	nop
	leave
	ret
	.size	_putnbr, .-_putnbr
	.globl	_putfloat
	.type	_putfloat, @function
_putfloat:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	movss	DWORD PTR -20[rbp], xmm0
	pxor	xmm0, xmm0
	comiss	xmm0, DWORD PTR -20[rbp]
	jbe	.L8
	mov	edi, 45
	call	_putchar
	movss	xmm0, DWORD PTR -20[rbp]
	movss	xmm1, DWORD PTR .LC1[rip]
	xorps	xmm0, xmm1
	movss	DWORD PTR -20[rbp], xmm0
.L8:
	movss	xmm0, DWORD PTR -20[rbp]
	cvttss2si	rax, xmm0
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	_putnbr
	mov	edi, 46
	call	_putchar
	pxor	xmm1, xmm1
	cvtsi2ss	xmm1, QWORD PTR -16[rbp]
	movss	xmm0, DWORD PTR -20[rbp]
	subss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	mov	DWORD PTR -8[rbp], 0
	jmp	.L10
.L11:
	movss	xmm1, DWORD PTR -4[rbp]
	movss	xmm0, DWORD PTR .LC2[rip]
	mulss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	movss	xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	mov	rdi, rax
	call	_putnbr
	movss	xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	pxor	xmm1, xmm1
	cvtsi2ss	xmm1, rax
	movss	xmm0, DWORD PTR -4[rbp]
	subss	xmm0, xmm1
	movss	DWORD PTR -4[rbp], xmm0
	add	DWORD PTR -8[rbp], 1
.L10:
	cmp	DWORD PTR -8[rbp], 5
	jle	.L11
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
	movss	DWORD PTR -12[rbp], xmm0
	lea	rax, -12[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	movd	xmm0, eax
	call	_putfloat
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
