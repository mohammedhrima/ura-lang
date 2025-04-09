.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    lea  rax, .STR1[rip]
    mov  QWORD PTR -16[rbp], rax ;// assign [user.name]
    mov  DWORD PTR -8[rbp], 27 ;// assign [user.age]
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "Mohammed"
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

