.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
swap_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare b (ref) */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare t */
   mov     rax, QWORD PTR -8[rbp]
   mov     rax, QWORD PTR [rax] /* assign from ref a */
   mov     QWORD PTR -24[rbp], rax /* assign t */
   mov     rax, QWORD PTR -16[rbp]
   mov     rax, QWORD PTR [rax] /* assign from ref b */
   mov     rbx,  QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref a */
   mov     rax, QWORD PTR -24[rbp]
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref b */
   leave
   ret
end_swap_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, 5 
   mov     QWORD PTR -16[rbp], rax /* assign y */
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
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   lea     rax, -16[rbp]
   push    rax /*ref y*/
   lea     rax, -8[rbp]
   push    rax /*ref x*/
   call    swap_
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
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "x: "
STR2: .string " y: "
STR3: .string "\n"
STR4: .string "x: "
STR5: .string " y: "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
