.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*assign b*/
    mov     QWORD PTR -8[rbp], 5
    mov     rax, 0
    leave
    ret
.section	.note.GNU-stack,"",@progbits

