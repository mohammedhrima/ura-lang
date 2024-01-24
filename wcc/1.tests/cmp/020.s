.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
test_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -8[rbp], 2 /* assign x */
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
end_test_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 300
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 1 /* assign x */
   push    QWORD PTR -16[rbp]
   call    test_
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   mov     QWORD PTR -24[rbp], 0 /* declare y */
   mov     QWORD PTR -16[rbp], 7 /* assign y */
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "x in test: "
STR2: .string "\n"
STR3: .string "x in main: "
STR4: .string "\n"
STR5: .string "x in main (after ref): "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
