#include <stdio.h>

void func(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9,
          int x10, int x11, int x12, int x13, int x14, int x15, int x16, int x17,
          int x18, int x19, int x20)
{
    printf("Values: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
           x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20);
}

int main()
{
    int x1 = 1;
    int x2 = 2;
    int x3 = 3;
    int x4 = 4;
    int x5 = 5;
    int x6 = 6;
    int x7 = 7;
    int x8 = 8;
    int x9 = 9;
    int x10 = 10;
    int x11 = 11;
    int x12 = 12;
    int x13 = 13;
    int x14 = 14;
    int x15 = 15;
    int x16 = 16;
    int x17 = 17;
    int x18 = 18;
    int x19 = 19;
    int x20 = 20;

    func(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20);

    return 0;
}
