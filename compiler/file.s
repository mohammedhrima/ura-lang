.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare stname */

   /* assign to stname */
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax
   /* declare ndname */

   /* assign to ndname */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax
   /* declare age */
   mov     QWORD PTR -24[rbp], 0
   sub     rsp, 30
   /* assign to age */
   mov     QWORD PTR -24[rbp], 25
   /* call output */
   lea   rbx, STR3[rip]
   call  putstr
   /* call output */
   mov     rbx, QWORD PTR -8[rbp]
   call    putstr
   /* call output */
   lea   rbx, STR4[rip]
   call  putstr
   /* call output */
   mov     rbx, QWORD PTR -16[rbp]
   call    putstr
   /* call output */
   lea   rbx, STR5[rip]
   call  putstr
   /* call output */
   mov   rax, QWORD PTR -24[rbp]
   mov   rdi, rax
   call  putnbr
   /* call output */
   lea   rbx, STR6[rip]
   call  putstr

   leave
   ret

STR1: .string "mohammed"
STR2: .string "mohammed"
STR3: .string "stname: "
STR4: .string " ndname: "
STR5: .string " age: "
STR6: .string "\n"
