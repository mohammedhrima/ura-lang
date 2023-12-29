#include <unistd.h>

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_putnbr(long num)
{
    size_t i = 0;
    if (num < 0)
    {
        write(1, "-", 1);
        num = -num;
    }
    if (num < 10)
        ft_putchar(num + '0');
    else
    {
        ft_putnbr(num / 10);
        ft_putnbr(num % 10);
    }
}

int main()
{
    long num = 1235;
    ft_putnbr(num);
    write(1, "\n", 1);
}