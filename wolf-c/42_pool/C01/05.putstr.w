func void putstr(ref chars str):
    int i
    while str[i] != '\0':
        output(str[i])
        i += 1

main():
    chars str = "abcd"
    putstr(str)
    output("\n")
