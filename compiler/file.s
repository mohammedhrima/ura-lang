.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare x */
   mov     QWORD PTR -4[rbp], 0
   /* assign to x */
   movss   xmm1, DWORD PTR FLT2[rip]
   movss   DWORD PTR -4[rbp], xmm1
   /* declare y */
   mov     QWORD PTR -8[rbp], 0
   /* assign to y */
   movss   xmm1, DWORD PTR FLT3[rip]
   movss   DWORD PTR -8[rbp], xmm1
   /* declare z */
   mov     QWORD PTR -12[rbp], 0
   movss   xmm1, DWORD PTR -8[rbp]
   addss   xmm1, DWORD PTR FLT4[rip]
   movss   DWORD PTR -16[rbp], xmm1
   movss   xmm1, DWORD PTR -4[rbp]
   addss   xmm1, DWORD PTR -16[rbp]
   movss   DWORD PTR -20[rbp], xmm1
   /* assign to z */
   movss   xmm1, DWORD PTR -20[rbp]
   movss   DWORD PTR -12[rbp], xmm1

   leave
   ret

FLT2:/* 2.000000 */
   .long  1073741824
FLT3:/* -1.000000 */
   .long  3212836864
FLT4:/* 5.000000 */
   .long  1084227584
