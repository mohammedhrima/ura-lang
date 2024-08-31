.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [a]
	mov     BYTE PTR -1[rbp], 0
	//      assign [b]
	mov     al, BYTE PTR -1[rbp]
	mov     BYTE PTR -2[rbp], al
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

