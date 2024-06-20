main():
    int x = 10
    int y = 7
    int z = 8
    if x == 15:
        output("if 0\n")
    elif x == 10:
        output("elif 1\n")
        if y == 7:
            output("1. if 0\n")
            if z < 10:
                output("2. if 0\n")
            else:
                output("3. else")
        else:
            output("1. else\n")
    elif x == 10:
        output("elif 2\n")
    elif x == 10:
        output("elif 3\n")
    elif x == 10:
        output("elif 4\n")
    elif x == 1:
        output("elif 5\n")
    else:
        output("else\n")
