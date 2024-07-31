#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define SIZE 100
typedef struct Memo Memo;
struct Memo
{
    uintptr_t buffer[SIZE];
    Memo *next;
};

Memo *head;
Memo *curr;
size_t pos;

void *_allocate(size_t size)
{
    if (head == NULL)
    {
        head = calloc(1, sizeof(Memo));
        curr = head;
    }
    else if (pos == SIZE)
    {
        // printf("reached end\n");
        pos = 0;
        curr->next = calloc(1, sizeof(Memo));
        curr = curr->next;
    }
    // printf("%zu ", pos);
    void *ptr = calloc(size + sizeof(size_t), sizeof(char));
    ((size_t *)ptr)[0] = size;
    curr->buffer[pos++] = (uintptr_t)ptr;
    return ptr + sizeof(size_t);
}

void _free_memory()
{
    while (head)
    {
        for (int i = 0; i < SIZE; i++)
            free((void *)head->buffer[i]);
        Memo *tmp = head;
        head = head->next;
        free(tmp);
    }
}

size_t _strlen(char *str)
{
    size_t i = 0;
    while (str && str[i])
        i++;
    return i;
}

void _strcpy(char *destination, char *source)
{
    int i = 0;
    while (destination && source && source[i])
    {
        destination[i] = source[i];
        i++;
    }
}

char *_strdup(char *str)
{
    if (str == NULL)
        return _allocate(1);
    char *res = _allocate(_strlen(str) + 1);
    _strcpy(res, str);
    return res;
}

void _putchar(char c)
{
    write(1, &c, 1);
}

int main()
{
    char *str = "abcdef";
    str = _strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    str = _strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    str = _strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    for (int i = 0; str[i]; i++)
        _putchar(str[i]);
    _putchar('\n');
    _free_memory();
}