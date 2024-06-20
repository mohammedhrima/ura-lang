.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
test2_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov     rax, 5 
   mov     QWORD PTR -16[rbp], rax /* assign x */
   mov	  rax, QWORD PTR -16[rbp]
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax
   leave
   ret
end_test2_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 0 /* test2 result */
   lea     rax, -16[rbp]
   call    test2_
   mov     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign x */
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

STR1: .string "x: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
