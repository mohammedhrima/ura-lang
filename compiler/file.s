.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare a */

   /* assign to a */
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax
   /* declare b */

   /* assign to b */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax
   /* declare s */

   sub     rsp, 30
