#include <stdio.h>

int processIntegers(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j,
                    int k, int l, int m, int n)
{
    int sum = a + b + c + d + e + f + g + h + i + j +
              k + l + m + n ;
    return sum;
}

int main()
{
    return processIntegers(1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14);
}
