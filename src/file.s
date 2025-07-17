.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// assign [a]
    mov  DWORD PTR -4[rbp], 11 ;// assign [a]
    lea  rdi, .STR2[rip] ;// assign [rdi]
    lea  rsi, .STR1[rip] ;// assign [rsi]
    mov  edx, DWORD PTR -4[rbp] ;// assign [edx]
    call printf
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR1: .string "Hello this is "
.STR2: .string "%s%d"
.section	.note.GNU-stack,"",@progbits

