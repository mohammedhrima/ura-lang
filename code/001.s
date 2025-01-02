	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	putchar
	.type	putchar, @function
putchar:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	eax, edi
	mov	BYTE PTR -4[rbp], al
	lea	rax, -4[rbp]
	mov	edx, 1
	mov	rsi, rax
	mov	edi, 1
	mov	eax, 0
	call	write@PLT
	leave
	ret
	.size	putchar, .-putchar
	.section	.rodata
.INT_MIN:
	.string	"-2147483648"
	.text
	.globl	putnbr
	.type	putnbr, @function

putnbr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	DWORD PTR -4[rbp], 0
	cmp	DWORD PTR -20[rbp], -2147483648
	jne	.putnbr0
	mov	edx, 11
	lea	rax, .INT_MIN[rip]
	mov	rsi, rax
	mov	edi, 1
	mov	eax, 0
	call	write@PLT
	jmp	.putnbr1
.putnbr0:
	cmp	DWORD PTR -20[rbp], 0
	jns	.putnbr2
	mov	edi, 45
	call	putchar
	add	DWORD PTR -4[rbp], eax
	neg	DWORD PTR -20[rbp]
.putnbr2:
	cmp	DWORD PTR -20[rbp], 9
	jle	.putnbr3
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
	add	DWORD PTR -4[rbp], eax
.putnbr3:
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
	mov	eax, edx
	add	eax, 48
	movsx	eax, al
	mov	edi, eax
	call	putchar
	add	DWORD PTR -4[rbp], eax
	mov	eax, DWORD PTR -4[rbp]
.putnbr1:
	leave
	ret

	.size	putnbr, .-putnbr
	.section	.rodata
.LC1:
	.string	"Ffffff"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC1[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	strlen@PLT
	mov	DWORD PTR -12[rbp], eax
	mov	eax, DWORD PTR -12[rbp]
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
