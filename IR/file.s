.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    mov     eax, 0
    /*assign_node i*/
    mov     DWORD PTR -4[rbp], 0
.while1:
    mov     eax, DWORD PTR -4[rbp]
    mov     ebx, 10
    cmp     eax, ebx
    setl    al
    cmp     al, 1
    jne     .endwhile1
    mov     edi, DWORD PTR -4[rbp]
    call    putnbr
    mov     eax, DWORD PTR -4[rbp]
    add     eax, 1
    /*assign_node i*/
    mov     DWORD PTR -4[rbp], eax
    jmp     .while1
.endwhile1:
    mov     eax, 0
    leave
    ret
.endmain:
.section	.note.GNU-stack,"",@progbits

