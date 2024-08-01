.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main

main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 24
    /*declare b*/
    mov     QWORD PTR -16[rbp], 0
    /*assign b*/
    mov     QWORD PTR -16[rbp], 5
    lea     rbx, -16[rbp]
    mov     QWORD PTR [rbx], 7
    mov     rax, QWORD PTR -16[rbp]
    leave
    ret
.section	.note.GNU-stack,"",@progbits

