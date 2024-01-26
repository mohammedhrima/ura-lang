#include <unistd.h>

void _putfloat(float num)
{
    if (num < 0)
    {
        _putchar('-');
        num = -num;
    }
    int integerPart = (int)num;
    _putnbr(integerPart);
    _putchar('.');
    float fractionalPart = num - integerPart;
    for (int i = 0; i < 6; ++i)
    {
        fractionalPart *= 10;
        _putnbr((int)fractionalPart);
        fractionalPart -= (int)fractionalPart;
    }
}

int main()
{
    float myFloat = 30.0;
    ft_putfloat(myFloat);

    return 0;
}