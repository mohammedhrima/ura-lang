.intel_syntax noprefix
.section	.note.GNU-stack,"",@progbits

.text
	.globl	main
.LC0:
	.string	"a"

main:
	push	rbp
	mov	rbp, rsp
	/* write(1,"a",1) */
	mov	rdi, 1
	lea	rsi, .LC0[rip]
	mov	rdx, 1
	call	write@PLT
	/* return 0 */
	mov	eax, 0
	pop	rbp
	ret
