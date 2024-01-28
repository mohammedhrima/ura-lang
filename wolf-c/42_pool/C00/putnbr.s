.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
putnbr_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare n */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 0
   setl    al
if2: /* if statement */
   cmp     al, 1
   jne     if1    /* jmp end statemnt */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   imul    rax, -1
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign n */
if1: /* end statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   setl    al
if4: /* if statement */
   cmp     al, 1
   jne     if5    /* jmp next statement */
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   jmp     if3    /* jmp end statement */
if5: /* else statement */
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
if3: /* end statement */
   leave
   ret
end_putnbr_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, 5
   push    rax
   call    putnbr_
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "-"
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
