.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -16[rbp], 0 /* len of arr */
   mov     QWORD PTR -8[rbp], 0 /* declare arr */
   mov     rsi, 8
   mov     rdi, 3
   call    calloc@PLT
   mov     QWORD PTR -48[rbp], 3 /*len*/
   mov     QWORD PTR -40[rbp], rax
   mov     QWORD PTR 0[rax], 11
   mov     QWORD PTR 8[rax], 22
   mov     QWORD PTR 16[rax], 33
   mov     rsi, 8
   mov     rdi, 3
   call    calloc@PLT
   mov     QWORD PTR -64[rbp], 3 /*len*/
   mov     QWORD PTR -56[rbp], rax
   mov     QWORD PTR 0[rax], 14
   mov     QWORD PTR 8[rax], 55
   mov     QWORD PTR 16[rax], 66
   mov     rsi, 8
   mov     rdi, 2
   call    calloc@PLT
   mov     QWORD PTR -32[rbp], 2 /*len*/
   mov     QWORD PTR -24[rbp], rax
   mov     rbx, QWORD PTR -40[rbp]
   mov     QWORD PTR 0[rax], rbx
   mov     rbx, QWORD PTR -56[rbp]
   mov     QWORD PTR 8[rax], rbx
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign  arr */
   mov     rax, QWORD PTR -32[rbp] /*array len*/
   mov     QWORD PTR -16[rbp], rax /*array len*/
   /* from array arr[] (array) */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 8
   mov     QWORD PTR -72[rbp], rax
   /* from array (null)[] (int) */
   mov     rax, QWORD PTR -72[rbp]
   mov     rax, [rax]
   add     rax, 0
   mov     QWORD PTR -80[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -80[rbp]
   mov     rax, QWORD PTR [rax]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
