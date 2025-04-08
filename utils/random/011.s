	.file	"011.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	mov	rax, rdi
	mov	rcx, rsi
	mov	rdx, rcx
	mov	QWORD PTR -16[rbp], rax
	mov	QWORD PTR -8[rbp], rdx
	mov	DWORD PTR -16[rbp], 1
	mov	BYTE PTR -12[rbp], 2
	mov	DWORD PTR -8[rbp], 3
	mov	DWORD PTR -4[rbp], 4
	nop
	pop	rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -16[rbp], 1
	mov	BYTE PTR -12[rbp], 2
	mov	DWORD PTR -8[rbp], 3
	mov	DWORD PTR -4[rbp], 4
	mov	rdx, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rdx
	mov	rsi, rax
	call	func
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
