.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*assign a*/
    mov     QWORD PTR -8[rbp], 12
    mov     rax, 0
    leave
    ret
.section	.note.GNU-stack,"",@progbits

