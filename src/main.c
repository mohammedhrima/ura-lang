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
#define TAB  4 /*TODO: to be checked if can be got from some c function*/

#define expand(type, variable) \
    type *variable;            \
    long variable##_count;     \
    long variable##_size;

#define push_back(parent, child)                                              \
    {                                                                         \
        if (parent##_size == 0) /* empty */                                   \
            parent = ura_alloc((parent##_size = 10), sizeof(*parent));        \
        else if (parent##_count + 1 == parent##_size)                         \
            parent = realloc(parent, (parent##_size *= 2) * sizeof(*parent)); \
        parent[parent##_count++] = child;                                     \
    }


typedef struct uraFile uraFile;
typedef enum Type Type;
typedef struct Token Token;
typedef struct Node Node;

int _print(File fp, const char *fmt, va_list args);
int eprint(char *fmt, ...);
Node *expr_node(int min_op);

struct uraFile {
    char *filename;
    long len;
    char *content;
};

// clang-format off
enum Type {
    NONE,
    IDENTIFIER,

    I32,

    LPARENT, RPARENT, DOTS,

    ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    FDEC,

    END,
};
// clang-format on

struct Token {
    Type type;

    bool is_dec;
    long space;

    struct {
        char *name;
        // struct {} Fn;
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

struct {
    int errors_count;
    expand(uraFile *, files);
    expand(Token *, tokens);

    char *curr_content;
    long exe_pos;

    expand(Node *, nodes);
} ura;

// MEMORY/ERROR/LOGGING HANDLING

void *ura_alloc(long count, long size) {
    void *res = calloc(count, size);
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

        [ASSIGN] = "ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", 
        [DIV] = "DIV", [MOD] = "MOD",

        [FDEC] = "FDEC",
    
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

        int prefix = 0;
        if (strncmp(fmt + i, "%i", 2) == 0) {
            prefix = va_arg(args, int);
            i += 2;
            // don't continue
        }

        if (prefix > 0)
            r += fprintf(fp, "%*s", prefix, "");
        if (fmt[i] != '%') /* %i used on its own */
            continue;

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
            switch (token->type) {
            case IDENTIFIER: {
                r += fprintf(fp, " name (%s)", token->name);
                break;
            }
            case I32: {
                r += fprintf(fp, " value (%ld)", token->i32.value);
                break;
            }
            default:
                break;
            }
            r += fprintf(fp, " space (%ld)", token->space);
            i += 2;
            continue;
        }

        printf("%s:%d unhandled case %s\n", FILE, LINE, fmt + i);
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

char *format(char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    File out = open_memstream(&buf, &size);
    if (!out) {
        eprint("format: open_memstream failed");
        return NULL;
    }

    va_list ap;
    va_start(ap, fmt);
    _print(out, fmt, ap);
    va_end(ap);
    fclose(out);
    return buf;
}

// TODO: this code needs to be reviewed
typedef struct NodePrint NodePrint;
struct NodePrint {
    expand(Node *, nodes);
    expand(int, depths);
};

void print_helper(NodePrint *elems, Node *node, int depth) {
    if (!node)
        return;

    push_back(elems->nodes, node);
    push_back(elems->depths, depth);

    print_helper(elems, node->left, depth + 2);
    print_helper(elems, node->right, depth + 2);
    for (long i = 0; i < node->children_count; i++)
        print_helper(elems, node->children[i], depth + 2);
}

int still_open(NodePrint *elems, long i, int level) {
    for (long j = i + 1; j < elems->nodes_count; j++)
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    return 0;
}

void print_node(Node *node) {
    NodePrint elems = {};

    print_helper(&elems, node, 0);

    for (long i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            print("%s", still_open(&elems, i, level) ? "│ " : "  ");
        if (depth)
            print("%s", still_open(&elems, i, depth) ? "├─" : "└─");
        print("%k\n", elems.nodes[i]->token);
    }

    free(elems.nodes);
    free(elems.depths);
}

// FILE HANDLING
uraFile *open_file(char *filename) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    if (file == NULL) {
        eprint("%s:%d calloc failed\n", FILE, LINE);
        return NULL;
    }
    file->filename = filename;
    push_back(ura.files, file);

    File fp = fopen(filename, "r");
    if (fp == NULL) {
        eprint("%s:%d fopen failed\n", FILE, LINE);
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

void free_token(Token *token) {
    free(token->name);
    free(token);
}

void free_node(Node *node) {
    if (!node)
        return;

    for (long i = 0; i < node->children_count; i++)
        free_node(node->children[i]);
    free(node->children);
    free_node(node->left);
    free_node(node->right);
    free(node);
}

// TOKENIZE
Token *new_token(Type type, long space) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    push_back(ura.tokens, new);
    return new;
}

Token *parse_token(Type type, long s, long e, long space) {
    Token *new = new_token(type, (space / TAB + (space % TAB != 0 ? 1 : 0)));
    new->type = type;
    switch (type) {
    case IDENTIFIER: {
        // if(e - s == 0) break;
        if (strncmp(ura.curr_content + s, "i32", e - s) == 0) {
            new->type = I32;
            new->is_dec = true;
        } else if (strncmp(ura.curr_content + s, "fn", e - s) == 0) {
            new->type = FDEC;
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
    return new;
}

void tokenize(char *content) {
    ura.curr_content = content;
    long s = 0;
    long e = 0;
    long space = 0;
    while (content && content[e]) {
        s = e;
        if (isspace(content[e])) {
            while (content[e] == '\n') {
                space = 0;
                e++;
            }
            if (content[s] == '\n')
                s = e - 1;
            while (isspace(content[e]) && content[e] != '\n') {
                if (content[s] == '\n')
                    space++;
                e++;
            }
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
            parse_token(IDENTIFIER, s, e, space);
            continue;
        }

        // bool is_float = false;
        while (isdigit(content[e]))
            e++;
        if (e != s) {
            parse_token(I32, s, e, space);
            continue;
        }
        // clang-format off
        struct {
            char *value;
            Type type;
        } specials[] = {
            {"(", LPARENT}, {")", RPARENT}, {":", DOTS}, 
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {"=", ASSIGN},
            {NULL, NONE}
        };
        // clang-format on
        for (int i = 0; specials[i].value; i++) {
            long len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                parse_token(specials[i].type, 0, 0, space);
                e += len;
            }
        }
        if (e != s)
            continue;
        eprint("%s:%d handle this case: <%s>\n", FILE, LINE, content + e);
        break;
    }
    parse_token(END, 0, 0, 0);
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

bool inside(long space) {
    return ura.tokens[ura.exe_pos]->space > space && ura.errors_count == 0;
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

Token *next() { // TODO: protect if next is END, don't advanced
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
    Node *node = NULL;
    switch (token->type) {
    case I32:
    case IDENTIFIER: {
        return new_node(token);
    }
    case FDEC: {
        node = new_node(token);
        if (peek(0)->type != IDENTIFIER) {
            eprint("Expected identifer after fn\n");
            free_node(node);
            return NULL;
        }
        node->token->name = strdup(next()->name);

        if (next()->type != LPARENT)
            eprint("Expected ( after function declaration\n");
        if (next()->type != RPARENT)
            eprint("Expected ) after function declaration\n");

        if (next()->type != DOTS)
            eprint("Expected : after function declaration\n");

        while (inside(node->token->space)) {
            push_back(node->children, expr_node(0));
        }
        return node;
    }
    case LPARENT: {
        next(); // skip (
        node = expr_node(0);
        if (next()->type != RPARENT)
            eprint("Expected )\n");
        return node;
    }
    default: { // TODO: replace this with unexpected token
        eprint("%s:%d handle this case %s\n", FILE, LINE, to_string(token->type));
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
            [ASSIGN] = 1,
            [ADD] = 10, [SUB] = 10, 
            [MUL] = 11, [DIV] = 11, [MOD] = 11,
        };
        // clang-format on
        Type type = peek(0)->type;
        // if(type == END) break;
        int op = prec[type];
        if (op <= min_op)
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
    while (!find(END, 0) && !ura.errors_count) {
        Node *child = expr_node(0);
        push_back(ura.nodes, child);
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
    print("=========PRINT AST==============\n");
    for (long i = 0; i < ura.nodes_count; i++)
        print_node(ura.nodes[i]);
    print("============Cleaning============\n");
    for (long i = 0; i < ura.nodes_count; i++)
        free_node(ura.nodes[i]);
    for (long i = 0; i < ura.tokens_count; i++)
        free_token(ura.tokens[i]);
    close_file(file);
}
