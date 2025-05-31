struct Infos:
   chars name

struct Info:
   chars name
   Infos i

struct User:
   int a
   int b
   Info info

func void m(User user):
   User user1