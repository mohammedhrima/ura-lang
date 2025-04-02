#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>

size_t calculate_padding(size_t offset, size_t alignment)
{
    return (alignment - (offset % alignment)) % alignment;
}

int main()
{
    // Define the Id struct members
    const char *member_names[] = {"a (char*)", "b (int)", "c (char)"};
    size_t sizes[] = {8, 4, 1};      // sizeof(char*), sizeof(int), sizeof(char)
    size_t alignments[] = {8, 4, 1}; // alignof(char*), alignof(int), alignof(char)
    size_t n = sizeof(sizes) / sizeof(sizes[0]);

    // For a struct on x86-64, we typically start at offset 0 and add fields
    size_t current_offset = 0;
    size_t member_offsets[3];

    // Calculate offsets and paddings
    for (int i = 0; i < n; i++)
    {
        // Add padding to satisfy alignment
        size_t padding = calculate_padding(current_offset, alignments[i]);
        current_offset += padding;

        // Store this member's offset
        member_offsets[i] = current_offset;

        // Move offset past this member
        current_offset += sizes[i];
    }

    // Calculate total struct size (with padding at the end to satisfy struct alignment)
    // Struct alignment is typically the largest alignment of any member
    size_t max_alignment = 8; // For Id struct, this would be alignof(char*)
    size_t end_padding = calculate_padding(current_offset, max_alignment);
    size_t total_size = current_offset + end_padding;

    // Print the struct layout
    printf("Struct layout simulation:\n");
    for (int i = 0; i < n; i++)
    {
        printf("Member %s: offset %zu\n", member_names[i], member_offsets[i]);
    }
    printf("Total struct size: %zu bytes\n\n", total_size);

    // Now calculate stack offsets (negative from rbp)
    printf("Stack offsets simulation:\n");
    size_t stack_start = 16; // Starting point, aligned to 16
    for (int i = 0; i < n; i++)
    {
        printf("Variable %s: rbp-%zu\n", member_names[i], stack_start - member_offsets[i]);
    }

    return 0;
}