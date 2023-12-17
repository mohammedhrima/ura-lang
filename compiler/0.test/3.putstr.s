.intel_syntax noprefix
.text
	.globl	main

ft_strlen:
	push rbp
	mov	QWORD PTR -8[rbp], 0
	jmp	.L2
.L3:
	add	QWORD PTR -8[rbp], 1
.L2:
	mov	rdx, rdi
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx ebx, BYTE PTR [rax]
	cmp	ebx, 0
	jne	.L3
	mov	rax, QWORD PTR -8[rbp]
	pop	rbp
	ret

ft_putstr:
	push	rbp
	call ft_strlen
	mov	rdx, rax
	mov	QWORD PTR -8[rbp], rax
	mov	rax, rdi
	mov	rsi, rax
	mov	rdi, 1
	call	write@PLT
	mov	rax, QWORD PTR -8[rbp]
	leave
	ret

.LC0:
	.string	"abc\n"

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC0[rip]
	mov	QWORD PTR -8[rbp], rax
	mov	rdi, rax
	call	ft_putstr
	leave
	ret
	.section	.note.GNU-stack,"",@progbits
