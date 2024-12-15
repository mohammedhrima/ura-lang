.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
m:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	mov     eax, 10
	leave   
	ret     
.endm:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [n]
	mov     DWORD PTR -4[rbp], 10
	call    m
