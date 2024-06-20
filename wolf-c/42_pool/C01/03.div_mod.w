func void divmod(int a, int b, ref int div, ref int mod):
    div = a / b
    mod = a % b

main():
    int x = 20
    int y = 3
    int d
    int m
    divmod(x, y, d, m)
    output(x, " / ", y, " = ", d, "\n")
    output(x, " % ", y, " = ", m, "\n")
