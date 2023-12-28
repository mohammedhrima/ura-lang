.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

ft_strlen:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -8[rbp], 0
	jmp	.L2
.L3:
	add	QWORD PTR -8[rbp], 1
.L2:
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L3
	mov	rax, QWORD PTR -8[rbp]
	mov rsp, rbp
	pop	rbp
	ret

ft_putstr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	ft_strlen
	mov	QWORD PTR -8[rbp], rax
	mov	rdx, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	mov rsp, rbp
	pop rbp
	ret

.LC0:
	.string	"abc\n"

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	ft_putstr
	mov	eax, 0
	leave
	ret
