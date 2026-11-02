/* ************************************************************************** */
/*                                                                            */
/*    _   _ ____      _        header.h                                       */
/*   | | | |  _ \    / \                                                      */
/*   | | | | |_) |  / _ \      Mohammed Hrima                                 */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com                       */
/*    \___/|_| \_\/_/   \_\    created: 2023/03/11 03:08:09                   */
/*                                                                            */
/*   updated: 2026/10/21 00:05:32                                             */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>

#ifndef bool
#    define bool  int
#    define true  1
#    define false 0
#endif

#ifndef vfprint
#    define vfprint vfprintf
#endif

#ifndef print
#    define print(...) fprintf(stdout, __VA_ARGS__)
#endif

#ifndef eprint
#    define eprint(...) fprintf(stderr, __VA_ARGS__)
#endif

#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
#define DIM(fmt)   "\033[2m" fmt RESET

#define FILE       __FILE__
#define LINE       __LINE__
#define FUNC       __func__
#define TAB        4

// clang-format off
#ifndef expand
#	define expand(T, e) T *e; size_t e##_count; size_t e##_size;
#endif
#ifndef zero_init
#define zero_init(e) e = NULL; e##_count = 0; e##_size = 0;
#endif

#ifndef append
#define append(p, c)                                          \
{                                                             \
	if (p##_size == 0)                                        \
		p = arena_alloc((p##_size = 10), sizeof(*p));         \
	else if (p##_count + 2 == p##_size) {                     \
		void *tmp = arena_alloc((p##_size *= 2), sizeof(*p)); \
		memcpy(tmp, p, (p##_count * sizeof(*p)));             \
		p = tmp;                                              \
	}                                                         \
	p[p##_count++] = c;                                       \
}
#endif
// clang-format on

#if defined(__APPLE__)
#    include <mach-o/dyld.h>
#elif defined(__linux__)
typedef struct _IO_FILE *ioFile;
#endif

typedef struct Arena Arena;
struct Arena {
    unsigned char *buff;
    size_t size;
    size_t used;
    Arena *next;
};

static Arena *arena_head;
static Arena *arena_curr;
static size_t heap_size;

Arena *new_arena(size_t size) {
    Arena *new = calloc(1, sizeof(Arena));
    if (new == NULL) {
        fprintf(stderr, "%s:%d calloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    new->buff = calloc(size, 1);
    if (new->buff == NULL) {
        fprintf(stderr, "%s:%d calloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    new->size = size;
    heap_size += (sizeof(Arena) + size);
    return new;
}

void arena_reserve(size_t size) {
    Arena *curr = arena_curr;
    Arena *head = arena_head;
    if (curr != NULL && curr->size - curr->used >= size)
        return;

    Arena *new = new_arena((size + 16 - 1) / 16 * 16);
    if (head == NULL)
        curr = head = new;
    else
        curr = curr->next = new;

    arena_curr = curr;
    arena_head = head;
}

void *arena_alloc(size_t asked_count, size_t asked_size) {
    size_t asked = asked_count * asked_size;
    asked = (asked + 16 - 1) / 16 * 16;

    Arena *head = arena_head;
    Arena *curr = arena_curr;
    if (head == NULL || curr->used + asked >= curr->size) {
        size_t size = curr != NULL ? (curr->size << 1) : (1 << 8);
        while (size < asked)
            size <<= 1;
        Arena *new = new_arena(size);

        if (head == NULL)
            curr = head = new;
        else
            curr = curr->next = new;
    }
    void *res = curr->buff + curr->used;
    curr->used += asked;

    arena_head = head;
    arena_curr = curr;

    return res;
}

void arena_free() {
    Arena *arena = arena_head;
    while (arena != NULL) {
        Arena *next = arena->next;
        free(arena->buff);
        free(arena);
        arena = next;
    }
    arena_head = NULL;
    arena_curr = NULL;
}

size_t arena_size() {
    return heap_size;
}

char *strdup2(const char *str) {
    char *res = arena_alloc(strlen(str) + 1, sizeof(char));
    strcpy(res, str);
    return res;
}

char *strndup2(const char *str, size_t n) {
    char *res = arena_alloc(n + 1, sizeof(char));
    strncpy(res, str, n);
    return res;
}

bool strcmp2(const char *left, const char *right) {
    return strcmp(left, right) == 0;
}

bool strncmp2(const char *left, const char *right, size_t n) {
    return strncmp(left, right, n) == 0;
}

bool strchr2(char *str, int c) {
    char *res = strchr(str, c);
    return c != '\0' && res != NULL && res[0] != '\0';
}

char *getcwd2(void) {
    char *path = getcwd(NULL, 0);
    if (path == NULL) {
        fprintf(stderr, "%s:%d getcwd failed\n", FILE, LINE);
        exit(1);
    }
    char *res = strdup2(path);
    free(path);
    return res;
}

char *dirname2(char *path) {
    return dirname(strdup2(path));
}

void mkdir2(const char *path) {
    if (mkdir(path, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "%s:%d mkdir failed\n", FILE, LINE);
        exit(1);
    }
}

char *format(const char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    ioFile out = open_memstream(&buf, &size);
    if (out == NULL) {
        fprintf(stderr, "%s:%d open_memstream failed\n", FILE, LINE);
        exit(1);
    }

    va_list ap;
    va_start(ap, fmt);
    vfprint(out, fmt, ap);
    va_end(ap);

    fclose(out);
    char *res = strdup2(buf);
    free(buf);
    return res;
}

typedef struct File File;
struct File {
    // char *name;
    char *path;
    // TODO:
    // some attributes might need to be removed
    // char *dir;
    char *buff;
    int len;
    bool opened;
};

File *new_file(char *path) {
    File *new = arena_alloc(1, sizeof(File));
    new->path = path;
    return new;
    // new->name =
}

void open_file(File *file) {
    char *path = file->path;
    if (realpath(path, NULL) == NULL) {
        fprintf(stderr, "realpath failed (%s)\n", file->path);
        exit(1);
    }
    file->path = path;
    ioFile fp = fopen(file->path, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen failed (%s)\n", file->path);
        exit(1);
    }
    if (fseek(fp, sizeof(char), SEEK_END) != 0) {
        fprintf(stderr, "fseek failed\n");
        exit(1);
    }
    file->len = ftell(fp);
    rewind(fp);
    file->buff = arena_alloc(file->len + 1, sizeof(char));
    fread(file->buff, file->len, sizeof(char), fp);
    fclose(fp);
    file->opened = true;
}

void close_file(File *file) {
    file->opened = false;
}