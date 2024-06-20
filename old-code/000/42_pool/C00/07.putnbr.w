func void putnbr(int n):
    if n < 0:
        output("-")
        n = -n
    if n < 10:
        output(n)
    else:
        putnbr(n / 10)
        putnbr(n % 10)

main():
    putnbr(-51515)
    output("\n")