.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   jmp     while3                    /* jmp to while condition*/
while4:                              /* while bloc*/
   mov     QWORD PTR -16[rbp], 0 /* assign y */
   jmp     mainwhile6                    /* jmp to while condition*/
mainwhile7:                              /* while bloc*/
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
   sub     rsp, 60
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign y */
mainwhile6:                                 /* while condition */
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 5
   setl    al
   cmp     al, 1
   je      mainwhile7                       /* je to while bloc*/
mainwhile5: /* end while*/
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
while3:                                 /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 5
   setl    al
   cmp     al, 1
   je      while4                       /* je to while bloc*/
while2: /* end while*/
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "y: "
STR2: .string " "
STR3: .string "\n"
STR4: .string "x: "
STR5: .string " "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
