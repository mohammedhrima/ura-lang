func void divmod(ref int a, ref int b):
    int div = a / b
    b = a % b
    a = div

main():
    int x = 20
    int y = 3
    divmod(x, y)
    output("x: ",x, " y: ", y, "\n")
