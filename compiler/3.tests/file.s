.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* assign x */
   mov     QWORD PTR -4[rbp], 10
   /* assign x */
   mov     QWORD PTR -4[rbp], 10
   /* assign y */
   mov     QWORD PTR -12[rbp], QWORD PTR -4[rbp]
   add     QWORD PTR -12[rbp], 3
   mov     QWORD PTR -16[rbp], 5
   add     QWORD PTR -16[rbp], QWORD PTR -12[rbp]
   mov     QWORD PTR -8[rbp], QWORD PTR -16[rbp]
   /* assign x */
   mov     QWORD PTR -20[rbp], QWORD PTR -4[rbp]
   add     QWORD PTR -20[rbp], 5
   mov     QWORD PTR -4[rbp], QWORD PTR -20[rbp]
   mov     rax, 0
   leave
   ret

