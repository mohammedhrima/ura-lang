#ifndef HEADER_H
#define HEADER_H

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
#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define URA_MAX_SIZE 999999

#define TOKENIZE 1
#define TAB      3
#define AST      1
#define IR       1
#define OPTIMIZE 1
#define ASM      1

#define CHECK(cond, fmt, ...) _check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define TODO(cond, fmt, ...)                                                                       \
	if (_check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define debug(fmt, ...)                                                                            \
	if (enable_debug) _debug(fmt, ##__VA_ARGS__)
#define SEG() raise(SIGSEGV)
#define EXPECT_TOKEN(type, fmt, ...)                                                               \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			CHECK(1, fmt, ##__VA_ARGS__);                                                             \
			return syntax_error();                                                                    \
		}                                                                                            \
	}

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

#define EXPAND(type, name)                                                                         \
	type name;                                                                                      \
	int  name##_count;                                                                              \
	int  name##_size

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

#define DATA_TYPES     INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT, ARRAY_TYPE, LIST_TYPE
#define LOGIC_TYPE     AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN

// No optimization (Debug Mode)
#define PASSES_O0 "default<O0>"
// Light optimizations
#define PASSES_O1 "default<O1>"
// Balanced speed and size
#define PASSES_O2 "default<O2>"
// Aggressive speed, inlining, vectorization, loop unrolling
#define PASSES_O3 "default<O3>"
// Optimize for small binary, like O2 + limiting the size of the executable
#define PASSES_Os "default<Os>"
// Smallest binary possible, may sacrifice speed
#define PASSES_Oz "default<Oz>"

typedef struct Token      Token;
typedef struct Keyword    Keyword;
typedef struct Node       Node;
typedef struct LLVM       LLVM;
typedef enum Type         Type;

typedef LLVMTypeRef       TypeRef;
typedef LLVMContextRef    Context;
typedef LLVMModuleRef     Module;
typedef LLVMBuilderRef    Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef      Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind      TypeKind;
typedef LLVMAttributeRef AttributeRef;
typedef LLVMMetadataRef MetadataRef;

#define PointerType  LLVMPointerTypeKind
#define IntegerType  LLVMIntegerTypeKind
#define FloatType    LLVMFloatTypeKind
#define DoubleType   LLVMDoubleTypeKind
#define VoidType     LLVMVoidTypeKind
#define FunctionType LLVMFunctionTypeKind
#define StructType   LLVMStructTypeKind

enum Type
{
	ID = 1,
	// Data types
	VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR, VARIADIC, REF,
	ARRAY, ARRAY_TYPE, ARRAY_LIT, LIST, LIST_TYPE,
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
	// Operator overloading keyword
	OPERATOR,
	// Static dispatch / pub
	PUB, DOUBLE_DOTS, DELETE,
	// end
	END,
};

struct Keyword {
	char *name;
	Type  type;
	bool  is_dec;
	bool  clear_name;
};

struct LLVM {
	bool is_set;
	bool is_loaded;
	//	Value   array_size;
	Value elem;
	//	Value  *dims;
	//	int     dims_count;
	//	int     dims_size;
	//	Block   bloc;
	TypeRef func_type;
	TypeRef struct_type;
	//	Value   va_count;
	//	Value   error_flag;
	//	Value   error_value;
	//	Block   _catch;
	//	Block   lpad;
	// statements/loops
	Block start;
	Block then;
	Block end;
};
struct Token {
	Type  type;
	Type  ret_type;

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
	bool  has_clean;
	bool  is_method_call;
	bool  is_pub;
	bool  is_static_call;

	char *filename;
	int   line;

	LLVM  llvm;

	// clang-format off
	struct {
		struct { long value; } Int;
		struct { int value; } Short;
		struct { long long value; } Long;
		struct { float value; } Float;
		struct { bool value; } Bool;
		struct { char *value; } Chars;
		struct { char value; } Char;
		struct { int index; Node *ptr; } Struct;
		struct { Type  sub_type; int   depth; Node *struct_ptr; } Array;
		struct { EXPAND(Token*, types); } Tuple;
		struct { Node *ptr; } Fcall;
		struct { Token *ptr; Token *start; Token *end; } Statement;
		struct { Type  type; char *name; } Catch;
	};
	// clang-format on
};

struct Node {
	Node  *left;
	Node  *right;
	Token *token;

	EXPAND(Node **, children);
	EXPAND(Token **, variables);
	EXPAND(Node **, functions);
	EXPAND(Node **, structs);
	EXPAND(Node **, modules);
};

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
// memory
void  *_allocate(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_local_memory();
void   free_global_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);
void   unuse(Node *node);

// tokenizer / parser helpers
void   tokenize(char *filename, int default_space);
void   tokenize_string(char *input, char *fake_name, int default_space);
void   instantiate_list_types(void);
char  *generate_list_source(const char *elem_type_name, const char *struct_name);
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
Node *expr_node(), *assign_node(), *and_node(), *or_node();
Node *bitor_node(), *bitxor_node(), *bitnot_node(), *bitand_node();
Node *equality_node(), *comparison_node();
Node *shift_node();
Node *add_sub_node(), *mul_div_node();
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
void    ensure_loaded(Node *node);
void    _alloca(Token *token);
void    hoist_allocas(Node *node);
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
Value   _get_default_value(Token *token);
Value   read_value(Token *token);
void    write_value(Token *token, Value val);
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

#endif