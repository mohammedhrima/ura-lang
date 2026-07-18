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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#if defined(__APPLE__)
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

#define SPLIT      "=================================================\n"
#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
#define BLUE(fmt)  BOLD "\033[34m" fmt RESET

#define LINE __LINE__
#define FUNC (char *)__func__
#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define URA_MAX_SIZE       999999
#define LIST_NAME_MAX      256
#define LIST_SOURCE_MAX    4096
#define LIST_STRUCT_PREFIX "__list_"
#define OP_PREFIX          ".operator."

#define TAB 3

#define TOKENIZE 1
#define AST      (TOKENIZE & 1)
#define IR       (AST      & 1)
#define OPTIMIZE (IR       & 1)
#define ASM      (IR       & 1)

#define CHECK(cond, fmt, ...) _check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define TODO(cond, fmt, ...) if (_check(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define debug(fmt, ...) if (ura.enable_debug) _debug(fmt, ##__VA_ARGS__)
#define EXPAND(type, name) type name; int  name##_count; int  name##_size;

#define resize_array(array, type) \
	{ \
		if (array == NULL) { \
			array##_size = 10; \
			array = allocate(array##_size, sizeof(type)); \
		} else if (array##_count + 5 >= array##_size) { \
			type *tmp = allocate(array##_size *= 2, sizeof(type)); \
			memcpy(tmp, array, array##_count * sizeof(type)); \
			free(array); \
			array = tmp; \
		} \
	}

#define DATA_TYPES     INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT, ARRAY_TYPE, LIST_TYPE, FN_TYPE
#define LOGIC_TYPE     AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define BITWISE_TYPE   BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN

#define PASSES_O0 "default<O0>" // No optimization (Debug Mode)
#define PASSES_O1 "default<O1>" // Light optimizations
#define PASSES_O2 "default<O2>" // Balanced speed and size
#define PASSES_O3 "default<O3>" // Aggressive speed, inlining, vectorization, loop unrolling
#define PASSES_Os "default<Os>" // Optimize for small binary, like O2 + limiting the size of the executable
#define PASSES_Oz "default<Oz>" // Smallest binary possible, may sacrifice speed

typedef struct Token      Token;
typedef struct Keyword    Keyword;
typedef struct Node       Node;
typedef struct LLVM       LLVM;
// typedef struct AutoClean  AutoClean;
typedef enum Type         Type;
typedef struct Source     Source;
typedef struct UraGlobal UraGlobal;

typedef LLVMTypeRef       TypeRef;
typedef LLVMContextRef    Context;
typedef LLVMModuleRef     Module;
typedef LLVMBuilderRef    Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef      Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind      TypeKind;
typedef LLVMAttributeRef  AttributeRef;
typedef LLVMMetadataRef   MetadataRef;
typedef LLVMErrorRef      Error;
typedef LLVMPassBuilderOptionsRef PassBuilderOptions;

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
	VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR, VARIADIC, REF, // Data types
	ARRAY, ARRAY_TYPE, ARRAY_LIT, LIST, LIST_TYPE,
	FN_TYPE,
	STRUCT_DEF, STRUCT_CALL, // Structures
	ENUM_DEF, ENUM_CALL, // Enums
	TUPLE, TUPLE_UNPACK, // Tuples
	ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, // Assignment
	EQUAL, NOT_EQUAL, LESS_EQUAL, GREAT, GREAT_EQUAL, LESS, // Comparison
	ADD, SUB, MUL, DIV, MOD, // Arithmetic
	AND, OR, NOT, // Logical
	LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, RANGE, ACCESS, AS, // Punctuation and Syntax
	RETURN, IF, ELIF, ELSE, WHILE, CONTINUE, BREAK, MATCH, CASE, DEFAULT, // Control Flow
	FOR, TO, STEP, IN, LOOP,
	FDEC, FCALL, PROTO, ARGS, CHILDREN, // Functions
	STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, SYNTAX_ERROR, // Built-ins
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT, // Bitwise
	NULLABLE, OPTIONAL, // Literals 
	MODULE, // Modules
	OPERATOR, // Operator overloading keyword
	PUB, DOUBLE_DOTS, CLEAN, NEW, // Static dispatch / pub
	END, // end
};

struct Keyword {
	char *name;
	Type  type;
	bool  is_dec;
	bool  clear_name;
};

struct Source {
	char *filename;
	char *dirname;
	char *content;
};

struct LLVM {
	bool    is_set;
	bool    is_loaded;
	Value   array_size;
	Value   elem;
	Value  *dims;
	int     dims_count;
	int     dims_size;
	Block   bloc;
	TypeRef func_type;
	TypeRef struct_type;
	Value   va_count;
	Value   error_flag;
	Value   error_value;
	Block   _catch;
	Block   lpad;
	Block   start;
	Block   then;
	Block   end;
	Block       prev_block;
	MetadataRef prev_scope;
	MetadataRef prev_loc;
};

// struct AutoClean {
// 	Value value;
// 	Node *type;
// };


struct Token {
	Type    type;
	Type    ret_type;
	Source *source;

	char   *name;
	int     indent;

	int     used;
	int     start_index;
	int     end_index; 

	bool    is_ref;
	bool    is_nullable;
	bool    is_dec;
	bool    is_global;
	bool    is_param;
	bool    is_variadic;
	bool    is_proto;
	bool    has_clean;
	bool    is_method_call;
	bool    is_pub;
	bool    is_static_call;
	bool    is_heap;

	int     line;

	LLVM    llvm;

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
		struct { EXPAND(Token**, types); } Tuple;
		struct { Node *ptr; Token *var; } Fcall;
		struct { Token *ptr; Token *start; Token *end; } Statement;
		struct { Type  type; char *name; } Catch;
		struct { EXPAND(Token**, params); Token *ret; } Fn;
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
	// EXPAND(AutoClean *, auto_cleans);
};

struct UraGlobal {
	bool             found_error;
	char            *output;
	int              error_count;
	int              max_errors;
	bool             enable_debug;
	bool             enable_exec;
	bool             enable_san;
	bool             enable_tree;
	bool             no_color;
	// bool             enable_prep;
	char            *flags;
	char            *lib;
	double           time_start;
	// const char      *ura_target_os;

	// Token          **tokens;
	// int              tokens_count;
	// int              tokens_size;
	// int              exe_count;
	// // char            *synth_list_paths[];
	// // int              synth_list_count;
	// Node           **scopes;
	// int              scopes_count;
	// int              scopes_size;
	// Node            *scope;
	// Node            *ura_scope;
	// char            *current_gen_module;

	char            *dir;
	char            *base;
	char            *build_dir;
	char            *ll_path;

	// Context          context;
	// Module           module;
	// Builder          builder;
	// TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
	LLVMDIBuilderRef debug_builder;
	MetadataRef      debug_compile_unit;
	MetadataRef      debug_file;
	MetadataRef      debug_scope;
	EXPAND(Node **, scopes);
	EXPAND(Token **, tokens);
	EXPAND(Source **, sources);
	int              sources_pos;
	int              calling_use;
	Node            *scope;
	Node            *head;
	int              exe_pos;

	Context          context;
	Module           module;
	Builder          builder;
	TypeRef          vd, f32, i1, i8, i16, i32, i64;
};

extern UraGlobal ura;

int _debug(char *conv, ...);
char *format(const char *fmt, ...);
double clock_now();
char *signal_name(int sig);
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void *allocate(int len, int size);
void new_source(char *file_name);
void exit_source();
void parse_error(Token *token, const char *fmt, ...);
void parser_recover(int indent);
void tokenize_error(int line, int s, int e, const char *fmt, ...);
Token *parse_token(int line, int s, int e, Type type, int space);
bool includes(Type to_find, ...);
bool lex_spaces(char *content, int *i, int *line, int *indent, int default_indent);
bool lex_multi_comment(char *content, int *i, int *line, int indent, int default_indent);
bool lex_comment(char *content, int *i, int line, int indent, int default_indent);
bool lex_chars(char *content, int *i, int line, int indent, int default_indent);
bool lex_char(char *content, int *i, int line, int indent, int default_indent);
bool lex_number(char *content, int *i, int line, int indent, int default_indent);
bool lex_use(char *content, int *i, int s, int line, int indent, int default_indent);
bool lex_link(char *content, int *i, int s, int line, int indent, int default_indent);
bool lex_identifier(char *content, int *i, int line, int indent, int default_indent);
void tokenize(int default_indent);
bool lex_symbol(char *content, int *i, int line, int *indent, int default_indent);
void set_name(Token *token, char *name);
void enter_scope(Node *node);
void exit_scope();
Token *find(Type type, ...);
Token *find_variable(char *name, bool *captured);
void declare_variable(Token *token);
Node *find_function(char *name);
void declare_function(Node *fn);
Token *new_token(Type type, int indent);
Node *new_node(Token *token);
Node *syntax_error();
void pnode(Node *node, char *indent);
void print_escaped(char c);
void print_node_label(Node *node);
void print_subtree(Node *node, char *prefix, bool is_last, char *role);
void print_children(Node *node, char *prefix);
void print_ast(Node *head);
Node *fdec_node(Node *node);
Node *fcall_node(Node *node);
Node *output_node(Node *node);
Node *match_node(Node *node);
Node *access_node(Node *node);
Node *array_lit_node(Node *node);
Node *array_ctor_node(Node *node);
void type_check_array_ctor(Node *node);
void code_gen_array_ctor(Node *node);
void free_array(Value slice, Type sub, int depth);
void code_gen_typeof(Node *node);
void code_gen_sizeof(Node *node);
void code_gen_clean(Node *node);
Value make_slice(Type sub, int depth, Value data, Value len);
Value build_array(Type sub, Value *dims, int depth, bool heap);
Value array_calloc(TypeRef elem, Value count, Value esz);
Node *if_node(Node *node);
Node *while_node(Node *node);
Node *ref_node(Node *node);
Node *id_node(Node *node);
void parse_type(Token *target);


Token *next();
Token *peek(int index);
Node *prime_node();
Node *expr_node(int min_op);
int get_operation_precedence(Type type);

char *to_string(Type type);
char *type_name(Type type);
int _vprint(File out, const char *conv, va_list args);
int parse_escape_seq(char *input, int s, int e, char *buf, int *ptr);
bool within(int indent);
bool is_data_type(Token *token);

TypeRef to_llvm_type(Type type);
TypeRef array_type(Type sub, int depth);
TypeRef llvm_type_of(Token *token);
Value default_value(Token *token);
void setup_paths(char *path_name);
void init_module(char *name);
void finalize_module(char *ll_path);
void debug_enter_function(Token *token);
void debug_exit_function(Token *token);
void set_debug_location(Token *token);
Value get_or_declare(char *name, TypeRef fn_type);
void render_caret(File out, Token *token);
void decolor(char *s);
void parse_block(Node *node, int indent);
Node *enclosing_continue();
Node *enclosing_break();
void analyze_block(Node *node);
void analyze_match(Node *node);
void type_check_block(Node *node);
void type_check_match(Node *node);
void code_gen_if(Node *node);
void code_gen_loop(Node *node);
Node *for_node(Node *node);
void analyze_for(Node *node);
void type_check_for(Node *node);
void code_gen_for_array(Node *node);
void code_gen_for(Node *node);
void code_gen_while(Node *node);
void code_gen_match(Node *node);
void guard(Token *op, Value is_bad, char *what);
void guard_nonzero(Token *op, Value divisor);
void guard_nonnull(Token *op, Value ptr);
void guard_bound(Token *op, Value ptr);
void guard_index(Token *op, Value idx, Value slice);
void guard_slice(Token *op, Value start, Value end, Value len);

void analyze(Node *node);
void analyze_fdec(Node *node);
void analyze_id(Node *node);
void analyze_fcall(Node *node);
void analyze_binop(Node *node);
void type_check(Node *node);
void type_check_fdec(Node *node);
void type_check_fcall(Node *node);
void type_check_binop(Node *node);
void emit_signature(Node *fn);
Value address_of(Node *node);
void code_gen(Node *node);
void code_gen_fdec(Node *node);
void code_gen_literal(Node *node);
void code_gen_id(Node *node);
void code_gen_fcall(Node *node);
void code_gen_assign(Node *node);
void code_gen_binop(Node *node);
void code_gen_output(Node *node);
Value access_ptr(Node *node);
void code_gen_slice(Node *node);
void code_gen_access(Node *node);
void code_gen_array_lit(Node *node);
void type_check_array_lit(Node *node);
void type_check_access(Node *node);
void code_gen_compound(Node *node);

void free_token(Token *token);
void free_node(Node *node);
void free_memory();

#endif