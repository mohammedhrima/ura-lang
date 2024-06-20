
func void rev_int_tab(ref array[int] arr, int size):
    int i
    while i < size / 2:
        int tmp = arr[i]
        arr[i] = arr[size - i - 1]
        arr[size - i - 1] = tmp
        i += 1

main():
    array[int] arr = [11,22,33,44,55]
    int size = 5
    rev_int_tab(arr, size)

    int j
    while j < size :
        output(arr[j], " ")
        j += 1
    output("\n")

    
