.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

.LC0:
    .string "abc\n"

ft_strlen:
    push rbp
    /* char *str */
    mov QWORD PTR -8[rbp], rbx
    /* size_t i = 0 */
    mov QWORD PTR -16[rbp], 0
    jmp .L2
.L3:
    /* i++ */
    add QWORD PTR -16[rbp], 1
.L2:
    mov rax, QWORD PTR -8[rbp]
    mov rdx, QWORD PTR -16[rbp]
    /* str + i */
    add rax, rdx
    movzx rax, BYTE PTR[rax]
    cmp rax, 0
    jne .L3
    /* return i */
    mov rax, QWORD PTR -16[rbp]
    pop rbp
    ret       

ft_putstr:
    push rbp
    /* char *str */
    mov QWORD PTR -8[rbp], rbx
    /* fd */
    mov rdi, 1
    /* pointer */
    mov rsi, rbx
    call ft_strlen
    /* lenght */
    mov rdx, rax
    call write@PLT
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp
    sub rsp, 24
    lea rax, .LC0[rip]
    mov QWORD PTR -8[rbp], rax
    mov rbx, rax
    call ft_putstr
    mov rax, 0
    leave
    ret
