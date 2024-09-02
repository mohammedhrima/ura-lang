#include <stdio.h>

#if 1

void foo(int a, int b, int c, int e, int f, int g, int h)
{
    printf("%d", a);
}

int main()
{
    int a = 10;
    int b = 11;
    int c = 12;
    int d = 13;
    int e = 14;
    foo(a, a, a, a, a, a, a);
}
#else
int main()
{
    int sum = 0;
    for (int i = 1; i <= 10; ++i)
    {
        sum += i;
    }
    printf("Sum: %d\n", sum);
    return 0;
}
#endif