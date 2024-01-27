.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     DWORD PTR -4[rbp], 0 /* declare x */
   movss   xmm1, DWORD PTR FLT1[rip]
   movss   DWORD PTR -4[rbp], xmm1 /* assign x */
   mov     QWORD PTR -12[rbp], 0 /* declare y (ref) */
   lea     rax, -4[rbp]
   mov     QWORD PTR -12[rbp], rax
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putfloat */
   mov     rax, QWORD PTR -12[rbp]
   mov     eax, QWORD PTR [rax]
   movd    xmm0, eax
   call    _putfloat
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

FLT1: .long 1092878336 /* 10.250000 */
STR2: .string "x: "
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
