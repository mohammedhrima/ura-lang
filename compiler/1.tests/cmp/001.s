.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax /* assign  stname */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax /* assign  ndname */
   mov     QWORD PTR -24[rbp], 0 /* declare age */
   sub     rsp, 30
   mov     QWORD PTR -24[rbp], 25 /* assign  age */

   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov   rax, QWORD PTR -24[rbp]
   mov   rdi, rax
   call  _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "mohammed"
STR2: .string "mohammed"
STR3: .string "stname: "
STR4: .string " ndname: "
STR5: .string " age: "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
