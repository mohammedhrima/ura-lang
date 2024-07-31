func int test1(ref int y):
    y = 10
    output("in test1: ", y, "\n")

func int test2(ref int y):
    y = 20
    output("in test2: ", y, "\n")

main():
    int x = 1
    test1(x)
    output("in main: " ,x, "\n")
    test2(x)
    output("in main: " ,x, "\n")

