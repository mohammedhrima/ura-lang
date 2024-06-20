.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
chihaja_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     rax, 5
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax
   leave
   ret
end_chihaja_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
