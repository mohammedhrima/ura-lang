
// int putchar(char c)
// {
//   return write(1, &c, 1);
// }
#include <stdio.h>
#include <unistd.h>

// int putnbr(int n)
// {
//   int res = 0;
//   if (n == -2147483648)
//     return write(1, "-2147483648", 11);
//   if (n < 0)
//   {
//     res += putchar('-');
//     n = -n;
//   }
//   if (n > 9)
//     res += putnbr(n / 10);
//   res += putchar(n % 10 + '0');
//   return res;
// }

int hello(char *str)
{
  return 1;
}
// #include <stdio.h>
int main()
{
  // hello("abc");
  // puts("fffff");
  // char *str = "Ffffff";
  // int i = strlen(str);
  // return i;
  int a = 5;
  int b = 10;
  int i = 0;
  while(i < 10)
  {
    write(1, "x", 1);
    // putnbr(i);
    i+= 1;
  }
}

/*
main:
	push    rbp
	mov     rbp, rsp
	sub		rsp, 8
	//      assign [i]
	mov     DWORD PTR -4[rbp], 0
.while1:
	cmp		DWORD PTR -4[rbp], 9
	jg     .endwhile1
	//      assign [edi]
	mov     edi, 1
	//      assign [rsi]
	lea     rsi, .STR1[rip]
	//      assign [edx]
	mov     edx, 1
	call    write@PLT
	//      assign [i]
	add     DWORD PTR -4[rbp], 1
	jmp     .while1
.endwhile1:
	mov     eax, DWORD PTR -4[rbp]
	leave   
	ret 
*/