.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

FLT2:
   .long  1092616192
FLT3:
   .long  1082130432
LC3:
	.string	"%f, %f \n"

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30

   /* assign x */
   movss     xmm1, DWORD PTR FLT2[rip]
   movss     DWORD PTR -4[rbp], xmm1

   movss xmm1, DWORD PTR FLT3[rip]
   movss DWORD PTR -8[rbp], xmm1

   /* assign x */
   movss     xmm1, DWORD PTR -4[rbp]
   addss     xmm1, DWORD PTR -8[rbp]
   movss     DWORD PTR -12[rbp], xmm1

   pxor xmm1, xmm1
   pxor xmm0, xmm0

	cvtss2sd	xmm0, DWORD PTR -4[rbp]
	cvtss2sd	xmm1, DWORD PTR -8[rbp]

	lea	rax, LC3[rip]
	mov	rdi, rax
	mov	eax, 2
	call	printf@PLT
	
	mov	eax, 0
   leave
   ret


