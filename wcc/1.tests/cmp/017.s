.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare y */
   mov     QWORD PTR -8[rbp], 5 /* assign y */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 7
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign y */
   leave
   ret
end_main:

.section	.note.GNU-stack,"",@progbits
