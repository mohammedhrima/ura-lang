#include <stdio.h>

typedef struct User {
   int d;
   int c;
   char b;
   int a;
} User;

int main()
{
   User user = (User) {.a = 1, .b = 2, .c = 3, .d = 4};
   User user2 = (User) {.a = 1, .b = 2, .c = 3, .d = 4};


   // User *ptr = &user;
   // ptr->a = 11;
   // ptr->b = 22;
   // ptr->c = 33;
}
