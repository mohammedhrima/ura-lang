#include <stdio.h>

struct var
{
    char a; // 1, 1
    int b;
    char c;
    long d;

};

int main()
{
    struct var v;
    v.a = 'a';
    v.b = 1;
    v.c = 'b';
    v.d = 2;
}