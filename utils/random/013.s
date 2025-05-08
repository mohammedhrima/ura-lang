	.file	"013.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], 1
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 4[rax], 2
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 8[rax], 3
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 12[rax], 4
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
	mov	DWORD PTR -12[rbp], 2
	mov	DWORD PTR -8[rbp], 3
	mov	DWORD PTR -4[rbp], 4
	lea	rax, -16[rbp]
	mov	rdi, rax
	call	func
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
