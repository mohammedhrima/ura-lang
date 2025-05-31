.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -8[rbp], esi ;// assign [user.b]
    mov  QWORD PTR -12[rbp], rdx ;// assign [user.info.name]
    mov  DWORD PTR -4[rbp], 123 ;// assign [user.a]
    mov  eax, 0
    leave 
    ret  
.endm:

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -16[rbp], 0 ;// assign [user.a]
    mov  DWORD PTR -12[rbp], 0 ;// assign [user.b]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -24[rbp], rax ;// assign [user.info.name]
    mov  edi, DWORD PTR -16[rbp] ;// assign [user.a]
    mov  esi, DWORD PTR -12[rbp] ;// assign [user.b]
    mov  rdx, QWORD PTR -24[rbp] ;// assign [user.info.name]
    call m
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

