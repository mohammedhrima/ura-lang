	.file	"006.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	putnbr
	.type	putnbr, @function
putnbr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], edi
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	nop
	leave
	ret
	.size	putnbr, .-putnbr
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	pxor	xmm0, xmm0
	movaps	XMMWORD PTR -64[rbp], xmm0
	movaps	XMMWORD PTR -48[rbp], xmm0
	movaps	XMMWORD PTR -32[rbp], xmm0
	movups	XMMWORD PTR -20[rbp], xmm0
	mov	eax, DWORD PTR -64[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -60[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -56[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -52[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -48[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -44[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -40[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -36[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -32[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -28[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -24[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -20[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -16[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -12[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, DWORD PTR -8[rbp]
	mov	edi, eax
	call	putnbr
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
