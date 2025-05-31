// struct Infos:
//    chars name
typedef char *chars;

struct Info
{
	chars name;
};

struct User
{
	struct Info info;
	// chars name;
	int a;
	int b;
};

// func void m(User user):

int main()
{
	struct User user0 = {.info.name = ""};
	struct User user1 = {.info.name= "a"};
}