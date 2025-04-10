	.file	"011.c"
	.intel_syntax noprefix
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR [rax], 1
	mov	rax, QWORD PTR -40[rbp]
	mov	BYTE PTR 4[rax], 2
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 8[rax], 3
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 12[rax], 4
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 16[rax], 5
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 20[rax], 7
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 24[rax], 8
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 28[rax], 9
	nop
	pop	rbp
	ret
	.size	func, .-func
	.globl	func2
	.type	func2, @function
func2:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR -8[rbp], rdi
	mov	QWORD PTR -16[rbp], rsi
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], 3
	nop
	pop	rbp
	ret
	.size	func2, .-func2
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	mov	DWORD PTR -32[rbp], 1
	mov	BYTE PTR -28[rbp], 2
	mov	DWORD PTR -24[rbp], 3
	mov	DWORD PTR -20[rbp], 4
	mov	DWORD PTR -16[rbp], 5
	mov	DWORD PTR -12[rbp], 6
	mov	DWORD PTR -8[rbp], 7
	mov	DWORD PTR -4[rbp], 8
	lea	rax, -32[rbp]
	mov	rdi, rax
	call	func
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
