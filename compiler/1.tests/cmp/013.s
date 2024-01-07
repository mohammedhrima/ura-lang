.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main


sayHi:
   mov     QWORD PTR -8[rbp], 0 /* declare y */
   mov     QWORD PTR -8[rbp], 7 /* assign  y */

   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 7
   sete    al
sayHi2:                          /* if statement */
   cmp     al, 1
   jne     sayHi3                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     sayHi1                /* jmp end statement */
sayHi3:                          /* else statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
sayHi1:                          /* end statement */
   leave
   ret

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 10 /* assign  x */

   call sayHi
   leave
   ret

STR1: .string "is seven\n"
STR2: .string "is not seven\n"
.section	.note.GNU-stack,"",@progbits
