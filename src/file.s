.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

swap:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  QWORD PTR -8[rbp], rdi ;// assign [a] is_ref
    mov  QWORD PTR -16[rbp], rsi ;// assign [b] is_ref
    mov  rax, QWORD PTR -8[rbp]
    mov  eax, DWORD PTR [rax]
    mov  DWORD PTR -20[rbp], eax ;// assign [t]
    mov  rax, QWORD PTR -16[rbp]
    mov  eax, DWORD PTR [rax]
    mov  rbx, QWORD PTR -8[rbp]
    mov  DWORD PTR [rbx], eax ;// assign [a] is_ref
    mov  rax, QWORD PTR -16[rbp]
    mov  ebx, DWORD PTR -20[rbp]
    mov  DWORD PTR [rax], ebx ;// assign [b] is_ref
    mov  eax, 0
    leave 
    ret  
.endswap:
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 1 ;// assign [a]
    mov  DWORD PTR -8[rbp], 2 ;// assign [b]
    lea  rdi, .STR7[rip] ;// assign [rdi]
    lea  rsi, .STR1[rip] ;// assign [rsi]
    mov  edx, DWORD PTR -4[rbp] ;// assign [edx]
    lea  rcx, .STR2[rip] ;// assign [rcx]
    mov  r8d, DWORD PTR -8[rbp] ;// assign [r8d]
    lea  r9, .STR3[rip] ;// assign [r9]
    call printf
    lea  rdi, -4[rbp] ;// assign [a] is_ref
    lea  rsi, -8[rbp] ;// assign [b] is_ref
    call swap
    lea  rdi, .STR8[rip] ;// assign [rdi]
    lea  rsi, .STR4[rip] ;// assign [rsi]
    mov  edx, DWORD PTR -4[rbp] ;// assign [edx]
    lea  rcx, .STR5[rip] ;// assign [rcx]
    mov  r8d, DWORD PTR -8[rbp] ;// assign [r8d]
    lea  r9, .STR6[rip] ;// assign [r9]
    call printf
    mov  eax, 0
    leave 
    ret  
.endmain:.STR1: .string "before swap: a:"
.STR2: .string " b: "
.STR3: .string "\n"
.STR4: .string "after swap: a:"
.STR5: .string " b: "
.STR6: .string "\n"
.STR7: .string "%s""%d""%s""%d""%s"
.STR8: .string "%s""%d""%s""%d""%s"
.section	.note.GNU-stack,"",@progbits

