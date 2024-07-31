#include <stdio.h>

int main()
{
    char str[] = {'a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 
    'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 
    'a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e',
     'f', 'a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 
     'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 0};
    str[0] = 'q';
    printf("%s \n", str);
}