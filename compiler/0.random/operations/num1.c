#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
/*
typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct
{
    long long quot;
    long long rem;
} lldiv_t;
*/

#define NUM_MAX 1000000
#define NUM_LEN 6
/*
    hold value between:
        0 - 1000000
*/
typedef struct Num Num;
struct Num
{
    long long value;
    Num *next;
};

Num *new_num(long long value)
{
    Num *new = calloc(1, sizeof(Num));
    new->value = value % NUM_MAX;
    if (value >= NUM_MAX)
        new->next = new_num(value / NUM_MAX);
    // printf("new num %ld\n", value);
    return new;
}

void print_num(Num *num)
{
    if (num->next)
        print_num(num->next);
    printf("%lld ", num->value);
}

void free_num(Num *num)
{
    if (num)
    {
        free_num(num->next);
        free(num);
    }
}

Num *add_num(Num *left, Num *right, long long value)
{
    value = value + (left ? left->value : 0) + (right ? right->value : 0);
    if (value)
    {
        Num *new = new_num(value % NUM_MAX);
        new->next = add_num(left ? left->next : NULL, right ? right->next : NULL, value / NUM_MAX);
        return new;
    }
    return NULL;
}

Num *string_to_num(char *str_value)
{
    printf("> '%s'\n", str_value);
    int len;
    if (!str_value || !(len = (int)strlen(str_value) - 1))
        return NULL;
    Num *new = new_num(0);
    // int tmp = (int)len % NUM_LEN;

    int i = 0;
    int j = 1;
    while (i <= len && i < NUM_LEN)
    {
        new->value = new->value + (str_value[len - i] - '0') * j;
        printf("> %3lld | i: %d\n", new->value, i);
        j *= 10;
        i++;
    }
    if (len - i > 0)
    {
        str_value[len - i + 1] = 0;
        new->next = string_to_num(str_value);
    }
    printf("new string_num %ld\n", new->value);
    return new;
}
/*
    - function that check if all nexts are zero
*/
int main()
{
#if 0
    Num *num1 = new_num(523372036854775807);
    Num *num2 = new_num(523372036854775807);
    Num *num3 = add_num(num1, num2, 0);
    print_num(num3);
    printf("\n");
#else
    char *value = strdup("123456789123456789");
    Num *num = string_to_num(value);
    print_num(num);
    printf("\n");
    free(value);
    free_num(num);
#endif
}