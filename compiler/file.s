.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   mov     rax, 0
   pop     rbp
   ret

.LC2:
   .string "abc"

