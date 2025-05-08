typedef struct {
   int a;
   int b;
   int c;
   int d;
} User;

void func(User *user)
{
   user->a = 1;
   user->b = 2;
   user->c = 3;
   user->d = 4;
}

int main()
{
   User user = {.a = 1, .b = 2, .c = 3, .d = 4};
   func(&user);
}