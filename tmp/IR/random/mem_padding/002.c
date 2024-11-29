#include <stdio.h>

#define s(p) sizeof(p)
#define ALIGNMENT 8

int main()
{
    size_t arr[] = {4, 1, 1, 1, 1, 4, 4};  // Sizes of elements
    size_t size = sizeof(arr) / sizeof(size_t);  // Number of elements in the array
    size_t res[size];  // Array to store the aligned offsets

    size_t i = 0;
    size_t ptr = 0;  // This pointer simulates the start address of the current element

    while (i < size)
    {
        // Check if the current element will cross the 8-byte boundary
        if (ptr % ALIGNMENT + arr[i] > ALIGNMENT)
            ptr += ALIGNMENT - (ptr % ALIGNMENT);
        res[i] = ptr;  // Store the aligned address
        ptr += arr[i]; // Move the pointer by the size of the current element
        i++;
    }

    // Print the original sizes
    i = 0;
    while (i < size)
        printf("%zu ", arr[i++]);
    printf("\n");

    // Print the padded addresses
    i = 0;
    while (i < size)
        printf("%zu ", res[i++]);
    printf("\n");

    return 0;
}
