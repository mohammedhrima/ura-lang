#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void **buffer;
size_t pos;
size_t len;

void *allocate(size_t size)
{
    if (buffer == NULL)
    {
        len = 10;
        buffer = malloc(len * sizeof(void *));
    }
    if (pos + 1 == len)
        buffer = realloc(buffer, (len *= 2) * sizeof(void *));
    buffer[pos] = calloc(size + sizeof(int), 1);
    ((int *)buffer[pos])[0] = size;
    return buffer[pos++] + sizeof(int);
}

int size(void *ptr)
{
    ptr = ptr - sizeof(int);
    return (*(int *)ptr);
}

void free_buffer()
{
    for (size_t i = 0; i < pos; i++)
        free(buffer[i]);
    free(buffer);
}

int main()
{
    void *ptr = allocate(50);
    printf("has size: %d \n", size(ptr));
    free_buffer();
    // printf("has size: %d \n", size(allocate(60)));
    // printf("has size: %d \n", size(allocate(70)));
    // printf("has size: %d \n", size(allocate(80)));
    // printf("has size: %d \n", size(allocate(90)));
    // printf("has size: %d \n", size(allocate(100)));
    // printf("len: %zu, pos: %d\n", len, pos);
    // char *a = "abcdef";
    // char *b = calloc(strlen("abcdef") + 1, 1);
    // strcpy(b, "abcdef");
    // free(b);
}