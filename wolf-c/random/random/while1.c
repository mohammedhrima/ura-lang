#include <stdio.h>

int main()
{
    int i = 0;
    while (i < 20)
    {
        printf("%d\n", i);
        if(i == 5)
        {
            if (i % 5 == 0)
            {
                if (i == 5)
                    break;
            }
        }
        i++;
    }
}