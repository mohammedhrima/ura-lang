	.file	"7.test.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	mov	rdx, rdi
	mov	eax, esi
	mov	QWORD PTR -16[rbp], rdx
	mov	DWORD PTR -8[rbp], eax
	nop
	pop	rbp
	ret
	.size	func, .-func
	.globl	func1
	.type	func1, @function
func1:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	nop
	pop	rbp
	ret
	.size	func1, .-func1
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR -12[rbp], 10
	mov	DWORD PTR -8[rbp], 11
	mov	DWORD PTR -4[rbp], 12
	mov	rdx, QWORD PTR -12[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	rdi, rdx
	mov	esi, eax
	call	func
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
