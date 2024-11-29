#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Custom __stack_chk_fail function
void __stack_chk_fail(void)
{
    void *caller = __builtin_return_address(0);
    fprintf(stderr, "Error: Stack smashing detected! Custom error message.\n");
    fprintf(stderr, "Potential overflow detected near the return address: %p\n", caller);
    abort();
}

int main()
{
    char buffer[10];

    // Intentionally causing a buffer overflow
    strcpy(buffer, "This is a very long string that will overflow the buffer");

    return 0;
}
