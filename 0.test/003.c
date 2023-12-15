#include <stdio.h>

#define GET_SIGN(x) ((x == 1) ? + : ((x == 2) ? - : 0))

int main() {
    int x = 1;

    int result = x GET_SIGN(1) 2;

    printf("Result: %d\n", result);

    return 0;
}