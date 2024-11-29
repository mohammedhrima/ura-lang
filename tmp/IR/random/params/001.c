#include <stdio.h>

void _putnbr(int x)
{
    printf("%d\n", x);
}

void func(
    int x1, int x2, int x3, int x4, int x5,
    int x6, int x7, int x8, int x9, int x10,
    int x11, int x12, int x13, int x14, int x15,
    int x16, int x17, int x18, int x19, int x20,
    int x21, int x22, int x23, int x24, int x25,
    int x26, int x27, int x28, int x29, int x30,
    int x31, int x32, int x33, int x34, int x35,
    int x36, int x37, int x38, int x39, int x40)
{
    _putnbr(x1);
    _putnbr(x2);
    _putnbr(x3);
    _putnbr(x4);
    _putnbr(x5);
    // _putnbr(x6);
    // _putnbr(x7);
    // _putnbr(x8);
    // _putnbr(x9);
    // _putnbr(x10);
    // _putnbr(x11);
    // _putnbr(x12);
    // _putnbr(x13);
    // _putnbr(x14);
    // _putnbr(x15);
    // _putnbr(x16);
    // _putnbr(x17);
    // _putnbr(x18);
    // _putnbr(x19);
    // _putnbr(x20);
    // _putnbr(x21);
    // _putnbr(x22);
    // _putnbr(x23);
    // _putnbr(x24);
    // _putnbr(x25);
    // _putnbr(x26);
    // _putnbr(x27);
    // _putnbr(x28);
    // _putnbr(x29);
    // _putnbr(x30);
    // _putnbr(x31);
    // _putnbr(x32);
    // _putnbr(x33);
    // _putnbr(x34);
    // _putnbr(x35);
    // _putnbr(x36);
    // _putnbr(x37);
    // _putnbr(x38);
    // _putnbr(x39);
    // _putnbr(x40);
}

void func2(int x0, int x2, int x3, int x4, int x5, int x6)
{
}

int main()
{
    func(
        1, 2, 3, 4, 5,
        6, 7, 8, 9, 10,
        11, 12, 13, 14, 15,
        16, 17, 18, 19, 20,
        21, 22, 23, 24, 25,
        26, 27, 28, 29, 30,
        31, 32, 33, 34, 35,
        36, 37, 38, 39, 40);
    func2(1, 2, 3, 4, 5, 6);

    return 0;
}
