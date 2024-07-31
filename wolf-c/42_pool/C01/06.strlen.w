func int strlen(ref chars str):
    int i
    while str[i] != '\0':
        i += 1
    return i

main():
    chars  str = "123"
    int len = strlen(str)
    output("len: ", len, "\n")
    
    
