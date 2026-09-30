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

// TODO: fix those
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
void arena_reserve(size_t size);
uraFile *new_file(char *name);
bool ura_strcmp(char *left, char *right);
bool ura_strncmp(char *left, char *right, size_t limit);
int _eprint(char *file, const char *func, int line, char *fmt, ...);
int print(char *fmt, ...);

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
    Arena *arena_curr;
    size_t arena_count;
    size_t heap_used;

    char *exec;
    int errors_count;

    uraFile curr_file;
    int curr_line;

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

// Global Ura
Ura ura;

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

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        help("%s <file>.ura\n", av[0]);
        return;
    }
    ura.exec = "exe.out";
    for (int i = 1; i < ac && ura.errors_count == 0; i++) {
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

struct Token {
    Type type;
    char *name;
    bool is_type;
    int space;

    struct {
        struct {
            long value;
        } i32;
        struct {
            int value; // TODO: check unicode stuff
        } i8;
        struct {
            int value;
        } b1;
        struct {
            char *value;
        } chars;
    };
};

void gen_tokens(uraFile *file) {
    ura.curr_file = file;
    ura.curr_line = 1;
    char *content = file->content;
    arena_reserve(strlen(content) * 192);

    size_t s = 0;
    size_t e = 0;
    int space = 0;
    while(content[e]) {
        s = e;
        if(isspace(content[e])) {
            while(content[e] == '\n') {
                ura.curr_line++;
                e++;
            }
            if(content[s] == '\n') {
                space = 0;
                s = e - 1;
            }
        }
    }
}

struct Node {
    Token *token;
    Node *left;
    Node *right;

    expand(Node *, children);
};


/*
TODO:
    [ ] open file
    [ ] tokenize it
    [ ] support: macos, windows

    [ ] drop method
    [ ] String struct
    [ ] to_string method
    [ ] output (should handl also struct.to_string)
    [ ] for template add a part in the code thaht JIT/interpreter
    [ ] operators overload
    [ ] for loops over a range: for i in 0..10
    [ ] more integer types (i8, i64, unsigned) and casting with as
    [ ] function inside function
    [ ] learn some design patterns
*/

int main(int ac, char **av) {
    atexit(ura_free);
    parse_args(ac, av);
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        gen_tokens(file);
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
void arena_reserve(size_t size) {
    Arena *curr = ura.arena_curr;
    if (curr && curr->size - curr->used >= size) return;
    Arena *new = new_arena((size + 16 - 1) / 16 * 16);
    if (curr) curr->next = new;
    else ura.arena_head = curr;
    ura.arena_curr = new;
}

void *ura_alloc(size_t asked_count, size_t asked_size) {
    Arena *curr = ura.arena_curr;

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
    ura.arena_curr = curr;
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
    new->dir = dirname(ura_strdup(new->path));
    return new;
}

// FORMATING / PRINTING
// clang-format off
int _print_type(File fp, Node *node) {
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
};

int _print_token(File fp, Token *token) {
    if (token == NULL) return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name != NULL) r += fprintf(fp, " name (%s)", token->name);
    if(token->name == NULL && !token->is_type) {
        switch(token->type) {
        case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case I32:   r += fprintf(fp, " value (%ld)", token->i32.value); break;
        case CHARS: r += fprintf(fp, " value (%s)", token->chars.value); break;
        case BOOL: {
            char *b1 = token->b1.value ? "True" : "False";
            r += fprintf(fp, " value (%s)", b1);
            break;
        }
        default: break;
        }
    }
    r += fprintf(fp, " space (%d)", token->space);
    return 0;
}

typedef struct NodePrint NodePrint;
struct NodePrint {
    expand(int, depths);
    expand(Node*, nodes);
};

bool includes(Type to_find, ...) {
    va_list ap;
    va_start(ap, to_find);
    Type curr = va_arg(ap, Type);
    while (curr) {
        if (curr == to_find) return true;
        curr = va_arg(ap, Type);
    }
    return false;
}

void _print_node_helper(NodePrint *elems, Node *node, int depth) {
    if(node == NULL) return;

    push_back(elems->nodes, node);
    push_back(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, 0)) {
        Node *left = node->left;
        bool is_struct = left && left->token->type == STRUCT_DEC;
        if (includes(node->token->type, ID, VAR, REF, 0) && is_struct) {
            push_back(elems->nodes, left);
            push_back(elems->depths, depth + 3);
        } 
        else _print_node_helper(elems, left, depth + 3);
    }
    if (!includes(node->token->type, FN_CALL, 0))
        _print_node_helper(elems, node->right, depth + 3);
    for (size_t i = 0; i < node->children_count; i++)
        _print_node_helper(elems, node->children[i], depth + 3);
}

bool is_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++) {
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    }
    return 0;
}

int _print_node(File fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    _print_node_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", is_open(&elems, i, level) ? "│ " : " ");
        if (depth) r += fprintf(fp, "%s", is_open(&elems, i, depth) ? "├─" : "└─");
        r += _print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return 0;
}

int _print(File fp, const char *fmt, va_list ap) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (fmt[++i] == '\0') break;
        Token *token = NULL;
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
    }
    return r;
}
// clang-format on

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