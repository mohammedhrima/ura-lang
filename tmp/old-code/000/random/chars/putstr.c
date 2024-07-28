#include <unistd.h>

size_t ft_strlen(char *str)
{
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

void ft_putstr(char *str)
{
    size_t len = ft_strlen(str);
    write(1, str, len);
}

int main()
{
    char *str = "abc\n";
    ft_putstr(str);
}