.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [b]
	mov     DWORD PTR -4[rbp], 5
	mov     eax, DWORD PTR -4[rbp]
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

