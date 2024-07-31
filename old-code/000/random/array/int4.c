#include <stdio.h>
#include <stdlib.h>

int main()
{
    long len = 4;
    long *arr = calloc(len + 1, sizeof(long));
#if 0
    long i = 0;
    while (i < len)
    {
        arr[i] = i + 1;
        i++;
    }
#else
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    arr[3] = 3;
#endif
}

/*

main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 32
	mov		QWORD PTR -16[rbp], 4
	mov		rax, QWORD PTR -16[rbp]
	add		rax, 1
	mov		esi, 8
	mov		rdi, rax
	call	calloc@PLT
	mov		QWORD PTR -24[rbp], rax
	mov		QWORD PTR -8[rbp], 0
	jmp		L0
L1:
	mov		rax, QWORD PTR -8[rbp]
	lea		rdx, 0[0+rax*8]
	mov		rax, QWORD PTR -24[rbp]
	add		rax, rdx
	mov		rdx, QWORD PTR -8[rbp]
	add		rdx, 1
	mov		QWORD PTR [rax], rdx
	add		QWORD PTR -8[rbp], 1
L0:
	mov		rax, QWORD PTR -8[rbp]
	cmp		rax, QWORD PTR -16[rbp]
	jl		L1
	mov		eax, 0
	leave
	ret
*/