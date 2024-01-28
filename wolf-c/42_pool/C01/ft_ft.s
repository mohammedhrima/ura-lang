.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
ftft_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare nbr (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     rax, 42 
   mov     rbx,  QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref nbr */
   leave
   ret
end_ftft_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare n */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign n */
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
   lea     rax, -8[rbp]
   push    rax /*ref n*/
   call    ftft_
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
   leave
   ret
end_main:

STR1: .string "n: "
STR2: .string "\n"
STR3: .string "n: "
STR4: .string "\n"
.section	.note.GNU-stack,"",@progbits
