#pragma once

// Headers
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/PassBuilder.h>
#include <llvm-c/DebugInfo.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
#define FUNC (char *)__func__
#define FILE (strrchr(__FILE__, '/') ? \
              strrchr(__FILE__, '/') + 1 : \
              (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
    : __FILE__))

#define TOKENIZE 1
#define TAB 3
#define AST 1
#define IR 1
#define OPTIMIZE 1
#define ASM 1

#ifndef DEBUG
#define DEBUG 1
#endif

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) \
        check_error(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)
#define todo(cond, fmt, ...)                                          \
        if (check_error(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)) \
        exit(1);
#define seg() raise(SIGSEGV)
#define expect_token(type, fmt, ...)          \
        {                                     \
           Token *find_token = find(type, 0); \
           if (!find_token)                   \
           {                                  \
              check(1, fmt, ## __VA_ARGS__);  \
              return syntax_error();          \
           }                                  \
        }


#if DEBUG
#define debug(fmt, ...) debug_(fmt, ## __VA_ARGS__)
#else
#define debug(fmt, ...)      \
        do                   \
        {                    \
        } while (0)
#endif

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, \
        LONG, PTR, SHORT, ARRAY_TYPE
#define LOGIC_TYPE AND, OR
#define MATH_TYPE ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define USING_HOIST false
// O0 is basically "Literal Translation" (Debug Mode)
#define PASSES_O0 "default<O0>"
// Removes basic redundancies and promotes memory to registers (mem2reg).
#define PASSES_O1 "default<O1>"
// Most production code uses this. Adds vectorization and aggressive inlining.
#define PASSES_O2 "default<O2>"
// Maximum speed optimizations, including heavy loop unrolling.
#define PASSES_O3 "default<O3>"
// Similar to O2, but disables optimizations that increase binary size.
#define PASSES_Os "default<Os>"
// Shrinks the code as much as possible, even if it makes it slower.
#define PASSES_Oz "default<Oz>"

#define AST_NODE(name, child_func, ...)           \
        Node *name()                              \
        {                                         \
           Node  *left = child_func();            \
           Token *token;                          \
           while ((token = find(__VA_ARGS__, 0))) \
           {                                      \
              Node *node = new_node(token);       \
              node->left  = left;                 \
              node->right = child_func();         \
              left        = node;                 \
           }                                      \
           return left;                           \
        }

// ----------------------------------------------------------------------------
// Type definitions
// ----------------------------------------------------------------------------
typedef struct Token Token;
typedef struct Node Node;
typedef struct LLVM LLVM;
typedef struct ExcepCTX ExcepCTX;
typedef enum Type Type;
typedef enum LogType LogType;

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind TypeKind;

#define PointerTypeKind LLVMPointerTypeKind
#define IntegerTypeKind LLVMIntegerTypeKind
#define VoidTypeKind    LLVMVoidTypeKind
#define FunctionTypeKind LLVMFunctionTypeKind
#define StructTypeKind  LLVMStructTypeKind
// ----------------------------------------------------------------------------
// Enums
// ----------------------------------------------------------------------------
enum Type
{
   ID = 1,
   // Data types
   VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR,
   CHARS, PTR, VARIADIC, REF, ARRAY, ARRAY_TYPE,
   // Structures
   STRUCT_DEF, STRUCT_CALL,
   // Assignment
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
   DIV_ASSIGN, MOD_ASSIGN,
   // Comparison
   EQUAL, NOT_EQUAL, LESS_EQUAL, GREAT,
   GREAT_EQUAL, LESS,
   // Arithmetic
   ADD, SUB, MUL, DIV, MOD,
   // Logical
   AND, OR, NOT,
   // Punctuation and Syntax
   LPAR, RPAR, LBRA, RBRA, COMA, DOT,
   DOTS, ACCESS, AS,
   // Control Flow
   RETURN, IF, ELIF, ELSE, END_IF,
   WHILE, CONTINUE, BREAK, END_BLOC,
   // Functions
   FDEC, FCALL, PROTO, ARGS, CHILDREN,
   // Built-ins
   STACK, HEAP, TYPEOF, SIZEOF, DEFAULT, SYNTAX_ERROR,
   // Bitwise
   BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT,
   // end
   END,
};

// ----------------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------------
struct LLVM
{
   bool is_set;
   bool is_loaded;
   Value array_size;
   Value elem;
   Block bloc;
   TypeRef funcType;
   TypeRef stType;
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
   int space;
   bool remove;

   int used;
   int pos;

   bool is_cond;
   bool is_ref;
   bool is_dec;
   bool is_param;
   bool is_cast;
   bool is_variadic;
   bool is_proto;
   bool is_init;
   bool is_method_call;

   bool ir_bound;
   bool asm_bound;

   char *filename;
   int line;

   LLVM llvm;

   struct
   {
      struct { long value;} Int;
      struct { int value;} Short;
      struct { long long value;} Long;
      struct { float value;} Float;
      struct { bool value;} Bool;
      struct { char *value;} Chars;
      struct { char value;} Char;
      struct { int index; Node *ptr; } Struct;
      struct { Type elem_type; int depth; } Array;
      struct { Node *ptr; } Fcall;
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

   Node **structs;
   int spos;
   int slen;

   Node **methods;
   int mlen;
   int mpos;
};

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
extern bool    found_error;

extern Token **tokens;
extern int     tk_pos;
extern int     tk_len;

extern Node   *global;
extern int     exe_pos;

extern Node  **Gscoop;
extern Node   *scoop;
extern int     scoop_len;
extern int     scoop_pos;

extern char  **used_files;
extern int     used_len;
extern int     used_pos;

extern Context context;
extern Module  module;
extern Builder builder;
extern TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
extern File    asm_fd;

extern char            *passes;
extern bool             enable_asan;

extern LLVMDIBuilderRef di_builder;
extern LLVMMetadataRef  di_compile_unit;
extern LLVMMetadataRef  di_file;
extern LLVMMetadataRef  di_current_scope;

// ----------------------------------------------------------------------------
// File Management
// ----------------------------------------------------------------------------
char *open_file(char *filename);
char *resolve_path(char *path);
bool  add_file(char *filename);
Node *syntax_error();


// ----------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------
void  *allocate_func(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);

// ----------------------------------------------------------------------------
// Error Handling
// ----------------------------------------------------------------------------
bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...);

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
void   tokenize(char *filename);
Token *new_token(Type type, int space);
Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
void   add_token(Token *token);
Node  *new_node(Token *token);
Node  *add_child(Node *node, Node *child);
void   enter_scoop(Node *node);
void   exit_scoop();
bool   includes(Type to_find, ...);
void   setName(Token *token, char *name);
char  *to_string(Type type);
Token *find(Type type, ...);
bool   within(int space);
bool   is_data_type(Token *token);

void   add_struct(Node *b, Node *node);
Node  *new_struct(Node *node);
Node  *get_struct(char *name);
void add_method(Node *b, Node *node);
Token *is_struct(Token *token);
void   add_attribute(Token *obj, Token *attr);
Node  *syntax_error();

Node *expr_node();
Node *assign_node();
Node *logic_and_node();
Node *logic_or_node();
Node *bitor_node();
Node *bitxor_node();
Node *bitnot_node();
Node *bitand_node();
Node *equality_node();
Node *comparison_node();
Node *shift_node();
Node *add_sub_node();
Node *mul_div_node();
Node *as_node();
Node *unary_node();
Node *access_node();
Node *prime_node();

// ----------------------------------------------------------------------------
// IR / Scope / Variable Management
// ----------------------------------------------------------------------------
void   gen_ir(Node *node);
void   add_variable(Node *b, Token *token);
Token *new_variable(Token *token);
Token *get_variable(char *name);
void   add_function(Node *b, Node *node);
Node  *new_function(Node *node);
Node  *get_function(char *name);

// ----------------------------------------------------------------------------
// ASM / Code Generation
// ----------------------------------------------------------------------------
void    gen_asm(Node *node);
void    init(char *name);
void    finalize(char *output);
void    load_if_necessary(Node *node);
void    _alloca(Token *token);
void    initialize_variable(Token *token);
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
void    _entry(Token *token);
Value   _get_default_value(Token *token);
Value   _build_return(Token *token);
Value   _get_param_with_name(Token *fn, int index, char *name);
void    _load(Token *to, Token *from);
Value   _load2(Token *token);
bool    compatible(Token *left, Token *right);

// ----------------------------------------------------------------------------
// Debugging
// ----------------------------------------------------------------------------
int  debug_(char *conv, ...);
void pnode(Node *node, char *indent);

// ----------------------------------------------------------------------------
// String Utilities
// ----------------------------------------------------------------------------
char *strjoin(char *str0, char *str1, char *str2);

// ----------------------------------------------------------------------------
// LLVM Wrappers
// ----------------------------------------------------------------------------
Value _build_store(Value val, Value ptr);
Value _build_load2(TypeRef ty, Value ptr, char *name);
Value _build_alloca(TypeRef ty, char *name);
Value _build_add(Value lhs, Value rhs, char *name);
Value _build_sub(Value lhs, Value rhs, char *name);
Value _build_mul(Value lhs, Value rhs, char *name);
Value _build_sdiv(Value lhs, Value rhs, char *name);
Value _build_srem(Value lhs, Value rhs, char *name);
Value _build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name);
Value _build_and(Value lhs, Value rhs, char *name);
Value _build_or(Value lhs, Value rhs, char *name);
Value _build_ret(Value val);
Value _build_ret_void();
void  _branch(Block bloc);
void  _condition(Value cond, Block then_block, Block else_block);
Block _append_block(char *name);
Value _build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name);
Value _const_chars(char *str, char *name);
Value _build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned indece, char *name);
Value _build_bit_cast(Value val, TypeRef dest_ty, char *name);
Value _build_sext(Value val, TypeRef dest_ty, char *name);
Value _build_trunc(Value val, TypeRef dest_ty, char *name);
Value _build_int_to_ptr(Value val, TypeRef dest_ty, char *name);
Value _build_ptr_to_int(Value val, TypeRef dest_ty, char *name);
Value _build_array_alloca(TypeRef ty, Value val, char *name);
Value _build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args, Block then_block, Block catch_block, char *name);
Value _build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses, char *name);
Value _build_extract_value(Value agg_val, unsigned index, char *name);
Value _build_va_arg(Value list, TypeRef ty, char *name);
Value _build_unreachable();
Value _build_not(Token *token, char*name);
Value _build_memcpy(Value dest, Value src, Value size);

Value _const_int(TypeRef ref_type, unsigned long long n, int sign_extend);
Value _const_null(TypeRef ty);

TypeRef _pointer_type(TypeRef element_ty, unsigned address_space);
TypeRef _function_type(TypeRef retType, TypeRef *types, int count, int is_variadic);
TypeRef _array_type(TypeRef element_type, unsigned element_count);
TypeRef _struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed);

Value       _get_named_function(char *name);
Value       _add_function(char *name, TypeRef function_type);
Value       _get_param(Value fn, unsigned index);
Block       _append_basic_block_in_context(Value func, char *name);
Block       _get_insert_block();
Value       _get_basic_block_parent(Block block);
Block       _get_entry_basic_block(Value func);
void        _position_at(Block block);
Value       _get_basic_block_terminator(Block block);
TypeRef     _type_of(Value val);
TypeKind    _get_type_kind(TypeRef ty);
unsigned    _get_int_type_width(TypeRef ref_ty);
TypeRef     _get_type_by_name2(char *name);
TypeRef     _global_get_value_type(Value global);
TypeRef     _get_return_type(TypeRef function_type);
const char *_get_target(Module m);
void        _set_personality_fn(Value func, Value pers_fn);
void        _add_clause(Value landing_pad, Value clause_val);
size_t      _abi_size_of_type(TargetData td, TypeRef ty);
TargetData  _get_module_data_layout(Module m);
Value   build_binary_op(Type op_type, Value lref, Value rref);
Value   allocate_stack(Value size, TypeRef elementType, char *name);
Value   allocate_heap(Value count, TypeRef elementType, char *name);
Value   get_store_ptr(Token *token);
Value   struct_field_ptr(Token *struct_tok, int field_index, char *name);
void   set_ret_type(Node *node);
void **resize_array(void **array, int *len, int pos, int element_size);
void set_debug_location(Token *token);
