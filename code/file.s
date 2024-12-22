.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
factorial:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	mov     eax, 1
	leave   
	ret     
.endfactorial:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [num]
	mov     DWORD PTR -4[rbp], 5
	call    output
	call    factorial
	//      assign [result]
