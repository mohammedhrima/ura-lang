#include <stdio.h>

void func(int *p)
{
}

int main()
{
    int b = 10;
    int *c = &b;
    int d = 1;
    *c = d;
    // func(&b);
}