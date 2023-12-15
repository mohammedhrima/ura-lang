.globl main
main:
   push   %rbp
   mov    %rsp, %rbp
   movl   $72, -4(%rbp)
   mov    $0, %rax
   pop    %rbp
   ret


