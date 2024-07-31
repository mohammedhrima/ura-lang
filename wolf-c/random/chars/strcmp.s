	.file	"strcmp.c"
	.intel_syntax noprefix
	.text
	.globl	_strcmp
	.type	_strcmp, @function
_strcmp:
	push	rbp
	mov		rbp, rsp
	mov		QWORD PTR -24[rbp], rdi
	mov		QWORD PTR -32[rbp], rsi
	mov		QWORD PTR -8[rbp], 0
	cmp		QWORD PTR -24[rbp], 0
	jne		_strcmp0
	cmp		QWORD PTR -32[rbp], 0
	jne		_strcmp0
	mov		eax, 1
	jmp		_strcmp1
_strcmp0:
	cmp		QWORD PTR -24[rbp], 0
	jne		_strcmp2
	cmp		QWORD PTR -32[rbp], 0
	je		_strcmp2
	mov		eax, 0
	jmp		_strcmp1
_strcmp2:
	cmp		QWORD PTR -24[rbp], 0
	je		_strcmp3
	cmp		QWORD PTR -32[rbp], 0
	jne		_strcmp3
	mov		eax, 0
	jmp		_strcmp1
_strcmp5:
	add		QWORD PTR -8[rbp], 1
_strcmp3:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		_strcmp4
	mov		rdx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	eax, BYTE PTR [rax]
	test	al, al
	je		_strcmp4
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov		rcx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp		dl, al
	je		_strcmp5
_strcmp4:
	mov		rdx, QWORD PTR -24[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov		rcx, QWORD PTR -32[rbp]
	mov		rax, QWORD PTR -8[rbp]
	add		rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp		dl, al
	sete	al
_strcmp1:
	pop		rbp
	ret
	.size	_strcmp, .-_strcmp

.section	.rodata
.LC0:
	.string	"abcd"
.LC1:
	.string	"> %d\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	lea	rax, .LC0[rip]
	mov	rsi, rax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	_strcmp
	movzx	eax, al
	mov	esi, eax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
