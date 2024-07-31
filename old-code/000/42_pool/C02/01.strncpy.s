.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
strncpy_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare src (ref) */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare dest (ref) */
   mov	  rax, QWORD PTR 24[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     QWORD PTR -24[rbp], 0 /* declare n */
   mov	  rax, QWORD PTR 32[rbp]
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR -32[rbp], 0 /* declare i */
   jmp     while1 /* jmp to while condition*/
while2: /* while bloc*/
   /* dest[] (char) */
   mov     rbx, QWORD PTR -16[rbp]
   mov     rbx, [rbx]
   mov     rcx, QWORD PTR -32[rbp]
   add     rbx, rcx
   mov     QWORD PTR -40[rbp], rbx
   /* src[] (char) */
   mov     rbx, QWORD PTR -8[rbp]
   mov     rbx, [rbx]
   mov     rcx, QWORD PTR -32[rbp]
   add     rbx, rcx
   mov     QWORD PTR -48[rbp], rbx
   mov     rax, QWORD PTR -48[rbp] /* assign from ref (null) */
   mov     al, BYTE PTR [rax]
   mov     rbx,  QWORD PTR -40[rbp]
   mov     BYTE PTR [rbx], al /* assign ref (null) */
   mov     rax, QWORD PTR -32[rbp]
   mov     rbx, 1
   add     rax, rbx
   mov     QWORD PTR -56[rbp], rax
   mov     rax, QWORD PTR -56[rbp]
   mov     QWORD PTR -32[rbp], rax /* assign i */
while1: /* while condition */
   /* src[] (char) */
   mov     rbx, QWORD PTR -8[rbp]
   mov     rbx, [rbx]
   mov     rcx, QWORD PTR -32[rbp]
   add     rbx, rcx
   mov     QWORD PTR -64[rbp], rbx
   mov     rax, QWORD PTR -64[rbp]
   mov     al, BYTE PTR [rax]
   mov     bl, 0
   cmp     al, bl
   setne   al
   /* && operation 0 */
   cmp     al, 1
   jne     while4
   mov     rax, QWORD PTR -32[rbp]
   mov     rbx, QWORD PTR -24[rbp]
   cmp     rax, rbx
   setl    al
while4:
   cmp     al, 1
   je      while2 /* je to while bloc*/
while3: /* end while */
   leave
   ret
end_strncpy_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare dest */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -8[rbp], rax /* assign  dest */
   mov     QWORD PTR -16[rbp], 0 /* declare src */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -16[rbp], rax /* assign  src */
   mov     rax, 2
   push    rax
   lea     rax, -8[rbp]
   push    rax /*ref dest*/
   lea     rax, -16[rbp]
   push    rax /*ref src*/
   call    strncpy_
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -16[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "abcdef"
STR2: .string "123"
STR3: .string "src: "
STR4: .string "\ndest: "
STR5: .string "\n"
.section	.note.GNU-stack,"",@progbits
