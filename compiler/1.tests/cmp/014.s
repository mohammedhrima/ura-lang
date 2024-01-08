.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 5 /* assign x */
   jmp     sayHi_sayBy_1
sayHi_sayBy_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret

sayHi_sayBy_1:
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   call    sayHi_sayBy_
   call    sayHi_sayBy_
   call    sayHi_sayBy_
   call    sayHi_sayBy_
   leave
   ret

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   call    sayHi_
   leave
   ret

STR1: .string "by\n"
STR2: .string "x: "
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
