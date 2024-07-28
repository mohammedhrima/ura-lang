#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    char *str0 = malloc(100);
    char *str1 = "abcd";
    strcpy(str0, str1);
    printf("%s \n", str0);
}