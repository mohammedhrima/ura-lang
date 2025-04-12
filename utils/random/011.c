typedef struct {
    int a;
    char b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
} User;
#if 0
void func(User p)
{
    p = (User){
        .a = 1,
        .b = 2,
        .c = 3,
        .d = 4,
    };
}
#else
void func(User p)
{
    p = (User){
        .a = 1,
        .b = 2,
        .c = 3,
        // .d = 4,
        // .e = 5,
        // .f = 7,
        // .g = 8,
        // .h = 9
    };
}

#endif
int main() {
    User user = (User){
        .a = 1,
        .b = 2,
        .c = 3,
        //.d = 4,
        //.e = 5,
        //.f = 6,
        //.g = 7,
        //.h = 8,
    };
    func(user);
}