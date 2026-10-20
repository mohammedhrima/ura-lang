// INCLUDES
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// MACROS
// clang-format off
#define expand(T, e) T *e; size_t e##_count; size_t e##_size;
// clang-format on

// #define push_back(p, c) { \
//  if(p##_size == 0)
//     p =
// }
#ifndef print
#    define print(...) printf(__VA_ARGS__)
#endif
#ifndef eprint
#    define eprint(...) printf(__VA_ARGS__)
#endif
#ifndef help
#    define help(...) printf(__VA_ARGS__)
#endif

#if defined(__APPLE__)
#    include <mach-o/dyld.h>
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

// STRUCTS
typedef struct Arena Arena;
struct Arena {
    unsigned char *buff;
    size_t size;
    size_t used;
    Arena *next;
};

// PROTOS
#include "proto.h"

/*
TODO:
+ create main function
*/


int main() {
}

// UTILS



static Arena *head;
void *arena_alloc(size_t asked_count, size_t asked_size) {
    if(head == NULL) {
        
        head = new_arena()
    }
    return NULL;
}

Arena *new_arena(size_t size) {
    Arena *new = calloc(1, sizeof(Arena));
    if (new == NULL) {
        eprint("calloc failed\n");
        exit(1);
    }
    new->buff = calloc(size, 1);
    if (new->buff == NULL) {
        eprint("calloc failed\n");
        exit(1);
    }
    new->size = size;

    return new;
}