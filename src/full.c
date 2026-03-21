// system.h
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
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/PassBuilder.h>
#include <llvm-c/DebugInfo.h>

// macros.h
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
	_check(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)
#define todo(cond, fmt, ...)                                          \
	if (_check(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)) exit(1);
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
#define debug(fmt, ...) _debug(fmt, ## __VA_ARGS__)
#else
#define debug(fmt, ...)      \
	do                   \
	{                    \
	} while (0)
#endif

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, \
	LONG, PTR, SHORT, ARRAY_TYPE
#define LOGIC_TYPE AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN
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

// types.h
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

#define PointerType LLVMPointerTypeKind
#define IntegerType LLVMIntegerTypeKind
#define VoidType    LLVMVoidTypeKind
#define FunctionType LLVMFunctionTypeKind
#define StructType  LLVMStructTypeKind

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
	// Enums
	ENUM_DEF, ENUM_CALL,
	// Tuples
	TUPLE, TUPLE_UNPACK,
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
	RETURN, IF, ELIF, ELSE,
	WHILE, CONTINUE, BREAK,
	// Functions
	FDEC, FCALL, PROTO, ARGS, CHILDREN,
	// Built-ins
	STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, SYNTAX_ERROR,
	// Bitwise
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT,
	// Literals
	NULLABLE,
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
	Value dim_sizes[4];
	int dim_count;
	Block bloc;
	TypeRef funcType;
	TypeRef stType;
	Value va_count;
	Value error_flag;
	Value error_value;
	Block Catch;
	Block lpad;

	// statements/loops
	Block start;
	Block then;
	Block end;
};

struct Token
{
	Type type;
	Type retType;

	char *name;
	int space;

	int used;
	int pos;

	bool is_ref;
	bool is_dec;
	bool is_global;
	bool is_param;
	bool is_variadic;
	bool is_proto;
	bool is_init;
	bool is_clean;
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
		struct { Type elem_type; int depth; Node *struct_ptr; } Array;
		struct { Token *types[8]; int count; } Tuple;
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
	int children_count;
	int children_size;

	Token **variables;
	int variables_count;
	int variables_size;

	Node **functions;
	int functions_count;
	int functions_size;

	Node **structs;
	int structs_count;
	int structs_size;

	Node **methods;
	int methods_count;
	int methods_size;
};

// globals.h
bool found_error;

Token          **tokens;
int tcount;
int tsize;
int ecount;

Node            *ura_scope;
Node            *scope;
Node           **level_scope;

int sccount;
int scsize;

char           **used_files;
int ucount;
int usize;

Context context;
Module module;
Builder builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
bool enable_bounds_check = false;
char            *op_flags;
bool enable_asan;
char            *argv0;

char           **links;
int lcount;
int lsize;

LLVMDIBuilderRef debug_builder;
LLVMMetadataRef debug_compile_unit;
LLVMMetadataRef debug_file;
LLVMMetadataRef debug_scope;

// functions prototypes
// ----------------------------------------------------------------------------
// File Management
// ----------------------------------------------------------------------------
char *open_file(char *filename);
char *resolve_path(char *path);
bool  add_file(char *filename);
Node *syntax_error();
void  add_link_key(char *key);

// ----------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------
void  *allocate_func(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_local_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);

// ----------------------------------------------------------------------------
// Error Handling
// ----------------------------------------------------------------------------
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
void   tokenize(char *filename);
Token *new_token(Type type, int space);
Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
void   add_token(Token *token);
Node  *new_node(Token *token);
Node  *add_child(Node *node, Node *child);
void   enter_scope(Node *node);
void   exit_scope();
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
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
Value   _get_default_value(Token *token);
Value   _build_return(Token *token);
bool    compatible(Token *left, Token *right);

// ----------------------------------------------------------------------------
// Debugging
// ----------------------------------------------------------------------------
int  _debug(char *conv, ...);
void pnode(Node *node, char *indent);

// ----------------------------------------------------------------------------
// String Utilities
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Other utilities
// ----------------------------------------------------------------------------
char *strjoin(char *str0, char *str1, char *str2);
Value   allocate_stack(Value size, TypeRef elementType, char *name);
Value   allocate_heap(Value count, TypeRef elementType, char *name);
void   set_ret_type(Node *node);

#define resize_array(array, type, size, count){ \
		if(array == NULL) { \
			size  = 10;                                     \
			array = allocate(size, sizeof(type));          \
		}                                                 \
		else if(count + 5 >= size)                        \
		{                                                 \
			type* tmp = allocate(size *= 2, sizeof(type)); \
			memcpy(tmp, array, count * sizeof(type));      \
			free(array);                                   \
			array = tmp;                                   \
		}                                                 \
}

// llvm.h
// ----------------------------------------------------------------------------
// LLVM Wrappers (static inline)
// ----------------------------------------------------------------------------
static inline Value _build_store(Value val, Value ptr) {
	return LLVMBuildStore(builder, val, ptr);
}

static inline Value _build_alloca(TypeRef ty, char *name) {
	return LLVMBuildAlloca(builder, ty, name);
}

static inline Value _build_add(Value lhs, Value rhs, char *name) {
	return LLVMBuildAdd(builder, lhs, rhs, name);
}

static inline Value _build_sub(Value lhs, Value rhs, char *name) {
	return LLVMBuildSub(builder, lhs, rhs, name);
}

static inline Value _build_mul(Value lhs, Value rhs, char *name) {
	return LLVMBuildMul(builder, lhs, rhs, name);
}

static inline Value _build_sdiv(Value lhs, Value rhs, char *name) {
	return LLVMBuildSDiv(builder, lhs, rhs, name);
}

static inline Value _build_srem(Value lhs, Value rhs, char *name) {
	return LLVMBuildSRem(builder, lhs, rhs, name);
}

static inline Value _build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name) {
	return LLVMBuildICmp(builder, op, lhs, rhs, name);
}

static inline Value _build_and(Value lhs, Value rhs, char *name) {
	return LLVMBuildAnd(builder, lhs, rhs, name);
}

static inline Value _build_or(Value lhs, Value rhs, char *name) {
	return LLVMBuildOr(builder, lhs, rhs, name);
}

static inline LLVMIntPredicate icmp_predicate(Type op)
{
	switch (op)
	{
	case LESS:        return LLVMIntSLT;
	case GREAT:       return LLVMIntSGT;
	case EQUAL:       return LLVMIntEQ;
	case LESS_EQUAL:  return LLVMIntSLE;
	case GREAT_EQUAL: return LLVMIntSGE;
	default:          return LLVMIntNE;// NOT_EQUAL
	}
}

static inline LLVMRealPredicate fcmp_predicate(Type op)
{
	switch (op)
	{
	case LESS:        return LLVMRealOLT;
	case GREAT:       return LLVMRealOGT;
	case EQUAL:       return LLVMRealOEQ;
	case LESS_EQUAL:  return LLVMRealOLE;
	case GREAT_EQUAL: return LLVMRealOGE;
	default:          return LLVMRealONE;// NOT_EQUAL
	}
}

static inline int is_float_value(Value v)
{
	LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
	return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

static inline Type assign_base_op(Type assign_op)
{
	switch (assign_op)
	{
	case ADD_ASSIGN: return ADD;
	case SUB_ASSIGN: return SUB;
	case MUL_ASSIGN: return MUL;
	case DIV_ASSIGN: return DIV;
	default:         return MOD;// MOD_ASSIGN
	}
}

static inline Value build_binary_op(Type op, Value l, Value r)
{
	char *name = to_string(op);
	if (is_float_value(l))
	{
		if (includes(op, COMPARISON_OPS, 0))
			return LLVMBuildFCmp(builder, fcmp_predicate(op), l, r, name);
		switch (op)
		{
		case ADD: return LLVMBuildFAdd(builder, l, r, name);
		case SUB: return LLVMBuildFSub(builder, l, r, name);
		case MUL: return LLVMBuildFMul(builder, l, r, name);
		case DIV: return LLVMBuildFDiv(builder, l, r, name);
		default:  todo(1, "build_binary_op: unhandled float op %s", name); return NULL;
		}
	}
	if (includes(op, COMPARISON_OPS, 0))
		return _build_icmp(icmp_predicate(op), l, r, name);
	switch (op)
	{
	case ADD:    return _build_add(l, r, name);
	case SUB:    return _build_sub(l, r, name);
	case MUL:    return _build_mul(l, r, name);
	case DIV:    return _build_sdiv(l, r, name);
	case MOD:    return _build_srem(l, r, name);
	case AND:    return _build_and(l, r, name);
	case OR:     return _build_or(l, r, name);
	case BAND:   return _build_and(l, r, name);
	case BOR:    return _build_or(l, r, name);
	case BXOR:   return LLVMBuildXor(builder, l, r, name);
	case LSHIFT: return LLVMBuildShl(builder, l, r, name);
	case RSHIFT: return LLVMBuildAShr(builder, l, r, name);
	default:     todo(1, "build_binary_op: unhandled op %s", name); return NULL;
	}
}

static inline Block _append_basic_block_in_context(Value func, char *name)
{
	return LLVMAppendBasicBlockInContext(context, func, name);
}

static inline Block _get_insert_block()
{
	return LLVMGetInsertBlock(builder);
}

static inline Value _get_basic_block_parent(Block block)
{
	return LLVMGetBasicBlockParent(block);
}

static inline Value _get_basic_block_terminator(Block block)
{
	return LLVMGetBasicBlockTerminator(block);
}

static inline void _branch(Block bloc)
{
	if (!_get_basic_block_terminator(_get_insert_block()))
		LLVMBuildBr(builder, bloc);
}

static inline Block _append_block(char *name)
{
	Value parent = _get_basic_block_parent(_get_insert_block());
	return _append_basic_block_in_context(parent, name);
}

static inline void _condition(Value cond, Block then_block, Block else_block)
{
	LLVMBuildCondBr(builder, cond, then_block, else_block);
}

static inline Value _build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name)
{
	return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

static inline Value _const_chars(char *str, char *name)
{
	return LLVMBuildGlobalStringPtr(builder, str, name);
}

static inline Value _build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned indece, char *name)
{
	return LLVMBuildGEP2(builder, ty, ptr, indices, indece, name);
}

static inline Value _build_trunc(Value val, TypeRef dest_ty, char *name)
{
	return LLVMBuildTrunc(builder, val, dest_ty, name);
}

static inline Value _build_not(Token *token, char *name)
{
	return LLVMBuildNot(builder, token->llvm.elem, name);
}

static inline Value _build_memcpy(Value dest, Value src, Value size)
{
	return LLVMBuildMemCpy(builder, dest, 0, src, 0, size);
}

static inline Value _const_int(TypeRef ref_type, unsigned long long n, int sign_extend)
{
	return LLVMConstInt(ref_type, n, sign_extend);
}

static inline TypeRef _pointer_type(TypeRef element_ty, unsigned address_space)
{
	return LLVMPointerType(element_ty, address_space);
}

static inline TypeRef _function_type(TypeRef retType, TypeRef *types, int count, int is_variadic)
{
	return LLVMFunctionType(retType, types, count, is_variadic);
}

static inline TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed)
{
	TypeRef type = LLVMStructCreateNamed(context, name);
	LLVMStructSetBody(type, element_types, element_count, packed);
	return type;
}

static inline Value _get_named_function(char *name)
{
	return LLVMGetNamedFunction(module, name);
}

static inline Value _add_function(char *name, TypeRef function_type)
{
	Value f = LLVMGetNamedFunction(module, name);
	if (f) return f;
	return LLVMAddFunction(module, name, function_type);
}

static inline void set__debuglocation(Token *token)
{
	if (!token || !debug_builder || !debug_scope) return;
	if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
	LLVMMetadataRef loc = LLVMDIBuilderCreateDebugLocation(
		context, token->line, 0,
		debug_scope, NULL
		);
	LLVMSetCurrentDebugLocation2(builder, loc);
}

static inline Block _get_entry_basic_block(Value func)
{
	return LLVMGetEntryBasicBlock(func);
}

static inline void _position_at(Block block)
{
	LLVMPositionBuilderAtEnd(builder, block);
}

static inline TypeRef _global_get_value_type(Value ura_scope)
{
	return LLVMGlobalGetValueType(ura_scope);
}


static inline size_t _abi_size_of_type(TargetData td, TypeRef ty)
{
	return LLVMABISizeOfType(td, ty);
}

static inline TargetData _get_module_data_layout(Module m)
{
	return LLVMGetModuleDataLayout(m);
}

// ----------------------------------------------------------------------------
// Higher-level helpers
// ----------------------------------------------------------------------------

static inline Value struct_field_ptr(Token *struct_tok, int field_index, char *name)
{
	TypeRef struct_type = get_llvm_type(struct_tok);
	Value indices[]   =
	{
		LLVMConstInt(i32, 0, 0),
		LLVMConstInt(i32, field_index, 0),
	};
	return _build_gep2(struct_type, struct_tok->llvm.elem, indices, 2, name);
}

static inline Value load_value(Token *token)
{
	if (token->llvm.is_loaded) return token->llvm.elem;
	// Already-computed values: function returns, literals, allocated buffers
	if (includes(token->type, MATH_TYPE, FCALL, STACK, HEAP, 0))
		return token->llvm.elem;
	if (!token->name && !includes(token->type, DOT, ACCESS, 0))
		return token->llvm.elem;

	// Scalar ref: double-deref (alloca-of-ptr → ptr → value)
	if (token->is_ref && token->type != STRUCT_CALL)
	{
		TypeRef type = get_llvm_type(token);
		Value ptr  = LLVMBuildLoad2(builder, _pointer_type(type, 0), token->llvm.elem, "ref_ptr");
		return LLVMBuildLoad2(builder, type, ptr, "ref_val");
	}

	char *name = token->name;
	if (token->type == DOT)    name = to_string(DOT);
	if (token->type == ACCESS) name = to_string(ACCESS);
	return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, name ? name : "");
}

static inline void _entry(Token *token)
{
	Block entry = _append_basic_block_in_context(token->llvm.elem, "entry");
	_position_at(entry);
}

Token *new_token(Type type, int space)
{
	Token* token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

// ----------------------------------------------------------------------------
// Escape sequence decoder
// input[s] == '\\', input[s+1] is the escape character.
// Writes the decoded byte(s) to buf[*j] and returns the new value of s
// (pointing at the last consumed character; the caller's loop does s++ after).
// ----------------------------------------------------------------------------
static int parse_escape_seq(char *input, int s, int e, char *buf, int *j)
{
	switch (input[s + 1])
	{
	case 'n': buf[(*j)++]  = '\n'; return s + 1;// newline
	case 't': buf[(*j)++]  = '\t'; return s + 1;// tab
	case 'r': buf[(*j)++]  = '\r'; return s + 1;// carriage return
	case 'b': buf[(*j)++]  = '\b'; return s + 1;// backspace
	case 'f': buf[(*j)++]  = '\f'; return s + 1;// form feed
	case 'v': buf[(*j)++]  = '\v'; return s + 1;// vertical tab
	case 'a': buf[(*j)++]  = '\a'; return s + 1;// alert (bell)
	case '\\': buf[(*j)++] = '\\'; return s + 1; // backslash
	case '"':  buf[(*j)++] = '"';  return s + 1;// double quote
	case '\'': buf[(*j)++] = '\''; return s + 1; // single quote
	case '?':  buf[(*j)++] = '\?'; return s + 1;// question mark (trigraph)
	case '0':
	{
		// three-digit octal: \0NN
		if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
		{
			int octal = (input[s + 1] - '0') * 64 +
			            (input[s + 2] - '0') * 8 +
			            (input[s + 3] - '0');
			if (octal <= 255) { buf[(*j)++] = (char)octal; return s + 3; }
			buf[(*j)++] = '\0'; return s + 1;
		}
		// two-digit octal: \0N
		else if (s + 1 < e && isdigit(input[s + 2]))
		{
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// plain null
		buf[(*j)++] = '\0';
		return s + 1;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7':
	{
		// three-digit octal: \NNN
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
		{
			int octal = (input[s + 1] - '0') * 64 +
			            (input[s + 2] - '0') * 8 +
			            (input[s + 3] - '0');
			if (octal <= 255) { buf[(*j)++] = (char)octal; return s + 3; }
			buf[(*j)++] = input[s]; return s + 1; // invalid, keep backslash
		}
		// two-digit octal
		else if (s + 2 < e && isdigit(input[s + 2]))
		{
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// single-digit octal
		buf[(*j)++] = (char)(input[s + 1] - '0');
		return s + 1;
	}
	case 'x': // Hexadecimal: \xFF
	{
		if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
		{
			int hex = 0;
			char c1  = input[s + 2];
			char c2  = input[s + 3];
			if      (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
			else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
			else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;
			if      (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
			else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
			else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);
			buf[(*j)++] = (char)hex;
			return s + 3;
		}
		buf[(*j)++] = input[s]; // invalid hex escape, keep backslash
		return s + 1;
	}
	case 'u': // \uXXXX — not fully implemented yet
		buf[(*j)++] = input[s];
		return s + 1;
	case 'U': // \UXXXXXXXX — not fully implemented yet
		buf[(*j)++] = input[s];
		return s + 1;
	default:
		buf[(*j)++] = input[s]; // unknown escape, keep backslash
		return s + 1;
	}
}

Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space)
{
	Token *new = new_token(type, space);
	new->line     = line;
	new->filename = filename;
	new->pos      = tcount;

	switch (type)
	{
	case INT:
	{
		while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
		break;
	}
	case FLOAT:
	{
		char buf[64] = {0};
		int len     = e - s;
		if (len > 63) len = 63;
		strncpy(buf, input + s, len);
		new->Float.value = (float)atof(buf);
		break;
	}
	case ID:
	{
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);
		int i = 0;
		struct { char *name; bool value; } bools[] =
		{
			{"True", true}, {"False", false}, {0, 0},
		};
		for (i = 0; bools[i].name; i++)
		{
			if (strcmp(new->name, bools[i].name) == 0)
			{
				setName(new, NULL);
				new->type       = BOOL;
				new->Bool.value = bools[i].value;
				break;
			}
		}
		if (new->name == NULL) break;
		// TODO: PTR's approach need to be fixed later
		struct { char *name; Type type; } dataTypes [] =
		{
			{"int", INT}, {"bool", BOOL}, {"chars", CHARS},
			{"char", CHAR}, {"float", FLOAT}, {"void", VOID},
			{"long", LONG}, {"pointer", CHARS}, {"short", SHORT},
			{0, 0},
		};
		for (i = 0; dataTypes[i].name; i++)
		{
			if (strcmp(dataTypes[i].name, new->name) == 0)
			{
				setName(new, NULL);
				new->type   = dataTypes[i].type;
				new->is_dec = true;
				break;
			}
		}
		if (dataTypes[i].name) break;

		struct { char *name; Type type; } keywords[] =
		{
			{"if", IF},  {"elif", ELIF}, {"else", ELSE},
			{"while", WHILE},  {"fn", FDEC}, {"return", RETURN},
			{"break", BREAK}, {"continue", CONTINUE}, {"ref", REF},
			{"struct", STRUCT_DEF}, {"enum", ENUM_DEF}, {"proto", PROTO}, {"as", AS},
			{0, 0},
		};
		for (i = 0; keywords[i].name; i++)
		{
			if (strcmp(keywords[i].name, new->name) == 0)
			{
				new->type = keywords[i].type;
				break;
			}
		}
		if (keywords[i].name) break;

		struct { char *name; Type type; } keywords2[] =
		{
			{"and", AND}, {"or", OR}, {"is", EQUAL},
			{"not", NOT}, {"typeof", TYPEOF}, {"sizeof", SIZEOF}, {"output", OUTPUT},
			{"stack", STACK}, {"heap", HEAP}, {"array", ARRAY_TYPE},
			{"null", NULLABLE},
			{0, 0},
		};
		for (i = 0; keywords2[i].name; i++)
		{
			if (strcmp(keywords2[i].name, new->name) == 0)
			{
				setName(new, NULL);
				new->type = keywords2[i].type;
				break;
			}
		}
		break;
	}
	case FDEC:
	{
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);
		break;
	}
	case CHARS:
	{
		if (e <= s)
		{
			new->Chars.value = allocate(1, sizeof(char));
			break;
		}
		int len   = e - s;
		new->Chars.value = allocate(len + 1, sizeof(char));
		char *value = new->Chars.value;
		int j     = 0;

		for (int i = 0; i < len && s < e; i++, s++)
		{
			if (s + 1 < e && input[s] == '\\')
				s = parse_escape_seq(input, s, e, value, &j);
			else
				value[j++] = input[s];
		}
		if (j < len)
		{
			char *resized = allocate(j + 1, sizeof(char));
			strcpy(resized, value);
			new->Chars.value = resized;
		}
		break;
	}
	case CHAR:
	{
		char value = 0;
		if (e == s + 1) value = input[s];
		else // special character
		{
			char escape_map[256] =
			{
				['n'] = '\n', ['t'] = '\t', ['r'] = '\r',
				['b'] = '\b', ['f'] = '\f', ['v'] = '\v',
				['a'] = '\a', ['0'] = '\0', ['\\'] = '\\',
				['"'] = '\"', ['\''] = '\'', ['?'] = '\?',
			};
			char c = escape_map[(unsigned char)input[s + 1]];
			check(!c && input[s + 1] != '0', "unknown escape character");
			value = c;
		}
		new->Char.value = value;
		break;
	}
	default:
		break;
	}
	return new;
}

// ----------------------------------------------------------------------------
// `use` import handler — resolves and tokenizes an imported module
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// `link` directive handler — collects native library link keys
// ----------------------------------------------------------------------------
static void handle_link_directive(char *input, int *i)
{
	while (isspace(input[*i])) (*i)++;
	if (input[*i] != '\"') return;
	(*i)++;
	int s = *i;
	while (input[*i] && input[*i] != '\"') (*i)++;
	if (input[*i] != '\"') return;

	char *raw = strndup(input + s, *i - s);
	(*i)++;

	// strip "@/" prefix to get the key name
	char *key = (raw[0] == '@' && raw[1] == '/') ? raw + 2 : raw;
	add_link_key(key);
	free(raw);
}

static bool calling_use = false; // file-level so handle_use_import can toggle it

static void handle_use_import(char *filename, char *input, int *i, int line)
{
	(void)line;
	while (isspace(input[*i])) (*i)++;
	check(input[*i] != '\"', "expected \" after use"); (*i)++;
	int s = *i;
	while (input[*i] && input[*i] != '\"') (*i)++;
	check(input[*i] != '\"', "expected \"");

	char *use = strndup(input + s, *i - s);
	(*i)++;

	char *use_filename = NULL;
	if (use[0] == '@')
	{
		char *lib = getenv("URA_LIB");
		if (!lib && argv0)
		{
			char resolved[PATH_MAX];
			if (realpath(argv0, resolved))
			{
				char resolved_copy[PATH_MAX];
				strncpy(resolved_copy, resolved, PATH_MAX - 1);
				resolved_copy[PATH_MAX - 1] = '\0';
				char       *bin_dir = dirname(resolved_copy);
				static char fallback[PATH_MAX];
				snprintf(fallback, sizeof(fallback), "%s/ura-lib", bin_dir);
				struct stat st;
				if (stat(fallback, &st) == 0 && S_ISDIR(st.st_mode))
					lib = fallback;
			}
		}
		check(!lib, "URA_LIB not set and no ura-lib/ found next to binary");
		char *resolved = strjoin(lib, use + 1, NULL);
		free(use);
		use          = resolved;
		use_filename = strdup(use);
	}
	else
		use_filename = strjoin(dirname(filename), "/", use);

	bool old;
	free(use);
	use         = strjoin(use_filename, ".ura", NULL);
	old         = calling_use;
	calling_use = true;
	tokenize(use);
	calling_use = old;
	free(use_filename);
}

// ----------------------------------------------------------------------------
// Tokenizer
// ----------------------------------------------------------------------------
void tokenize(char *filename)
{
	char *input = open_file(filename);
	if (found_error) return;

	debug(GREEN BOLD "TOKENIZE: [%s]\n" RESET, filename);

	struct { char *value; Type type; } specials[] =
	{
		{"...", VARIADIC}, {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN},
		{"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN},
		{"!=", NOT_EQUAL}, {"!", NOT},{"==", EQUAL},
		{"<<", LSHIFT}, {">>", RSHIFT}, {"<=", LESS_EQUAL},
		{">=", GREAT_EQUAL}, {"<", LESS}, {">", GREAT}, {"=", ASSIGN},
		{"+", ADD}, {"-", SUB},{"*", MUL}, {"/", DIV}, {"%", MOD},
		{"(", LPAR}, {")", RPAR}, {"[", LBRA}, {"]", RBRA}, {",", COMA},
		{"&&", AND}, {"||", OR}, {"&", BAND}, {"|", BOR}, {"^", BXOR},
		{"~", BNOT},
		{0, (Type)0}
	};

	int space    = 0;
	int line     = 1;
	bool new_line = true;

	if (!add_file(filename)) return;
	for (int i = 0; input[i] && !found_error; )
	{
		int s = i;
		char c = input[i];

		if (isspace(c))
		{
			if (c == '\n')
			{
				line++;
				new_line = true;
				space    = 0;
			}
			else if (new_line) space += (c == '\t') ? TAB : 1;
			i++;
			continue;
		}
		if (strncmp(input + i, "/*", 2) == 0)
		{
			i += 2;
			while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2))
			{
				if (input[i] == '\n') line++;
				i++;
			}
			check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'");
			i += 2;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0)
		{
			while (input[i] && input[i] != '\n') i++;
			continue;
		}
		new_line = false;
		if (c == '\"')
		{
			i++;
			while (input[i] && input[i] != '\"')
			{
				if (input[i] == '\\') i++;
				i++;
			}
			check(input[i] != '\"', "expected '\"'");
			parse_token(filename, line, input, s + 1, i, CHARS, space);
			i++;
			continue;
		}
		if (c == '\'')
		{
			i++;
			if (input[i] == '\\') i++;
			if (input[i] && input[i] != '\'') i++;
			check(input[i] != '\'', "expected '\''");
			parse_token(filename, line, input, s + 1, i, CHAR, space);
			i++;
			continue;
		}
		if (isalpha(c) || strchr("@$_", c))
		{
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i]))) i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i]))
				handle_use_import(filename, input, &i, line);
			else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i]))
				handle_link_directive(input, &i);
			else
				parse_token(filename, line, input, s, i, ID, space);
			continue;
		}
		if (isdigit(c))
		{
			while (isdigit(input[i])) i++;
			if (input[i] == '.' && isdigit(input[i + 1]))
			{
				i++; // consume '.'
				while (isdigit(input[i])) i++;
				parse_token(filename, line, input, s, i, FLOAT, space);
			}
			else
			{
				parse_token(filename, line, input, s, i, INT, space);
			}
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++)
		{
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0)
			{
				parse_token(filename, line, NULL, 0, 0, specials[j].type, space);
				i    += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		check(1, "Syntax error <%c>", c);
	}
	if (!calling_use) new_token(END, -1);
	free(input);
}

Node *expr_node() {
	return assign_node();
}

AST_NODE(assign_node, logic_and_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN)
AST_NODE(logic_and_node, logic_or_node, AND)
AST_NODE(logic_or_node, bitor_node, OR)
AST_NODE(bitor_node, bitxor_node, BOR)
AST_NODE(bitxor_node, bitnot_node, BXOR)

Node *bitnot_node() // ~
{
	Token *token;
	if ((token = find(BNOT, 0)))
	{
		Node *node = new_node(token);
		node->left = bitnot_node();
		return node;
	}
	return bitand_node();
}

AST_NODE(bitand_node, equality_node, BAND)
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL)
AST_NODE(comparison_node, shift_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL)
AST_NODE(shift_node, add_sub_node, LSHIFT, RSHIFT)
AST_NODE(add_sub_node, mul_div_node, ADD, SUB)
AST_NODE(mul_div_node, as_node, MUL, DIV, MOD)

Node *as_node() // as
{
	Node  *left = unary_node();
	Token *token;
	while ((token = find(AS, 0)))
	{
		Node *node = new_node(token);
		node->left  = left;
		node->right = unary_node();
		Token *to = node->right->token;
		if (check(to == NULL || !to->is_dec, "expected data type after to"))
			return syntax_error();
		to->is_dec = false;
		left       = node;
	}
	return left;
}

Node *unary_node() // - + (unary)
{
	Token *token;
	if ((token = find(ADD, SUB, 0)))
	{
		Node *node = new_node(token);
		node->left  = unary_node();
		node->right = new_node(new_token(INT, token->space));
		node->right->token->filename  = token->filename;
		node->right->token->line      = token->line;
		node->right->token->Int.value = token->type == SUB ? -1 : 1;
		node->token->type             = MUL;
		return node;
	}
	return access_node();
}

Node *access_node() // . []
{
	Node  *left = prime_node();
	Token *token;
	while ((token = find(DOT, LBRA, 0)))
	{
		Node *node = new_node(token);
		if (token->type == DOT)
		{
			node->left  = left;
			node->right = prime_node();

			check(!node->right, "expected (attribute/method call) after .");
			if (node->right->token->type == FCALL)
			{
				Node *func = node->right;
				func->token->is_method_call = true;
				add_child(func->left, left);
				node->right = NULL;
				node->left  = NULL;
				node        = func;
			}
		}
		else // LBRA
		{
			node->token->type = ACCESS;
			node->left        = left;
			node->right       = expr_node();
			check(!node->right, "expected something between []");
			expect_token(RBRA, "expected right bracket");
		}
		left = node;
	}
	return left;
}

// ----------------------------------------------------------------------------
// prime_node sub-parsers (token already consumed by caller)
// ----------------------------------------------------------------------------

// [INT/CHARS/…] varname               variable declaration
// [INT] [42]                           literal value
// [STRUCT_CALL] varname → StructType  struct variable declaration
// [TUPLE_UNPACK]                       a int, b chars = fn()
// ├──[INT] a
// ├──[CHARS] b
// └──left: [FCALL] fn ret [TUPLE]
// [FCALL] name ret [TYPE]             function call
// └──[ARGS]
//    ├──arg1
//    └──arg2
static Node *parse_var_dec(Token *token)
{
	// Literal value: "hello", 1, 'c'
	if (token->type != ID && !token->is_dec && !token->name)
		return new_node(token);
	// int, char, chars, etc. declaration keyword
	if (token->is_dec) return new_node(token);

	// variable declaration: name type [ref] [= ...]
	if (token->type == ID && is_data_type(tokens[ecount]))
	{
		Token *tmp    = find(DATA_TYPES, 0);// skip data type
		bool is_ref = find(REF, 0) != NULL;
		if (tmp->type == ARRAY_TYPE)
		{
			expect_token(LBRA, "expected [ after array");
			int depth     = 1;
			while (find(LBRA, 0)) depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			check(!elem_type, "expected element type in array type");
			for (int i = 0; i < depth; i++)
				expect_token(RBRA, "expected ] in array type");
			tmp->Array.elem_type = elem_type->type;
			tmp->Array.depth     = depth;
			tmp->retType         = ARRAY_TYPE;
			if (elem_type->type == ID)
			{
				Node *sd = get_struct(elem_type->name);
				if (sd) { tmp->Array.elem_type = STRUCT_CALL; tmp->Array.struct_ptr = sd; }
			}
		}
		setName(tmp, token->name);
		tmp->is_dec = true;
		tmp->is_ref = is_ref;
		if (is_ref)
		{
			expect_token(ASSIGN, "'%s': ref must be initialized at "
			             "declaration, e.g: x int ref = a", token->name);
			ecount--;
		}
		// Tuple unpack: a int, b chars = foo()
		if (!is_ref
		    && tokens[ecount]->type == COMA
		    && ecount + 2 < tcount
		    && tokens[ecount + 1]->type == ID
		    && is_data_type(tokens[ecount + 2]))
		{
			Token *tu_tok = new_token(TUPLE_UNPACK, token->space);
			Node  *tu     = new_node(tu_tok);
			add_child(tu, new_node(tmp)); // first decl
			while (tokens[ecount]->type == COMA)
			{
				ecount++; // consume ,
				Token *nname = find(ID, 0);
				Token *ntype = find(DATA_TYPES, 0);
				if (ntype->type == ARRAY_TYPE)
				{
					expect_token(LBRA, "expected [ after array");
					int depth     = 1;
					while (find(LBRA, 0)) depth++;
					Token *elem_type = find(DATA_TYPES, ID, 0);
					check(!elem_type, "expected element type in array type");
					for (int i = 0; i < depth; i++)
						expect_token(RBRA, "expected ] in array type");
					ntype->Array.elem_type = elem_type->type;
					ntype->Array.depth     = depth;
					ntype->retType         = ARRAY_TYPE;
					if (elem_type->type == ID)
					{
						Node *sd = get_struct(elem_type->name);
						if (sd) { ntype->Array.elem_type = STRUCT_CALL; ntype->Array.struct_ptr = sd; }
					}
				}
				setName(ntype, nname->name);
				ntype->is_dec = true;
				add_child(tu, new_node(ntype));
			}
			expect_token(ASSIGN, "expected = in tuple unpack");
			tu->left = expr_node();
			return tu;
		}
		return new_node(tmp);
	}

	// variable declaration (struct type): name StructName [ref] [= ...]
	Node *st_dec = NULL;
	bool cond   = token->type == ID && tokens[ecount]->type == ID;
	if (cond && (st_dec = get_struct(tokens[ecount]->name)))
	{
		find(ID, 0); // skip struct data type
		bool is_ref = find(REF, 0) != NULL;
		if (is_ref)
		{
			expect_token(ASSIGN, "'%s': ref must be initialized at "
			             "declaration, e.g: x int ref = a", token->name);
			ecount--;
		}
		token->type       = STRUCT_CALL;
		token->is_dec     = true;
		token->is_ref     = is_ref;
		token->Struct.ptr = st_dec;
		return new_node(token);
	}

	// function call or main declaration
	if (token->type == ID && find(LPAR, 0))
	{
		Node *node = new_node(token);
		if (strcmp(token->name, "main") == 0)
		{
			// Function main:
			//    + children: code bloc
			expect_token(RPAR, "expected ) after main declaration");
			expect_token(DOTS, "expected : after main() declaration");

			enter_scope(node);
			node->token->type    = FDEC;
			node->token->retType = INT;
			node->left           = new_node(new_token(ARGS, node->token->space));

			Node *last = NULL;
			while (within(node->token->space))
			{
				last = expr_node();
				add_child(node, last);
			}
			if (!last || last->token->type != RETURN)
			{
				last       = new_node(new_token(RETURN, node->token->space + TAB));
				last->left = new_node(new_token(INT, node->token->space + TAB));
				add_child(node, last);
			}
			exit_scope();
		}
		else
		{
			// Function call:
			//    + left:
			//       + children: parameters
			node->token->type = FCALL;
			Token *arg = NULL;
			node->left        = new_node(new_token(ARGS, node->token->space));

			while (!found_error && !(arg = find(RPAR, END, 0)))
			{
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			check(!found_error && arg->type != RPAR, "expected ) after function call");
		}
		return node;
	}
	return new_node(token);
}

// [STRUCT_DEF] Name
// ├──[FIELD_TYPE] field1
// ├──[FIELD_TYPE] field2
// └──methods[]: [FDEC] Name.method …
static Node *parse_struct_def(Token *token)
{
	// Struct def layout:
	//    + children: attributes
	//    + methods[]: method FDEC nodes
	Node  *node = new_node(token);
	Token *st_name;
	if (check(!(st_name = find(ID, 0)), "expected identifier after struct definition"))
		return NULL;
	expect_token(DOTS, "expected dots after struct definition\n");

	setName(node->token, st_name->name);
	node->token->type       = STRUCT_DEF;
	node->token->Struct.ptr = node;
	new_struct(node);
	enter_scope(node);

	Node **methods      = NULL;
	int method_count = 0;
	int method_cap   = 0;

	while (within(node->token->space))
	{
		Node *child = prime_node();
		if (!child) return syntax_error();
		if (child->token->type == FDEC)
		{
			resize_array(methods, Node*, method_cap, method_count);
			methods[method_count++] = child;
		}
		else
		{
			if (check(!child->token->is_dec, "invalid attribute"))
			{
				exit_scope();
				free(methods);
				return syntax_error();
			}
			child->token->is_dec = false;
			add_child(node, child);
		}
	}
	exit_scope();

	// Hoist methods AFTER struct is registered so gen_asm sees STRUCT_DEF first
	for (int i = 0; i < method_count; i++)
	{
		Token *mt = methods[i]->token;
		if (strcmp(mt->name, "init") == 0)
		{
			if (check(mt->retType != VOID, "struct '%s': init() must return void", node->token->name))
			{
				free(methods);
				return syntax_error();
			}
			if (check(methods[i]->left->children_count != 1, "struct '%s': init() must take no parameters", node->token->name))
			{
				free(methods);
				return syntax_error();
			}
			node->token->is_init = true;
		}
		else if (strcmp(mt->name, "clean") == 0)
		{
			if (check(mt->retType != VOID, "struct '%s': clean() must return void", node->token->name))
			{
				free(methods);
				return syntax_error();
			}
			if (check(methods[i]->left->children_count != 1, "struct '%s': clean() must take no parameters", node->token->name))
			{
				free(methods);
				return syntax_error();
			}
			node->token->is_clean = true;
		}
		char *qualified = strjoin(node->token->name, ".", mt->name);
		setName(mt, qualified);
		free(qualified);
		add_method(node, methods[i]);
	}
	free(methods);
	return node;
}

// [ENUM_DEF] Name
// ├──[INT] VariantA  (value=0, is_global)
// ├──[INT] VariantB  (value=1, is_global)
// └──...
static Node *parse_enum_def(Token *token)
{
	Token *ename;
	if (check(!(ename = find(ID, 0)), "expected identifier after enum"))
		return NULL;
	if (check(!find(DOTS, 0), "expected ':' after enum name"))
		return NULL;

	setName(token, ename->name);
	token->type = ENUM_DEF;
	Node *node    = new_node(token);

	int counter = 0;
	while (within(token->space))
	{
		Token *var_tok = find(ID, 0);
		if (!var_tok) break;
		// Create a synthetic ura_scope INT constant for this variant
		Token *var = allocate(1, sizeof(Token));
		var->type      = INT;
		var->name      = strdup(var_tok->name);
		var->is_global = true;
		var->is_dec    = true;
		var->Int.value = counter++;
		var->line      = var_tok->line;
		var->filename  = var_tok->filename;
		new_variable(var);
		add_child(node, new_node(var));
		// Consume any commas and additional variants on the same line
		while (find(COMA, 0))
		{
			var_tok        = find(ID, 0);
			if (!var_tok) break;
			var            = allocate(1, sizeof(Token));
			var->type      = INT;
			var->name      = strdup(var_tok->name);
			var->is_global = true;
			var->is_dec    = true;
			var->Int.value = counter++;
			var->line      = var_tok->line;
			var->filename  = var_tok->filename;
			new_variable(var);
			add_child(node, new_node(var));
		}
	}
	return node;
}

// [IF]
// ├──left:     condition
// ├──children: body stmts
// └──right: [ELIF]
//           ├──left:     condition
//           ├──children: body stmts
//           └──right: [ELSE]
//                     └──children: body stmts
static Node *parse_if(Token *token)
{
	// if/elif/else layout:
	//    + left    : condition
	//    + children: code bloc
	//    + right   : elif/else node (same layout)
	Node *node = new_node(token);
	enter_scope(node);

	node->left = expr_node();
	expect_token(DOTS, "expected : after if condition");

	while (within(node->token->space)) add_child(node, expr_node());

	Node *curr = node;
	while (includes(tokens[ecount]->type, ELSE, ELIF, 0) && within(node->token->space - TAB))
	{
		Token *tok = find(ELSE, ELIF, 0);

		curr->right = new_node(tok);
		curr        = curr->right;

		if (tok->type == ELIF)
		{
			enter_scope(curr);
			curr->left = expr_node();
			expect_token(DOTS, "expected : after elif condition");
			while (within(tok->space)) add_child(curr, expr_node());
			exit_scope();
		}
		else if (tok->type == ELSE)
		{
			enter_scope(curr);
			expect_token(DOTS, "expected : after else");
			while (within(tok->space)) add_child(curr, expr_node());
			exit_scope();
			break;
		}
	}
	exit_scope();
	return node;
}

// [FDEC] name ret [TYPE]
// ├──[ARGS]
// │  ├──[TYPE] param1
// │  └──[STRUCT_CALL] param2    (struct param)
// └──children: body stmts
//
// [FDEC] name ret [TUPLE]       (tuple return)
// ├──[ARGS]
// │  └──…
// └──children: body stmts
static Node *parse_fdec(Token *token)
{
	// Function declaration layout:
	//    + left:
	//       + children: arguments
	//    + children: code block
	Node  *node  = new_node(token);
	Token *fname = find(ID, 0);
	if (check(!fname, "expected identifier after fn declaration"))
		return syntax_error();

	// Check if we're inside a struct scope (method)
	Node *struct_owner = NULL;
	Node *parent       = level_scope[sccount];
	if (parent && parent->token->type == STRUCT_DEF)
		struct_owner = parent;

	enter_scope(node);
	expect_token(LPAR, "expected ( after function declaration");

	node->left = new_node(new_token(ARGS, node->token->space));

	Token *last;
	while (!found_error && !(last = find(RPAR, END, 0)))
	{
		if (find(VARIADIC, 0))
		{
			node->token->is_variadic = true;
			last = find(RPAR, 0);
			check(!last, "expected ) after function ... in variadic function");
			break;
		}
		else
		{
			Token *name = find(ID, 0);
			if (check(!name, "expected identifier in function argument %s", fname->name))
				return syntax_error();

			Token *data_type = find(DATA_TYPES, ID, 0);
			bool is_ref    = find(REF, 0) != NULL;
			if (check(!data_type, "expected data type in function argument")) break;
			if (data_type->type == ID)
			{
				Node *to_find = get_struct(data_type->name);
				if (to_find) data_type->type = STRUCT_CALL;
			}
			Node *curr;
			if (data_type->type == STRUCT_CALL)
			{
				Node *st = get_struct(data_type->name);
				if (check(!st, "Unknown struct type '%s'", data_type->name)) break;
				data_type->Struct.ptr = st;
				curr = new_node(data_type);
				data_type->is_ref     = is_ref;
				setName(data_type, name->name);
			}
			else
			{
				curr         = new_node(name);
				name->is_ref = is_ref;
				name->type   = data_type->type;
			}
			curr->token->is_dec   = true;
			curr->token->is_param = true;
			add_child(node->left, curr);
		}
		if (tokens[ecount]->type != RPAR) expect_token(COMA, "expected coma");
	}
	check(!found_error && last->type != RPAR, "expected ) after function declaration");

	// If inside a struct, prepend implicit 'self' parameter
	if (struct_owner)
	{
		Token *self_tok = new_token(STRUCT_CALL, node->token->space);
		setName(self_tok, "self");
		self_tok->Struct.ptr = struct_owner;
		self_tok->is_dec     = true;
		self_tok->is_ref     = true;
		add_child(node->left, new_node(self_tok));
	}

	if (tokens[ecount]->type == LPAR)
	{
		// Tuple return type: fn foo(…) (int, chars):
		ecount++; // consume (
		node->token->retType = TUPLE;
		int tc = 0;
		while (tokens[ecount]->type != RPAR && tc < 8)
		{
			if (tokens[ecount]->type == ARRAY_TYPE)
			{
				Token *t  = tokens[ecount++];
				expect_token(LBRA, "expected [ in tuple array type");
				Token *et = find(DATA_TYPES, ID, 0);
				if (check(!et, "expected element type in tuple array")) return syntax_error();
				expect_token(RBRA, "expected ] in tuple array type");
				t->Array.elem_type = et->type;
				t->retType         = ARRAY_TYPE;
				if (et->type == ID)
				{
					Node *sd = get_struct(et->name);
					if (sd) { t->Array.elem_type = STRUCT_CALL; t->Array.struct_ptr = sd; }
				}
				node->token->Tuple.types[tc++] = t;
			}
			else if (tokens[ecount]->type == ID)
			{
				Token *t  = tokens[ecount++];
				Node  *st = get_struct(t->name);
				if (check(!st, "unknown type '%s' in tuple", t->name)) return syntax_error();
				t->type       = STRUCT_CALL;
				t->Struct.ptr = st;
				node->token->Tuple.types[tc++] = t;
			}
			else if (is_data_type(tokens[ecount]))
			{
				node->token->Tuple.types[tc++] = tokens[ecount++];
			}
			else
			{
				check(1, "unexpected token in tuple return type");
				return syntax_error();
			}
			if (tokens[ecount]->type == COMA) ecount++;
		}
		expect_token(RPAR, "expected ) after tuple return types");
		node->token->Tuple.count = tc;
	}
	else
	{
		if (check(!is_data_type(tokens[ecount]) && tokens[ecount]->type != ID,
		          "expected data type after fun declaration"))
			return syntax_error();
		Token *ret_tok = tokens[ecount++];
		if (ret_tok->type == ID)
		{
			Node *st = get_struct(ret_tok->name);
			if (check(!st, "unknown return type '%s'", ret_tok->name))
				return syntax_error();
			node->token->retType    = STRUCT_CALL;
			node->token->Struct.ptr = st;
		}
		else
			node->token->retType = ret_tok->type;
	}
	node->token->is_ref = find(REF, 0) != NULL;
	setName(node->token, fname->name);

	if (!node->token->is_proto)
	{
		Token *next = find(DOTS, 0);
		if (check(!found_error && !next, "expected : after function declaration"))
			return syntax_error();
		Node *child = NULL;

		if (next->type == DOTS)
			while (within(node->token->space)) child = add_child(node, expr_node());
		else
		{
			Token *retToken = copy_token(next);
			retToken->type = RETURN;
			Node  *retNode  = new_node(retToken);
			retNode->left  = expr_node();
			child          = add_child(node, retNode);
		}
		if (next->type == DOTS)
		{
			if (node->token->retType != VOID)
				check(!child || child->token->type != RETURN,
				      "expected return statement %s", node->token->name);
			else
			{
				Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
				ret->left = new_node(new_token(VOID, node->token->space + TAB));
				add_child(node, ret);
			}
		}
	}
	exit_scope();
	return node;
}

// [RETURN] ret [TYPE]        (single value)
// └──left: expr
//
// [RETURN] ret [TUPLE]       (multiple values)
// ├──expr1
// └──expr2
//
// [RETURN] ret [VOID]        (void function — left = [VOID] copy)
// └──left: [VOID]
static Node *parse_return(Token *token)
{
	// TODO: check if return type is compatible with function in current scope (done in gen_ir)
	Node *node = new_node(token);
	for (int i = sccount; i >= 0; i--)
	{
		Node *curr = level_scope[i];
		if (curr->token->type == FDEC)
		{
			if (curr->token->retType == VOID)
			{
				node->left = copy_node(node);
				node->left->token->type = VOID;
			}
			else if (curr->token->retType == TUPLE)
			{
				add_child(node, expr_node()); // first value
				while (tokens[ecount]->type == COMA)
				{
					ecount++;
					add_child(node, expr_node());
				}
			}
			else node->left = expr_node();
			break;
		}
	}
	return node;
}

// ----------------------------------------------------------------------------
// Primary expression parser
// ----------------------------------------------------------------------------
Node *prime_node()
{
	Node  *node = NULL;
	Token *token;

	if ((token = find(ID, DATA_TYPES, 0))) return parse_var_dec(token);
	if ((token = find(STRUCT_DEF, 0)))     return parse_struct_def(token);
	if ((token = find(ENUM_DEF, 0)))       return parse_enum_def(token);
	if ((token = find(IF, 0)))             return parse_if(token);

	if ((token = find(WHILE, 0)))
	{
		// while layout:
		//    + left    : condition
		//    + children: code bloc
		node       = new_node(token);
		enter_scope(node);
		node->left = expr_node();
		expect_token(DOTS, "expected : after if condition");
		while (within(node->token->space)) add_child(node, expr_node());
		exit_scope();
		return node;
	}
	if ((token = find(BREAK, CONTINUE, 0)))
		return new_node(token);

	if ((token = find(STACK, HEAP, 0)))
	{
		node = new_node(token);
		expect_token(LBRA, "expected [ after stack");

		int depth = 1;
		while (find(LBRA, 0)) depth++;

		Token *elem_type = find(DATA_TYPES, ID, 0);
		check(!elem_type, "expected element type in stack");

		for (int i = 0; i < depth; i++)
			expect_token(RBRA, "expected ] in stack type");

		expect_token(LPAR, "expected ( after stack[type]");

		node->token->retType         = ARRAY;
		node->token->Array.elem_type = elem_type->type;
		node->token->Array.depth     = depth;
		if (elem_type->type == ID)
		{
			Node *sd = get_struct(elem_type->name);
			if (sd) { node->token->Array.elem_type = STRUCT_CALL; node->token->Array.struct_ptr = sd; }
		}

		for (int i = 0; i < depth; i++)
		{
			add_child(node, expr_node());
			if (i < depth - 1)
				expect_token(COMA, "expected , between dimensions");
		}
		expect_token(RPAR, "expected ) after stack size");
		return node;
	}
	if ((token = find(TYPEOF, SIZEOF, 0)))
	{
		char *msg  = token->type == TYPEOF ? "typeof" : "sizeof";
		Type type = token->type == TYPEOF ? CHARS : INT;
		node                 = new_node(token);
		expect_token(LPAR, "%s: expected (", msg);
		node->left           = prime_node();
		expect_token(RPAR, "%s: expected )", msg);
		node->token->retType = type;
		return node;
	}
	if ((token = find(OUTPUT, 0)))
	{
		node       = new_node(token);
		expect_token(LPAR, "output: expected (");
		node->left = new_node(new_token(ARGS, token->space));
		Token *end_tok = NULL;
		while (!found_error && !(end_tok = find(RPAR, END, 0)))
		{
			Node *curr = expr_node();
			add_child(node->left, curr);
			find(COMA, 0);
		}
		if (check(!found_error && end_tok->type != RPAR, "output: expected )"))
			return syntax_error();
		node->token->retType = VOID;
		return node;
	}
	if ((token = find(PROTO, 0)))
	{
		if (includes(tokens[ecount]->type, FDEC, STRUCT_DEF, 0))
			tokens[ecount]->is_proto = true;
		else check(1, "expected <fn> or <struct> after proto");
		return expr_node();
	}

	if ((token = find(FDEC, 0)))   return parse_fdec(token);
	if ((token = find(RETURN, 0))) return parse_return(token);

	if ((token = find(NOT, 0)))
	{
		node       = new_node(token);
		node->left = expr_node();
		return node;
	}
	if ((token = find(LPAR, 0)))
	{
		if (tokens[ecount]->type != RPAR) node = expr_node();
		expect_token(RPAR, "expected right )");
		return node;
	}
	debug("%k\n", tokens[ecount - 1]);
	check(1, "Unexpected token has type %s line %d", to_string(tokens[ecount]->type), tokens[ecount]->line);
	return syntax_error();
}

void unuse(Node *node)
{
	if (!node || !node->token) return;

	if (node->left  && node->left->token)
	{
		if (node->left->token->used  > 0) node->left->token->used--;
	}
	if (node->right && node->right->token)
	{
		if (node->right->token->used > 0) node->right->token->used--;
	}
	for (int i = 0; i < node->children_count; i++)
		if (node->children[i] && node->children[i]->token)
			if (node->children[i]->token->used > 0) node->children[i]->token->used--;
}

Node *optimize_ir(Node *node, bool *changed)
{
	if (!node || found_error) return NULL;

	node->left  = optimize_ir(node->left, changed);
	node->right = optimize_ir(node->right, changed);

	if (node->children_count)
	{
		int j            = 0;
		Node **new_children = allocate(node->children_size, sizeof(Node*));
		for (int i = 0; i < node->children_count; i++)
		{
			Node *child = optimize_ir(node->children[i], changed);
			if (child) new_children[j++] = child;
		}
		node->children_count = j;
		node->children       = new_children;
	}

	if (!node->token) return node;
	switch (node->token->type)
	{
	case FDEC: case PROTO:
	{
		if (node->token->used == 0 && strcmp(node->token->name, "main") != 0)
		{
			debug(RED "remove %k\n" RESET, node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
		// TODO: add disable default if next is assignment
#if 0
	case INT: case BOOL: case CHAR: case FLOAT:
	case LONG: case CHARS: case PTR: case STRUCT_CALL:
	{
		if (node->token->is_dec && node->token->used == 0)
		{
			debug(RED "remove unused var %k\n" RESET, node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	case ADD: case SUB: case MUL: case DIV: case MOD:
	case AND: case OR: case NOT:
	case EQUAL: case NOT_EQUAL:
	case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	{
		if (node->token->used == 0)
		{
			debug(RED "remove unused op %k\n" RESET, node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	case ASSIGN:
	{
		bool cond = node->left == NULL || (node->left
		                                   && node->left->token
		                                   && node->left->token->used == 0
		                                   && is_data_type(node->left->token));
		if (cond)
		{
			debug(RED "remove unused assign %k\n" RESET, node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
#endif
	default: break;
	}

	return node;
}

// ----------------------------------------------------------------------------
// gen_ir helpers — extracted from large switch cases
// ----------------------------------------------------------------------------

// [FCALL] obj.method ret [TYPE]   (is_method_call = true)
// └──[ARGS]
//    ├──arg1
//    ├──arg2
//    └──[STRUCT_CALL] obj         ← last child = receiver
static void ir_method_call(Node *node)
{
	// resolve object type
	Node *obj_node = node->left->children[node->left->children_count - 1];
	gen_ir(obj_node);
	if (found_error) return;
	Token *obj = obj_node->token;

	// build qualified name: "User.greet"
	obj->Struct.ptr->token->used++; // mark struct def as used
	char *struct_name = obj->Struct.ptr->token->name;
	char *qname       = strjoin(struct_name, ".", node->token->name);
	setName(node->token, qname);
	free(qname);
	node->token->is_method_call = false;

	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func = copy_node(func);

	// first arg already gen_ir'd (obj), do rest
	obj_node->token->used++;
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++)
	{
		Node *carg = node->left->children[i];
		gen_ir(carg);
		if (check(carg->token->type == ID, "Indeclared variable %s", carg->token->name))
			break;
		carg->token->used++;
	}
	free_node(func);
	set_ret_type(node);
}

// [FCALL] name ret [TYPE]
// └──[ARGS]
//    ├──arg1
//    └──arg2
static void ir_regular_call(Node *node)
{
	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func = copy_node(func);

	Node *call_args = node->left;
	Node *dec_args  = func->left;
	// TODO: check number of arguments
	for (int i = 0; !found_error && call_args && i < call_args->children_count; i++)
	{
		Node *carg = call_args->children[i];
		gen_ir(carg);
		carg->token->used++;
		Token *src = carg->token;
		if (check(src->type == ID, "Undeclared variable %s", carg->token->name)) break;

		if (i < dec_args->children_count)
		{
			bool param_is_ref = dec_args->children[i]->token->is_ref;
			if (param_is_ref)
				if (check(!src->name || src->type == FCALL,
				          "'%s': ref parameter requires a named variable",
				          dec_args->children[i]->token->name))
					break;

			Type param_type   = dec_args->children[i]->token->type;
			Type arg_type     = src->type;
			bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
			bool arg_is_int   = includes(arg_type,   NUMERIC_TYPES, 0);
			if (param_is_int && arg_is_int && param_type != arg_type)
			{
				Token *as_tok   = new_token(AS, src->space);
				as_tok->retType = param_type;
				Node  *tgt_node = new_node(new_token(param_type, src->space));
				Node  *as_node  = new_node(as_tok);
				as_node->left          = carg;
				as_node->right         = tgt_node;
				call_args->children[i] = as_node;
			}
		}
	}
	free_node(func);
	set_ret_type(node);
}

// [DOT] ret [FIELD_TYPE]
// ├──left:  [STRUCT_CALL] obj
// └──right: [ID] field   → resolved to field's type + index
static void ir_dot(Node *node)
{
	gen_ir(node->left);
	if (found_error) return;
	node->left->token->used++;
	// Also mark the struct definition itself as used
	if (node->left->token->Struct.ptr)
		node->left->token->Struct.ptr->token->used++;
	Type retType = 0;

	Node *src     = NULL;
	if (node->left->token->type == STRUCT_CALL)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else if (node->left->token->retType == STRUCT_CALL && node->left->token->Struct.ptr)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else
	{
		check(1, "handle this case %s", to_string(node->left->token->type));
		return;
	}

	bool found = false;
	for (int i = 0; src && i < src->children_count; i++)
	{
		Node *src_child = src->children[i];
		if (strcmp(src_child->token->name, node->right->token->name) == 0)
		{
			found   = true;
			retType = src_child->token->type;
			node->right->token->Struct.index = i;
			node->right->token->type         = src_child->token->type;
			node->right->token->retType      = src_child->token->retType;
			if (src_child->token->type == STRUCT_CALL)
				node->right->token->Struct.ptr = src_child->token->Struct.ptr;
			else if (src_child->token->type == ARRAY_TYPE)
			{
				node->right->token->Array.elem_type = src_child->token->Array.elem_type;
				node->right->token->Array.depth     = src_child->token->Array.depth;
			}
			break;
		}
	}
	if (check(!found, ""))
	{
		debug("%s%k doesn't have attribute %k%s\n", RED,
		      node->left->token, node->right->token, RESET);
		return;
	}

	node->token->retType = retType;
	if (retType == STRUCT_CALL)
		node->token->Struct.ptr = node->right->token->Struct.ptr;
	else if (retType == ARRAY_TYPE)
	{
		node->token->Array.elem_type = node->right->token->Array.elem_type;
		node->token->Array.depth     = node->right->token->Array.depth;
	}
}

// ----------------------------------------------------------------------------
// Semantic analysis / name resolution pass
// ----------------------------------------------------------------------------

void gen_ir(Node *node)
{
	if (found_error) return;
	Node *left  = node->left;
	Node *right = node->right;
	switch (node->token->type)
	{
	// [ID] varname   → resolved to declared token (type, llvm.elem, etc.)
	case ID:
	{
		Token *find = get_variable(node->token->name);
		if (find)
			node->token = find;
		break;
	}
	// [STRUCT_CALL] varname → StructType   (is_dec = declaration | usage)
	case STRUCT_CALL:
	{
		Node *src = get_struct(node->token->Struct.ptr->token->name);
		node->token->Struct.ptr = src;
		if (node->token->is_dec)
		{
			if (sccount == 1) node->token->is_global = true;
			else src->token->used++; // local declaration → always emit struct def
			new_variable(node->token);
			if (src->token->is_init)
			{
				// TODO: to be checked later
				char *qname   = strjoin(src->token->name, ".init", NULL);
				Node *init_fn = get_function(qname);
				free(qname);
				if (init_fn) init_fn->token->used++;
			}
		}
		break;
	}
	// [INT/BOOL/CHAR/CHARS/FLOAT/LONG/VOID] name   (is_dec → register var)
	// [INT] [42]                                     (literal → mark used)
	case INT: case BOOL: case CHAR: case ARRAY_TYPE:
	case FLOAT: case LONG: case CHARS: case PTR: case VOID:
	{
		if (node->token->is_dec)
		{
			if (sccount == 1) node->token->is_global = true;
			new_variable(node->token);
		}
		else node->token->used++;
		break;
	}
	// [TUPLE_UNPACK]
	// ├──[TYPE] a
	// ├──[TYPE] b
	// └──left: [FCALL] fn ret [TUPLE]
	case TUPLE_UNPACK:
	{
		// Register each LHS variable, then process RHS
		for (int i = 0; i < node->children_count; i++)
		{
			Node *lhs = node->children[i];
			if (lhs->token->is_dec)
			{
				if (sccount == 1) lhs->token->is_global = true;
				new_variable(lhs->token);
			}
		}
		gen_ir(node->left); // RHS function call
		break;
	}
	// [ASSIGN] ret [LEFT_TYPE]
	// ├──left:  lvalue
	// └──right: rvalue
	case ASSIGN:
	{
		// TODO: check compatibility
		// TODO: add all ir_bound checks in &ll places
		gen_ir(left);
		gen_ir(right);
		if (!compatible(left->token, right->token))
		{
			check(1, "Invalid assignement %k= %k", left->token, right->token);
			break;
		}
		bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound) &&
		                (!right->token->name || right->token->type == FCALL);
		if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name))
			break;
		node->token->retType = left->token->retType;
		if (left->token->is_ref)
			left->token->ir_bound = true;
		break;
	}
	// [+=/-=/etc.] ret [LEFT_TYPE]
	// ├──left:  lvalue
	// └──right: rvalue
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
	{
		gen_ir(left);
		bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound);
		if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name))
			break;
		gen_ir(right);
		node->token->retType = left->token->retType;
		node->token->used++;
		node->left->token->used++;
		node->right->token->used++;
		break;
	}
	// [NOT] ret [BOOL]        [BNOT] ret [LEFT_TYPE]
	// └──left: expr           └──left: expr
	case NOT: case BNOT:
	{
		// TODO: left must be boolean
		gen_ir(left);
		node->token->used++;
		node->left->token->used++;
		if (node->token->type == NOT)
			node->token->retType = BOOL;
		else
			node->token->retType = left->token->retType ?
			                       left->token->retType : left->token->type;
		break;
	}
	// [ADD/SUB/MUL/DIV/MOD/AND/OR/BAND/BOR/BXOR/LSHIFT/RSHIFT/…] ret [TYPE]
	// ├──left:  expr
	// └──right: expr
	case ADD: case SUB: case MUL: case DIV: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
	case GREAT_EQUAL: case MOD: case AND: case OR:
	case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
	{
		// TODO: check compatibility
		gen_ir(left);
		gen_ir(right);
		check(!left || !left->token, "error in assignment, left is NULL");
		check(!right || !right->token, "error in assignment, right is NULL");
		if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
			node->token->retType = left->token->retType;
		else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0))
			node->token->retType = BOOL;
		node->token->used++;
		node->left->token->used++;
		node->right->token->used++;
		set_ret_type(node);
		break;
	}
	// [WHILE]
	// ├──left:     condition
	// └──children: body stmts
	case WHILE:
	{
		enter_scope(node);
		gen_ir(node->left); // condition
		node->left->token->used++;
		for (int i = 0; i < node->children_count; i++) gen_ir(node->children[i]);
		exit_scope();
		node->token->used++;
		break;
	}
	// [IF]
	// ├──left:     condition
	// ├──children: body stmts
	// └──right: [ELIF]
	//           ├──left:     condition
	//           ├──children: body stmts
	//           └──right: [ELSE]
	//                     └──children: body stmts
	case IF:
	{
		enter_scope(node);
		Node *curr = node;
		while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
		{
			if (includes(curr->token->type, IF, ELIF, 0))
			{
				check(curr->left == NULL, "error");
				gen_ir(curr->left); // condition
				curr->left->token->used++;
			}
			for (int i = 0; i < curr->children_count; i++)
				gen_ir(curr->children[i]);
			curr = curr->right;
		}
		exit_scope();
		node->token->used++;
		break;
	}
	// [FDEC] name ret [TYPE]
	// ├──[ARGS]
	// │  ├──[TYPE] param1
	// │  └──[TYPE] param2
	// └──children: body stmts
	case FDEC:
	{
		new_function(node);
		enter_scope(node);
		Node **params = (node->left ? node->left->children : NULL);
		for (int i = 0; params && i < node->left->children_count && !found_error; i++)
		{
			gen_ir(params[i]);
			params[i]->token->used++;
		}
		for (int i = 0; !node->token->is_proto && i < node->children_count; i++)
			gen_ir(node->children[i]);
		exit_scope();
		break;
	}
	// [FCALL] name ret [TYPE]   →   ir_regular_call
	// [FCALL] obj.method        →   ir_method_call  (is_method_call=true)
	case FCALL:
	{
		if (node->token->is_method_call && node->left)
			ir_method_call(node);
		else
			ir_regular_call(node);
		break;
	}
	// [AS] ret [TARGET_TYPE]
	// ├──left:  expr
	// └──right: [TARGET_TYPE]
	case AS:
	{
		gen_ir(node->left);
		node->token->retType = node->right->token->type;
		break;
	}
	// [DOT] ret [FIELD_TYPE]     →   ir_dot
	// ├──left:  [STRUCT_CALL] obj
	// └──right: [ID] field
	case DOT:
	{
		ir_dot(node);
		break;
	}
	// [ACCESS] ret [ELEM_TYPE]
	// ├──left:  array / chars
	// └──right: index expr
	case ACCESS:
	{
		gen_ir(node->left);
		gen_ir(node->right);
		if (found_error) break;
		node->right->token->used++;
		Type retType = 0;
		Type type    = node->left->token->retType ? node->left->token->retType : node->left->token->type;
		int depth   = node->left->token->Array.depth;
		switch (type)
		{
		case CHARS: retType = CHAR; break;
		case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
			if (depth > 1)
			{
				// still multi-dim: return sub-array type, depth-1
				retType = ARRAY;
				node->token->Array.elem_type  = node->left->token->Array.elem_type;
				node->token->Array.depth      = depth - 1;
				node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
			}
			else
			{
				retType = node->left->token->Array.elem_type;
				if (retType == STRUCT_CALL)
					node->token->Struct.ptr = node->left->token->Array.struct_ptr;
			}
			break;
		default:
			check(1, "handle this case %s", to_string(node->left->token->type));
			break;
		}
		node->token->retType = retType;
		break;
	}
	// [RETURN] ret [TYPE]        [RETURN] ret [TUPLE]
	// └──left: expr              ├──expr1
	//                            └──expr2
	case RETURN:
	{
		if (node->children_count > 0)
		{
			// Tuple return: process each child expression
			for (int i = 0; i < node->children_count; i++)
				gen_ir(node->children[i]);
			node->token->retType = TUPLE;
			break;
		}
		gen_ir(node->left);
		node->token->retType = node->left->token->type;
		if (node->left->token->type == STRUCT_CALL)
			node->token->Struct.ptr = node->left->token->Struct.ptr;
		node->left->token->used++;
		break;
	}
	// [OUTPUT]
	// └──[ARGS]
	//    ├──fmt expr
	//    └──value exprs...
	case OUTPUT:
	{
		for (int i = 0; i < node->left->children_count; i++)
		{
			gen_ir(node->left->children[i]);
			node->left->children[i]->token->used++; // keep globals/vars alive (mirrors FCALL arg handling)
		}
		// output() compiles to printf internally — codegen declares printf as needed
		node->token->retType = VOID;
		break;
	}
	// [TYPEOF] ret [CHARS]       [SIZEOF] ret [INT]
	// └──left: expr              └──left: expr
	case TYPEOF: case SIZEOF:
	{
		gen_ir(node->left);
		Token *type_tok = node->left->token;
		Type type     = type_tok->type ? type_tok->type : type_tok->retType;
		if (node->token->type == TYPEOF)
		{
			node->token->retType     = CHARS;
			node->token->Chars.value = strdup(to_string(type));
		}
		else
		{
			node->token->retType   = INT;
			node->token->Int.value = 0; // placeholder
		}
		node->token->used++;
		break;
	}
	// [STACK] ret [ARRAY] elem_type    [HEAP] ret [ARRAY] elem_type
	// └──left: size expr               └──left: count expr
	case STACK: case HEAP:
	{
		for (int i = 0; i < node->children_count; i++)
		{
			gen_ir(node->children[i]);
			node->children[i]->token->used++;
		}
		node->token->used++;
		break;
	}
	// [STRUCT_DEF] Name
	// ├──[FIELD_TYPE] field1
	// ├──[FIELD_TYPE] field2
	// └──methods[]: [FDEC] ...
	case STRUCT_DEF:
	{
		for (int i = 0; node && i < node->methods_count; i++)
			gen_ir(node->methods[i]);
		break;
	}
	// [ENUM_DEF] Name
	// ├──[INT] VariantA  (ura_scope constant)
	// └──...
	case ENUM_DEF:
	{
		for (int i = 0; i < node->children_count; i++)
			node->children[i]->token->used++;
		break;
	}
	// [BREAK]    [CONTINUE]
	case BREAK: case CONTINUE: break;
	default:
	{
		todo(1, "handle this case %s", to_string(node->token->type));
		break;
	}
	}
}

// FILE MANAGEMENT
char *open_file(char* filename)
{
	// TODO: use realpath here
	if (found_error) return NULL;

	File file = fopen(filename, "r");
	if (check(!file, "openning %s", filename)) return NULL;
	fseek(file, 0, SEEK_END);
	int size  = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *input = allocate((size + 1), sizeof(char));
	if (input) fread(input, size, sizeof(char), file);
	fclose(file);
	return input;
}

char *resolve_path(char* path)
{
	if (path == NULL) return NULL;
	char  *cleaned = allocate(strlen(path) + 5, 1);
	if (!cleaned) return NULL;
	size_t i = 0, j = 0;
	while (path[i])
	{
		cleaned[j++] = path[i++];
		while (path[i] == '/')
		{
			if (cleaned[j - 1] != '/') cleaned[j++] = '/';
			i++;
		}
	}
	if (j > 1 && cleaned[j - 1] == '/') j--;
	cleaned[j] = '\0';
	return cleaned;
}

bool add_file(char *filename)
{
	resize_array(used_files, char*, usize, ucount);
	for (int i = 0; i < ucount; i++)
	{
		if (strcmp(filename, used_files[i]) == 0)
			return false;
	}
	used_files[ucount++] = filename;
	return true;
}

// MEMORY
void *allocate_func(int line, int len, int size)
{
	void *res = calloc(len, size);
	check(!res, "allocate did failed in line %d", line);
	return res;
}

void free_token(Token *token)
{
	free(token->name);
	free(token->Chars.value);
	free(token);
}

void free_node(Node* node)
{
	if (!node) return;
	for (int i = 0; i < node->children_count; i++) free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->functions);
	free(node->variables);
	free(node->structs);
	free(node->methods);
	free(node);
}

void free_local_memory()
{
	for (int i = 0; tokens && tokens[i]; i++)
	{
		free_token(tokens[i]);
		tokens[i] = NULL;
	}
	tcount = 0;
	ecount = 0;
}

// PARSING HELPERS
inline void add_token(Token *token)
{
	resize_array(tokens, Token*, tsize, tcount);
	tokens[tcount++] = token;
}

Node *new_node(Token *token)
{
	debug("new node: %k\n", token);
	Node *new = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *add_child(Node *node, Node *child)
{
	if (child)
	{
		resize_array(node->children, Node*, node->children_size, node->children_count);
		child->token->space = node->token->space + TAB;
		node->children[node->children_count++] = child;
	}
	return child;
}

void enter_scope(Node *node)
{
	debug(CYAN "Enter Scope: %k index %d\n" RESET, node->token, sccount + 1);
	resize_array(level_scope, Node*, scsize, sccount);
	sccount++;
	level_scope[sccount] = node;
	scope = level_scope[sccount];
}

void exit_scope()
{
	if (check(sccount < 0, "No active scope to exit")) return;
	debug("%sExit Scope: %k index %d%s\n", CYAN, level_scope[sccount]->token, sccount, RESET);
	level_scope[sccount] = NULL;
	sccount--;
	scope = level_scope[sccount];
}

bool includes(Type to_find, ...)
{
	if (found_error) return false;
	va_list ap; Type current; va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

void setName(Token *token, char *name)
{
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

char *to_string(Type type)
{
	char *res[END + 1] =
	{
		[ID]         = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
		[INT]        = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
		[FDEC]       = "FDEC",
		[FCALL]      = "CALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
		[IF]         = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
		[WHILE]      = "WHILE", [BREAK] = "BRK", [CONTINUE] = "CONT",
		[RETURN]     = "RET",
		[BAND]       = "BAND", [BOR] = "BOR", [BXOR] = "BXOR",
		[BNOT]       = "BNOT", [LSHIFT] = "LSHIFT", [RSHIFT] = "RSHIFT",
		[ADD]        = "ADD",
		[SUB]        = "SUB", [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
		[ADD_ASSIGN] = "ADD_ASS", [SUB_ASSIGN] = "SUB_ASS",
		[MUL_ASSIGN] = "MUL_ASS", [DIV_ASSIGN] = "DIV_ASS",
		[MOD_ASSIGN] = "MOD_ASS", [ACCESS] = "ACC",
		[MOD]        = "MOD", [COMA] = "COMA", [REF] = "REF",
		[EQUAL]      = "EQ", [NOT_EQUAL] = "NEQ", [LESS] = "LT",
		[GREAT]      = "GT", [LESS_EQUAL] = "LE", [NOT] = "NOT",
		[GREAT_EQUAL]= "GE", [AND] = "AND", [OR] = "OR",
		[DOTS]       = "DOTS",  [PROTO] = "PROT", [VARIADIC] = "VAR",
		[TYPEOF]     = "TYPEOF", [SIZEOF] = "SIZEOF", [OUTPUT] = "OUTPUT", [ARGS] = "ARGS",
		[CHILDREN]   = "CHILDREN",
		[AS]         = "AS", [STACK] = "STACK", [HEAP] = "HEAP",
		[ARRAY_TYPE] = "ARRAY_TYPE",
		//[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
		[STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
		[ENUM_DEF]   = "ENUM_DEF",   [ENUM_CALL]   = "ENUM_CALL",
		[TUPLE]      = "TUPLE",      [TUPLE_UNPACK] = "TUPLE_UNPACK",
		[LBRA]       = "LBRA", [RBRA] = "RBRA", [ARRAY] = "ARRAY",
		[DOT]        = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR",
	};

	if (check(!res[type], "handle this case %d\n", type))
	{
		// seg();
		exit(1);
	}
	return res[type];
}

Token *find(Type type, ...)
{
	if (found_error) return NULL;
	va_list ap; va_start(ap, type);
	while (type)
	{
		if (type == tokens[ecount]->type)
			return tokens[ecount++];
		type = va_arg(ap, Type);
	}
	return NULL;
};

bool within(int space)
{
	Token *curr = tokens[ecount];
	return !found_error && curr->space > space && curr->type != END;
}

Node *new_struct(Node *node)
{
	debug(CYAN "new struct [%s] in scope %k\n" RESET, node->token->name, scope->token);
	for (int i = 0; i < scope->structs_count; i++)
	{
		Token *curr = scope->structs[i]->token;
		bool cond = (strcmp(curr->name, node->token->name) == 0);
		check(cond, "Redefinition of %s", node->token->name);
	}
	resize_array(scope->structs, Node*, scope->structs_size, scope->structs_count);
	scope->structs[scope->structs_count++] = node;
	return node;
}

Node *get_struct(char *name)
{
	debug(CYAN "get struct [%s] from scope %k\n"RESET, name, scope->token);
	for (int j = sccount; j > 0; j--)
	{
		Node *node = level_scope[j];
		todo(node == NULL, RED "Error accessing NULL, %d"RESET, j);
		for (int i = 0; i < node->structs_count; i++)
			if (strcmp(node->structs[i]->token->name, name) == 0)
				return node->structs[i];
	}
	return NULL;
}

inline void add_struct(Node *parent, Node *node)
{
	resize_array(parent->structs, Node*, parent->structs_size, parent->structs_count);
	parent->structs[parent->structs_count++] = node;
}

inline void add_method(Node *parent, Node *node)
{
	resize_array(parent->methods, Node*, parent->methods_size, parent->methods_count);
	parent->methods[parent->methods_count++] = node;
}

Node *syntax_error()
{
	found_error = true;
	static Node *node;
	if (node == NULL) node = new_node(new_token(SYNTAX_ERROR, -1));
	return node;
	return node;
}

// IR / SCOPE / VARIABLE MANAGEMENT
inline void add_variable(Node *parent, Token *token)
{
	resize_array(parent->variables, Token*, parent->variables_size, parent->variables_count);
	parent->variables[parent->variables_count++] = token;
}

Token *new_variable(Token *token)
{
	debug(CYAN "new variable [%k] in scope %k\n" RESET, token, scope->token);
	for (int i = 0; i < scope->variables_count; i++)
	{
		Token *curr = scope->variables[i];
		bool cond = (strcmp(curr->name, token->name) == 0);
		check(cond, "Redefinition of %s", token->name);
	}
	add_variable(scope, token);
	return token;
}

Token *get_variable(char *name)
{
	debug(CYAN "get variable [%s] from scope %k\n" RESET, name, scope->token);
	for (int j = sccount; j > 0; j--)
	{
		Node *scope = level_scope[j];
		for (int i = 0; i < scope->variables_count; i++)
			if (strcmp(scope->variables[i]->name, name) == 0)
				return scope->variables[i];
	}
	check(1, "%s not found", name);
	// seg();
	return syntax_error()->token;
}

void add_function(Node *parent, Node *node)
{
	resize_array(parent->functions, Node*, parent->functions_size, parent->functions_count);
	parent->functions[parent->functions_count++] = node;
}

Node *new_function(Node *node)
{
	for (int i = 0; i < scope->functions_count; i++)
	{
		Node *func = scope->functions[i];
		bool cond = strcmp(func->token->name, node->token->name) == 0;
		check(cond, "Redefinition of %s", node->token->name);
	}
	add_function(scope, node);
	return node;
}

Node *get_function(char *name)
{
	for (int j = sccount; j > 0; j--)
	{
		Node *scope = level_scope[j];
		for (int i = 0; i < scope->functions_count; i++)
			if (strcmp(scope->functions[i]->token->name, name) == 0)
				return scope->functions[i];
	}
	check(1, "'%s' Not found", name);
	return syntax_error();
}

Token *copy_token(Token *token)
{
	if (token == NULL) return NULL;
	Token *new = allocate(1, sizeof(Token));
	memcpy(new, token, sizeof(Token));
	new->name = NULL;
	if (token->name) setName(new, token->name);
	if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
	add_token(new);
	return new;
}

// DEBUGGING AND ERROR HANDLING
int vprint_(File out, char *conv, va_list args)
{
	int res = 0;

	for (int i = 0; conv[i]; i++)
	{
		if (conv[i] != '%') { res += fprintf(out, "%c", conv[i]); continue; }

		i++;
		int left_align = 0;
		if (conv[i] == '-') { left_align = 1; i++; }

		int width = 0;
		while (isdigit(conv[i])) { width = width * 10 + (conv[i] - '0'); i++; }

		int prec = -1;
		if (conv[i] == '.')
		{
			i++; prec = 0;
			while (conv[i] >= '0' && conv[i] <= '9') { prec = prec * 10 + (conv[i] - '0'); i++; }
		}

		if      (strncmp(conv + i, "zu",  2) == 0) { res += fprintf(out, "%d",   va_arg(args, int));       i++;   }
		else if (strncmp(conv + i, "lld", 3) == 0) { res += fprintf(out, "%lld", va_arg(args, long long)); i += 2; }
		else
		{
			switch (conv[i])
			{
			case 'c': res += fprintf(out, "%c", va_arg(args, int)); break;
			case 's':
			{
				char *str = va_arg(args, char *);
				if (!str) str = "(null_str)";
				if (left_align)
					res += (prec >= 0) ? fprintf(out, "%-*.*s", width, prec, str) : fprintf(out, "%-*s", width, str);
				else
					res += (prec >= 0) ? fprintf(out, "%*.*s",  width, prec, str) : fprintf(out, "%*s",  width, str);
				break;
			}
			case 'p': res += fprintf(out, "%p",   va_arg(args, void *)); break;
			case 'x': res += (prec >= 0) ? fprintf(out, "%.*x", prec, va_arg(args, unsigned int)) : fprintf(out, "%x", va_arg(args, unsigned int)); break;
			case 'X': res += (prec >= 0) ? fprintf(out, "%.*X", prec, va_arg(args, unsigned int)) : fprintf(out, "%X", va_arg(args, unsigned int)); break;
			case 'd': res += (prec >= 0) ? fprintf(out, "%.*d", prec, va_arg(args, int))          : fprintf(out, "%d", va_arg(args, int));          break;
			case 'f': res += (prec >= 0) ? fprintf(out, "%.*f", prec, va_arg(args, double))       : fprintf(out, "%f", va_arg(args, double));       break;
			case '%': res += fprintf(out, "%%"); break;
			case 't': res += fprintf(out, "%s", to_string((Type)va_arg(args, Type))); break;
			case 'k':
			{
				Token *token = va_arg(args, Token *);
				if (!token) { fprintf(out, "(null)"); break; }

				fprintf(out, "[%s] ", to_string(token->type));

				switch (token->type)
				{
				case VOID: case CHARS: case CHAR: case INT:
				case BOOL: case FLOAT: case LONG:
				{
					if (token->name) { fprintf(out, "%s ", token->name); break; }
					if (token->type == VOID) break;
					// print_value inline
					switch (token->type)
					{
					case INT:   fprintf(out, "[%lld] ", (long long)token->Int.value); break;
					case LONG:  fprintf(out, "[%lld] ", token->Long.value); break;
					case BOOL:  fprintf(out, "[%s] ", token->Bool.value ? "True" : "False"); break;
					case FLOAT: fprintf(out, "[%f] ", token->Float.value); break;
					case CHAR:
					{
						fprintf(out, "[");
						char c = token->Char.value;
						switch (c)
						{
						case '\n': fprintf(out, "\\n");  break;
						case '\t': fprintf(out, "\\t");  break;
						case '\r': fprintf(out, "\\r");  break;
						case '\\': fprintf(out, "\\\\"); break;
						case '\"': fprintf(out, "\\\""); break;
						default:   fprintf(out, "%c", c); break;
						}
						fprintf(out, "] ");
						break;
					}
					case CHARS:
					{
						fprintf(out, "[\"");
						char *str = token->Chars.value;
						if (str)
							for (int j = 0; str[j]; j++)
							{
								switch (str[j])
								{
								case '\n': fprintf(out, "\\n");  break;
								case '\t': fprintf(out, "\\t");  break;
								case '\r': fprintf(out, "\\r");  break;
								case '\\': fprintf(out, "\\\\"); break;
								case '\"': fprintf(out, "\\\""); break;
								default:   fprintf(out, "%c", str[j]); break;
								}
							}
						fprintf(out, "\"] ");
						break;
					}
					case ADD: case SUB: case NOT_EQUAL:
						fprintf(out, "%s ", to_string(token->type)); break;
					default: break;
					}
					break;
				}
				case STRUCT_CALL:
					fprintf(out, "name [%s] ", token->name);
					fprintf(out, "st_name [%s] ", token->Struct.ptr->token->name);
					break;
				case STRUCT_DEF:
				case FCALL: case FDEC: case ID:
					fprintf(out, "%s ", token->name);
					break;
				default: break;
				}

				if (token->is_ref)      fprintf(out, "ref ");
				if (token->ir_bound)    fprintf(out, "bound ");
				if (token->retType)     fprintf(out, "ret [%s] ", to_string(token->retType));
				if (token->is_variadic) fprintf(out, "variadic ");
				break;
			}
			default: todo(1, "invalid format specifier [%c]", conv[i]); break;
			}
		}
	}
	return res;
}

int _debug(char *conv, ...)
{
	va_list args;
	va_start(args, conv);
	int res = vprint_(stdout, conv, args);
	va_end(args);
	return res;
}

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...)
{
	if (!cond) return cond;
	found_error = true;
	fprintf(stderr, BOLD RED "ura_error: %s %s:%d " RESET, funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	vprint_(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void pnode(Node *node, char *indent)
{
	if (!node || !node->token || !DEBUG) return;
	Node **subs     = NULL;
	int count    = 0;
	int capacity = 0;

#define push(n) do { \
		resize_array(subs, Node *, capacity, count); \
		subs[count++] = (n); \
} while(0)

	debug("%k\n", node->token);
	if (includes(node->token->type, IF, ELIF, ELSE, 0))
	{
		if (node->left) push(node->left);
		for (int i = 0; i < node->children_count; i++) push(node->children[i]);
		if (node->right) push(node->right);
	}
	else
	{
		if (node->left)  push(node->left);
		if (node->right) push(node->right);
		for (int i = 0; i < node->children_count; i++) push(node->children[i]);
		if (node->token->type == STRUCT_DEF)
		{
			for (int i = 0; i < node->methods_count; i++)
				push(node->methods[i]);
		}
	}
	for (int i = 0; i < count; i++)
	{
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int is_last   = (i == count - 1);
		const char *connecgittor = is_last ? "└──" : "├──";
		const char *bar       = is_last ? "   " : "│  ";

		char new_indent[4096];
		snprintf(new_indent, sizeof(new_indent), "%s%s", indent, bar);

		debug("%s%s", indent, connector);
		pnode(child, new_indent);
	}
	free(subs);
#undef push
}

// STRING
char *strjoin(char *str0, char *str1, char *str2)
{
	int len0 = str0 ? strlen(str0) : 0;
	int len1 = str1 ? strlen(str1) : 0;
	int len2 = str2 ? strlen(str2) : 0;
	char *res  = allocate(len0 + len1 + len2 + 1, 1);
	if (str0) strcpy(res, str0);
	if (str1) strcpy(res + len0, str1);
	if (str2) strcpy(res + len0 + len1, str2);
	return res;
}

// OTHER UTILITIES
bool is_data_type(Token *token)
{
	return includes(token->type, DATA_TYPES, 0);
}

Node *copy_node(Node *node)
{
	Node *new = allocate(1, sizeof(Node));
	new->token = copy_token(node->token);
	if (node->left) new->left = copy_node(node->left);
	if (node->right) new->right = copy_node(node->right);
	for (int i = 0; i < node->children_count; i++)
		add_child(new, copy_node(node->children[i]));
	for (int i = 0; i < node->structs_count; i++)
		add_struct(new, node->structs[i]);
	for (int i = 0; i < node->variables_count; i++)
		add_variable(new, copy_token(node->variables[i]));
	return new;
}

bool compatible(Token *left, Token *right)
{
	Type lt = left->retType  ? left->retType  : left->type;
	Type rt = right->retType ? right->retType : right->type;

	if (lt == rt) return true;

	if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;

	if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.elem_type == CHAR)
		return true;
	if (rt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && left->Array.elem_type  == CHAR)
		return true;

	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY      && rt == ARRAY_TYPE) return true;

	bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
	bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
	if (lt_numeric && rt_numeric) return true;

	if (lt == VOID || rt == VOID) return false;

	if (lt == STRUCT_CALL && rt == STRUCT_CALL)
		return left->Struct.ptr == right->Struct.ptr;
	return false;
}

Type get_ret_type(Node *node)
{
	if (!node || !node->token) return 0;

	Token *token = node->token;
	Node  *left  = node->left;
	Node  *right = node->right;

	if (token->retType) return token->retType;

	// Scalar types and struct return themselves
	if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
		return token->type;
	if (token->type == FCALL)
		return token->Fcall.ptr ? token->Fcall.ptr->token->retType : 0;
	// Arithmetic, bitwise, and assign ops inherit the left operand type
	if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0))
		return get_ret_type(left);
	// Comparison and logical ops always yield bool
	if (includes(token->type, COMPARISON_OPS, AND, OR, NOT, BNOT, 0))
		return BOOL;
	if (token->type == AS)
		return right ? (right->token->retType ?
		                right->token->retType : right->token->type) : 0;
	if (token->type == RETURN) return get_ret_type(left);
	if (token->type == DOT)    return get_ret_type(right);
	if (token->type == ACCESS)
	{
		Type left_type = get_ret_type(left);
		if (left_type == CHARS || left_type == STACK) return CHAR;
		return left_type;
	}
	if (token->type == ID) return token->type != ID ? token->type : 0;
	todo(1, "handled this case [%s]", to_string(token->type));
	return 0;
}

void set_ret_type(Node *node)
{
	if (!node || !node->token) return;
	Type t = get_ret_type(node);
	if (t) node->token->retType = t;

	if (node->token->type == DOT &&
	    node->token->retType == STRUCT_CALL && node->right)
		node->token->Struct.ptr = node->right->token->Struct.ptr;

	// For FCALL returning struct, copy Struct.ptr from the function declaration
	if (node->token->type == FCALL &&
	    node->token->retType == STRUCT_CALL &&
	    node->token->Fcall.ptr)
		node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}

// ----------------------------------------------------------------------------
// LLVM higher-level helpers (from llvm.c)
// ----------------------------------------------------------------------------
Value allocate_stack(Value size, TypeRef elementType, char *name)
{
	Value indices[] =
	{
		LLVMConstInt(i32, 0, 0),
		LLVMConstInt(i32, 0, 0)
	};

	if (LLVMIsConstant(size))
	{
		unsigned long long constSize    = LLVMConstIntGetZExtValue(size);
		TypeRef arrayType    = LLVMArrayType(elementType, constSize);
		Value array_alloca = LLVMBuildAlloca(builder, arrayType, name);
		return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
	}

	Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
	return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef elementType, char *name)
{
	Value calloc_func = _get_named_function("calloc");
	if (!calloc_func)
	{
		TypeRef params[]  = {i64, i64};
		TypeRef func_type = _function_type(p8, params, 2, 0);
		calloc_func = _add_function("calloc", func_type);
	}
	TargetData td        = _get_module_data_layout(module);
	size_t elem_size = _abi_size_of_type(td, elementType);

	Value count_i64;
	unsigned width = LLVMGetIntTypeWidth(LLVMTypeOf(count));
	if (width < 64)
		count_i64 = LLVMBuildZExt(builder, count, i64, "count");
	else if (width > 64)
		count_i64 = LLVMBuildTrunc(builder, count, i64, "count");
	else
		count_i64 = count;

	Value size_i64    = _const_int(i64, elem_size, 0);
	Value args[]      = {count_i64, size_i64};
	TypeRef calloc_type = _global_get_value_type(calloc_func);
	return _build_call2(calloc_type, calloc_func, args, 2, name);
}


void hoist_allocas(Node *node)
{
	if (!node) return;
	Token *tok = node->token;

	// don't recurse into nested functions
	if (tok->type == FDEC) return;

	if (includes(tok->type, INT, LONG, SHORT, CHARS, CHAR, BOOL, ARRAY_TYPE, 0) && tok->is_dec)
	{
		if (!tok->llvm.elem)
			_alloca(tok);
	}
	else if (tok->type == STRUCT_CALL && tok->is_dec && !tok->is_ref)
	{
		if (!tok->llvm.elem)
		{
			TypeRef struct_type = get_llvm_type(tok);
			tok->llvm.elem = _build_alloca(struct_type, tok->name);
		}
	}

	if (node->left)  hoist_allocas(node->left);
	if (node->right) hoist_allocas(node->right);
	for (int i = 0; i < node->children_count; i++)
		hoist_allocas(node->children[i]);
}


void load_if_necessary(Node *node)
{
	Token *token = node->token;

	if (token->is_ref)
		return;
	if (includes(token->type, MATH_TYPE, 0))
		return;
	if (includes(token->type, DATA_TYPES, 0) && !token->name)
		return;
	if (token->llvm.is_loaded)
		return;
	if (includes(token->type, STACK, HEAP, FCALL, 0))
		return;

	if (token->name || includes(token->type, ACCESS, DOT, 0))
	{
		Token *new = copy_token(token);
		new->llvm.elem      = load_value(token);
		new->llvm.is_loaded = true;
		node->token         = new;
	}
}

void _alloca(Token *token)
{
	TypeRef type = get_llvm_type(token);
	if (token->is_ref)
		type = _pointer_type(type, 0);

	Block current = _get_insert_block();
	Value func    = _get_basic_block_parent(current);
	Block entry   = _get_entry_basic_block(func);
	// Walk past leading allocas to find the insertion point
	Value inst    = LLVMGetFirstInstruction(entry);
	while (inst && LLVMGetInstructionOpcode(inst) == LLVMAlloca)
		inst = LLVMGetNextInstruction(inst);

	// Position before the first non-alloca instruction (safe even when it's a terminator)
	if (inst)
		LLVMPositionBuilderBefore(builder, inst);
	else
		_position_at(entry);

	token->llvm.elem = _build_alloca(type, token->name);

	_position_at(current);
}

TypeRef get_llvm_type(Token *token)
{
	Type type = token->type;
	if (token->retType)
		type = token->retType;
	if (type == STRUCT_DEF)
		return token->llvm.stType;
	if (type == TUPLE)
		return token->llvm.stType;
	if (type == STRUCT_CALL)
	{
		if (check(!token->Struct.ptr, "STRUCT_CALL: Struct.ptr is NULL for token '%s' type=%d retType=%d",
		          token->name ? token->name : "(null)", token->type, token->retType))
			return LLVMVoidTypeInContext(context);
		return get_llvm_type(token->Struct.ptr->token);
	}
	if (includes(type, ARRAY, ARRAY_TYPE, 0))
	{
		TypeRef base;
		if (token->Array.elem_type == STRUCT_CALL && token->Array.struct_ptr)
			base = get_llvm_type(token->Array.struct_ptr->token);
		else
		{
			Token tmp = {.type = token->Array.elem_type};
			base = get_llvm_type(&tmp);
		}
		return _pointer_type(base, 0); // flat allocation: always single ptr to base
	}
	// if (type == FCALL)
	//    return get_llvm_type(token->Fcall.ptr->token);
	TypeRef res[END] = {
		[INT]    = i32,
		[CHAR]   = i8,
		[CHARS]  = p8,
		[BOOL]   = i1,
		[VOID]   = vd,
		[LONG]   = i64,
		[FLOAT]  = f32,
		[ACCESS] = i8,
	};
	if(check(!res[type], "handle this case [%s]", to_string(type)))
	{
		seg();
	}
	return res[type];
}

Value _get_default_value(Token *token)
{
	TypeRef type = get_llvm_type(token);

	if (token->is_ref)
		return LLVMConstNull(_pointer_type(type, 0));

	if (includes(token->type, NUMERIC_TYPES, 0))
		return LLVMConstInt(type, 0, false);
	if (token->type == FLOAT)
		return LLVMConstReal(type, 0.0);
	if (includes(token->type, CHARS, ARRAY_TYPE, 0))
		return LLVMConstNull(type);
	check(1, "handle this case %s", to_string(token->type));
	return NULL;
}

void _const_value(Token *token)
{
	TypeRef type  = get_llvm_type(token);
	long long value = 0;

	switch (token->type)
	{
	case INT:  value = (long long)token->Int.value; break;
	case BOOL: value = (long long)token->Bool.value; break;
	case CHAR: value = (int)token->Char.value; break;
	case FLOAT:
	{
		token->llvm.elem      = LLVMConstReal(type, (double)token->Float.value);
		token->llvm.is_loaded = true;
		return;
	}
	case CHARS:
	{
		char name[200];
		char      *processed;
		static int index = 0;
		snprintf(name, sizeof(name), "STR%d", index++);
		processed = allocate(strlen(token->Chars.value) * 2 + 1, 1);
		int j = 0;
		for (int i = 0; token->Chars.value[i]; i++)
		{
			if (token->Chars.value[i] == '\\' && token->Chars.value[i + 1])
			{
				switch (token->Chars.value[i + 1])
				{
				case 'n': processed[j++]  = '\n'; i++; break;
				case 't': processed[j++]  = '\t'; i++; break;
				case 'r': processed[j++]  = '\r'; i++; break;
				case '0': processed[j++]  = '\0'; i++; break;
				case '\\': processed[j++] = '\\'; i++; break;
				case '\"': processed[j++] = '\"'; i++; break;
				case '\'': processed[j++] = '\''; i++; break;
				default: processed[j++]   = token->Chars.value[i]; break;
				}
			}
			else
			{
				processed[j++] = token->Chars.value[i];
			}
		}
		token->llvm.elem = _const_chars(processed, name);
		free(processed);
		return;
	}
	default: check(1, "handle this case %s", to_string(token->type)); return;
	}
	token->llvm.elem = _const_int(type, value, 0);
}

Value _build_return(Token *token)
{
	if (!_get_basic_block_terminator(_get_insert_block()))
	{
		Value value = token->llvm.elem;
		if (token->type != VOID)
		{
			debug("build return %k\n", token);
			return LLVMBuildRet(builder, value);
		}
		else
			return LLVMBuildRetVoid(builder);
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// ASM GENERATION
// ----------------------------------------------------------------------------
void init(char *name)
{
	context = LLVMContextCreate();
	module  = LLVMModuleCreateWithNameInContext(name, context);
	builder = LLVMCreateBuilderInContext(context);


	vd  = LLVMVoidTypeInContext(context);
	f32 = LLVMFloatTypeInContext(context);
	i1  = LLVMInt1TypeInContext(context);
	i8  = LLVMInt8TypeInContext(context);
	i16 = LLVMInt16TypeInContext(context);
	i32 = LLVMInt32TypeInContext(context);
	i64 = LLVMInt64TypeInContext(context);
	p8  = _pointer_type(i8, 0);
	p32 = _pointer_type(i32, 0);

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();
	LLVMInitializeNativeAsmParser();
   #if defined(__APPLE__)
	LLVMSetTarget(module, "arm64-apple-macosx16.0.0");
   #elif defined(__linux__)
	LLVMSetTarget(module, "x86_64-pc-linux-gnu");
   #else
	LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
   #endif

	// if (enable_asan)
	// {
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Debug Info Version",
	                  18, LLVMValueAsMetadata(LLVMConstInt(i32, 3, 0)));
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Dwarf Version",
	                  13, LLVMValueAsMetadata(LLVMConstInt(i32, 4, 0)));
	// }

	// Debug info
	debug_builder = LLVMCreateDIBuilder(module);
	char *base          = strrchr(name, '/');
	char *filename_only = base ? base + 1 : name;
	char dir[1024]     = ".";
	if (base) { size_t len = base - name; strncpy(dir, name, len); dir[len] = '\0'; }

	debug_file = LLVMDIBuilderCreateFile(
		debug_builder, filename_only,
		strlen(filename_only), dir, strlen(dir));

	debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
		debug_builder, LLVMDWARFSourceLanguageC,
		debug_file, "ura", 3, 0, "", 0, 0, "", 0,
		LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);

	debug_scope = debug_compile_unit;

	// if (using_refs) getRefAssignFunc();
	// if (enable_bounds_check) getNullCheckFunc();
}

void finalize(char *output)
{
	char *error = NULL;

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

	LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

	if (op_flags)
	{
		LLVMErrorRef err = LLVMRunPasses(module, op_flags, NULL, options);
		if (err)
		{
			char *msg = LLVMGetErrorMessage(err);
			fprintf(stderr, RED "Optimizer Error: %s\n" RESET, msg);
			LLVMDisposeErrorMessage(msg);
			found_error = true;
			return;
		}
	}

	LLVMDIBuilderFinalize(debug_builder);
	LLVMDisposeDIBuilder(debug_builder);
	debug_builder = NULL;

	if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error))
	{
		fprintf(stderr, "Module verification failed:\n%s\n", error);
		LLVMDisposeMessage(error);
		LLVMDisposePassBuilderOptions(options);
		found_error = true;
		// return;
	}
	else
	{
		LLVMDisposePassBuilderOptions(options);

	}

	LLVMPrintModuleToFile(module, output, NULL);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);
}

void emit_scope_clean(Node *scope, int from)
{
	for (int i = from; i < scope->variables_count; i++)
	{
		Token *var = scope->variables[i];
		if (var->type != STRUCT_CALL || !var->llvm.elem) continue;
		if (var->is_ref) continue; // refs are borrowed, not owned — caller cleans up
		Node *sd       = var->Struct.ptr;
		char *qname    = strjoin(sd->token->name, ".clean", NULL);
		Value clean_fn = _get_named_function(qname);
		free(qname);
		if (!clean_fn) continue;
		Value args[] = { var->llvm.elem };
		_build_call2(_global_get_value_type(clean_fn), clean_fn, args, 1, "");

	}
}

void append_output_arg(Token *tok, char *fmt, int *functions_count, Value *args, int *nargs)
{
	Type type = tok->type ? tok->type : tok->retType;

	// String literal: embed directly in the format string (escape % → %%)
	if (tok->type == CHARS && !tok->name)
	{
		const char *s = tok->Chars.value;
		for (int i = 0; s[i]; i++)
		{
			if (s[i] == '%')
			{
				fmt[(*functions_count)++] = '%'; // double % for printf
				fmt[(*functions_count)++] = '%';
			}
			else if (s[i] == '\\' && s[i + 1])
			{
				switch (s[++i])
				{
				case 'n':  fmt[(*functions_count)++] = '\n'; break;
				case 't':  fmt[(*functions_count)++] = '\t'; break;
				case 'r':  fmt[(*functions_count)++] = '\r'; break;
				case '\\': fmt[(*functions_count)++] = '\\'; break;
				case '"':  fmt[(*functions_count)++] = '"';  break;
				default:   fmt[(*functions_count)++] = '\\'; fmt[(*functions_count)++] = s[i]; break;
				}
			}
			else
				fmt[(*functions_count)++] = s[i];
		}
		return;
	}

	// For expression nodes (DOT, FCALL, etc.), resolve to actual value type
	switch (type)
	{
	case INT: case LONG: case SHORT: case CHAR: case CHARS:
	case BOOL: case FLOAT: case STRUCT_CALL:
		break;
	case FCALL:
		type = tok->retType;
		break;
	default:
		if (tok->retType) type = tok->retType;
		break;
	}

	switch (type)
	{
	case INT: case SHORT:
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = load_value(tok);
		break;
	case BOOL:
	{
		Value bool_val  = load_value(tok);
		Value true_str  = _const_chars("True",  "true_str");
		Value false_str = _const_chars("False", "false_str");
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 's';
		args[(*nargs)++]          = LLVMBuildSelect(builder, bool_val, true_str, false_str, "bool_str");
		break;
	}
	case LONG:
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 'l';
		fmt[(*functions_count)++] = 'l'; fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = load_value(tok);
		break;
	case CHAR:
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 'c';
		args[(*nargs)++]          = load_value(tok);
		break;
	case CHARS:
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 's';
		args[(*nargs)++]          = load_value(tok);
		break;
	case FLOAT:
		fmt[(*functions_count)++] = '%'; fmt[(*functions_count)++] = 'f';
		args[(*nargs)++]          = LLVMBuildFPExt(builder, load_value(tok), LLVMDoubleTypeInContext(context), "f2d");
		break;
	case STRUCT_CALL:
	{
		fmt[(*functions_count)++] = '{';
		fmt[(*functions_count)++] = ' ';
		Node *sd = tok->Struct.ptr;
		for (int i = 0; i < sd->children_count; i++)
		{
			Token *field          = sd->children[i]->token;
			int functions_size = strlen(field->name);
			memcpy(fmt + *functions_count, field->name, functions_size); *functions_count += functions_size;
			fmt[(*functions_count)++] = ':'; fmt[(*functions_count)++] = ' ';
			Token ftok = *field;
			ftok.llvm.elem            = struct_field_ptr(tok, i, field->name);
			append_output_arg(&ftok, fmt, functions_count, args, nargs);
			if (i < sd->children_count - 1) { fmt[(*functions_count)++] = ','; fmt[(*functions_count)++] = ' '; }
		}
		fmt[(*functions_count)++] = ' ';
		fmt[(*functions_count)++] = '}';
		break;
	}
	default:
		fmt[(*functions_count)++] = '?';
		break;
	}
}

void gen_asm(Node *node)
{
	set__debuglocation(node->token);
	Node *left  = node->left;
	Node *right = node->right;

	if (check(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type)
	{
	case STRUCT_CALL:
	{
		if (node->token->is_dec)
		{
			if (node->token->is_global)
			{
				if (node->token->used == 0) { node->token->is_dec = false; return; }
				TypeRef type = get_llvm_type(node->token);
				node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
				LLVMSetInitializer(node->token->llvm.elem, LLVMConstNull(type));
			}
			else if (node->token->is_ref)
			{
				// TypeRef type = get_llvm_type(node->token);
				// Value   null = LLVMConstNull(LLVMPointerType(type, 0));
				// _build_store(null, node->token->llvm.elem);
			}
			else
			{
				_alloca(node->token);
				TypeRef struct_type = get_llvm_type(node->token);
				_build_store(LLVMConstNull(struct_type), node->token->llvm.elem);

				Node *src     = node->token->Struct.ptr;
				char *qname   = strjoin(src->token->name, ".init", NULL);
				Value init_fn = _get_named_function(qname);
				free(qname);
				if (init_fn)
				{
					Value args[] = { node->token->llvm.elem };
					_build_call2(_global_get_value_type(init_fn), init_fn, args, 1, "");
				}
			}
			node->token->is_dec = false;
			return;
		}
		break;
	}
	case NULLABLE:
	{
		node->token->llvm.elem = LLVMConstNull(p8);
		break;
	}
	case INT: case LONG: case SHORT: case CHARS:
	case CHAR: case BOOL: case ARRAY_TYPE: case FLOAT:
	{
		if (node->token->is_dec)
		{
			if (node->token->is_global)
			{
				if (node->token->used == 0) { node->token->is_dec = false; return; }
				TypeRef type = get_llvm_type(node->token);
				node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
				LLVMSetInitializer(node->token->llvm.elem, _get_default_value(node->token));
			}
			else
			{
				_alloca(node->token);
				Value defval = _get_default_value(node->token);
				_build_store(defval, node->token->llvm.elem);
			}
			node->token->is_dec = false;
			return;
		}
		else if (node->token->name)
			return;
		_const_value(node->token);
		break;
	}
	case ASSIGN:
	{
		if (left->token->is_global && left->token->is_dec)
		{
			gen_asm(left);
			gen_asm(right);
			if (left->token->llvm.elem && right->token->llvm.elem && LLVMIsConstant(right->token->llvm.elem))
				LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
			return;
		}
		if (left->token->is_ref && !left->token->asm_bound && !left->token->is_param)
		{
			if (!left->token->llvm.elem)
				_alloca(left->token);
			gen_asm(right);
			_build_store(right->token->llvm.elem, left->token->llvm.elem);
			left->token->asm_bound = true;
			left->token->is_dec    = false;
			break;
		}

		gen_asm(left);
		gen_asm(right);

		// propagate multi-dim info from stack/heap allocation to the variable
		if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dim_count > 1)
		{
			left->token->llvm.dim_count = right->token->llvm.dim_count;
			for (int i = 0; i < right->token->llvm.dim_count; i++)
				left->token->llvm.dim_sizes[i] = right->token->llvm.dim_sizes[i];
		}

		TypeRef type = get_llvm_type(right->token);

		Value val;
		if (right->token->is_ref)
			val = load_value(right->token);
		else
		{
			load_if_necessary(right);
			val = right->token->llvm.elem;
		}

		if (left->token->is_ref)
		{
			Value ptr = LLVMBuildLoad2(builder, _pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
			_build_store(val, ptr);
		}
		else
			_build_store(val, left->token->llvm.elem);

		break;
	}
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
	{
		gen_asm(left);
		gen_asm(right);

		TypeRef type        = get_llvm_type(left->token);
		Value current_val = load_value(left->token);

		Value right_val;
		if (right->token->is_ref)
			right_val = load_value(right->token);
		else
		{
			load_if_necessary(right);
			right_val = right->token->llvm.elem;
		}

		Value result = build_binary_op(assign_base_op(node->token->type), current_val, right_val);

		if (left->token->is_ref)
		{
			Value ptr = LLVMBuildLoad2(builder, _pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
			_build_store(result, ptr);
		}
		else
			_build_store(result, left->token->llvm.elem);
		break;
	}
	case NOT: case BNOT:
	{
		gen_asm(left);
		load_if_necessary(left);
		if (left->token->is_ref)
		{
			left->token->llvm.elem      = load_value(left->token);
			left->token->llvm.is_loaded = true;
		}
		node->token->llvm.elem = _build_not(left->token, to_string(node->token->type));
		break;
	}
	case ADD: case SUB: case MUL: case DIV: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
	case GREAT_EQUAL: case MOD: case AND: case OR:
	case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
	{
		gen_asm(left);
		gen_asm(right);

		load_if_necessary(left);
		load_if_necessary(right);

		Value lref = left->token->llvm.elem;
		Value rref = right->token->llvm.elem;

		if (left->token->is_ref && !left->token->llvm.is_loaded)
			lref = load_value(left->token);
		if (right->token->is_ref && !right->token->llvm.is_loaded)
			rref = load_value(right->token);

		node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
		if (includes(node->token->type, COMPARISON_OPS, 0))
			node->token->retType = BOOL;
		else
			node->token->retType = left->token->retType ? left->token->retType : left->token->type;
		break;
	}
	case STACK:
	{
		Token tmp    = {.type = node->token->Array.elem_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int depth  = node->token->Array.depth;

		// evaluate each dimension, compute total element count, store dim_sizes
		Value total = _const_int(i32, 1, 0);
		for (int i = 0; i < depth; i++)
		{
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			node->token->llvm.dim_sizes[i] = dv;
			total = _build_mul(total, dv, "dim");
		}
		node->token->llvm.dim_count = depth;

		TargetData td          = _get_module_data_layout(module);
		size_t elem_size   = _abi_size_of_type(td, elem_t);
		Value total_bytes = _build_mul(total, _const_int(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_stack(total_bytes, elem_t, "stack");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case HEAP:
	{
		Token tmp    = {.type = node->token->Array.elem_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int depth  = node->token->Array.depth;

		Value total  = _const_int(i32, 1, 0);
		for (int i = 0; i < depth; i++)
		{
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			node->token->llvm.dim_sizes[i] = dv;
			total = _build_mul(total, dv, "dim");
		}
		node->token->llvm.dim_count = depth;

		TargetData td          = _get_module_data_layout(module);
		size_t elem_size   = _abi_size_of_type(td, elem_t);
		Value total_bytes = _build_mul(total, _const_int(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_heap(total_bytes, elem_t, "heap");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case FCALL:
	{
		LLVM srcFunc     = node->token->Fcall.ptr->token->llvm;
		bool is_variadic = node->token->Fcall.ptr->token->is_variadic;
		int count       = node->left->children_count;
		Node **argNodes    = node->left->children;
		Node  *dec_args    = node->token->Fcall.ptr->left;
		Value *args        = NULL;

		if (count)
		{
			args = allocate(count + 3, sizeof(Value));

			int fixed_params = is_variadic ? node->token->Fcall.ptr->left->children_count : count;

			// handle fixed params (both variadic and non-variadic)
			for (int i = 0; i < fixed_params; i++)
			{
				gen_asm(argNodes[i]);
				bool param_is_ref = (i < dec_args->children_count) && dec_args->children[i]->token->is_ref;
				bool arg_is_ref   = argNodes[i]->token->is_ref;

				if (param_is_ref && arg_is_ref)
				{
					// ref → ref: load the pointer out of the ref's slot and pass it
					TypeRef type = get_llvm_type(argNodes[i]->token);
					args[i] = LLVMBuildLoad2(builder, _pointer_type(type, 0), argNodes[i]->token->llvm.elem, "ref_arg");
				}
				else if (param_is_ref && !arg_is_ref)
				{
					// non-ref → ref param: pass the variable's alloca directly
					args[i] = argNodes[i]->token->llvm.elem;
				}
				else if (!param_is_ref && arg_is_ref)
				{
					// ref → non-ref param: double dereference to get the value
					args[i] = load_value(argNodes[i]->token);
				}
				else
				{
					// non-ref → non-ref: normal load
					load_if_necessary(argNodes[i]);
					args[i] = argNodes[i]->token->llvm.elem;

					// ARM64 ABI: proto calls pass small structs as i64 via memcpy
					bool is_proto_call   = node->token->Fcall.ptr->token->is_proto;
					bool param_is_struct = (i < dec_args->children_count)
					                       && includes(dec_args->children[i]->token->type, STRUCT_CALL, STRUCT_DEF, 0);
					if (is_proto_call && param_is_struct)
					{
						TypeRef st_type = get_llvm_type(dec_args->children[i]->token);
						Value st_ptr  = _build_alloca(st_type, "st_slot");
						_build_store(argNodes[i]->token->llvm.elem, st_ptr);
						Value i64p    = _build_alloca(i64, "i64_slot");
						_build_memcpy(i64p, st_ptr, _const_int(i64, 4, 0));
						args[i] = LLVMBuildLoad2(builder, i64, i64p, "i64_arg");
					}
				}
			}

			if (is_variadic)
			{
				bool is_proto_call  = node->token->Fcall.ptr->token->is_proto;
				int variadic_count = count - fixed_params;
				if (!is_proto_call)
				{
					// Ura-style: insert count before variadic args
					args[fixed_params] = _const_int(i32, variadic_count, 0);
					for (int i = fixed_params; i < count; i++)
					{
						gen_asm(argNodes[i]);
						load_if_necessary(argNodes[i]);
						args[i + 1] = argNodes[i]->token->llvm.elem;
					}
					count++;
				}
				else
				{
					// C-style: pass variadic args directly, no count
					for (int i = fixed_params; i < count; i++)
					{
						gen_asm(argNodes[i]);
						load_if_necessary(argNodes[i]);
						args[i] = argNodes[i]->token->llvm.elem;
					}
				}
			}
		}

		char   *name     = node->token->Fcall.ptr->token->retType != VOID ? node->token->name : "";
		TypeRef funcType = srcFunc.funcType;
		Value elem     = srcFunc.elem;
		if (check(!srcFunc.funcType, "FCALL: funcType is NULL for '%s'", node->token->name))
			break;
		if (check(!srcFunc.elem, "FCALL: elem is NULL for '%s'", node->token->name))
			break;
		node->token->llvm.elem = LLVMBuildCall2(builder, funcType, elem, args, count, name);
		free(args);

		// ARM64 ABI: proto returning struct actually returns i64; convert back to struct
		bool proto_returns_struct = node->token->Fcall.ptr->token->is_proto
		                            && node->token->Fcall.ptr->token->retType == STRUCT_CALL;
		if (proto_returns_struct)
		{
			Value i64_ret = node->token->llvm.elem;
			TypeRef st_type = get_llvm_type(node->token);
			Value i64p    = _build_alloca(i64, "ret_i64");
			_build_store(i64_ret, i64p);
			Value st_ptr  = _build_alloca(st_type, "ret_struct");
			_build_memcpy(st_ptr, i64p, _const_int(i64, 4, 0));
			node->token->llvm.elem = LLVMBuildLoad2(builder, st_type, st_ptr, "ret_struct_val");
		}
		break;
	}
	case FDEC:
	{
		// debug("================================\n");
		// pnode(node, "");
		// debug("\n================================\n");
		enter_scope(node);
		TypeRef retType;
		if (node->token->retType == TUPLE)
		{
			int n  = node->token->Tuple.count;
			TypeRef *ft = allocate(n, sizeof(TypeRef));
			for (int i = 0; i < n; i++)
				ft[i] = get_llvm_type(node->token->Tuple.types[i]);
			retType = LLVMStructTypeInContext(context, ft, n, 0);
			node->token->llvm.stType = retType;
			free(ft);
		}
		else if (node->token->is_proto && node->token->retType == STRUCT_CALL)
			retType = i64;
		else if (node->token->retType == STRUCT_CALL && node->token->is_ref)
			retType = _pointer_type(get_llvm_type(node->token), 0);
		else
			retType = get_llvm_type(node->token);

		TypeRef *paramTypes  = NULL;
		int param_count = node->left->children_count;
		int fixed_count = param_count;
		int _count      = param_count;

		// Proto variadic functions (C interop) don't use the Ura count slot
		if (node->token->is_variadic && !node->token->is_proto)
			_count = fixed_count + 1;

		if (param_count)
		{
			paramTypes = allocate(_count + 2, sizeof(TypeRef));

			for (int i = 0; i < fixed_count; i++)
			{
				Token *param = node->left->children[i]->token;
				if (node->token->is_proto && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0))
					paramTypes[i] = i64;
				else if (param->is_ref) paramTypes[i] = _pointer_type(get_llvm_type(param), 0);
				else paramTypes[i] = get_llvm_type(param);
			}

			if (node->token->is_variadic && !node->token->is_proto)
				paramTypes[fixed_count] = i32;
		}

		char   *fname = node->token->name;
		TypeRef funcType;
		if (strcmp(fname, "main") == 0)
		{
			// Only use (i32, ptr) signature if os ura_scope is actually accessed
			bool os_used = false;
			for (int i = 0; i < ura_scope->children_count; i++)
			{
				Node *child = ura_scope->children[i];
				if (child->token->type == STRUCT_CALL && child->token->is_global
				    && strcmp(child->token->name, "os") == 0 && child->token->used > 0)
				{ os_used = true; break; }
			}
			if (os_used)
			{
				TypeRef argv_type  = _pointer_type(_pointer_type(i8, 0), 0);// char**
				TypeRef main_pts[] = { i32, argv_type };
				funcType = _function_type(retType, main_pts, 2, false);
			}
			else
				funcType = _function_type(retType, NULL, 0, false);
		}
		else
			funcType = _function_type(retType, paramTypes, _count, node->token->is_variadic);
		Value existingFunc = _get_named_function(fname);
		if (existingFunc) node->token->llvm.elem = existingFunc;
		else node->token->llvm.elem = _add_function(fname, funcType);
		node->token->llvm.funcType = funcType;


		if (!node->token->is_proto)
		{
			if (enable_asan)
				LLVMAddAttributeAtIndex(
					node->token->llvm.elem,
					LLVMAttributeFunctionIndex,
					LLVMCreateEnumAttribute(
						context,
						LLVMGetEnumAttributeKindForName(
							"sanitize_address", 16),
						0
						));

			// Debug info for this function
			LLVMMetadataRef di_func_type = LLVMDIBuilderCreateSubroutineType(
				debug_builder, debug_file, NULL, 0, LLVMDIFlagZero);

			LLVMMetadataRef di_func = LLVMDIBuilderCreateFunction(
				debug_builder,
				debug_compile_unit,
				fname, strlen(fname),
				fname, strlen(fname),
				debug_file,
				node->token->line,
				di_func_type,
				0,
				1,
				node->token->line,
				LLVMDIFlagZero,
				0
				);
			LLVMSetSubprogram(node->token->llvm.elem, di_func);
			debug_scope = di_func;
			_entry(node->token);

			// set a valid location for param allocas
			LLVMMetadataRef entry_loc = LLVMDIBuilderCreateDebugLocation(
				context, node->token->line, 0, di_func, NULL);
			LLVMSetCurrentDebugLocation2(builder, entry_loc);

			int param_idx = 0;
			for (int i = 0; i < fixed_count; i++)
			{
				Token *param_token = node->left->children[i]->token;
				Value param       = LLVMGetParam(node->token->llvm.elem, param_idx++);
				LLVMSetValueName(param, param_token->name);
				_alloca(param_token);
				param_token->is_dec = false;
				LLVMBuildStore(builder, param, param_token->llvm.elem);
			}

			// Auto-insert nested struct field init calls before user body
			{
				int functions_size = strlen(fname);
				bool is_init_fn     = functions_size > 5 && strcmp(fname + functions_size - 5, ".init") == 0;
				if (is_init_fn && fixed_count >= 1)
				{
					Token *self_tok = node->left->children[0]->token;
					if (self_tok->type == STRUCT_CALL && self_tok->is_ref)
					{
						Node   *sd       = self_tok->Struct.ptr;
						TypeRef st_type  = sd->token->llvm.stType;
						TypeRef ptr_type = _pointer_type(st_type, 0);
						Value self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
						for (int i = 0; i < sd->children_count; i++)
						{
							Token *field = sd->children[i]->token;
							if (field->type != STRUCT_CALL) continue;
							Node  *fsd   = field->Struct.ptr;
							char  *qname = strjoin(fsd->token->name, ".init", NULL);
							Value ffn   = _get_named_function(qname);
							free(qname);
							if (!ffn) continue;
							Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
							Value field_ptr = _build_gep2(st_type, self_ptr, indices, 2, field->name);
							Value args[]    = { field_ptr };
							_build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
						}
					}
				}
			}

			// Inject ura_scope struct init calls at the start of main
			if (strcmp(fname, "main") == 0)
			{
				for (int i = 0; i < ura_scope->children_count; i++)
				{
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0) continue;
					char *qname  = strjoin(child->token->Struct.ptr->token->name, ".init", NULL);
					Value init_f = _get_named_function(qname);
					free(qname);
					if (!init_f) continue;
					Value args[] = { child->token->llvm.elem };
					_build_call2(_global_get_value_type(init_f), init_f, args, 1, "");
				}
				// Populate os.argc and os.argv from main params (only if os is used)
				for (int i = 0; i < ura_scope->children_count; i++)
				{
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0) continue;
					if (strcmp(child->token->name, "os") != 0) continue;
					Value argc_val = LLVMGetParam(node->token->llvm.elem, 0);
					Value argv_val = LLVMGetParam(node->token->llvm.elem, 1);
					Value argc_ptr = struct_field_ptr(child->token, 0, "os.argc");
					_build_store(argc_val, argc_ptr);
					Value argv_ptr = struct_field_ptr(child->token, 1, "os.argv");
					_build_store(argv_val, argv_ptr);
					break;
				}
			}

	 #if USING_HOIST
			for (int i = 0; i < node->children_count; i++)
				hoist_allocas(node->children[i]);
	 #endif

			for (int i = 0; i < node->children_count; i++)
			{
				gen_asm(node->children[i]);
				if (_get_basic_block_terminator(_get_insert_block()))
					break;
			}

		}
		exit_scope();
		debug_scope = debug_compile_unit;
		break;
	}
	case TUPLE_UNPACK:
	{
		// Evaluate RHS (function call returning a tuple/aggregate)
		gen_asm(node->left);
		Value tuple_val = node->left->token->llvm.elem;
		// Alloca each LHS variable and extract the corresponding field
		for (int i = 0; i < node->children_count; i++)
		{
			Node *lhs   = node->children[i];
			gen_asm(lhs); // handles is_dec → _alloca, sets is_dec = false
			Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
			_build_store(field, lhs->token->llvm.elem);
		}
		break;
	}
	case WHILE:
	{
		enter_scope(node);
		Block start = _append_block("while.start");
		Block then  = _append_block("while.then");
		Block end   = _append_block("while.end");

		// Store blocks for break/continue
		node->token->llvm.start = start;
		node->token->llvm.then  = then;
		node->token->llvm.end   = end;

		_branch(start);

		_position_at(start);
		gen_asm(node->left); // condition
		load_if_necessary(node->left);
		_condition(node->left->token->llvm.elem, then, end);

		_position_at(then);
		for (int i = 0; i < node->children_count; i++)
		{
			if (_get_basic_block_terminator(_get_insert_block()))
				break;
			gen_asm(node->children[i]);
		}
		_branch(start);

		_position_at(end);
		exit_scope();
		break;
	}
	case IF:
	{
		enter_scope(node);

		Block if_start = _append_block("if.start");
		Block end      = _append_block("if.end");

		// Store blocks for potential break/continue
		node->token->llvm.start = if_start;
		node->token->llvm.end   = end;

		_branch(if_start);

		Node *curr = node;

		while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
		{
			if (includes(curr->token->type, IF, ELIF, 0))
			{
				Block start;
				Block then;
				Block next;

				if (curr->token->type == IF)
				{
					start = if_start;
					then  = _append_block("if.then");
				}
				else
				{
					start = curr->token->llvm.bloc;
					then  = _append_block("elif.then");
				}

				// Store then block
				curr->token->llvm.then = then;

				if (curr->right)
				{
					if (curr->right->token->type == ELSE) next = _append_block("if.else");
					else next = _append_block("elif.start");
				}
				else next = end;

				_position_at(start);
				gen_asm(curr->left); // condition
				load_if_necessary(curr->left);
				_condition(curr->left->token->llvm.elem, then, next);

				_position_at(then);
				for (int i = 0; i < curr->children_count; i++)
				{
					gen_asm(curr->children[i]);
					if (_get_basic_block_terminator(_get_insert_block()))
						break;
				}
				_branch(end);

				if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
					curr->right->token->llvm.bloc = next;
			}
			else if (curr->token->type == ELSE)
			{
				_position_at(curr->token->llvm.bloc);
				for (int i = 0; i < curr->children_count; i++)
				{
					gen_asm(curr->children[i]);
					if (_get_basic_block_terminator(_get_insert_block()))
						break;
				}
				_branch(end);
			}
			curr = curr->right;
		}
		_position_at(end);
		exit_scope();
		break;
	}
	case BREAK:
	{
		for (int i = sccount; i > 0; i--)
		{
			if (level_scope[i]->token->type == WHILE)
			{
				_branch(level_scope[i]->token->llvm.end);
				return;
			}
		}
		check(1, "break outside loop");
		break;
	}
	case CONTINUE:
	{
		for (int i = sccount; i > 0; i--)
		{
			if (level_scope[i]->token->type == WHILE)
			{
				_branch(level_scope[i]->token->llvm.start);
				return;
			}
		}
		check(1, "continue outside loop");
		break;
	}
	case RETURN:
	{
		emit_scope_clean(scope, 0);

		// Auto-insert nested field clean calls for .clean methods before return
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC)
				{ fdec = level_scope[i]; break; }
			if (fdec)
			{
				char *fn_name    = fdec->token->name;
				int fn_len     = strlen(fn_name);
				bool is_clean_f = fn_len > 6 && strcmp(fn_name + fn_len - 6, ".clean") == 0;
				int fxd_count  = fdec->left->children_count;
				if (is_clean_f && fxd_count >= 1)
				{
					Token *self_tok = fdec->left->children[0]->token;
					if (self_tok->type == STRUCT_CALL && self_tok->is_ref)
					{
						Node   *sd       = self_tok->Struct.ptr;
						TypeRef st_type  = sd->token->llvm.stType;
						TypeRef ptr_type = _pointer_type(st_type, 0);
						Value self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
						for (int i = 0; i < sd->children_count; i++)
						{
							Token *field = sd->children[i]->token;
							if (field->type != STRUCT_CALL) continue;
							Node  *fsd   = field->Struct.ptr;
							char  *qname = strjoin(fsd->token->name, ".clean", NULL);
							Value ffn   = _get_named_function(qname);
							free(qname);
							if (!ffn) continue;
							Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
							Value field_ptr = _build_gep2(st_type, self_ptr, indices, 2, field->name);
							Value args[]    = { field_ptr };
							_build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
						}
					}
				}
			}
		}

		// Inject ura_scope struct clean calls before main returns
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC)
				{ fdec = level_scope[i]; break; }
			if (fdec && strcmp(fdec->token->name, "main") == 0)
			{
				for (int i = 0; i < ura_scope->children_count; i++)
				{
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0) continue;
					char *qname   = strjoin(child->token->Struct.ptr->token->name, ".clean", NULL);
					Value clean_f = _get_named_function(qname);
					free(qname);
					if (!clean_f) continue;
					Value args[] = { child->token->llvm.elem };
					_build_call2(_global_get_value_type(clean_f), clean_f, args, 1, "");
				}
			}
		}

		// Tuple return: pack multiple values into an anonymous struct
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC) { fdec = level_scope[i]; break; }
			if (fdec && fdec->token->retType == TUPLE && node->children_count > 0)
			{
				TypeRef tuple_type = fdec->token->llvm.stType;
				Value agg        = LLVMGetUndef(tuple_type);
				for (int i = 0; i < node->children_count; i++)
				{
					gen_asm(node->children[i]);
					load_if_necessary(node->children[i]);
					Value v = node->children[i]->token->llvm.elem;
					agg = LLVMBuildInsertValue(builder, agg, v, i, "");
				}
				LLVMBuildRet(builder, agg);
				break;
			}
		}

		if (node->left && node->left->token->type != VOID)
		{
			gen_asm(node->left);
			Token *ret_tok = node->left->token;

			// Find the enclosing FDEC to check its return type
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC)
				{ fdec = level_scope[i]; break; }

			bool fdec_returns_ref    = fdec && fdec->token->is_ref;
			bool fdec_returns_struct = fdec && fdec->token->retType == STRUCT_CALL;

			if (fdec_returns_struct && !fdec_returns_ref)
			{
				// return by value: load the full struct
				Value val = load_value(ret_tok);
				LLVMBuildRet(builder, val);
				break;
			}
			else if (fdec_returns_struct && fdec_returns_ref)
			{
				// return by ref: return the pointer directly (self->llvm.elem holds User*)
				// self is stored as alloca of ptr, so load the ptr
				TypeRef st_type  = get_llvm_type(ret_tok);
				TypeRef ptr_type = _pointer_type(st_type, 0);
				Value ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
				LLVMBuildRet(builder, ptr);
				break;
			}
			load_if_necessary(node->left);
		}
		if (node->left)
			_build_return(node->left->token);
		break;
	}
	case AS:
	{
		gen_asm(left);
		load_if_necessary(left);

		Value source     = left->token->llvm.elem;
		TypeRef sourceType = LLVMTypeOf(source);
		TypeRef targetType = get_llvm_type(node->right->token);

		TypeKind sourceKind = LLVMGetTypeKind(sourceType);
		TypeKind targetKind = LLVMGetTypeKind(targetType);

		Value result;

		// Pointer to integer
		if (sourceKind == PointerType && targetKind == IntegerType)
			result = LLVMBuildPtrToInt(builder, source, targetType, "as");
		// Integer to pointer
		else if (sourceKind == IntegerType && targetKind == PointerType)
			result = LLVMBuildIntToPtr(builder, source, targetType, "as");
		// Integer to integer
		else if (sourceKind == IntegerType && targetKind == IntegerType)
		{
			unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
			unsigned targetBits = LLVMGetIntTypeWidth(targetType);

			if (sourceBits > targetBits)
				result = LLVMBuildTrunc(builder, source, targetType, "as");
			else if (sourceBits < targetBits)
				result = LLVMBuildSExt(builder, source, targetType, "as");
			else
				result = source;
		}
		// Pointer to pointer (bitcast)
		else if (sourceKind == PointerType && targetKind == PointerType)
			result = LLVMBuildBitCast(builder, source, targetType, "as");

		else
		{
			check(1, "unsupported cast from %d to %d", sourceKind, targetKind);
			result = source;
		}

		node->token->llvm.elem      = result;
		node->token->llvm.is_loaded = true;
		break;
	}
	case ENUM_DEF:
	{
		for (int i = 0; i < node->children_count; i++)
		{
			Token *var = node->children[i]->token;
			Value g   = LLVMAddGlobal(module, i32, var->name);
			LLVMSetInitializer(g, _const_int(i32, (unsigned)var->Int.value, 0));
			LLVMSetGlobalConstant(g, 1);
			var->llvm.elem = g;
			// var->llvm.is_set = true;
			var->is_dec    = false;
		}
		break;
	}
	case STRUCT_DEF:
	{
		if (node->token->used == 0) return;
		// Ensure nested struct field types are also emitted
		for (int i = 0; i < node->children_count; i++)
		{
			Token *ft = node->children[i]->token;
			if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0)
			{
				ft->Struct.ptr->token->used++;
				gen_asm(ft->Struct.ptr);
			}
		}
		int pos   = node->children_count;
		TypeRef *types = allocate((pos + 1), sizeof(TypeRef));
		int j     = 0;
		for (int i = 0; i < pos; i++)
			types[j++] = get_llvm_type(node->children[i]->token);
		char *struct_name = strjoin("struct.", node->token->name, NULL);
		node->token->llvm.stType = _named_struct_type(struct_name, types, j, 0);
		free(struct_name);
		free(types);

		TypeRef st_type     = node->token->llvm.stType;
		TypeRef ptr_type    = _pointer_type(st_type, 0);
		TypeRef lc_params[] = { ptr_type };
		TypeRef lc_fn_type  = _function_type(vd, lc_params, 1, 0);

		if (!node->token->is_init)
		{
			char *fname = strjoin(node->token->name, ".init", NULL);
			Value fn    = _add_function(fname, lc_fn_type);
			free(fname);
			Block entry = _append_basic_block_in_context(fn, "entry");
			_position_at(entry);
			LLVMSetCurrentDebugLocation2(builder, NULL);
			Value self = LLVMGetParam(fn, 0);
			LLVMBuildStore(builder, LLVMConstNull(st_type), self);
			for (int i = 0; i < node->children_count; i++)
			{
				Token *field = node->children[i]->token;
				if (field->type != STRUCT_CALL) continue;
				Node  *fsd   = field->Struct.ptr;
				char  *qname = strjoin(fsd->token->name, ".init", NULL);
				Value ffn   = _get_named_function(qname);
				free(qname);
				if (!ffn) continue;
				Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
				Value field_ptr = _build_gep2(st_type, self, indices, 2, field->name);
				Value args[]    = { field_ptr };
				_build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
			}
			LLVMBuildRetVoid(builder);
			node->token->is_init = true;
		}

		if (!node->token->is_clean)
		{
			char *fname = strjoin(node->token->name, ".clean", NULL);
			Value fn    = _add_function(fname, lc_fn_type);
			free(fname);
			Block entry = _append_basic_block_in_context(fn, "entry");
			_position_at(entry);
			LLVMSetCurrentDebugLocation2(builder, NULL);
			Value self = LLVMGetParam(fn, 0);
			for (int i = 0; i < node->children_count; i++)
			{
				Token *field = node->children[i]->token;
				if (field->type != STRUCT_CALL) continue;
				Node  *fsd   = field->Struct.ptr;
				char  *qname = strjoin(fsd->token->name, ".clean", NULL);
				Value ffn   = _get_named_function(qname);
				free(qname);
				if (!ffn) continue;
				Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
				Value field_ptr = _build_gep2(st_type, self, indices, 2, field->name);
				Value args[]    = { field_ptr };
				_build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
			}
			LLVMBuildRetVoid(builder);
			node->token->is_clean = true;
		}

		for (int i = 0; i < node->methods_count; i++)
			gen_asm(node->methods[i]);
		break;
	}
	case DOT:
	{
		gen_asm(node->left);

		Token *struct_tok  = node->left->token;
		int field_index = node->right->token->Struct.index;

		// If the struct is a ref, we need to load the pointer first
		// before indexing into it, otherwise we GEP into the alloca
		// that holds the pointer, not into the struct itself.
		if (struct_tok->is_ref)
		{
			TypeRef struct_type = get_llvm_type(struct_tok);
			Value struct_ptr  = LLVMBuildLoad2(builder,
			                                   _pointer_type(struct_type, 0),
			                                   struct_tok->llvm.elem,
			                                   struct_tok->name
			                                   );
			Value indices[] =
			{
				_const_int(i32, 0, 0),
				_const_int(i32, field_index, 0),
			};
			node->token->llvm.elem = _build_gep2(
				struct_type, struct_ptr, indices, 2, node->right->token->name
				);
		}
		else
		{
			Value field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);
			node->token->llvm.elem = field_ptr;
		}
		break;
	}
	case ACCESS:
	{
		// bool enable_bounds_check = false;
		gen_asm(node->left);

		if (node->left->token->type == STRUCT_CALL)
		{
			Token *struct_tok  = node->left->token;
			int field_index = node->right->token->Struct.index;

			Value field_ptr   = struct_field_ptr(struct_tok, field_index, node->right->token->name);

			node->token->llvm.elem = field_ptr;
			/* retType already set by gen_ir; mark as NOT loaded so
			   load_if_necessary() will emit the load when reading. */
			break;
		}

		gen_asm(node->right);
		Value leftValue;
		if (node->left->token->is_ref)
		{
			todo(1, "stop");
			// If left is a ref, dereference it to get the actual array
			leftValue = load_value(node->left->token);
		}
		else
		{
			// todo(1, "stop");
			load_if_necessary(node->left);
			leftValue = node->left->token->llvm.elem;
			TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
			if (kind == PointerType && node->left->token->name &&
			    !node->left->token->llvm.is_loaded &&
			    node->left->token->type != STACK)
			{
				leftValue = LLVMBuildLoad2(builder, p8, leftValue, "ptr_load");
			}
		}

		// Load the index (handles refs)
		load_if_necessary(node->right);
		Value rightRef = node->right->token->llvm.elem;

		// multi-dim flat array: compute stride, emit GEP to sub-array, propagate dims
		int left_depth = node->left->token->llvm.dim_count;
		if (left_depth > 1)
		{
			Value stride = _const_int(i32, 1, 0);
			for (int d = 1; d < left_depth; d++)
				stride = _build_mul(stride, node->left->token->llvm.dim_sizes[d], "stride");
			Value flat_idx = _build_mul(rightRef, stride, "flat_idx");

			TypeRef base_t;
			if (node->left->token->Array.elem_type == STRUCT_CALL && node->left->token->Array.struct_ptr)
				base_t = get_llvm_type(node->left->token->Array.struct_ptr->token);
			else
			{
				Token tmp = {.type = node->left->token->Array.elem_type};
				base_t = get_llvm_type(&tmp);
			}
			Value indices[] = {flat_idx};
			node->token->llvm.elem        = _build_gep2(base_t, leftValue, indices, 1, "row");
			node->token->llvm.is_loaded   = true;
			node->token->retType          = ARRAY;
			node->token->Array.elem_type  = node->left->token->Array.elem_type;
			node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
			node->token->Array.depth      = left_depth - 1;
			node->token->llvm.dim_count   = left_depth - 1;
			for (int d = 1; d < left_depth; d++)
				node->token->llvm.dim_sizes[d - 1] = node->left->token->llvm.dim_sizes[d];
			break;
		}

		TypeRef element_type;
		Type left_elem_type = node->left->token->retType
		      ? node->left->token->retType
		      : node->left->token->type;
		if (left_elem_type == CHARS)
		{
			element_type         = i8;
			node->token->retType = CHAR;
		}
		else if (left_elem_type == ARRAY_TYPE || left_elem_type == ARRAY)
		{
			Type et = node->left->token->Array.elem_type;
			if (et == STRUCT_CALL && node->left->token->Array.struct_ptr)
			{
				element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
				node->token->Struct.ptr = node->left->token->Array.struct_ptr;
			}
			else
			{
				Token tmp = {.type = et};
				element_type = get_llvm_type(&tmp);
			}
			node->token->retType = et;
		}
		else if (node->left->token->type == HEAP || node->left->token->type == ARRAY)
		{
			check(1, "hello");
			Type et = node->left->token->Array.elem_type;
			if (et == STRUCT_CALL && node->left->token->Array.struct_ptr)
			{
				element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
				node->token->Struct.ptr = node->left->token->Array.struct_ptr;
			}
			else
			{
				Token tmp = {.type = et};
				element_type = get_llvm_type(&tmp);
			}
			node->token->retType = et;
		}
		else
		{
			element_type         = get_llvm_type(node->left->token);
			node->token->retType = node->left->token->type;
		}

		// Add bounds checking if enabled
		if (enable_bounds_check)
		{
			// We need to track array sizes - for now we'll use a conservative approach
			// For CHARS (strings), we can use strlen at runtime
			// For STACK arrays, we need to store the size

			Value size_val       = NULL;

			Type left_elem_type = node->left->token->retType
		      ? node->left->token->retType
		      : node->left->token->type;
			if (left_elem_type == CHARS)
			{
				// For strings, we need to get the length
				// First check if it's a string literal or a variable
				if (node->left->token->name && !left->token->llvm.array_size)
				{
					// It's a variable - we need strlen
					Value strlen_func = _get_named_function("strlen");
					if (!strlen_func)
					{
						TypeRef strlen_type = _function_type(i64, (TypeRef[]) {p8}, 1, false);
						strlen_func = _add_function("strlen", strlen_type);
					}
					Value strlen_result = _build_call2(_global_get_value_type(strlen_func), strlen_func,
					                                   (Value[]) {leftValue}, 1, "strlen");
					size_val = _build_trunc(strlen_result, i32, "size");
				}
				else if (node->left->token->llvm.array_size)
				{
					size_val = node->left->token->llvm.array_size;
				}
				else
				{
					// String literal - we know the size at compile time
					// This is handled in _chars function
					size_val = _const_int(i32, strlen(node->left->token->Chars.value), 0);
				}
			}
			else if (node->left->token->type == STACK)
			{
				// For STACK arrays, we should have stored the size
				// For now, use a placeholder - you'll need to enhance this
				size_val = _const_int(i32, 1000000, 0); // Conservative large value
			}

			if (size_val)
			{
				// Get bounds check function
				Value bounds_check = _get_named_function("__bounds_check");

				// Create filename string
				static Value filename_str = NULL;

				if (!filename_str)
				{
					char filename[256] = {0};

					if (getcwd(filename, sizeof(filename)) != NULL)
					{
						size_t len = strlen(filename);
						snprintf(filename + len, sizeof(filename) - len, "/%s", "input_file");
						filename_str = _const_chars(filename, "filename");
					}
				}


				// Call bounds check: __bounds_check(index, size, line, filename)
				Value line_val = _const_int(i32, node->token->line, 0);
				_build_call2(_global_get_value_type(bounds_check), bounds_check,
				             (Value[]) {rightRef, size_val, line_val, filename_str}, 4, "");
			}
		}

		Value indices[] = { rightRef };
		Value gep       = _build_gep2(element_type, leftValue, indices, 1, "ACCESS");
		node->token->llvm.elem = gep;

		break;
	}
	case TYPEOF: case SIZEOF:
	{
		Token *type_tok = node->left->token;
		if (node->token->type == TYPEOF)
		{
			node->token->llvm.elem      = _const_chars(node->token->Chars.value, "typeof");
			node->token->llvm.is_loaded = true;
		}
		else
		{
			TypeRef type = get_llvm_type(type_tok);
			TargetData td   = _get_module_data_layout(module);
			size_t size = _abi_size_of_type(td, type);
			node->token->Int.value      = (long long)size;
			node->token->llvm.elem      = _const_int(i32, size, 0);
			node->token->llvm.is_loaded = true;
		}
		break;
	}
	case OUTPUT:
	{
		int argc = node->left->children_count;
		Node **argv = node->left->children;

		// Compute format string capacity: literals + format specifiers + struct expansion
		int fmt_cap = 64;
		for (int i = 0; i < argc; i++)
			if (argv[i]->token->type == CHARS && !argv[i]->token->name)
				fmt_cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
			else
				fmt_cap += 128;

		char  *fmt             = allocate(fmt_cap, 1);
		Value *args            = allocate(256, sizeof(Value));
		int functions_count = 0;
		int nargs           = 0;

		for (int i = 0; i < argc; i++)
		{
			gen_asm(argv[i]);
			append_output_arg(argv[i]->token, fmt, &functions_count, args, &nargs);
		}
		fmt[functions_count] = '\0';

		Value printf_fn = _get_named_function("printf");
		if (!printf_fn)
		{
			TypeRef params[]  = {p8, i32};// fmt, nargs (language variadic convention)
			TypeRef printf_ft = _function_type(i32, params, 2, 1);
			printf_fn = _add_function("printf", printf_ft);
		}
		TypeRef printf_ft = _global_get_value_type(printf_fn);
		// Follow the language's variadic calling convention: insert arg count between
		// fixed params and variadic args — same as FCALL does for variadic functions
		Value *call_args = allocate(nargs + 3, sizeof(Value));
		call_args[0] = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
		call_args[1] = _const_int(i32, nargs, 0);
		memcpy(call_args + 2, args, nargs * sizeof(Value));
		LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 2, "");
		free(fmt); free(args); free(call_args);
		break;
	}
	default:
		pnode(node, "");
		todo(1, "handle this case %s", to_string(node->token->type));
		// seg();
		break;
	}
}

void add_link_key(char *key)
{
	// deduplicate
	for (int i = 0; i < lcount; i++)
		if (strcmp(links[i], key) == 0) return;
	resize_array(links, char*, lsize, lcount);
	links[lcount++] = strdup(key);
}

char *compile(char *filename)
{
	debug("%s===========================================\n", RED);
	debug("START COMPILATION\n");
	debug("===========================================\n%s",RESET);
	// reset ura_scope state for each file
	found_error = false;
	tcount      = 0; ecount = 0;
	sccount     = 0; scope = NULL;
	ura_scope   = NULL;

	tokenize(filename);
	if (found_error) return NULL;

#if AST
	if (found_error) return NULL;
	ura_scope = new_node(new_token(ID, -TAB));
	setName(ura_scope->token, "ura-scope");
	enter_scope(ura_scope);
	while (!find(END, 0) && !found_error)
		add_child(ura_scope, expr_node());
	debug("%s===========================================\n", GREEN);
	debug("AFTER PARSING\n");
	debug("===========================================\n%s",RESET);
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
	if (found_error) return NULL;
#endif

#if IR
	if (found_error) return NULL;
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_ir(ura_scope->children[i]);
	if (found_error) return NULL;
	debug("%s===========================================\n", GREEN);
	debug("AFTER IR\n");
	debug("===========================================\n%s",RESET);
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#if OPTIMIZE
	bool changed = true;
	int op      = 0;
	while(changed)
	{
		changed = false;
		optimize_ir(ura_scope, &changed);
		if(changed) op++;
	}
	debug("%s===========================================\n", GREEN);
	debug("AFTER OPTMIZING\n");
	debug("===========================================\n%s",RESET);
	for (int i = 0; op && !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#endif
#endif


#if ASM
	if (found_error) return NULL;

	// build/  directory next to the source file
	char *dir  = dirname(strdup(filename));
	char *base = basename(strdup(filename));
	// strip .ura extension
	char *dot  = strrchr(base, '.');
	if (dot) *dot = '\0';

	// mkdir build/
	char *build_dir = strjoin(dir, "/build", NULL);
	mkdir(build_dir, 0755);

	char *ll_path    = strjoin(build_dir, "/", strjoin(base, ".ll", NULL));

	char *moduleName = resolve_path(filename);
	init(moduleName);
	free(moduleName);

	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_asm(ura_scope->children[i]);

	finalize(ll_path);
	free(build_dir);
	return found_error ? NULL : ll_path;
#endif
	return NULL;
}

int main(int argc, char **argv)
{
	argv0 = argv[0];
	if(check(argc < 2, "usage: ura <file.ura> [file2.ura ...]"
	         "[-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
		return 1;

	debug("recieved %d files compiling\n", argc - 1);
	for(int i = 1; i < argc; i++)
	{
		debug("%s\n", argv[i]);
	};
	char  *output       = "exe.out";
	char **src_files    = NULL;
	int src_count    = 0;
	bool testing_mode = false;
	bool no_exec      = false;

	for (int i = 1; i < argc; i++)
	{
		if      (strcmp(argv[i], "-O0")     == 0) op_flags = PASSES_O0;
		else if (strcmp(argv[i], "-O1")     == 0) op_flags = PASSES_O1;
		else if (strcmp(argv[i], "-O2")     == 0) op_flags = PASSES_O2;
		else if (strcmp(argv[i], "-O3")     == 0) op_flags = PASSES_O3;
		else if (strcmp(argv[i], "-Os")     == 0) op_flags = PASSES_Os;
		else if (strcmp(argv[i], "-Oz")     == 0) op_flags = PASSES_Oz;
		else if (strcmp(argv[i], "-san")    == 0) enable_asan = true;
		else if (strcmp(argv[i], "-testing") == 0) testing_mode = true;
		else if (strcmp(argv[i], "-no-exec") == 0) no_exec = true;
		else if (strcmp(argv[i], "-o")      == 0)
		{
			check(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		}
		else if (argv[i][0] != '-')
		{
			src_files = realloc(src_files, (src_count + 1) * sizeof(char *));
			src_files[src_count++] = argv[i];
		}
		else
			fprintf(stderr, "unknown flag: %s\n", argv[i]);
	}

	bool link_ok = true;
	char final_cmd[8192];
	int pos     = 0;

	pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
	if (enable_asan)
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos,
		                " -fsanitize=address,undefined -fno-omit-frame-pointer -g");

	for (int i = 0; i < src_count && link_ok; i++)
	{
		char *ll = compile(src_files[i]);
      #if ASM
		if (!ll)
		{
			link_ok = false;
			break;
		}
		if (testing_mode)
		{
			free(ll);
			free_local_memory();
			continue;
		}
		if (enable_asan)
			pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
		else
		{
			char s[4096];
			snprintf(s, sizeof(s), "%s", ll);
			char *dot = strrchr(s, '.');
			if (dot) strcpy(dot, ".s");

			char llc_cmd[4096];
			snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
			if (system(llc_cmd) != 0)
			{
				fprintf(stderr, RED "llc failed for %s\n" RESET, ll);
				link_ok = false;
				free(ll);
				break;
			}
			pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", s);
		}
		free(ll);
      #endif

		free_local_memory();
	}

	// append link flags from link directives (e.g. link "@/raylib")
	for (int i = 0; i < lcount; i++)
	{
		char env_name[128];
		snprintf(env_name, sizeof(env_name), "URA_LINK_%s", links[i]);
		char *flags = getenv(env_name);
		if (flags)
			pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
	}

	if (!testing_mode && link_ok && !no_exec)
	{
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
		if (system(final_cmd) != 0)
			fprintf(stderr, RED "linking failed\n" RESET);
		else
		{
			char full[4096];
			realpath(output, full);

			if (enable_asan)
			{
				char *asan_file = getenv("ASAN_FILE");
				if (asan_file)
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1),
					setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
				else
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
			}

			fprintf(stderr, GREEN "running %s...\n" RESET, full);

			char run[8192];
			snprintf(run, sizeof(run), "\"%s\"", full);

			int status    = system(run);
			int exit_code = WEXITSTATUS(status);

			fprintf(stderr, exit_code == 0
		 ? GREEN "exit code: %d\n" RESET
		 : RED   "exit code: %d\n" RESET, exit_code);
		}
	}

	free(src_files);
	return found_error || !link_ok;
}
