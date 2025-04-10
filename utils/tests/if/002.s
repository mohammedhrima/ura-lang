.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    .if1:
    cmp  DWORD PTR -4[rbp], 1
    jne   .else6
        mov  DWORD PTR -4[rbp], 2 ;// assign [i]
        .if2:
            cmp  DWORD PTR -4[rbp], 2
            jne   .elif3
                mov  DWORD PTR -4[rbp], 3 ;// assign [i]
            jmp  .endif2
            .elif3:
            cmp  DWORD PTR -4[rbp], 3
            jne   .else4
                mov  DWORD PTR -4[rbp], 4 ;// assign [i]
            jmp  .endif2
            .else4:
                mov  DWORD PTR -4[rbp], 2 ;// assign [i]
                .if5:
                cmp  DWORD PTR -4[rbp], 2
                jne   .endif5
                    mov  DWORD PTR -4[rbp], 5 ;// assign [i]
                .endif5:
        .endif2:
        jmp  .endif1
    .else6:
        mov  DWORD PTR -4[rbp], 4 ;// assign [i]
    .endif1:
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

