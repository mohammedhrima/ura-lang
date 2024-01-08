.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   mov     QWORD PTR -8[rbp], 10 /* assign a */
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov     QWORD PTR -16[rbp], 25 /* assign b */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   add     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -24[rbp], rax
   sub     rsp, 30
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret

STR1: .string "a is "
STR2: .string " b is "
STR3: .string "\na + b = "
STR4: .string "\n"
.section	.note.GNU-stack,"",@progbits
