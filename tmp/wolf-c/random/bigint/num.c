#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *add(char *string1, char *string2)
{
    int len1 = (int)strlen(string1);
    int len2 = (int)strlen(string2);
    int len = len1 > len2 ? len1 : len2;
    int i = 0;
    char *res = calloc(len + 2, sizeof(char));
    // printf("add:\n   %s has len %d\n   %s has len %d\n", string1, len1, string2, len2);
    int rest = 0;
    while (len1 > 0 && len2 > 0)
    {
        int a = string1[len1 - 1] + string2[len2 - 1] - 2 * '0' + rest;
        if (a >= 10)
        {
            rest = a / 10;
            a = a % 10;
        }
        else
            rest = 0;
        // printf("1. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len1--;
        len2--;
    }
    while (len1 > 0)
    {
        int a = string1[len1 - 1] - '0' + rest;
        if (a >= 10)
        {
            rest = a / 10;
            a = a % 10;
        }
        else
            rest = 0;
        // printf("2. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len1--;
    }
    while (len2 > 0)
    {
        int a = string2[len2 - 1] - '0' + rest;
        if (a >= 10)
        {
            rest = a / 10;
            a = a % 10;
        }
        else
            rest = 0;
        // printf("3. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len2--;
    }
    if (rest)
    {
        res[i++] = rest + '0';
        // printf("4. > %d > %c\n", rest, rest + '0');
    }
    i = 0;
    len = (int)strlen(res);
    while (i < len / 2)
    {
        char tmp = res[i];
        res[i] = res[len - i - 1];
        res[len - i - 1] = tmp;
        i++;
    }
    return res;
}

char *sub(char *string1, char *string2)
{
    int len1 = (int)strlen(string1);
    int len2 = (int)strlen(string2);
    int len = len1 > len2 ? len1 : len2;

    printf("%s - %s\n\n", string1, string2);
    int i = 0;
    char *tmp = calloc(len + 1, sizeof(char));
    while (len1 - i)
    {
        tmp[len - i - 1] = string1[len1 - i - 1];
        i++;
    }
    while (len - i)
    {
        tmp[len - i - 1] = '0';
        i++;
    }
    string1 = tmp;
    i = 0;
    tmp = calloc(len + 1, sizeof(char));
    while (len2 - i)
    {
        tmp[len - i - 1] = string2[len2 - i - 1];
        i++;
    }
    while (len - i)
    {
        tmp[len - i - 1] = '0';
        i++;
    }
    string2 = tmp;
    printf("string1: %s\nstring2: %s\n\n", string1, string2);
    // exit(0);

    // char *tmp = calloc(len, sizeof(char));
    // printf("add:\n   %s has len %d\n   %s has len %d\n", string1, len1, string2, len2);
    i = 0;
    char *res = calloc(len + 2, sizeof(char));
    int rest = 0;
    while (len1 > 0 && len2 > 0)
    {
        int a = string1[len1 - 1] - '0' - (string2[len2 - 1] - '0') - rest;
        if (a < 0)
        {
            a += 10;
            rest = 1;
        }
        else
            rest = 0;
        printf("1. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len1--;
        len2--;
    }
    while (len1 > 0)
    {
        int a = string1[len1 - 1] - '0' - rest;
        if (a < 0)
        {
            a += 10;
            rest = 1;
        }
        else
            rest = 0;
        printf("2. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len1--;
    }
    while (len2 > 0)
    {
        int a = string2[len2 - 1] - '0' - rest;
        if (a < 0)
        {
            a += 10;
            rest = 1;
        }
        else
            rest = 0;
        printf("3. > %d > %c\n", a, a + '0');
        res[i++] = a + '0';
        len2--;
    }
    if (rest)
    {
        res[i++] = '-';
        printf("4. > %d > %c\n", rest, rest + '0');
    }
    i = 0;
    len = (int)strlen(res);
    while (i < len / 2)
    {
        char tmp = res[i];
        res[i] = res[len - i - 1];
        res[len - i - 1] = tmp;
        i++;
    }
    free(string1);
    free(string2);
    return res;
}

int main()
{
    char *num1 = "3";
    char *num2 = "10000";
    char *res = sub(num1, num2);
    printf("%s - %s = %s\n", num1, num2, res);
    free(res);
}