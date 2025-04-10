.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    lea  rax, .STR0[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [str]
    lea  rax, .STR1[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [str]
    mov  DWORD PTR -12[rbp], 0 ;// assign [a]
    mov  rdi, QWORD PTR -8[rbp] ;// assign [rdi]
    call strlen
    mov  DWORD PTR -12[rbp], eax ;// assign [a]
    mov  edi, DWORD PTR -12[rbp] ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR1: .string "fffff"
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

