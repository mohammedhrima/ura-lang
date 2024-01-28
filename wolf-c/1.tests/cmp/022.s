.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
test1_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 10 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref y */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rax, QWORD PTR [rax]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_test1_:

test2_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 20 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref y */
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rax, QWORD PTR [rax]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_test2_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 1 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   lea     rax, -8[rbp]
   push    rax /*ref x*/
   mov     QWORD PTR -16[rbp], 0 /*test1 result*/
   lea     rax, -16[rbp]
   call    test1_
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   lea     rax, -8[rbp]
   push    rax /*ref x*/
   mov     QWORD PTR -24[rbp], 0 /*test2 result*/
   lea     rax, -24[rbp]
   call    test2_
   /* call _putstr */
   lea     rax, STR7[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR8[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "in test1: "
STR2: .string "\n"
STR3: .string "in test2: "
STR4: .string "\n"
STR5: .string "in main: "
STR6: .string "\n"
STR7: .string "in main: "
STR8: .string "\n"
.section	.note.GNU-stack,"",@progbits
