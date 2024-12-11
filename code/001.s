	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	putnbr
	.type	putnbr, @function
putnbr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	cmp	DWORD PTR -20[rbp], 8
	jg	.L2
	mov	eax, DWORD PTR -20[rbp]
	add	eax, 48
	mov	BYTE PTR -1[rbp], al
	lea	rax, -1[rbp]
	mov	edx, 1
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	jmp	.L4
.L2:
	mov	eax, DWORD PTR -20[rbp]
	movsx	rdx, eax
	imul	rdx, rdx, 1717986919
	shr	rdx, 32
	mov	ecx, edx
	sar	ecx, 2
	cdq
	mov	eax, ecx
	sub	eax, edx
	mov	edi, eax
	call	putnbr
	mov	ecx, DWORD PTR -20[rbp]
	movsx	rax, ecx
	imul	rax, rax, 1717986919
	shr	rax, 32
	mov	edx, eax
	sar	edx, 2
	mov	eax, ecx
	sar	eax, 31
	sub	edx, eax
	mov	eax, edx
	sal	eax, 2
	add	eax, edx
	add	eax, eax
	sub	ecx, eax
	mov	edx, ecx
	mov	edi, edx
	call	putnbr
.L4:
	nop
	leave
	ret

	.size	putnbr, .-putnbr
	.section	.rodata
.LC0:
	.string	" "
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], 0
	jmp	.L6
.L7:
	add	DWORD PTR -4[rbp], 1
	mov	eax, DWORD PTR -4[rbp]
	mov	edi, eax
	call	putnbr
	mov	edx, 1
	lea	rax, .LC0[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
.L6:
	cmp	DWORD PTR -4[rbp], 4
	jle	.L7
	mov	eax, DWORD PTR -4[rbp]
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
