.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	lea     rax, .STR1[rip]
	mov     rdi, rax
	call    putstr
	mov     eax, 0
	leave   
	ret     
.endmain:
.STR1: .string "abcde"
.section	.note.GNU-stack,"",@progbits

