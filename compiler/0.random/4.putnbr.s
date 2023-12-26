.intel_syntax noprefix

ft_putchar:
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

.LC0:
	.string	"-"

ft_putnbr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -8[rbp], 0
	cmp	QWORD PTR -24[rbp], 0
	jns	.L3
	mov	edx, 1
	lea	rax, .LC0[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	neg	QWORD PTR -24[rbp]
.L3:
	cmp	QWORD PTR -24[rbp], 9
	jg	.L4
	mov	rax, QWORD PTR -24[rbp]
	add	eax, 48
	movsx	eax, al
	mov	edi, eax
	call	ft_putchar
	jmp	.L6
.L4:
	mov	rcx, QWORD PTR -24[rbp]
	movabs	rdx, 7378697629483820647
	mov	rax, rcx
	imul	rdx
	mov	rax, rdx
	sar	rax, 2
	sar	rcx, 63
	mov	rdx, rcx
	sub	rax, rdx
	mov	rdi, rax
	call	ft_putnbr
	mov	rcx, QWORD PTR -24[rbp]
	movabs	rdx, 7378697629483820647
	mov	rax, rcx
	imul	rdx
	sar	rdx, 2
	mov	rax, rcx
	sar	rax, 63
	sub	rdx, rax
	mov	rax, rdx
	sal	rax, 2
	add	rax, rdx
	add	rax, rax
	sub	rcx, rax
	mov	rdx, rcx
	mov	rdi, rdx
	call	ft_putnbr
.L6:
	nop
	leave
	ret
	.size	ft_putnbr, .-ft_putnbr
	.section	.rodata
.LC1:
	.string	"\n"
	.text
	.globl	main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	/* declare num */
	mov	QWORD PTR -8[rbp], 1235
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_putnbr

	/* call write(1,"\n",1) */
	lea	rax, .LC1[rip]
	/*fd*/
	mov	rdi, 1
	/*pointer*/
	mov	rsi, rax
	/*length*/
	mov	rdx, 1
	call	write@PLT

	/*return 0*/
	mov	eax, 0
	leave
	ret
	.section	.note.GNU-stack,"",@progbits
