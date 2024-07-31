.intel_syntax noprefix

_putchar:
   push	    rbp
   mov	    rbp, rsp
   sub	    rsp, 16
   mov	    eax, edi
   mov	    BYTE PTR -4[rbp], al
   lea	    rax, -4[rbp]
   mov	    edx, 1
   mov	    rsi, rax
   mov	    edi, 1
   call	    write@PLT
   mov 		eax, 0
   leave
   ret

_sign: .string "-"

_putnbr:
   push	   	rbp
   mov		rbp, rsp
   sub		rsp, 32
   mov		QWORD PTR -24[rbp], rdi
   mov		QWORD PTR -8[rbp], 0
   cmp		QWORD PTR -24[rbp], 0
   jns		_putnbr0
   mov		edx, 1
   lea		rax, _sign[rip]
   mov		rsi, rax
   mov		edi, 1
   call	   	write@PLT
   neg		QWORD PTR -24[rbp]
_putnbr0:
   cmp		QWORD PTR -24[rbp], 9
   jg		_putnbr1
   mov		rax, QWORD PTR -24[rbp]
   add		eax, 48
   movsx	eax, al
   mov		edi, eax
   call	  	_putchar
   jmp		_putnbr2
_putnbr1:
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	   	rdx
   mov		rax, rdx
   sar		rax, 2
   sar		rcx, 63
   mov		rdx, rcx
   sub		rax, rdx
   mov		rdi, rax
   call	   	_putnbr
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	   	rdx
   sar		rdx, 2
   mov		rax, rcx
   sar		rax, 63
   sub		rdx, rax
   mov		rax, rdx
   sal		rax, 2
   add		rax, rdx
   add		rax, rax
   sub		rcx, rax
   mov		rdx, rcx
   mov		rdi, rdx
   call	   	_putnbr
_putnbr2:
   mov 		eax, 0
   leave
   ret

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

.globl	main

main:
	push		rbp
	mov			rbp, rsp
	sub			rsp, 16
	movss		xmm0, DWORD PTR .LC3[rip]
	movss		DWORD PTR -4[rbp], xmm0
	pxor		xmm1, xmm1
	cvtss2sd	xmm1, DWORD PTR -4[rbp]
	movq		rax, xmm1
	movq		xmm0, rax
	mov			eax, 1
	call		_putfloat
	mov			eax, 0
	leave
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
