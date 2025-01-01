.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
hello:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	//      declare [str]
	mov     rdi, 0
	//      assign [str]
	lea     rdi, .STR1[rip]
	mov     rdi, rax
	mov     eax, 1
	leave   
	ret     
.endhello:
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 0
	//      declare [a]
	mov     QWORD PTR -8[rbp], 0
	//      assign [a]
	lea     rax, .STR2[rip]
	mov     QWORD PTR -8[rbp], rax
	mov     eax, 0
	leave   
	ret     
.endmain:
.STR2: .string "ee"
.STR1: .string ""
.section	.note.GNU-stack,"",@progbits

