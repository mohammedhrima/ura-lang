.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*assign_node c*/
    mov     BYTE PTR -8[rbp], 101
    mov     rax, 0
    leave
    ret
.section	.note.GNU-stack,"",@progbits

