typedef struct {
   int a;
   int b;
   int c;
   int d;
} User;

void func(User *user)
{

}

int main()
{
   User user = {.a = 1, .b = 2, .c = 3, .d = 4};
   func(&user);
}