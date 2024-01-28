.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
makeComb_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, 10
   idiv    rbx
   mov     QWORD PTR -24[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, 10
   idiv    rbx
   mov     QWORD PTR -32[rbp], rax
   mov     rax, 10
   imul    rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -40[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, QWORD PTR -40[rbp]
   mov     QWORD PTR -48[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -48[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -16[rbp]
   cdq
   mov     rbx, 10
   idiv    rbx
   mov     QWORD PTR -56[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -56[rbp]
   mov     rdi, rax
   call    _putnbr
   mov     rax, QWORD PTR -16[rbp]
   cdq
   mov     rbx, 10
   idiv    rbx
   mov     QWORD PTR -64[rbp], rax
   mov     rax, 10
   imul    rax, QWORD PTR -64[rbp]
   mov     QWORD PTR -72[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   sub     rax, QWORD PTR -72[rbp]
   mov     QWORD PTR -80[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -80[rbp]
   mov     rdi, rax
   call    _putnbr
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 98
   setne   al
   /* || operation 0 */
   cmp     al, 1
   je      if3
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 99
   setne   al
if3:
if2: /* if statement */
   cmp     al, 1
   jne     if1    /* jmp end statemnt */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
if1: /* end statement */
   leave
   ret
end_makeComb_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 0 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   jmp     while4 /* jmp to while condition*/
while5: /* while bloc*/
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -24[rbp], rax
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign y */
   jmp     while7 /* jmp to while condition*/
while8: /* while bloc*/
   push    QWORD PTR -16[rbp] /*y*/
   push    QWORD PTR -8[rbp] /*x*/
   call    makeComb_
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 1
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign y */
while7: /* while condition */
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 99
   setle   al
   cmp     al, 1
   je      while8 /* je to while bloc*/
while9: /* end while */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 1
   mov     QWORD PTR -40[rbp], rax
   mov     rax, QWORD PTR -40[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
while4: /* while condition */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 99
   setle   al
   cmp     al, 1
   je      while5 /* je to while bloc*/
while6: /* end while */
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string " "
STR2: .string ", "
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
