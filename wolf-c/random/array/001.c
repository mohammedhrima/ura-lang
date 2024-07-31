#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _memcpy(void *destination, void *source, size_t len)
{
    size_t i;
    char *pointer1;
    char *pointer2;

    pointer1 = (char *)destination;
    pointer2 = (char *)source;
    i = 0;
    while (i < len)
    {
        pointer1[i] = pointer2[i];
        i++;
    }
}

void *_memdub(void *src, size_t len, size_t size)
{
    void *res = calloc(len, size);
    _memcpy(res, src, len * size);
    return res;
}

int main()
{
    int arr[] = {11,22,33,44,55 };
    int *cpy = _memdub(arr, 5, sizeof(int));
    int i = 0;
    while (i < 5)
        printf("%d ", arr[i++]);
    printf("\n");
}