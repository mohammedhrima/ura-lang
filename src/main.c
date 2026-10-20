// INCLUDES
#include <stdio.h>
#include <stdarg.h>
// DEFINE MACROS


typedef struct Token Token;
typedef struct Node Node;
typedef struct NodePrint NodePrint;
typedef struct uraError uraError;

#define print(...)  _print(stdout, __VA_ARGS__)
#define help(...)   _print(stdout, __VA_ARGS__)
#define eprint(...) _print(stderr, __VA_ARGS__)

#ifndef TOKENIZE
#    define TOKENIZE 1

#endif


#include "./ir.h"

int _fprint(ioFile fp, const char *fmt, va_list ap);
#undef vfprint
#define vfprint _fprint

struct Token {
    Type type;
    char *name;
    int space;

    int line;
    int s;
    int e;

    bool is_type;
    bool is_variadic;
    uraIR ir;

    union {
        struct {
            int index;
        } attr;
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

struct Node {
    Token *token;
    Node *left;
    Node *right;

    expand(Node *, children);
};

struct uraError {
    Node *file;
    Token *token;
    int s;
    int e;
    int line;
    char *msg;
    char *help;
};

struct NodePrint {
    expand(int, depths);
    expand(Node *, nodes);
};

// Global Ura
struct {
    Arena *arena_head;
    Arena *arena_curr;
    size_t arena_count;
    size_t heap_used;

    char *ll_path;
    char *exec_path;

    // int errors_count;
    // File errors;
    // char *errors_buff;
    // int errors_len;


    Node *tree;
    expand(Token *, tokens);

    expand(Node *, scopes);
    Node *scope;

    expand(File *, files);
    expand(uraError, errors);

    int exe_pos;

    Context context;
    Module module;
    Builder builder;
} ura;

struct {
    char *value;
    Token token;
} keywords[] = {
    // clang-format off
	{ "bool", { .type = BOOL, .is_type = true } },
	{ "char", { .type = I8, .is_type = true } },
	{ "i8", { .type = I8, .is_type = true } },
	{ "i32", { .type = I32, .is_type = true } },

	{ "True", { .type = BOOL, .b1 = { .value = true } } },
	{ "False", { .type = BOOL, .b1 = { .value = false } } },

	{ "struct", { .type = STRUCT } },
	{ "and", { .type = AND } }, { "or", { .type = OR } },
	{ "not", { .type = NOT } },
	{ "proto", { .type = PROTO } }, { "fn", { .type = FN_DEC } },
	{ "return", { .type = RETURN } },
	{ "if", { .type = IF } }, { "elif", { .type = ELIF } },
	{ "else", { .type = ELSE } },
	{ "while", { .type = WHILE } }, { "break", { .type = BRK } },
	{ "continue", { .type = CNT } },
	{ "template", { .type = TEMPLATE } },
	// { "null", {.type = ZERO } },
	{ "as", {.type = AS } },

	{ NULL },
    // clang-format on
};

struct {
    char *value;
    Type type;
} specials[] = {
    // clang-format off
	{ "(", LPARENT }, { ")", RPARENT }, { ":", DOTS },
	{ "[", LBRACK }, { "]", RBRACK },

	{ "+=", ADD_ASSIGN }, { "-=", SUB_ASSIGN }, { "*=", MUL_ASSIGN },
	{ "/=", DIV_ASSIGN }, { "%=", MOD_ASSIGN },
	
	{ "...", VARIADIC }, { ".", DOT },
	
	{ "+", ADD }, { "-", SUB }, { "*", MUL }, { "/", DIV }, { "%", MOD },

	{ ">=", GE }, { "<=", LE }, { ">", GT }, { "<", LT },
	{ "==", EQ }, { "!=", NQ }, { "!", NOT },

	{ "=", ASSIGN }, { ",", COMA },
	{ "&&", AND }, { "||", OR },
	{ "&", REF },
	{NULL, NONE}
    // clang-format on
};

char *binary_ops[END + 1] = {
    // clang-format off
	[ADD] = "+", [SUB] = "-", [MUL] = "*", 
	[DIV] = "/", [MOD] = "%",
	
	[ADD_ASSIGN] = "+", [SUB_ASSIGN] = "-", 
	[MUL_ASSIGN] = "*", [DIV_ASSIGN] = "/", 
	[MOD_ASSIGN] = "%",
	
	[EQ] = "==", [NQ] = "!=", 
	[LT] = "<", [GT] = ">", 
	[LE] = "<=", [GE] = ">=",
	
	[AND] = "and", [OR] = "or",
    // clang-format on
};

int precedence[END + 1] = {
    // clang-format off
    [ASSIGN] = 1,     [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
    [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1, [MOD_ASSIGN] = 1,

    [OR] = 11,        [AND] = 12,

    [EQ] = 21,        [NQ] = 21,        [GT] = 22,
    [LT] = 22,        [GE] = 22,        [LE] = 22,

    [ADD] = 30,       [SUB] = 30,       
    [MUL] = 31,       [DIV] = 31,       [MOD] = 31,

    [AS] = 35,

    [DOT] = 51,
    // clang-format on
};

char *escaped[255] = {
    ['n'] = "\n",   ['t'] = "\t",   ['r'] = "\r",   ['b'] = "\b",
    ['f'] = "\f",   ['v'] = "\v",   ['a'] = "\a",   ['?'] = "\?",
    ['0'] = "\0",   ['"'] = "\"",   ['\''] = "\'",  ['\\'] = "\\",

    ['\n'] = "\\n", ['\t'] = "\\t", ['\r'] = "\\r", ['\0'] = "\\0",
};

// TOKENIZE
Token *new_token(Type type, Token *from) {
    Token *new = arena_alloc(1, sizeof(Token));
    new->type = type;
    if (from != NULL) {
        new->space = from->space;
        new->line = from->line;
        new->s = from->s;
        new->e = from->e;
    }
    append(ura.tokens, new);
    return new;
}

void tokenize(File *file, int space) {
}

// AST


// ANALYZE

/*
TODO:
    + replace all fprintf with builtins
    + try buffring instead of calling fprintf everytime
    + use (reopen already opened file)
    + reuse (don't reopen opened files)
*/

int main(int ac, char **av) {
    atexit(arena_free);
    arena_reserve(1000);
    parse_args(ac, av);
    ura.tokens_count = 0;
    ura.exe_pos = 0;

#if TOKENIZE
    print(GREEN("======================================\n"));
    print(GREEN("[TOKENIZE]:\n"));
    for (size_t i = 0; i < ura.files_count; i++) {
        File *file = ura.files[i];
        tokenize(file, 0);
    }
    new_token(END, NULL);

    check_errors();

    print(GREEN("======================================\n"));
    for (size_t i = 0; i < ura.tokens_count; i++)
        print(CYAN("token ") "%k\n", ura.tokens[i]);

#    if AST
    print(GREEN("======================================\n"));
    print(GREEN("[AST]:\n"));
    for (size_t i = 0; i < ura.tree->children_count; i++) {
        Node *file = ura.tree->children[i]; // File node / empty children
        enter_scope(file);
        parse_structs(file);
        while (peek(0)->type != END) {
            push_child(file, parse_expr(0));
        }
        // handle_global_scope(file);
        exit_scope();
    }
    check_errors(); // the tree is broken: don't analyze it
    print(GREEN("======================================\n"));
    for (size_t i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#        if ANALYZE
    print(GREEN("======================================\n"));
    print(GREEN("[ANALYZE]:\n"));
    for (size_t i = 0; i < ura.tree->children_count; i++) {
        Node *file = ura.tree->children[i];
        analyze(file, 0);
    }
    check_errors(); // the tree is not valid code: don't generate it
    print(GREEN("======================================\n"));
    for (size_t i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#            if ASM
    print(GREEN("======================================\n"));
    print(GREEN("[CODE GEN]:\n"));
    make_dir(format("%s/build", cwd_path())); // TODO: save cwd in ura
    codegen_beg("ura");
    for (size_t i = 0; i < ura.tree->children_count; i++) {
        Node *file = ura.tree->children[i];
        codegen(file);
    }
    codegen_finish();
    gen_bin();
    // return exec_bin();
#            endif // ASM
#        endif     // ANALYZE
#    endif         // AST
#endif             // TOKENIZE
    printf("did allocate %zu\n", arena_size());
}

// UTILS / FORMATING / PRINTING
int _print_type(ioFile fp, Node *node) {
    // clang-format off
    if (node == NULL) return fprintf(fp, "void");
    switch(node->token->type) {
    case REF:		return fprintf(fp, "&") + _print_type(fp, node->left);
    case STRUCT:	return fprintf(fp, "%s", node->token->name);
    case BOOL:		return fprintf(fp, "bool");
    case I8:		return fprintf(fp, "i8");
    case I32:		return fprintf(fp, "i32");
    case ARRAY:		return _print_type(fp, node->left) + fprintf(fp, "[]");
    default:		return fprintf(fp, "%s", to_string(node->token->type));
    }
    // clang-format on
};

int _print_char(ioFile fp, char c) {
    char *str = escaped[c];
    return str ? fprintf(fp, "%s", str) : fprintf(fp, "%c", c);
}

// clang-format off
int _print_token(ioFile fp, Token *token) {
    if (token == NULL) return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name != NULL) r += fprintf(fp, " name (%s)", token->name);
    if(token->name == NULL && !token->is_type) { 
        switch(token->type) {
        case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case I32:   r += fprintf(fp, " value (%ld)", token->i32.value); break;
        case CHARS: {
            #if 1
            r += fprintf(fp, " value (");
            char *value = token->chars.value;
            for(int i = 0; value && value[i]; i++)
                r += _print_char(fp, value[i]);
            r += fprintf(fp, ")"); 
            #else
            r += fprintf(fp, " value (%s)", token->chars.value);
            #endif
            break;
        }
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

void _print_node_helper(NodePrint *elems, Node *node, int depth) {
    if (node == NULL)
        return;

    append(elems->nodes, node);
    append(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, FN_CALL, 0)) {
        Node *left = node->left;
        bool is_struct = left && left->token->type == STRUCT;
        if (is_struct) {
            append(elems->nodes, left);
            append(elems->depths, depth + 3);
            // print(">>>> %n\n", left);
            // exit(1);
            // if (left->left) {
            //     append(elems->nodes, left->left);
            //     append(elems->depths, depth + 3);
            // }
        } else
            _print_node_helper(elems, left, depth + 3);
    }
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

int _print_node(ioFile fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    _print_node_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", is_open(&elems, i, level) ? "│ " : " ");
        if (depth)
            r += fprintf(fp, "%s", is_open(&elems, i, depth) ? "├─" : "└─");
        r += _print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return 0;
}

int _print(ioFile fp, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = _fprint(fp, fmt, args);
    va_end(args);
    return r;
}

int _fprint(ioFile fp, const char *fmt, va_list ap) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (fmt[++i] == '\0')
            break;
        Token *token = NULL;
        switch (fmt[i]) { // clang-format off
        case 's': r += fprintf(fp, "%s", va_arg(ap, char *)); break;
        case 'd': r += fprintf(fp, "%d", va_arg(ap, int)); break;
        case 'c': r += fprintf(fp, "%c", va_arg(ap, int)); break;
        case 'f': r += fprintf(fp, "%f", va_arg(ap, double)); break;
        case 'i': r += fprintf(fp, "%*s", va_arg(ap, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 't': r += fprintf(fp, "%s", to_string(va_arg(ap, Type))); break;
        case 'k': r += _print_token(fp, va_arg(ap, Token *)); break;
        case 'n': r += _print_node(fp, va_arg(ap, Node *)); break;
        case 'N': r += _print_type(fp, va_arg(ap, Node *)); break;
        case 'z': {
            r += fprintf(fp, "%zu", va_arg(ap, size_t));
            i++;
            break;
        }
        case '.': {
            int n = va_arg(ap, int);
            char *str = va_arg(ap, char *);
            r += fprintf(fp, "%.*s", n, str);
            i += 2; // skip *s
            break;
        } 
        default: break; // clang-format on
        }
    }
    return r;
}

void check_errors(void) {
    if (ura.errors_count == 0)
        return;
    exit(1);
}

bool includes(Type to_find, ...) {
    va_list ap;
    va_start(ap, to_find);
    Type curr = va_arg(ap, Type);
    while (curr) {
        if (curr == to_find)
            return true;
        curr = va_arg(ap, Type);
    }
    return false;
}

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        help("%s <file>.ura\n", av[0]);
        exit(1);
    }
    for (int i = 1; i < ac && ura.errors_count == 0; i++) {
        char *arg = av[i];
        if (strcmp2(arg, "-o")) {
            if (i + 1 >= ac) {
                eprint("expected argument:\n");
                help("-o exe.out\n");
                exit(1);
            }
            set_exec_path(av[++i]);
        } else {
            size_t n = strlen(arg);
            bool is_ura = n > 4 && strcmp2(arg + n - 4, ".ura");
            if (!is_ura) {
                eprint("Invalid file '%s'\n", arg);
                help("use %s.ura\n", arg);
                exit(1);
            }
            File *file = new_file(arg);
            append(ura.files, file);
        }
    }
}
