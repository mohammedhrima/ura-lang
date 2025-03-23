#include <stdio.h>
#include <stddef.h>

#if 0
// Define the struct with the given data types
struct MyStruct {
    char c;       // char
    int i;        // int
    char *p;      // char *
    short s;      // short
};

int main() {
    // Print the offset of each member in the struct
    printf("Offset of char c: %zu\n", offsetof(struct MyStruct, c));
    printf("Offset of int i: %zu\n", offsetof(struct MyStruct, i));
    printf("Offset of char *p: %zu\n", offsetof(struct MyStruct, p));
    printf("Offset of short s: %zu\n", offsetof(struct MyStruct, s));

    return 0;
}
#else
size_t calculate_padding(size_t offset, size_t alignment) {
    // Calculate the amount of padding needed to align the offset
    size_t padding = (alignment - (offset % alignment)) % alignment;
    return padding;
}

int main() {
    size_t arr[] = {sizeof(char), sizeof(int), sizeof(char *), sizeof(short)};
    size_t n = sizeof(arr) / sizeof(arr[0]); // Number of elements in the array

    size_t offset = 0; // Start with offset 0

    for (size_t i = 0; i < n; i++) {
        size_t size = arr[i]; // Size of the current data type
        size_t alignment = size; // Alignment is typically equal to the size of the data type

        // Calculate padding required to align the current offset
        size_t padding = calculate_padding(offset, alignment);

        // Add padding to the offset
        offset += padding;

        // Print the offset for the current data type
        printf("Offset for element %zu (size %zu): %zu\n", i, size, offset);

        // Update the offset for the next element
        offset += size;
    }

    return 0;
}

#endif