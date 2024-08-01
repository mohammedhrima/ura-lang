#include <stdio.h>

#if 0
void printTwentyIntegers(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, 
                        int a9, int a10, int a11, int a12, int a13, int a14, int a15, 
                        int a16, int a17, int a18, int a19, int a20) {
    printf("The 20 integers are:\n");
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
           a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, 
           a16, a17, a18, a19, a20);
}

int main() {
    // Example usage of the function
    printTwentyIntegers(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
    return 0;
}
#endif

void print(int *ptr)
{
    printf("%d", *ptr);
}

int main()
{
    int a = 10;
    int *ptr = &a;
    print(ptr);
}