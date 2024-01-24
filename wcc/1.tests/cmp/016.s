.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare z */
   mov	  rax, QWORD PTR 32[rbp]
   mov     QWORD PTR -24[rbp], rax
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
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 300
   mov     QWORD PTR -32[rbp], 0 /* declare x */
   mov     QWORD PTR -32[rbp], 10 /* assign x */
   mov     QWORD PTR -40[rbp], 0 /* declare y */
   mov     QWORD PTR -40[rbp], 5 /* assign y */
   mov     QWORD PTR -48[rbp], 0 /* declare z */
   mov     QWORD PTR -48[rbp], 7 /* assign z */
   push    QWORD PTR -48[rbp]
   push    QWORD PTR -40[rbp]
   push    QWORD PTR -32[rbp]
   call    sayHi_
   leave
   ret
end_main:

STR1: .string "x: "
STR2: .string " y: "
STR3: .string " z: "
STR4: .string "\n"
.section	.note.GNU-stack,"",@progbits
