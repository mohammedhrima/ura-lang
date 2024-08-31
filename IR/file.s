.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [a]
	mov     DWORD PTR -4[rbp], 15
	//      assign [b]
	mov     DWORD PTR -8[rbp], 5
	//      declare [c]
	mov     DWORD PTR -12[rbp], 0
	mov     eax, DWORD PTR -4[rbp]
	add     eax, DWORD PTR -8[rbp]
	//      assign [c]
	mov     DWORD PTR -12[rbp], eax
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

