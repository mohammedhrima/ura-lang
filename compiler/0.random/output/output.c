#include <unistd.h>

#if 0
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

// size_t ft_strlen(char *str)
// {
//     size_t i = 0;
//     while (str[i])
//         i++;
//     return i;
// }

// void ft_putstr(char *str)
// {
//     size_t len = ft_strlen(str);
//     write(1, str, len);
// }

int main()
{
    ft_putnbr(100);
}
#else
void ft_putchar(char c)
{
    write(1, &c, 1);
}

void func(long n)
{
    if (n > 10)
        func(n / 10);
    else
        ft_putchar(n + 'c');
}

int main()
{
    func(100);
}
#endif