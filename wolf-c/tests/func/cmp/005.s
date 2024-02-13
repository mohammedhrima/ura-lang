.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
test_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare s */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR -32[rbp], 0 /* declare y */
   mov	  rax, QWORD PTR 32[rbp]
   mov     QWORD PTR -32[rbp], rax
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
   /* call _putstr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -32[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_test_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, 2
   push    rax
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _strdup
   push    rax
   mov     rax, 1
   push    rax
   mov     QWORD PTR -8[rbp], 0 /* test result */
   lea     rax, -8[rbp]
   call    test_
   leave
   ret
end_main:

STR1: .string "x: "
STR2: .string " s: "
STR3: .string " y: "
STR4: .string "\n"
STR5: .string "something"
.section	.note.GNU-stack,"",@progbits
