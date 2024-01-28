.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov     rax, 5 
   mov     QWORD PTR -16[rbp], rax /* assign x */
   jmp     end_sayHi_sayBy1_
sayHi_sayBy1_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -24[rbp], rax
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     end_sayHi_sayHi_sayBy1_something_
sayHi_sayHi_sayBy1_something_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -16[rbp], rax
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_sayHi_sayHi_sayBy1_something_:

   mov     QWORD PTR -24[rbp], 0 /*something result*/
   lea     rax, -24[rbp]
   call    sayHi_sayHi_sayBy1_something_
   leave
   ret
end_sayHi_sayBy1_:

   jmp     end_sayHi_sayBy2_
sayHi_sayBy2_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -32[rbp], rax
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_sayHi_sayBy2_:

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
   mov     QWORD PTR -40[rbp], 0 /*sayBy1 result*/
   lea     rax, -40[rbp]
   call    sayHi_sayBy1_
   mov     QWORD PTR -48[rbp], 0 /*sayBy2 result*/
   lea     rax, -48[rbp]
   call    sayHi_sayBy2_
   mov     QWORD PTR -56[rbp], 0 /*sayBy1 result*/
   lea     rax, -56[rbp]
   call    sayHi_sayBy1_
   mov     QWORD PTR -64[rbp], 0 /*sayBy2 result*/
   lea     rax, -64[rbp]
   call    sayHi_sayBy2_
   leave
   ret
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /*sayHi result*/
   lea     rax, -8[rbp]
   call    sayHi_
   leave
   ret
end_main:

STR1: .string "by1\n"
STR2: .string "something\n"
STR3: .string "by2\n"
STR4: .string "x: "
STR5: .string "\n"
.section	.note.GNU-stack,"",@progbits
