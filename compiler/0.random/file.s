.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
  push 1
  push 2
  push 3
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 5
  pop rdi
  pop rax
  add rax, rdi
  push rax
  pop rax
  ret
