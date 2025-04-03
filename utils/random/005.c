#include <stdio.h>
#include <stddef.h>

typedef struct
{
    char *name;
    int size;
    int align;
    char *type;
} Simulator;

int calculate_padding(int offset, int alignment)
{
    return (alignment - (offset % alignment)) % alignment;
}

void print_struct_layout(Simulator *sims, int size)
{

    int offsets[size];
    int offset = 0;

    for (int i = 0; i < size; i++)
    {
        int padding = calculate_padding(offset, sims[i].align);
        offset += padding;
        offsets[i] = offset;
        offset += sims[i].size;
    }

    int max_align = 1;
    for (int i = 0; i < size; i++)
        if (sims[i].align > max_align)
            max_align = sims[i].align;

    int final_padding = calculate_padding(offset, max_align);
    int total_size = offset + final_padding;

    printf("Total struct size: %d bytes \n", total_size);

    int stack_alignment = 4;

    int stack_space = ((total_size + stack_alignment - 1) / stack_alignment) * stack_alignment;
    int base_rbp_offset = -stack_space;

    for (int i = 0; i < size; i++)
    {
        int rbp_offset = base_rbp_offset + offsets[i];
        printf("  %s (%s):  rbp %d\n", sims[i].name, sims[i].type, rbp_offset);
    }
}

#define macro(t) sizeof(t), _Alignof(t), #t
int main()
{
    // int s = 0;
    // struct {long a0; int a1; char a2;} a;
    // printf("%d\n", (void*)&s - (void*)&(a.a0) + sizeof(s));
    // printf("%d\n", (void*)&s - (void*)&(a.a1) + sizeof(s));
    // printf("%d\n", (void*)&s - (void*)&(a.a2) + sizeof(s));

    Simulator sims[] = {
        // (Simulator){"", macro(int)},
        (Simulator){"a", macro(long)},
        (Simulator){"b", macro(int)},
        (Simulator){"c", macro(char)},
    };
    print_struct_layout(sims, sizeof(sims) / sizeof(sims[0]));
    return 0;
}