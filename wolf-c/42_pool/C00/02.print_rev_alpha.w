func void printAlpha():
    char c = 'z'
    while c >= 'a':
        output(c, " ")
        c -= 1
    output("\n")

main():
    printAlpha()