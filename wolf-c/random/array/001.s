	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	_memcpy
	.type	_memcpy, @function
_memcpy:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -40[rbp], rdi
	mov	QWORD PTR -48[rbp], rsi
	mov	QWORD PTR -56[rbp], rdx
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -48[rbp]
	mov	QWORD PTR -24[rbp], rax
	mov	QWORD PTR -8[rbp], 0
	jmp	.L2
.L3:
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	mov	rcx, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	add	rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov	BYTE PTR [rdx], al
	add	QWORD PTR -8[rbp], 1
.L2:
	mov	rax, QWORD PTR -8[rbp]
	cmp	rax, QWORD PTR -56[rbp]
	jb	.L3
	nop
	nop
	pop	rbp
	ret
	.size	_memcpy, .-_memcpy
	.globl	_memdub
	.type	_memdub, @function
_memdub:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	QWORD PTR -40[rbp], rdx
	mov	rdx, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	imul	rax, QWORD PTR -40[rbp]
	mov	rdx, rax
	mov	rcx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	_memcpy
	mov	rax, QWORD PTR -8[rbp]
	leave
	ret
	.size	_memdub, .-_memdub
	.section	.rodata
.LC0:
	.string	"%d "
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	DWORD PTR -48[rbp], 11
	mov	DWORD PTR -44[rbp], 22
	mov	DWORD PTR -40[rbp], 33
	mov	DWORD PTR -36[rbp], 44
	mov	DWORD PTR -32[rbp], 55
	lea	rax, -48[rbp]
	mov	edx, 4
	mov	esi, 5
	mov	rdi, rax
	call	_memdub
	mov	QWORD PTR -16[rbp], rax
	mov	DWORD PTR -4[rbp], 0
	jmp	.L7
.L8:
	mov	eax, DWORD PTR -4[rbp]
	lea	edx, 1[rax]
	mov	DWORD PTR -4[rbp], edx
	cdqe
	mov	eax, DWORD PTR -48[rbp+rax*4]
	mov	esi, eax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
.L7:
	cmp	DWORD PTR -4[rbp], 4
	jle	.L8
	mov	edi, 10
	call	putchar@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
