#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __stack_chk_fail(void) {
    fprintf(stderr, "Error: Stack smashing detected! Custom error message.\n");
    abort();  // Terminates the program
}

int main() {
    char buffer[10];
    strcpy(buffer, "This is a very long string that will overflow the buffer");

    return 0;
}
