.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// declare [a]
    .if1:
    cmp  DWORD PTR -4[rbp], 1
    jne   .elif2
        lea  rdi, .STR1[rip] ;// assign [rdi]
        call putstr
        jmp  .endif1
    .elif2:
    cmp  DWORD PTR -4[rbp], 0
    jne   .else3
        lea  rdi, .STR2[rip] ;// assign [rdi]
        call putstr
        jmp  .endif1
    .else3:
        lea  rdi, .STR3[rip] ;// assign [rdi]
        call putstr
    .endif1:
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "cond 1\n"
.STR2: .string "cond 2\n"
.STR3: .string "cond 3\n"
.section	.note.GNU-stack,"",@progbits

