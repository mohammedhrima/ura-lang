.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
hello:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	mov     eax, 12
	leave   
	ret     
.endhello:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [num]
	mov     DWORD PTR -4[rbp], 5
	mov     eax, 0
	leave   
	ret     
.endmain:
.section	.note.GNU-stack,"",@progbits

