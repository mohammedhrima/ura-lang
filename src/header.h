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
Value create_variable(Token *var, Node *type);
Value create_value(Token *token);
Value create_load(Token *var, Node *type);
Value create_dref(Value ptr, Node *type);
Value create_math_op(Token *left, Token *op_token, Token *right);
Value create_logic_op(Token *left, Token *op_token, Token *right);
Value create_comparision_op(Token *left, Token *op_token, Token *right);
Value address_of(Node *node);
Value create_assign(Node *left, Node *right);
void create_function(Node *node);
void create_entry(Token *token);
Value create_param(Token *fn, Token *param, size_t pos);
Value create_function_call(Node *node);
Value create_return(Token *token);
void create_default_return(Node *node);
void create_struct(Node *node);
Value get_parent_bloc();
Bloc create_label(char *name);
void create_jmp_condition(Value cond, Bloc then, Bloc next);
bool is_bloc_terminated();
void create_jmp_out(Bloc bloc);
void create_jmp(Bloc bloc);
void create_at(Bloc bloc);
void create_last_label(Bloc bloc);
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

#define eprint(...) _eprint(FILE, FUNC, LINE, __VA_ARGS__)

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
    IDENTIFIER,

    STRUCT_DEC, //STRUCT_CALL, 
    VOID, I32, I8, BOOL,
    CHARS,
    VARIADIC,
    REF, OWN, DREF,

    LPARENT, RPARENT, DOTS,

    ASSIGN, 
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    GT, LT, GE, LE, EQ, NQ,

    AND, OR,
    
    PROTO,
    FN_DEC, ARGS, COMA, RETURN,
    FN_CALL,

    IF, ELIF, ELSE,
    WHILE, BRK, CNT,

    VAR_DEC, VAR, VAR_LOAD,

    END,
};
// clang-format on

struct Token {
    Type type;
    Type ret_type;

    bool is_type;
    bool is_variadic;
    size_t space;

    ASM llvm;

    struct {
        char *name;
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
    expand(Node *, functions);
    expand(Node *, variables);
    expand(Node *, structs);
};

void *ura_alloc(size_t count, size_t size);
const char *to_string(Type type);
int _print(File fp, const char *fmt, va_list args);
int _eprint(char *file, const char *func, int line, char *fmt, ...);
int print(char *fmt, ...);
bool includes(Type to_find, ...);
Node *prime_node(void);
Node *expr_node(int min_op);
void enter_scope(Node *node);
void exit_scope(void);
void code_gen(Node *node);

struct Ura {
    int errors_count;
    expand(uraFile *, files);
    expand(Token *, tokens);
    expand(Node *, nodes);
    expand(Node *, scopes);
    Node *scope;

    char *curr_content;
    size_t exe_pos;

    Node *ast;

    Context context;
    Module module;
    Builder builder;
    char *exec;
};

extern Ura ura;