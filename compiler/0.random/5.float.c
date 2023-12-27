#include <stdint.h>
#include <stdio.h>

int main()
{
    float a = 10;
    float b = 7;
    float c = a + b;
    printf("%f + %f = %f\n", a, b, c);

    // uint32_t u = *(uint32_t *)(&x);
    // printf("%zu\n", u);
    double d = 10;
    double e = 7;
    double f = d + e;
    printf("%lf + %f = %f\n", d , e, f);
}