struct Infos:
   chars name

struct Info:
   chars name

struct User:
   int a
   int b
   Info info

func void m(User user):
   user.a = 123

main():
   User user
   m(user)