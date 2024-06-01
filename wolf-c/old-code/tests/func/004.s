.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare y */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR -32[rbp], 0 /* declare z */
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
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
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
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, 5 
   mov     QWORD PTR -16[rbp], rax /* assign y */
   mov     QWORD PTR -24[rbp], 0 /* declare z */
   mov     rax, 7 
   mov     QWORD PTR -24[rbp], rax /* assign z */
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -32[rbp], rax /* assign z */
   mov     rax, QWORD PTR -32[rbp] /*z*/
   push    rax
   mov     rax, QWORD PTR -16[rbp]
   mov     QWORD PTR -24[rbp], rax /* assign y */
   mov     rax, QWORD PTR -24[rbp] /*y*/
   push    rax
   mov     rax, QWORD PTR -8[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign x */
   mov     rax, QWORD PTR -16[rbp] /*x*/
   push    rax
   mov     QWORD PTR -32[rbp], 0 /* sayHi result */
   lea     rax, -32[rbp]
   call    sayHi_
   leave
   ret
end_main:

STR1: .string "x: "
STR2: .string " y: "
STR3: .string " z: "
STR4: .string "\n"
.section	.note.GNU-stack,"",@progbits
