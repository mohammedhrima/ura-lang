typedef struct {
    int a;
    char b;
    int c;
    int d;
} User;

void func(User p)
{
    p = (User){
        .a = 1,
        .b = 2,
        .c = 3,
        .d = 4,
    };
}

int main() {
    User user = (User){
        .a = 1,
        .b = 2,
        .c = 3,
        .d = 4,
    };
    func(user);
}