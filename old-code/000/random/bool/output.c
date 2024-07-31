#include <stdio.h>
#include <stdbool.h>
#include<unistd.h>

size_t _strlen(char *str)
{
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

void _putstr(char *str)
{
    size_t len = _strlen(str);
    write(1, str, len);
}

void _putbool(bool value)
{
    if(value)
        _putstr("True");
    else
        _putstr("False");
}

int main()
{
    bool value = true;
    _putbool(value);
}