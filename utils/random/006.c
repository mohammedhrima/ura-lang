#include <stdio.h>

// Define the User struct
typedef struct User {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int o;
} User;

// Function to print an integer
void putnbr(int n) {
    printf("%d\n", n);
}

int main() {
    // Initialize the struct (all members are set to 0 by default)
    User obj = {};

    // Print all members of the struct
    putnbr(obj.a);
    putnbr(obj.b);
    putnbr(obj.c);
    putnbr(obj.d);
    putnbr(obj.e);
    putnbr(obj.f);
    putnbr(obj.g);
    putnbr(obj.h);
    putnbr(obj.i);
    putnbr(obj.j);
    putnbr(obj.k);
    putnbr(obj.l);
    putnbr(obj.m);
    putnbr(obj.n);
    putnbr(obj.o);

    return 0;
}