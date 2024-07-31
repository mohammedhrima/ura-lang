#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
/*
    hold value between:
        0 - 1000000000000000000
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
    return new;
}

void print_num(Num *num)
{
    if (num->next)
        print_num(num->next);
    printf("%lld ", num->value);
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

int main()
{
#if 0
    // fmod() : function
    div_t d = div(15, 2);
    // int x = 15;
    // int y = 7;
    // int z = x / y;
    printf("%d\n", d);
#elif 0
    long long l = 999999999 + 1;
    long long r = 999999999 + 1;
    long long m = 9223372036854775807;
    /*1000000000000000000*/
    long long res = l * r;
    printf("%lld\n", res);
#else
    Num *num1 = new_num(523372036854775807);
    Num *num2 = new_num(523372036854775807);
    Num *num3 = add_num(num1, num2, 0);
    print_num(num3);
    printf("\n");
#endif
}