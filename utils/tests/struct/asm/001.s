.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    lea  rax, .STR2[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [user.name]
    mov  DWORD PTR -16[rbp], 27 ;// assign [user.age]
    mov  eax, 0
    leave 
    ret  
.endmain:.STR2: .string "Mohammed"
.section	.note.GNU-stack,"",@progbits

