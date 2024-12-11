#include <unistd.h>

void putnbr(int n)
{
    if(n < 9)
    {
        char c = '0' + n;
        write(1, &c, 1);
    }
    else
    {
        putnbr(n / 10);
        putnbr(n % 10);
    }
}

int main()
{
    int i = 0;
    while(i < 5)
    {
        i = i + 1;
        putnbr(i);
        write(1, " ", 1);
    }
    return i;
}