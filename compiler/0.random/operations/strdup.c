#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// #define SIZE 100
// typedef struct Memo Memo;
// struct Memo
// {
//     uintptr_t buffer[SIZE];
//     Memo *next;
// };

// Memo *head;
// Memo *curr;
// size_t pos;

// void *allocate(size_t size)
// {
//     if (head == NULL)
//     {
//         head = calloc(1, sizeof(Memo));
//         curr = head;
//     }
//     else if (pos == SIZE)
//     {
//         // printf("reached end\n");
//         pos = 0;
//         curr->next = calloc(1, sizeof(Memo));
//         curr = curr->next;
//     }
//     // printf("%zu ", pos);
//     void *ptr = calloc(size + sizeof(size_t), sizeof(char));
//     ((size_t *)ptr)[0] = size;
//     curr->buffer[pos++] = (uintptr_t)ptr;
//     return ptr + sizeof(size_t);
// }

// void free_memory()
// {
//     while (head)
//     {
//         for (int i = 0; i < SIZE; i++)
//             free((void *)head->buffer[i]);
//         Memo *tmp = head;
//         head = head->next;
//         free(tmp);
//     }
// }

char *ft_strdup(char *str)
{
    if (str == NULL)
        return allocate(1);
    char *res = allocate(strlen(str) + 1);
    strcpy(res, str);
    return res;
}

int main()
{
    // void *ptr = allocate(100);
    // ptr = allocate(90);
    // printf("%zu\n", ((size_t *)ptr)[-1]);
    // ptr = allocate(80);
    // printf("%zu\n", ((size_t *)ptr)[-1]);
    // ptr = allocate(70);
    // printf("%zu\n", ((size_t *)ptr)[-1]);
    // ptr = allocate(60);
    // printf("%zu\n", ((size_t *)ptr)[-1]);
    char *str = "abcdef";
    str = ft_strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    str = ft_strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    str = ft_strdup(str);
    printf("%zu\n", ((size_t *)str)[-1]);
    free_memory();
}