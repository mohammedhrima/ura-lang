.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
hello:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	mov     edi, 0
	mov     esi, 0
	mov     eax, 12
	leave   
	ret     
.endhello:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	mov     edi, 1
	call    hello
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

