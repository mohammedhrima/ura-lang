#ifndef HEADER_H
#define HEADER_H

// ============================================================
// HEADERS
// ============================================================
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
#include <llvm-c/Transforms/PassBuilder.h>
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

#if defined(__APPLE__)
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

#define SPLIT      "=================================================\n"
#define RESET      "\033[0m"
#define BOLD       "\e[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
#define BLUE(fmt)  BOLD "\x1b[34m" fmt RESET

#define LINE __LINE__
#define FUNC (char *)__func__
#define FILE                                                                                       \
	(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1                                            \
	                        : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
#define URA_MAX_SIZE 999999

#define TOKENIZE 1
#define TAB      3
#define AST      1
#define IR       1
#define OPTIMIZE 1
#define ASM      1

#ifndef DEBUG
#define DEBUG 1
#endif

#define allocate(len, size)   allocate_func(LINE, len, size)
#define check(cond, fmt, ...) _check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define todo(cond, fmt, ...)                                                                       \
	if (_check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define seg() raise(SIGSEGV)
#define expect_token(type, fmt, ...)                                                               \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			check(1, fmt, ##__VA_ARGS__);                                                             \
			return syntax_error();                                                                    \
		}                                                                                            \
	}

#if DEBUG
#define debug(fmt, ...) _debug(fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)                                                                            \
	do {                                                                                            \
	} while (0)
#endif

#define DATA_TYPES     INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT, ARRAY_TYPE
#define LOGIC_TYPE     AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN
#define USING_HOIST    false
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

#define AST_NODE(name, child_func, ...)                                                            \
	Node *name() {                                                                                  \
		Node  *left = child_func();                                                                  \
		Token *token;                                                                                \
		while ((token = find(__VA_ARGS__, 0))) {                                                     \
			Node *node  = new_node(token);                                                            \
			node->left  = left;                                                                       \
			node->right = child_func();                                                               \
			left        = node;                                                                       \
		}                                                                                            \
		return left;                                                                                 \
	}

// ----------------------------------------------------------------------------
// Type definitions
// ----------------------------------------------------------------------------
typedef struct Token      Token;
typedef struct Types      Types;
typedef struct Node       Node;
typedef struct LLVM       LLVM;
typedef struct ExcepCTX   ExcepCTX;
typedef enum Type         Type;
typedef enum LogType      LogType;

typedef LLVMTypeRef       TypeRef;
typedef LLVMContextRef    Context;
typedef LLVMModuleRef     Module;
typedef LLVMBuilderRef    Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef      Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind      TypeKind;

#define PointerType  LLVMPointerTypeKind
#define IntegerType  LLVMIntegerTypeKind
#define FloatType    LLVMFloatTypeKind
#define DoubleType   LLVMDoubleTypeKind
#define VoidType     LLVMVoidTypeKind
#define FunctionType LLVMFunctionTypeKind
#define StructType   LLVMStructTypeKind

// ----------------------------------------------------------------------------
// Enums
// ----------------------------------------------------------------------------
enum Type
{
	ID = 1,
	// Data types
	VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR, VARIADIC, REF,
	ARRAY, ARRAY_TYPE,
	// Structures
	STRUCT_DEF, STRUCT_CALL,
	// Enums
	ENUM_DEF, ENUM_CALL,
	// Tuples
	TUPLE, TUPLE_UNPACK,
	// Assignment
	ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
	// Comparison
	EQUAL, NOT_EQUAL, LESS_EQUAL, GREAT, GREAT_EQUAL, LESS,
	// Arithmetic
	ADD, SUB, MUL, DIV, MOD,
	// Logical
	AND, OR, NOT,
	// Punctuation and Syntax
	LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS, AS,
	// Control Flow
	RETURN, IF, ELIF, ELSE, WHILE, CONTINUE, BREAK,
	// Functions
	FDEC, FCALL, PROTO, ARGS, CHILDREN,
	// Built-ins
	STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, SYNTAX_ERROR,
	// Bitwise
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT,
	// Literals
	NULLABLE,
	// Modules
	MODULE,
	// end
	END,
};

// ----------------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------------

struct Types {
	char *name;
	Type  type;
};

struct LLVM {
	bool    is_set;
	bool    is_loaded;
	Value   array_size;
	Value   elem;
	Value   dim_sizes[4];
	int     dim_count;
	Block   bloc;
	TypeRef funcType;
	TypeRef stType;
	Value   va_count;
	Value   error_flag;
	Value   error_value;
	Block   Catch;
	Block   lpad;

	// statements/loops
	Block start;
	Block then;
	Block end;
};

struct Token {
	Type  type;
	Type  retType;

	char *name;
	int   space;

	int   used;
	int   pos;

	bool  is_ref;
	bool  is_dec;
	bool  is_global;
	bool  is_param;
	bool  is_variadic;
	bool  is_proto;
	bool  has_init;
	bool  has_clean;
	bool  is_method_call;

	bool  ir_bound;
	bool  asm_bound;

	char *filename;
	int   line;

	LLVM  llvm;

	struct {
		struct {
			long value;
		} Int;
		struct {
			int value;
		} Short;
		struct {
			long long value;
		} Long;
		struct {
			float value;
		} Float;
		struct {
			bool value;
		} Bool;
		struct {
			char *value;
		} Chars;
		struct {
			char value;
		} Char;
		struct {
			int   index;
			Node *ptr;
		} Struct;
		struct {
			Type  elem_type;
			int   depth;
			Node *struct_ptr;
		} Array;
		struct {
			Token *types[8];
			int    count;
		} Tuple;
		struct {
			Node *ptr;
		} Fcall;
		struct {
			Token *ptr;
			Token *start;
			Token *end;
		} Statement;
		struct {
			Type  type;
			char *name;
		} Catch;
	};
};

struct Node {
	Node   *left;
	Node   *right;
	Token  *token;

	Node  **children;
	int     children_count;
	int     children_size;

	Token **variables;
	int     variables_count;
	int     variables_size;

	Node  **functions;
	int     functions_count;
	int     functions_size;

	Node  **structs;
	int     structs_count;
	int     structs_size;

	Node **modules;
	int modules_count;
	int modules_size;
};

// globals.h
extern bool             found_error;

extern Token          **tokens;
extern int              tcount;
extern int              tsize;
extern int              ecount;

extern Node            *ura_scope;
extern Node            *scope;
extern Node           **level_scope;

extern int              sccount;
extern int              scsize;

extern char           **used_files;
extern int              ucount;
extern int              usize;

extern Context          context;
extern Module           module;
extern Builder          builder;
extern TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
extern char            *op_flags;
extern bool             enable_san;
extern char            *argv0;
extern char            *ura_lib;
extern int              calling_use;

extern char           **links;
extern int              lcount;
extern int              lsize;

extern LLVMDIBuilderRef debug_builder;
extern LLVMMetadataRef  debug_compile_unit;
extern LLVMMetadataRef  debug_file;
extern LLVMMetadataRef  debug_scope;

#define resize_array(array, type, size, count)                                                     \
	{                                                                                               \
		if (array == NULL) {                                                                         \
			size  = 10;                                                                               \
			array = allocate(size, sizeof(type));                                                     \
		} else if (count + 5 >= size) {                                                              \
			type *tmp = allocate(size *= 2, sizeof(type));                                            \
			memcpy(tmp, array, count * sizeof(type));                                                 \
			free(array);                                                                              \
			array = tmp;                                                                              \
		}                                                                                            \
	}

// ----------------------------------------------------------------------------
// Forward declarations — full.c functions
// ----------------------------------------------------------------------------
// memory
void  *allocate_func(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_local_memory();
void   free_global_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);
void   unuse(Node *node);

// tokenizer / parser helpers
void   tokenize(char *filename, int default_space);
Token *new_token(Type type, int space);
Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
void   add_token(Token *token);
Node  *new_node(Token *token);
Node  *add_child(Node *node, Node *child);
void   enter_scope(Node *node);
void   exit_scope();
Token *find(Type type, ...);
bool   within(int space);
bool   includes(Type to_find, ...);
void   setName(Token *token, char *name);
char  *to_string(Type type);
bool   is_data_type(Token *token);
char  *resolve_path(char *path);
Node  *syntax_error();

// variables / functions / structs
Token *new_variable(Token *token);
void   add_variable(Node *b, Token *token);
Token *get_variable(char *name);
Node  *new_function(Node *node);
void   add_function(Node *b, Node *node);
Node  *get_function(char *name);
Node  *new_struct(Node *node);
void   add_struct(Node *b, Node *node);
Node  *get_struct(char *name);

// parser nodes
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
Node *optimize_ir(Node *node, bool *changed);

// IR / codegen
void    gen_ir(Node *node);
void    gen_asm(Node *node);
void    init(char *name);
void    finalize(char *output);
void    load_if_necessary(Node *node);
void    _alloca(Token *token);
void    hoist_allocas(Node *node);
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
Value   _get_default_value(Token *token);
Value   _build_return(Token *token);
Value   load_value(Token *token);
Value   struct_field_ptr(Token *struct_tok, int field_index, char *name);
Value   allocate_stack(Value size, TypeRef elementType, char *name);
Value   allocate_heap(Value count, TypeRef elementType, char *name);
bool    compatible(Token *left, Token *right);
void    set_ret_type(Node *node);
void    _branch(Block bloc);
Block   _append_block(char *name);
Value   _add_function(char *name, TypeRef function_type);
TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed);
void    set_debug_location(Token *token);

// LLVM helpers
LLVMIntPredicate  icmp_predicate(Type op);
LLVMRealPredicate fcmp_predicate(Type op);
int               is_float_value(Value v);
Type              assign_base_op(Type assign_op);

// debug / utils
bool  _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
int   _debug(char *conv, ...);
void  pnode(Node *node, char *indent);
char *strjoin(char *str0, char *str1, char *str2);
int   vprint_(File out, char *conv, va_list args);

#endif /* HEADER_H */
