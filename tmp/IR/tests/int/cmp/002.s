.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [a]
	mov     DWORD PTR -4[rbp], 12
	mov     eax, DWORD PTR -4[rbp]
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

