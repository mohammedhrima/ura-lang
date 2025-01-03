.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	//      assign [a]
	mov     DWORD PTR -4[rbp], 6
	//      assign [b]
	mov     DWORD PTR -8[rbp], 4
	mov     eax, 4
	add     eax, DWORD PTR -4[rbp]
	//      assign [c]
	mov     DWORD PTR -12[rbp], eax
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

