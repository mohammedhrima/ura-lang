func void swap(ref int a, ref int b):
    int t = a
    a = b
    b = t

main():
    int x = 10
    int y = 5
    output("x: ",x, " y: ", y, "\n")
    swap(x, y)
    output("x: ",x, " y: ", y, "\n")
