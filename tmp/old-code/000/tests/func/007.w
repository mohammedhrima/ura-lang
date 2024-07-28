func int test(ref int y):
    y = 2
    output("in test: ", y, "\n")

main():
    int x = 1
    test(x)
    output("in main: " ,x, "\n")
