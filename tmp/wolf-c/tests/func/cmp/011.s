.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
chihaja_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y (ref) */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, 11 
   mov     rbx,  QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref x */
   mov     rax, 12 
   mov     rbx,  QWORD PTR -16[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref y */
   leave
   ret
end_chihaja_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   mov     rax, 1 
   mov     QWORD PTR -8[rbp], rax /* assign a */
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov     rax, 2 
   mov     QWORD PTR -16[rbp], rax /* assign b */
   lea     rax, -16[rbp]
   push    rax /*ref b*/
   lea     rax, -8[rbp]
   push    rax /*ref a*/
   call    chihaja_
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
   leave
   ret
end_main:

STR1: .string "a: "
STR2: .string " b: "
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
