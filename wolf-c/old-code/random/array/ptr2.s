.intel_syntax noprefix
.include "../../import/header.s"

.text
   .globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 16

   /* array 1*/
   mov     QWORD PTR -8[rbp],  11
   mov     QWORD PTR -16[rbp], 12
   mov     QWORD PTR -24[rbp], 13

   /* array 2*/
   mov     QWORD PTR -32[rbp], 14
   mov     QWORD PTR -40[rbp], 15
   mov     QWORD PTR -48[rbp], 16

   mov     QWORD PTR -56[rbp], 0 /* ptr1 */
   mov     QWORD PTR -64[rbp], 0 /* ptr2 */


   lea     rax, -8[rbp]             /* load address in stack*/
   mov     QWORD PTR -56[rbp], rax  /* to ptr1 */
   lea     rax, -32[rbp]            /* load address in stack*/
   mov     QWORD PTR -64[rbp], rax  /* to ptr2 */

   lea     rax, -56[rbp]
   mov     QWORD PTR -72[rbp], rax  /* ptr3 */

   mov     rax, QWORD PTR -72[rbp]
   sub     rax, 0
   mov     rax, [rax]
   sub     rax, 8
   mov     rax, [rax]

   mov     rdi, rax
   call    _putnbr

   lea     rax, STR0[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, 0
   leave
   ret

STR0: .string "\n"
.section	.note.GNU-stack,"",@progbits
