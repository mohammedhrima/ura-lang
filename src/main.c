#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

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

#define FILE __FILE__
#define LINE __LINE__
#define FUNC __func__

#define expand(type, variable) \
    type *variable;            \
    size_t variable##_count;   \
    size_t variable##_size;    \
    size_t variable##_type_size;

#define push_back(parent, child)                                             \
    if (parent##_size == 0) /* empty */                                      \
    {                                                                        \
        parent##_type_size = sizeof(child);                                  \
        parent = ura_alloc((parent##_size = 10), parent##_type_size);        \
    } else if (parent##_count + 1 == parent##_size)                          \
        parent = realloc(parent, (parent##_size *= 2) * parent##_type_size); \
    parent[parent##_count++] = child;


int _print(File fp, const char *fmt, va_list args);
int eprint(char *fmt, ...);

typedef struct uraFile uraFile;
struct uraFile {
    char *filename;
    size_t len;
    char *content;
};

// clang-format off
typedef enum Type Type;
enum Type {
    NONE,
    IDENTIFIER,

    I32,

    LPARENT, RPARENT, DOTS,

    ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    END,
};
// clang-format on

typedef struct Token Token;
struct Token {
    Type type;

    bool is_dec;

    struct {
        char *name;
        // struct {} Fn;
        struct {
            long value;
        } i32;
    };
};

typedef struct Node Node;
struct Node {
    Token *token;

    Node *left;
    Node *right;

    expand(Node *, children);
};

struct {
    int errors_count;
    expand(uraFile *, files);
    expand(Token *, tokens);

    char *curr_content;
    size_t exe_pos;

    Node *head;
} ura;

// MEMORY/ERROR/LOGGING HANDLING

void *ura_alloc(size_t count, size_t size) {
    void *res = calloc(count, sizeof(uraFile));
    if (!res) {
        eprint("ura_alloc failed\n");
    }
    return res;
}

// clang-format off
const char *to_string(Type type) {
    char *types[END + 1] = {
        [IDENTIFIER] = "IDENTIFER",
        [I32] = "I32",

        [LPARENT] = "LPARENT", [RPARENT] = "LPARENT",
        [DOTS] = "DOTS",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", 
        [DIV] = "DIV", [MOD] = "MOD",

        [END] = "END",
    };
    if (types[type] == NULL)
        return "UNKNOWN";
    return types[type];
}
// clang-format on

int _print(File fp, const char *fmt, va_list args) {
    int r = 0;
    for (int i = 0; fmt[i];) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            i++;
            continue;
        }

#define macro(value, type)                             \
    if (strncmp(fmt + i, value, strlen(value)) == 0) { \
        r += fprintf(fp, value, va_arg(args, type));   \
        i += strlen(value);                            \
        continue;                                      \
    }

        macro("%s", char *);
        macro("%d", int);
        macro("%f", double); // TODO: to be checked
#undef macro
        if (strncmp(fmt + i, "%%", 2) == 0) {
            r += fprintf(fp, "%%");
            i += 2;
            continue;
        }
        if (strncmp(fmt + i, "%k", 2) == 0) {
            Token *token = va_arg(args, Token *);
            r += fprintf(fp, "%s", token ? to_string(token->type) : "(null token)");
            i += 2;
            continue;
        }

        printf("%s:%d unhandled case %s\n", FILE, LINE, fmt + i);
        // i++;
        break;
    }
    return r;
}

int eprint(char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = _print(stderr, fmt, args);
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

// FILE HANDLING
uraFile *open_file(char *filename) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    if (file == NULL) {
        eprint("%s:%d", FILE, LINE, "calloc failed");
        return NULL;
    }
    file->filename = filename;
    push_back(ura.files, file);

    File fp = fopen(filename, "r");
    if (fp == NULL) {
        eprint("%s:%d", FILE, LINE, "fopen failed");
        return NULL;
    }
    // TODO: check those to if they failed
    fseek(fp, sizeof(char), SEEK_END);
    file->len = ftell(fp);
    rewind(fp);
    file->content = ura_alloc(file->len + 1, sizeof(char));
    fread(file->content, file->len, sizeof(char), fp);
    return file;
}

void close_file(uraFile *file) {
    free(file->filename);
    free(file->content);
}

// TOKENIZE
Token *new_token(Type type, size_t s, size_t e) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    switch (type) {
    case IDENTIFIER: {
        if (strncmp(ura.curr_content + s, "i32", e - s) == 0) {
            new->type = I32;
            new->is_dec = true;
        } else {
            new->name = ura_alloc(e - s + 1, sizeof(char));
            strncpy(new->name, ura.curr_content + s, e - s);
        }
        break;
    }
    case I32: {
        // TODO: check INT_MIN, INT_MAX boundries
        new->i32.value = atoi(ura.curr_content + s);
        break;
    }
    default:
        break;
    }
    print("new token %k\n", new);
    push_back(ura.tokens, new);
    return new;
}

void tokenize(char *content) {
    ura.curr_content = content;
    size_t s = 0;
    size_t e = 0;
    while (content && content[e]) {
        s = e;
        if (isspace(content[e])) {
            e++;
            continue;
        }

        while (strncmp(content + s, "//", 2) == 0 && content[e] && content[e] != '\n')
            e++;
        if (e != s)
            continue;

        while (isalpha(content[s]) && (isalnum(content[e]) || content[e] == '_'))
            e++;
        if (e != s) // found identifier
        {
            new_token(IDENTIFIER, s, e);
            continue;
        }

        // bool is_float = false;
        while (isdigit(content[e]))
            e++;
        if (e != s) {
            new_token(I32, s, e);
            continue;
        }
        // clang-format off
        struct {
            char *value;
            Type type;
        } specials[] = {
            {"(", LPARENT}, {")", RPARENT}, {":", DOTS}, 
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {NULL, NONE}
        };
        // clang-format on
        for (int i = 0; specials[i].value; i++) {
            size_t len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                new_token(specials[i].type, 0, 0);
                e += len;
            }
        }
        if (e != s)
            continue;
        eprint("%s:%d handle this case: <%s>", FILE, LINE, content + e);
        break;
    }
    new_token(END, 0, 0);
}

// ABSTRACT SYNTAX TREE
Token *find(Type type, ...) {
    va_list ap;
    va_start(ap, type);
    while (type && ura.tokens[ura.exe_pos]) {
        if (type == ura.tokens[ura.exe_pos]->type)
            return ura.tokens[ura.exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
}

Token *peek(int index) {
    if (ura.exe_pos + index > ura.tokens_count) {
        eprint("%s:%d index out of range\n", FILE, LINE);
        return NULL;
    }
    return ura.tokens[ura.exe_pos + index];
}

Token *expect(Type type) {
    return NULL;
}

Token *next() {
    Token *token = peek(0);
    ura.exe_pos++;
    return token;
}

Node *new_node(Token *token) {
    Node *new = ura_alloc(1, sizeof(Node));
    new->token = token;
    print("new node %k\n", new->token);
    return new;
}

Node *prime_node() {
    Token *token = next();
    switch (token->type) {
    case I32: {
        return new_node(token);
    }
    default: {
        eprint("%s:%d handle this case", FILE, LINE);
        break;
    }
    }
    return NULL;
}

Node *expr_node(int min_op) {
    Node *left = prime_node();
    while (true) {
        // clang-format off
        int prec[END + 1] = {
            [ADD] = 10, [SUB] = 10, 
            [MUL] = 11, [DIV] = 11, [MOD] = 11,
        };
        // clang-format on
        Type type = peek(0)->type;
        int op = prec[type];
        if (op < min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        node->right = expr_node(op);

        left = node;
    }
    return left;
}

void generate_ast() {
    if (ura.errors_count)
        return;
    ura.head = new_node(new_token(IDENTIFIER, 0, 0));
    while(!find(END, 0) && !ura.errors_count) {
        Node *child = expr_node(0);
        push_back(ura.head->children, child);
    }

}

/*
TODO:
    + ast
    + print ast
*/

int main() {
    uraFile *file = open_file(strdup("./file.ura"));
    print("============Tokenize============\n");
    tokenize(file->content);
    print("============AST=================\n");
    generate_ast();
    print("============Cleaning============\n");
    close_file(file);
}