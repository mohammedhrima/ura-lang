#pragma once

// Headers
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

// Macros
#if defined(__APPLE__)
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

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
#define DEBUG 1
#endif

#if ASM
#define OPTIMIZE 1
#else
#define OPTIMIZE 1
#endif

#define allocate(len, size) \
   allocate_func(LINE, len, size)
#define check(cond, fmt, ...) \
   check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
// #define to_string(type) to_string_(FILE, LINE, type)
#define todo(cond, fmt, ...) \
   if (check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) \
      exit(1);
#define seg() raise(SIGSEGV);

#if DEBUG
#define debug(fmt, ...) debug_(fmt, ##__VA_ARGS__)
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

// enums / structs
typedef struct Token Token;
typedef struct Node Node;
typedef struct LLVM LLVM;
typedef enum Type Type;
typedef enum LogType LogType;
typedef struct ExcepCTX ExcepCTX;

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind TypeKind;

enum Type
{
   ID = 1,
   // Data types
   VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR,
   VARIADIC, REF,

   // Structures
   STRUCT_DEF, STRUCT_CALL,

   // Assignment
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
   // Comparison
   EQUAL, NOT_EQUAL, LESS_EQUAL, GREAT_EQUAL, LESS, GREAT,
   // Arithmetic
   ADD, SUB, MUL, DIV, MOD,
   // Logical
   AND, OR, NOT,

   // Punctuation and Syntax
   LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS,
   AS, TYPEOF,

   // Control Flow
   RETURN,
   IF, ELIF, ELSE, END_IF,
   WHILE, CONTINUE, BREAK,
   END_BLOC,

   // Functions
   FDEC, FCALL, PROTO,

   // Built ins
   STACK,

   SYNTAX_ERROR,
   END,
};

struct LLVM
{
   bool is_set;
   bool is_loaded;
   Value array_size;
   Value elem;
   Block bloc;
   TypeRef funcType;
   Value va_count;
   Value error_flag;
   Value error_value;
   Block catch;
   Block lpad;

   // statements/loops
   Block start;
   Block then;
   Block end;
};

struct ExcepCTX
{
   Block lpad;
   Block catch;
   Block end;
   Value storage;
   Type type;
   bool in_catch;
};

struct Token
{
   Type type;
   Type retType;
   Type assign_type;

   char *name;
   char *llvm_name;
   int space;
   bool remove;

   int used;
   int pos;

   bool is_cond;

   // TODO: to be fixed later, has_ref
   // must depends on scoop position
   // bool has_ref;
   bool is_ref;

   bool is_dec;
   bool is_arg;
   bool is_param;
   bool is_cast;
   bool is_variadic;
   bool is_proto;

   char *filename;
   int line;

   LLVM llvm;

   struct
   {

      struct { long value; } Int;
      struct { int value; } Short;
      struct { long long value; } Long;
      struct { float value; } Float;
      struct { bool value; } Bool;
      struct { char *value; } Chars;
      struct { char value; } Char;
      struct { char *name; Token **attrs; int size; int pos; int index; } Struct;
      // struct { Token *ptr; int index; } Param;
      struct { Token *ptr; } Fcall;
      // struct { bool is_variadic; } Fdec;
      struct { Token *ptr; Token *start; Token *end; } Statement;
      struct { Type type; char *name; } Catch;
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

   Token **variables;
   int vpos;
   int vlen;

   Node **functions;
   int fpos;
   int flen;

   Token **structs;
   int spos;
   int slen;
};

// GLOBAL
extern bool found_error;

extern Token **tokens;
extern int tk_pos;
extern int tk_len;

extern Node *global;
extern int exe_pos;

extern Node **Gscoop;
extern Node *scoop;
extern int scoopSize;
extern int scoop_pos;

extern char **used_files;
extern int used_size;
extern int used_pos;

extern Context context;
extern Module module;
extern Builder builder;
extern TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
extern File asm_fd;

extern bool enable_bounds_check;
extern Value boundsCheckFunc;
// extern Value nullCheckFunc;
extern Value vaStartFunc;
extern Value vaEndFunc;
extern bool using_refs;
// extern Value refAssignFunc;

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
void tokenize(char *filename);
Token* new_token(Type type, int space);
Token* parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
void add_token(Token *token);
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
Node *new_node(Token *token);
bool includes(Type to_find, ...);
Token *find(Type type, ...);
Token* expect_token(Type type, char *error_msg, ...);
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
Token *syntax_error_token();
Node *syntax_error_node();

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
void enter_scoop(Node *node);
void exit_scoop();
bool compatible(Token *left, Token *right);
void gen_ir(Node *node);
TypeRef get_llvm_type(Token* token);
void init(char *name);
void finalize(char *moduleName);
TypeRef get_llvm_type(Token *token);
Value allocate_stack(Value size, TypeRef elementType, char *name);
void _load(Token *to, Token *from);
void gen_asm(Node *node);

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------
char* open_file(char *filename);
bool add_file(char *filename);
char *to_string(Type type);
void setName(Token *token, char *name);
bool within(int space);
bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(int line, int len, int size);
char *strjoin(char *str0, char *str1, char *str2);
char* resolve_path(char* path);
Value create_null_check_function();

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
int debug_(char *conv, ...);
int pnode(Node *node, char *indent);
int ptoken(Token *token);
int print_escaped(char *str) ;
int print_value(Token *token);

// ----------------------------------------------------------------------------
// LLVM Wrappers
// ----------------------------------------------------------------------------
// Builder operations
Value llvm_build_store(Value val, Value ptr);
Value llvm_build_load2(TypeRef ty, Value ptr, char *name);
Value llvm_build_alloca(TypeRef ty, char *name);
Value llvm_build_add(Value lhs, Value rhs, char *name);
Value llvm_build_sub(Value lhs, Value rhs, char *name);
Value llvm_build_mul(Value lhs, Value rhs, char *name);
Value llvm_build_sdiv(Value lhs, Value rhs, char *name);
Value llvm_build_srem(Value lhs, Value rhs, char *name);
Value llvm_build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name);
Value llvm_build_and(Value lhs, Value rhs, char *name);
Value llvm_build_or(Value lhs, Value rhs, char *name);
Value llvm_build_ret(Value val);
Value llvm_build_ret_void();
void _branch(Block bloc);
void _condition(Value cond, Block then_block, Block else_block);
Block _append_block(char *name);
Value llvm_build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name);
Value llvm_build_global_string_ptr(char *str, char *name);
Value llvm_build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned num_indices, char *name);
Value llvm_build_bit_cast(Value val, TypeRef dest_ty, char *name);
Value llvm_build_sext(Value val, TypeRef dest_ty, char *name);
Value llvm_build_trunc(Value val, TypeRef dest_ty, char *name);
Value llvm_build_int_to_ptr(Value val, TypeRef dest_ty, char *name);
Value llvm_build_ptr_to_int(Value val, TypeRef dest_ty, char *name);
Value llvm_build_array_alloca(TypeRef ty, Value val, char *name);
Value llvm_build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args, Block then_block, Block catch_block, char *name);
Value llvm_build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses, char *name);
Value llvm_build_extract_value(Value agg_val, unsigned index, char *name);
Value llvm_build_va_arg(Value list, TypeRef ty, char *name);
Value llvm_build_unreachable();
Value llvm_build_global_string_ptr_raw(char *str, char *name);

TypeRef llvm_pointer_type(TypeRef element_ty, unsigned address_space);
TypeRef llvm_function_type(TypeRef retType, TypeRef *types, int param_count, int is_var_arg);
TypeRef llvm_array_type(TypeRef element_type, unsigned element_count);
Value llvm_const_int(TypeRef int_type, unsigned long long n, int sign_extend);
Value llvm_get_named_function(char *name);
Value llvm_add_function(char *name, TypeRef function_type);
Value llvm_get_param(Value fn, unsigned index);
Block llvm_append_basic_block_in_context(Value func, char *name);
Block llvm_get_insert_block();
Value llvm_get_basic_block_parent(Block block);
Block llvm_get_entry_basic_block(Value func);
void _position_at(Block block);
Value llvm_get_basic_block_terminator(Block block);
TypeRef llvm_type_of(Value val);
TypeKind llvm_get_type_kind(TypeRef ty);
unsigned llvm_get_int_type_width(TypeRef int_ty);
TypeRef llvm_get_type_by_name2(char *name);
TypeRef llvm_struct_type_in_context(TypeRef *types, unsigned count, int packed);
TypeRef llvm_global_get_value_type(Value global);
TypeRef llvm_get_return_type(TypeRef function_type);
const char *llvm_get_target(Module m);
void llvm_set_personality_fn(Value func, Value pers_fn);
void llvm_add_clause(Value landing_pad, Value clause_val);
size_t llvm_abi_size_of_type(TargetData td, TypeRef ty);
TargetData llvm_get_module_data_layout(Module m);
Value llvm_build_not(Token *token);
Value llvm_const_null(TypeRef ty);
TypeRef get_llvm_type(Token *token);
Value check_null(Token *token);
Value deref_or_load(Token *token);
Value getNullCheckFunc();
Value getRefAssignFunc();
