	.file	"bool.c"
	.intel_syntax noprefix
	.text
	.globl	_strlen
	.type	_strlen, @function
_strlen:
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
	pop	rbp
	ret
	.size	_strlen, .-_strlen
	.globl	_putstr
	.type	_putstr, @function
_putstr:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_strlen
	mov	QWORD PTR -8[rbp], rax
	mov	rdx, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	nop
	leave
	ret
	.size	_putstr, .-_putstr
	.section	.rodata
.LC0:
	.string	"True"
.LC1:
	.string	"False"
	.text

_putbool:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	eax, edi
	mov	BYTE PTR -4[rbp], al
	cmp	BYTE PTR -4[rbp], 0
	je	.L7
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	_putstr
	jmp	.L9
.L7:
	lea	rax, .LC1[rip]
	mov	rdi, rax
	call	_putstr
.L9:
	nop
	leave
	ret
	.size	_putbool, .-_putbool
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	BYTE PTR -1[rbp], 1
	movzx	eax, BYTE PTR -1[rbp]
	mov	edi, eax
	call	_putbool
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
