.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 16
	//      assign [a]
	movss   xmm0, DWORD PTR .FLT1[rip]
	movss   DWORD PTR -4[rbp], xmm0
	//      assign [b]
	movss   xmm0, DWORD PTR -4[rbp]
	movss   DWORD PTR -8[rbp], xmm0
	mov     eax, 0
	leave   
	ret     
.endmain:
.FLT1: .long 1067030938 /* 1.200000 */
.section	.note.GNU-stack,"",@progbits

