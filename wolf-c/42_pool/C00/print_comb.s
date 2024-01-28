.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
printComb_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   mov     rax, 0 
   mov     QWORD PTR -8[rbp], rax /* assign a */
   jmp     while1 /* jmp to while condition*/
while2: /* while bloc*/
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -24[rbp], rax
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign b */
   jmp     while4 /* jmp to while condition*/
while5: /* while bloc*/
   mov     QWORD PTR -32[rbp], 0 /* declare c */
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 1
   mov     QWORD PTR -40[rbp], rax
   mov     rax, QWORD PTR -40[rbp]
   mov     QWORD PTR -32[rbp], rax /* assign c */
   jmp     while7 /* jmp to while condition*/
while8: /* while bloc*/
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putnbr */
   mov     rax, QWORD PTR -32[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -32[rbp]
   add     rax, 1
   mov     QWORD PTR -48[rbp], rax
   mov     rax, QWORD PTR -48[rbp]
   mov     QWORD PTR -32[rbp], rax /* assign c */
while7: /* while condition */
   mov     rax, QWORD PTR -32[rbp]
   cmp     rax, 10
   setl    al
   cmp     al, 1
   je      while8 /* je to while bloc*/
while9: /* end while */
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 1
   mov     QWORD PTR -56[rbp], rax
   mov     rax, QWORD PTR -56[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign b */
while4: /* while condition */
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 10
   setl    al
   cmp     al, 1
   je      while5 /* je to while bloc*/
while6: /* end while */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -64[rbp], rax
   mov     rax, QWORD PTR -64[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign a */
while1: /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   setl    al
   cmp     al, 1
   je      while2 /* je to while bloc*/
while3: /* end while */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_printComb_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   call    printComb_
   leave
   ret
end_main:

STR1: .string " "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
