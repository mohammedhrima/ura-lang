struct Data:
    chars name
    int a

struct User:
    Data data
    char e
    int b
    char c

main():
    User user1
    user1.data.a = 10
    User user2
    user2.data.a = 11