	.file	"struct.c"
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
	mov	DWORD PTR -36[rbp], edi
	mov	DWORD PTR -40[rbp], esi
	mov	DWORD PTR -44[rbp], edx
	mov	eax, DWORD PTR -36[rbp]
	mov	DWORD PTR -12[rbp], eax
	mov	eax, DWORD PTR -40[rbp]
	mov	DWORD PTR -8[rbp], eax
	mov	eax, DWORD PTR -44[rbp]
	mov	DWORD PTR -4[rbp], eax
	mov	rax, QWORD PTR -12[rbp]
	mov	ecx, DWORD PTR -4[rbp]
	mov	rdx, rcx
	pop	rbp
	ret
	.size	func1, .-func1
	.section	.rodata
.LC0:
	.string	"v: (%d, %d, %d)\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	mov		edx, 3
	mov		esi, 2
	mov		edi, 1
	call	func1
	mov		QWORD PTR -12[rbp], rax
	mov		eax, DWORD PTR -4[rbp]
	and		eax, 0
	or		eax, edx
	mov		DWORD PTR -4[rbp], eax
	mov		ecx, DWORD PTR -4[rbp]
	mov		edx, DWORD PTR -8[rbp]
	mov		eax, DWORD PTR -12[rbp]
	mov		esi, eax
	lea		rax, .LC0[rip]
	mov		rdi, rax
	mov		eax, 0
	call	printf@PLT
	mov		eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
