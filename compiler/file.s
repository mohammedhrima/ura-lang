.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30

   /* x = 10 */
   mov     rax, 10
   mov     QWORD PTR -4[rbp], rax
   
   /* y = 5 + x + 2 + 1 */
   mov     rax, 5
   add     rax, QWORD PTR -4[rbp]
   add     rax, 2
   add     rax, 1
   mov     QWORD PTR -8[rbp], rax

   /* x = x + 5 */
   mov     rax, QWORD PTR -4[rbp]
   add     rax, 5
   mov     QWORD PTR -4[rbp], rax
   leave
   ret

