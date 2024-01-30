#include <stdlib.h>

size_t _strlen(char *str)
{
    size_t i = 0;
    while (str && str[i])
        i++;
    return i;
}

char *_strdup(char *str)
{
    char *res = calloc(_strlen(str) + 1, sizeof(char));
    size_t i = 0;
    while (str && str[i])
    {
        res[i] = str[i];
        i++;
    }
    return res;
}

int main()
{
    char *str = _strdup("abcde");
    free(str);
}