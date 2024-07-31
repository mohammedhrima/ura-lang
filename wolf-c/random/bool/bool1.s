	.file	"bool1.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"True"
.LC1:
	.string	"False"
	.text
	.globl	putbool
	.type	putbool, @function
putbool:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	eax, edi
	mov	BYTE PTR -4[rbp], al
	cmp	BYTE PTR -4[rbp], 0
	je	.L2
	mov	edx, 5
	lea	rax, .LC0[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	jmp	.L4
.L2:
	mov	edx, 6
	lea	rax, .LC1[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
.L4:
	nop
	leave
	ret
	.size	putbool, .-putbool
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	movss	xmm0, DWORD PTR .LC2[rip]
	movss	DWORD PTR -4[rbp], xmm0
	pxor	xmm0, xmm0
	movss	DWORD PTR -8[rbp], xmm0
	movss	xmm0, DWORD PTR -4[rbp]
	ucomiss	xmm0, DWORD PTR -8[rbp]
	setp	al
	mov	edx, 1
	movss	xmm0, DWORD PTR -4[rbp]
	ucomiss	xmm0, DWORD PTR -8[rbp]
	cmovne	eax, edx
	mov	BYTE PTR -9[rbp], al
	movzx	eax, BYTE PTR -9[rbp]
	mov	edi, eax
	call	putbool
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.section	.rodata
	.align 4
.LC2:
	.long	1082549862
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
