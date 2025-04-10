.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    lea  rax, .STR1[rip]
    mov  QWORD PTR -16[rbp], rax ;// assign [user.name]
    mov  DWORD PTR -8[rbp], 27 ;// assign [user.age]
    lea  rdi, .STR3[rip] ;// assign [rdi]
    mov  esi, DWORD PTR -8[rbp] ;// assign [esi]
    lea  rdx, .STR2[rip] ;// assign [rdx]
    mov  rcx, QWORD PTR -16[rbp] ;// assign [rcx]
    call printf
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR1: .string "Mohammed"
.STR2: .string ", "
.STR0: .string ""
.STR3: .string "%d%s%s"
.section	.note.GNU-stack,"",@progbits

