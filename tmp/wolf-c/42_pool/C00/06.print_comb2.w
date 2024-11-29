func void makeComb(int x, int y):
    output(x/10, x%10, " ", y/10, y%10)
    if x != 98 or y != 99:
        output(", ")

main():
    int x = 0
    while x <= 99:
        int y = x + 1
        while y <= 99:
            makeComb(x,y)
            y += 1
        x += 1
    output("\n")