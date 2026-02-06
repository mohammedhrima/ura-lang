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
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
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


   END,
};

struct LLVM
{
   bool is_set;
   bool is_loaded;
   Value array_size;
   Value elem;
   Value va_count;
   Value error_flag;
   Value error_value;
   Block catch;
   Block lpad;
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

   int ir_reg;
   int used;
   int pos;

   bool is_cond;
   bool is_ref;
   bool has_ref;
   bool is_dec;
   bool is_arg;
   bool is_param;
   bool is_cast;
   bool is_variadic;

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
      struct { Token *func_ptr; int index; } Param;
      struct { Token **args; Token *ptr; int size; int pos; } Fcall;
      struct { Token **args; int pos; int len; bool is_variadic; } Fdec;
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

extern Value boundsCheckFunc;
extern Value nullCheckFunc;
extern Value vaStartFunc;
extern Value vaEndFunc;

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
void enter_scoop(Node *node);
void exit_scoop();
void copy_insts();
bool compatible(Token *left, Token *right);
void generate_ir(Node *node);
TypeRef get_llvm_type(Token* token);
Value get_value(Token *token);
void enter_func(Value func);
void exit_func();
Value get_current_func();
void init(char *name);
void finalize(char *moduleName);
Value load_variable(Token *token);
Value create_string(char *value);
TypeRef get_llvm_type(Token *token);
Value get_value(Token *token);
Value llvm_get_ref(Token *token);
void create_function(Token *token);
void call_function(Token *curr);
Block create_bloc(char *name);
void branch(Block bloc);
void open_block(Block bloc);
Value load_variable(Token *token);
Value assign2(Token *variable, Token* value);
Value operation(Token *token, Token* left, Token* right);
Value NotOperation(Token *token);
Value return_(Value value);
Value allocate_variable(TypeRef type, char *name);
Value get_param(Token *token);
void build_condition(Token* curr, Token *left, Token* right);
Value access_(Token *curr, Token *left, Token *right);
Value cast(Token *from, Token *to);
Value allocate_stack(Value size, TypeRef elementType, char *name);
bool did_opimize();
Value safe_access_(Token *curr, Token *left, Token *right);
void store_array_size(Value array_ptr, Value size);
Value get_array_size(Value array_ptr);
void handle_asm(Node *node);
void _load(Token *to, Token *from);


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
Type getRetType(Node *node);
char* resolve_path(char* path);

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
int debug_(char *conv, ...);
int pnode(Node *node, char *side, int space);
int ptoken(Token *token);
void print_ast(Node *head);
void print_ir();
int print_escaped(char *str) ;
int print_value(Token *token);
void print_inst(Node *node);

// ----------------------------------------------------------------------------
// LLVM Wrappers
// ----------------------------------------------------------------------------
// Builder operations
Value llvm_build_store(Token *token, Value val, Value ptr);
Value llvm_build_load2(Token *token, TypeRef ty, Value ptr, char *name);
Value llvm_build_alloca(Token *token, TypeRef ty, char *name);
Value llvm_build_add(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_sub(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_mul(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_sdiv(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_srem(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_icmp(Token *token, LLVMIntPredicate op, Value lhs, Value rhs, char *name);
Value llvm_build_and(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_or(Token *token, Value lhs, Value rhs, char *name);
Value llvm_build_ret(Token *token, Value val);
Value llvm_build_ret_void(Token *token);
Value llvm_build_br(Token *token, Block dest);
Value llvm_build_cond_br(Token *token, Value cond, Block then_block, Block else_block);
Value llvm_build_call2(Token *token, TypeRef ty, Value fn, Value *args, unsigned num_args,
                       char *name);
Value llvm_build_global_string_ptr(Token *token, const char *str, char *name);
Value llvm_build_gep2(Token *token, TypeRef ty, Value ptr, Value *indices, unsigned num_indices,
                      char *name);
Value llvm_build_bit_cast(Token *token, Value val, TypeRef dest_ty, char *name);
Value llvm_build_sext(Token *token, Value val, TypeRef dest_ty, char *name);
Value llvm_build_trunc(Token *token, Value val, TypeRef dest_ty, char *name);
Value llvm_build_int_to_ptr(Token *token, Value val, TypeRef dest_ty, char *name);
Value llvm_build_ptr_to_int(Token *token, Value val, TypeRef dest_ty, char *name);
Value llvm_build_array_alloca(Token *token, TypeRef ty, Value val, char *name);
Value llvm_build_invoke2(Token *token, TypeRef ty, Value fn, Value *args, unsigned num_args,
                         Block then_block, Block catch_block, char *name);
Value llvm_build_landing_pad(Token *token, TypeRef ty, Value pers_fn, unsigned num_clauses,
                             char *name);
Value llvm_build_extract_value(Token *token, Value agg_val, unsigned index, char *name);
Value llvm_build_va_arg(Token *token, Value list, TypeRef ty, char *name);
Value llvm_build_unreachable(Token *token);
Value llvm_build_global_string_ptr_raw(const char *str, char *name);

// Type creation wrappers
TypeRef llvm_pointer_type(TypeRef element_ty, unsigned address_space);
TypeRef llvm_function_type(TypeRef return_type, TypeRef *param_types, unsigned param_count,
                           int is_var_arg);
TypeRef llvm_array_type(TypeRef element_type, unsigned element_count);

// Constant creation wrappers
Value llvm_const_int(TypeRef int_type, unsigned long long n, int sign_extend);

// Function management wrappers
Value llvm_get_named_function(char *name);
Value llvm_add_function(char *name, TypeRef function_type);
Value llvm_get_param(Value fn, unsigned index);

// Block management wrappers
Block llvm_append_basic_block_in_context(Value func, char *name);
Block llvm_get_insert_block();
Value llvm_get_basic_block_parent(Block block);
Block llvm_get_entry_basic_block(Value func);
void llvm_position_builder_at_end(Block block);
Value llvm_get_basic_block_terminator(Block block);

// Type queries
TypeRef llvm_type_of(Value val);
LLVMTypeKind llvm_get_type_kind(TypeRef ty);
unsigned llvm_get_int_type_width(TypeRef int_ty);

// Type lookup wrappers
TypeRef llvm_get_type_by_name2(char *name);
TypeRef llvm_struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed);

// Function type queries
TypeRef llvm_global_get_value_type(Value global);
TypeRef llvm_get_return_type(TypeRef function_type);

// Module and target information
const char *llvm_get_target(LLVMModuleRef m);

// Personality function
void llvm_set_personality_fn(Value func, Value pers_fn);

// Landing pad clauses
void llvm_add_clause(Value landing_pad, Value clause_val);

// Data layout information
size_t llvm_abi_size_of_type(LLVMTargetDataRef td, TypeRef ty);
LLVMTargetDataRef llvm_get_module_data_layout(LLVMModuleRef m);

// Const null value
Value llvm_const_null(TypeRef ty);
TypeRef get_llvm_type(Token *token);


