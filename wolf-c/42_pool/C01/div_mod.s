.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
divmod_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare div (ref) */
   mov	  rax, QWORD PTR 32[rbp]
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR -32[rbp], 0 /* declare mod (ref) */
   mov	  rax, QWORD PTR 40[rbp]
   mov     QWORD PTR -32[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, QWORD PTR -16[rbp]
   idiv    rbx
   mov     QWORD PTR -40[rbp], rax
   mov     rax, QWORD PTR -40[rbp]
   mov     rbx,  QWORD PTR -24[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref div */
   mov     rax, QWORD PTR -8[rbp]
   cdq
   mov     rbx, QWORD PTR -16[rbp]
   idiv    rbx
   mov     QWORD PTR -48[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   imul    rax, QWORD PTR -48[rbp]
   mov     QWORD PTR -56[rbp], rax
   mov     rax, QWORD PTR -8[rbp]
   sub     rax, QWORD PTR -56[rbp]
   mov     QWORD PTR -64[rbp], rax
   mov     rax, QWORD PTR -64[rbp]
   mov     rbx,  QWORD PTR -32[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref mod */
   leave
   ret
end_divmod_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 20 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, 3 
   mov     QWORD PTR -16[rbp], rax /* assign y */
   mov     QWORD PTR -24[rbp], 0 /* declare d */
   mov     QWORD PTR -32[rbp], 0 /* declare m */
   lea     rax, -32[rbp]
   push    rax /*ref m*/
   lea     rax, -24[rbp]
   push    rax /*ref d*/
   push    QWORD PTR -16[rbp] /*y*/
   push    QWORD PTR -8[rbp] /*x*/
   call    divmod_
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -32[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string " / "
STR2: .string " = "
STR3: .string "\n"
STR4: .string " % "
STR5: .string " = "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
