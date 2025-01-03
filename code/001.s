	.file	"001.c"
	.intel_syntax noprefix
	.text
	.globl	hello
	.type	hello, @function
hello:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	mov	eax, 1
	pop	rbp
	ret
	.size	hello, .-hello
	.section	.rodata
.LC0:
	.string	"x"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -8[rbp], 5
	mov	DWORD PTR -12[rbp], 10
	mov	DWORD PTR -4[rbp], 0
	jmp	.L4
.L5:
	mov	edx, 1
	lea	rax, .LC0[rip]
	mov	rsi, rax
	mov	edi, 1
	call	write@PLT
	add	DWORD PTR -4[rbp], 1
.L4:
	cmp	DWORD PTR -4[rbp], 9
	jle	.L5
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
