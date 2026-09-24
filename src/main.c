// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // dirname
#include <stdbool.h>

// TYPEDEFS
typedef struct Arena Arena;
typedef struct Ura Ura;
typedef struct uraFile uraFile;
typedef struct Token Token;
typedef struct Node Node;
typedef enum Type Type;

// MACROS
#define FILE       __FILE__
#define LINE       __LINE__
#define FUNC       __func__
#define TAB        4

#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
#define DIM(fmt)   "\033[2m" fmt RESET

#define expand(type, name) \
    type *name;            \
    size_t name##_count;   \
    size_t name##_size;

#define push_back(parent, child)                                          \
    {                                                                     \
        if (parent##_size == 0)                                           \
            parent = ura_alloc((parent##_size = 10), sizeof(*parent));    \
        else if (parent##_count + 1 == parent##_size) {                   \
            void *tmp = ura_alloc((parent##_size *= 2), sizeof(*parent)); \
            memcpy(tmp, parent, (parent##_count * sizeof(*parent)));      \
            parent = tmp;                                                 \
        }                                                                 \
        parent[parent##_count++] = child;                                 \
    }
#define eprint(...) _eprint(FILE, FUNC, LINE, __VA_ARGS__)
#define help(...)   printf(__VA_ARGS__)

#ifndef bool
#    define bool  int
#    define true  1
#    define false 0
#endif

#if defined(__APPLE__)
#    include <mach-o/dyld.h>
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif


// PROTOTYPES
void ura_free();
uraFile *new_file(char *name);
bool ura_strcmp(char *left, char *right);
bool ura_strncmp(char *left, char *right, size_t limit);
int _eprint(char *file, const char *func, int line, char *fmt, ...);

// STRUCTS / ENUMS
// clang-format off
struct Arena {
    void *buf;
    size_t size;
    size_t used;
    Arena *next;
};

struct Ura {
    Arena *arena_head;
    size_t arena_count;
    size_t heap_used;

    char *exec;
    int errors_count;

    expand(uraFile*, files);
};

struct uraFile {
    char *name;
    char *path;
    char *dir;
    char *content;
    size_t len;

    char *build;
    char *ll_path;
};

// clang-format off
enum Type {
    NONE,
    ERR,
    ID,

    VAR_DEC, VAR, VAR_LOAD,

    TEMPLATE_DEC, TEMPLATE_TYPE, TEMPLATE_INIT,
    VOID, BOOL, I8, I32, CHARS,
    REF,
    NULL_,

    STRUCT_DEC, DOT, ATTR,

    OWN, DREF,

    LPARENT, RPARENT, DOTS, COMA,
    LBRACK, RBRACK,
    ARRAY, ARRAY_LIT, ACCESS,

    ASSIGN,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    GT, LT, GE, LE, EQ, NQ,

    AND, OR,

    PROTO,
    FN_DEC, ARGS, VARIADIC, FN_CALL, RETURN,
    SIZEOF,

    IF, ELIF, ELSE,
    WHILE, BRK, CNT,

    END,
};

const char *to_string(Type type) {
    char *types[END + 1] = {
        // clang-format off
        [ERR] = "ERR",
        [ID] = "IDENTIFER",

        [VAR_DEC] = "VAR_DEC", [VAR] = "VAR", [VAR_LOAD] = "VAR_LOAD",
        [TEMPLATE_DEC] = "TEMPLATE_DEC", [TEMPLATE_TYPE] = "TEMPLATE_TYPE",
        [TEMPLATE_INIT] = "TEMPLATE_INIT",

        [VOID] = "VOID", [BOOL] = "BOOL", [I8] = "I8", [I32] = "I32",
        [CHARS] = "CHARS",
        [REF] = "REF",
        [NULL_] = "NULL",

        [STRUCT_DEC] = "STRUCT_DEC", [DOT] = "DOT", [ATTR] = "ATTR",

        [OWN] = "OWN", [DREF] = "DREF",

        [LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
        [LBRACK] = "LBRACK", [RBRACK] = "RBRACK", 
        [DOTS] = "DOTS", [COMA] = "COMA",
        
        [ARRAY] = "ARRAY", [ARRAY_LIT] = "ARRAY_LIT",
        [ACCESS] = "ACCESS",
        
        [ASSIGN] = "ASSIGN",
        [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
        [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
        [MOD_ASSIGN] = "MOD_ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE",
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [AND] = "AND", [OR] = "OR",

        [PROTO] = "PROTO",
        [FN_DEC] = "FN_DEC", [ARGS] = "ARGS", [VARIADIC] = "VARIADIC",
        [FN_CALL] = "FN_CALL", [RETURN] = "RETURN",
        [SIZEOF] = "SIZEOF",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
        [WHILE] = "WHILE", [BRK] = "BRK", [CNT] = "CNT",

        [END] = "END",
        // clang-format on
    };
    if (types[type] == NULL)
        return "UNKNOWN";
    return types[type];
}
// clang-format on

struct Token {
    Type type;
    char *name;

    struct {
        struct {
            long value;
        } i32;
    };
};

struct Node {
    Token *token;
    Node *left;
    Node *right;

    expand(Node *, children);
};

// clang-format on

Ura ura;

/*
TODO:
    [ ] open file
    [ ] tokenize it

    [ ] support: macos, windows
*/

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        help("%s <file>.ura\n", av[0]);
        return;
    }
    ura.exec = "exe.out";
    for (int i = 0; i < ac && ura.errors_count; i++) {
        char *arg = av[i];
        if (ura_strcmp(arg, "-o")) {
            if (i + 1 >= ac) {
                eprint("expected argument:\n");
                help("-o exe.out\n");
                exit(1);
            }
            ura.exec = av[++i];
        } else {
            size_t n = strlen(arg);
            bool is_ura = n > 4 && ura_strcmp(arg + n - 4, ".ura");
            if (!is_ura) {
                eprint("Invalid file '%s'\n", arg);
                help("use %s.ura\n", arg);
                exit(1);
            }
            new_file(arg);
        }
    }
}

int main(int ac, char **av) {
    atexit(ura_free);
    parse_args(ac, av);
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
    }
}

// MEMORY
Arena *new_arena(size_t size) {
    Arena *new = calloc(1, sizeof(Arena));
    if (new == NULL) {
        eprint("calloc failed\n");
        exit(1);
    }
    new->buf = calloc(size, 1);
    if (new->buf == NULL) {
        eprint("calloc failed\n");
        exit(1);
    }
    new->size = size;
    ura.arena_count++;
    ura.heap_used += (size + sizeof(Arena));
    return new;
}

// clang-format off
void *ura_alloc(size_t asked_count, size_t asked_size) {
    static Arena *curr;

    size_t asked = asked_count * asked_size;
    asked = (asked + 16 - 1)  / 16 * 16;

    Arena *head = ura.arena_head;
    if (head == NULL || curr->used + asked >= curr->size) {
        size_t size = 1 << 8;
        if (curr != NULL) size = curr->size << 1;
        while (size < asked) size <<= 1;
        Arena *arena = new_arena(size);

        if(ura.arena_head == NULL) { // first time
            ura.arena_head = arena;
            curr = ura.arena_head;
        }
        else {
            curr->next = arena;
            curr = curr->next;
        }
    }
    void *res = curr->buf + curr->used;
    curr->used += asked;
    return res;
}
// clang-format on

void ura_free() {
    Arena *curr = ura.arena_head;
    while (curr) {
        Arena *next = curr->next;
        free(curr->buf);
        free(curr);
        curr = next;
    }
}

char *ura_strdup(char *str) {
    char *res = ura_alloc(strlen(str) + 1, sizeof(char));
    strcpy(res, str);
    return res;
}

// clang-format off
bool ura_strcmp(char *left, char *right) {
    size_t i = 0;
    while (left[i] && left[i] == right[i]) i++;
    return left[i] == '\0' && right[i] == '\0';
}

bool ura_strncmp(char *left, char *right, size_t limit) {
    size_t i = 0;
    while (i < limit && left[i] && left[i] == right[i]) i++;
    return i == limit;
}
// clang-format on

// FILE MANAGEMENT
uraFile *new_file(char *name) {
    uraFile *new = ura_alloc(1, sizeof(uraFile));
    new->name = name;
    push_back(ura.files, new);

    char *path = realpath(name, NULL);
    if (path == NULL) {
        eprint("realpath failed\n");
        exit(1);
    }
    new->path = ura_strdup(path);
    free(path);
    new->dir = dirname(new->path);

    printf("path: %s\n", new->path);
    printf("dir: %s\n", new->dir);

    return new;
}

// FORMATING / PRINTING
int _print_type(File fp, Node *node) {
    // clang-format off
    if (node == NULL) return fprintf(fp, "void");
    switch(node->token->type) {
    case REF:        return fprintf(fp, "&") + _print_type(fp, node->left);
    case STRUCT_DEC: return fprintf(fp, "%s", node->token->name);
    case BOOL:       return fprintf(fp, "b1");
    case I8:         return fprintf(fp, "i8");
    case I32:        return fprintf(fp, "i32");
    case NULL_:      return fprintf(fp, "null");
    case ARRAY:      return _print_type(fp, node->left) + fprintf(fp, "[]");
    default:         return fprintf(fp, "%s", to_string(node->token->type));
    }
    // clang-format on
};

int _print_token(File fp, Token *token) {
    if (token == NULL)
        return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name != NULL)
        
    return 0;
}

int _print_node(File fp, Node *node) {
    return 0;
}

int _print(File fp, const char *fmt, va_list ap) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (fmt[++i] == '\0')
            break;
        Token *token = NULL;
        // clang-format off
        switch (fmt[i]) {
        case 's': r += fprintf(fp, "%s", va_arg(ap, char*)); break;
        case 'd': r += fprintf(fp, "%d", va_arg(ap, int)); break;
        case 'c': r += fprintf(fp, "%c", va_arg(ap, int)); break;
        case 'f': r += fprintf(fp, "%f", va_arg(ap, double)); break;
        case 'z': r += fprintf(fp, "%zu", va_arg(ap, size_t)); i++; break;
        case 'i': r += fprintf(fp, "%*s", va_arg(ap, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 't': r += fprintf(fp, "%s", to_string(va_arg(ap, Type))); break;
        case 'k': r += _print_token(fp, va_arg(ap, Token*)); break;
        case 'n': r += _print_node(fp, va_arg(ap, Node *)); break;
        case 'N': r += _print_type(fp, va_arg(ap, Node*)); break;
        default : break;
        }
        // clang-format on
    }
    return r;
}

char *format(char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    File out = open_memstream(&buf, &size);
    if (!out) {
        eprint("open_memstream failed\n");
        exit(1);
    }

    va_list ap;
    va_start(ap, fmt);
    _print(out, fmt, ap);
    va_end(ap);

    fclose(out);
    char *res = ura_strdup(buf);
    free(buf);
    return res;
}

int _eprint(char *file, const char *func, int line, char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = fprintf(stderr, RED("%s:%d %s") " ", file, line, func);
    r += _print(stderr, fmt, args);
    va_end(args);
    return r;
}

int print(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = _print(stdout, fmt, args);
    va_end(args);
    return r;
}