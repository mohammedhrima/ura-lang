typedef struct
{
    char *a;
    int b;
    char c;
} Id;

typedef struct 
{
    Id id;
    int age;
} User;


int main()
{
    // User user = {0};
    Id i = {0};
    i.a = (char*)125;
    i.b = 12;
    i.c = 13;
}
