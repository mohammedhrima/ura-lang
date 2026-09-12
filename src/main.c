#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/DebugInfo.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm/Config/llvm-config.h>
#if LLVM_VERSION_MAJOR >= 13
#    if LLVM_VERSION_MAJOR == 13
// LLVM 13's PassBuilder.h declares LLVMCreatePassBuilderOptions() without
// (void), tripping the strict-prototypes error its own headers switch on
#        undef LLVM_C_STRICT_PROTOTYPES_BEGIN
#        undef LLVM_C_STRICT_PROTOTYPES_END
#        define LLVM_C_STRICT_PROTOTYPES_BEGIN
#        define LLVM_C_STRICT_PROTOTYPES_END
#    endif
#    include <llvm-c/Transforms/PassBuilder.h>
#else
#    include <llvm-c/Transforms/PassManagerBuilder.h>
#endif
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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

#define FILE       __FILE__
#define LINE       __LINE__
#define FUNC       __func__
#define TAB        4 /*TODO: to be checked if can be got from some c function*/

#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
// #define BLUE(fmt)  BOLD "\033[34m" fmt RESET
// #define YELLOW(fmt) BOLD "\033[0;33m" fmt RESET

#define expand(type, variable) \
    type *variable;            \
    size_t variable##_count;   \
    size_t variable##_size;

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
typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Bloc;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTargetRef Target;
typedef LLVMTargetMachineRef TargetMachine;
typedef LLVMTypeKind TypeKind;
typedef LLVMAttributeRef AttributeRef;
typedef LLVMMetadataRef MetadataRef;
#if LLVM_VERSION_MAJOR >= 13
typedef LLVMErrorRef Error;
typedef LLVMPassBuilderOptionsRef PassBuilderOptions;
#endif

#define PointerType  LLVMPointerTypeKind
#define IntegerType  LLVMIntegerTypeKind
#define FloatType    LLVMFloatTypeKind
#define DoubleType   LLVMDoubleTypeKind
#define VoidType     LLVMVoidTypeKind
#define FunctionType LLVMFunctionTypeKind
#define StructType   LLVMStructTypeKind

int _print(File fp, const char *fmt, va_list args);
int _eprint(char *file, int line, char *fmt, ...);
#define eprint(...) _eprint(FILE, LINE, __VA_ARGS__)
Node *expr_node(int min_op);
void enter_scope(Node *node);
void exit_scope(void);

struct uraFile {
    char *filename;
    size_t len;
    char *content;
};

// clang-format off
enum Type {
    NONE,
    IDENTIFIER,

    VOID, I32,

    LPARENT, RPARENT, DOTS,

    ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    FDEC,
    RETURN,

    END,
};
// clang-format on

struct Token {
    Type type;
    Type ret_type;

    bool is_dec;
    size_t space;

    // TODO: move this in asm.c
    struct {
        bool is_set;
        bool is_loaded;

        Value elem;
        Bloc bloc;
        TypeRef func_type;
    } llvm;

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
    expand(Node *, functions);
};

struct {
    int errors_count;
    expand(uraFile *, files);
    expand(Token *, tokens);
    expand(Node *, scopes);
    Node *scope;

    char *curr_content;
    size_t exe_pos;

    Node *ast;

    Context context;
    Module module;
    Builder builder;
} ura;

// MEMORY/ERROR/LOGGING HANDLING

void *ura_alloc(size_t count, size_t size) {
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
        [VOID] = "VOID", [I32] = "I32",

        [LPARENT] = "LPARENT", [RPARENT] = "LPARENT",
        [DOTS] = "DOTS",

        [ASSIGN] = "ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", 
        [DIV] = "DIV", [MOD] = "MOD",

        [FDEC] = "FDEC",
        [RETURN] = "RETURN",
    
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
        if (strncmp(fmt + i, "%t", 2) == 0) {
            Type type = va_arg(args, Type);
            r += fprintf(fp, "%s", to_string(type));
            i += 2;
            continue;
        }
        if (strncmp(fmt + i, "%k", 2) == 0) {
            Token *token = va_arg(args, Token *);
            r += fprintf(fp, "%s", token ? to_string(token->type) : "(null token)");
            if (token->name)
                r += fprintf(fp, " name (%s)", token->name);
            switch (token->type) {
            case IDENTIFIER: {
                break;
            }
            case I32: {
                if (token->name || token->is_dec)
                    break;
                r += fprintf(fp, " value (%ld)", token->i32.value);
                break;
            }
            default:
                break;
            }
            r += fprintf(fp, " space (%ld)", token->space);
            if (token->ret_type)
                r += fprintf(fp, " ret (%s)", to_string(token->ret_type));
            i += 2;
            continue;
        }

        printf("%s:%d unhandled case %s\n", RED(FILE), LINE, fmt + i);
        break;
    }
    return r;
}

int _eprint(char *file, int line, char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = fprintf(stderr, RED("%s:%d") " ", file, line);
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

char *format(char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    File out = open_memstream(&buf, &size);
    if (!out) {
        eprint("format: open_memstream failed\n");
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
    for (size_t i = 0; i < node->children_count; i++)
        print_helper(elems, node->children[i], depth + 2);
}

int still_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++)
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    return 0;
}

void print_node(Node *node) {
    NodePrint elems = {};

    print_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
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
void new_file(char *filename) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    if (file == NULL) {
        eprint("calloc failed\n");
        return;
    }
    file->filename = filename;
    push_back(ura.files, file);

    File fp = fopen(filename, "r");
    if (fp == NULL) {
        eprint("fopen failed\n");
        return;
    }
    // TODO: check those to if they failed
    fseek(fp, sizeof(char), SEEK_END);
    file->len = (size_t)ftell(fp);
    rewind(fp);
    file->content = ura_alloc(file->len + 1, sizeof(char));
    fread(file->content, file->len, sizeof(char), fp);
}

void close_file(uraFile *file) {
    free(file->filename);
    free(file->content);
    free(file);
}

void free_token(Token *token) {
    free(token->name);
    free(token);
}

void free_node(Node *node) {
    if (!node)
        return;
    for (size_t i = 0; i < node->children_count; i++)
        free_node(node->children[i]);
    free_node(node->left);
    free_node(node->right);
    free(node->children);
    free(node->functions);
    free(node);
}

void ura_clean(void) {
    free_node(ura.ast);
    for (size_t i = 0; i < ura.tokens_count; i++)
        free_token(ura.tokens[i]);
    for (size_t i = 0; i < ura.files_count; i++)
        close_file(ura.files[i]);
}
// TOKENIZE
Token *new_token(Type type, size_t space) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    push_back(ura.tokens, new);
    return new;
}

Token *parse_token(Type type, size_t s, size_t e, size_t space) {
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
        } else if (strncmp(ura.curr_content + s, "return", e - s) == 0) {
            new->type = RETURN;
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
    size_t s = 0;
    size_t e = 0;
    size_t space = 0;
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
            size_t len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                parse_token(specials[i].type, 0, 0, space);
                e += len;
            }
        }
        if (e != s)
            continue;
        eprint("handle this case: <%s>\n", content + e);
        break;
    }
    parse_token(END, 0, 0, 0);
}

// ABSTRACT SYNTAX TREE
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

bool inside(size_t space) {
    return ura.tokens[ura.exe_pos]->space > space && ura.errors_count == 0;
}

Token *peek(size_t index) {
    if (ura.exe_pos + index > ura.tokens_count) {
        eprint("index out of range\n");
        return NULL;
    }
    return ura.tokens[ura.exe_pos + index];
}

Token *expect(Type type) {
    return NULL;
}

Token *next(void) { // TODO: protect if next is END, don't advanced
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

Node *prime_node(void) {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case I32: {
        return new_node(token);
    }
    case IDENTIFIER: {
        Token *next_token = peek(0)->is_dec && includes(peek(0)->type, I32, 0) ? peek(0) : NULL;
        if (next_token) {
            next();
            token->type = next_token->type;
            token->is_dec = true;
        }
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
        enter_scope(node);

        if (next()->type != LPARENT)
            eprint("Expected ( after function declaration\n");
        if (next()->type != RPARENT)
            eprint("Expected ) after function declaration\n");

        Token *ret_token = peek(0)->is_dec && includes(peek(0)->type, I32, 0) ? peek(0) : NULL;
        if (ret_token) {
            next();
            node->token->ret_type = ret_token->type;
        } else
            node->token->ret_type = VOID;

        if (next()->type != DOTS)
            eprint("Expected : after function declaration\n");

        while (inside(node->token->space)) {
            push_back(node->children, expr_node(0));
        }
        exit_scope();
        return node;
    }
    case LPARENT: {
        next(); // skip (
        node = expr_node(0);
        if (next()->type != RPARENT)
            eprint("Expected )\n");
        return node;
    }
    case RETURN: {
        node = new_node(token);
        node->left = expr_node(0);
        return node;
    }
    default: { // TODO: replace this with unexpected token
        eprint("handle this case %s\n", to_string(token->type));
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
        int op = prec[peek(0)->type];
        if (op <= min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        node->right = expr_node(op);

        left = node;
    }
    return left;
}


void generate_ast(void) {
    if (ura.errors_count)
        return;
    ura.ast = new_node(new_token(0, 0));
    while (!includes(peek(0)->type, END, 0) && !ura.errors_count) {
        Node *child = expr_node(0);
        push_back(ura.ast->children, child);
    }
}

// INTERMEDIATE REPRESENTATION
void enter_scope(Node *node) {
    print("%s: %s\n", CYAN("enter scope"), node->token->name);
    ura.scope = node;
    push_back(ura.scopes, node);
}

void exit_scope() {
    print("%s: %s\n", CYAN("exit scope"), ura.scope->token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count == 0)
        ura.scope = NULL;
    else
        ura.scope = ura.scopes[ura.scopes_count - 1];
}

void declare_function(Node *fn) {
    Token *new = fn->token;
    // for (int i = 0; i < ura.scope->functions_count; i++) {
    //     // Token *old = ura.scope->functions[i]->token;
    //     // if (strcmp(old->name, new->name) != 0)
    //     //     continue;
    //     // if (!old->is_proto && !new->is_proto) {
    //     //     // parse_error(new, ERR_REDECL_FUNCTION, new->name);
    //     //     // parse_note(old, NOTE_PREV_DECL, new->name);
    //     //     return;
    //     // }
    //     // if (!same_signature(old, new)) {
    //     //     // parse_error(new, ERR_SIG_CONFLICT, new->name, signature_diff(old, new));
    //     //     // parse_note(old, NOTE_PREV_DECL, new->name);
    //     //     return;
    //     // }
    //     // if (old->is_proto && !new->is_proto)
    //     //     ura.scope->functions[i] = fn;
    //     return;
    // }
    // resize_array(ura.scope->functions, Node *);
    // ura.scope->functions[ura.scope->functions_count++] = fn;
    push_back(ura.scopes, fn);
}

void analyze(Node *node) {
}

void type_check(Node *node) {
}

void generate_ir(void) {
    enter_scope(ura.ast);
    // skip last one because it's ura scope
    for (size_t i = 0; i < ura.ast->children_count; i++)
        if (ura.ast->children[i]->token->type == FDEC)
            declare_function(ura.ast->children[i]);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        analyze(ura.ast->children[i]);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        type_check(ura.ast->children[i]);
    exit_scope();
}

// ASSEMBLY
#include "asm.c"

void code_gen(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case I32: {
        Value elem;
        if (node->token->name)
            elem = create_variable(node->token);
        else
            elem = create_value(node->token);
        node->token->llvm.elem = elem;
        break;
    }
    case ADD: {
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        node->token->llvm.elem = create_math_op(node->left->token, node->token, node->right->token);
        break;
    }
    case ASSIGN : {
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility

        node->token->llvm.elem = create_assign(node->left->token, node->right->token);
        break;
    }
    case FDEC: {
        enter_scope(node);

        Token *token = node->token;
        if (token->llvm.func_type)
            return;
        create_function(node->token);
        create_entry(token);

        // extract parameters as variables

        // code gen children
        for (size_t i = 0; i < node->children_count; i++)
            code_gen(node->children[i]);

        exit_scope();
        break;
    }
    case RETURN: {
        code_gen(node->left);
        node->token->llvm.elem = create_return(node->left->token);
        break;
    }
    default:
        break;
    }
}

void init_module(char *name) {
    ura.context = LLVMContextCreate();
    ura.module = LLVMModuleCreateWithNameInContext(name, ura.context);
    ura.builder = LLVMCreateBuilderInContext(ura.context);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    // LLVMInitializeNativeAsmParser(); // TODO: to be checked

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(ura.module, triple);
    Target target;
    if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
        TargetMachine machine =
            LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault,
                                    LLVMRelocDefault, LLVMCodeModelDefault);
        TargetData layout = LLVMCreateTargetDataLayout(machine);
        LLVMSetModuleDataLayout(ura.module, layout);
        LLVMDisposeTargetData(layout);
        LLVMDisposeTargetMachine(machine);
    }
    LLVMDisposeMessage(triple);

    // TODO: add asan stuff here
    // TODO: add flags stuff (Passes)
}

void finalize_module(char *ll_path) {
}

void generate_asm(void) {
    enter_scope(ura.ast);
    init_module("ura-module");

    for (size_t i = 0; i < ura.ast->children_count; i++) {
        Node *node = ura.ast->children[i];
        code_gen(node);
    }
    exit_scope();

    char *error = NULL;
    // PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
    // if (ura.flags) {
    //     Error err = LLVMRunPasses(ura.module, ura.flags, NULL, opts);
    //     if (err) {
    //         char *msg = LLVMGetErrorMessage(err);
    //         CHECK(1, "optimizer error: %s", msg);
    //         LLVMDisposeErrorMessage(msg);
    //     }
    // }
    // if (ura.debug_builder) {
    //     LLVMDIBuilderFinalize(ura.debug_builder);
    //     LLVMDisposeDIBuilder(ura.debug_builder);
    //     ura.debug_builder = NULL;
    // }
    if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
        eprint("module verification failed:\n%s", error);
    LLVMDisposeMessage(error);
    // LLVMDisposePassBuilderOptions(opts);
    LLVMPrintModuleToFile(ura.module, "build/out.ll", NULL);
}

/*
TODO:
    + start creating an abstraction on top of llvm
    + implement math operation on variables
    + function returns variable
    + function takes parameters
*/

void print_nodes(char *text) {
    print(text);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        print_node(ura.ast->children[i]);
}

int main() {
    new_file(strdup("./file.ura"));
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        tokenize(file->content);
        generate_ast();
        print_nodes(GREEN("============AST=================\n"));
        generate_ir();
        print_nodes(GREEN("============IR==================\n"));
        generate_asm();
        print_nodes(GREEN("============ASM==================\n"));
        print(GREEN("============CLEANING============\n"));
        ura_clean();
    }
    free(ura.tokens);
    free(ura.files);
}
