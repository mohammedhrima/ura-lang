.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    mov  DWORD PTR -4[rbp], 1 ;// assign [i]
    mov  DWORD PTR -8[rbp], 0 ;// assign [a]
    mov  DWORD PTR -8[rbp], 10 ;// assign [a]
    mov  DWORD PTR -12[rbp], 0 ;// assign [b]
    mov  DWORD PTR -12[rbp], 5 ;// assign [b]
    mov  DWORD PTR -16[rbp], 0 ;// assign [c]
    mov  DWORD PTR -16[rbp], 6 ;// assign [c]
    mov  DWORD PTR -20[rbp], 0 ;// assign [d]
    mov  DWORD PTR -20[rbp], 10 ;// assign [d]
    .while1:
    cmp  DWORD PTR -4[rbp], 10
    jge   .endwhile1
        mov  edi, 1 ;// assign [edi]
        lea  rsi, .STR1[rip] ;// assign [rsi]
        mov  edx, 1 ;// assign [edx]
        call write
        add  DWORD PTR -4[rbp], 1 ;// add_assign [i]
        add  DWORD PTR -8[rbp], 1 ;// add_assign [a]
        add  DWORD PTR -12[rbp], 1 ;// add_assign [b]
        jmp  .while1
    .endwhile1:
    mov  eax, DWORD PTR -4[rbp]
    leave 
    ret  
.endmain:
.STR1: .string "x"
.section	.note.GNU-stack,"",@progbits

