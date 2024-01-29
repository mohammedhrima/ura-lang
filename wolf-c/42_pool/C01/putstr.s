.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax /* assign  s */
   leave
   ret
end_main:

STR1: .string "abcd"
.section	.note.GNU-stack,"",@progbits
