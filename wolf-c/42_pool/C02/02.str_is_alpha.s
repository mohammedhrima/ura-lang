.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
print_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare n (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rax, QWORD PTR [rax]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_print_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, 11 
   mov     rbx,  QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax /* assign ref n */
   mov     rax, QWORD PTR -8[rbp] /*n*/
   mov     rax, [rax]
   push    rax
   call    print_
   leave
   ret
end_main:

STR1: .string "n: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
