.text
putstr:
   push     rbp
   mov      rbp, rsp
   /* char *str */
   mov      QWORD PTR -8[rbp], rbx
   /* fd */
   mov      rdi, 1
   /* pointer */
   mov      rsi, rbx
   call     len
   /* lenght */
   mov      rdx, rax
   call     write@PLT
   mov      rsp, rbp
   pop      rbp
   ret

putchar:
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

len:
   push      rbp
   mov       rbp, rsp
   /* char *str */
   mov       QWORD PTR -8[rbp], rbx
   /* size_t i = 0 */
   mov       QWORD PTR -16[rbp], 0
   jmp       len2
len1:
   /* i++ */
   add       QWORD PTR -16[rbp], 1
len2:
   mov       rax, QWORD PTR -8[rbp]
   mov       rdx, QWORD PTR -16[rbp]
   /* str + i */
   add       rax, rdx
   movzx     rax, BYTE PTR[rax]
   cmp       rax, 0
   jne       len1
   /* return i */
   mov       rax, QWORD PTR -16[rbp]
   mov       rsp, rbp
   pop       rbp
   ret

sign: .string "-"
putnbr:
   push	   rbp
   mov		rbp, rsp
   sub		rsp, 32
   mov		QWORD PTR -24[rbp], rdi
   mov		QWORD PTR -8[rbp], 0
   cmp		QWORD PTR -24[rbp], 0
   jns		putnbr0
   mov		edx, 1
   lea		rax, sign[rip]
   mov		rsi, rax
   mov		edi, 1
   call	   write@PLT
   neg		QWORD PTR -24[rbp]
putnbr0:
   cmp		QWORD PTR -24[rbp], 9
   jg		   putnbr1
   mov		rax, QWORD PTR -24[rbp]
   add		eax, 48
   movsx	   eax, al
   mov		edi, eax
   call	   putchar
   jmp		putnbr2
putnbr1:
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	   rdx
   mov		rax, rdx
   sar		rax, 2
   sar		rcx, 63
   mov		rdx, rcx
   sub		rax, rdx
   mov		rdi, rax
   call	   putnbr
   mov		rcx, QWORD PTR -24[rbp]
   movabs	rdx, 7378697629483820647
   mov		rax, rcx
   imul	   rdx
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
   call	   putnbr
putnbr2:
   nop
   leave
   ret

/* allocation function */
.bss
	head:	.zero	8
	curr:	.zero	8
	pos:	.zero	8

allocate:
	push	rbp
	mov		rbp, rsp
	push	rbx
	sub		rsp, 40
	mov		QWORD PTR -40[rbp], rdi
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		allocate0
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR curr[rip], rax
	jmp		allocate1
allocate0:
	mov		rax, QWORD PTR pos[rip]
	cmp		rax, 100
	jne		allocate1
	mov		QWORD PTR pos[rip], 0
	mov		rbx, QWORD PTR curr[rip]
	mov		esi, 808
	mov		edi, 1
	call	calloc@PLT
	mov		QWORD PTR 800[rbx], rax
	mov		rax, QWORD PTR curr[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR curr[rip], rax
allocate1:
	mov		rax, QWORD PTR -40[rbp]
	add		rax, 8
	mov		esi, 1
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -24[rbp], rax
	mov		rax, QWORD PTR -24[rbp]
	mov		rdx, QWORD PTR -40[rbp]
	mov		QWORD PTR [rax], rdx
	mov		rdx, QWORD PTR curr[rip]
	mov		rax, QWORD PTR pos[rip]
	lea		rcx, 1[rax]
	mov		QWORD PTR pos[rip], rcx
	mov		rcx, QWORD PTR -24[rbp]
	mov		QWORD PTR [rdx+rax*8], rcx
	mov		rax, QWORD PTR -24[rbp]
	add		rax, 8
	mov		rbx, QWORD PTR -8[rbp]
	leave
	ret

free_memory:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16
	jmp		free_memory0
free_memory3:
	mov		DWORD PTR -4[rbp], 0
	jmp		free_memory1
free_memory2:
	mov		rax, QWORD PTR head[rip]
	mov		edx, DWORD PTR -4[rbp]
	movsx	rdx, edx
	mov		rax, QWORD PTR [rax+rdx*8]
	mov		rdi, rax
	call	free@PLT
	add		DWORD PTR -4[rbp], 1
free_memory1:
	cmp		DWORD PTR -4[rbp], 99
	jle		free_memory2
	mov		rax, QWORD PTR head[rip]
	mov		QWORD PTR -16[rbp], rax
	mov		rax, QWORD PTR head[rip]
	mov		rax, QWORD PTR 800[rax]
	mov		QWORD PTR head[rip], rax
	mov		rax, QWORD PTR -16[rbp]
	mov		rdi, rax
	call	free@PLT
free_memory0:
	mov		rax, QWORD PTR head[rip]
	test	rax, rax
	jne		free_memory3
	nop
	nop
	leave
	ret
