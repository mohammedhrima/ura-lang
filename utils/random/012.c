#include <stdio.h>

void swap(int *a, int *b)
{
   int t = *a;
   *a = *b;
   *b = t;
}

int main()
{
   int a = 1;
   int b = 2;
   swap(&a, &b);
   printf("%s%d%s%d%s", "before swap: a:", a, " b: ", b, "\n");
}