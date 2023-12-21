.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   movss   xmm1, DWORD PTR .FLT5[rip]
   movss   DWORD PTR -4[rbp], xmm1
   mov     rax, 0
   leave
   ret

FLT5:
   .long  1095761920

