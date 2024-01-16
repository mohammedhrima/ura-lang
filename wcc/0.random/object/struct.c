#include <stdio.h>

typedef struct
{
    int x;
    int y;
    int z;
} var;

void func(var x)
{
}

void func1(long x)
{
}

int main()
{
    var v = {
        .x = 10,
        .y = 11,
        .z = 12,
    };
    func(v);
}