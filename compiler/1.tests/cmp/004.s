.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare x */
   mov     QWORD PTR -8[rbp], 0
   /* assign to x */
   mov     QWORD PTR -8[rbp], 17
   /* declare y */
   mov     QWORD PTR -16[rbp], 0
   /* assign to y */
   mov     QWORD PTR -16[rbp], 5
   /* declare z */
   mov     QWORD PTR -24[rbp], 0
   sub     rsp, 30
   mov     rax, QWORD PTR -8[rbp]
   imul    rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -32[rbp], rax
   /* assign to z */
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -24[rbp], rax

   leave
   ret

.section	.note.GNU-stack,"",@progbits
