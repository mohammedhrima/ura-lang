putstr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	strlen
	mov		rdx, rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rax
	mov		edi, 1
	call	write
	nop
	leave
	ret

.sign: .string "-"

putnbr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	cmp		QWORD PTR -8[rbp], 0
	jns		putnbr0
	neg		QWORD PTR -8[rbp]
	mov		edi, 45
	call	putchar
putnbr0:
	cmp		QWORD PTR -8[rbp], 9
	jle		putnbr1
	mov		rcx, QWORD PTR -8[rbp]
	movabs	rdx, 7378697629483820647
	mov		rax, rcx
	imul	rdx
	mov		rax, rdx
	sar		rax, 2
	sar		rcx, 63
	mov		rdx, rcx
	sub		rax, rdx
	mov		rdi, rax
	call	putnbr
	mov		rcx, QWORD PTR -8[rbp]
	movabs	rdx, 7378697629483820647
	mov		rax, rcx
	imul	rdx
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
	mov		eax, edx
	add		eax, 48
	movsx	eax, al
	mov		edi, eax
	call	putchar
	jmp		putnbr2
putnbr1:
	mov		rax, QWORD PTR -8[rbp]
	add		eax, 48
	movsx	eax, al
	mov		edi, eax
	call	putchar
putnbr2:
	nop
	leave
	ret

.minus_1_float:	.long	-2147483648
.ten_float: .long	1092616192
putfloat:
	push		rbp
	mov			rbp, rsp
	sub			rsp, 32
	movss		DWORD PTR -20[rbp], xmm0
	pxor		xmm0, xmm0
	comiss		xmm0, DWORD PTR -20[rbp]
	jbe			putfloat0
	mov			edi, 45
	call		putchar
	movss		xmm0, DWORD PTR -20[rbp]
	movss		xmm1, DWORD PTR .minus_1_float[rip]
	xorps		xmm0, xmm1
	movss		DWORD PTR -20[rbp], xmm0
putfloat0:	
	movss		xmm0, DWORD PTR -20[rbp]
	cvttss2si	rax, xmm0
	mov			QWORD PTR -16[rbp], rax
	mov			rax, QWORD PTR -16[rbp]
	mov			rdi, rax
	call		putnbr
	mov			edi, 46
	call		putchar
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, QWORD PTR -16[rbp]
	movss		xmm0, DWORD PTR -20[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	mov			DWORD PTR -8[rbp], 0
	jmp			putfloat1
putfloat2:
	movss		xmm1, DWORD PTR -4[rbp]
	movss		xmm0, DWORD PTR .ten_float[rip]
	mulss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	mov			rdi, rax
	call		putnbr
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, rax
	movss		xmm0, DWORD PTR -4[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	add			DWORD PTR -8[rbp], 1
putfloat1:
	cmp			DWORD PTR -8[rbp], 5
	jle			putfloat2
	nop
	leave
	ret

.True: .string	"True"
.False: .string	"False"
putbool:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		eax, edi
	mov		BYTE PTR -4[rbp], al
	cmp		BYTE PTR -4[rbp], 0
	je		putbool0
	lea		rax, .True[rip]
	mov		rdi, rax
	call	putstr
	jmp		putbool1
putbool0:
	lea		rax, .False[rip]
	mov		rdi, rax
	call	putstr
putbool1:
	mov		eax, 0
	leave
	ret


/*
.strjoin:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	strlen
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -32[rbp]
	mov		rdi, rax
	call	strlen
	mov		rdx, QWORD PTR -8[rbp]
	add		rax, rdx
	add		rax, 1
	mov		rdi, rax
	call	.allocate
	mov		QWORD PTR -16[rbp], rax
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -16[rbp]
	mov		rsi, rdx
	mov		rdi, rax
	call	.strcpy
	mov		rdx, QWORD PTR -16[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rdx, rax
	mov		rax, QWORD PTR -32[rbp]
	mov		rsi, rax
	mov		rdi, rdx
	call	.strcpy
	mov		rax, QWORD PTR -16[rbp]
	leave
	ret*/


