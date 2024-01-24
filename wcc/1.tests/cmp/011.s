.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 10 /* assign x */
   jmp     while1                    /* jmp to while condition*/
while2:                              /* while bloc*/
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
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
while1:                                 /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 20
   setle   al
   cmp     al, 1
   je      while2                       /* je to while bloc*/
while3: /* end while*/
   leave
   ret
end_main:

STR1: .string "x: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
