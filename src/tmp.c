#include <stdio.h>

int main() {
    // char str[255] = {};
    // printf("%d\n", 001);
    // printf("%d\n", 008);
    // printf("%d\n", 016);
    // printf("%d\n", 032);
    int a = 1;

    while (a < 255) {
        printf("%d\n", a);
        a <<= 1;
    }
}