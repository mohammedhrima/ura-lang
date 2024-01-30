	.file	"func.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"abcd"
	.text
	.globl	func
	.type	func, @function
func:
	push	rbp
	mov	rbp, rsp
	lea	rax, .LC0[rip]
	mov	rdi, rax
	call	puts@PLT
	nop
	pop	rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	eax, 0
	call	func
	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
