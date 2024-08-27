.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    /*assign_node a*/
    mov     BYTE PTR -1[rbp], 99

    /*assign_node b*/
    mov     al, BYTE PTR -1[rbp]
    mov     BYTE PTR -2[rbp], al
    leave
    ret
.endmain:
.section	.note.GNU-stack,"",@progbits

