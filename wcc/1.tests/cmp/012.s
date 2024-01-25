.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 4 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   jmp     while1                    /* jmp to while condition*/
while2:                              /* while bloc*/
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, 2
   idiv    rbx
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 2
   imul    rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -24[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   cmp     rax, 0
   sete    al
if5:                          /* if statement */
   cmp     al, 1
   jne     if6                /* jmp next statement */
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if4                /* jmp end statement */
if6:                          /* else statement */
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
if4:                          /* end statement */
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, 1
   mov     QWORD PTR -40[rbp], rax
   mov     rax, QWORD PTR -40[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
while1:                                 /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 0
   setg    al
   cmp     al, 1
   je      while2                       /* je to while bloc*/
while3: /* end while*/
   leave
   ret
end_main:

STR1: .string " is even\n"
STR2: .string " is odd\n"
.section	.note.GNU-stack,"",@progbits
