#include <stdio.h>

typedef struct User {
    int a;
    char b;
    int c;
} User;

int main()
{
    User user = (User) {.a = 1, .b = 2, .c = 3};

    User *ptr = &user;
    // ptr->a = 11;
    // ptr->b = 22;
    // ptr->c = 33;
}