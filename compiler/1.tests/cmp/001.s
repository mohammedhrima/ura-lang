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
   lea   rax, STR3[rip]
   mov     rdi, rax
   call  _putstr
   /* call output */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putstr
   /* call output */
   lea   rax, STR4[rip]
   mov     rdi, rax
   call  _putstr
   /* call output */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putstr
   /* call output */
   lea   rax, STR5[rip]
   mov     rdi, rax
   call  _putstr
   /* call output */
   mov   rax, QWORD PTR -24[rbp]
   mov   rdi, rax
   call  _putnbr
   /* call output */
   lea   rax, STR6[rip]
   mov     rdi, rax
   call  _putstr

   leave
   ret

STR1: .string "mohammed"
STR2: .string "mohammed"
STR3: .string "stname: "
STR4: .string " ndname: "
STR5: .string " age: "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
