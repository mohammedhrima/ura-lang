#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void putstr(const char *str)
{
   write(1, str, strlen(str));
}

void putnbr(int n)
{
   if (n == -2147483648)
   {
      putstr("-2147483648");
      return;
   }
   if (n < 0)
   {
      putchar('-');
      n = -n;
   }
   if (n > 9) putnbr(n / 10);
   putchar((n % 10) + '0');
}

void putfloat(float f)
{
   if (f < 0)
   {
      putchar('-');
      f = -f;
   }

   int int_part = (int)f;
   putnbr(int_part);
   putchar('.');

   f -= int_part;

   for (int i = 0; i < 5; i++)
   {
      f *= 10;
      putnbr((int)f);
      f -= (int)f;
   }
}

void putbool(int b)
{
   if (b) putstr("True");
   else putstr("False");
}

char *strjoin(const char *s1, const char *s2)
{
   if(!s1) return s2;
   if(!s2) return s1;

   char *result = calloc(strlen(s1) + strlen(s2) + 1, 1);
   if (!result) return NULL;
   strcpy(result, s1);
   strcat(result, s2);
   return result;
}