.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 5 /* assign x */
   jmp end_sayBy1_
sayBy1_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp end_something_
something_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_something_:

   call    something_
   leave
   ret
end_sayBy1_:

   jmp end_sayBy2_
sayBy2_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_sayBy2_:

   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   call    sayBy1_
   call    sayBy2_
   call    sayBy1_
   call    sayBy2_
   leave
   ret
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
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
