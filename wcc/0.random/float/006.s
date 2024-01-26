	.file	"float.c"
	.intel_syntax noprefix
	.text
	.globl	_putfloat
	.type	_putfloat, @function
_putfloat:
	push		rbp
	mov			rbp, rsp
	sub			rsp, 32
	movss		DWORD PTR -20[rbp], xmm0
	pxor		xmm0, xmm0
	comiss		xmm0, DWORD PTR -20[rbp]
	jbe			_putfloat0
	mov			edi, 45
	mov			eax, 0
	call		_putchar
	movss		xmm0, DWORD PTR -20[rbp]
	movss		xmm1, DWORD PTR _minus_1_float[rip]
	xorps		xmm0, xmm1
	movss		DWORD PTR -20[rbp], xmm0
_putfloat0:
	movss		xmm0, DWORD PTR -20[rbp]
	cvttss2si	eax, xmm0
	mov			DWORD PTR -12[rbp], eax
	mov			eax, DWORD PTR -12[rbp]
	mov			edi, eax
	mov			eax, 0
	call		_putnbr
	mov			edi, 46
	mov			eax, 0
	call		_putchar
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, DWORD PTR -12[rbp]
	movss		xmm0, DWORD PTR -20[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	mov			DWORD PTR -8[rbp], 0
	jmp			_putfloat1
_putfloat2:
	movss		xmm1, DWORD PTR -4[rbp]
	movss		xmm0, DWORD PTR _ten_float[rip]
	mulss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	eax, xmm0
	mov			edi, eax
	mov			eax, 0
	call		_putnbr
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	eax, xmm0
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, eax
	movss		xmm0, DWORD PTR -4[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	add			DWORD PTR -8[rbp], 1
_putfloat1:
	cmp			DWORD PTR -8[rbp], 5
	jle			_putfloat2
	nop
	leave
	ret

	.size	_putfloat, .-_putfloat
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.section	.rodata
	.align 16
_minus_1_float:
	.long	-2147483648
	.long	0
	.long	0
	.long	0
	.align 4
_ten_float:
	.long	1092616192
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
