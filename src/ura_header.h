#pragma once

// HEADERS
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <libgen.h>
#include <limits.h>
#include <sys/types.h>
#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>

// MACROS
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC (char*)__func__
#define FILE (char*)__FILE__

#define TOKENIZE 1
#define TAB 3
#define AST 1
#define IR 1
#define ASM 1

#ifndef DEBUG
#define DEBUG 0
#endif

#if ASM
#define OPTIMIZE 1
#else
#define OPTIMIZE 1
#endif

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define to_string(type) to_string_(FILE, LINE, type)
#define todo(cond, fmt, ...) if (check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define seg() raise(SIGSEGV);

#if DEBUG
#define debug(fmt, ...) print(fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...) do { } while (0)
#endif

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT
#define LOGIC_TYPE AND, OR
#define MATH_TYPE ADD, SUB, MUL, DIV, MOD

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

// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct Inst Inst;
typedef enum Type Type;
typedef enum LogType LogType;
typedef struct LLVM LLVM;

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef ContextRef;
typedef LLVMModuleRef ModuleRef;
typedef LLVMBuilderRef BuilderRef;
typedef LLVMBasicBlockRef BasicBlockRef;
typedef LLVMValueRef ValueRef;

// STRUCTS
enum LogType
{
   SYMBOL = 1,
   VALUE,
};

enum Type
{
   END = 1,

   ID, REF,
   REF_ID, REF_HOLD_ID, REF_VAL, REF_HOLD_REF, REF_REF,
   ID_ID, ID_REF, ID_VAL,

   // Data types
   VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR,
   VARIADIC, STACK, TYPEOF,
   ARRAY, AS,

   // Struct Usage
   STRUCT_DEF, STRUCT_BODY, STRUCT_ALLOC, STRUCT_CALL,

   // Assignment
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
   // Comparison
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   // Arithmetic
   ADD, SUB, MUL, DIV, MOD,
   // Logical
   AND, OR, NOT,

   // Punctuation & Syntax
   LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS,

   // Control Flow
   RETURN, ARROW,
   IF, ELIF, ELSE, END_IF, BUILD_COND,
   WHILE, CONTINUE, BREAK, END_WHILE,
   APPEND_BLOC, SET_POS, BUILD_BR,
   END_BLOC,

   // Functions
   FDEC, FCALL, PROTO_FUNC, CHILDREN,
};

struct LLVM
{
   bool is_set;
   ValueRef elem;
   BasicBlockRef bloc;
   TypeRef type;
   TypeRef funcType;
};

struct Token
{
   Type type;
   Type retType;
   Type assign_type;
   LogType logType;
   char *logName;

   char *name;
   int space;
   bool remove;

   int ir_reg;
   int used;
   int pos;

   bool is_cond;
   bool is_ref;
   bool has_ref;
   bool is_declare;
   // bool is_attr;
   bool is_proto;
   bool is_arg;
   bool is_param;
   // bool is_call_param;
   bool is_cast;
   bool is_variadic;

   char *filename;
   int line;

   LLVM llvm;

   struct
   {
      // integer
      struct { long value; } Int;
      // short
      struct { int value; } Short;
      // long
      struct { long long value; } Long;
      // float
      struct { float value; } Float;
      // boolean
      struct { bool value; } Bool;
      // chars
      struct { char *value; } Chars;
      // char
      struct { char value; } Char;
      // structure
      struct
      {
         Token *ptr;
         char *name;
         Token **attrs;
         int size;
         int pos;
         int index; // attribute index
      } Struct;
      struct
      {
         Token *func_ptr;
         int index;
      } Param;
      // function call
      struct
      {
         Token *func_ptr; // function declaration
         Token **args;
         int size;
         int pos;
      } Fcall;
      // function declaration
      struct
      {
         Token **args;
         int size;
         int pos;
      } Fdec;
      // statement
      struct
      {
         Token *ptr;
         Token *start;
         Token *end;
      } Statement;
   };
};

struct Node
{
   Node *left;
   Node *right;
   Token *token;

   Node **children;
   int cpos; // children pos
   int csize; // children size

   // bloc Infos
   struct
   {
      Node **functions;
      int fpos;
      int fsize;

      Token **structs;
      int spos;
      int ssize;

      Token **vars;
      int vpos;
      int vsize;
   };
};

struct Inst
{
   // bool remove;
   Token *token;
   Token *left;
   Token *right;
};

// GLOBAL
extern bool found_error;

extern Token **tokens;
extern int tk_pos;
extern int tk_len;

extern Node *global;
extern int exe_pos;
extern Inst **OrgInsts;
extern Inst **insts;

extern Node **Gscoop;
extern Node *scoop;
extern int scoopSize;
extern int scoopPos;

extern char **used_files;
extern int used_size;
extern int used_pos;

extern ContextRef context;
extern ModuleRef module;
extern BuilderRef builder;
extern TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

#if defined(__APPLE__)
extern struct __sFILE *asm_fd;
#elif defined(__linux__)
extern struct _IO_FILE *asm_fd;
#endif

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
void tokenize(char *filename);
Token* new_token(Type type, int space);
Token* parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
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
Node *cast_node();
Node *prime();
Node *new_node(Token *token);
bool includes(Type to_find, ...);
Token *find(Type type, ...);
Node *new_function(Node *node);
Node *get_function(char *name);
Token *get_variable(char *name);
Token *new_variable(Token *token);
void free_node(Node *node);
Token *copy_token(Token *token);
Node *copy_node(Node *node);
Token *new_struct(Token *token);
Token *get_struct(char *name);
Token *is_struct(Token *token);
void add_attribute(Token *obj, Token *attr);
Node* add_child(Node *node, Node *child);
void add_variable(Node *bloc, Token *token);
void add_struct(Node *bloc, Token *token);

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
Inst *new_inst(Token *token);
void add_inst(Inst *inst);
void enter_scoop(Node *node);
void exit_scoop();
void copy_insts();
bool compatible(Token *left, Token *right);
Token *generate_ir(Node *node);
void handle_asm(Inst *inst);
TypeRef get_llvm_type(Token* token);
ValueRef get_value(Token *token);
void enter_func(ValueRef func);
void exit_func();
ValueRef get_current_func();
void init(char *name);
void finalize(char *moduleName);
ValueRef load_variable(Token *token);
ValueRef create_string(char *value);
TypeRef get_llvm_type(Token *token);
ValueRef get_value(Token *token);
ValueRef llvm_get_ref(Token *token);
void create_function(Token *token);
void call_function(Token *token, LLVM *source, ValueRef *args, int argsCount);
BasicBlockRef create_bloc(char *name);
void branch(BasicBlockRef bloc);
void open_block(BasicBlockRef bloc);
ValueRef load_variable(Token *token);
ValueRef assign2(Token *variable, Token* value);
ValueRef operation(Token *token, Token* left, Token* right);
ValueRef NotOperation(Token *token);
ValueRef return_(ValueRef value);
ValueRef allocate_variable(TypeRef type, char *name);
ValueRef get_param(Token *token);
void build_condition(Token* curr, Token *left, Token* right);
ValueRef access_(Token *curr, Token *left, Token *right);
ValueRef cast(Token *from, Token *to);
ValueRef allocate_stack(ValueRef size, TypeRef elementType, char *name);
bool did_opimize();

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------
char* open_file(char *filename);
bool add_file(char *filename);
char *to_string_(char *filename, int line, Type type);
void setName(Token *token, char *name);
bool within_space(int space);
bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(int line, int len, int size);
char *strjoin(char *str0, char *str1, char *str2);
Type getRetType(Node *node);
char* resolve_path(char* path);

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
int print(char *conv, ...);
int pnode(Node *node, char *side, int space);
int ptoken(Token *token);
void print_ast(Node *head);
void print_ir();
int print_escaped(char *str) ;
int print_value(Token *token);

