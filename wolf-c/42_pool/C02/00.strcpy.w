func void strcpy(ref chars src,ref chars dest):
    int i
    while src[i] != '\0':
        dest[i] = src[i]
        i += 1

main():
    chars dest = "abcdef"
    chars src = "123"
    strcpy(src, dest)
    output("src:  ", src, "\ndest: ", dest, "\n")

  

