#include <stdio.h>
#include <stddef.h>

typedef struct {
   char *name;
   int size;
   int align;
   char *type;
} Simulator;

int calculate_padding(int offset, int alignment) {
   return (alignment - (offset % alignment)) % alignment;
}

void print_struct_layout(Simulator *sims, int size) {
   int offset = 0;
   int max_align = 1;

   // First pass: calculate member offsets and max alignment
   printf("Member layout:\n");
   for (int i = 0; i < size; i++) {
      int padding = calculate_padding(offset, sims[i].align);
      offset += padding;

      printf("%s (%s): offset %d, size %d, align %d",
             sims[i].name, sims[i].type, offset, sims[i].size, sims[i].align);

      if (padding > 0) {
         printf(" (padding %d bytes)", padding);
      }
      printf("\n");

      offset += sims[i].size;
      if (sims[i].align > max_align) {
         max_align = sims[i].align;
      }
   }

   // Calculate final padding
   int final_padding = calculate_padding(offset, max_align);
   int total_size = offset + final_padding;

   printf("\nTotal struct size: %d bytes\n", total_size);
   printf("Struct alignment requirement: %d bytes\n", max_align);

   // Memory map visualization
   printf("\nMemory layout:\n");
   offset = 0;
   for (int i = 0; i < size; i++) {
      int padding = calculate_padding(offset, sims[i].align);
      if (padding > 0) {
         printf("%d: [padding %d bytes]\n", offset, padding);
         offset += padding;
      }
      printf("%d: %s (%s) - %d bytes\n",
             offset, sims[i].name, sims[i].type, sims[i].size);
      offset += sims[i].size;
   }
   if (final_padding > 0) {
      printf("%d: [final padding %d bytes]\n", offset, final_padding);
   }
}

#define macro(t) sizeof(t), _Alignof(t), #t

int main() {
   Simulator sims[] = {
      {"a", macro(long)},
      {"b", macro(int)},
      {"c", macro(char)},
   };

   print_struct_layout(sims, sizeof(sims) / sizeof(sims[0]));
   return 0;
}