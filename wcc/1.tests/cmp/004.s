.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   sub     rsp, 200
   mov     QWORD PTR -8[rbp], 17 /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     QWORD PTR -16[rbp], 5 /* assign y */
   mov     QWORD PTR -24[rbp], 0 /* declare z */
   mov     rax, QWORD PTR -8[rbp]
   imul    rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -24[rbp], rax /* assign z */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "z: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
