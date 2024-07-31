.intel_syntax noprefix
.include "../../import/header.s"

.text
	.globl	main

.LC0:
	.string	"abcdef"
.LC1:
	.string	"%zu\n"

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC0[rip]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	_strdup
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	_strdup
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	_strdup
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	sub	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	DWORD PTR -4[rbp], 0
	jmp	.L25
.L26:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -16[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	movsx	eax, al
	mov	edi, eax
	call	_putchar
	add	DWORD PTR -4[rbp], 1
.L25:
	mov	eax, DWORD PTR -4[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -16[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L26
	mov	edi, 10
	call	_putchar
	mov	eax, 0
	call	_free_memory
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
