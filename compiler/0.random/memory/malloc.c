#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    // printf("has size: %d \n", size(allocate(50)));
    // printf("has size: %d \n", size(allocate(60)));
    // printf("has size: %d \n", size(allocate(70)));
    // printf("has size: %d \n", size(allocate(80)));
    // printf("has size: %d \n", size(allocate(90)));
    // printf("has size: %d \n", size(allocate(100)));
    // printf("len: %zu, pos: %d\n", len, pos);
    char *a = "abcdef";
    char *b = calloc(strlen(a) + 1, 1);
    strcpy(b, a);
    free(b);
}