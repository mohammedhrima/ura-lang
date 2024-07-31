#include <stdio.h>

long processIntegers(long a, long b, long c, long d, long e, long f, long g, long h, long i, long j,
                    long k, long l, long m, long n)
{
    long sum = a + b + c + d + e + f + g + h + i + j +
              k + l + m + n ;
    return sum;
}

int main()
{
    return processIntegers(1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14);
}
