.text
.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$123, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	ret

.LC0:
	.string	"%d"
