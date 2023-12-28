#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 1000

typedef struct
{
    int digits[MAX_DIGITS];
    int length;
} BigInteger;

void initialize(BigInteger *num, const char *str)
{
    int len = strlen(str);
    num->length = len;
    for (int i = 0; i < len; i++)
        num->digits[i] = str[len - 1 - i] - '0';
}

void printBigInteger(const BigInteger *num)
{
    for (int i = num->length - 1; i >= 0; i--)
        printf("%d", num->digits[i]);
    printf("\n");
}

void add(const BigInteger *a, const BigInteger *b, BigInteger *result)
{
    int carry = 0;
    int maxLen = (a->length > b->length) ? a->length : b->length;

    for (int i = 0; i < maxLen; i++)
    {
        int sum = carry + a->digits[i] + b->digits[i];
        result->digits[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry > 0)
    {
        result->digits[maxLen] = carry;
        result->length = maxLen + 1;
    }
    else
        result->length = maxLen;
}

void multiply(const BigInteger *a, const BigInteger *b, BigInteger *result)
{
    // Initialize result to 0
    for (int i = 0; i < MAX_DIGITS; i++)
        result->digits[i] = 0;
    result->length = 0;
    for (int i = 0; i < a->length; i++)
    {
        int carry = 0;
        for (int j = 0; j < b->length; j++)
        {
            int product = a->digits[i] * b->digits[j] + result->digits[i + j] + carry;
            result->digits[i + j] = product % 10;
            carry = product / 10;
        }
        while (carry > 0)
        {
            result->digits[i + b->length] += carry;
            carry /= 10;
        }
    }
    // Find the length of the result
    int i;
    for (i = MAX_DIGITS - 1; i >= 0; i--)
        if (result->digits[i] != 0)
            break;
    result->length = i + 1;
}

void divide(const BigInteger *dividend, const BigInteger *divisor, BigInteger *quotient, BigInteger *remainder)
{
    // Initialize quotient and remainder to 0
    for (int i = 0; i < MAX_DIGITS; i++)
    {
        quotient->digits[i] = 0;
        remainder->digits[i] = 0;
    }
    quotient->length = 0;
    remainder->length = 0;
    // Find the most significant digit of the divisor
    int divisorMSD = 0;
    while (divisor->digits[divisorMSD] == 0 && divisorMSD < divisor->length)
        divisorMSD++;
    // Perform long division
    for (int i = dividend->length - 1; i >= 0; i--)
    {
        // Bring down the next digit of the dividend
        int currentDigit = dividend->digits[i];
        int partialDividend = remainder->digits[0] * 10 + currentDigit;
        // Determine the quotient digit
        int q = partialDividend / divisor->digits[divisorMSD];
        quotient->digits[i] = q;
        // Update the remainder
        int carry = 0;
        for (int j = divisorMSD; j >= 0; j--)
        {
            int product = q * divisor->digits[j] + carry;
            carry = product / 10;
            if (remainder->length <= i + j)
                remainder->digits[i + j] = 0;
            remainder->digits[i + j] = (remainder->digits[i + j] + product) % 10;
        }
    }
    // Find the length of the quotient
    int i;
    for (i = MAX_DIGITS - 1; i >= 0; i--)
        if (quotient->digits[i] != 0)
            break;
    quotient->length = i + 1;
    // Find the length of the remainder
    for (i = MAX_DIGITS - 1; i >= 0; i--)
        if (remainder->digits[i] != 0)
            break;
    remainder->length = i + 1;
}

int main()
{
    BigInteger num1, num2, result, quotient, remainder;
#if 0
    initialize(&num1, "999999999999999999999999999999999999999999999999");
    initialize(&num2, "1");
    add(&num1, &num2, &result);
    printBigInteger(&result);
#elif 0
    initialize(&num1, "25");
    initialize(&num2, "5");
    multiply(&num1, &num2, &result);
    printBigInteger(&result);
#else
    initialize(&num1, "2468888888888888888888888888888888888888");
    initialize(&num2, "2");
    divide(&num1, &num2, &quotient, &remainder);
    printf("Quotient: ");
    printBigInteger(&quotient);
    printf("Remainder: ");
    printBigInteger(&remainder);
#endif
    return 0;
}
