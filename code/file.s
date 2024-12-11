.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [i]
	mov     DWORD PTR -4[rbp], 8
	//      assign [j]
	mov     DWORD PTR -8[rbp], 25
	mov     eax, 2
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

