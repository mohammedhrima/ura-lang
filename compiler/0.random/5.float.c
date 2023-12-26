#include <stdint.h>
#include <stdio.h>

int main()
{
    float x = 10;
    float y = 7;
    float z = x + y;

    // uint32_t u = *(uint32_t *)(&x);
    // printf("%zu\n", u);
    printf("%f + %f = %f\n", x, y, z);
}