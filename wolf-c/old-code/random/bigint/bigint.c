#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM 1000000
#define MAX_LEN 6
typedef struct
{
    int *array;
    size_t len;
} Num;

Num *new_num(char *value)
{
    Num *new = calloc(1, sizeof(Num));
    // new->array = calloc(new->len, sizeof(int));
    int len = (int)strlen(value);
    // new->len = 0;
    int j = -1;
    int i = 0;
    while (i < len)
    {
        if (i % MAX_LEN == 0)
        {
            j++;
            int *tmp = calloc(new->len++, sizeof(int));
            if (new->array)
            {
                memcpy(tmp, new->array, (new->len - 1) * sizeof(int));
                free(new->array);
                new->array = tmp;
                printf("> %d\n", new->array[j]);
            }
            else
                new->array = tmp;
        }
        new->array[j] = new->array[j] * 10 + (value[i] - '0');
        i++;
    }
    return new;
};

void free_num(Num *num)
{
    free(num->array);
    free(num);
}

void print_num(Num *num)
{
    for (size_t i = 0; i < num->len; i++)
        printf("%d ", num->array[i]);
}

int main()
{
    Num *num = new_num("111111222222333333");
    print_num(num);
    free_num(num);
    printf("\n");
}