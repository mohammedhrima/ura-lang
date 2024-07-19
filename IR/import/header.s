_putstr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	mov		rax, QWORD PTR -8[rbp]
	mov		rdi, rax
	call	_strlen
	mov		rdx, rax
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rax
	mov		edi, 1
	call	write@PLT
	nop
	leave
	ret

_putchar:
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

_strlen:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -8[rbp], 0
	jmp		_strlen0
_strlen2:
	add		QWORD PTR -8[rbp], 1
_strlen0:
	cmp		QWORD PTR -24[rbp], 0
	je		_strlen1
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	 	_strlen2
_strlen1:
	mov		rax, QWORD PTR -8[rbp]
	pop		rbp
	ret
/*
_strcpy:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		DWORD PTR -4[rbp], 0
	jmp		_strcpy0
_strcpy1:
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
_strcpy0:
	cmp		QWORD PTR -24[rbp], 0
	je		_strcpy2
	cmp		QWORD PTR -32[rbp], 0
	je		_strcpy2
	mov		eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov		rax, QWORD PTR -32[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne		_strcpy1
_strcpy2:
	nop
	pop		rbp
	ret
*/

_strcmp:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		QWORD PTR -8[rbp], 0
	cmp		QWORD PTR -24[rbp], 0
	jne		_strcmp0
	cmp		QWORD PTR -32[rbp], 0
	jne		_strcmp0
	mov		eax, 1
	jmp		_strcmp1
_strcmp0:
	cmp		QWORD PTR -24[rbp], 0
	jne		_strcmp2
	cmp		QWORD PTR -32[rbp], 0
	je		_strcmp2
	mov		eax, 0
	jmp		_strcmp1
_strcmp2:
	cmp		QWORD PTR -24[rbp], 0
	je		_strcmp3
	cmp		QWORD PTR -32[rbp], 0
	jne		_strcmp3
	mov		eax, 0
	jmp		_strcmp1
_strcmp5:
	add		QWORD PTR -8[rbp], 1
_strcmp3:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		_strcmp4
	mov		rdx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		_strcmp4
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov		rcx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp		dl, al
	je		_strcmp5
_strcmp4:
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
_strcmp1:
	pop		rbp
	ret

_sign: .string "-"

_putnbr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	cmp		QWORD PTR -8[rbp], 0
	jns		_putnbr0
	neg		QWORD PTR -8[rbp]
	mov		edi, 45
	call	_putchar
_putnbr0:
	cmp		QWORD PTR -8[rbp], 9
	jle		_putnbr1
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
	call	_putnbr
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
	call	_putchar
	jmp		_putnbr2
_putnbr1:
	mov		rax, QWORD PTR -8[rbp]
	add		eax, 48
	movsx	eax, al
	mov		edi, eax
	call	_putchar
_putnbr2:
	nop
	leave
	ret

_minus_1_float:	.long	-2147483648
_ten_float: .long	1092616192
_putfloat:
	push		rbp
	mov			rbp, rsp
	sub			rsp, 32
	movss		DWORD PTR -20[rbp], xmm0
	pxor		xmm0, xmm0
	comiss		xmm0, DWORD PTR -20[rbp]
	jbe			_putfloat0
	mov			edi, 45
	call		_putchar
	movss		xmm0, DWORD PTR -20[rbp]
	movss		xmm1, DWORD PTR _minus_1_float[rip]
	xorps		xmm0, xmm1
	movss		DWORD PTR -20[rbp], xmm0
_putfloat0:	
	movss		xmm0, DWORD PTR -20[rbp]
	cvttss2si	rax, xmm0
	mov			QWORD PTR -16[rbp], rax
	mov			rax, QWORD PTR -16[rbp]
	mov			rdi, rax
	call		_putnbr
	mov			edi, 46
	call		_putchar
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, QWORD PTR -16[rbp]
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
	cvttss2si	rax, xmm0
	mov			rdi, rax
	call		_putnbr
	movss		xmm0, DWORD PTR -4[rbp]
	cvttss2si	rax, xmm0
	pxor		xmm1, xmm1
	cvtsi2ss	xmm1, rax
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

_True: .string	"True"
_False: .string	"False"
_putbool:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		eax, edi
	mov		BYTE PTR -4[rbp], al
	cmp		BYTE PTR -4[rbp], 0
	je		_putbool0
	lea		rax, _True[rip]
	mov		rdi, rax
	call	_putstr
	jmp		_putbool1
_putbool0:
	lea		rax, _False[rip]
	mov		rdi, rax
	call	_putstr
_putbool1:
	mov		eax, 0
	leave
	ret
/*
_strdup:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	cmp		QWORD PTR -24[rbp], 0
	jne		_strdup1
	mov		edi, 1
	call	_allocate
	jmp		_strdup0
_strdup1:
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	_strlen
	add		rax, 1
	mov		rdi, rax
	call	_allocate
	mov		QWORD PTR -8[rbp], rax
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	mov		rsi, rdx
	mov		rdi, rax
	call	_strcpy
	mov		rax, QWORD PTR -8[rbp]
_strdup0:
	leave
	ret
*/
/* allocation function */
.bss
	head:	.zero	8
	curr:	.zero	8
	pos:	.zero	8
.text

_allocate:
	push	rbp
	mov		rbp, rsp
	push	rbx
	sub		rsp, 40
	mov		QWORD PTR -40[rbp], rdi
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		_allocate0
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR curr[rip], rax
	jmp		_allocate1
_allocate0:
	mov		rax, QWORD PTR pos[rip]
	cmp		rax, 100
	jne		_allocate1
	mov		QWORD PTR pos[rip], 0
	mov		rbx, QWORD PTR curr[rip]
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR 800[rbx], rax
	mov		rax, QWORD PTR curr[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR curr[rip], rax
_allocate1:
	mov		rax, QWORD PTR -40[rbp]
	add		rax, 8
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -24[rbp], rax
	mov		rax, QWORD PTR -24[rbp]
	mov		rdx, QWORD PTR -40[rbp]
	mov		QWORD PTR [rax], rdx
	mov		rdx, QWORD PTR curr[rip]
	mov		rax, QWORD PTR pos[rip]
	lea		rcx, 1[rax]
	mov		QWORD PTR pos[rip], rcx
	mov		rcx, QWORD PTR -24[rbp]
	mov		QWORD PTR [rdx+rax*8], rcx
	mov		rax, QWORD PTR -24[rbp]
	add		rax, 8
	mov		rbx, QWORD PTR -8[rbp]
    leave
    ret

/*
_free_memory:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	jmp		_free_memory0
_free_memory3:
	mov		DWORD PTR -4[rbp], 0
	jmp		_free_memory1
_free_memory2:
	mov		rax, QWORD PTR head[rip]
	mov		edx, DWORD PTR -4[rbp]
	movsx	rdx, edx
	mov		rax, QWORD PTR [rax+rdx*8]
	mov		rdi, rax
	call	free@PLT
	add		DWORD PTR -4[rbp], 1
_free_memory1:
	cmp		DWORD PTR -4[rbp], 99
	jle		_free_memory2
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR head[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR -16[rbp]
	mov		rdi, rax
	call	free@PLT
_free_memory0:
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		_free_memory3
    leave
    ret
*/
/*_strjoin:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	_strlen
	mov		QWORD PTR -8[rbp], rax
	mov		rax, QWORD PTR -32[rbp]
	mov		rdi, rax
	call	_strlen
	mov		rdx, QWORD PTR -8[rbp]
	add		rax, rdx
	add		rax, 1
	mov		rdi, rax
	call	_allocate
	mov		QWORD PTR -16[rbp], rax
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -16[rbp]
	mov		rsi, rdx
	mov		rdi, rax
	call	_strcpy
	mov		rdx, QWORD PTR -16[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rdx, rax
	mov		rax, QWORD PTR -32[rbp]
	mov		rsi, rax
	mov		rdi, rdx
	call	_strcpy
	mov		rax, QWORD PTR -16[rbp]
	leave
	ret*/

.macro  _strjoin_macro  left right dest
	mov		rax, \left
   	mov     rdi, rax
	mov		rax, \right
	mov     rsi, rax
	call	_strjoin
	mov		\dest, rax
.endm

_strdup:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		rax, QWORD PTR -24[rbp]
	mov		rdi, rax
	call	_strlen
	add		rax, 1
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -16[rbp], rax
	mov		QWORD PTR -8[rbp], 0
	jmp		_strdup0
_strdup2:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	mov		rcx, QWORD PTR -16[rbp]
	mov		rdx, QWORD PTR -8[rbp]
	add		rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov		BYTE PTR [rdx], al
	add		QWORD PTR -8[rbp], 1
_strdup0:
	cmp		QWORD PTR -24[rbp], 0
	je		_strdup1
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne		_strdup2
_strdup1:
	mov		rax, QWORD PTR -16[rbp]
	leave
	ret

_memcpy:
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
	jmp		_memcpy0
_memcpy1:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	mov		rcx, QWORD PTR -16[rbp]
	mov		rdx, QWORD PTR -8[rbp]
	add		rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov		BYTE PTR [rdx], al
	add		QWORD PTR -8[rbp], 1
_memcpy0:
	mov		rax, QWORD PTR -8[rbp]
	cmp		rax, QWORD PTR -56[rbp]
	jb		_memcpy1
	nop
	nop
	pop		rbp
	ret

_memdub:
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
	call	_memcpy
	mov		rax, QWORD PTR -8[rbp]
	leave
	ret
