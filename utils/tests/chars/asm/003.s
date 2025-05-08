.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  rdi, 1 ;// assign [size]
    mov  rsi, 2 ;// assign [count]
    call calloc
    mov  QWORD PTR -8[rbp], rax ;// assign [str]
    mov  rax, QWORD PTR -8[rbp]
    add  rax, 0
    mov  BYTE PTR [rax], 97 ;// assign [rax] is_ref
    lea  rdi, .STR1[rip] ;// assign [rdi]
    mov  rsi, QWORD PTR -8[rbp] ;// assign [rsi]
    call printf
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.STR1: .string "%s"
.section	.note.GNU-stack,"",@progbits

