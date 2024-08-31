.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [a]
	lea     rax, .STR1[rip]
	mov     QWORD PTR -8[rbp], rax
	//      assign [b]
	mov     rax, QWORD PTR -8[rbp]
	mov     QWORD PTR -16[rbp], rax
	mov     eax, 0
	leave   
	ret     
.endmain:
.STR1: .string "abcdef"
.section	.note.GNU-stack,"",@progbits

