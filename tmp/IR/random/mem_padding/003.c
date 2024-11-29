#include <stdio.h>
#include <string.h>

#define s(p) sizeof(p)

void print(size_t *ptr, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%zu ", ptr[i]);
    printf("\n");
}

int main()
{
    size_t arr[] = {4, 4, 4, 1, 4};
    size_t ali[] = {4, 4, 4, 1, 4};

    size_t res[sizeof(arr) / sizeof(size_t)];
    memset(res, 0, sizeof(res));
#if 1
    int offset = 0;
    for (size_t i = 0; i < sizeof(arr) / sizeof(size_t); i++)
    {
        printf("offset: %d, alignemen %zu\n", offset, ali[i]);
        offset = (offset + ali[i] - 1) / ali[i] * ali[i];
        res[i] = offset;
        offset += arr[i];
    }
#else
    size_t ptr = 0;
    for (size_t i = 0; i < sizeof(arr) / sizeof(size_t); i++)
    {
        ptr = (ptr + arr[i] - 1) / arr[i] * arr[i];
        res[i] = ptr;
        ptr += arr[i];
    }
#endif
    print(arr, sizeof(arr) / sizeof(size_t));
    print(res, sizeof(arr) / sizeof(size_t));
    printf("struct alignment: %d\n", offset);
    /*
    + aliginment dyal strutc huw max alignemnt dyal lmemebers
    + size dyaleha huwa offset dyal l'elemnt li westeha
    +
    */
}