func void printComb():
    int a = 0
    while a < 10:
        int b = a + 1
        while b < 10:
            int c = b + 1
            while c < 10:
                output(a, b, c," ")
                c += 1
            b += 1
        a += 1
    output("\n")

main():
    printComb()