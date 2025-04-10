	.file	"012.c"
	.intel_syntax noprefix
	.text
	.globl	swap
	.type	swap, @function
swap:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR [rax]
	mov	DWORD PTR -4[rbp], eax
	mov	rax, QWORD PTR -32[rbp]
	mov	edx, DWORD PTR [rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	DWORD PTR [rax], edx
	mov	rax, QWORD PTR -32[rbp]
	mov	edx, DWORD PTR -4[rbp]
	mov	DWORD PTR [rax], edx
	nop
	pop	rbp
	ret
	.size	swap, .-swap
	.section	.rodata
.LC0:
	.string	"\n"
.LC1:
	.string	" b: "
.LC2:
	.string	"before swap: a:"
.LC3:
	.string	"%s%d%s%d%s"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], 1
	mov	DWORD PTR -8[rbp], 2
	lea  rdi, -4[rbp] ;// assign [a] is_ref
	lea  rsi, -8[rbp] ;// assign [b] is_ref
	call	swap
	mov	edx, DWORD PTR -8[rbp]
	mov	eax, DWORD PTR -4[rbp]
	lea	r9, .LC0[rip]
	mov	r8d, edx
	lea	rdx, .LC1[rip]
	mov	rcx, rdx
	mov	edx, eax
	lea	rax, .LC2[rip]
	mov	rsi, rax
	lea	rax, .LC3[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
