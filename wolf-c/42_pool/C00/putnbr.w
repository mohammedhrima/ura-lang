func void putnbr(int n):
    if n < 0:
        output("-")
        n = -n
    if n < 10:
        output(n)
    else:
        output(n / 10)
        output(n % 10)

main():
    putnbr(5)
    output("\n")