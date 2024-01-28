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
   jmp     end_sayHi_sayBy_
sayHi_sayBy_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -24[rbp], rax
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_sayHi_sayBy_:

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
   mov     QWORD PTR -32[rbp], 0 /*sayBy result*/
   lea     rax, -32[rbp]
   call    sayHi_sayBy_
   mov     QWORD PTR -40[rbp], 0 /*sayBy result*/
   lea     rax, -40[rbp]
   call    sayHi_sayBy_
   mov     QWORD PTR -48[rbp], 0 /*sayBy result*/
   lea     rax, -48[rbp]
   call    sayHi_sayBy_
   mov     QWORD PTR -56[rbp], 0 /*sayBy result*/
   lea     rax, -56[rbp]
   call    sayHi_sayBy_
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

STR1: .string "by\n"
STR2: .string "x: "
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
