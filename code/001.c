
int putchar(char c)
{
  return write(1, &c, 1);
}


int putnbr(int n)
{
  int res = 0;
  if (n == -2147483648)
    return write(1, "-2147483648", 11);
  if (n < 0)
  {
    res += putchar('-');
    n = -n;
  }
  if (n > 9)
    res += putnbr(n / 10);
  res += putchar(n % 10 + '0');
  return res;
}

// int hello(char *str)
// {
//   return 1;
// }
// #include <stdio.h>
int main()
{
  // hello("abc");
  // puts("fffff");
  char *str = "Ffffff";
  int i = strlen(str);
  return i;
}