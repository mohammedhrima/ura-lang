func int test(int x):
    x = 2
    output("x in test: ", x, "\n")

main():
    int x = 1
    test(x)
    output("x in main: ", x, "\n")
    ref int y = x
    y = 7
    output("x in main (after ref): ", x, "\n")

