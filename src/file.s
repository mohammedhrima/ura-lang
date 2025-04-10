.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    lea  rax, .STR1[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [str]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR1: .string "fffff"
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

