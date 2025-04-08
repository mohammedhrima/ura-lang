#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int a;
    char b;
    int c;
    int d;
} user0;

typedef struct
{
    char a;
    char b;
    int c;
    char d;
} user1;

typedef struct
{
    long a;
    char b;
    char c;
    int d;
} user2;

typedef struct
{
    long a;
    int b;
} user3;

int main()
{
    user0 u0 = {.a = 1, .b = 2, .c = 3, .d = 4};
    user1 u1 = {.a = 1, .b = 2, .c = 3, .d = 4};
    user2 u2 = {.a = 1, .b = 2, .c = 3, .d = 4};
    printf("%d\n", sizeof(user2));
    user3 u3 = {.a = 1, .b = 2};
}
