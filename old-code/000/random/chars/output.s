.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

ft_putchar:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		rax, rdi
	mov		BYTE PTR -4[rbp], al
	lea		rax, -4[rbp]
	mov		rdx, 1
	mov		rsi, rax
	mov		rdi, 1
	call	write@PLT
	nop
	leave
	ret

func:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		QWORD PTR -8[rbp], rdi
	cmp		QWORD PTR -8[rbp], 9
	jle		L3
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
	call	func
	jmp		L5
L3:
	mov		rax, QWORD PTR -8[rbp]
	add		rax, 48
	movsx	rax, al
	mov		rdi, rax
	call	ft_putchar
L5:
	nop
	leave
	ret

main:
	push	rbp
	mov		rbp, rsp
	mov		rdi, 11
	call	func
	mov		rdi, 10
	call	ft_putchar
	mov		rax, 0
	pop		rbp
	ret
