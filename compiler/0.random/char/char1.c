#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char *strjoin(char *left, char *right)
// {
//     char *res = calloc(strlen(left) + strlen(right) + 1, sizeof(char));
//     strcpy(res, left);
//     strcpy(res + strlen(res), right);
//     return res;
// }

void **pointer;
size_t size = 100;
int pos = 0;

void *allocate(size_t len)
{
    if (pointer == NULL)
        pointer = malloc(size * sizeof(void *));
    void *ptr = calloc(len, 1);
    pointer[pos++] = ptr;
    if (pos + 10 == size)
    {
        void *tmp = malloc(2 * size * sizeof(void *));
        memcpy(tmp, pointer, pos * sizeof(void *));
        free(pointer);
        pointer = tmp;
        size *= 2;
    }
    return ptr;
}

void free_pointer()
{
    size_t i = 0;
    while (i < pos)
        free(pointer[i++]);
    free(pointer);
}

int main()
{
    allocate(size);
    free_pointer();
}