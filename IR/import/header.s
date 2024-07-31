.putstr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	.strlen
	mov		rdx, rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rax
	mov		edi, 1
	call	write@PLT
	nop
	leave
	ret

.putchar:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		eax, edi
	mov		BYTE PTR -4[rbp], al
	lea		rax, -4[rbp]
	mov		edx, 1
	mov		rsi, rax
	mov		edi, 1
	call	write@PLT
	nop
	leave
	ret

.strlen:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -8[rbp], 0
	jmp		.strlen0
.strlen2:
	add		QWORD PTR -8[rbp], 1
.strlen0:
	cmp		QWORD PTR -24[rbp], 0
	je		.strlen1
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	 	.strlen2
.strlen1:
	mov		rax, QWORD PTR -8[rbp]
	pop		rbp
	ret

.strcpy:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		DWORD PTR -4[rbp], 0
	jmp		.strcpy0
.strcpy1:
	mov		eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov		rax, QWORD PTR -32[rbp]
	add		rax, rdx
	mov		edx, DWORD PTR -4[rbp]
	movsx	rcx, edx
	mov		rdx, QWORD PTR -24[rbp]
	add		rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov		BYTE PTR [rdx], al
	add		DWORD PTR -4[rbp], 1
.strcpy0:
	cmp		QWORD PTR -24[rbp], 0
	je		.strcpy2
	cmp		QWORD PTR -32[rbp], 0
	je		.strcpy2
	mov		eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov		rax, QWORD PTR -32[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne		.strcpy1
.strcpy2:
	nop
	pop		rbp
	ret

.strcmp:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		QWORD PTR -8[rbp], 0
	cmp		QWORD PTR -24[rbp], 0
	jne		.strcmp0
	cmp		QWORD PTR -32[rbp], 0
	jne		.strcmp0
	mov		eax, 1
	jmp		.strcmp1
.strcmp0:
	cmp		QWORD PTR -24[rbp], 0
	jne		.strcmp2
	cmp		QWORD PTR -32[rbp], 0
	je		.strcmp2
	mov		eax, 0
	jmp		.strcmp1
.strcmp2:
	cmp		QWORD PTR -24[rbp], 0
	je		.strcmp3
	cmp		QWORD PTR -32[rbp], 0
	jne		.strcmp3
	mov		eax, 0
	jmp		.strcmp1
.strcmp5:
	add		QWORD PTR -8[rbp], 1
.strcmp3:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		.strcmp4
	mov		rdx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		.strcmp4
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov		rcx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp		dl, al
	je		.strcmp5
.strcmp4:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov		rcx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp		dl, al
	sete	al
.strcmp1:
	pop		rbp
	ret

.sign: .string "-"

.putnbr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	cmp		QWORD PTR -8[rbp], 0
	jns		.putnbr0
	neg		QWORD PTR -8[rbp]
	mov		edi, 45
	call	.putchar
.putnbr0:
	cmp		QWORD PTR -8[rbp], 9
	jle		.putnbr1
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
	call	.putnbr
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
	call	.putchar
	jmp		.putnbr2
.putnbr1:
	mov		rax, QWORD PTR -8[rbp]
	add		eax, 48
	movsx	eax, al
	mov		edi, eax
	call	.putchar
.putnbr2:
	nop
	leave
	ret

.minus_1_float:	.long	-2147483648
.ten_float: .long	1092616192
.putfloat:
	push		rbp
	mov			rbp, rsp
	sub			rsp, 32
	movss		DWORD PTR -20[rbp], xmm0
	pxor		xmm0, xmm0
	comiss		xmm0, DWORD PTR -20[rbp]
	jbe			.putfloat0
	mov			edi, 45
	call		.putchar
	movss		xmm0, DWORD PTR -20[rbp]
	movss		xmm1, DWORD PTR .minus_1_float[rip]
	xorps		xmm0, xmm1
	movss		DWORD PTR -20[rbp], xmm0
.putfloat0:	
	movss		xmm0, DWORD PTR -20[rbp]
	cvttss2si	rax, xmm0
	mov			QWORD PTR -16[rbp], rax
	mov			rax, QWORD PTR -16[rbp]
	mov			rdi, rax
	call		.putnbr
	mov			edi, 46
	call		.putchar
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, QWORD PTR -16[rbp]
	movss		xmm0, DWORD PTR -20[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	mov			DWORD PTR -8[rbp], 0
	jmp			.putfloat1
.putfloat2:
	movss		xmm1, DWORD PTR -4[rbp]
	movss		xmm0, DWORD PTR .ten_float[rip]
	mulss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	mov			rdi, rax
	call		.putnbr
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, rax
	movss		xmm0, DWORD PTR -4[rbp]
	subss		xmm0, xmm1
	movss		DWORD PTR -4[rbp], xmm0
	add			DWORD PTR -8[rbp], 1
.putfloat1:
	cmp			DWORD PTR -8[rbp], 5
	jle			.putfloat2
	nop
	leave
	ret

.True: .string	"True"
.False: .string	"False"
.putbool:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		eax, edi
	mov		BYTE PTR -4[rbp], al
	cmp		BYTE PTR -4[rbp], 0
	je		.putbool0
	lea		rax, .True[rip]
	mov		rdi, rax
	call	.putstr
	jmp		.putbool1
.putbool0:
	lea		rax, .False[rip]
	mov		rdi, rax
	call	.putstr
.putbool1:
	mov		eax, 0
	leave
	ret

.strdup:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	cmp		QWORD PTR -24[rbp], 0
	jne		.strdup1
	mov		edi, 1
	call	.allocate
	jmp		.strdup0
.strdup1:
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	.strlen
	add		rax, 1
	mov		rdi, rax
	call	.allocate
	mov		QWORD PTR -8[rbp], rax
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rdx
	mov		rdi, rax
	call	.strcpy
	mov		rax, QWORD PTR -8[rbp]
.strdup0:
	leave
	ret

/* allocation function */
.bss
	.head:	.zero	8
	.curr:	.zero	8
	.pos:	.zero	8
.text

.allocate:
	push	rbp
	mov		rbp, rsp
	push	rbx
	sub		rsp, 40
	mov		QWORD PTR -40[rbp], rdi
	mov		rax, QWORD PTR .head[rip]
	test	rax, rax
	jne		.allocate0
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR .head[rip], rax
	mov		rax, QWORD PTR .head[rip]
	mov		QWORD PTR .curr[rip], rax
	jmp		.allocate1
.allocate0:
	mov		rax, QWORD PTR .pos[rip]
	cmp		rax, 100
	jne		.allocate1
	mov		QWORD PTR .pos[rip], 0
	mov		rbx, QWORD PTR .curr[rip]
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR 800[rbx], rax
	mov		rax, QWORD PTR .curr[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR .curr[rip], rax
.allocate1:
	mov		rax, QWORD PTR -40[rbp]
	add		rax, 8
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -24[rbp], rax
	mov		rax, QWORD PTR -24[rbp]
	mov		rdx, QWORD PTR -40[rbp]
	mov		QWORD PTR [rax], rdx
	mov		rdx, QWORD PTR .curr[rip]
	mov		rax, QWORD PTR .pos[rip]
	lea		rcx, 1[rax]
	mov		QWORD PTR .pos[rip], rcx
	mov		rcx, QWORD PTR -24[rbp]
	mov		QWORD PTR [rdx+rax*8], rcx
	mov		rax, QWORD PTR -24[rbp]
	add		rax, 8
	mov		rbx, QWORD PTR -8[rbp]
    leave
    ret

.free_memory:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	jmp		.free_memory0
.free_memory3:
	mov		DWORD PTR -4[rbp], 0
	jmp		.free_memory1
.free_memory2:
	mov		rax, QWORD PTR .head[rip]
	mov		edx, DWORD PTR -4[rbp]
	movsx	rdx, edx
	mov		rax, QWORD PTR [rax+rdx*8]
	mov		rdi, rax
	call	free@PLT
	add		DWORD PTR -4[rbp], 1
.free_memory1:
	cmp		DWORD PTR -4[rbp], 99
	jle		.free_memory2
	mov		rax, QWORD PTR .head[rip]
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR .head[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR .head[rip], rax
	mov		rax, QWORD PTR -16[rbp]
	mov		rdi, rax
	call	free@PLT
.free_memory0:
	mov		rax, QWORD PTR .head[rip]
	test	rax, rax
	jne		.free_memory3
    leave
    ret

.strjoin:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	.strlen
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -32[rbp]
	mov		rdi, rax
	call	.strlen
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
	ret

.macro  .strjoin_macro  left right dest
	mov		rax, \left
   	mov     rdi, rax
	mov		rax, \right
	mov     rsi, rax
	call	.strjoin
	mov		\dest, rax
.endm

.memcpy:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -40[rbp], rdi
	mov		QWORD PTR -48[rbp], rsi
	mov		QWORD PTR -56[rbp], rdx
	mov		rax, QWORD PTR -40[rbp]
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR -48[rbp]
	mov		QWORD PTR -24[rbp], rax
	mov		QWORD PTR -8[rbp], 0
	jmp		.memcpy0
.memcpy1:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	mov		rcx, QWORD PTR -16[rbp]
	mov		rdx, QWORD PTR -8[rbp]
	add		rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov		BYTE PTR [rdx], al
	add		QWORD PTR -8[rbp], 1
.memcpy0:
	mov		rax, QWORD PTR -8[rbp]
	cmp		rax, QWORD PTR -56[rbp]
	jb		.memcpy1
	nop
	nop
	pop		rbp
	ret

.memdub:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 48
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		QWORD PTR -40[rbp], rdx
	mov		rdx, QWORD PTR -40[rbp]
	mov		rax, QWORD PTR -32[rbp]
	mov		rsi, rdx
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -32[rbp]
	imul	rax, QWORD PTR -40[rbp]
	mov		rdx, rax
	mov		rcx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rcx
	mov		rdi, rax
	call	.memcpy
	mov		rax, QWORD PTR -8[rbp]
	leave
	ret
