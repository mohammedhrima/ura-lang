#include <stdio.h>
#include <stdbool.h>

bool _strcmp(char *left, char *right)
{
    size_t i = 0;
    if (!left && !right)
        return true;
    if (!left && right)
        return false;
    if (left && !right)
        return false;
    while (left[i] && right[i] && left[i] == right[i])
        i++;
    return (left[i] == right[i]);
}

int main()
{
    printf("> %d\n", _strcmp("abcd", "abcd"));
}