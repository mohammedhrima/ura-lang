#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
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
#include <assert.h>

#ifndef GEN_IR
#    define GEN_IR 1
#endif
#ifndef GEN_ASM
#    define GEN_ASM 1
#endif

typedef struct uraFile uraFile;
typedef enum Type Type;
typedef struct Token Token;
typedef struct Node Node;
typedef struct Ura Ura;
typedef struct Arena Arena;
typedef struct NodePrint NodePrint;

// begin LLVM code
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

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Bloc;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTargetRef Target;
typedef LLVMTargetMachineRef TargetMachine;
typedef LLVMCodeGenOptLevel CodeGenOptLevel;
typedef LLVMRelocMode RelocMode;
typedef LLVMCodeModel CodeModel;
typedef LLVMTypeKind TypeKind;
typedef LLVMAttributeRef AttributeRef;
typedef LLVMMetadataRef MetadataRef;
#if LLVM_VERSION_MAJOR >= 13
typedef LLVMErrorRef Error;
typedef LLVMPassBuilderOptionsRef PassBuilderOptions;
#endif

#define PointerType         LLVMPointerTypeKind
#define IntegerType         LLVMIntegerTypeKind
#define FloatType           LLVMFloatTypeKind
#define DoubleType          LLVMDoubleTypeKind
#define VoidType            LLVMVoidTypeKind
#define FunctionType        LLVMFunctionTypeKind
#define StructType          LLVMStructTypeKind

#define CodeGenLevelDefault LLVMCodeGenLevelDefault
#define RelocDefault        LLVMRelocDefault
#define CodeModelDefault    LLVMCodeModelDefault

typedef struct ASM ASM;
struct ASM {
    // bool is_set;
    // bool is_loaded;

    Value elem;
    Bloc bloc;
    // TypeRef func_type;
    TypeRef type;

    // while loop stuff
    Bloc cond;
    Bloc then;
    Bloc end;
};

void asm_init(char *name);
void asm_finalize(char *ll_path);
TypeRef get_llvm_type(Type type);
TypeRef get_data_type(Node *type);
Value create_alloca(TypeRef type, char *name);
Value copy_to_stack(Value constant);
Value create_variable(Node *node);
Value create_value(Node *node);
Value create_load(Node *node);
void create_struct(Node *node);
Value create_bin_op(Node *node);
Value address_of(Node *node);
Value create_assign(Node *left, Node *right);
void create_function(Node *node);
void create_entry(Node *node);
Value create_param(Node *fn, Node *param, size_t pos);
Value create_function_call(Node *node);
Value create_return(Node *node);
void create_default_return(Node *node);
Value get_parent_bloc();
Bloc create_label(char *name);
void create_jmp_condition(Value cond, Bloc then, Bloc next);
bool is_bloc_terminated();
void create_jmp_out(Bloc bloc);
void create_jmp(Bloc bloc);
void create_at(Bloc bloc);
void create_last_label(Bloc bloc);
Value create_array(Node *node);
Value create_sizeof(Node *node);
// end LLVM code

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
#define DIM(fmt)   "\033[2m" fmt RESET
// #define BLUE(fmt)  BOLD "\033[34m" fmt RESET
// #define YELLOW(fmt) BOLD "\033[0;33m" fmt RESET

#define expand(type, variable) \
    type *variable;            \
    size_t variable##_count;   \
    size_t variable##_size;

#define push_back(parent, child)                                          \
    {                                                                     \
        if (parent##_size == 0) /* empty */                               \
            parent = ura_alloc((parent##_size = 10), sizeof(*parent));    \
        else if (parent##_count + 1 == parent##_size) {                   \
            void *tmp = ura_alloc((parent##_size *= 2), sizeof(*parent)); \
            memcpy(tmp, parent, (parent##_count * sizeof(*parent)));      \
            parent = tmp;                                                 \
        }                                                                 \
        parent[parent##_count++] = child;                                 \
    }

#define eprint(...)          _eprint(FILE, FUNC, LINE, __VA_ARGS__)
#define error_at(token, ...) _error_at(FILE, FUNC, LINE, token, format(__VA_ARGS__))
#define help(...)            _help(format(__VA_ARGS__))

struct uraFile {
    char *name;
    char *dir;
    char *base;
    char *build_dir;
    char *ll_path;
    size_t len;
    char *content;
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
// clang-format on

struct Token {
    Type type;
    Type ret_type;

    bool is_type;
    bool is_variadic;
    size_t space;
    size_t line;
    size_t s, e;

    ASM llvm;

    int used;

    struct {
        char *name;
        char *asm_name;
        struct {
            long value;
        } i32;
        struct {
            int value; // TODO: to be used to unicode stuff
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

struct Arena {
    unsigned char *buf;
    size_t size;
    size_t used;
    Arena *next;
};

struct NodePrint {
    expand(Node *, nodes);
    expand(int, depths);
};

void *ura_alloc(size_t count, size_t size);
void arena_reserve(size_t bytes);
void free_arena(void);
void new_file(char *name);
char *ura_strdup(char *str);
char *strjoin(char *first, ...);
Node *parse_type(void);
Node *clone_node(Node *node, Node *template, Node *args, char *suffix);
const char *to_string(Type type);
int _print(File fp, const char *fmt, va_list args);
int _eprint(char *file, const char *func, int line, char *fmt, ...);
void _error_at(char *file, const char *func, int line, Token *token, char *message);
void _help(char *message);
char *format(char *fmt, ...);
void diag_flush(void);
bool assert_function_matches_call(Node *call);
bool assert_builtin_takes_one(Node *call);
Node *find_variable(char *name);
Node *find_type_by_name(char *name);
Node *type_of(Node *node);
bool assert_type_fits(Node *expected, Node *value, bool report);
void assert_condition_is_bool(Node *cond);
bool assert_operands_are_valid(Node *node);
bool assert_name_is_declared(Node *node);
bool assert_not_declared_twice(Node *node);
bool assert_indexing_an_array(Node *node);
bool assert_receiver_is_struct(Node *node);
bool assert_attribute_exists(Node *node, Node *struct_dec);
bool assert_address_of_variable(Node *node);
bool assert_assignment_is_valid(Node *node, bool bare_name);
bool assert_operator_fits_operands(Node *node);
bool assert_return_matches_function(Node *node, Node *fn);
bool assert_inside_loop(Node *node);
bool assert_is_struct_or_func(Token *token);
bool assert_templates_param(Token *token);
bool assert_template_args_match(Node *template, Token *name, Node *args);
bool assert_declaration_is_valid(Node *parent, size_t i);
bool same_params(Node *left, Node *right);
Token *new_token(Type type, Token *from);
Node *new_node(Token *token);
Node *recover(Token *start);
Token *assert_next_is(Type type, char *message);
bool assert_character_is_known(Token *token);
bool assert_token_is_expected(Token *token);
bool assert_ref_is_single(void);
bool assert_type_follows(Node *type, Token *after);
bool assert_type_is_known(Node *type);
bool assert_struct_not_inside_itself(Node *type);
bool assert_array_type_is_closed(Token *open, Node *type);
bool assert_arguments_are_separated(Token *call);
bool assert_access_is_closed(Node *access);
bool assert_array_literal_valid(Node *literal);
bool assert_is_struct_member(Node *attr);
bool assert_address_is_single(Token *amp);
bool assert_parenthesis_is_closed(Token *open);
bool assert_variadic_is_last(Node *arg);
bool assert_parameter_has_type(Node *arg);
bool assert_function_parameters_are_separated(void);
bool assert_template_parameters_are_separated(void);
bool assert_condition_follows(Token *keyword);
bool assert_else_has_no_condition(Token *keyword);
bool assert_operand_follows(Token *op);
bool assert_type_is_printable(Node *value, char *spec);
bool assert_call_is_unambiguous(Node *call, Node *found);
Node *is_data_type(Token *token);
bool assert_literal_is_valid(Token *literal, bool closed);
bool assert_number_fits(Token *number, Type type, bool report);
Token *peek(size_t index);
Token *next(void);
int print(char *fmt, ...);
bool includes(Type to_find, ...);
Node *prime_node(void);
Node *expr_node(int min_op);
void enter_scope(Node *node);
void exit_scope(void);
void code_gen(Node *node);
void gen_body(Node *node);
int print_node(File fp, Node *node);
void print_nodes(char *text);

struct Ura {
    int errors_count;
    char *error_file;
    const char *error_func;
    int error_line;
    expand(uraFile *, files);
    expand(Token *, tokens);
    expand(Node *, nodes);
    expand(Node *, scopes);
    Node *scope;

    char *curr_content;
    uraFile *curr_file;
    size_t curr_line;
    size_t exe_pos;

    Node *ast;

    Context context;
    Module module;
    Builder builder;
    char *exec;

    Arena *arena_head;
    Arena *arena_curr;
    size_t arena_index;

    size_t heap_size;
};

extern Ura ura;

Ura ura;

// ============================= tokenizer helpers =============================

Token *new_token(Type type, Token *from) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    if (from) {
        new->space = from->space;
        new->line = from->line;
        new->s = from->s;
        new->e = from->e;
    }
    push_back(ura.tokens, new);
    return new;
}

// TODO: parse for char type also
char *parse_escaped(char *input, size_t s, size_t e) {
    char *res = ura_alloc(e - s + 1, sizeof(char));
    size_t r = 0;
    while (s < e) {
        int c = -1;
        // clang-format off
        if(s < e && input[s] == '\\')
        {
            switch(input[s + 1]) {
                case 'n':  c = '\n'; break;
                case 't':  c = '\t'; break;
                case 'r':  c = '\r'; break;
                case 'b':  c = '\b'; break;
                case 'f':  c = '\f'; break;
                case 'v':  c = '\v'; break;
                case 'a':  c = '\a'; break;
                case '\\': c = '\\'; break;
                case '"':  c = '\"'; break;
                case '\'': c = '\''; break;
                case '?':  c = '\?'; break;
                case '0':  c = '\0'; break;
                // TODO: add octal stuff etc...
                default: break;
            }
            if(c != -1) s += 2;
            else c = input[s++];
        }
        else c = input[s++];
        // clang-format on
        res[r++] = c;
    }
    return res;
}

Token *parse_token(Type type, size_t s, size_t e, size_t space) {
    Token *new = new_token(type, NULL);
    new->space = space / TAB + (space % TAB != 0 ? 1 : 0);
    space = new->space;


    switch (type) {
    case ID: {
        // if(e - s == 0) break;
        struct {
            char *value;
            Token token;
        } keywords[] = {
            // clang-format off
            { "b1", { .type = BOOL, .is_type = true } },
            { "char", { .type = I8, .is_type = true } },
            { "i8", { .type = I8, .is_type = true } },
            { "i32", { .type = I32, .is_type = true } },
            { "template", { .type = TEMPLATE_DEC, .is_type = true }},

            { "True", { .type = BOOL, .b1 = { .value = true } } },
            { "False", { .type = BOOL, .b1 = { .value = false } } },

            { "struct", { .type = STRUCT_DEC } },
            { "and", { .type = AND } }, { "or", { .type = OR } },
            { "proto", { .type = PROTO } }, { "fn", { .type = FN_DEC } },
            { "return", { .type = RETURN } },
            { "if", { .type = IF } }, { "elif", { .type = ELIF } },
            { "else", { .type = ELSE } },
            { "while", { .type = WHILE } }, { "break", { .type = BRK } },
            { "continue", { .type = CNT } },
            { "null", {.type = NULL_ } },

            { NULL },
            // clang-format on
        };
        size_t i = 0;
        for (; keywords[i].value; i++) {
            char *value = keywords[i].value;
            size_t len = strlen(value);
            if (e - s == len && strncmp(ura.curr_content + s, value, e - s) == 0) {
                *new = keywords[i].token;
                new->space = space;
                break;
            }
        }
        if (keywords[i].value)
            break;

        new->name = ura_alloc(e - s + 1, sizeof(char));
        strncpy(new->name, ura.curr_content + s, e - s);
        break;
    }
    case CHARS: {
        new->chars.value = parse_escaped(ura.curr_content, s + 1, e - 1);
        break;
    }
    case I8: {
        new->i8.value = parse_escaped(ura.curr_content, s + 1, e - 1)[0];
        break;
    }
    case I32: {
        new->i32.value = strtol(ura.curr_content + s, NULL, 10); // TODO: INT_MIN
        break;
    }
    default:
        break;
    }
    new->line = ura.curr_line;
    new->s = s;
    new->e = e;
    print("new token %k\n", new);
    return new;
}

// ================================= tokenizer =================================

void gen_tokens(uraFile *file) {
    ura.curr_content = file->content;
    ura.curr_file = file;
    ura.curr_line = 1;
    char *content = file->content;
    arena_reserve(strlen(content) * 192);

    size_t s = 0;
    size_t e = 0;
    size_t space = 0;
    while (content && content[e]) {
        s = e;
        if (isspace(content[e])) {
            while (content[e] == '\n') {
                space = 0;
                ura.curr_line++;
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
        while (strncmp(content + s, "/*", 2) == 0 && content[e] &&
               strncmp(content + e, "*/", 2) != 0)
            e++;
        if (e != s) {
            e += 2;
            continue;
        }

        while (isalpha(content[s]) && (isalnum(content[e]) || content[e] == '_'))
            e++;
        if (e != s) // found identifier
        {
            parse_token(ID, s, e, space);
            continue;
        }

        if (content[s] == '"' || content[s] == '\'') {
            char quote = content[s];
            e++;
            while (content[e] && content[e] != quote && content[e] != '\n') {
                if (content[e] == '\\' && content[e + 1] && content[e + 1] != '\n')
                    e++;
                e++;
            }
            bool closed = content[e] == quote;
            if (closed)
                e++;
            Token *literal = parse_token(quote == '"' ? CHARS : I8, s, e, space);
            assert_literal_is_valid(literal, closed);
            continue;
        }

        // bool is_float = false;
        while (isdigit(content[e]))
            e++;
        if (e != s) {
            assert_number_fits(parse_token(I32, s, e, space), I32, true);
            continue;
        }
        struct {
            char *value;
            Type type;
        } specials[] = {
            // clang-format off
            {"(", LPARENT}, {")", RPARENT}, {":", DOTS},
            {"[", LBRACK}, {"]", RBRACK},
            {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},
            {"/=", DIV_ASSIGN}, {"%=", MOD_ASSIGN},
            {"...", VARIADIC}, {".", DOT},
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {">=", GE}, {"<=", LE}, {">", GT}, {"<", LT},
            {"==", EQ}, {"!=", NQ},
            {"=", ASSIGN}, {",", COMA},
            {"&&", AND}, {"||", OR},
            {"&", REF},
            {NULL, NONE}
            // clang-format on
        };
        for (int i = 0; specials[i].value; i++) {
            size_t len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                parse_token(specials[i].type, e, e + len, space);
                if (specials[i].type == DOTS)
                    space += TAB;
                e += len;
            }
        }
        if (e != s)
            continue;
        assert_character_is_known(parse_token(I8, s, e, space));
        break;
    }
    parse_token(END, e, e, 0);
}

// ================================ ast helpers ================================

bool inside(size_t space) {
    return ura.tokens[ura.exe_pos]->space > space;
}

Token *peek(size_t index) {
    if (ura.exe_pos + index > ura.tokens_count) {
        eprint("index out of range\n");
        return NULL;
    }
    return ura.tokens[ura.exe_pos + index];
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
    push_back(ura.nodes, new);
    return new;
}

void parse_bloc(Node *parent) {
    while (inside(parent->token->space)) {
        Node *child = expr_node(0);
        if (child->token->type == ASSIGN && child->left->token->type == VAR_DEC) {
            Node *dec = child->left;
            push_back(parent->children, dec);
            child->left = dec->left;
        }
        push_back(parent->children, child);
    }
}

// TODO: to be cheked
Node *find_variable(char *name) {
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        if (scope->token->type == STRUCT_DEC) // attributes only through self
            continue;
        Node *args = NULL;
        if (includes(scope->token->type, FN_DEC, PROTO, 0))
            args = scope->left;
        size_t params = 0;
        if (args)
            params = args->children_count;

        for (size_t j = 0; j < params + scope->children_count; j++) {
            Node *dec = NULL;
            if (j < params)
                dec = args->children[j];
            else
                dec = scope->children[j - params];
            if (dec->token->type != VAR_DEC)
                continue;
            Node *var = dec->left;
            if (var->token->type != VAR)
                continue;
            if (strcmp(var->token->name, name) == 0)
                return var;
        }
    }
    return NULL;
}

// TODO: check the part were we manage
// that struct should not have it's own as attribute
Node *find_in_children(Node *parent, char *name) {
    for (size_t j = 0; j < parent->children_count; j++) {
        Node *curr = parent->children[j];
        if (!includes(curr->token->type, STRUCT_DEC, TEMPLATE_TYPE, 0))
            continue;
        if (strcmp(curr->token->name, name) == 0)
            return curr;
    }
    return NULL;
}

Node *find_type_by_name(char *name) {
    expand(Node *, nodes);
    nodes = NULL;
    nodes_count = 0;
    nodes_size = 0;


    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        // I' inside a struct, can hold a ref to itself
        if (scope->token->type == STRUCT_DEC) {
            if (strcmp(scope->token->name, name) == 0)
                return scope;
            continue;
        }
        // TODO: check with lot of templates
        // I'm inside a template
        Node *parent = scope;
        if (scope->token->type == TEMPLATE_DEC) {
            // check templates params: T, ...
            parent = scope->left;
        }
        for (size_t j = 0; j < parent->children_count; j++) {
            Node *curr = parent->children[j];

            if (curr->token->type == TEMPLATE_DEC) {
            } else {
                if (includes(curr->token->type, STRUCT_DEC, TEMPLATE_TYPE, 0) == false)
                    continue;
                if (strcmp(curr->token->name, name) == 0)
                    return curr;
            }
        }
    }
    return NULL;
}

Node *is_data_type(Token *token) {
    if (token->is_type && includes(token->type, BOOL, I8, I32, 0))
        return new_node(token);
    if (token->type == ID)
        return find_type_by_name(token->name);
    return NULL;
}

Node *find_template(char *name) {
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *curr = scope->children[j];
            if (curr->token->type != TEMPLATE_DEC)
                continue;
            for (size_t k = 0; k < curr->children_count; k++) {
                if (strcmp(curr->children[k]->token->name, name) == 0)
                    return curr;
            }
        }
    }
    return NULL;
}

// TODO: to be checked
Node *clone_link(Node *node, Node *template, Node *args, char *suffix) {
    if (!node)
        return NULL;
    if (!includes(node->token->type, STRUCT_DEC, FN_DEC, PROTO, 0))
        return clone_node(node, template, args, suffix);
    for (size_t i = 0; i < template->children_count; i++) {
        if (template->children[i] != node)
            continue;
        char *name = strjoin(node->token->name, suffix, NULL);
        return find_in_children(ura.ast, name); // Vec -> Vec<i32>
    }
    return node; // declared outside: share it
}

Node *clone_node(Node *node, Node *template, Node *args, char *suffix) {
    if (!node)
        return NULL;
    if (includes(node->token->type, TEMPLATE_TYPE, ID, 0)) {
        Node *params = template->left;
        for (size_t i = 0; i < params->children_count; i++) {
            if (i >= args->children_count)
                break;
            if (strcmp(params->children[i]->token->name, node->token->name) == 0)
                return args->children[i]; // T -> i32
        }
    }
    Node *copy = new_node(new_token(node->token->type, node->token));
    *copy->token = *node->token;
    if (node->token->type == FN_CALL) {
        for (size_t i = 0; i < template->children_count; i++) {
            if (strcmp(template->children[i]->token->name, node->token->name) != 0)
                continue;
            copy->token->name = strjoin(node->token->name, suffix, NULL);
            break;
        }
    }
    copy->left = clone_link(node->left, template, args, suffix);
    copy->right = clone_link(node->right, template, args, suffix);
    for (size_t i = 0; i < node->children_count; i++)
        push_back(copy->children,
                  clone_node(node->children[i], template, args, suffix));
    return copy;
}

// TODO: to be chekcked
Node *instantiate_template(Node *node, Token *name, Node *args) {
    char *suffix = ura_strdup("<");
    for (size_t i = 0; i < args->children_count; i++) {
        if (i > 0)
            suffix = strjoin(suffix, ",", NULL);
        suffix = strjoin(suffix, format("%N", args->children[i]), NULL);
    }
    suffix = strjoin(suffix, ">", NULL);

    char *key = strjoin(name->name, suffix, NULL); // Vec<i32>
    Node *found = find_in_children(ura.ast, key);
    if (found) // already instantiated
        return found;

    Node *instance = NULL;
    for (size_t i = 0; i < node->children_count; i++) { // the empty clones
        Node *dec = node->children[i];
        Node *copy = new_node(new_token(dec->token->type, dec->token));
        *copy->token = *dec->token;
        copy->token->name = strjoin(dec->token->name, suffix, NULL);
        if (dec->token->type == PROTO) // keeps the symbol it links against
            copy->token->asm_name = dec->token->name;
        push_back(ura.ast->children, copy);
        if (strcmp(copy->token->name, key) == 0)
            instance = copy;
    }
    for (size_t i = 0; i < node->children_count; i++) { // then their bodies
        Node *dec = node->children[i];
        Node *copy =
            ura.ast->children[ura.ast->children_count - node->children_count + i];
        copy->left = clone_link(dec->left, node, args, suffix);
        copy->right = clone_link(dec->right, node, args, suffix);
        for (size_t j = 0; j < dec->children_count; j++) {
            Node *child = clone_node(dec->children[j], node, args, suffix);
            push_back(copy->children, child);
        }
    }
    return instance;
}

Node *parse_template_init(Node *node, Token *name) {
    if (!assert_next_is(LT, format("expected '<' after '%K'", name)))
        return recover(name);

    Node *args = new_node(new_token(ARGS, name)); // instance args
    while (!includes(peek(0)->type, GT, END, 0)) {
        Node *arg = parse_type();
        if (arg == NULL)
            return recover(name);
        push_back(args->children, arg);
        if (peek(0)->type == COMA) // TODO: add assertion here
            next();
    }
    if (!assert_next_is(GT, format("expected '>' after '%K'", name)))
        return recover(name);
    if (!assert_template_args_match(node, name, args))
        return new_node(new_token(ERR, name));
    return instantiate_template(node, name, args);
}

Node *parse_type(void) {
    if (!assert_ref_is_single()) // assert no & & or &&
        return recover(peek(0));

    if (peek(0)->type == REF) {
        Node *node = new_node(next());
        node->left = is_data_type(peek(0));
        // assert there is type after &
        if (!assert_type_follows(node->left, node->token))
            return new_node(new_token(ERR, node->token));
        next();
        return node;
    }

    Node *template = NULL;
    if (peek(0)->type == ID)
        template = find_template(peek(0)->name);

    Node *type = NULL;
    if (template) {
        type = parse_template_init(template, next());
    } else {
        type = is_data_type(peek(0));

        if (!assert_type_is_known(type)) {
            return new_node(new_token(ERR, next()));
        }
        if (!type)
            return NULL;

        if (!assert_struct_not_inside_itself(type))
            return new_node(new_token(ERR, next()));

        next();
    }

    while (peek(0)->type == LBRACK) { // i32[][]
        Token *open = next();
        if (!assert_array_type_is_closed(open, type)) // [][][]
            return recover(open);
        Node *array = new_node(new_token(ARRAY, type->token));
        array->token->is_type = true;
        array->left = type;
        type = array;
    }
    return type;
}

// ==================================== ast ====================================

Node *prime_node(void) {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case ID: {
        Node *next_elem = parse_type();
        if (next_elem) {
            node = new_node(new_token(VAR_DEC, token));
            // token->type = VAR;
            node->left = new_node(token);
            node->left->left = next_elem;
            return node;
        }
        if (peek(0)->type == LT) // foo<i32>(1)
        {
            Node *template = find_template(token->name);
            if (template) {
                Node *instance = parse_template_init(template, token);
                if (instance->token->type == ERR)
                    return instance;
                token->name = instance->token->name;
            }
        }
        if (peek(0)->type == LPARENT) // FN_CALL
        {
            next();
            token->type = FN_CALL;
            node = new_node(token);
            node->left = new_node(new_token(ARGS, node->token));
            while (peek(0)->type != RPARENT) {
                Node *arg = expr_node(0);
                push_back(node->left->children, arg);
                if (!assert_arguments_are_separated(token))
                    return recover(token);
            }
            next();
            return node;
        }
        if (peek(0)->type == LBRACK) // str[0]
        {
            Node *node = new_node(token);
            while (peek(0)->type == LBRACK) {
                Node *access = new_node(next()); // skip [
                access->token->type = ACCESS;
                access->left = node;
                access->right = expr_node(0);
                if (!assert_access_is_closed(access))
                    return recover(token);
                node = access;
            }
            return node;
        }

        return new_node(token);
    } // clang-format off
    // values
    case CHARS: {
        Token *next_token = peek(0);
        while(next_token->type == CHARS) {
            char *value = strjoin(token->chars.value, next_token->chars.value, NULL);
            token->chars.value = value;
            next();
            next_token = peek(0);
        }
        return new_node(token);
    }
    case NULL_: case BOOL: case I8: 
    case I32: case VARIADIC: { // clang-format on
        return new_node(token);
    }
    case LBRACK: {
        Node *node = new_node(token);
        node->token->type = ARRAY_LIT;
        while (peek(0)->type != RBRACK) {
            if (!assert_array_literal_valid(node))
                return recover(token);
            if (peek(0)->type == COMA)
                next();
            push_back(node->children, prime_node());
        }
        next();
        return node;
    }
    case TEMPLATE_DEC: {
        Node *node = new_node(token);
        node->token->is_type = true;
        enter_scope(node); // enter template scope

        if (!assert_next_is(LT, format("expected '<' after '%K'", token))) {
            exit_scope();
            return recover(token);
        }
        node->left = new_node(new_token(ARGS, node->token)); // <A,B,C>
        while (!includes(peek(0)->type, GT, 0)) {
            Node *arg = prime_node();
            if (!assert_templates_param(arg->token)) {
                exit_scope();
                return recover(token);
            }
            arg->token->type = TEMPLATE_TYPE;
            push_back(node->left->children, arg);
            if (!assert_template_parameters_are_separated()) {
                exit_scope();
                return recover(token);
            }
        }
        if (!assert_next_is(GT, format("expected '>' after '%K'", token))) {
            exit_scope();
            return recover(token);
        }
        if (!assert_next_is(DOTS, format("expected ':' after '%K'", token))) {
            exit_scope();
            return recover(token);
        }

        while (inside(node->token->space)) {
            Node *child = prime_node();
            if (!assert_is_struct_or_func(child->token)) {
                exit_scope();
                return recover(token);
            }
            push_back(node->children, child);
        }
        exit_scope();
        return node;
    }
    case STRUCT_DEC: {
        node = new_node(token);

        node->token->is_type = true;
        Token *name = assert_next_is(ID, "expected a struct name after 'struct'");
        if (!name)
            return recover(token);
        node->token->name = ura_strdup(name->name);
        node->token->s = name->s;
        node->token->e = name->e;
        char *colon = format("expected ':' after 'struct %s'", name->name);
        if (!assert_next_is(DOTS, colon))
            return recover(token);
        enter_scope(node);
        while (inside(node->token->space)) {
            Node *attr = expr_node(0);
            if (assert_is_struct_member(attr))
                push_back(node->children, attr);
        }
        exit_scope();
        return node;
    } // clang-format off
    case AND: case REF: { // clang-format on
        if (!assert_address_is_single(token)) {
            expr_node(40);
            return new_node(new_token(ERR, token));
        }
        token->type = OWN;
        node = new_node(token);
        node->left = expr_node(40); // TODO: to be checked
        return node;
    }
    case LPARENT: {
        node = expr_node(0);
        assert_parenthesis_is_closed(token);
        return node;
    } // clang-format off
    case SUB: case ADD: { // clang-format on
        node = new_node(token);
        node->right = prime_node();
        node->left = new_node(new_token(I32, node->token));
        node->left->token->i32.value = token->type == SUB ? -1 : 1;
        node->token->type = MUL;
        return node;
    } // clang-format off
    case PROTO: case FN_DEC: { // clang-format on
        Node *struct_dec = ura.scope->token->type == STRUCT_DEC ? ura.scope : NULL;
        node = new_node(token);
        Token *name = assert_next_is(ID, "expected a function name after 'fn'");
        if (!name)
            return recover(token);
        node->token->name = ura_strdup(name->name);
        node->token->s = name->s;
        node->token->e = name->e;
        enter_scope(node);

        if (!assert_next_is(LPARENT, format("expected '(' after '%s'", name->name))) {
            exit_scope();
            return recover(token);
        }
        node->left = new_node(new_token(ARGS, node->token));
        if (struct_dec) {
            Node *self = new_node(new_token(VAR_DEC, node->token));
            self->left = new_node(new_token(VAR, node->token));
            self->left->token->name = ura_strdup("self");
            self->left->left = new_node(new_token(REF, node->token));
            self->left->left->left = struct_dec;
            push_back(node->left->children, self);
            char *struct_name = struct_dec->token->name;
            node->token->asm_name = strjoin(struct_name, ".", node->token->name, NULL);
        }
        while (!includes(peek(0)->type, RPARENT, 0)) {
            Node *arg = prime_node();
            if (arg->token->type == VARIADIC) {
                // push_back(node->left->children, arg);
                node->token->is_variadic = true;
                if (!assert_variadic_is_last(arg)) {
                    exit_scope();
                    return recover(token);
                }
                break;
            }
            if (!assert_parameter_has_type(arg)) {
                exit_scope();
                return recover(token);
            }
            push_back(node->left->children, arg);
            if (!assert_function_parameters_are_separated()) {
                exit_scope();
                return recover(token);
            }
        }
        next();

        node->right = parse_type();

        if (node->token->type == FN_DEC) { // : and bloc only for fn
            if (!assert_next_is(DOTS, "expected ':' after the function signature")) {
                exit_scope();
                return recover(token);
            }
            parse_bloc(node);
        }

        exit_scope();
        return node;
    }
    case RETURN: {
        node = new_node(token);
        if (peek(0)->line == token->line && peek(0)->type != END)
            node->left = expr_node(0);
        return node;
    }
    case IF: {
        node = new_node(token);
        if (assert_condition_follows(token))
            node->left = expr_node(0);
        else
            node->left = new_node(new_token(ERR, token));
        if (!assert_next_is(DOTS, "expected ':' after the 'if' condition"))
            return recover(token);
        parse_bloc(node);

        Node *curr = node;
        while (inside(node->token->space - 1)) {
            // TODO: space - 1 this might overflow
            // space is size_t
            Token *next_token = peek(0);
            if (!includes(next_token->type, ELIF, ELSE, 0))
                break;

            curr->right = new_node(next());
            curr = curr->right;

            if (curr->token->type == ELIF) {
                curr->left = expr_node(0);
                if (!assert_next_is(DOTS, "expected ':' after the 'elif' condition"))
                    return recover(curr->token);
            } else if (!assert_else_has_no_condition(curr->token))
                return recover(curr->token);
            else if (!assert_next_is(DOTS, "expected ':' after 'else'"))
                return recover(curr->token);
            parse_bloc(curr);
        }
        return node;
    }
    case WHILE: {
        node = new_node(token);
        node->left = expr_node(0);
        if (!assert_next_is(DOTS, "expected ':' after the 'while' condition"))
            return recover(token);
        parse_bloc(node);
        return node;
    } // clang-format off
    case BRK: case CNT:
        // clang-format on
        return new_node(token);
        // clang-format off
    case ELIF: case ELSE: { // clang-format on
        assert_token_is_expected(token);
        return recover(token);
    }
    default: {
        assert_token_is_expected(token);
        return new_node(new_token(ERR, token));
    }
    }
    return NULL;
}

Node *expr_node(int min_op) {
    Node *left = prime_node();
    while (true) {
        int prec[END + 1] = {
            // clang-format off
            [ASSIGN] = 1,
            [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
            [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1,
            [MOD_ASSIGN] = 1,

            [OR] = 11, [AND] = 12,

            [EQ] = 21, [NQ] = 21,
            [GT] = 22, [LT] = 22,
            [GE] = 22, [LE] = 22,

            [ADD] = 30, [SUB] = 30,
            [MUL] = 31, [DIV] = 31, [MOD] = 31,

            [DOT] = 41,
            // clang-format on
        };
        int op = prec[peek(0)->type];
        if (op <= min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        if (!assert_operand_follows(node->token)) {
            node->token->type = ERR;
            return node;
        }
        node->right = expr_node(op);
        // TODO: to be cheked
        if (node->token->type == DOT && node->right->token->type == ACCESS) {
            Node *outer = node->right; // v.arr[1] is (v.arr)[1], not v.(arr[1])
            Node *inner = outer;
            while (inner->left->token->type == ACCESS)
                inner = inner->left;
            node->right = inner->left;
            inner->left = node;
            node = outer;
        }

        left = node;
    }
    return left;
}

void gen_ast(void) {
    if (ura.errors_count)
        return;
    ura.ast = new_node(new_token(ID, NULL));
    ura.ast->token->name = ura_strdup("ura-scope");
    enter_scope(ura.ast);
    while (!includes(peek(0)->type, END, 0)) {
        Node *child = expr_node(0);
        push_back(ura.ast->children, child);
    }
    exit_scope();
}

// =============================== gen ir helpers ===============================

void enter_scope(Node *node) {
    Token *token = node->token;
    print("%s: %t %s\n", CYAN("enter scope"), token->type, token->name);
    ura.scope = node;
    push_back(ura.scopes, node);
}

void exit_scope() {
    Token *token = ura.scope->token;
    print("%s: %t %s\n", CYAN("exit scope"), token->type, token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count == 0)
        ura.scope = NULL;
    else
        ura.scope = ura.scopes[ura.scopes_count - 1];
}

Node *type_of(Node *node) {
    switch (node->token->type) {
    case VAR:
        return node->left;
    case VAR_LOAD:
        return node->left->left;
    case DOT: {
        Node *struct_dec = type_of(node->left);
        size_t index = node->right->token->i32.value;
        return struct_dec->children[index]->left->left;
    } // clang-format off
    case FN_CALL: return node->right->right;
    case OWN: { // clang-format on
        if (node->right == NULL) {
            node->right = new_node(new_token(REF, node->token));
            node->right->left = type_of(node->left);
        }
        return node->right;
    }
    case ACCESS:
    case DREF: {
        return type_of(node->left)->left;
    } // clang-format off
    case NULL_: return node->left ? node->left : node;
    case REF: case ARRAY: case STRUCT_DEC: return node;
    case BOOL: case I8: case I32: {
        return node;
    }
    case CHARS: { // TODO: refacor hit part later on
        static Node *node;
        if (node == NULL) {
            node = new_node(new_token(ARRAY, NULL));
            node->token->is_type = true;
            node->left = new_node(new_token(I8, NULL));
            node->left->token->is_type = true;
        }
        return node;
    }
    case ADD: case SUB: case MUL: case DIV: case MOD: {
        return type_of(node->left); // TODO: to be checked later
    }
    // TODO: to be checked
    case SIZEOF: {
        static Node *i32;
        if (i32 == NULL) {
            i32 = new_node(new_token(I32, NULL));
            i32->token->is_type = true;
        }
        return i32;
    }
    case GT: case LT: case GE: case LE: case EQ: case NQ: case AND: case OR: {
        static Node *node;
        if (node == NULL) {
            node = new_node(new_token(BOOL, NULL));
            node->token->is_type = true;
        }
        return node;
    } // clang-format on
    case ARRAY_LIT: {
        if (!node->left) {
            node->left = new_node(new_token(ARRAY, node->token));
            node->left->token->is_type = true;
            if (node->children_count)
                node->left->left = type_of(node->children[0]);
        }
        return node->left;
    }
    default: {
        eprint("handle this case %t", node->token->type);
        exit(1);
        break;
    }
    }
    return NULL;
}

bool same_params(Node *left, Node *right) {
    if (left->token->is_variadic != right->token->is_variadic)
        return false;
    if (left->left->children_count != right->left->children_count)
        return false;
    for (size_t i = 0; i < left->left->children_count; i++) {
        // ARGS -> VAR_DEC -> VAR -> type
        Node *lchild = left->left->children[i]->left->left;
        Node *rchild = right->left->children[i]->left->left;
        if (!assert_type_fits(lchild, rchild, false))
            return false;
    }
    return true;
}

bool args_fit(Node *fdec, Node *call) {
    size_t want = fdec->left->children_count;
    size_t got = call->left->children_count;
    if (fdec->token->is_variadic ? got < want : got != want)
        return false;
    for (size_t i = 0; i < want; i++) {
        Node *param = fdec->left->children[i]->left->left;
        Node *arg = call->left->children[i];
        if (!assert_type_fits(param, arg, false))
            return false;
    }
    return true;
}

// TODO: later on gave her fd as parmeter to be used in errput
Node *output_function(Node *call) {
    static Node *node;
    if (node == NULL) {
        node = new_node(new_token(PROTO, 0));
        node->left = new_node(new_token(ARGS, 0));
        node->token->name = ura_strdup("output");
        node->token->asm_name = ura_strdup("printf");

        Node *fmt = new_node(new_token(VAR, 0));
        fmt->token->name = ura_strdup("fmt");
        fmt->left = type_of(new_node(new_token(CHARS, 0)));

        Node *var_dec = new_node(new_token(VAR_DEC, 0));
        var_dec->left = fmt;
        push_back(node->left->children, var_dec);

        node->token->is_variadic = true;

        push_back(ura.ast->children, node);
    }

    // TODO: handle struct type
    expand(Node *, args);
    args_size = 0;
    args_count = 0;
    args = NULL;

    Node *fmt_arg = new_node(new_token(CHARS, 0));
    fmt_arg->token->chars.value = ura_strdup("");
    push_back(args, fmt_arg);

    char *specs[END + 1] = {
        // clang-format off
        [I8] = "%c", [I32] = "%d", [BOOL] = "%d", [REF] = "%p",
    }; // clang-format on

    for (size_t i = 0; i < call->left->children_count; i++) {
        // TODO: hanlde struct
        // hanlde also '&' (to print th address)
        Node *child = call->left->children[i];
        Node *type = type_of(child);
        Node *elem = type && type->token->type == ARRAY ? type->left : NULL;
        bool is_string = elem && elem->token->type == I8;
        char *spec = is_string ? "%s" : type ? specs[type->token->type] : NULL;
        if (!assert_type_is_printable(child, spec))
            continue;
        char *fmt = fmt_arg->token->chars.value;
        fmt_arg->token->chars.value = strjoin(fmt, spec, NULL);
        push_back(args, child);
    }
    call->left->children = args;
    call->left->children_count = args_count;
    call->left->children_size = args_size;
    return node;
}

char *type_text(Node *type) {
    if (!type)
        return ura_strdup("void");
    switch (type->token->type) {
    case STRUCT_DEC:
        return format("struct.%s", type->token->name);
    case REF:
        return format("&%s", type_text(type->left));
    case ARRAY:
        return format("%s[]", type_text(type->left));
    default:
        return format("%N", type);
    }
}

Node *typeof_function(Node *call) {
    if (!assert_builtin_takes_one(call))
        return NULL;
    call->token->chars.value = type_text(type_of(call->left->children[0]));
    call->token->type = CHARS;
    return NULL;
}

Node *sizeof_function(Node *call) {
    if (!assert_builtin_takes_one(call))
        return NULL;
    call->left = type_of(call->left->children[0]);
    call->token->type = SIZEOF;
    return NULL;
}

// pick the overload in this scope whose parameters match the call
Node *find_function(Node *scope, Node *call) {
    char *name = call->token->name;
    if (strcmp(name, "output") == 0) {
        return output_function(call);
    }
    Node *found = NULL;
    for (size_t i = 0; i < scope->children_count; i++) {
        Node *curr = scope->children[i];
        if (!includes(curr->token->type, FN_DEC, PROTO, 0))
            continue;
        if (strcmp(curr->token->name, name) != 0)
            continue;
        if (!args_fit(curr, call))
            continue;

        // proto printf(str chars, ...)
        // fn printf(str chars):
        // printf("hello")
        if (!assert_call_is_unambiguous(call, found))
            return found;
        found = curr;
    }
    if (!found)
        return NULL;
    for (size_t i = 0; i < found->left->children_count; i++) {
        Node *param = found->left->children[i]->left->left;
        assert_type_fits(param, call->left->children[i], true);
    }
    return found;
}

// =================================== gen ir ===================================

void analyze_ast(Node *node) {
    switch (node->token->type) {
    case ERR: {
        break;
    }
    case ID: {
        node->left = find_variable(node->token->name);
        if (!assert_name_is_declared(node))
            break;
        node->token->type = VAR_LOAD;
        if (node->left->left->token->type == REF) {
            node->token->asm_name = strjoin(node->token->name, ".", "dref", NULL);
            Node *load = new_node(new_token(VAR_LOAD, node->token));
            load->token->name = node->token->name;
            load->left = node->left;

            node->token->type = DREF;
            node->left = load;
        }
        break;
    }
    case VAR_DEC: {
        assert_not_declared_twice(node);
        node->left->token->type = VAR;
        break;
    } // clang-format off
    case VAR:
    case BOOL: case I8: case I32: case CHARS: break; // clang-format on
    case ARRAY_LIT: {
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        break;
    }
    case ACCESS: {
        analyze_ast(node->left);
        analyze_ast(node->right);
        assert_indexing_an_array(node);
        break;
    }
    case TEMPLATE_TYPE:
    case TEMPLATE_DEC: {
        // enter_scope(node);
        // for (size_t i = 0; i < node->left->children_count; i++) {
        //     Node *child = node->left->children[i];
        //     analyze_ast(child);
        // }
        // analyze_ast(node->right);
        // exit_scope();
        break;
    }
    case STRUCT_DEC: {
        enter_scope(node);
        for (size_t i = 0; i < node->children_count; i++) {
            if (!includes(node->children[i]->token->type, FN_DEC, PROTO, 0))
                analyze_ast(node->children[i]);
        }
        for (size_t i = 0; i < node->children_count; i++) {
            if (includes(node->children[i]->token->type, FN_DEC, PROTO, 0))
                analyze_ast(node->children[i]);
        }
        exit_scope();
        break;
    }
    case DOT: {
        analyze_ast(node->left);
        if (!assert_operands_are_valid(node))
            break;
        Node *receiver_type = type_of(node->left);
        if (receiver_type && receiver_type->token->type == REF) {
            Node *deref = new_node(new_token(DREF, node->token));
            deref->left = node->left;
            node->left = deref;
        }
        if (!assert_receiver_is_struct(node))
            break;
        Node *struct_dec = type_of(node->left);
        if (node->right->token->type == FN_CALL) {
            Node *call = node->right;
            Node *receiver = node->left;
            Node *self = new_node(new_token(OWN, node->token));
            self->left = receiver;

            push_back(call->left->children, self);
            for (size_t i = call->left->children_count - 1; i > 0; i--)
                call->left->children[i] = call->left->children[i - 1];
            call->left->children[0] = self;

            for (size_t i = 1; i < call->left->children_count; i++)
                analyze_ast(call->left->children[i]);

            call->right = find_function(struct_dec, call);
            if (!call->right) {
                enter_scope(struct_dec);
                assert_function_matches_call(call);
                exit_scope();
                node->token->type = ERR;
                break;
            }
            node->token = call->token;
            node->left = call->left;
            node->right = call->right;
            break;
        }
        Node *attr = node->right;
        size_t index = 0;
        for (size_t i = 0; i < struct_dec->children_count; i++) {
            Node *child = struct_dec->children[i];
            if (child->token->type != VAR_DEC)
                continue;
            if (strcmp(child->left->token->name, attr->token->name) == 0) {
                attr->token->i32.value = index;
                attr->token->type = ATTR;
                break;
            }
            index++;
        }
        if (!assert_attribute_exists(node, struct_dec))
            break;
        Token *left = node->left->token;
        char *base = left->name ? left->name : left->asm_name;
        char *name = attr->token->name;
        node->token->asm_name = base ? strjoin(base, ".", name, NULL) : name;
        if (type_of(node)->token->type == REF) { // dref left
            Node *dot = new_node(node->token);
            dot->left = node->left;
            dot->right = node->right;
            node->token = new_token(DREF, dot->token);
            node->token->name = dot->token->asm_name;
            node->token->asm_name = strjoin(dot->token->asm_name, ".", "dref", NULL);
            node->left = dot;
            node->right = NULL;
        }
        break;
    }
    case OWN: {
        analyze_ast(node->left);
        if (!assert_operands_are_valid(node))
            break;
        if (node->left->token->type == DREF) {
            Node *load = node->left->left;
            node->token = load->token;
            node->left = load->left;
            node->right = load->right;
            break;
        }
        assert_address_of_variable(node);
        break;
    }
    case DREF: {
        analyze_ast(node->left);
        break;
    }
    case NULL_: {
        break;
    } // clang-format off
    case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
    case DIV_ASSIGN: case MOD_ASSIGN: { // clang-format on
        Type ops[END + 1] = {
            [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
            [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
        };
        Type type = ops[node->token->type];
        if (!type) {
            eprint("handle this case %t", node->token->type);
            exit(1);
        }
        Node *math = new_node(new_token(type, node->token));
        math->left = node->left;
        math->right = node->right;
        analyze_ast(math);
        node->token->type = math->token->type == ERR ? ERR : ASSIGN;
        node->right = math;
        break;
    }
    case ASSIGN: { // clang-format on
        bool rebind = includes(node->right->token->type, OWN, NULL_, 0);
        bool bare_name = node->left->token->type == ID;
        analyze_ast(node->left);
        analyze_ast(node->right);
        if (rebind && node->left->token->type == DREF)
            node->left = node->left->left;
        assert_assignment_is_valid(node, bare_name);
        break;
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD:
    case GT: case LT: case GE: case LE: case EQ: case NQ:
    case AND: case OR: { // clang-format on
        analyze_ast(node->left);
        analyze_ast(node->right);
        assert_operator_fits_operands(node);
        break;
    } // clang-format off
    case PROTO: case FN_DEC: { // clang-format on
        enter_scope(node);
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze_ast(node->left->children[i]);
        // code bloc
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        exit_scope();
        break;
    }
    case FN_CALL: {
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze_ast(node->left->children[i]);
        if (strcmp(node->token->name, "typeof") == 0) {
            typeof_function(node); // becomes a string literal
            break;
        }
        if (strcmp(node->token->name, "sizeof") == 0) {
            sizeof_function(node); // becomes a constant
            break;
        }
        for (size_t i = ura.scopes_count; i > 0 && !node->right; i--) {
            Node *scope = ura.scopes[i - 1];
            if (scope->token->type != STRUCT_DEC)
                node->right = find_function(scope, node);
        }
        assert_function_matches_call(node);
        break;
    }
    case RETURN: {
        Node *fn = NULL;
        for (size_t i = ura.scopes_count; i > 0 && !fn; i--) {
            if (ura.scopes[i - 1]->token->type == FN_DEC)
                fn = ura.scopes[i - 1];
        }
        if (node->left)
            analyze_ast(node->left);
        if (fn)
            assert_return_matches_function(node, fn);
        break;
    }
    case IF: {
        enter_scope(node);
        analyze_ast(node->left);
        assert_condition_is_bool(node->left);
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        Node *curr = node->right;
        while (curr) {
            enter_scope(curr);
            if (curr->left) {
                analyze_ast(curr->left);
                assert_condition_is_bool(curr->left);
            }
            for (size_t i = 0; i < curr->children_count; i++)
                analyze_ast(curr->children[i]);
            curr = curr->right;
            exit_scope();
        }
        exit_scope();
        break;
    }
    case WHILE: {
        enter_scope(node);
        analyze_ast(node->left);
        assert_condition_is_bool(node->left);
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        exit_scope();
        break;
    } // clang-format off
    case BRK: case CNT: {
        // clang-format on
        for (size_t i = ura.scopes_count; i > 0 && !node->left; i--) {
            if (ura.scopes[i - 1]->token->type == WHILE)
                node->left = ura.scopes[i - 1];
        }
        assert_inside_loop(node);
        break;
    }
    default:
        eprint("handle this case %t\n", node->token->type);
        exit(1);
        break;
    }
}

void gen_ir(void) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    // skip last one because it's ura scope
    // TODO: to be cheked later because
    // we might need to declare function
    // inside function
    size_t j = 0;
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        Node *child = ura.ast->children[i];
        if (child->token->type ==
            TEMPLATE_DEC) // TODO: view not on tope taht pushed to ura.ast
            continue;
        if (assert_declaration_is_valid(ura.ast, i))
            ura.ast->children[j++] = child;
    }
    ura.ast->children_count = j;
    print_nodes(GREEN("==========CHECK================\n"));
    for (size_t i = 0; i < ura.ast->children_count; i++)
        analyze_ast(ura.ast->children[i]);
    exit_scope();
}

// ============================== analyse helpers ==============================

void asm_init(char *name) {
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
        CodeGenOptLevel level = CodeGenLevelDefault;
        RelocMode reloc = RelocDefault;
        CodeModel model = CodeModelDefault;
        TargetMachine machine;
        machine = LLVMCreateTargetMachine(target, triple, "", "", level, reloc, model);
        TargetData layout = LLVMCreateTargetDataLayout(machine);
        LLVMSetModuleDataLayout(ura.module, layout);
        LLVMDisposeTargetData(layout);
        LLVMDisposeTargetMachine(machine);
    }
    LLVMDisposeMessage(triple);

    // TODO: add asan stuff here
    // TODO: add flags stuff (Passes)
}

void asm_finalize(char *ll_path) {
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
        eprint("module verification failed:\n%s\n", error);
    LLVMDisposeMessage(error);
    // LLVMDisposePassBuilderOptions(opts);
    LLVMPrintModuleToFile(ura.module, ll_path, NULL);
}

TypeRef get_llvm_type(Type type) {
    // ura.vd = LLVMVoidTypeInContext(ura.context);
    // ura.i1 = LLVMInt1TypeInContext(ura.context);
    // ura.i8 = LLVMInt8TypeInContext(ura.context);
    // ura.i16 = LLVMInt16TypeInContext(ura.context);
    // ura.i32 = LLVMInt32TypeInContext(ura.context);
    // ura.i64 = LLVMInt64TypeInContext(ura.context);
    // ura.f32 = LLVMFloatTypeInContext(ura.context);
    // ura.f64 = LLVMDoubleTypeInContext(ura.context);
    // clang-format off
    switch (type) {
    case VOID:  return LLVMVoidTypeInContext(ura.context);
    case BOOL:  return LLVMInt1TypeInContext(ura.context);
    case I8:    return LLVMInt8TypeInContext(ura.context);
    case I32:   return LLVMInt32TypeInContext(ura.context);
    default:
        eprint("handle this case %t\n", type);
        exit(1);
        break;
    }
    // clang-format on
    return NULL;
};

TypeRef get_data_type(Node *type) {
    // assert(type);
    // assert(type->token);
    if (includes(type->token->type, REF, ARRAY, 0))
        return LLVMPointerType(get_data_type(type->left), 0);
    if (type->token->type == STRUCT_DEC)
        return type->token->llvm.type;
    return get_llvm_type(type->token->type);
}

// TODO: to be cheked
Value create_alloca(TypeRef type, char *name) {
    Bloc current = LLVMGetInsertBlock(ura.builder);
    Bloc entry = LLVMGetEntryBasicBlock(LLVMGetBasicBlockParent(current));
    Value inst = LLVMGetFirstInstruction(entry);
    while (inst && LLVMIsAAllocaInst(inst))
        inst = LLVMGetNextInstruction(inst);
    if (inst)
        LLVMPositionBuilderBefore(ura.builder, inst);
    else
        LLVMPositionBuilderAtEnd(ura.builder, entry);
    Value alloca = LLVMBuildAlloca(ura.builder, type, name);
    LLVMPositionBuilderAtEnd(ura.builder, current);
    return alloca;
}

// TODO: to be checked
Value copy_to_stack(Value constant) {
    TypeRef type = LLVMTypeOf(constant);
    Value global = LLVMAddGlobal(ura.module, type, "const");
    LLVMSetInitializer(global, constant);
    LLVMSetGlobalConstant(global, 1);
    LLVMSetLinkage(global, LLVMPrivateLinkage);
    LLVMSetUnnamedAddress(global, LLVMGlobalUnnamedAddr);

    Value local = create_alloca(type, "array");
    TargetData data = LLVMGetModuleDataLayout(ura.module);
    TypeRef i64 = LLVMInt64TypeInContext(ura.context);
    Value size = LLVMConstInt(i64, LLVMABISizeOfType(data, type), 0);
    LLVMBuildMemCpy(ura.builder, local, 1, global, 1, size);

    TypeRef i32 = get_llvm_type(I32);
    Value zero[] = { LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, 0, 0) };
    return LLVMBuildInBoundsGEP2(ura.builder, type, local, zero, 2, "");
}

// node is VAR_DEC: its left is the VAR, the VAR's left is the type
Value create_variable(Node *node) {
    Token *var = node->left->token;
    Node *type = node->left->left;
    return create_alloca(get_data_type(type), var->name);
}

Value create_value(Node *node) {
    Token *token = node->token;
    if (token->type == CHARS) {
        char *text = token->chars.value;
        size_t len = strlen(text);
        return copy_to_stack(LLVMConstStringInContext(ura.context, text, len, 0));
    }
    if (token->type == NULL_) {
        TypeRef i8_ptr = LLVMPointerType(get_llvm_type(I8), 0);
        TypeRef type = node->left ? get_data_type(node->left) : i8_ptr;
        return LLVMConstPointerNull(type);
    }
    TypeRef llvm_type = get_llvm_type(token->type);
    // if (token->is_ref)
    //     llvm_type = LLVMPointerType(llvm_type, 0);
    // clang-format off
    switch (token->type) {
    case BOOL:  return LLVMConstInt(llvm_type, token->b1.value, 0);
    case I8:    return LLVMConstInt(llvm_type, token->i8.value, 0);
    case I32:   return LLVMConstInt(llvm_type, token->i32.value, 0);
    default:
        eprint("handle this case %t\n", token->type);
        exit(1);
        break;
    }
    // clang-format on
    return NULL;
}

// node is VAR_LOAD: its left is the VAR that own the storage
Value create_load(Node *node) {
    Value ptr = address_of(node);
    TypeRef type = LLVMGetElementType(LLVMTypeOf(ptr));
    Token *token = node->token;
    char *name = token->asm_name ? token->asm_name : token->name;
    return LLVMBuildLoad2(ura.builder, type, ptr, name ? name : "");
}

void create_struct(Node *node) {
    Token *token = node->token;
    token->llvm.type = LLVMStructCreateNamed(ura.context, token->name);
    assert(token->llvm.type != NULL);
    // TODO: protect when struct has no attributes
    TypeRef *attrs = ura_alloc(node->children_count, sizeof(TypeRef));
    size_t count = 0;
    for (size_t i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (!includes(child->token->type, VAR_DEC, 0))
            continue;
        attrs[count++] = get_data_type(child->left->left);
    }
    LLVMStructSetBody(token->llvm.type, attrs, count, 0);
}

Value create_array(Node *node) {
    size_t count = node->children_count;
    TypeRef elem = get_data_type(node->left->left);
    Value *values = ura_alloc(count, sizeof(Value));
    for (size_t i = 0; i < count; i++) {
        Value value = node->children[i]->token->llvm.elem;
        values[i] = LLVMIsConstant(value) ? value : LLVMConstNull(elem);
    }
    Value first = copy_to_stack(LLVMConstArray(elem, values, count));

    TypeRef i32 = get_llvm_type(I32);
    for (size_t i = 0; i < count; i++) {
        Value value = node->children[i]->token->llvm.elem;
        if (LLVMIsConstant(value))
            continue;
        Value index = LLVMConstInt(i32, i, 0);
        Value ptr = LLVMBuildInBoundsGEP2(ura.builder, elem, first, &index, 1, "");
        LLVMBuildStore(ura.builder, value, ptr);
    }
    return first;
}

Value create_sizeof(Node *node) {
    TargetData data = LLVMGetModuleDataLayout(ura.module);
    unsigned long long size = LLVMABISizeOfType(data, get_data_type(node->left));
    return LLVMConstInt(get_llvm_type(I32), size, 0);
}

// TODO: add a flag to define if it's float or unsigned or something
Value create_bin_op(Node *node) {
    // TODO: handle unsigned types
    // clang-format off
    LLVMOpcode opcodes[END + 1] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub, [MUL] = LLVMMul, [DIV] = LLVMSDiv,
        [MOD] = LLVMSRem, [AND] = LLVMAnd, [OR] = LLVMOr,
    };
    LLVMIntPredicate predicates[END + 1] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
        [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
    };
    // clang-format on

    Type type = node->token->type;
    const char *name = to_string(type);
    Value left = node->left->token->llvm.elem;
    Value right = node->right->token->llvm.elem;
    if (opcodes[type])
        return LLVMBuildBinOp(ura.builder, opcodes[type], left, right, name);
    if (predicates[type])
        return LLVMBuildICmp(ura.builder, predicates[type], left, right, name);
    eprint("unknown operation %t\n", type);
    exit(1);
}

Value address_of(Node *node) {
    switch (node->token->type) { // clang-format off
    case VAR:      return node->token->llvm.elem;
    case VAR_LOAD: return node->left->token->llvm.elem;
    case DOT: {
        Value ptr = address_of(node->left); // address of struct, not its value
        TypeRef type = LLVMGetElementType(LLVMTypeOf(ptr));
        Token *attr = node->right->token;
        unsigned index = attr->i32.value;
        char *name = node->token->asm_name;
        return LLVMBuildStructGEP2(ura.builder, type, ptr, index, name);
    }
    case ACCESS: {
        code_gen(node->left);
        code_gen(node->right);
        Value ptr = node->left->token->llvm.elem;
        TypeRef type = LLVMGetElementType(LLVMTypeOf(ptr));
        Value index = node->right->token->llvm.elem;
        return LLVMBuildInBoundsGEP2(ura.builder, type, ptr, &index, 1, "");
    }
    case DREF: {
        code_gen(node->left); // TODO: to be checked, I don't like it here
        return node->left->token->llvm.elem;
    }
    default: {
        eprint("can't assign to %t\n", node->token->type);
        exit(1);
        break;
    }
    } // clang-format on
    return NULL;
}

Value create_assign(Node *left, Node *right) {
    Value left_elem = address_of(left);
    Value right_elem = right->token->llvm.elem;
    return LLVMBuildStore(ura.builder, right_elem, left_elem);
}

void create_function(Node *node) {
    Token *token = node->token;
    // set return type
    TypeRef ret = node->right ? get_data_type(node->right) : get_llvm_type(VOID);
    // set params signature
    // TODO: add them
    TypeRef *args = NULL;
    size_t args_count = node->left->children_count;
    if (args_count) {
        args = ura_alloc(node->left->children_count, sizeof(TypeRef));
        for (size_t i = 0; i < args_count; i++) {
            Node *type = node->left->children[i]->left->left;

            args[i] = get_data_type(type);
        }
    }
    // TODO: set args count, set if function is variadic or not
    token->llvm.type = LLVMFunctionType(ret, args, args_count, token->is_variadic);
    char *name = token->asm_name ? token->asm_name : token->name;
    token->llvm.elem = LLVMAddFunction(ura.module, name, token->llvm.type);
}

void create_entry(Node *node) {
    Value fn = node->token->llvm.elem;
    Bloc bloc = LLVMAppendBasicBlockInContext(ura.context, fn, "entry");
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

// fn is FN_DEC, param is VAR_DEC: store the incoming argument into its slot
Value create_param(Node *fn, Node *param_node, size_t pos) {
    assert(fn->token->llvm.elem);
    assert(param_node->left->token->llvm.elem);

    Value param = LLVMGetParam(fn->token->llvm.elem, pos);
    Value value = param_node->left->token->llvm.elem;

    return LLVMBuildStore(ura.builder, param, value);
}

Value create_function_call(Node *node) {
    Token *fdec = node->right->token;
    Value *args = NULL;
    size_t args_count = node->left->children_count;
    if (args_count) {
        args = ura_alloc(args_count, sizeof(Value));
        for (size_t i = 0; i < args_count; i++) {
            args[i] = node->left->children[i]->token->llvm.elem;
        }
    }
    TypeRef type = fdec->llvm.type;
    Value fn = fdec->llvm.elem;
    char *name = node->right->right ? node->token->name : "";
    Value res = LLVMBuildCall2(ura.builder, type, fn, args, args_count, name);
    return res;
}

Value create_return(Node *node) {
    if (!node)
        return LLVMBuildRetVoid(ura.builder);
    return LLVMBuildRet(ura.builder, node->token->llvm.elem);
}

void create_default_return(Node *node) {
    if (is_bloc_terminated())
        return;
    if (!node->right) {
        LLVMBuildRetVoid(ura.builder);
        return;
    }
    LLVMBuildRet(ura.builder, create_value(node->right));
}

Value get_parent_bloc() {
    return LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
}

Bloc create_label(char *name) {
    Value parent = get_parent_bloc();
    return LLVMAppendBasicBlockInContext(ura.context, parent, name);
}

void create_jmp_condition(Value cond, Bloc then, Bloc next) {
    LLVMBuildCondBr(ura.builder, cond, then, next);
    LLVMPositionBuilderAtEnd(ura.builder, then);
}

bool is_bloc_terminated() {
    return LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)) != NULL;
}

void create_jmp_out(Bloc bloc) {
    LLVMBuildBr(ura.builder, bloc);
}

void create_jmp(Bloc bloc) {
    if (!is_bloc_terminated())
        LLVMBuildBr(ura.builder, bloc);
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

void create_at(Bloc bloc) {
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

void create_last_label(Bloc bloc) {
    LLVMMoveBasicBlockAfter(bloc, LLVMGetLastBasicBlock(get_parent_bloc()));
}

// ================================== analyse ==================================

void code_gen(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case VAR_DEC: {
        node->left->token->llvm.elem = create_variable(node);
        break;
    }
    case VAR: {
        break;
    }
    case VAR_LOAD:
    case DOT:
    case ACCESS:
    case DREF: {
        node->token->llvm.elem = create_load(node);
        break;
    } // clang-format off
    case NULL_: case BOOL: case I8: case I32: case CHARS: {
        // clang-format on
        node->token->llvm.elem = create_value(node);
        break;
    }
    case SIZEOF: {
        node->token->llvm.elem = create_sizeof(node);
        break;
    }
    case ARRAY_LIT: {
        for (size_t i = 0; i < node->children_count; i++)
            code_gen(node->children[i]);
        node->token->llvm.elem = create_array(node);
        break;
    }
    case STRUCT_DEC: {
        for (size_t i = 0; i < node->children_count; i++) {
            if (includes(node->children[i]->token->type, FN_DEC, 0))
                code_gen(node->children[i]);
        }
        break;
    }
    case OWN: {
        node->token->llvm.elem = address_of(node->left);
        break;
    }
    case ASSIGN: {
        // code_gen(node->left);
        code_gen(node->right);
        if (ura.errors_count)
            break;
        // TODO: check compatibility
        node->token->llvm.elem = create_assign(node->left, node->right);
        break;
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD: 
    case GT: case LT: case GE: case LE: case EQ: case NQ: 
    case AND: case OR: { // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_bin_op(node);
        break;
    }
    case PROTO: {
        break;
    }
    case FN_DEC: {
        enter_scope(node);

        create_entry(node);

        // extract parameters as variables
        for (size_t i = 0; i < node->left->children_count; i++) {
            Node *dec = node->left->children[i];
            code_gen(dec);
            create_param(node, dec, i);
        }
        // code gen children
        gen_body(node);
        create_default_return(node);
        exit_scope();
        break;
    }
    case FN_CALL: {
        for (size_t i = 0; i < node->left->children_count; i++) {
            code_gen(node->left->children[i]);
        }
        node->token->llvm.elem = create_function_call(node);
        break;
    }
    case RETURN: {
        if (node->left)
            code_gen(node->left);
        node->token->llvm.elem = create_return(node->left);
        break;
    }
    case IF: {
        Bloc end = create_label("endif");
        for (Node *curr = node; curr; curr = curr->right) {
            enter_scope(curr);
            if (curr->token->type == ELSE) {
                gen_body(curr);
                create_jmp(end);
                exit_scope();
                break;
            }
            Bloc then = create_label("then");
            Bloc next = end;
            if (curr->right) {
                bool is_else = curr->right->token->type == ELSE;
                next = create_label(is_else ? "else" : "elif");
            }

            code_gen(curr->left);
            create_jmp_condition(curr->left->token->llvm.elem, then, next);
            gen_body(curr);
            create_jmp(end);
            exit_scope();
            create_at(next);
            create_last_label(end);
        }
        break;
    }
    case WHILE: {
        Bloc cond = create_label("cond");
        Bloc then = create_label("then");
        Bloc end = create_label("endwhile");
        node->token->llvm.cond = cond;
        node->token->llvm.then = then;
        node->token->llvm.end = end;

        enter_scope(node);
        create_jmp(cond);
        code_gen(node->left);
        create_jmp_condition(node->left->token->llvm.elem, then, end);

        gen_body(node);
        create_jmp(cond);
        exit_scope();

        create_at(end);
        // create_last_label(end);
        break;
    }
    case BRK: {
        create_jmp_out(node->left->token->llvm.end);
        break;
    }
    case CNT: {
        create_jmp_out(node->left->token->llvm.cond);
        break;
    }
    default:
        eprint("handle this case %t\n", node->token->type);
        exit(1);
        break;
    }
}

void gen_body(Node *node) {
    for (size_t i = 0; i < node->children_count; i++) {
        code_gen(node->children[i]);
        if (is_bloc_terminated())
            break;
    }
}

void gen_funcs_and_structs_asm(Node *node) {
    if (!node)
        return;

    for (size_t i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        Type type = child->token->type;
        if (includes(type, FN_DEC, PROTO, 0))
            create_function(child);
        else if (includes(type, STRUCT_DEC, 0))
            create_struct(child);
        gen_funcs_and_structs_asm(child);
    }
}

void gen_asm(uraFile *file) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    asm_init("ura-module");

    gen_funcs_and_structs_asm(ura.ast);
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        code_gen(ura.ast->children[i]);
    }
    // for (size_t i = 0; i < ura.ast->functions_count; i++) {
    //     code_gen(ura.ast->functions[i]);
    // }
    exit_scope();
    asm_finalize(file->ll_path);
}

void gen_bin(uraFile *file) {
    if (ura.errors_count)
        return;

    char *argv[] = { "clang", file->ll_path, "-o", ura.exec, NULL };

    pid_t pid = fork();
    if (pid < 0) {
        eprint("fork failed\n");
        return;
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        eprint("clang failed to compile %s\n", file->ll_path);
}

// ==================================== main ====================================

void parse_arguments(int ac, char **av) {
    if (ac < 2) {
        eprint("expected argument: ura <file_name>.ura\n");
        return;
    }
    ura.exec = "exe.out";
    for (int i = 1; i < ac && !ura.errors_count; i++) {
        char *arg = av[i];
        if (strcmp(arg, "-o") == 0) {
            if (i + 1 >= ac) {
                eprint("expected argument: ura <file_name>.ura\n");
                return;
            }
            ura.exec = av[++i]; // output file
        } else {
            size_t n = strlen(arg);
            bool is_ura = n > 4 && strcmp(arg + n - 4, ".ura") == 0;
            if (!is_ura) {
                eprint("Invalid file '%s'\n", arg);
                return;
            }
            new_file(arg);
        }
    }
}

int main(int ac, char **av) {
    atexit(diag_flush);
    atexit(free_arena);
    parse_arguments(ac, av);
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        gen_tokens(file);
        gen_ast();
        print_nodes(GREEN("============AST=================\n"));
#if GEN_IR
        gen_ir();
        print_nodes(GREEN("============IR==================\n"));
#    if GEN_ASM
        gen_asm(file);
        print_nodes(GREEN("============ASM==================\n"));
        gen_bin(file);
#    endif
#endif
        print(GREEN("============CLEANING============\n"));
    }
    // TODO: to be check when compiling multiple files
    if (ura.errors_count) {
        diag_flush();
        int count = ura.errors_count;
        char *plural = count == 1 ? "" : "s";
        fprintf(stderr, RED("error:") " aborting due to %d error%s\n", count, plural);
    }
    print("heap used (%zu) in (%zu) arenas \n", ura.heap_size, ura.arena_index);
    return ura.errors_count != 0;
}

// ============================== memory and files ==============================

Arena *new_arena(size_t arena_size) {
    Arena *new = calloc(1, sizeof(Arena));
    if (new == NULL) {
        eprint("failed\n");
        exit(1);
    }
    new->buf = calloc(arena_size, sizeof(char));
    if (new->buf == NULL) {
        eprint("failed\n");
        exit(1);
    }
    new->size = arena_size;
    ura.arena_index++;
    ura.heap_size += (arena_size + sizeof(Arena));
    return new;
}

void arena_reserve(size_t bytes) {
    Arena *curr = ura.arena_curr;
    if (curr && curr->size - curr->used >= bytes)
        return;
    Arena *new = new_arena(bytes);
    if (curr)
        curr->next = new;
    else
        ura.arena_head = new;
    ura.arena_curr = new;
}

void *ura_alloc(size_t count, size_t size) {
    size_t heap_size = count * size;
    heap_size = (heap_size + 16 - 1) / 16 * 16;

    if (ura.arena_curr == NULL ||
        ura.arena_curr->used + heap_size > ura.arena_curr->size) {
        size_t arena_size = 1 << 8;
        if (ura.arena_curr != NULL)
            arena_size = ura.arena_curr->size * 2;
        while (arena_size < heap_size)
            arena_size <<= 1;

        Arena *new = new_arena(arena_size);
        if (ura.arena_curr == NULL)
            ura.arena_head = new;
        else
            ura.arena_curr->next = new;
        ura.arena_curr = new;
    }

    void *ptr = ura.arena_curr->buf + ura.arena_curr->used;
    ura.arena_curr->used += heap_size;
    return ptr;
}

char *ura_strdup(char *str) {
    char *res = ura_alloc(strlen(str) + 1, sizeof(char));
    strcpy(res, str);
    return res;
}

void free_arena() {
    Arena *arena = ura.arena_head;
    while (arena) {
        Arena *next = arena->next;
        free(arena->buf);
        free(arena);
        arena = next;
    }
}

void new_file(char *name) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    // TODO: check this one when implementing arena allocator
    file->name = ura_strdup(name);
    push_back(ura.files, file);

    char *slash = strrchr(name, '/');
    if (slash) {
        file->dir = ura_strdup(name);
        file->dir[slash - name] = '\0';
        file->base = ura_strdup(slash + 1);
    } else {
        file->dir = ura_strdup(".");
        file->base = ura_strdup(name);
    }
    char *dot = strrchr(file->base, '.');
    if (dot)
        *dot = '\0';
    file->build_dir = format("%s/build", file->dir);
    mkdir(file->build_dir, 0755);
    file->ll_path = format("%s/%s.ll", file->build_dir, file->base);

    File fp = fopen(name, "r");
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
    fclose(fp);
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

char *strjoin(char *first, ...) {
    size_t len = 0;
    va_list args;
    va_start(args, first);
    for (char *str = first; str; str = va_arg(args, char *))
        len += strlen(str);
    va_end(args);

    char *res = ura_alloc(len + 1, sizeof(char));
    size_t at = 0;
    va_start(args, first);
    for (char *str = first; str; str = va_arg(args, char *)) {
        size_t size = strlen(str);
        memcpy(res + at, str, size);
        at += size;
    }
    va_end(args);
    return res;
}

// ================================== printing ==================================

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

int print_type(File fp, Node *type) {
    if (!type)
        return fprintf(fp, "void");
    // clang-format off
    switch (type->token->type) {
    case REF:        return fprintf(fp, "&") + print_type(fp, type->left);
    case STRUCT_DEC: return fprintf(fp, "%s", type->token->name);
    case BOOL:       return fprintf(fp, "b1");
    case I8:         return fprintf(fp, "i8");
    case I32:        return fprintf(fp, "i32");
    case NULL_:      return fprintf(fp, "null");
    case ARRAY:      return print_type(fp, type->left) + fprintf(fp, "[]");
    default:         return fprintf(fp, "%s", to_string(type->token->type));
    }
    // clang-format on
}

int print_token(File fp, Token *token) {
    if (!token)
        return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name)
        r += fprintf(fp, " name (%s)", token->name);

    if (!token->name && !token->is_type) {
        // clang-format off
        switch (token->type) {
        case I32:   r += fprintf(fp, " value (%ld)", token->i32.value); break;
        case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case CHARS: r += fprintf(fp, " value (%s)", token->chars.value); break;
        case BOOL: { // clang-format on
            char *b1 = token->b1.value ? "True" : "False";
            r += fprintf(fp, " value (%s)", b1);
            break;
        }
        default:
            break;
        }
    }
    r += fprintf(fp, " space (%ld)", token->space);
    if (token->ret_type)
        r += fprintf(fp, " ret (%s)", to_string(token->ret_type));
    return r;
}

int _print(File fp, const char *fmt, va_list args) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (!fmt[i + 1])
            break;
        Token *token = NULL;
        int len = 0;
        switch (fmt[++i]) { // clang-format off
        case 's': r += fprintf(fp, "%s", va_arg(args, char *)); break;
        case 'd': r += fprintf(fp, "%d", va_arg(args, int)); break;
        case 'c': r += fprintf(fp, "%c", va_arg(args, int)); break;
        case 'f': r += fprintf(fp, "%f", va_arg(args, double)); break;
        case 'z': r += fprintf(fp, "%zu", va_arg(args, size_t)); i++; break;
        case 'i': r += fprintf(fp, "%*s", va_arg(args, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 't': r += fprintf(fp, "%s", to_string(va_arg(args, Type))); break;
        case 'N': r += print_type(fp, va_arg(args, Node *)); break;
        case 'k': r += print_token(fp, va_arg(args, Token *)); break;
        case 'n': r += print_node(fp, va_arg(args, Node *)); break;
        case 'K': {
            token = va_arg(args, Token *);
            len = (int)(token->e - token->s);
            r += fprintf(fp, "%.*s", len, ura.curr_content + token->s);
            break;
        }
        default:  r += fprintf(fp, "%%%c", fmt[i]); break;
        } // clang-format on
    }
    return r;
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
    char *res = ura_strdup(buf);
    free(buf);
    return res;
}

void print_helper(NodePrint *elems, Node *node, int depth) {
    if (!node)
        return;

    push_back(elems->nodes, node);
    push_back(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, 0)) {
        Node *left = node->left;
        bool struct_type = left && left->token->type == STRUCT_DEC;
        if (includes(node->token->type, ID, VAR, REF, 0) && struct_type) {
            push_back(elems->nodes, left);
            push_back(elems->depths, depth + 3);
        } else {
            print_helper(elems, left, depth + 3);
        }
    }
    if (!includes(node->token->type, FN_CALL, 0)) {
        print_helper(elems, node->right, depth + 3);
    }
    for (size_t i = 0; i < node->children_count; i++) {
        print_helper(elems, node->children[i], depth + 3);
    }
}

int still_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++)
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    return 0;
}

int print_node(File fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    print_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", still_open(&elems, i, level) ? "│ " : " ");
        if (depth)
            r += fprintf(fp, "%s", still_open(&elems, i, depth) ? "├─" : "└─");
        r += print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return r;
}

void print_nodes(char *text) {
    if (ura.errors_count)
        return;
    print(text);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        print_node(stdout, ura.ast->children[i]);
}

// =================================== errors ===================================

void diag_flush(void) {
    if (ura.error_file) {
        char *file = ura.error_file;
        int line = ura.error_line;
        const char *func = ura.error_func;
        fprintf(stderr, DIM("raised at %s:%d %s") "\n", file, line, func);
    }
    ura.error_file = NULL;
}

void _error_at(char *file, const char *func, int line, Token *token, char *message) {
    diag_flush();
    ura.errors_count++;
    ura.error_file = file;
    ura.error_func = func;
    ura.error_line = line;

    char *content = ura.curr_content;
    size_t s = token->s;
    while (s > 0 && content[s - 1] != '\n')
        s--;
    size_t e = token->s;
    while (content[e] && content[e] != '\n')
        e++;
    size_t col = token->s - s + 1;
    // TODO: chekc this one
    char *name = ura.curr_file->name;

    size_t row = token->line;
    fprintf(stderr, RED("error:") " %s:%zu:%zu %s\n", name, row, col, message);

    int width = snprintf(NULL, 0, "%zu", row);
    fprintf(stderr, "%*s |\n", width, "");
    fprintf(stderr, "%zu | %.*s\n", token->line, (int)(e - s), content + s);
    fprintf(stderr, "%*s | %*s", width, "", (int)(token->s - s), "");
    for (size_t i = token->s; i < token->e || i == token->s; i++)
        fputc('^', stderr);
    fprintf(stderr, "\n");
}

void _help(char *message) {
    fprintf(stderr, CYAN("help:") " %s\n", message);
}

bool assert_literal_is_valid(Token *literal, bool closed) {
    char *content = ura.curr_content;
    bool is_char = content[literal->s] == '\'';
    size_t len = literal->e - literal->s - 2;
    bool escaped = content[literal->s + 1] == '\\';
    if (!closed && !is_char)
        error_at(literal, "unterminated string");
    else if (!closed)
        error_at(literal, "unterminated character");
    else if (is_char && len == 0)
        error_at(literal, "empty character literal");
    else if (is_char && len != (escaped ? 2 : 1)) {
        char *text = ura_alloc(len + 1, sizeof(char));
        strncpy(text, content + literal->s + 1, len);
        error_at(literal, "a character holds one letter");
        help("for a string, write '\"%s\"'", text);
    } else
        return true;
    return false;
}

bool assert_number_fits(Token *number, Type type, bool report) {
    // clang-format off
    struct { char *name; long min; long max; } range[END + 1] = {
        [I8]  = { "i8",  -128,    127     },
        [I32] = { "i32", INT_MIN, INT_MAX },
    };
    // clang-format on
    long value = number->i32.value;
    if (value >= range[type].min && value <= range[type].max)
        return true;
    if (report)
        error_at(number, "'%K' doesn't fit in '%s'", number, range[type].name);
    return false;
}

Node *recover(Token *start) {
    while (peek(0)->type != END) {
        Token *token = peek(0);
        if (token->line > start->line && token->space <= start->space)
            break;
        next();
    }
    return new_node(new_token(ERR, start));
}

Token *assert_next_is(Type type, char *message) {
    Token *token = peek(0);
    if (token->type == type)
        return next();
    Token *prev = ura.tokens[ura.exe_pos - 1];
    error_at(token->line == prev->line ? token : prev, message);
    return NULL;
}

bool assert_character_is_known(Token *token) {
    error_at(token, "unexpected character '%c'", ura.curr_content[token->s]);
    return false;
}

bool assert_is_struct_or_func(Token *token) {
    if (includes(token->type, FN_DEC, PROTO, STRUCT_DEC, 0))
        return true;
    error_at(token, "expected struct/fn after template");
    return false;
}

bool assert_templates_param(Token *token) {
    if (token->type == ID)
        return true;
    error_at(token, "expected names after template");
    help("use template<T, V>");
    return false;
}

bool assert_template_args_match(Node *template, Token *name, Node *args) {
    size_t want = template->left->children_count;
    size_t got = args->children_count;
    if (want == got)
        return true;
    char *plural = want == 1 ? "" : "s";
    char *type = name->name;
    error_at(name, "'%s' takes %zu type argument%s, got %zu", type, want, plural, got);

    char *params = ura_strdup("");
    for (size_t i = 0; i < want; i++) {
        if (i > 0)
            params = strjoin(params, ", ", NULL);
        params = strjoin(params, template->left->children[i]->token->name, NULL);
    }
    help("it is declared 'template<%s>'", params);
    return false;
}

bool assert_token_is_expected(Token *token) {
    if (includes(token->type, ELIF, ELSE, 0))
        error_at(token, "'%K' without a matching 'if'", token);
    else
        error_at(token, "unexpected '%K'", token);
    return false;
}

bool assert_ref_is_single(void) {
    Token *first = peek(0);
    Token *second = peek(1);
    bool double_amp =
        first->type == AND && is_data_type(second); // && (followed refrences)
    bool two_refs = first->type == REF && second->type == REF; // & &
    if (!double_amp && !two_refs)
        return true;
    error_at(double_amp ? first : second, "a ref can't point to a ref");
    return false;
}

bool assert_type_follows(Node *type, Token *after) {
    if (type)
        return true;
    error_at(after, "expected a type after '%K'", after);
    return false;
}

bool assert_type_is_known(Node *type) {
    Token *name = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    // print("type %s\n", type ? "exits": "not exists");
    // print("%s\n", name->type == ID ? "is id" : "is not id");
    if (type || name->type != ID || name->line != prev->line)
        return true;
    error_at(name, "unknown type '%s'", name->name);
    return false;
}

bool assert_struct_not_inside_itself(Node *type) {
    if (type != ura.scope)
        return true;
    char *name = type->token->name;
    error_at(peek(0), "struct '%s' can't contain itself", name);
    help("use a ref: '&%s'", name);
    return false;
}

bool assert_array_type_is_closed(Token *open, Node *type) {
    Token *close = peek(0);
    if (close->type == RBRACK) {
        next();
        return true;
    }
    error_at(close->line == open->line ? close : open, "expected ']' after '['");
    if (close->type == I32)
        help("arrays have no size in the type: '%N[]'", type);
    return false;
}

bool assert_arguments_are_separated(Token *call) {
    Token *after = peek(0);
    if (after->type == COMA && peek(1)->type == RPARENT) {
        error_at(peek(1), "expected an argument after ','");
        next();
        return true;
    }
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, RPARENT, 0))
        return true;
    if (after->line == call->line)
        error_at(after, "expected ',' between arguments");
    else
        error_at(call, "expected ')' to close the call to '%s'", call->name);
    return false;
}

bool assert_access_is_closed(Node *access) {
    if (peek(0)->type == RBRACK) {
        next();
        return true;
    }
    error_at(access->token, "unclosed '['");
    return false;
}

bool assert_array_literal_valid(Node *literal) {
    Token *open = literal->token;
    Token *curr = peek(0);
    bool ends = curr->line > open->line && curr->space <= open->space;
    if (ends || curr->type == END) {
        error_at(open, "unclosed '['");
        return false;
    }
    if (literal->children_count && curr->type != COMA) {
        error_at(curr, "expected ',' between elements");
        return false;
    }
    return true;
}

bool assert_is_struct_member(Node *attr) {
    Token *at = attr->token;
    if (includes(at->type, VAR_DEC, FN_DEC, 0))
        return true;
    if (at->type == ID) {
        error_at(at, "attribute '%s' needs a type", at->name);
    } else
        error_at(at, "a struct holds only attributes and methods");
    return false;
}

bool assert_address_is_single(Token *amp) {
    if (amp->type == REF && peek(0)->type != REF)
        return true;
    error_at(amp, "can't take the address of an address");
    return false;
}

bool assert_parenthesis_is_closed(Token *open) {
    if (peek(0)->type == RPARENT) {
        next();
        return true;
    }
    error_at(open, "unclosed '('");
    return false;
}

bool assert_variadic_is_last(Node *arg) {
    if (peek(0)->type == RPARENT)
        return true;
    error_at(arg->token, "'...' must be the last parameter");
    return false;
}

bool assert_parameter_has_type(Node *arg) {
    if (arg->token->type != ID)
        return true;
    error_at(arg->token, "parameter '%s' needs a type", arg->token->name);
    return false;
}

bool assert_function_parameters_are_separated(void) {
    Token *after = peek(0);
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, RPARENT, 0))
        return true;
    if (after->type == ID)
        error_at(after, "expected ',' between parameters");
    else
        error_at(after, "expected ')' to close the parameters");
    return false;
}

bool assert_template_parameters_are_separated(void) {
    Token *after = peek(0);
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, GT, 0))
        return true;
    if (after->type == ID)
        error_at(after, "expected ',' between parameters");
    else
        error_at(after, "expected '>' to close the parameters");
    return false;
}

bool assert_condition_follows(Token *keyword) {
    if (peek(0)->type != DOTS)
        return true;
    error_at(keyword, "expected a condition after '%K'", keyword);
    return false;
}

bool assert_else_has_no_condition(Token *keyword) {
    Token *after = peek(0);
    if (after->type == DOTS || after->line != keyword->line)
        return true;
    error_at(after, "'else' takes no condition");
    help("use 'elif'");
    return false;
}

bool assert_operand_follows(Token *op) {
    Token *after = peek(0);
    if (after->line == op->line && after->type != END)
        return true;
    error_at(op, "expected an expression after '%K'", op);
    return false;
}

bool assert_type_is_printable(Node *value, char *spec) {
    if (spec)
        return true;
    error_at(value->token, "can't output '%N'", type_of(value));
    return false;
}

bool assert_call_is_unambiguous(Node *call, Node *found) {
    if (!found)
        return true;
    error_at(call->token, "ambiguous call to '%s'", call->token->name);
    return false;
}

// TODO: to be reviewed
bool assert_type_fits(Node *expected, Node *value, bool report) {
    Token *token = value ? value->token : NULL;
    Type kind = token ? token->type : VOID;
    Type want = expected ? expected->token->type : VOID;
    if (kind == ERR || want == ERR)
        return true;

    if (kind == NULL_ && want == REF) {
        if (report)
            value->left = expected;
        return true;
    }

    if (kind == I32 && !token->is_type && want == I8) {
        if (!assert_number_fits(token, I8, report))
            return false;
        if (report) {
            long number = token->i32.value;
            token->type = I8;
            token->i8.value = number;
        }
        return true;
    }

    if (kind == ARRAY_LIT && want == ARRAY) {
        bool fits = true;
        for (size_t i = 0; i < value->children_count; i++) {
            Node *element = value->children[i];
            fits = assert_type_fits(expected->left, element, report) && fits;
        }
        if (report && fits)
            value->left = expected;
        return fits;
    }

    Node *actual = value ? type_of(value) : NULL;
    bool fits = expected == actual;
    if (expected && actual && want == actual->token->type) {
        if (want == STRUCT_DEC)
            fits = strcmp(expected->token->name, actual->token->name) == 0;
        else if (includes(want, REF, ARRAY, 0))
            fits = assert_type_fits(expected->left, actual->left, false);
        else
            fits = true;
    }
    if (report && !fits)
        error_at(token, "expected '%N', found '%N'", expected, actual);
    return fits;
}

void assert_condition_is_bool(Node *cond) {
    static Node *b1;
    if (!b1) {
        b1 = new_node(new_token(BOOL, NULL));
        b1->token->is_type = true;
    }
    if (assert_type_fits(b1, cond, true))
        return;
    Node *type = type_of(cond);
    bool number = type && includes(type->token->type, I8, I32, 0);
    bool literal = includes(cond->token->type, I8, I32, 0);
    if (number && !literal) // 'while a != 0' instead of 'while a'
        help("write '%K != 0'", cond->token);
}

bool assert_operands_are_valid(Node *node) {
    bool left = !node->left || node->left->token->type != ERR;
    bool right = !node->right || node->right->token->type != ERR;
    if (left && right)
        return true;
    node->token->type = ERR;
    return false;
}

// TODO: to be cheked
bool assert_not_declared_twice(Node *node) {
    Token *var = node->left->token;
    Node *scope = ura.scope;
    Node *args = NULL;
    if (includes(scope->token->type, FN_DEC, PROTO, 0))
        args = scope->left;
    size_t body = scope->children_count;
    size_t params = 0;
    if (args)
        params = args->children_count;

    Node *twin = NULL;
    for (size_t i = 0; i < body + params && !twin; i++) {
        Node *dec = NULL;
        if (i < body)
            dec = scope->children[i];
        else
            dec = args->children[i - body];
        if (dec->token->type != VAR_DEC)
            continue;
        if (strcmp(dec->left->token->name, var->name) == 0)
            twin = dec->left;
    }
    if (!twin || twin == node->left)
        return true;
    error_at(var, "'%s' is already declared", var->name);
    return false;
}

// TODO: to be cheked
bool assert_name_is_declared(Node *node) {
    if (node->left)
        return true;
    char *name = node->token->name;
    if (find_type_by_name(name))
        error_at(node->token, "'%s' is a type, not a value", name);
    else {
        error_at(node->token, "'%s' not found", name);
        Node *self = find_variable("self");
        Node *struct_dec = NULL;
        if (self)
            struct_dec = self->left->left;
        size_t count = 0;
        if (struct_dec)
            count = struct_dec->children_count;
        for (size_t i = 0; i < count; i++) {
            Node *attr = struct_dec->children[i];
            if (attr->token->type != VAR_DEC)
                continue;
            if (strcmp(attr->left->token->name, name) == 0) {
                help("did you mean 'self.%s'?", name);
                break;
            }
        }
    }
    node->token->type = ERR;
    return false;
}

bool assert_indexing_an_array(Node *node) {
    if (!assert_operands_are_valid(node))
        return false;
    Node *type = type_of(node->left);
    if (!type || type->token->type != ARRAY) {
        Token *left = node->left->token;
        error_at(left, "'%K' is '%N', not an array", left, type);
        node->token->type = ERR;
        return false;
    }
    Node *index = type_of(node->right);
    if (!index || !includes(index->token->type, I8, I32, 0)) {
        error_at(node->right->token, "expected 'i32', found '%N'", index);
        node->token->type = ERR;
        return false;
    }
    return true;
}

bool assert_receiver_is_struct(Node *node) {
    Node *type = type_of(node->left);
    if (type && type->token->type == STRUCT_DEC)
        return true;
    Token *left = node->left->token;
    error_at(left, "'%K' is '%N', not a struct", left, type);
    node->token->type = ERR;
    return false;
}

bool assert_attribute_exists(Node *node, Node *struct_dec) {
    Token *attr = node->right->token;
    if (attr->type == ATTR)
        return true;
    char *type = struct_dec->token->name;
    error_at(attr, "struct '%s' has no attribute '%K'", type, attr);
    node->token->type = ERR;
    return false;
}

bool assert_address_of_variable(Node *node) {
    if (includes(node->left->token->type, VAR_LOAD, DOT, 0))
        return true;
    error_at(node->left->token, "'&' expects a variable or a struct attribute");
    node->token->type = ERR;
    return false;
}

bool assert_assignment_is_valid(Node *node, bool bare_name) {
    Node *left = node->left;
    Node *right = node->right;
    bool left_failed = left->token->type == ERR;
    if (bare_name && left_failed && right->token->type != ERR) {
        char *name = left->token->name;
        help("declare it: '%s %N = %K'", name, type_of(right), right->token);
    }
    if (!assert_operands_are_valid(node))
        return false;
    if (!includes(left->token->type, VAR, VAR_LOAD, DREF, DOT, ACCESS, 0)) {
        error_at(left->token, "can't assign to a value");
        node->token->type = ERR;
        return false;
    }
    if (left->token->type == VAR && left->left->token->type == REF) {
        Node *type = type_of(right);
        if (!type || !includes(type->token->type, REF, NULL_, 0)) {
            Token *value = right->token;
            Token *var = left->token;
            Token *pointer = left->left->left->token;
            char *name = var->name;
            error_at(value, "'%s' is a ref, initialize it with an address", name);
            help("write '%K &%K = &%K'", var, pointer, value);
            return false;
        }
    }
    if (assert_type_fits(type_of(left), right, true))
        return true;
    node->token->type = ERR;
    return false;
}

// TODO: to be reviewed
bool assert_operator_fits_operands(Node *node) {
    if (!assert_operands_are_valid(node))
        return false;
    Node *left = node->left;
    Node *right = node->right;
    Token *op = node->token;
    if (right->token->type == NULL_ && left->token->type != NULL_)
        right->left = type_of(left);
    if (left->token->type == NULL_ && right->token->type != NULL_)
        left->left = type_of(right);
    Node *null = left->token->type == NULL_ ? left : right;
    Node *other = null == left ? right : left;
    Node *type = null->left;
    bool is_ref = type && type->token->type == REF;
    if (null->token->type == NULL_ && !is_ref && other->token->type != NULL_) {
        error_at(null->token, "'null' needs a ref on the other side");
        node->token->type = ERR;
        return false;
    }
    if (includes(op->type, AND, OR, 0)) {
        Node *b1 = type_of(node);
        bool fits = assert_type_fits(b1, left, true);
        if (fits && assert_type_fits(b1, right, true))
            return true;
        node->token->type = ERR;
        return false;
    }
    Node *ltype = type_of(left);
    Node *rtype = type_of(right);
    bool left_number = ltype && includes(ltype->token->type, I8, I32, 0);
    bool right_number = rtype && includes(rtype->token->type, I8, I32, 0);
    bool unary = left->token->s == op->s;
    if (!includes(op->type, EQ, NQ, 0) && (!left_number || !right_number)) {
        if (unary)
            error_at(op, "can't apply '%K' to '%N'", op, rtype);
        else
            error_at(op, "can't apply '%K' to '%N' and '%N'", op, ltype, rtype);
        node->token->type = ERR;
        return false;
    }
    if (ltype && ltype->token->type == STRUCT_DEC) {
        error_at(op, "can't compare '%N' values", ltype);
        node->token->type = ERR;
        return false;
    }
    bool literal = left->token->type == I32 && !left->token->is_type;
    Node *expected = literal ? rtype : ltype;
    Node *value = literal ? left : right;
    if (assert_type_fits(expected, value, true))
        return true;
    node->token->type = ERR;
    return false;
}

bool assert_return_matches_function(Node *node, Node *fn) {
    Node *type = fn->right;
    if (!node->left && type) {
        char *name = fn->token->name;
        error_at(node->token, "'%s' must return a value of type '%N'", name, type);
        return false;
    }
    if (!node->left || assert_type_fits(type, node->left, true))
        return true;
    node->token->type = ERR;
    return false;
}

bool assert_inside_loop(Node *node) {
    if (node->left)
        return true;
    error_at(node->token, "'%K' outside of a loop", node->token);
    node->token->type = ERR;
    return false;
}

// TODO: to be cheked
bool assert_declaration_is_valid(Node *parent, size_t i) {
    Node *node = parent->children[i];
    size_t count = node->children_count;

    Token *token = node->token;
    char *name = token->name;
    switch (token->type) {
    case STRUCT_DEC: {
        for (size_t j = 0; j < i; j++) {
            Node *other = parent->children[j];
            if (other->token->type != STRUCT_DEC)
                continue;
            if (strcmp(name, other->token->name) != 0)
                continue;
            error_at(token, "struct '%s' is already defined", name);
            return false;
        }
        bool has_attr = false;
        for (size_t j = 0; j < count; j++)
            has_attr = has_attr || node->children[j]->token->type == VAR_DEC;
        if (!has_attr)
            error_at(token, "struct '%s' has no attributes", name);
        break;
    }
    case FN_DEC: {
        for (size_t j = 0; j < i; j++) {
            Node *other = parent->children[j];
            if (other->token->type != FN_DEC)
                continue;
            if (strcmp(name, other->token->name) != 0)
                continue;
            if (!same_params(node, other))
                continue;
            if (assert_type_fits(node->right, other->right, false))
                error_at(token, "'%s' is already defined with these parameters", name);
            else
                error_at(token, "'%s' can't be overloaded on its return type", name);
            return false;
        }
        break;
    }
    case RETURN: {
        error_at(token, "'return' outside of a function");
        return false;
    }
    case PROTO: {
        for (size_t j = 0; j < parent->children_count; j++) {
            Node *other = parent->children[j];
            if (other == node) // never match yourself
                continue;
            if (other->token->type != FN_DEC)
                continue;
            if (!node->token->name || !other->token->name)
                continue;
            if (strcmp(node->token->name, other->token->name) != 0)
                continue;

            if (!same_params(node, other))
                continue;
            if (!assert_type_fits(node->right, other->right, false)) {
                Token *fn = node->token;
                error_at(fn, "'%s' can't be overloaded on its return type", fn->name);
                return false;
            }
            return false; // keep fn, drop proto
        }
        break;
    }
    default:
        break;
    }
    return true;
}

// TODO: to be cheked
bool assert_builtin_takes_one(Node *call) {
    size_t got = call->left->children_count;
    if (got == 1)
        return true;
    char *name = call->token->name;
    error_at(call->token, "'%s' takes 1 argument, got %zu", name, got);
    call->token->type = ERR;
    return false;
}

bool assert_function_matches_call(Node *call) {
    if (call->right)
        return true;
    call->token->type = ERR;
    char *name = call->token->name;
    Node *struct_dec = ura.scope->token->type == STRUCT_DEC ? ura.scope : NULL;
    Node *fn = NULL;
    size_t count = 0;
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        if (scope->token->type == STRUCT_DEC && scope != struct_dec)
            continue;
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *curr = scope->children[j];
            if (!includes(curr->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(curr->token->name, name) != 0)
                continue;
            fn = curr;
            count++;
        }
        if (struct_dec)
            break;
    }
    if (count == 0 && struct_dec) {
        char *type = struct_dec->token->name;
        error_at(call->token, "struct '%s' has no method '%s'", type, name);
        return false;
    }
    if (count == 0 && find_variable(name)) {
        error_at(call->token, "'%s' is a variable, not a function", name);
        return false;
    }
    if (count == 0) {
        error_at(call->token, "function '%s' not found", name);
        Node *self = find_variable("self");
        Node *struct_dec = NULL;
        if (self)
            struct_dec = self->left->left;
        size_t methods = 0;
        if (struct_dec)
            methods = struct_dec->children_count;
        for (size_t i = 0; i < methods; i++) {
            Node *method = struct_dec->children[i];
            if (!includes(method->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(method->token->name, name) == 0) {
                help("did you mean 'self.%s()'?", name);
                break;
            }
        }
        return false;
    }
    error_at(call->token, "no overload of '%s' takes these arguments", name);
    if (count > 1) {
        help("%zu functions are named '%s', none fits these arguments", count, name);
        return false;
    }

    size_t skip = struct_dec ? 1 : 0;
    size_t want = fn->left->children_count - skip;
    size_t got = call->left->children_count - skip;
    if (fn->token->is_variadic ? got < want : got != want) {
        char *plural = want == 1 ? "" : "s";
        help("'%s' takes %zu argument%s, got %zu", name, want, plural, got);
        return false;
    }
    for (size_t i = skip; i < fn->left->children_count; i++) {
        Node *var = fn->left->children[i]->left;
        Node *arg = call->left->children[i];
        Node *expected = var->left;
        Node *actual = type_of(arg);
        if (assert_type_fits(expected, arg, false))
            continue;
        Token *given = arg->token;
        char *param = var->token->name;
        help("'%K' is '%N', but '%s' wants '%N'", given, actual, param, expected);
        bool is_ref = expected->token->type == REF;
        if (is_ref && assert_type_fits(expected->left, arg, false))
            help("pass its address: '&%K'", given);
        return false;
    }
    return false;
}

/*
TODO:
    [ ] drop method
    [ ] String struct
    [ ] to_string method
    [ ] output (should handl also struct.to_string)
    [ ] for template add a part in the code thaht JIT/interpreter
    [ ] operators overload
    [ ] for loops over a range: for i in 0..10
    [ ] more integer types (i8, i64, unsigned) and casting with as
    [ ] function inside function
*/
