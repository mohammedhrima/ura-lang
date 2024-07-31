	.file	"002.c"
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
.L4:
	add	QWORD PTR -8[rbp], 1
.L2:
	cmp	QWORD PTR -24[rbp], 0
	je	.L3
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L4
.L3:
	mov	rax, QWORD PTR -8[rbp]
	pop	rbp
	ret
	.size	_strlen, .-_strlen
	.globl	_strdup
	.type	_strdup, @function
_strdup:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_strlen
	add	rax, 1
	mov	esi, 1
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	QWORD PTR -8[rbp], 0
	jmp	.L7
.L9:
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	mov	rcx, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	add	rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov	BYTE PTR [rdx], al
	add	QWORD PTR -8[rbp], 1
.L7:
	cmp	QWORD PTR -24[rbp], 0
	je	.L8
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	jne	.L9
.L8:
	mov	rax, QWORD PTR -16[rbp]
	leave
	ret
	.size	_strdup, .-_strdup
	.section	.rodata
.LC0:
	.string	"abcde"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	_strdup
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	movzx	eax, BYTE PTR 2[rax]
	mov	BYTE PTR -9[rbp], al
	mov	rax, QWORD PTR -8[rbp]
	add	rax, 3
	mov	BYTE PTR [rax], 122
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	free@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
