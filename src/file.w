
struct Id:
    chars name

struct User:
    // Id id
    int age

main():
    User user
    user.age = 123
    // user.id.name = "abc"
    putnbr(user.age)