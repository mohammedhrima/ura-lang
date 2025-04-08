#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define s(e) sizeof(e)

int main()
{
    size_t *all[] = {
        {s(int), s(char), s(int), s(int)},
        {s(char), s(char), s(int), s(char)},
        {s(long), s(char), s(char), s(int)},
        {s(long), s(int)},
    };

    for(int i = 0; i < 4; i++)
    {
        size_t sizes[] = all[i];
        int ptrs[s(sizes) / s(sizes[0])] = {0};
        int offsets[s(sizes) / s(sizes[0])] = {0};
    
        int offset = 0;
        int align = 0;
        for (int i = 0; i < s(sizes) / s(sizes[0]); i++)
        {
            int size = sizes[i];
            offset = (offset + size - 1) / size * size;
            offsets[i] = offset;
            offset += size;
            if (size > align)
                align = size;
        }
    
        // struct offset
        int ptr = 0;
        int struct_offset = 0;
    
        struct_offset += ptr + (offset + align - 1) / align * align;
        for (int i = 0; i < s(sizes) / s(sizes[0]); i++)
        {
            ptrs[i] = struct_offset - (ptr + offsets[i]);
        }
        for (int i = 0; i < s(sizes) / s(sizes[0]); i++)
        {
            printf("ptr [%d], offset [%d]\n", ptrs[i], offsets[i]);
        }
        ptr += offset;
        printf("============================================\n");
    }

}
