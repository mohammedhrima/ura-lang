.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     DWORD PTR -4[rbp], 0 /* declare x */
   movss   xmm1, DWORD PTR FLT1[rip]
   movss   DWORD PTR -4[rbp], xmm1 /* assign x */
   leave
   ret
end_main:

FLT1: .long 1093140480 /* 10.500000 */
.section	.note.GNU-stack,"",@progbits
