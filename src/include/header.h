#pragma once

// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>

// MACROS
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1
#define TAB 3

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#else
#define IR 0
#endif

#define WITH_COMMENTS 1

#if IR
#define BUILTINS 1
#ifndef OPTIMIZE
#define OPTIMIZE 0
#endif

#define ASM 0
#else
#define ASM 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#ifndef TESTING
#define TESTING false
#endif

#define TREE 0

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define to_string(type) to_string_(FILE, LINE, type)
#define todo(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__); exit(1);
#define stop(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__); exit(1);
#define seg() \
{ \
    char str[12]; \
    str[-12] = 'e'; \
}

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT

#define AST_NODE(name, child_func, ...) \
Node *name() { \
    Node *left = child_func(); \
    Token *token; \
    while ((token = find(__VA_ARGS__, 0))) { \
        Node *node = new_node(token); \
        node->left = left; \
        node->right = child_func(); \
        left = node; \
    } \
    return left; \
}

// STRUCTS
typedef enum
{
    TMP = 1, CHILDREN, DEFAULT,
    // TODO: don't assign from reference if it does not have reference
    REF_ID, REF_HOLD_ID, REF_VAL, REF_HOLD_REF, REF_REF, ID_ID, ID_REF, ID_VAL,
    ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
    ADD, SUB, MUL, DIV, MOD,
    AND, OR, NOT,
    LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS,
    RETURN,
    IF, ELIF, ELSE,
    WHILE, CONTINUE, BREAK,
    FDEC, FCALL, PROTO,
    VOID, INT, CHARS, CHAR, BOOL, FLOAT, PTR, LONG, SHORT,
    STRUCT_DEF, STRUCT_CALL, ID, REF,
    ARRAY,
    JNE, JE, JMP, BLOC, END_BLOC,
    PUSH, POP,
    END
} Type;

typedef struct LLvm
{
    LLVMTypeRef funcType;
    LLVMValueRef element;
} LLvm;

typedef struct Token
{
    Type type;
    Type retType; // return type
    Type assign_type;

    char *name;
    // bool declare; // is variable declaration
    int space; // indentation
    bool remove;
    int ir_reg;
    // char *creg;
    int index;

    bool is_cond;
    bool is_ref;
    bool has_ref;
    bool declare;
    bool is_attr;
    bool is_proto;
    bool is_arg;
    int offset; // used for structs and []
    char *filename;
    int line;

    LLvm llvm;
    struct
    {
        // integer
        struct
        {
            long value;
            int power;
            struct Int *next;
        } Int;
        // long
        struct
        {
            long long value;
            int power;
            struct Int *next;
        } Long;
        // float
        struct
        {
            float value;
        } Float;
        // boolean
        struct
        {
            bool value;
            char c;
        } Bool;
        // chars
        struct
        {
            char *value;
        } Chars;
        // char
        struct
        {
            char value;
        } Char;
        // structure
        struct
        {
            char *name;
            struct Token **attrs;
            int pos;
            int len;
        } Struct;
        // function call
        struct
        {
            struct Token *ptr;
        } Fcall;
    };
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;

    struct Node **children;
    int cpos; // children pos
    int csize; // children size

    // bloc Infos
    struct {
        struct Node **functions;
        int fpos;
        int fsize;

        Token **structs;
        int spos;
        int ssize;

        Token **vars;
        int vpos;
        int vsize;
    };
} Node;

typedef struct Inst
{
    Token *token;
    Token *left;
    Token *right;

    struct Inst **children;
    int cpos;
    int csize;
} Inst;

// GLOBAL
extern bool found_error;

extern Token **tokens;
extern int tk_pos;
extern int tk_len;

extern char *input;
extern Node *global;
extern int exe_pos;
// extern Inst **OrgInsts;
// extern Inst **insts;

extern Node **Gscoop;
extern Node *scoop;
extern int scoopSize;
extern int scoopPos;

extern int ptr;
#if defined(__APPLE__)
extern struct __sFILE *asm_fd;
#elif defined(__linux__)
extern struct _IO_FILE *asm_fd;
#endif

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------

Token* new_token(Type type, int space);
void parse_token(char *input, int s, int e, Type type, int space, char *filename, int line);

void add_token(Token *token);
Node *expr();
Node *assign();
Node *logic();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *dot();
Node *sign();
Node *brackets();
Node *prime();
Node *new_node(Token *token);
bool includes(Type to_find, ...);
Token *find(Type type, ...);
void generate_ast();
Node *new_function(Node *node);
Node *get_function(char *name);
Token *get_variable(char *name);
Token *new_variable(Token *token);
void free_node(Node *node);
Token *copy_token(Token *token);
Node *copy_node(Node *node);
Token *new_struct(Token *token);
Token *get_struct(char *name);
Token *get_struct_by_id(int id);
Token *is_struct(Token *token);
void add_attribute(Token *obj, Token *attr);
Node* add_child(Node *node, Node *child);
void add_variable(Node *bloc, Token *token);
void set_struct_size(Token *token);

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
void generate(char *name);
Inst *new_inst(Token *token);
Inst* add_inst(Inst *parent, Inst *child);

void enter_scoop(Node *node);
void exit_scoop();
void copy_insts();
bool compatible(Token *left, Token *right);
void initialize();
void asm_space(int space);
void finalize();
void pasm(char *fmt, ...);
Token *generate_ir(Node *node, Inst *parent);
int calculate_padding(int offset, int alignment);
void generate_asm(char *name);
void to_default(Token *token, Type type);

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------
char* open_file(char *filename);
const char *to_string_(const char *filename, const int line, Type type);
void setName(Token *token, char *name);
void setReg(Token *token, char *creg);
bool within_space(int space);
bool check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(int line, int len, int size);
void create_builtin(char *name, Type *params, Type retType);
char *strjoin(char *str0, char *str1, char *str2);
int sizeofToken(Token *token);
int alignofToken(Token *token);
void add_builtins();
Type getRetType(Node *node);
bool optimize_ir();
void config();
void setAttrName(Token *parent, Token *child);
void create_struct(char *name, Token *attrs);
void set_remove(Node *node);
char* resolve_path(char* path);

#if DEBUG_INC_PTR
void inc_ptr_(char *filename, int line, int size);
#else
void inc_ptr(int size);
#endif

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
int debug(char *conv, ...);
int pnode(Node *node, char *side, int space);
int ptoken(Token *token);
void print_ast(Node *head);
void print_ir();
int print_value(Token *token);