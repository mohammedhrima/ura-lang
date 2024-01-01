.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare stname */

   /* assign to stname */
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax
   /* declare ndname */

   /* assign to ndname */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax
   /* declare age */
   mov     QWORD PTR -24[rbp], 0
   sub     rsp, 30
   /* assign to age */
   mov     QWORD PTR -24[rbp], 25
   /* call output */
   lea   rbx, STR3[rip]
   call  Putstr
   /* call output */
   mov     rbx, QWORD PTR -8[rbp]
   call    Putstr
   /* call output */
   lea   rbx, STR4[rip]
   call  Putstr
   /* call output */
   mov     rbx, QWORD PTR -16[rbp]
   call    Putstr
   /* call output */
   lea   rbx, STR5[rip]
   call  Putstr
   /* call output */
   mov   rax, QWORD PTR -24[rbp]
   mov   rdi, rax
   call  Putnbr
   /* call output */
   lea   rbx, STR6[rip]
   call  Putstr

   leave
   ret

STR1:
   .string    "mohammed"
STR2:
   .string    "mohammed"
STR3:
   .string    "stname: "
STR4:
   .string    " ndname: "
STR5:
   .string    " age: "
STR6:
   .string    "\n"
Len:
   push      rbp
   mov       rbp, rsp
   /* char *str */
   mov       QWORD PTR -8[rbp], rbx
   /* size_t i = 0 */
   mov       QWORD PTR -16[rbp], 0
   jmp       Label2
Label1:
   /* i++ */
   add       QWORD PTR -16[rbp], 1
Label2:
   mov       rax, QWORD PTR -8[rbp]
   mov       rdx, QWORD PTR -16[rbp]
   /* str + i */
   add       rax, rdx
   movzx     rax, BYTE PTR[rax]
   cmp       rax, 0
   jne       Label1
   /* return i */
   mov       rax, QWORD PTR -16[rbp]
   mov       rsp, rbp
   pop       rbp
   ret

Putstr:
   push     rbp
   mov      rbp, rsp
   /* char *str */
   mov      QWORD PTR -8[rbp], rbx
   /* fd */
   mov      rdi, 1
   /* pointer */
   mov      rsi, rbx
   call     Len
   /* lenght */
   mov      rdx, rax
   call     write@PLT
   mov      rsp, rbp
   pop      rbp
   ret

Putchar:
   push	    rbp
   mov	    rbp, rsp
   sub	    rsp, 16
   mov	    eax, edi
   mov	    BYTE PTR -4[rbp], al
   lea	    rax, -4[rbp]
   mov	    edx, 1
   mov	    rsi, rax
   mov	    edi, 1
   call	    write@PLT
   nop
   leave
   ret

STR7: .string "-"
Putnbr:
   push	    rbp
   mov		rbp, rsp
   sub		rsp, 32
   mov		QWORD PTR -24[rbp], rdi
   mov		QWORD PTR -8[rbp], 0
   cmp		QWORD PTR -24[rbp], 0
   jns		Label3
   mov		edx, 1
   lea		rax, STR7[rip]
   mov		rsi, rax
   mov		edi, 1
   call	    write@PLT
   neg		QWORD PTR -24[rbp]
Label3:
   cmp		QWORD PTR -24[rbp], 9
   jg		Label4
   mov		rax, QWORD PTR -24[rbp]
   add		eax, 48
   movsx	    eax, al
   mov		edi, eax
   call	    Putchar
   jmp		Label5
Label4:
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	    rdx
   mov		rax, rdx
   sar		rax, 2
   sar		rcx, 63
   mov		rdx, rcx
   sub		rax, rdx
   mov		rdi, rax
   call	    Putnbr
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	    rdx
   sar		rdx, 2
   mov		rax, rcx
   sar		rax, 63
   sub		rdx, rax
   mov		rax, rdx
   sal		rax, 2
   add		rax, rdx
   add		rax, rax
   sub		rcx, rax
   mov		rdx, rcx
   mov		rdi, rdx
   call	    Putnbr
Label5:
   nop
   leave
   ret

