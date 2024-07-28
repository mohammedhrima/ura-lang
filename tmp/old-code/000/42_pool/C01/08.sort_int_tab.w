
func void swap(ref int left, ref int right):
    int tmp = right
    right = left
    left = tmp

func void sort_int_tab(ref array[int] arr, int size):
    int i
    while i < size:
        int j = i
        while j < size:
            if arr[j] < arr[i]:
                swap(arr[j], arr[i])
            j += 1
        i += 1

main():
    array[int] arr = [9,12,1,13,5,8,1]
    int len = 7
    sort_int_tab(arr, len)
    int i
    while i < len:
        output(arr[i], " ")
        i += 1
    output("\n")

