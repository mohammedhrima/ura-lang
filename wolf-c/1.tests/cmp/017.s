.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare arr */
   mov     QWORD PTR -16[rbp], 66
   mov     QWORD PTR -24[rbp], 55
   mov     QWORD PTR -32[rbp], 14
   mov     QWORD PTR -40[rbp], 33
   mov     QWORD PTR -48[rbp], 22
   mov     QWORD PTR -56[rbp], 11
   lea     rax, -32[rbp]
   mov     QWORD PTR -64[rbp], rax
   lea     rax, -56[rbp]
   mov     QWORD PTR -72[rbp], rax
   lea     rax, QWORD PTR -72[rbp] /* assign to arr */
   mov     QWORD PTR -8[rbp], rax
   /* arr[] (array) */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 8
   mov     rax, [rax]
   mov     QWORD PTR -80[rbp], rax
   /* (null)[] (int) */
   mov     rax, QWORD PTR -80[rbp]
   add     rax, 0
   mov     rax, [rax]
   mov     QWORD PTR -88[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -88[rbp]
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
