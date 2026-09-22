#pragma once

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
const char *to_string(Type type);
int _print(File fp, const char *fmt, va_list args);
int _eprint(char *file, const char *func, int line, char *fmt, ...);
void _error_at(char *file, const char *func, int line, Token *token, char *message);
void _help(char *message);
char *format(char *fmt, ...);
void diag_flush(void);
bool assert_function_matches_call(Node *call);
Node *find_by_type(Type type, char *name);
Node *find_in_children(Node *parent, Type type, char *name);
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
bool assert_parameters_are_separated(void);
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