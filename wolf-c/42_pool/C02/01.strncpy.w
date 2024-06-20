func void strncpy(ref chars src, ref chars dest, int n):
    int i
    while src[i] != '\0' and i < n:
        dest[i] = src[i]
        i += 1

main():
    chars dest = "abcdef"
    chars src = "123"
    strncpy(src, dest, 2)
    output("src: ", src, "\ndest: ", dest, "\n")

  

