.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
s_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     rax, 5
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax
   leave
   ret
end_s_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 0 /* s result */
   lea     rax, -16[rbp]
   call    s_
   mov     QWORD PTR -24[rbp], 0 /* s result */
   lea     rax, -24[rbp]
   call    s_
   mov     rax, QWORD PTR -16[rbp]
   mov     rbx, QWORD PTR -24[rbp]
   add     rax, rbx
   mov     QWORD PTR -32[rbp], rax
   mov     QWORD PTR -40[rbp], 0 /* s result */
   lea     rax, -40[rbp]
   call    s_
   mov     rax, QWORD PTR -32[rbp]
   mov     rbx, QWORD PTR -40[rbp]
   add     rax, rbx
   mov     QWORD PTR -48[rbp], rax
   mov     rax, QWORD PTR -48[rbp]
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
