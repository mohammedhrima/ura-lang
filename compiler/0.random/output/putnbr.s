.intel_syntax noprefix
.section	.note.GNU-stack,"",@progbits
.text
	.globl	main
STR0:
	.string	"-"

Putchar:
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
Putnbr:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -8[rbp], 0
	cmp		QWORD PTR -24[rbp], 0
	jns		Label0
	mov		edx, 1
	lea		rax, STR0[rip]
	mov		rsi, rax
	mov		edi, 1
	call	write@PLT
	neg		QWORD PTR -24[rbp]
Label0:
	cmp		QWORD PTR -24[rbp], 9
	jg		Label1
	mov		rax, QWORD PTR -24[rbp]
	add		eax, 48
	movsx	eax, al
	mov		edi, eax
	call	Putchar
	jmp		Lable2
Label1:
	mov		rcx, QWORD PTR -24[rbp]
	movabs	rdx, 7378697629483820647
	mov		rax, rcx
	imul	rdx
	mov		rax, rdx
	sar		rax, 2
	sar		rcx, 63
	mov		rdx, rcx
	sub		rax, rdx
	mov		rdi, rax
	call	Putnbr
	mov		rcx, QWORD PTR -24[rbp]
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
	mov		rdi, rdx
	call	Putnbr
Lable2:
	nop
	leave
	ret

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	QWORD PTR -8[rbp], 1235
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_putnbr
	mov	eax, 0
	leave
	ret

