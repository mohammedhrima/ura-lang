	.file	"011.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	pxor	xmm0, xmm0
	movups	XMMWORD PTR 16[rbp], xmm0
	movups	XMMWORD PTR 32[rbp], xmm0
	mov	DWORD PTR 16[rbp], 1
	mov	BYTE PTR 20[rbp], 2
	mov	DWORD PTR 24[rbp], 3
	nop
	pop	rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	pxor	xmm0, xmm0
	movaps	XMMWORD PTR -32[rbp], xmm0
	movaps	XMMWORD PTR -16[rbp], xmm0
	mov	DWORD PTR -32[rbp], 1
	mov	BYTE PTR -28[rbp], 2
	mov	DWORD PTR -24[rbp], 3
	sub	rsp, 32
	mov	rcx, rsp
	mov	rax, QWORD PTR -32[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR [rcx], rax
	mov	QWORD PTR 8[rcx], rdx
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	mov	QWORD PTR 16[rcx], rax
	mov	QWORD PTR 24[rcx], rdx
	call	func
	add	rsp, 32
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
