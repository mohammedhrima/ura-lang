.intel_syntax noprefix
.text
	.globl	main

.macro  add_3  n
    mov eax, \n + 3
.endm

main:
	push	rbp
	mov	rbp, rsp
    add_3 1
	pop	rbp
	ret
.section	.note.GNU-stack,"",@progbits
