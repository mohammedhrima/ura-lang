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
	movss   xmm0, DWORD PTR .FLT2[rip]
	movss   DWORD PTR -8[rbp], xmm0
	//      declare [c]
	mov     DWORD PTR -12[rbp], 0
	movss   xmm0, DWORD PTR -4[rbp]
	addss   xmm0, DWORD PTR -8[rbp]
	//      assign [c]
	movss   DWORD PTR -12[rbp], xmm0
	mov     eax, 0
	leave   
	ret     
.endmain:
.FLT1: .long 1067030938 /* 1.200000 */
.FLT2: .long 1075000115 /* 2.300000 */
.section	.note.GNU-stack,"",@progbits

