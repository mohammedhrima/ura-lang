#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef ContextRef;
typedef LLVMModuleRef ModuleRef;
typedef LLVMBuilderRef BuilderRef;
typedef LLVMBasicBlockRef BasicBlockRef;
typedef LLVMValueRef ValueRef;

typedef enum Type Type;
typedef struct Token Token;
typedef struct Node Node;

#if defined(__APPLE__)
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

#define seg() raise(SIGSEGV);
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
#define TAB 4
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"

#define DATA_TYPES INT, LONG, CHARS, CHAR, BOOL, VOID
#define MATH_OP ADD, SUB, MUL, DIV

enum Type {
   ID = 1, CHAR, CHARS, INT, LONG, BOOL, VOID,
   LPAR, RPAR, FDEC, FCALL, NEWLINE,
   IF, ELIF, ELSE, WHILE, RETURN, END_BLOCK,
   ADD, SUB, MUL, DIV, MOD, COMA,
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
   DIV_ASSIGN, MOD_ASSIGN,
   EQUAL, NOT_EQUAL, LESS, MORE,
   LESS_EQUAL, MORE_EQUAL,
   AND, OR,
   COLON, COMMA, DOTS,
   LBRACKET, RBRACKET, PROTO, VARIADIC, REF,
   VA_LIST, ACCESS, LBRA, RBRA,
   AS, STACK,
   TRY, CATCH, THROW,
   USE,
   DOT, MEMBER_ACCESS,

   END,
};

struct Token
{
   Type type;
   Type cast_type;
   int line;
   int pos;
   char *name;
   char *llvm_name;
   bool is_dec;
   bool is_ref;
   bool has_ref;
   int space;

   struct
   {
      ValueRef elem;
      LLVMValueRef va_count;
      LLVMValueRef error_flag;
      LLVMValueRef error_value;
      BasicBlockRef catch_block;
      BasicBlockRef landing_pad;
   } llvm;

   struct
   {
      struct { long value; } Int;
      struct { char *value; } Chars;
      struct { char value; } Char;
      struct { Type retType; Token **args; int args_len; bool is_variadic; } Fdec;
      struct { Token **args; int args_len; } Fcall;
      struct { Type error_type; char *error_name; } Catch;
   };
};

struct Node
{
   Node *left;
   Node *right;
   Token *token;

   Node **children;
   int cpos;
   int clen;

   Node **variables;
   int vpos;
   int vlen;

   Node **functions;
   int fpos;
   int flen;
};

extern ContextRef context;
extern ModuleRef module;
extern BuilderRef builder;
extern TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

extern ValueRef boundsCheckFunc;
extern ValueRef nullCheckFunc;
extern ValueRef vaStartFunc;
extern ValueRef vaEndFunc;
extern char *importedFiles[100];
extern int importedFileCount;
extern int block_counter;
extern Token *tokens[1000];


Node *expr_node();
Node *assign_node();
Node *logic_node();
Node *equality_node();
Node *comparison_node();
Node *add_sub_node();
Node *mul_div_node();
Node *dot_node();
Node *sign_node();
Node *brackets_node();
Node *cast_node();
Node *prime_node();

void create_function(Token *func);
ValueRef build_va_start(ValueRef va_list_ptr, ValueRef last_param_ptr);
ValueRef build_va_arg(ValueRef va_list_ptr, TypeRef type);
ValueRef build_va_end(ValueRef va_list_ptr);
void skip_newlines();
void tokenize();
char *to_string(Type type);
char *strjoin(char *str, ...);
char* open_file(char *filename);
TypeRef get_llvm_type(Token *token);
bool includes(Type to_find, ...);
void create_int(Token *token);
ValueRef create_string(char *value);
char *substr(char *input, int s, int e);
void ptoken(Token *token);
void pnode(Node *node, char *side, int space);
void free_tokens();
void init(char *name);
void finalize();
Token *copy_token(Token *token);
Token *get_function(char *name);
BasicBlockRef _append_block(char *name);
void load_if_neccessary(Node *node);
void add_variable(Node *node);
void generate_ir(Node *node);
void exit_scoop();
void _branch(BasicBlockRef bloc);
void enter_scoop(Node *node);









