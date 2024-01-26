.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
test4_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare z (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 4 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref z */
   leave
   ret
end_test4_:

test3_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare z (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 3 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref z */
   push    QWORD PTR -16[rbp] /*z*/
   mov     QWORD PTR -24[rbp], 0 /*test4 result*/
   lea     rax, -24[rbp]
   call    test4_
   leave
   ret
end_test3_:

test2_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 2 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref y */
   push    QWORD PTR -16[rbp] /*y*/
   mov     QWORD PTR -24[rbp], 0 /*test3 result*/
   lea     rax, -24[rbp]
   call    test3_
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
   mov     QWORD PTR -16[rbp], 0 /*test2 result*/
   lea     rax, -16[rbp]
   call    test2_
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
   leave
   ret
end_main:

STR1: .string "in main: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
