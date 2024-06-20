#include <stdio.h>

int remain;
int divisor;

int division(int tempdividend, int tempdivisor)
{
    int quotient = 1;

    if (tempdivisor == tempdividend)
    {
        remain = 0;
        return 1;
    }
    else if (tempdividend < tempdivisor)
    {
        remain = tempdividend;
        return 0;
    }

    do
    {
        tempdivisor = tempdivisor << 1;
        quotient = quotient << 1;
    } while (tempdivisor <= tempdividend);

    /* Call division recursively */
    quotient = quotient + division(tempdividend - tempdivisor, divisor);

    return quotient;
}

int main()
{
    int dividen = 125454;
    divisor = 123;

    // printf("\nEnter the Dividend: ");
    // scanf("%d", &dividend);
    // printf("\nEnter the Divisor: ");
    // scanf("%d", &divisor);

    printf("%d / %d: quotient = %d\n", dividen, divisor, division(dividen, divisor));
    printf("%d / %d: remain   = %d\n", dividen, divisor, remain);
    // getch();
}