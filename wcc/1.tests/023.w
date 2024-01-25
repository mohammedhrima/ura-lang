func int test4(ref int z):
    z = 4

func int test3(ref int z):
    z = 3
    test4(z)

func int test2(ref int y):
    y = 2
    test3(y)

main():
    int x = 1
    test2(x)
    output("in main: " ,x, "\n")

