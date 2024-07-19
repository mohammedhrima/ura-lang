#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *str = strdup("afhhfh5fgh21fffffj56fg4j65ghbcd");
    printf("%s\n", str);
    free(str);
}