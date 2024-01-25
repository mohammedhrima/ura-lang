.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   jmp     while1                    /* jmp to while condition*/
while2:                              /* while bloc*/
   mov     rax, 0 
   mov     QWORD PTR -16[rbp], rax /* assign y */
   jmp     while4                    /* jmp to while condition*/
while5:                              /* while bloc*/
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 1
   mov     QWORD PTR -24[rbp], rax
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign y */
while4:                                 /* while condition */
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 5
   setl    al
   cmp     al, 1
   je      while5                       /* je to while bloc*/
while6: /* end while*/
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
while1:                                 /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 5
   setl    al
   cmp     al, 1
   je      while2                       /* je to while bloc*/
while3: /* end while*/
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "y: "
STR2: .string ", "
STR3: .string "\nx: "
STR4: .string "\n"
STR5: .string "\n"
.section	.note.GNU-stack,"",@progbits
