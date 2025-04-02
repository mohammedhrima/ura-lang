.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    lea  rax, .STR2[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [planet]
    .if1:
    mov  rdi, QWORD PTR -8[rbp] ;// assign [rdi]
    lea  rsi, .STR3[rip] ;// assign [rsi]
    call strcmp
    cmp  eax, 0
    jne   .elif3
        lea  rdi, .STR4[rip] ;// assign [rdi]
        call putstr
        jmp  .endif1
    .elif3:
    mov  rdi, QWORD PTR -8[rbp] ;// assign [rdi]
    lea  rsi, .STR5[rip] ;// assign [rsi]
    call strcmp
    cmp  eax, 0
    jne   .elif5
        lea  rdi, .STR6[rip] ;// assign [rdi]
        call putstr
        jmp  .endif1
    .elif5:
    mov  rdi, QWORD PTR -8[rbp] ;// assign [rdi]
    lea  rsi, .STR7[rip] ;// assign [rsi]
    call strcmp
    cmp  eax, 0
    jne   .else7
        lea  rdi, .STR8[rip] ;// assign [rdi]
        call putstr
        jmp  .endif1
    .else7:
        lea  rdi, .STR9[rip] ;// assign [rdi]
        call putstr
    .endif1:
    mov  eax, 0
    leave 
    ret  
.endmain:.STR2: .string "Mars"
.STR3: .string "Earth"
.STR4: .string "🌍 Welcome home, astronaut!"
.STR5: .string "Mars"
.STR6: .string "👽 Greetings Martian explorer! Temperature: -80°C"
.STR7: .string "Jupiter"
.STR8: .string "⚡ Danger! Extreme gas giant storms detected!"
.STR9: .string "🚀 Unknown planet. Proceeding with caution..."
.section	.note.GNU-stack,"",@progbits

