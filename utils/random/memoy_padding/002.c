#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define s(e) sizeof(e)

typedef struct
{
    int size;
    int ptr;
    int offset;
    int align;
} Data_type;

int size = 0;
int pos = 0;
Data_type **arr;

Data_type *new_data_type(int size)
{
    Data_type *new = calloc(1, sizeof(Data_type));
    new->size = size;
    new->align = size;

    if (arr == NULL)
    {
        size = 10;
        arr = calloc(size, sizeof(Data_type *));
    }
    else if (pos + 1 == size)
    {
        Data_type **tmp = calloc(size * 2, sizeof(Data_type *));
        memcpy(tmp, arr, pos * sizeof(Data_type *));
        free(arr);
        arr = tmp;
        size *= 2;
    }

    arr[pos++] = new;
    return new;
}

void free_data_types()
{
    for (int i = 0; i < pos; i++)
        free(arr[i]);
    free(arr);
    arr = NULL;
    pos = 0;
    size = 0;
}

void log_data_types(int j, int struct_offset)
{
    printf("============== struct %d ================\n", j);
    printf("struct total size: %d\n", struct_offset);
    for (int i = 0; i < pos; i++)
    {
        Data_type *c = arr[i];
        printf("member[%d]: size = %d, offset = %d, align = %d, ptr = %d\n", i, c->size, c->offset, c->align, c->ptr);
    }
    printf("=========================================\n\n");
}

void create_struct(int j)
{
    switch (j)
    {
    case 0:
        new_data_type(s(int));
        new_data_type(s(char));
        new_data_type(s(int));
        new_data_type(s(int));
        break;
    case 1:
        new_data_type(s(char));
        new_data_type(s(char));
        new_data_type(s(int));
        new_data_type(s(char));
        break;
    case 2:
        new_data_type(s(long));
        new_data_type(s(char));
        new_data_type(s(char));
        new_data_type(s(int));
        break;
    case 3:
        new_data_type(s(long));
        new_data_type(s(int));
        break;
    }
}

int main()
{
    int ptr = 0;

    for (int j = 0; j < 4; j++)
    {
        create_struct(j);

        int offset = 0;
        int max_align = 0;

        for (int i = 0; i < pos; i++)
        {
            Data_type *c = arr[i];
            int align = c->align;

            int padding = (align - (offset % align)) % align;
            offset += padding;
            c->offset = offset;
            offset += c->size;

            if (align > max_align)
                max_align = align;
        }
        int final_padding = (max_align - (offset % max_align)) % max_align;
        int struct_offset = offset + final_padding;

        for (int i = 0; i < pos; i++)
        {
            Data_type *c = arr[i];
            c->ptr = ptr + struct_offset - c->offset;
        }

        log_data_types(j, struct_offset);
        ptr += struct_offset;
        free_data_types();
    }

    return 0;
}
