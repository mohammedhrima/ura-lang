.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare stname */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -8[rbp], rax /* assign  stname */
   mov     QWORD PTR -16[rbp], 0 /* declare ndname */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -16[rbp], rax /* assign  ndname */
   mov     QWORD PTR -24[rbp], 0 /* declare age */
   mov     rax, 25 
   mov     QWORD PTR -24[rbp], rax /* assign age */
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -24[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "mohammed"
STR2: .string "mohammed"
STR3: .string "stname: "
STR4: .string " ndname: "
STR5: .string " age: "
STR6: .string "\n"
.section	.note.GNU-stack,"",@progbits
