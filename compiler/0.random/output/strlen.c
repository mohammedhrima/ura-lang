#include <unistd.h>

size_t ft_strlen(char *str)
{
    size_t i = 0;
    while(str[i])
        i++;
    return i;
}

int main()
{
    char *str = "abc";
    ft_strlen(str);
}