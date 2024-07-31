#include <stdio.h>
#include <stdlib.h>
// long func1(long x)
// {
//     long y = x / 6;
//     return y;
// }

int main()
{
    long long n = 1811111999111944441;
    long long m = n / 70;
    // void *ptr = malloc(sizeof(long long) * 4);
    // long m = func1(n);
    
    printf("> %ld\n", n);
}

/*
.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
	.globl	main

STR0:
	.string	"> %ld\n"

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	
	mov	QWORD PTR -16[rbp], 1811111111
	mov	rax, QWORD PTR -16[rbp]

	cdq
	mov rbx, 2
	idiv rbx
	
	mov	rsi, rax
	lea	rax, STR0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	leave
	ret
*/