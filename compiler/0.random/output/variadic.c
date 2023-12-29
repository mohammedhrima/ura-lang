#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

char *to_find; // Declare to_find as a global variable

char *check(char *type, ...)
{
    va_list ap;
    va_start(ap, type);

    while (1)
    {
        printf("-> %s \n", type);
        if (type == NULL)
        {
            va_end(ap);
            return NULL;
        }
        if (strcmp(type, to_find) == 0)
        {
            va_end(ap);
            return type;
        }
        type = va_arg(ap, char *);
    }

    va_end(ap);
    return NULL;
}

int main()
{
    to_find = "e";
    printf("%s \n", check("a", "b", "c", "e", NULL));
    return 0;
}
