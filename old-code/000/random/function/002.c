#include <stdio.h>

long processIntegers(long *a, long *b, long *c, long *d, long *e, long *f, long *g, long *h, long *i, long *j,
                     long *k, long *l, long *m, long *n)
{
    *a = 77;
    long sum = *a + *b + *c + *d + *e + *f + *g + *h + *i + *j +
               *k + *l + *m + *n;
    return sum;
}

int main()
{
    long x1 = 1;
    long x2 = 2;
    long x3 = 3;
    long x4 = 4;
    long x5 = 5;
    long x6 = 6;
    long x7 = 7;
    long x8 = 8;
    long x9 = 9;
    long x10 = 10;
    long x11 = 11;
    long x12 = 12;
    long x13 = 13;
    long x14 = 14;
    processIntegers(
        &x1, &x2, &x3,
        &x4, &x5,
        &x6, &x7, &x8,
        &x9, &x10, &x11,
        &x12, &x13, &x14

    );
    printf(">>>> %ld\n", x1);
}
