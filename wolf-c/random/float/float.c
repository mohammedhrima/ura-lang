#include <unistd.h>

void _putchar(char c)
{
    write(1, &c, 1);
}

void _putnbr(long num)
{
    if (num < 0)
    {
        num = -num;
        _putchar('-');
    }
    if (num >= 10)
    {
        _putnbr(num / 10);
        _putchar(num % 10 + '0');
    }
    else
        _putchar(num + '0');
}

void _putfloat(float num)
{
    if (num < 0)
    {
        _putchar('-');
        num = -num;
    }
    long integerPart = (long)num;
    _putnbr(integerPart);
    _putchar('.');
    float fractionalPart = num - integerPart;
    for (int i = 0; i < 6; ++i)
    {
        fractionalPart *= 10;
        _putnbr((long)fractionalPart);
        fractionalPart -= (long)fractionalPart;
    }
}

int main()
{
    float myFloat = 30.0;
    float *ptr = &myFloat;
#if 1
    _putfloat(*ptr);
#else
    _putfloat(30.0);
#endif

    return 0;
}