#include <stdio.h>

void func(int *p)
{
    printf("%d", *p);
}

int main()
{
    int c = 10;
    func(&c);
}