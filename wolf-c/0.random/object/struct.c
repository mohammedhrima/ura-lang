#include <stdio.h>

typedef struct
{
    int x;
    int y;
    int z;
} var;

void func()
{
    int x = 10;
    int y = 5;
    int z = 7;
}

var func1(int x, int y, int z)
{
    return (var){x, y, z};
}

int main()
{
    var v;
#if 0
    v = (var){
        .x = 10,
        .y = 11,
        .z = 12,
    };
#else
    v = func1(1, 2, 3);
#endif
    // func(v);
    printf("v: (%d, %d, %d)\n", v.x, v.y, v.z);
}