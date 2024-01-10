.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 4 /* assign x */
   jmp     main0               /* jmp to while loop condition*/
main1:                         /* while loop bloc*/
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, 2
   idiv    rbx
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, 0
   mov     QWORD PTR -24[rbp], rax
   sub     rsp, 30
   mov     rax, QWORD PTR -24[rbp]
   cmp     rax, 0
   sete    al
main3:                          /* if statement */
   cmp     al, 1
   jne     main5                /* jmp next statement */
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     main4                /* jmp end statement */
main5:                          /* else statement */
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
main4:                          /* end statement */
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, 1
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
main0:                            /* while loop condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 0
   setg    al
   cmp     al, 1
   je      main1                  /* je to while loop bloc*/
   leave
   ret
end_main:

STR1: .string " is even\n"
STR2: .string " is odd\n"
.section	.note.GNU-stack,"",@progbits
