.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
print:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    mov     DWORD PTR -4[rbp], edi
    lea     rax, .STR1[rip]
    mov     rdi, rax
    call    putstr
    mov     eax, 0
    leave
    ret
.endprint:
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    mov     edi, 11
    call    print
    mov     eax, 0
    leave
    ret
.endmain:
.STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits

