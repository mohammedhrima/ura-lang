	.file	"multi.c"
	.intel_syntax noprefix
	.text
	.globl	processIntegers
	.type	processIntegers, @function
processIntegers:
	push	rbp
	mov	rbp, rsp
	mov	eax, DWORD PTR 16[rbp] /* 7 */
	mov	eax, DWORD PTR 24[rbp] /* 8 */

	pop	rbp
	ret
	.size	processIntegers, .-processIntegers
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	push	14
	push	13
	push	12
	push	11
	push	10
	push	9
	push	8
	push	7
	mov	r9d, 6
	mov	r8d, 5
	mov	ecx, 4
	mov	edx, 3
	mov	esi, 2
	mov	edi, 1
	call	processIntegers
	add	rsp, 64
	leave
	ret
.size	main, .-main
.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
.section	.note.GNU-stack,"",@progbits
