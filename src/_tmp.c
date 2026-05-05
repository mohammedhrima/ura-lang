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

#define CHECK(cond, fmt, ...) _CHECK(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define TODO(cond, fmt, ...)                                                                       \
	if (_CHECK(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define SEG() raise(SIGSEGV)
#define EXPECT_TOKEN(type, fmt, ...)                                                               \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			CHECK(1, fmt, ##__VA_ARGS__);                                                             \
			return syntax_error();                                                                    \
		}                                                                                            \
	}

#define debug(fmt, ...)                                                                            \
	if (enable_debug) _debug(fmt, ##__VA_ARGS__)

#define DATA_TYPES     INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT, ARRAY_TYPE, LIST_TYPE
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
typedef LLVMAttributeRef AttributeRef;

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
			Type  sub_type;
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

	Node  **modules;
	int     modules_count;
	int     modules_size;

	Value  *temps;
	Node  **temp_defs;
	int     temps_count;
	int     temps_size;
};

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
void  *allocate(int len, int size);
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
void    ensure_loaded(Node *node);
void    _alloca(Token *token);
void    hoist_allocas(Node *node);
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
Value   _get_default_value(Token *token);
Value   read_value(Token *token);
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
bool  _CHECK(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
int   _debug(char *conv, ...);
void  pnode(Node *node, char *indent);
char *strjoin(char *str0, char *str1, char *str2);
int   _vprint(File out, char *conv, va_list args);

#endif /* HEADER_H */

// Global variable definitions
bool             found_error;
bool             enable_prep;

Token          **tokens;
int              tokens_count;
int              tokens_size;
int              exe_count;

Node            *ura_scope;
Node            *scope;
Node           **scopes;

int              scopes_count;
int              scopes_size;

char           **used_files;
int              ucount;
int              usize;

Context          context;
Module           module;
Builder          builder;
TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
char            *flags;
bool             enable_san;
bool             enable_debug = true;
char            *argv0;
char            *ura_lib;
int              calling_use;
char            *current_gen_module;

char           **links;
int              lcount;
int              lsize;
char           **files;
int              files_count;
int              files_size;

LLVMDIBuilderRef debug_builder;
LLVMMetadataRef  debug_compile_unit;
LLVMMetadataRef  debug_file;
LLVMMetadataRef  debug_scope;

// generate_list_source — generates Ura source code for a list struct.
// elem_type_name: the element type as it appears in Ura source (e.g., "int", "__list_int")
// struct_name:    the struct name to generate (e.g., "__list_int")
// Returns a heap-allocated string that must be freed by the caller.
char *generate_list_source(const char *elem_type_name, const char *struct_name) {
	char *src = allocate(4096, sizeof(char));
	int   pos = 0;

	bool  elem_is_struct =
	    (elem_type_name[0] == '_') || (elem_type_name[0] >= 'A' && elem_type_name[0] <= 'Z');

	// struct __list_T:
	pos += snprintf(src + pos, 4096 - pos, "struct %s:\n", struct_name);

	// fields — data, __len, __cap
	if (elem_is_struct)
		pos += snprintf(src + pos, 4096 - pos, "   data array[%s]\n", elem_type_name);
	else pos += snprintf(src + pos, 4096 - pos, "   data array[%s]\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "   __len int\n");
	pos += snprintf(src + pos, 4096 - pos, "   __cap int\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// operator delete
	pos += snprintf(src + pos, 4096 - pos, "   operator delete() void:\n");
	pos += snprintf(src + pos, 4096 - pos, "      free(self.data as pointer)\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn push(e T) void
	pos += snprintf(src + pos, 4096 - pos, "   fn push(e %s) void:\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      if self.__len >= self.__cap:\n");
	pos += snprintf(src + pos, 4096 - pos, "         if self.__cap == 0: self.__cap = 8\n");
	pos += snprintf(src + pos, 4096 - pos, "         else: self.__cap *= 2\n");
	pos += snprintf(src + pos, 4096 - pos,
	                "         self.data = realloc(self.data as pointer, self.__cap * sizeof(%s))\n",
	                elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      self.data[self.__len] = e\n");
	pos += snprintf(src + pos, 4096 - pos, "      self.__len += 1\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn pop() T
	pos += snprintf(src + pos, 4096 - pos, "   fn pop() %s:\n", elem_type_name);
	pos += snprintf(src + pos, 4096 - pos, "      self.__len -= 1\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.data[self.__len]\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn len() int
	pos += snprintf(src + pos, 4096 - pos, "   fn len() int:\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.__len\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	// fn cap() int
	pos += snprintf(src + pos, 4096 - pos, "   fn cap() int:\n");
	pos += snprintf(src + pos, 4096 - pos, "      return self.__cap\n");
	pos += snprintf(src + pos, 4096 - pos, "\n");

	return src;
}

// icmp_predicate
LLVMIntPredicate icmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMIntSLT;
	case GREAT:       return LLVMIntSGT;
	case EQUAL:       return LLVMIntEQ;
	case LESS_EQUAL:  return LLVMIntSLE;
	case GREAT_EQUAL: return LLVMIntSGE;
	default:          return LLVMIntNE; // NOT_EQUAL
	}
}

// fcmp_predicate
LLVMRealPredicate fcmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMRealOLT;
	case GREAT:       return LLVMRealOGT;
	case EQUAL:       return LLVMRealOEQ;
	case LESS_EQUAL:  return LLVMRealOLE;
	case GREAT_EQUAL: return LLVMRealOGE;
	default:          return LLVMRealONE; // NOT_EQUAL
	}
}

// is_float_value
int is_float_value(Value v) {
	LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
	return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

// assign_base_op
Type assign_base_op(Type assign_op) {
	switch (assign_op) {
	case ADD_ASSIGN: return ADD;
	case SUB_ASSIGN: return SUB;
	case MUL_ASSIGN: return MUL;
	case DIV_ASSIGN: return DIV;
	default:         return MOD; // MOD_ASSIGN
	}
}

// _branch
void _branch(Block bloc) {
	if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) LLVMBuildBr(builder, bloc);
}

// _append_block
Block _append_block(char *name) {
	Value parent = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
	return LLVMAppendBasicBlockInContext(context, parent, name);
}

// _named_struct_type
TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed) {
	TypeRef type = LLVMStructCreateNamed(context, name);
	LLVMStructSetBody(type, element_types, element_count, packed);
	return type;
}

// _add_function
Value _add_function(char *name, TypeRef function_type) {
	Value f = LLVMGetNamedFunction(module, name);
	if (f) return f;
	return LLVMAddFunction(module, name, function_type);
}

// set_debug_location
void set_debug_location(Token *token) {
	if (!token || !debug_builder || !debug_scope)              return;
	if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
	LLVMMetadataRef loc =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, debug_scope, NULL);
	LLVMSetCurrentDebugLocation2(builder, loc);
}

// struct_field_ptr
Value struct_field_ptr(Token *struct_tok, int field_index, char *name) {
	TypeRef struct_type = get_llvm_type(struct_tok);
	Value   indices[]   = {
	        LLVMConstInt(i32, 0, 0),
	        LLVMConstInt(i32, field_index, 0),
   };
	return LLVMBuildGEP2(builder, struct_type, struct_tok->llvm.elem, indices, 2, name);
}

// read_value
Value read_value(Token *token) {
	if (token->llvm.is_loaded) return token->llvm.elem;
	// Already-computed values: function returns, literals, allocated buffers
	if (includes(token->type, MATH_TYPE, FCALL, STACK, HEAP, 0)) return token->llvm.elem;
	if (!token->name && !includes(token->type, DOT, ACCESS, 0))  return token->llvm.elem;

	// Scalar ref: double-deref (alloca-of-ptr → ptr → value)
	if (token->is_ref && token->type != STRUCT_CALL) {
		TypeRef type = get_llvm_type(token);
		Value   ptr  = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), token->llvm.elem, "ref_ptr");
		return LLVMBuildLoad2(builder, type, ptr, "ref_val");
	}

	char *name = token->name;
	if (token->type == DOT)    name = to_string(DOT);
	if (token->type == ACCESS) name = to_string(ACCESS);
	return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, name ? name : "");
}

// write_value
void write_value(Token *token, Value val) {
	Value dest = token->llvm.elem;
	if (token->is_ref) {
		TypeRef type = get_llvm_type(token);
		dest = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), token->llvm.elem, "ref_ptr");
	}
	LLVMBuildStore(builder, val, dest);
}

// new_token
Token *new_token(Type type, int space) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

// unuse
void unuse(Node *node) {
	if (!node || !node->token) return;

	if (node->left && node->left->token) {
		if (node->left->token->used > 0) node->left->token->used--;
	}
	if (node->right && node->right->token) {
		if (node->right->token->used > 0) node->right->token->used--;
	}
	for (int i = 0; i < node->children_count; i++)
		if (node->children[i] && node->children[i]->token)
			if (node->children[i]->token->used > 0) node->children[i]->token->used--;
}

// resolve_path
char *resolve_path(char *path) {
	if (path == NULL) return NULL;
	char *cleaned = allocate(strlen(path) + 5, 1);
	if (!cleaned) return NULL;
	size_t i = 0, j = 0;
	while (path[i]) {
		cleaned[j++] = path[i++];
		while (path[i] == '/') {
			if (cleaned[j - 1] != '/') cleaned[j++] = '/';
			i++;
		}
	}
	if (j > 1 && cleaned[j - 1] == '/') j--;
	cleaned[j] = '\0';
	return cleaned;
}

// _allocate
void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

// free_token
void free_token(Token *token) {
	free(token->name);
	free(token->Chars.value);
	free(token->llvm.dims);
	free(token);
}

// free_node
void free_node(Node *node) {
	if (!node) return;
	for (int i = 0; i < node->children_count; i++)
		free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->functions);
	free(node->variables);
	free(node->structs);
	free(node);
}

// free_local_memory
void free_local_memory() {
	for (int i = 0; tokens && tokens[i]; i++) {
		free_token(tokens[i]);
		tokens[i] = NULL;
	}
	tokens_count = 0;
	exe_count    = 0;
}

void free_global_memory() {
	for (int i = 0; links && links[i]; i++) {
		free(links[i]);
		links[i] = NULL;
	}
	lcount = 0;
}

// add_token
void add_token(Token *token) {
	resize_array(tokens, Token *, tokens_size, tokens_count);
	tokens[tokens_count++] = token;
}

// new_node
Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

// add_child
Node *add_child(Node *node, Node *child) {
	if (child) {
		resize_array(node->children, Node *, node->children_size, node->children_count);
		child->token->space                    = node->token->space + TAB;
		node->children[node->children_count++] = child;
	}
	return child;
}

// includes
bool includes(Type to_find, ...) {
	if (found_error) return false;
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

// setName
void setName(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

// to_string
char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
	    [INT] = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
	    [FDEC] = "FDEC", [FCALL] = "CALL", [END] = "END", [LPAR] = "LPAR",
	    [RPAR] = "RPAR", [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
	    [WHILE] = "WHILE", [BREAK] = "BRK", [CONTINUE] = "CONT",
	    [SHORT] = "SHORT", [RETURN] = "RET", [BAND] = "BAND", [BOR] = "BOR",
	    [BXOR] = "BXOR", [BNOT] = "BNOT", [LSHIFT] = "LSHIFT",
	    [RSHIFT] = "RSHIFT", [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
	    [DIV] = "DIV", [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD_ASS",
	    [SUB_ASSIGN] = "SUB_ASS", [MUL_ASSIGN] = "MUL_ASS",
	    [DIV_ASSIGN] = "DIV_ASS", [MOD_ASSIGN] = "MOD_ASS", [ACCESS] = "ACC",
	    [MOD] = "MOD", [COMA] = "COMA", [REF] = "REF", [EQUAL] = "EQ",
	    [NOT_EQUAL] = "NEQ", [LESS] = "LT", [GREAT] = "GT",
	    [LESS_EQUAL] = "LE", [NOT] = "NOT", [GREAT_EQUAL] = "GE",
	    [AND] = "AND", [OR] = "OR", [DOTS] = "DOTS", [PROTO] = "PROT",
	    [VARIADIC] = "VAR", [TYPEOF] = "TYPEOF", [SIZEOF] = "SIZEOF",
	    [OUTPUT] = "OUTPUT", [ARGS] = "ARGS", [CHILDREN] = "CHILDREN",
	    [AS] = "AS", [STACK] = "STACK", [HEAP] = "HEAP",
	    [ARRAY_TYPE] = "ARRAY_TYPE", [ARRAY_LIT] = "ARRAY_LIT",
	    [NULLABLE] = "NULLABLE",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	    [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [ENUM_DEF] = "ENUM_DEF", [ENUM_CALL] = "ENUM_CALL", [TUPLE] = "TUPLE",
	    [TUPLE_UNPACK] = "TUPLE_UNPACK", [LBRA] = "LBRA", [RBRA] = "RBRA",
	    [ARRAY] = "ARRAY", [LIST] = "LIST", [LIST_TYPE] = "LIST_TYPE",
	    [DOT] = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR", [MODULE] = "MODULE",
	    [OPERATOR] = "OPERATOR_KW", [PUB] = "PUB",
	    [DOUBLE_DOTS] = "DOUBLE_DOTS", [DELETE] = "DELETE",
	};

	if (CHECK(!res[type], "handle this case %d\n", type)) {
		// SEG();
		exit(1);
	}
	return res[type];
}

// new_struct
Node *new_struct(Node *node) {
	debug(CYAN("new struct [%s] in scope %k\n"), node->token->name, scope->token);
	for (int i = 0; i < scope->structs_count; i++) {
		Token *curr = scope->structs[i]->token;
		bool   cond = (strcmp(curr->name, node->token->name) == 0);
		CHECK(cond, "Redefinition of %s", node->token->name);
	}
	resize_array(scope->structs, Node *, scope->structs_size, scope->structs_count);
	scope->structs[scope->structs_count++] = node;
	return node;
}

// add_struct
void add_struct(Node *parent, Node *node) {
	resize_array(parent->structs, Node *, parent->structs_size, parent->structs_count);
	parent->structs[parent->structs_count++] = node;
}

// syntax_error
Node *syntax_error() {
	found_error = true;
	static Node *node;
	if (node == NULL) node = new_node(new_token(SYNTAX_ERROR, -1));
	return node;
}

// add_variable
void add_variable(Node *parent, Token *token) {
	resize_array(parent->variables, Token *, parent->variables_size, parent->variables_count);
	parent->variables[parent->variables_count++] = token;
}

// new_variable
Token *new_variable(Token *token) {
	debug(CYAN("new variable [%k] in scope %k\n"), token, scope->token);
	for (int i = 0; i < scope->variables_count; i++) {
		Token *curr = scope->variables[i];
		bool   cond = (strcmp(curr->name, token->name) == 0);
		CHECK(cond, "Redefinition of %s", token->name);
	}
	add_variable(scope, token);
	return token;
}

// copy_token
Token *copy_token(Token *token) {
	if (token == NULL) return NULL;
	Token *new = allocate(1, sizeof(Token));
	memcpy(new, token, sizeof(Token));
	new->name = NULL;
	if (token->name)        setName(new, token->name);
	if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
	new->llvm.dims       = NULL;
	new->llvm.dims_count = 0;
	new->llvm.dims_size  = 0;
	for (int i = 0; i < token->llvm.dims_count; i++) {
		resize_array(new->llvm.dims, Value, new->llvm.dims_size, new->llvm.dims_count);
		new->llvm.dims[new->llvm.dims_count++] = token->llvm.dims[i];
	}
	add_token(new);
	return new;
}

// _vprint
int _vprint(File out, char *conv, va_list args) {
	int res = 0;

	for (int i = 0; conv[i]; i++) {
		if (conv[i] != '%') {
			res += fprintf(out, "%c", conv[i]);
			continue;
		}
		i++;
		int left_align = 0;
		if (conv[i] == '-') {
			left_align = 1;
			i++;
		}
		int width = 0;
		while (isdigit(conv[i])) {
			width = width * 10 + (conv[i] - '0');
			i++;
		}
		int prec = -1;
		if (conv[i] == '.') {
			i++;
			prec = 0;
			while (conv[i] >= '0' && conv[i] <= '9') {
				prec = prec * 10 + (conv[i] - '0');
				i++;
			}
		}
		if (strncmp(conv + i, "zu", 2) == 0) {
			res += fprintf(out, "%d", va_arg(args, int));
			i++;
		} else if (strncmp(conv + i, "lld", 3) == 0) {
			res += fprintf(out, "%lld", va_arg(args, long long));
			i += 2;
		} else {
			switch (conv[i]) {
			case 'c': res += fprintf(out, "%c", va_arg(args, int)); break;
			case 's': {
				char *str = va_arg(args, char *);
				if (!str) str = "(null_str)";
				if (left_align)
					res += (prec >= 0) ? fprintf(out, "%-*.*s", width, prec, str)
					                   : fprintf(out, "%-*s", width, str);
				else
					res += (prec >= 0) ? fprintf(out, "%*.*s", width, prec, str)
					                   : fprintf(out, "%*s", width, str);
				break;
			}
			case 'p': res += fprintf(out, "%p", va_arg(args, void *)); break;
			case 'x':
				res += (prec >= 0) ? fprintf(out, "%.*x", prec, va_arg(args, unsigned int))
				                   : fprintf(out, "%x", va_arg(args, unsigned int));
				break;
			case 'X':
				res += (prec >= 0) ? fprintf(out, "%.*X", prec, va_arg(args, unsigned int))
				                   : fprintf(out, "%X", va_arg(args, unsigned int));
				break;
			case 'd':
				res += (prec >= 0) ? fprintf(out, "%.*d", prec, va_arg(args, int))
				                   : fprintf(out, "%d", va_arg(args, int));
				break;
			case 'f':
				res += (prec >= 0) ? fprintf(out, "%.*f", prec, va_arg(args, double))
				                   : fprintf(out, "%f", va_arg(args, double));
				break;
			case '%': res += fprintf(out, "%%"); break;
			case 't': res += fprintf(out, "%s", to_string((Type)va_arg(args, Type))); break;
			case 'k': {
				Token *token = va_arg(args, Token *);
				if (!token) {
					fprintf(out, "(null)");
					break;
				}

				fprintf(out, "[%s] ", to_string(token->type));

				switch (token->type) {
				case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:
				case LONG: {
					if (token->name) {
						fprintf(out, "%s ", token->name);
						break;
					}
					if (token->type == VOID) break;
					switch (token->type) {
					case INT:   fprintf(out, "[%lld] ", (long long)token->Int.value); break;
					case LONG:  fprintf(out, "[%lld] ", token->Long.value); break;
					case BOOL:  fprintf(out, "[%s] ", token->Bool.value ? "True" : "False"); break;
					case FLOAT: fprintf(out, "[%f] ", token->Float.value); break;
					case CHAR:  {
						fprintf(out, "[");
						char c = token->Char.value;
						switch (c) {
						case '\n': fprintf(out, "\\n"); break;
						case '\t': fprintf(out, "\\t"); break;
						case '\r': fprintf(out, "\\r"); break;
						case '\\': fprintf(out, "\\\\"); break;
						case '\"': fprintf(out, "\\\""); break;
						default:   fprintf(out, "%c", c); break;
						}
						fprintf(out, "] ");
						break;
					}
					case CHARS: {
						fprintf(out, "[\"");
						char *str = token->Chars.value;
						if (str)
							for (int j = 0; str[j]; j++) {
								switch (str[j]) {
								case '\n': fprintf(out, "\\n"); break;
								case '\t': fprintf(out, "\\t"); break;
								case '\r': fprintf(out, "\\r"); break;
								case '\\': fprintf(out, "\\\\"); break;
								case '\"': fprintf(out, "\\\""); break;
								default:   fprintf(out, "%c", str[j]); break;
								}
							}
						fprintf(out, "\"] ");
						break;
					}
					case ADD:       case SUB:
					case NOT_EQUAL: fprintf(out, "%s ", to_string(token->type)); break;
					default:        break;
					}
					break;
				}
				case STRUCT_CALL:
					fprintf(out, "name [%s] ", token->name);
					fprintf(out, "st_name [%s] ", token->Struct.ptr->token->name);
					break;
				case STRUCT_DEF: case FCALL: case FDEC:
				case ID:         fprintf(out, "%s ", token->name); break;
				default:         break;
				}

				if (token->is_ref)      fprintf(out, "ref ");
				if (token->ret_type)    fprintf(out, "ret [%s] ", to_string(token->ret_type));
				if (token->is_variadic) fprintf(out, "variadic ");
				break;
			}
			default: TODO(1, "invalid format specifier [%c]", conv[i]); break;
			}
		}
	}
	return res;
}

// _debug
int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = _vprint(stdout, conv, args);
	va_end(args);
	return res;
}

// _check
bool _CHECK(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	found_error = true;
	fprintf(stderr, RED("ura_error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

// pnode
void pnode(Node *node, char *indent) {
	if (!node || !node->token || !enable_debug) return;
	Node **subs     = NULL;
	int    count    = 0;
	int    capacity = 0;

#define push(n)                                                                                    \
	do {                                                                                            \
		resize_array(subs, Node *, capacity, count);                                                 \
		subs[count++] = (n);                                                                         \
	} while (0)

	debug("%k\n", node->token);
	if (includes(node->token->type, IF, ELIF, ELSE, 0)) {
		if (node->left) push(node->left);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
		if (node->right) push(node->right);
	} else {
		if (node->left)  push(node->left);
		if (node->right) push(node->right);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
	}
	for (int i = 0; i < node->modules_count; i++)
		push(node->modules[i]);
	for (int i = 0; i < node->structs_count; i++)
		push(node->structs[i]);
	for (int i = 0; i < node->functions_count; i++)
		push(node->functions[i]);
	for (int i = 0; i < count; i++) {
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int         is_last = (i == count - 1);
		const char *bar     = is_last ? "   " : "│  ";

		char        new_indent[4096]; // TODO: to be fixed later
		snprintf(new_indent, sizeof(new_indent), "%s%s", indent, bar);

		char *connector = is_last ? "└──" : "├──";
		debug("%s%s", indent, connector);
		pnode(child, new_indent);
	}
	free(subs);
#undef push
}

// strjoin
char *strjoin(char *str0, char *str1, char *str2) {
	int   len0 = str0 ? strlen(str0) : 0;
	int   len1 = str1 ? strlen(str1) : 0;
	int   len2 = str2 ? strlen(str2) : 0;
	char *res  = allocate(len0 + len1 + len2 + 1, 1);
	if (str0) strcpy(res, str0);
	if (str1) strcpy(res + len0, str1);
	if (str2) strcpy(res + len0 + len1, str2);
	return res;
}

// is_data_type
bool is_data_type(Token *token) { return includes(token->type, DATA_TYPES, 0); }

// copy_node
Node *copy_node(Node *node) {
	Node *new  = allocate(1, sizeof(Node));
	new->token = copy_token(node->token);
	if (node->left)  new->left = copy_node(node->left);
	if (node->right) new->right = copy_node(node->right);
	for (int i = 0; i < node->children_count; i++)
		add_child(new, copy_node(node->children[i]));
	for (int i = 0; i < node->structs_count; i++)
		add_struct(new, node->structs[i]);
	for (int i = 0; i < node->variables_count; i++)
		add_variable(new, copy_token(node->variables[i]));
	return new;
}

// hoist_allocas
void hoist_allocas(Node *node) {
	if (!node) return;
	Token *tok = node->token;

	// don't recurse into nested functions
	if (tok->type == FDEC) return;

	if (includes(tok->type, INT, LONG, SHORT, CHARS, CHAR, BOOL, ARRAY_TYPE, 0) && tok->is_dec) {
		if (!tok->llvm.elem) _alloca(tok);
	} else if (tok->type == STRUCT_CALL && tok->is_dec && !tok->is_ref) {
		if (!tok->llvm.elem) {
			TypeRef struct_type = get_llvm_type(tok);
			tok->llvm.elem      = LLVMBuildAlloca(builder, struct_type, tok->name);
		}
	}

	if (node->left)  hoist_allocas(node->left);
	if (node->right) hoist_allocas(node->right);
	for (int i = 0; i < node->children_count; i++)
		hoist_allocas(node->children[i]);
}

// ensure_loaded
void ensure_loaded(Node *node) {
	Token *token = node->token;

	if (token->is_ref)                                        return;
	if (includes(token->type, MATH_TYPE, 0))                  return;
	if (includes(token->type, DATA_TYPES, 0) && !token->name) return;
	if (token->llvm.is_loaded)                                return;
	if (includes(token->type, STACK, HEAP, FCALL, 0))         return;

	if (token->name || includes(token->type, ACCESS, DOT, 0)) {
		Token *new          = copy_token(token);
		new->llvm.elem      = read_value(token);
		new->llvm.is_loaded = true;
		node->token         = new;
	}
}

// _alloca
void _alloca(Token *token) {
	TypeRef type = get_llvm_type(token);
	if (token->is_ref) type = LLVMPointerType(type, 0);

	Block current = LLVMGetInsertBlock(builder);
	Value func    = LLVMGetBasicBlockParent(current);
	Block entry   = LLVMGetEntryBasicBlock(func);
	// Walk past leading allocas to find the insertion point
	Value inst = LLVMGetFirstInstruction(entry);
	while (inst && LLVMGetInstructionOpcode(inst) == LLVMAlloca)
		inst = LLVMGetNextInstruction(inst);

	// Position before the first non-alloca instruction (safe even when it's a terminator)
	if (inst) LLVMPositionBuilderBefore(builder, inst);
	else LLVMPositionBuilderAtEnd(builder, entry);

	token->llvm.elem = LLVMBuildAlloca(builder, type, token->name);

	LLVMPositionBuilderAtEnd(builder, current);
}

// get_llvm_type
TypeRef get_llvm_type(Token *token) {
	Type type = token->type;
	if (token->ret_type)     type = token->ret_type;
	if (type == STRUCT_DEF)  return token->llvm.struct_type;
	if (type == TUPLE)       return token->llvm.struct_type;
	if (type == STRUCT_CALL) {
		if (CHECK(!token->Struct.ptr,
		          "STRUCT_CALL: Struct.ptr is NULL for token '%s' type=%d ret_type=%d",
		          token->name ? token->name : "(null)", token->type, token->ret_type))
			return LLVMVoidTypeInContext(context);
		return get_llvm_type(token->Struct.ptr->token);
	}
	if (includes(type, ARRAY, ARRAY_TYPE, 0)) {
		TypeRef base;
		if (token->Array.sub_type == STRUCT_CALL && token->Array.struct_ptr)
			base = get_llvm_type(token->Array.struct_ptr->token);
		else {
			Token tmp = {.type = token->Array.sub_type};
			base      = get_llvm_type(&tmp);
		}
		return LLVMPointerType(base, 0); // flat allocation: always single ptr to base
	}
	// if (type == FCALL)
	//    return get_llvm_type(token->Fcall.ptr->token);
	TypeRef res[END] = {
	    [INT] = i32,  [CHAR] = i8,   [CHARS] = p8,  [BOOL] = i1,  [VOID] = vd,
	    [LONG] = i64, [FLOAT] = f32, [ACCESS] = i8, [SHORT] = i2, [NULLABLE] = p8,
	};
	if (CHECK(!res[type], "handle this case [%s]", to_string(type))) {
		SEG();
	}
	return res[type];
}

// _get_default_value
Value _get_default_value(Token *token) {
	TypeRef type = get_llvm_type(token);

	if (token->is_ref) return LLVMConstNull(LLVMPointerType(type, 0));

	if (includes(token->type, NUMERIC_TYPES, 0))     return LLVMConstInt(type, 0, false);
	if (token->type == FLOAT)                        return LLVMConstReal(type, 0.0);
	if (includes(token->type, CHARS, ARRAY_TYPE, 0)) return LLVMConstNull(type);
	CHECK(1, "handle this case %s", to_string(token->type));
	return NULL;
}

const char *type_to_ura_name(Type type) {
	switch (type) {
	case INT:   return "int";
	case LONG:  return "long";
	case SHORT: return "short";
	case CHAR:  return "char";
	case CHARS: return "chars";
	case BOOL:  return "bool";
	case FLOAT: return "float";
	case PTR:   return "pointer";
	case VOID:  return "void";
	default:    return NULL;
	}
}

//
Value build_binary_op(Type op, Value l, Value r) {
	char *name = to_string(op);
	if (is_float_value(l)) {
		if (includes(op, COMPARISON_OPS, 0))
			return LLVMBuildFCmp(builder, fcmp_predicate(op), l, r, name);
		switch (op) {
		case ADD: return LLVMBuildFAdd(builder, l, r, name);
		case SUB: return LLVMBuildFSub(builder, l, r, name);
		case MUL: return LLVMBuildFMul(builder, l, r, name);
		case DIV: return LLVMBuildFDiv(builder, l, r, name);
		default:  TODO(1, "build_binary_op: unhandled float op %s", name); return NULL;
		}
	}
	if (includes(op, COMPARISON_OPS, 0))
		return LLVMBuildICmp(builder, icmp_predicate(op), l, r, name);
	switch (op) {
	case ADD:    return LLVMBuildAdd(builder, l, r, name);
	case SUB:    return LLVMBuildSub(builder, l, r, name);
	case MUL:    return LLVMBuildMul(builder, l, r, name);
	case DIV:    return LLVMBuildSDiv(builder, l, r, name);
	case MOD:    return LLVMBuildSRem(builder, l, r, name);
	case AND:    return LLVMBuildAnd(builder, l, r, name);
	case OR:     return LLVMBuildOr(builder, l, r, name);
	case BAND:   return LLVMBuildAnd(builder, l, r, name);
	case BOR:    return LLVMBuildOr(builder, l, r, name);
	case BXOR:   return LLVMBuildXor(builder, l, r, name);
	case LSHIFT: return LLVMBuildShl(builder, l, r, name);
	case RSHIFT: return LLVMBuildAShr(builder, l, r, name);
	default:     TODO(1, "build_binary_op: unhandled op %s", name); return NULL;
	}
}

void _entry(Token *token) {
	Block entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
}

static int parse_escape_seq(char *input, int s, int e, char *buf, int *j) {
	switch (input[s + 1]) {
	case 'n':  buf[(*j)++] = '\n'; return s + 1;  // newline
	case 't':  buf[(*j)++] = '\t'; return s + 1;  // tab
	case 'r':  buf[(*j)++] = '\r'; return s + 1;  // carriage return
	case 'b':  buf[(*j)++] = '\b'; return s + 1;  // backspace
	case 'f':  buf[(*j)++] = '\f'; return s + 1;  // form feed
	case 'v':  buf[(*j)++] = '\v'; return s + 1;  // vertical tab
	case 'a':  buf[(*j)++] = '\a'; return s + 1;  // alert (bell)
	case '\\': buf[(*j)++] = '\\'; return s + 1; // backslash
	case '"':  buf[(*j)++] = '"'; return s + 1;   // double quote
	case '\'': buf[(*j)++] = '\''; return s + 1; // single quote
	case '?':  buf[(*j)++] = '\?'; return s + 1;  // question mark (trigraph)
	case '0':  {
		// three-digit octal: \0NN
		if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[(*j)++] = (char)octal;
				return s + 3;
			}
			buf[(*j)++] = '\0';
			return s + 1;
		}
		// two-digit octal: \0N
		else if (s + 1 < e && isdigit(input[s + 2])) {
			int octal   = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// plain null
		buf[(*j)++] = '\0';
		return s + 1;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
		// three-digit octal: \NNN
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[(*j)++] = (char)octal;
				return s + 3;
			}
			buf[(*j)++] = input[s];
			return s + 1; // invalid, keep backslash
		}
		// two-digit octal
		else if (s + 2 < e && isdigit(input[s + 2])) {
			int octal   = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// single-digit octal
		buf[(*j)++] = (char)(input[s + 1] - '0');
		return s + 1;
	}
	case 'x': // Hexadecimal: \xFF
	{
		if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3])) {
			int  hex = 0;
			char c1  = input[s + 2];
			char c2  = input[s + 3];
			if (c1 >= '0' && c1 <= '9')      hex += (c1 - '0') * 16;
			else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
			else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;
			if (c2 >= '0' && c2 <= '9')      hex += (c2 - '0');
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

static bool match_table(Token *new, char *name, Types *table, bool is_dec) {
	for (int i = 0; table[i].name; i++) {
		if (strcmp(table[i].name, name) == 0) {
			new->type   = table[i].type;
			new->is_dec = is_dec;
			if (is_dec) setName(new, NULL);
			return true;
		}
	}
	return false;
}

Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space) {
	Token *new    = new_token(type, space);
	new->line     = line;
	new->filename = filename;

	switch (type) {
	case INT: {
		while (s < e)
			new->Int.value = new->Int.value * 10 + input[s++] - '0';
		break;
	}
	case FLOAT: {
		char buf[64] = {0};
		int  len     = e - s;
		if (len > 63) len = 63;
		strncpy(buf, input + s, len);
		new->Float.value = (float)atof(buf);
		break;
	}
	case ID: {
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);

		struct {
			char *name;
			bool  value;
		} bools[] = {{"True", true}, {"False", false}, {0, 0}};
		for (int i = 0; bools[i].name; i++)
			if (strcmp(new->name, bools[i].name) == 0) {
				setName(new, NULL);
				new->type       = BOOL;
				new->Bool.value = bools[i].value;
				goto id_done;
			}

		Types data_types[] = {{"int", INT},     {"bool", BOOL}, {"chars", CHARS}, {"char", CHAR},
		                      {"float", FLOAT}, {"void", VOID}, {"long", LONG},   {"pointer", CHARS},
		                      {"short", SHORT}, {0, 0}};
		if (match_table(new, new->name, data_types, true)) goto id_done;

		Types keywords[] = {{"if", IF},         {"elif", ELIF},
		                    {"else", ELSE},     {"while", WHILE},
		                    {"fn", FDEC},       {"return", RETURN},
		                    {"break", BREAK},   {"continue", CONTINUE},
		                    {"ref", REF},       {"struct", STRUCT_DEF},
		                    {"enum", ENUM_DEF}, {"proto", PROTO},
		                    {"mod", MODULE},    {"operator", OPERATOR},
		                    {"as", AS},         {"pub", PUB},
		                    {"delete", DELETE}, {0, 0}};
		if (match_table(new, new->name, keywords, false)) goto id_done;

		Types keywords2[] = {{"and", AND},        {"or", OR},         {"is", EQUAL},
		                     {"not", NOT},        {"typeof", TYPEOF}, {"sizeof", SIZEOF},
		                     {"stack", STACK},    {"heap", HEAP},     {"array", ARRAY_TYPE},
		                     {"List", LIST_TYPE}, {"null", NULLABLE}, {0, 0}};
		if (match_table(new, new->name, keywords2, false)) setName(new, NULL);
	id_done:
		break;
	}
	case FDEC: {
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);
		break;
	}
	case CHARS: {
		if (e <= s) {
			new->Chars.value = allocate(1, sizeof(char));
			break;
		}
		int len          = e - s;
		new->Chars.value = allocate(len + 1, sizeof(char));
		char *value      = new->Chars.value;
		int   j          = 0;

		for (int i = 0; i < len && s < e; i++, s++) {
			if (s + 1 < e && input[s] == '\\') s = parse_escape_seq(input, s, e, value, &j);
			else value[j++] = input[s];
		}
		if (j < len) {
			char *resized = allocate(j + 1, sizeof(char));
			strcpy(resized, value);
			new->Chars.value = resized;
		}
		break;
	}
	case CHAR: {
		char value = 0;
		if (e == s + 1) value = input[s];
		else // special character
		{
			char escape_map[256] = {
			    ['n'] = '\n', ['t'] = '\t', ['r'] = '\r',  ['b'] = '\b', ['f'] = '\f',  ['v'] = '\v',
			    ['a'] = '\a', ['0'] = '\0', ['\\'] = '\\', ['"'] = '\"', ['\''] = '\'', ['?'] = '\?',
			};
			char c = escape_map[(unsigned char)input[s + 1]];
			CHECK(!c && input[s + 1] != '0', "unknown escape character");
			value = c;
		}
		new->Char.value = value;
		break;
	}
	default: break;
	}
	return new;
}

// ----------------------------------------------------------------------------
// Tokenizer
// ----------------------------------------------------------------------------
void tokenize(char *file_name, int default_space) {
	if (found_error) return;
	char *filename = realpath(file_name, NULL);
	File  file     = fopen(filename, "r");
	if (CHECK(!file, "openning %s", file_name)) return;
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *input = allocate((size + 1), sizeof(char));
	fread(input, size, sizeof(char), file);
	fclose(file);

	debug(GREEN("TOKENIZE: [%s]\n"), file_name);

	struct {
		char *value;
		Type  type;
	} specials[] = {
	    {"...", VARIADIC},  {".", DOT},         {"::", DOUBLE_DOTS}, {":", DOTS},
	    {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},  {"/=", DIV_ASSIGN},
	    {"%=", MOD_ASSIGN}, {"!=", NOT_EQUAL},  {"!", NOT},          {"==", EQUAL},
	    {"<<", LSHIFT},     {">>", RSHIFT},     {"<=", LESS_EQUAL},  {">=", GREAT_EQUAL},
	    {"<", LESS},        {">", GREAT},       {"=", ASSIGN},       {"+", ADD},
	    {"-", SUB},         {"*", MUL},         {"/", DIV},          {"%", MOD},
	    {"(", LPAR},        {")", RPAR},        {"[", LBRA},         {"]", RBRA},
	    {",", COMA},        {"&&", AND},        {"||", OR},          {"&", BAND},
	    {"|", BOR},         {"^", BXOR},        {"~", BNOT},         {0, (Type)0},
	};

	int  space    = default_space;
	int  line     = 1;
	bool new_line = true;

	resize_array(used_files, char *, usize, ucount);
	for (int i = 0; i < ucount; i++) {
		if (strcmp(file_name, used_files[i]) == 0) return;
	}
	used_files[ucount++] = file_name;

	for (int i = 0; input[i] && !found_error;) {
		int  s = i;
		char c = input[i];

		if (isspace(c)) {
			if (c == '\n') {
				line++;
				new_line = true;
				space    = default_space;
			} else if (new_line) space += (c == '\t') ? TAB : 1;
			i++;
			continue;
		}
		if (strncmp(input + i, "/*", 2) == 0) {
			i += 2;
			while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2)) {
				if (input[i] == '\n') line++;
				i++;
			}
			CHECK(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'");
			i += 2;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}

		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\') i++;
				i++;
			}
			CHECK(input[i++] != '\"', "expected '\"'");
			parse_token(file_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\')             i++;
			if (input[i] && input[i] != '\'') i++;
			CHECK(input[i++] != '\'', "expected '\''");
			parse_token(file_name, line, input, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				CHECK(input[i++] != '\"', "expected \" after use");
				int s = i;
				while (input[i] && input[i] != '\"')
					i++;
				CHECK(input[i++] != '\"', "expected \" after use <filename>");

				char *use = strndup(input + s, i - s - 1);
				char *tmp = strjoin(use, ".ura", NULL);
				free(use);
				use = tmp;
				if (use[0] == '@') {
					tmp = strjoin(ura_lib, "/", use + 1);
					free(use);
					use = tmp;
				} else if (use[0] != '/') {
					char *dir_copy = strdup(file_name);
					char *dir_name = dirname(dir_copy);
					tmp            = strjoin(dir_name, "/", use);
					free(dir_copy);
					free(use);
					use = tmp;
				}
				calling_use++;
				tokenize(use, space);
				calling_use--;
			} else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				CHECK(input[i++] != '\"', "expected \" after link");
				int s = i;
				while (input[i] && input[i] != '\"')
					i++;
				CHECK(input[i++] != '\"', "expected \" after link <filename>");
				char *raw = strndup(input + s, i - s - 1);
				// strip "@/" prefix: link "@/raylib" → "raylib" → env URA_LINK_raylib
				if (raw[0] == '@' && raw[1] == '/') {
					char *stripped = strdup(raw + 2);
					free(raw);
					raw = stripped;
				}
				for (int i = 0; i < lcount; i++)
					if (strcmp(links[i], raw) == 0) goto _continue;
				resize_array(links, char *, lsize, lcount);
				links[lcount++] = raw;
			} else parse_token(file_name, line, input, s, i, ID, space);
		_continue:
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(file_name, line, input, s, i, FLOAT, space);
			} else parse_token(file_name, line, input, s, i, INT, space);
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++) {
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0) {
				parse_token(file_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		CHECK(1, "Syntax error <%c>", c);
	}
	if (!calling_use) new_token(END, -1);
	free(input);
}

void tokenize_string(char *input, char *fake_name, int default_space) {
	if (found_error) return;
	debug(GREEN("TOKENIZE: [%s]\n"), fake_name);

	struct {
		char *value;
		Type  type;
	} specials[] = {
	    {"...", VARIADIC},  {".", DOT},         {"::", DOUBLE_DOTS}, {":", DOTS},
	    {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},  {"/=", DIV_ASSIGN},
	    {"%=", MOD_ASSIGN}, {"!=", NOT_EQUAL},  {"!", NOT},          {"==", EQUAL},
	    {"<<", LSHIFT},     {">>", RSHIFT},     {"<=", LESS_EQUAL},  {">=", GREAT_EQUAL},
	    {"<", LESS},        {">", GREAT},       {"=", ASSIGN},       {"+", ADD},
	    {"-", SUB},         {"*", MUL},         {"/", DIV},          {"%", MOD},
	    {"(", LPAR},        {")", RPAR},        {"[", LBRA},         {"]", RBRA},
	    {",", COMA},        {"&&", AND},        {"||", OR},          {"&", BAND},
	    {"|", BOR},         {"^", BXOR},        {"~", BNOT},         {0, (Type)0},
	};

	int  space    = default_space;
	int  line     = 1;
	bool new_line = true;

	for (int i = 0; input[i] && !found_error;) {
		int  s = i;
		char c = input[i];

		if (isspace(c)) {
			if (c == '\n') {
				line++;
				new_line = true;
				space    = default_space;
			} else if (new_line) space += (c == '\t') ? TAB : 1;
			i++;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}
		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\') i++;
				i++;
			}
			CHECK(input[i++] != '\"', "expected '\"'");
			parse_token(fake_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\')             i++;
			if (input[i] && input[i] != '\'') i++;
			CHECK(input[i++] != '\'', "expected '\''");
			parse_token(fake_name, line, input, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			parse_token(fake_name, line, input, s, i, ID, space);
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(fake_name, line, input, s, i, FLOAT, space);
			} else parse_token(fake_name, line, input, s, i, INT, space);
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++) {
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0) {
				parse_token(fake_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		CHECK(1, "Syntax error <%c>", c);
	}
	// No END token — generated code is prepended to user code
}

Node *expr_node() { return assign_node(); }

AST_NODE(assign_node, logic_or_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
         MOD_ASSIGN)
AST_NODE(logic_or_node, logic_and_node, OR)
AST_NODE(logic_and_node, bitor_node, AND)
AST_NODE(bitor_node, bitxor_node, BOR)
AST_NODE(bitxor_node, bitand_node, BXOR)
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
	while ((token = find(AS, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = unary_node();
		Token *to   = node->right->token;
		if (CHECK(to == NULL || !to->is_dec, "expected data type after as")) return syntax_error();
		to->is_dec = false;
		left       = node;
	}
	return left;
}

Node *unary_node() // - + (unary)
{
	Token *token;
	if ((token = find(ADD, SUB, 0))) {
		Node *node                    = new_node(token);
		node->left                    = unary_node();
		node->right                   = new_node(new_token(INT, token->space));
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
	while ((token = find(DOT, LBRA, DOUBLE_DOTS, 0))) {
		Node *node = new_node(token);
		switch (token->type) {
		case DOT: {
			node->left  = left;
			node->right = prime_node();

			CHECK(!node->right, "expected (attribute/method call) after .");
			if (node->right->token->type == FCALL) {
				Node *func                  = node->right;
				func->token->is_method_call = true;
				add_child(func->left, left);
				node->right = NULL;
				node->left  = NULL;
				node        = func;
			}
			break;
		}
		case LBRA: {
			node->token->type = ACCESS;
			node->left        = left;
			node->right       = expr_node();
			CHECK(!node->right, "expected something between []");
			EXPECT_TOKEN(RBRA, "expected right bracket");
			break;
		}
		case DOUBLE_DOTS: {
			// Foo::method() — static dispatch
			Node *call = prime_node();
			if (!call || call->token->type != FCALL) {
				CHECK(1, "expected function call after '::'");
				return syntax_error();
			}
			char *qname = strjoin(left->token->name, ".", call->token->name);
			setName(call->token, qname);
			free(qname);
			call->token->is_static_call = true;
			node                        = call;
			break;
		}
		default: {
			CHECK(1, "Unexpected %k line %d", token, token->line);
			return syntax_error();
		}
		}

		left = node;
	}
	return left;
}

static void params_node(Node *node);

static Node *var_dec_node(Token *token) {
	// Literal value: "hello", 1, 'c'
	if (token->type != ID && !token->is_dec && !token->name) return new_node(token);
	// int, char, chars, etc. declaration keyword
	if (token->is_dec) {
		if (CHECK(tokens[exe_count]->type == ID && tokens[exe_count]->line == token->line,
		          "Unexpected id after data type %k", tokens[exe_count]))
			return syntax_error();
		return new_node(token);
	}
	// TODO: this one should expected using expect

	// variable declaration: name type [ref] [= ...]
	if (token->type == ID && is_data_type(tokens[exe_count]) &&
	    tokens[exe_count]->line == token->line) {
		Token *tmp    = tokens[exe_count++]; // skip data type
		bool   is_ref = find(REF, 0) != NULL;
		if (includes(tmp->type, ARRAY_TYPE, LIST_TYPE, 0)) {
			Type container = tmp->type;
			EXPECT_TOKEN(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
			int depth = 1;
			while (find(LBRA, 0))
				depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			CHECK(!elem_type, "expected element type in %s type",
			      container == LIST_TYPE ? "list" : "array");
			for (int i = 0; i < depth; i++)
				EXPECT_TOKEN(RBRA, "expected ] in %s type", container == LIST_TYPE ? "list" : "array");
			if (container == LIST_TYPE) {
				// Rewrite to STRUCT_CALL pointing to __list_T
				const char *ename =
				    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
				char sname[256];
				snprintf(sname, sizeof(sname), "__list_%s", ename);
				for (int d = 1; d < depth; d++) {
					char tmp_name[256];
					snprintf(tmp_name, sizeof(tmp_name), "__list_%s", sname);
					snprintf(sname, sizeof(sname), "%s", tmp_name);
				}
				Node *list_st = get_struct(sname);
				if (CHECK(!list_st, "list struct '%s' not found", sname)) return syntax_error();
				tmp->type       = STRUCT_CALL;
				tmp->Struct.ptr = list_st;
			} else {
				tmp->Array.sub_type = elem_type->type;
				tmp->Array.depth    = depth;
				tmp->ret_type       = container;
				if (elem_type->type == ID) {
					Node *sd = get_struct(elem_type->name);
					if (sd) {
						tmp->Array.sub_type   = STRUCT_CALL;
						tmp->Array.struct_ptr = sd;
					}
				}
			}
		}
		setName(tmp, token->name);
		tmp->is_dec = true;
		tmp->is_ref = is_ref;
		// Tuple unpack: a int, b chars = foo()
		if (!is_ref && tokens[exe_count]->type == COMA && exe_count + 2 < tokens_count &&
		    tokens[exe_count + 1]->type == ID && is_data_type(tokens[exe_count + 2])) {
			Token *tu_tok = new_token(TUPLE_UNPACK, token->space);
			Node  *tu     = new_node(tu_tok);
			add_child(tu, new_node(tmp)); // first decl
			while (tokens[exe_count]->type == COMA) {
				exe_count++; // consume ,
				Token *nname = find(ID, 0);
				Token *ntype = find(DATA_TYPES, 0);
				if (includes(ntype->type, ARRAY_TYPE, LIST_TYPE, 0)) {
					Type container = ntype->type;
					EXPECT_TOKEN(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
					int depth = 1;
					while (find(LBRA, 0))
						depth++;
					Token *elem_type = find(DATA_TYPES, ID, 0);
					CHECK(!elem_type, "expected element type in %s type",
					      container == LIST_TYPE ? "list" : "array");
					for (int i = 0; i < depth; i++)
						EXPECT_TOKEN(RBRA, "expected ] in %s type",
						             container == LIST_TYPE ? "list" : "array");
					if (container == LIST_TYPE) {
						const char *ename =
						    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
						char sname[256];
						snprintf(sname, sizeof(sname), "__list_%s", ename);
						for (int d = 1; d < depth; d++) {
							char tn[256];
							snprintf(tn, sizeof(tn), "__list_%s", sname);
							snprintf(sname, sizeof(sname), "%s", tn);
						}
						Node *list_st = get_struct(sname);
						if (CHECK(!list_st, "list struct '%s' not found", sname)) return syntax_error();
						ntype->type       = STRUCT_CALL;
						ntype->Struct.ptr = list_st;
					} else {
						ntype->Array.sub_type = elem_type->type;
						ntype->Array.depth    = depth;
						ntype->ret_type       = container;
						if (elem_type->type == ID) {
							Node *sd = get_struct(elem_type->name);
							if (sd) {
								ntype->Array.sub_type   = STRUCT_CALL;
								ntype->Array.struct_ptr = sd;
							}
						}
					}
				}
				setName(ntype, nname->name);
				ntype->is_dec = true;
				add_child(tu, new_node(ntype));
			}
			EXPECT_TOKEN(ASSIGN, "expected = in tuple unpack");
			tu->left = expr_node();
			return tu;
		}
		return new_node(tmp);
	}

	// variable declaration (struct type): name StructName [ref] [= ...]
	Node *st_dec = NULL;
	bool  cond   = token->type == ID && tokens[exe_count]->type == ID;
	if (cond && (st_dec = get_struct(tokens[exe_count]->name))) {
		find(ID, 0); // skip struct data type
		bool is_ref = find(REF, 0) != NULL;
		if (is_ref) {
			EXPECT_TOKEN(ASSIGN,
			             "'%s': ref must be initialized at "
			             "declaration, e.g: x int ref = a",
			             token->name);
			exe_count--;
		}
		token->type       = STRUCT_CALL;
		token->is_dec     = true;
		token->is_ref     = is_ref;
		token->Struct.ptr = st_dec;
		return new_node(token);
	}

	// function call or main declaration
	if (token->type == ID && find(LPAR, 0)) {
		Node *node = new_node(token);
		if (strcmp(token->name, "main") == 0) {
			enter_scope(node);
			node->token->type     = FDEC;
			node->token->ret_type = INT;

			params_node(node);
			if (found_error) return syntax_error();

			EXPECT_TOKEN(DOTS, "expected : after main() declaration");

			Node *last = NULL;
			while (within(node->token->space)) {
				last = expr_node();
				add_child(node, last);
			}
			if (!last || last->token->type != RETURN) {
				last       = new_node(new_token(RETURN, node->token->space + TAB));
				last->left = new_node(new_token(INT, node->token->space + TAB));
				add_child(node, last);
			}

			exit_scope();
			return node;
		} else if (strcmp(token->name, "output") == 0) {
			node              = new_node(token);
			node->token->type = OUTPUT;
			node->left        = new_node(new_token(ARGS, token->space));
			Token *end_tok    = NULL;
			while (!found_error && !(end_tok = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			if (CHECK(!found_error && end_tok->type != RPAR, "output: expected )"))
				return syntax_error();
			node->token->ret_type = VOID;
			return node;
		} else {
			node->token->type = FCALL;
			Token *arg        = NULL;
			node->left        = new_node(new_token(ARGS, node->token->space));

			while (!found_error && !(arg = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			CHECK(!found_error && arg->type != RPAR, "expected ) after function call");
		}
		return node;
	}
	// just an ID
	if (token->type == ID) return new_node(token);
	CHECK(1, "Unexpected token has type %k line %d", tokens[exe_count], tokens[exe_count]->line);
	return syntax_error();
}

static Node *fdec_node(Token *token);

static Node *struct_def_node(Token *token) {
	Node  *node = new_node(token);
	Token *st_name;
	if (CHECK(!(st_name = find(ID, 0)), "expected identifier after struct definition")) return NULL;
	EXPECT_TOKEN(DOTS, "expected dots after struct definition\n");

	setName(node->token, st_name->name);
	node->token->type       = STRUCT_DEF;
	node->token->Struct.ptr = node;
	new_struct(node);
	enter_scope(node);

	while (within(node->token->space)) {
		// pub fn — static method, no self injected
		if (find(PUB, 0)) {
			Token *fdec_tok = find(FDEC, 0);
			if (!fdec_tok) {
				CHECK(1, "struct '%s': expected fn after pub", node->token->name);
				exit_scope();
				return syntax_error();
			}
			fdec_tok->is_pub = true; // set before fdec_node so self is NOT injected
			Node *child      = fdec_node(fdec_tok);
			if (!child || child->token->type != FDEC) {
				CHECK(1, "struct '%s': expected fn after pub", node->token->name);
				exit_scope();
				return syntax_error();
			}
			child->token->is_pub = true;
			char *qualified      = strjoin(node->token->name, ".", child->token->name);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
			continue;
		}
		Token *op_kw;
		if ((op_kw = find(OPERATOR, 0))) {
			Token *op_tok = find(ASSIGN, ADD, SUB, MUL, DIV, MOD, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
			                     DIV_ASSIGN, MOD_ASSIGN, EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL,
			                     GREAT_EQUAL, AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, DELETE, 0);
			// Check for named operators: operator output
			if (!op_tok && tokens[exe_count]->type == ID &&
			    strcmp(tokens[exe_count]->name, "output") == 0) {
				op_tok       = tokens[exe_count++];
				op_tok->type = OUTPUT;
			}
			if (CHECK(!op_tok, "struct '%s': expected operator symbol after 'operator'",
			          node->token->name)) {
				exit_scope();
				return syntax_error();
			}
			// operator delete() void: — destructor
			if (op_tok->type == DELETE) {
				Token *fdec_tok = new_token(FDEC, op_kw->space);
				setName(fdec_tok, "delete");
				Node *child = fdec_node(fdec_tok);
				if (CHECK(child->token->ret_type != VOID,
				          "struct '%s': operator delete must return void", node->token->name)) {
					exit_scope();
					return syntax_error();
				}
				if (CHECK(child->left->children_count != 1,
				          "struct '%s': operator delete takes no parameters", node->token->name)) {
					exit_scope();
					return syntax_error();
				}
				char *qualified = strjoin(node->token->name, ".delete", "");
				setName(child->token, qualified);
				free(qualified);
				node->token->has_clean = true;
				new_function(child);
				continue;
			}
			// operator output() — custom printing
			if (op_tok->type == OUTPUT) {
				Token *fdec_tok = new_token(FDEC, op_kw->space);
				setName(fdec_tok, "output");
				Node *child     = fdec_node(fdec_tok);
				char *qualified = strjoin(node->token->name, ".operator.output", "");
				setName(child->token, qualified);
				free(qualified);
				new_function(child);
				continue;
			}
			char  *op_base  = strjoin("operator.", to_string(op_tok->type), "");
			Token *fdec_tok = new_token(FDEC, op_kw->space);
			setName(fdec_tok, op_base);
			free(op_base);
			Node *child = fdec_node(fdec_tok);
			// suffix with first param type for unique overload names
			// e.g. "operator.ASSIGN.CHARS" or "operator.ASSIGN.String"
			char *op_named;
			if (child->left && child->left->children_count >= 2) {
				Token *p      = child->left->children[0]->token;
				char  *suffix = (p->type == STRUCT_CALL && p->Struct.ptr) ? p->Struct.ptr->token->name
				                                                          : to_string(p->type);
				op_named      = strjoin(child->token->name, ".", suffix);
			} else {
				op_named = strjoin(child->token->name, "", "");
			}
			char *qualified = strjoin(node->token->name, ".", op_named);
			free(op_named);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
			continue;
		}
		Node *child = prime_node();
		if (!child)                     return syntax_error();
		if (child->token->type == FDEC) {
			resize_array(child->functions, Node *, child->functions_size, child->functions_count);
			char *qualified = strjoin(node->token->name, ".", child->token->name);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
		} else {
			if (CHECK(!child->token->is_dec, "invalid attribute")) {
				exit_scope();
				return syntax_error();
			}
			child->token->is_dec = false;
			add_child(node, child);
		}
	}
	exit_scope();
	return node;
}

static Node *enum_def_node(Token *token) {
	Token *ename;
	if (CHECK(!(ename = find(ID, 0)), "expected identifier after enum")) return NULL;
	if (CHECK(!find(DOTS, 0), "expected ':' after enum name"))           return NULL;

	setName(token, ename->name);
	token->type = ENUM_DEF;
	Node *node  = new_node(token);

	int   counter = 0;
	do {
		if (!within(token->space)) break;
		Token *var = find(ID, 0);
		if (!var) break;
		var->type      = INT;
		var->is_global = true;
		var->is_dec    = true;
		var->Int.value = counter++;
		new_variable(var);
		add_child(node, new_node(var));
	} while (find(COMA, 0));

	Token *next = tokens[exe_count];
	if (next->type == ID && within(next->space))
		CHECK(1, "enum '%s': expected ',' before '%s'", token->name, next->name);

	return node;
}

static Node *if_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	node->left = expr_node();
	EXPECT_TOKEN(DOTS, "expected : after if condition");

	while (within(node->token->space))
		add_child(node, expr_node());

	Node *curr = node;
	while (includes(tokens[exe_count]->type, ELSE, ELIF, 0) && within(node->token->space - TAB)) {
		Token *tok = find(ELSE, ELIF, 0);

		curr->right = new_node(tok);
		curr        = curr->right;

		if (tok->type == ELIF) {
			enter_scope(curr);
			curr->left = expr_node();
			EXPECT_TOKEN(DOTS, "expected : after elif condition");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
		} else if (tok->type == ELSE) {
			enter_scope(curr);
			EXPECT_TOKEN(DOTS, "expected : after else");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
			break;
		}
	}
	exit_scope();
	return node;
}

// Like EXPECT_TOKEN but for void-returning helpers. On failure: signals
// syntax_error() and returns from the enclosing function with no value.
#define EXPECT_TOKEN_VOID(type, fmt, ...)                                                          \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			CHECK(1, fmt, ##__VA_ARGS__);                                                             \
			syntax_error();                                                                           \
			return;                                                                                   \
		}                                                                                            \
	}

// Parse a function's parameter list (caller has already consumed `(`). Walks
// up to and including the closing `)`. On any error, sets found_error via
// syntax_error() and returns. Used by fdec_node and the `main` declaration.
static void params_node(Node *node) {
	node->left = new_node(new_token(ARGS, node->token->space));

	Token *last;
	while (!found_error && !(last = find(RPAR, END, 0))) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			last                     = find(RPAR, 0);
			CHECK(!last, "expected ) after function ... in variadic function");
			break;
		}

		Token *name = find(ID, 0);
		if (CHECK(!name, "expected identifier in function argument %s", node->token->name)) {
			syntax_error();
			return;
		}

		Token *data_type = find(DATA_TYPES, ID, 0);
		bool   is_ref    = find(REF, 0) != NULL;
		if (CHECK(!data_type, "expected data type in function argument")) break;

		if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
			Type container = data_type->type;
			EXPECT_TOKEN_VOID(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
			int depth = 1;
			while (find(LBRA, 0)) depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			CHECK(!elem_type, "expected element type in %s type",
			      container == LIST_TYPE ? "list" : "array");
			for (int j = 0; j < depth; j++)
				EXPECT_TOKEN_VOID(RBRA, "expected ] in %s type",
				                  container == LIST_TYPE ? "list" : "array");
			if (container == LIST_TYPE) {
				const char *ename =
				    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
				char sname[256];
				snprintf(sname, sizeof(sname), "__list_%s", ename);
				for (int d = 1; d < depth; d++) {
					char tn[256];
					snprintf(tn, sizeof(tn), "__list_%s", sname);
					snprintf(sname, sizeof(sname), "%s", tn);
				}
				Node *list_st = get_struct(sname);
				if (CHECK(!list_st, "list struct '%s' not found", sname)) break;
				data_type->type       = STRUCT_CALL;
				data_type->Struct.ptr = list_st;
				setName(data_type, sname);
			} else {
				data_type->Array.sub_type = elem_type->type;
				data_type->Array.depth    = depth;
				data_type->ret_type       = container;
				if (elem_type->type == ID) {
					Node *sd = get_struct(elem_type->name);
					if (sd) {
						data_type->Array.sub_type   = STRUCT_CALL;
						data_type->Array.struct_ptr = sd;
					}
				}
			}
		}
		if (data_type->type == ID) {
			Node *to_find = get_struct(data_type->name);
			if (to_find) data_type->type = STRUCT_CALL;
		}

		Node *curr;
		if (data_type->type == STRUCT_CALL) {
			Node *st = get_struct(data_type->name);
			if (CHECK(!st, "Unknown struct type '%s'", data_type->name)) break;
			data_type->Struct.ptr = st;
			curr                  = new_node(data_type);
			data_type->is_ref     = is_ref;
			setName(data_type, name->name);
		} else {
			curr         = new_node(name);
			name->is_ref = is_ref;
			name->type   = data_type->type;
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				name->ret_type         = data_type->type;
				name->Array.sub_type   = data_type->Array.sub_type;
				name->Array.depth      = data_type->Array.depth;
				name->Array.struct_ptr = data_type->Array.struct_ptr;
			}
		}
		curr->token->is_dec   = true;
		curr->token->is_param = true;
		add_child(node->left, curr);

		if (tokens[exe_count]->type != RPAR) EXPECT_TOKEN_VOID(COMA, "expected coma");
	}
	CHECK(!found_error && last && last->type != RPAR, "expected ) after function declaration");
}

static Node *fdec_node(Token *token) {
	Node *node = new_node(token);
	if (!node->token->name || strcmp(node->token->name, "fn") == 0) {
		Token *fname = find(ID, 0);
		if (CHECK(!fname, "expected identifier after fn declaration")) return syntax_error();
		setName(node->token, fname->name);
	}

	// Check if we're inside a struct scope (method)
	Node *struct_owner = NULL;
	Node *parent       = scopes[scopes_count];
	if (parent && parent->token->type == STRUCT_DEF) struct_owner = parent;

	enter_scope(node);
	EXPECT_TOKEN(LPAR, "expected ( after function declaration");
	params_node(node);
	if (found_error) return syntax_error();

	// If inside a struct, prepend implicit 'self' parameter (skip for pub/static fns)
	if (struct_owner && !node->token->is_pub) {
		Token *self_tok = new_token(STRUCT_CALL, node->token->space);
		setName(self_tok, "self");
		self_tok->Struct.ptr = struct_owner;
		self_tok->is_dec     = true;
		self_tok->is_ref     = true;
		add_child(node->left, new_node(self_tok));
	}

	if (tokens[exe_count]->type == LPAR) {
		// Tuple return type: fn foo(…) (int, chars):
		exe_count++; // consume (
		node->token->ret_type = TUPLE;
		int tc                = 0;
		while (tokens[exe_count]->type != RPAR && tc < 8) {
			if (includes(tokens[exe_count]->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				Token *t         = tokens[exe_count++];
				Type   container = t->type;
				EXPECT_TOKEN(LBRA, "expected [ in tuple %s type",
				             container == LIST_TYPE ? "list" : "array");
				Token *et = find(DATA_TYPES, ID, 0);
				if (CHECK(!et, "expected element type in tuple %s",
				          container == LIST_TYPE ? "list" : "array"))
					return syntax_error();
				EXPECT_TOKEN(RBRA, "expected ] in tuple %s type",
				             container == LIST_TYPE ? "list" : "array");
				if (container == LIST_TYPE) {
					const char *ename = et->type == ID ? et->name : type_to_ura_name(et->type);
					char        sname[256];
					snprintf(sname, sizeof(sname), "__list_%s", ename);
					Node *list_st = get_struct(sname);
					if (CHECK(!list_st, "list struct '%s' not found", sname)) return syntax_error();
					t->type       = STRUCT_CALL;
					t->Struct.ptr = list_st;
				} else {
					t->Array.sub_type = et->type;
					t->ret_type       = container;
					if (et->type == ID) {
						Node *sd = get_struct(et->name);
						if (sd) {
							t->Array.sub_type   = STRUCT_CALL;
							t->Array.struct_ptr = sd;
						}
					}
				}
				node->token->Tuple.types[tc++] = t;
			} else if (tokens[exe_count]->type == ID) {
				Token *t  = tokens[exe_count++];
				Node  *st = get_struct(t->name);
				if (CHECK(!st, "unknown type '%s' in tuple", t->name)) return syntax_error();
				t->type                        = STRUCT_CALL;
				t->Struct.ptr                  = st;
				node->token->Tuple.types[tc++] = t;
			} else if (is_data_type(tokens[exe_count])) {
				node->token->Tuple.types[tc++] = tokens[exe_count++];
			} else {
				CHECK(1, "unexpected token in tuple return type");
				return syntax_error();
			}
			if (tokens[exe_count]->type == COMA) exe_count++;
		}
		EXPECT_TOKEN(RPAR, "expected ) after tuple return types");
		node->token->Tuple.count = tc;
	} else {
		if (CHECK(!is_data_type(tokens[exe_count]) && tokens[exe_count]->type != ID,
		          "expected data type after fun declaration"))
			return syntax_error();
		Token *ret_tok = tokens[exe_count++];
		if (ret_tok->type == ID) {
			Node *st = get_struct(ret_tok->name);
			if (CHECK(!st, "unknown return type '%s'", ret_tok->name)) return syntax_error();
			node->token->ret_type   = STRUCT_CALL;
			node->token->Struct.ptr = st;
		} else node->token->ret_type = ret_tok->type;
	}
	node->token->is_ref = find(REF, 0) != NULL;

	if (!node->token->is_proto) {
		Token *next = find(DOTS, 0);
		if (CHECK(!found_error && !next, "expected : after function declaration"))
			return syntax_error();
		Node *child = NULL;

		if (next->type == DOTS)
			while (within(node->token->space))
				child = add_child(node, expr_node());
		else {
			Token *retToken = copy_token(next);
			retToken->type  = RETURN;
			Node *retNode   = new_node(retToken);
			retNode->left   = expr_node();
			child           = add_child(node, retNode);
		}
		if (next->type == DOTS) {
			if (node->token->ret_type != VOID)
				CHECK(!child || child->token->type != RETURN, "expected return statement %s",
				      node->token->name);
			else {
				Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
				ret->left = new_node(new_token(VOID, node->token->space + TAB));
				add_child(node, ret);
			}
		}
	}
	exit_scope();
	return node;
}

static Node *return_node(Token *token) {
	// TODO: check if return type is compatible with function in current scope (done in gen_ir)
	Node *node = new_node(token);
	for (int i = scopes_count; i >= 0; i--) {
		Node *curr = scopes[i];
		if (curr->token->type == FDEC) {
			if (curr->token->ret_type == VOID) {
				node->left              = copy_node(node);
				node->left->token->type = VOID;
			} else if (curr->token->ret_type == TUPLE) {
				add_child(node, expr_node()); // first value
				while (tokens[exe_count]->type == COMA) {
					exe_count++;
					add_child(node, expr_node());
				}
			} else node->left = expr_node();
			break;
		}
	}
	return node;
}

// ----------------------------------------------------------------------------
// Primary expression parser
// ----------------------------------------------------------------------------
Node *prime_node() {
	Node  *node = NULL;
	Token *token;

	if (find(PUB, 0)) {
		CHECK(1, "'pub' is only valid inside struct definitions");
		return syntax_error();
	}
	if ((token = find(ID, DATA_TYPES, 0))) return var_dec_node(token);
	if ((token = find(MODULE, 0)))         {
		Token *id = find(ID, 0);
		CHECK(!id, "expect module identifier after 'mod'");
		EXPECT_TOKEN(DOTS, "expect DOTs after mod name");
		setName(token, id->name);
		node = new_node(token);
		enter_scope(node);
		while (within(token->space)) {
			Node  *curr   = prime_node();
			Token *ctoken = curr->token;
			CHECK(!includes(ctoken->type, STRUCT_DEF, FDEC, 0) && !ctoken->is_dec,
			      "only functions declaration"
			      "are allowed within module");
			add_child(node, curr);
		}
		exit_scope();

		return node;
	}
	if ((token = find(STRUCT_DEF, 0))) return struct_def_node(token);
	if ((token = find(ENUM_DEF, 0)))   return enum_def_node(token);
	if ((token = find(IF, 0)))         return if_node(token);

	if ((token = find(WHILE, 0))) {
		// while layout:
		//    + left    : condition
		//    + children: code bloc
		node = new_node(token);
		enter_scope(node);
		node->left = expr_node();
		EXPECT_TOKEN(DOTS, "expected : after if condition");
		while (within(node->token->space))
			add_child(node, expr_node());
		exit_scope();
		return node;
	}
	if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);

	if ((token = find(STACK, HEAP, 0))) {
		node = new_node(token);
		EXPECT_TOKEN(LBRA, "expected [ after stack");

		int depth = 1;
		while (find(LBRA, 0))
			depth++;

		Token *elem_type = find(DATA_TYPES, ID, 0);
		CHECK(!elem_type, "expected element type in stack");

		for (int i = 0; i < depth; i++)
			EXPECT_TOKEN(RBRA, "expected ] in stack type");

		EXPECT_TOKEN(LPAR, "expected ( after stack[type]");

		node->token->ret_type       = ARRAY;
		node->token->Array.sub_type = elem_type->type;
		node->token->Array.depth    = depth;
		if (elem_type->type == ID) {
			Node *sd = get_struct(elem_type->name);
			if (sd) {
				node->token->Array.sub_type   = STRUCT_CALL;
				node->token->Array.struct_ptr = sd;
			}
		}

		for (int i = 0; i < depth; i++) {
			add_child(node, expr_node());
			if (i < depth - 1) EXPECT_TOKEN(COMA, "expected , between dimensions");
		}
		EXPECT_TOKEN(RPAR, "expected ) after stack size");
		return node;
	}
	if ((token = find(TYPEOF, SIZEOF, 0))) {
		char *msg  = token->type == TYPEOF ? "typeof" : "sizeof";
		Type  type = token->type == TYPEOF ? CHARS : INT;
		node       = new_node(token);
		EXPECT_TOKEN(LPAR, "%s: expected (", msg);
		node->left = prime_node();
		EXPECT_TOKEN(RPAR, "%s: expected )", msg);
		node->token->ret_type = type;
		return node;
	}
	if ((token = find(PROTO, 0))) {
		if (includes(tokens[exe_count]->type, FDEC, STRUCT_DEF, 0))
			tokens[exe_count]->is_proto = true;
		else CHECK(1, "expected <fn> or <struct> after proto");
		return expr_node();
	}

	if ((token = find(FDEC, 0)))   return fdec_node(token);
	if ((token = find(RETURN, 0))) return return_node(token);

	// ref expr — take address of variable
	if ((token = find(REF, 0))) {
		node       = new_node(token);
		node->left = prime_node();
		return node;
	}
	if ((token = find(NOT, 0))) {
		node       = new_node(token);
		node->left = expr_node();
		return node;
	}
	if ((token = find(BNOT, 0))) {
		node       = new_node(token);
		node->left = prime_node();
		return node;
	}
	if ((token = find(NULLABLE, 0))) return new_node(token);
	// Array literal: [expr, expr, ...]
	if ((token = find(LBRA, 0))) {
		token->type = ARRAY_LIT;
		node        = new_node(token);
		if (tokens[exe_count]->type != RBRA) {
			add_child(node, expr_node());
			while (find(COMA, 0))
				add_child(node, expr_node());
		}
		EXPECT_TOKEN(RBRA, "expected ] to close array literal");
		return node;
	}
	if ((token = find(LPAR, 0))) {
		if (tokens[exe_count]->type != RPAR) node = expr_node();
		EXPECT_TOKEN(RPAR, "expected right )");
		return node;
	}
	debug("%k\n", tokens[exe_count - 1]);
	CHECK(1, "Unexpected token has type %s line %d", to_string(tokens[exe_count]->type),
	      tokens[exe_count]->line);
	return syntax_error();
}

Node *optimize_ir(Node *node, bool *changed) {
	// TODO: remove unsed attributes
	if (!node || found_error) return NULL;

	node->left  = optimize_ir(node->left, changed);
	node->right = optimize_ir(node->right, changed);

	if (node->children_count) {
		int    j            = 0;
		Node **new_children = allocate(node->children_size, sizeof(Node *));
		for (int i = 0; i < node->children_count; i++) {
			Node *child = optimize_ir(node->children[i], changed);
			if (child) new_children[j++] = child;
		}
		node->children_count = j;
		node->children       = new_children;
	}

	if (!node->token) return node;
	switch (node->token->type) {
	case FDEC: case PROTO: {
		if (node->token->used == 0 && strcmp(node->token->name, "main") != 0) {
			debug(RED("remove %k\n"), node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	// TODO: unused var/op/assign removal needs fixing — too aggressive, breaks 26 tests
	// Re-enable after fixing the `used` tracking in gen_ir
	default: break;
	}

	return node;
}

// ----------------------------------------------------------------------------
// gen_ir helpers — extracted from large switch cases
// ----------------------------------------------------------------------------

static void ir_method_call(Node *node) {
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
	if (!func)               return;
	if (func->token->is_pub) {
		CHECK(1, "'%s' is a pub fn — use '::'", node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);

	// first arg already gen_ir'd (obj), do rest
	obj_node->token->used++;
	Node *dec_args = func->left;
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++) {
		Node *carg = node->left->children[i];
		gen_ir(carg);
		if (CHECK(carg->token->type == ID, "Undeclared variable %s", carg->token->name)) break;
		carg->token->used++;

		Token *src = carg->token;
		if (i < dec_args->children_count - 1) { // -1: last dec_arg is implicit self
			Type param_type   = dec_args->children[i]->token->type;
			Type arg_type     = src->type;
			bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
			bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
			if (param_is_int && arg_is_int && param_type != arg_type) {
				Token *as_tok           = new_token(AS, src->space);
				as_tok->ret_type        = param_type;
				Node *tgt_node          = new_node(new_token(param_type, src->space));
				Node *as_node_          = new_node(as_tok);
				as_node_->left          = carg;
				as_node_->right         = tgt_node;
				node->left->children[i] = as_node_;
			} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
				CHECK(1, "'%s' argument %d: cannot pass %s as %s", node->token->name, i + 1,
				      to_string(arg_type), to_string(param_type));
				break;
			}
		}
	}
	free_node(func);
	set_ret_type(node);
}

static void ir_regular_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);

	Node *call_args = node->left;
	Node *dec_args  = func->left;
	// TODO: check number of arguments
	for (int i = 0; !found_error && call_args && i < call_args->children_count; i++) {
		Node *carg = call_args->children[i];
		gen_ir(carg);
		carg->token->used++;
		Token *src = carg->token;
		if (CHECK(src->type == ID, "Undeclared variable %s", carg->token->name)) break;

		if (i < dec_args->children_count) {
			bool param_is_ref = dec_args->children[i]->token->is_ref;
			if (param_is_ref)
				if (CHECK(src->type == FCALL || (src->type != DOT && src->type != ACCESS && !src->name),
				          "'%s': ref parameter requires a named variable",
				          dec_args->children[i]->token->name))
					break;

			Type param_type   = dec_args->children[i]->token->type;
			Type arg_type     = src->type;
			bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
			bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
			if (param_is_int && arg_is_int && param_type != arg_type) {
				Token *as_tok          = new_token(AS, src->space);
				as_tok->ret_type       = param_type;
				Node *tgt_node         = new_node(new_token(param_type, src->space));
				Node *as_node          = new_node(as_tok);
				as_node->left          = carg;
				as_node->right         = tgt_node;
				call_args->children[i] = as_node;
			} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
				CHECK(1, "'%s' argument %d: cannot pass %s as %s — use 'value as %s'",
				      node->token->name, i + 1, to_string(arg_type), to_string(param_type),
				      to_string(param_type));
				break;
			}
		}
	}
	free_node(func);
	set_ret_type(node);
}

static void ir_dot(Node *node) {
	gen_ir(node->left);
	if (found_error) return;
	node->left->token->used++;
	// Also mark the struct definition itself as used
	if (node->left->token->Struct.ptr) node->left->token->Struct.ptr->token->used++;
	Type  ret_type = 0;

	Node *src = NULL;
	if (node->left->token->type == STRUCT_CALL)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else if (node->left->token->ret_type == STRUCT_CALL && node->left->token->Struct.ptr)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else {
		CHECK(1, "handle this case %s", to_string(node->left->token->type));
		return;
	}

	bool found = false;
	for (int i = 0; src && i < src->children_count; i++) {
		Node *src_child = src->children[i];
		if (strcmp(src_child->token->name, node->right->token->name) == 0) {
			found                            = true;
			ret_type                         = src_child->token->type;
			node->right->token->Struct.index = i;
			node->right->token->type         = src_child->token->type;
			node->right->token->ret_type     = src_child->token->ret_type;
			if (src_child->token->type == STRUCT_CALL)
				node->right->token->Struct.ptr = src_child->token->Struct.ptr;
			else if (src_child->token->type == ARRAY_TYPE) {
				node->right->token->Array.sub_type   = src_child->token->Array.sub_type;
				node->right->token->Array.depth      = src_child->token->Array.depth;
				node->right->token->Array.struct_ptr = src_child->token->Array.struct_ptr;
			}
			break;
		}
	}
	if (CHECK(!found, "")) {
		debug(RED("%k doesn't have attribute %k\n"), node->left->token, node->right->token);
		return;
	}

	node->token->ret_type = ret_type;
	if (ret_type == STRUCT_CALL)     node->token->Struct.ptr = node->right->token->Struct.ptr;
	else if (ret_type == ARRAY_TYPE) {
		node->token->Array.sub_type   = node->right->token->Array.sub_type;
		node->token->Array.depth      = node->right->token->Array.depth;
		node->token->Array.struct_ptr = node->right->token->Array.struct_ptr;
	}
}

// ----------------------------------------------------------------------------
// Semantic analysis / name resolution pass
// ----------------------------------------------------------------------------

Node *find_function(char *name); // forward decl — defined after gen_ir

void  gen_ir(Node *node) {
	 if (found_error) return;
   Node *left  = node->left;
	 Node *right = node->right;
	 switch (node->token->type) {

	 case ID: {
		 Token *find = get_variable(node->token->name);
		 if (find) node->token = find;
      break;
   }

	 case STRUCT_CALL: {
		 Node *src               = get_struct(node->token->Struct.ptr->token->name);
		 node->token->Struct.ptr = src;
		 if (node->token->is_dec) {
			 if (scopes_count == 1) {
				 node->token->is_global = true;
         } else src->token->used++; // local declaration → always emit struct def
			 new_variable(node->token);
      }
		 break;
   }

	 case NULLABLE: {
		 node->token->ret_type = CHARS; // null is a null chars (i8*) pointer
		 node->token->used++;
		 break;
   }
	 case INT:   case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
	 case CHARS: case PTR: case VOID: case SHORT: {
		 if (node->token->is_dec) {
			 if (scopes_count == 1) {
				 node->token->is_global = true;
         }
			 new_variable(node->token);
      } else node->token->used++;
		 break;
   }
	 case TUPLE_UNPACK: {
		 // Register each LHS variable, then process RHS
      for (int i = 0; i < node->children_count; i++) {
			 Node *lhs = node->children[i];
			 if (lhs->token->is_dec) {
				 if (scopes_count == 1) lhs->token->is_global = true;
            new_variable(lhs->token);
         }
      }
      gen_ir(node->left); // RHS function call
      break;
   }
	 case ASSIGN: {
		 // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      // Struct operator overload: check for StructName.operator.ASSIGN.PARAMTYPE
      Type lt = left->token->ret_type ? left->token->ret_type : left->token->type;
      if (lt == STRUCT_CALL && left->token->Struct.ptr) {
			 Type  rt           = right->token->ret_type ? right->token->ret_type : right->token->type;
			 char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
			                          ? right->token->Struct.ptr->token->name
			                          : to_string(rt);
			 char *op_base =
			     strjoin(left->token->Struct.ptr->token->name, ".operator.", to_string(ASSIGN));
			 char *op_name = strjoin(op_base, ".", param_suffix);
			 free(op_base);
			 Node *func = find_function(op_name);
			 free(op_name);
			 if (func) {
				 node->token->Fcall.ptr = func;
				 func->token->used++;
				 break;
         }
      }
      if (!compatible(left->token, right->token)) {
			 CHECK(1, "type mismatch: cannot assign %k to %k — use '%k as %s'", right->token,
			       left->token, right->token, to_string(left->token->type));
			 break;
      }
      node->token->ret_type = left->token->ret_type;
      break;
   }
	 case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	 case MOD_ASSIGN: {
		 gen_ir(left);
		 gen_ir(right);
		 node->token->ret_type = left->token->ret_type;
		 node->token->used++;
		 node->left->token->used++;
		 node->right->token->used++;
		 // Struct compound-assign operator overload dispatch
		 {
			 Type lt = left->token->ret_type ? left->token->ret_type : left->token->type;
			 if (lt == STRUCT_CALL && left->token->Struct.ptr) {
				 Type  rt = right->token->ret_type ? right->token->ret_type : right->token->type;
				 char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
				                          ? right->token->Struct.ptr->token->name
				                          : to_string(rt);
				 char *op_base      = strjoin(left->token->Struct.ptr->token->name, ".operator.",
				                              to_string(node->token->type));
				 char *op_name      = strjoin(op_base, ".", param_suffix);
				 free(op_base);
				 Node *func = find_function(op_name);
				 free(op_name);
				 if (func) {
					 node->token->Fcall.ptr = func;
					 func->token->used++;
					 node->token->ret_type = func->token->ret_type;
            }
         }
      }
		 break;
   }
	 case NOT: case BNOT: {
		 // TODO: left must be boolean
      gen_ir(left);
      node->token->used++;
      node->left->token->used++;
      if (node->token->type == NOT) node->token->ret_type = BOOL;
      else
         node->token->ret_type = left->token->ret_type ? left->token->ret_type : left->token->type;
      break;
   }
	 case ADD:    case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
	 case LESS:   case GREAT: case LESS_EQUAL: case GREAT_EQUAL: case MOD:
	 case AND:    case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	 case RSHIFT: {
		 // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      CHECK(!left || !left->token, "error in assignment, left is NULL");
      CHECK(!right || !right->token, "error in assignment, right is NULL");
      if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
         node->token->ret_type = left->token->ret_type;
      else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0))
         node->token->ret_type = BOOL;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      set_ret_type(node);
      // Struct binary operator overload dispatch
      {
			 Type lt = left->token->ret_type ? left->token->ret_type : left->token->type;
			 if (lt == STRUCT_CALL && left->token->Struct.ptr) {
				 Type  rt = right->token->ret_type ? right->token->ret_type : right->token->type;
				 char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
				                          ? right->token->Struct.ptr->token->name
				                          : to_string(rt);
				 char *op_base      = strjoin(left->token->Struct.ptr->token->name, ".operator.",
				                              to_string(node->token->type));
				 char *op_name      = strjoin(op_base, ".", param_suffix);
				 free(op_base);
				 Node *func = find_function(op_name);
				 free(op_name);
				 if (func) {
					 node->token->Fcall.ptr = func;
					 func->token->used++;
					 if (func->token->ret_type == STRUCT_CALL) {
						 node->token->ret_type   = STRUCT_CALL;
						 node->token->Struct.ptr = func->token->Struct.ptr;
               } else {
						 node->token->ret_type = func->token->ret_type;
               }
            }
         }
      }
      break;
   }
	 case WHILE: {
		 enter_scope(node);
		 gen_ir(node->left); // condition
		 node->left->token->used++;
		 for (int i = 0; i < node->children_count; i++)
         gen_ir(node->children[i]);
      exit_scope();
		 node->token->used++;
		 break;
   }
	 case IF: {
		 enter_scope(node);
		 Node *curr = node;
		 while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
			 if (includes(curr->token->type, IF, ELIF, 0)) {
				 CHECK(curr->left == NULL, "error");
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
	 case FDEC: {
		 if (!find_function(node->token->name)) new_function(node);
      enter_scope(node);
		 Node **params = (node->left ? node->left->children : NULL);
		 for (int i = 0; params && i < node->left->children_count && !found_error; i++) {
			 gen_ir(params[i]);
			 params[i]->token->used++;
      }
		 for (int i = 0; !node->token->is_proto && i < node->children_count; i++)
         gen_ir(node->children[i]);
      exit_scope();
		 break;
   }
	 case FCALL: {
		 if (node->token->is_static_call) {
			 Node *func = get_function(node->token->name);
			 if (CHECK(!func, "no pub fn '%s'", node->token->name)) break;
         if (CHECK(!func->token->is_pub, "'%s' is not a pub fn — use '.' for instance calls",
			           node->token->name))
            break;
         func->token->used++;
			 node->token->Fcall.ptr = func;
			 if (func->token->ret_type == STRUCT_CALL) {
				 node->token->ret_type   = STRUCT_CALL;
				 node->token->Struct.ptr = func->token->Struct.ptr;
         } else {
				 node->token->ret_type = func->token->ret_type;
         }
			 for (int i = 0; i < node->left->children_count; i++)
            gen_ir(node->left->children[i]);
      } else if (node->token->is_method_call && node->left) {
			 ir_method_call(node);
      } else {
			 ir_regular_call(node);
      }
		 break;
   }
	 case AS: {
		 gen_ir(node->left);
		 node->token->ret_type = node->right->token->type;
		 break;
   }
	 case DOT: {
		 ir_dot(node);
		 break;
   }
	 case ACCESS: {
		 gen_ir(node->left);
		 gen_ir(node->right);
		 if (found_error) break;
      node->right->token->used++;
		 Type ret_type = 0;
		 Type type =
          node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
		 int depth = node->left->token->Array.depth;
		 switch (type) {
		 case CHARS: ret_type = CHAR; break;
		 case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
         if (depth > 1) {
				 // still multi-dim: return sub-array type, depth-1
            ret_type                      = ARRAY;
            node->token->Array.sub_type   = node->left->token->Array.sub_type;
            node->token->Array.depth      = depth - 1;
            node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
         } else {
				 ret_type = node->left->token->Array.sub_type;
				 if (ret_type == STRUCT_CALL)
               node->token->Struct.ptr = node->left->token->Array.struct_ptr;
         }
			 break;
		 case STRUCT_CALL: {
			 // __list_* struct: transform v[i] → v.data[i]
         Node *st = node->left->token->Struct.ptr;
         if (st && strncmp(st->token->name, "__list_", 7) == 0) {
				 // Build DOT node for .data (field 0) manually — no ir_dot call
            Token *dot_tok = new_token(DOT, node->left->token->space);
            Node  *dot     = new_node(dot_tok);
            dot->left      = node->left;
            // Set up right side (field token)
            Token *field_tok        = copy_token(st->children[0]->token); // data field
            field_tok->Struct.index = 0;
            dot->right              = new_node(field_tok);
            // Set DOT token metadata from the data field
            dot->token->ret_type         = st->children[0]->token->ret_type;
            dot->token->type             = DOT;
            dot->token->Array.sub_type   = st->children[0]->token->Array.sub_type;
            dot->token->Array.depth      = st->children[0]->token->Array.depth;
            dot->token->Array.struct_ptr = st->children[0]->token->Array.struct_ptr;
            node->left->token->used++;
            node->left = dot;
            // Resolve ACCESS return type from the data field's array type
            type  = dot->token->ret_type ? dot->token->ret_type : dot->token->type;
            depth = dot->token->Array.depth;
            if (includes(type, ARRAY_TYPE, ARRAY, 0)) {
					 if (depth > 1) {
						 ret_type                      = ARRAY;
						 node->token->Array.sub_type   = dot->token->Array.sub_type;
						 node->token->Array.depth      = depth - 1;
						 node->token->Array.struct_ptr = dot->token->Array.struct_ptr;
               } else {
						 ret_type = dot->token->Array.sub_type;
						 if (ret_type == STRUCT_CALL)
                     node->token->Struct.ptr = dot->token->Array.struct_ptr;
               }
            }
         } else {
				 CHECK(1, "struct '%s' does not support [] access", st ? st->token->name : "(null)");
         }
         break;
      }
		 default: CHECK(1, "handle this case %s", to_string(node->left->token->type)); break;
      }
		 node->token->ret_type = ret_type;
		 break;
   }
	 case RETURN: {
		 if (node->children_count > 0) {
			 // Tuple return: process each child expression
         for (int i = 0; i < node->children_count; i++)
            gen_ir(node->children[i]);
         node->token->ret_type = TUPLE;
         break;
      }
		 gen_ir(node->left);
		 node->token->ret_type = node->left->token->type;
		 if (node->left->token->type == STRUCT_CALL)
         node->token->Struct.ptr = node->left->token->Struct.ptr;
      node->left->token->used++;
		 break;
   }

	 case OUTPUT: {
		 for (int i = 0; i < node->left->children_count; i++) {
			 gen_ir(node->left->children[i]);
			 node->left->children[i]
			     ->token->used++; // keep globals/vars alive (mirrors FCALL arg handling)
      }
		 // output() compiles to printf internally — codegen declares printf as needed
		 node->token->ret_type = VOID;
		 break;
   }
	 case TYPEOF: case SIZEOF: {
		 // If the argument is an unresolved ID, check if it's a struct name
      if (node->left && node->left->token->type == ID && node->left->token->name) {
			 Node *st = get_struct(node->left->token->name);
			 if (st) {
				 node->left->token->type       = STRUCT_CALL;
				 node->left->token->Struct.ptr = st;
         }
      }
      gen_ir(node->left);
      Token *type_tok = node->left->token;
      Type   type     = type_tok->type ? type_tok->type : type_tok->ret_type;
      if (node->token->type == TYPEOF) {
			 node->token->ret_type    = CHARS;
			 node->token->Chars.value = strdup(to_string(type));
      } else {
			 node->token->ret_type  = INT;
			 node->token->Int.value = 0; // placeholder
      }
      node->token->used++;
      break;
   }
	 case STACK: case HEAP: {
		 for (int i = 0; i < node->children_count; i++) {
			 gen_ir(node->children[i]);
			 node->children[i]->token->used++;
      }
		 node->token->used++;
		 break;
   }
	 case MODULE: {
		 char *mname        = node->token->name;
		 char *saved        = current_gen_module;
		 current_gen_module = mname;
		 for (int i = 0; i < node->functions_count; i++) {
			 Node *fn    = node->functions[i];
			 char *qname = strjoin(mname, ".", fn->token->name);
			 setName(fn->token, qname);
			 free(qname);
      }
		 for (int i = 0; i < node->functions_count; i++)
         gen_ir(node->functions[i]);
      current_gen_module = saved;
		 break;
   }
	 case STRUCT_DEF: {
		 // Pre-register all methods so they can reference each other
      for (int i = 0; node && i < node->functions_count; i++) {
			 Node *fn = node->functions[i];
			 if (!find_function(fn->token->name)) add_function(scope, fn);
      }
		 for (int i = 0; node && i < node->functions_count; i++)
         gen_ir(node->functions[i]);
      break;
   }
	 case ENUM_DEF: {
		 for (int i = 0; i < node->children_count; i++)
         node->children[i]->token->used++;
      break;
   }
	 case ARRAY_LIT: {
		 for (int i = 0; i < node->children_count; i++)
         gen_ir(node->children[i]);
      if (node->children_count > 0) {
			 Token *first             = node->children[0]->token;
			 Type   ft                = first->ret_type ? first->ret_type : first->type;
			 node->token->ret_type    = ARRAY;
			 node->token->Array.depth = 1;
			 if (includes(ft, ARRAY_LIT, ARRAY, ARRAY_TYPE, 0)) {
				 node->token->Array.sub_type   = first->Array.sub_type;
				 node->token->Array.struct_ptr = first->Array.struct_ptr;
				 node->token->Array.depth      = first->Array.depth + 1;
         } else {
				 node->token->Array.sub_type = ft;
				 if (ft == STRUCT_CALL) node->token->Array.struct_ptr = first->Struct.ptr;
         }
      }
		 break;
   }
      // ref expr — address-of
	 case REF: {
		 gen_ir(node->left);
		 node->token->is_ref = true;
		 Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
		 node->token->ret_type = lt;
		 break;
   }
      // [BREAK]    [CONTINUE]
	 case BREAK:
	 case CONTINUE: break;
	 default:       {
		 TODO(1, "handle this case %s", to_string(node->token->type));
		 break;
   }
   }
}

void enter_scope(Node *node) {
	scopes_count++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, scopes_count);
	resize_array(scopes, Node *, scopes_size, scopes_count);
	scopes[scopes_count] = node;
	scope                = scopes[scopes_count];
}

void exit_scope() {
	if (CHECK(scopes_count < 0, "No active scope to exit")) return;
	debug(CYAN("Exit Scope: %k index %d\n"), scopes[scopes_count]->token, scopes_count);
	scopes[scopes_count] = NULL;
	scopes_count--;
	scope = scopes[scopes_count];
}

Token *find(Type type, ...) {
	if (found_error) return NULL;
	va_list ap;
	va_start(ap, type);
	while (type) {
		if (type == tokens[exe_count]->type) return tokens[exe_count++];
		type = va_arg(ap, Type);
	}
	return NULL;
};

bool within(int space) {
	Token *curr = tokens[exe_count];
	return !found_error && curr->space > space && curr->type != END;
}

Node *get_struct(char *name) {
	debug(CYAN("get struct [%s] from scope %k\n"), name, scope->token);
	for (int j = scopes_count; j > 0; j--) {
		Node *node = scopes[j];
		TODO(node == NULL, RED("Error accessing NULL, %d"), j);
		for (int i = 0; i < node->structs_count; i++)
			if (strcmp(node->structs[i]->token->name, name) == 0) return node->structs[i];
	}
	return NULL;
}

// IR / SCOPE / VARIABLE MANAGEMENT

Token *get_variable(char *name) {
	debug(CYAN("get variable [%s] from scope %k, has %d vars\n"), name, scope->token,
	      scope->variables_count);
	for (int j = scopes_count; j > 0; j--) {
		Node *scope = scopes[j];
		for (int i = 0; i < scope->variables_count; i++) {
			CHECK(scope->variables[i] == NULL, "unexpected error variables");
			CHECK(scope->variables[i]->name == NULL, "unexpected error name");
			if (strcmp(scope->variables[i]->name, name) == 0) return scope->variables[i];
		}
	}
	CHECK(1, "%s not found", name);
	// SEG();
	return syntax_error()->token;
}

void add_function(Node *parent, Node *node) {
	resize_array(parent->functions, Node *, parent->functions_size, parent->functions_count);
	parent->functions[parent->functions_count++] = node;
}

Node *new_function(Node *node) {
	for (int i = 0; i < scope->functions_count; i++) {
		Node *func = scope->functions[i];
		bool  cond = strcmp(func->token->name, node->token->name) == 0;
		CHECK(cond, "Redefinition of %s", node->token->name);
	}
	add_function(scope, node);
	return node;
}

// Returns the function node or NULL (no error) — for optional overload probing
Node *find_function(char *name) {
	for (int j = scopes_count; j > 0; j--) {
		Node *sc = scopes[j];
		for (int i = 0; i < sc->functions_count; i++)
			if (strcmp(sc->functions[i]->token->name, name) == 0) return sc->functions[i];
	}
	if (current_gen_module) {
		char *qname = strjoin(current_gen_module, ".", name);
		for (int j = scopes_count; j > 0; j--) {
			Node *sc = scopes[j];
			for (int i = 0; i < sc->functions_count; i++) {
				if (strcmp(sc->functions[i]->token->name, qname) == 0) {
					free(qname);
					return sc->functions[i];
				}
			}
		}
		free(qname);
	}
	return NULL;
}

Node *get_function(char *name) {
	Node *f = find_function(name);
	if (f) return f;
	CHECK(1, "'%s' Not found", name);
	return syntax_error();
}

// DEBUGGING AND ERROR HANDLING

// STRING

// OTHER UTILITIES

bool compatible(Token *left, Token *right) {
	Type lt = left->ret_type ? left->ret_type : left->type;
	Type rt = right->ret_type ? right->ret_type : right->type;

	if (lt == rt) return true;

	if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;

	if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.sub_type == CHAR)
		return true;
	if (rt == CHARS && includes(lt, ARRAY_TYPE, ARRAY, 0) && left->Array.sub_type == CHAR)
		return true;

	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY && rt == ARRAY_TYPE) return true;

	// pointer/chars ↔ array[T]: all pointers at LLVM level
	if ((includes(lt, PTR, CHARS, 0) && includes(rt, ARRAY_TYPE, ARRAY, 0)) ||
	    (includes(rt, PTR, CHARS, 0) && includes(lt, ARRAY_TYPE, ARRAY, 0)))
		return true;

	bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
	bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
	if (lt_numeric && rt_numeric && lt == rt) return true;

	if (lt == VOID || rt == VOID) return false;

	if (lt == STRUCT_CALL && rt == STRUCT_CALL) return left->Struct.ptr == right->Struct.ptr;
	return false;
}

Type get_ret_type(Node *node) {
	if (!node || !node->token) return 0;

	Token *token = node->token;
	Node  *left  = node->left;
	Node  *right = node->right;

	if (token->ret_type) return token->ret_type;

	// Scalar types and struct return themselves
	if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
		return token->type;
	if (token->type == FCALL) return token->Fcall.ptr ? token->Fcall.ptr->token->ret_type : 0;
	// Arithmetic, bitwise, and assign ops inherit the left operand type
	if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0)) return get_ret_type(left);
	// Comparison and logical ops always yield bool
	if (includes(token->type, COMPARISON_OPS, AND, OR, NOT, BNOT, 0)) return BOOL;
	if (token->type == AS)
		return right ? (right->token->ret_type ? right->token->ret_type : right->token->type) : 0;
	if (token->type == RETURN) return get_ret_type(left);
	if (token->type == DOT)    return get_ret_type(right);
	if (token->type == ACCESS) {
		Type left_type = get_ret_type(left);
		if (left_type == CHARS || left_type == STACK) return CHAR;
		return left_type;
	}
	if (token->type == ID) return token->type != ID ? token->type : 0;
	TODO(1, "handled this case [%s]", to_string(token->type));
	return 0;
}

void set_ret_type(Node *node) {
	if (!node || !node->token) return;
	Type t = get_ret_type(node);
	if (t) node->token->ret_type = t;

	if (node->token->type == DOT && node->token->ret_type == STRUCT_CALL && node->right)
		node->token->Struct.ptr = node->right->token->Struct.ptr;

	// For FCALL returning struct, copy Struct.ptr from the function declaration
	if (node->token->type == FCALL && node->token->ret_type == STRUCT_CALL && node->token->Fcall.ptr)
		node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}

// ----------------------------------------------------------------------------
// LLVM higher-level helpers (from llvm.c)
// ----------------------------------------------------------------------------
Value allocate_stack(Value size, TypeRef elementType, char *name) {
	Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, 0, 0)};

	if (LLVMIsConstant(size)) {
		unsigned long long constSize    = LLVMConstIntGetZExtValue(size);
		TypeRef            arrayType    = LLVMArrayType(elementType, constSize);
		Value              array_alloca = LLVMBuildAlloca(builder, arrayType, name);
		return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
	}

	Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
	return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef elementType, char *name) {
	Value calloc_func = LLVMGetNamedFunction(module, "calloc");
	if (!calloc_func) {
		TypeRef params[]  = {i64, i64};
		TypeRef func_type = LLVMFunctionType(p8, params, 2, 0);
		calloc_func       = _add_function("calloc", func_type);
	}
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, elementType);

	Value      count_i64;
	unsigned   width = LLVMGetIntTypeWidth(LLVMTypeOf(count));
	if (width < 64)      count_i64 = LLVMBuildZExt(builder, count, i64, "count");
	else if (width > 64) count_i64 = LLVMBuildTrunc(builder, count, i64, "count");
	else count_i64 = count;

	Value   size_i64    = LLVMConstInt(i64, elem_size, 0);
	Value   args[]      = {count_i64, size_i64};
	TypeRef calloc_type = LLVMGlobalGetValueType(calloc_func);
	return LLVMBuildCall2(builder, calloc_type, calloc_func, args, 2, name);
}

void _const_value(Token *token) {
	TypeRef   type  = get_llvm_type(token);
	long long value = 0;

	switch (token->type) {
	case INT:   value = (long long)token->Int.value; break;
	case BOOL:  value = (long long)token->Bool.value; break;
	case CHAR:  value = (int)token->Char.value; break;
	case FLOAT: {
		token->llvm.elem      = LLVMConstReal(type, (double)token->Float.value);
		token->llvm.is_loaded = true;
		return;
	}
	case CHARS: {
		char       name[200];
		char      *processed;
		static int index = 0;
		snprintf(name, sizeof(name), "STR%d", index++);
		processed = allocate(strlen(token->Chars.value) * 2 + 1, 1);
		int j     = 0;
		for (int i = 0; token->Chars.value[i]; i++) {
			if (token->Chars.value[i] == '\\' && token->Chars.value[i + 1]) {
				switch (token->Chars.value[i + 1]) {
				case 'n':  processed[j++] = '\n'; i++; break;
				case 't':  processed[j++] = '\t'; i++; break;
				case 'r':  processed[j++] = '\r'; i++; break;
				case '0':  processed[j++] = '\0'; i++; break;
				case '\\': processed[j++] = '\\'; i++; break;
				case '\"': processed[j++] = '\"'; i++; break;
				case '\'': processed[j++] = '\''; i++; break;
				default:   processed[j++] = token->Chars.value[i]; break;
				}
			} else {
				processed[j++] = token->Chars.value[i];
			}
		}
		token->llvm.elem = LLVMBuildGlobalStringPtr(builder, processed, name);
		free(processed);
		return;
	}
	default: CHECK(1, "handle this case %s", to_string(token->type)); return;
	}
	token->llvm.elem = LLVMConstInt(type, value, 0);
}

// ----------------------------------------------------------------------------
// ASM GENERATION
// ----------------------------------------------------------------------------
void init(char *name) {
	context = LLVMContextCreate();
	module  = LLVMModuleCreateWithNameInContext(name, context);
	builder = LLVMCreateBuilderInContext(context);

	vd  = LLVMVoidTypeInContext(context);
	f32 = LLVMFloatTypeInContext(context);
	i1  = LLVMInt1TypeInContext(context);
	i2  = LLVMIntTypeInContext(context, 2);
	i4  = LLVMIntTypeInContext(context, 4);
	i8  = LLVMInt8TypeInContext(context);
	i16 = LLVMInt16TypeInContext(context);
	i32 = LLVMInt32TypeInContext(context);
	i64 = LLVMInt64TypeInContext(context);
	p8  = LLVMPointerType(i8, 0);
	p32 = LLVMPointerType(i32, 0);

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

	// if (enable_san)
	// {
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18,
	                  LLVMValueAsMetadata(LLVMConstInt(i32, 3, 0)));
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13,
	                  LLVMValueAsMetadata(LLVMConstInt(i32, 4, 0)));
	// }

	// Debug info
	debug_builder       = LLVMCreateDIBuilder(module);
	char *base          = strrchr(name, '/');
	char *filename_only = base ? base + 1 : name;
	char  dir[1024]     = ".";
	if (base) {
		size_t len = base - name;
		strncpy(dir, name, len);
		dir[len] = '\0';
	}

	debug_file = LLVMDIBuilderCreateFile(debug_builder, filename_only, strlen(filename_only), dir,
	                                     strlen(dir));

	debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
	    debug_builder, LLVMDWARFSourceLanguageC, debug_file, "ura", 3, 0, "", 0, 0, "", 0,
	    LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);

	debug_scope = debug_compile_unit;
}

void finalize(char *output) {
	char *error = NULL;

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

	LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

	if (flags) {
		LLVMErrorRef err = LLVMRunPasses(module, flags, NULL, options);
		if (err) {
			char *msg = LLVMGetErrorMessage(err);
			CHECK(1, "Optimizer Error: %s\n", msg);
			LLVMDisposeErrorMessage(msg);
			found_error = true;
			return;
		}
	}

	LLVMDIBuilderFinalize(debug_builder);
	LLVMDisposeDIBuilder(debug_builder);
	debug_builder = NULL;

	if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
		CHECK(1, "Module verification failed:\n%s\n", error);
		LLVMDisposeMessage(error);
		LLVMDisposePassBuilderOptions(options);
		found_error = true;
		// return;
	} else {
		LLVMDisposePassBuilderOptions(options);
	}

	LLVMPrintModuleToFile(module, output, NULL);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);
}

// Look up `<type_name>.delete` and call it on `self_ptr`. No-op if undefined.
void call_delete(char *type_name, Value self_ptr) {
	char *qname = strjoin(type_name, ".delete", NULL);
	Value fn    = LLVMGetNamedFunction(module, qname);
	free(qname);
	if (!fn) return;
	Value args[] = {self_ptr};
	LLVMBuildCall2(builder, LLVMGlobalGetValueType(fn), fn, args, 1, "");
}

void emit_scope_clean(Node *scope, int from, Token *skip) {
	for (int i = from; i < scope->variables_count; i++) {
		Token *var = scope->variables[i];
		if (var == skip)     continue;
		if (!var->llvm.elem) continue;
		if (var->is_ref)     continue;

		if (var->type == STRUCT_CALL)
			call_delete(var->Struct.ptr->token->name, var->llvm.elem);
	}
	// Auto-free temp structs created in this scope
	for (int i = 0; i < scope->temps_count; i++)
		call_delete(scope->temp_defs[i]->token->name, scope->temps[i]);
	scope->temps_count = 0;
}

void append_output_arg(Token *tok, char *fmt, int *functions_count, Value *args, int *nargs) {
	Type type = tok->type ? tok->type : tok->ret_type;

	// String literal: embed directly in the format string (escape % → %%)
	if (tok->type == CHARS && !tok->name) {
		const char *s = tok->Chars.value;
		for (int i = 0; s[i]; i++) {
			if (s[i] == '%') {
				fmt[(*functions_count)++] = '%'; // double % for printf
				fmt[(*functions_count)++] = '%';
			} else if (s[i] == '\\' && s[i + 1]) {
				switch (s[++i]) {
				case 'n':  fmt[(*functions_count)++] = '\n'; break;
				case 't':  fmt[(*functions_count)++] = '\t'; break;
				case 'r':  fmt[(*functions_count)++] = '\r'; break;
				case '\\': fmt[(*functions_count)++] = '\\'; break;
				case '"':  fmt[(*functions_count)++] = '"'; break;
				default:
					fmt[(*functions_count)++] = '\\';
					fmt[(*functions_count)++] = s[i];
					break;
				}
			} else fmt[(*functions_count)++] = s[i];
		}
		return;
	}

	// For expression nodes (DOT, FCALL, etc.), resolve to actual value type
	switch (type) {
	case INT: case LONG: case SHORT: case CHAR: case CHARS: case BOOL:
	case FLOAT:
	case STRUCT_CALL: break;
	case FCALL:       type = tok->ret_type; break;
	default:
		if (tok->ret_type) type = tok->ret_type;
		break;
	}

	switch (type) {
	case INT: case SHORT:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = read_value(tok);
		break;
	case BOOL: {
		Value bool_val            = read_value(tok);
		Value true_str            = LLVMBuildGlobalStringPtr(builder, "True", "true_str");
		Value false_str           = LLVMBuildGlobalStringPtr(builder, "False", "false_str");
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 's';
		args[(*nargs)++] = LLVMBuildSelect(builder, bool_val, true_str, false_str, "bool_str");
		break;
	}
	case LONG:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'l';
		fmt[(*functions_count)++] = 'l';
		fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = read_value(tok);
		break;
	case CHAR:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'c';
		args[(*nargs)++]          = read_value(tok);
		break;
	case CHARS:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 's';
		args[(*nargs)++]          = read_value(tok);
		break;
	case FLOAT:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'f';
		args[(*nargs)++] =
		    LLVMBuildFPExt(builder, read_value(tok), LLVMDoubleTypeInContext(context), "f2d");
		break;
	case STRUCT_CALL: {
		Node *sd       = tok->Struct.ptr;
		char *out_name = strjoin(sd->token->name, ".operator.output", NULL);
		Value out_fn   = LLVMGetNamedFunction(module, out_name);
		free(out_name);
		if (out_fn) {
			Value   self_ptr = tok->llvm.elem;
			TypeRef fn_type  = LLVMGlobalGetValueType(out_fn);
			Value   result   = LLVMBuildCall2(builder, fn_type, out_fn, &self_ptr, 1, "output_op");
			TypeRef ret_type = LLVMGetReturnType(fn_type);
			if (LLVMGetTypeKind(ret_type) == PointerType) {
				// Returns chars — use directly as %s
				fmt[(*functions_count)++] = '%';
				fmt[(*functions_count)++] = 's';
				args[(*nargs)++]          = result;
			} else if (LLVMGetTypeKind(ret_type) == StructType) {
				// Returns a struct (e.g. String) — spill to alloca and recurse
				Value tmp = LLVMBuildAlloca(builder, ret_type, "out_tmp");
				LLVMBuildStore(builder, result, tmp);
				// Find the struct def for the returned type by checking struct fields
				// The returned struct should have its own operator output
				// Look up by iterating known structs to find one matching ret_type
				Node *ret_sd = NULL;
				for (int si = scopes_count; si > 0 && !ret_sd; si--)
					for (int sj = 0; sj < scopes[si]->structs_count; sj++)
						if (scopes[si]->structs[sj]->token->llvm.struct_type == ret_type) {
							ret_sd = scopes[si]->structs[sj];
							break;
						}
				if (ret_sd) {
					// Register temp for auto-cleanup
					if (scope->temps == NULL || scope->temps_count + 5 >= scope->temps_size) {
						int    new_size = scope->temps == NULL ? 10 : scope->temps_size * 2;
						Value *nt       = allocate(new_size, sizeof(Value));
						Node **nd       = allocate(new_size, sizeof(Node *));
						if (scope->temps) {
							memcpy(nt, scope->temps, scope->temps_count * sizeof(Value));
							memcpy(nd, scope->temp_defs, scope->temps_count * sizeof(Node *));
							free(scope->temps);
							free(scope->temp_defs);
						}
						scope->temps      = nt;
						scope->temp_defs  = nd;
						scope->temps_size = new_size;
					}
					scope->temps[scope->temps_count]     = tmp;
					scope->temp_defs[scope->temps_count] = ret_sd;
					scope->temps_count++;
					Token ftok      = *tok;
					ftok.Struct.ptr = ret_sd;
					ftok.llvm.elem  = tmp;
					ftok.type       = STRUCT_CALL;
					append_output_arg(&ftok, fmt, functions_count, args, nargs);
				}
			}
			break;
		}
		// Default: { field: value }
		fmt[(*functions_count)++] = '{';
		fmt[(*functions_count)++] = ' ';
		for (int i = 0; i < sd->children_count; i++) {
			Token *field          = sd->children[i]->token;
			int    functions_size = strlen(field->name);
			memcpy(fmt + *functions_count, field->name, functions_size);
			*functions_count += functions_size;
			fmt[(*functions_count)++] = ':';
			fmt[(*functions_count)++] = ' ';
			Token ftok                = *field;
			ftok.llvm.elem            = struct_field_ptr(tok, i, field->name);
			append_output_arg(&ftok, fmt, functions_count, args, nargs);
			if (i < sd->children_count - 1) {
				fmt[(*functions_count)++] = ',';
				fmt[(*functions_count)++] = ' ';
			}
		}
		fmt[(*functions_count)++] = ' ';
		fmt[(*functions_count)++] = '}';
		break;
	}
	default: fmt[(*functions_count)++] = '?'; break;
	}
}

// FDEC helpers
TypeRef func_ret_type(Token *token) {
	if (token->ret_type == TUPLE) {
		int      n  = token->Tuple.count;
		TypeRef *ft = allocate(n, sizeof(TypeRef));
		for (int i = 0; i < n; i++) ft[i] = get_llvm_type(token->Tuple.types[i]);
		TypeRef st              = LLVMStructTypeInContext(context, ft, n, 0);
		token->llvm.struct_type = st;
		free(ft);
		return st;
	}
	if (token->is_proto && token->ret_type == STRUCT_CALL) return i64;
	if (token->ret_type == STRUCT_CALL && token->is_ref)
		return LLVMPointerType(get_llvm_type(token), 0);
	return get_llvm_type(token);
}

TypeRef *func_param_types(Node *fdec, int *out_count) {
	Token *token       = fdec->token;
	int    fixed_count = fdec->left->children_count;
	int    total       = fixed_count + (token->is_variadic ? 1 : 0);
	*out_count         = total;
	if (fixed_count == 0) return NULL;

	TypeRef *types = allocate(total + 2, sizeof(TypeRef));
	for (int i = 0; i < fixed_count; i++) {
		Token *param = fdec->left->children[i]->token;
		if (token->is_proto && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0))
			types[i] = i64;
		else if (param->is_ref) types[i] = LLVMPointerType(get_llvm_type(param), 0);
		else types[i] = get_llvm_type(param);
	}
	if (token->is_variadic) types[fixed_count] = i32;
	return types;
}

// Build (or reuse) the LLVM function type and register the function in the module.
void resolve_func_type(Node *fdec) {
	Token *token = fdec->token;
	if (token->llvm.func_type) return;

	TypeRef  ret_type    = func_ret_type(token);
	int      pcount;
	TypeRef *param_types = func_param_types(fdec, &pcount);
	TypeRef  func_type   = LLVMFunctionType(ret_type, param_types, pcount, token->is_variadic);

	Value existing        = LLVMGetNamedFunction(module, token->name);
	token->llvm.elem      = existing ? existing : _add_function(token->name, func_type);
	token->llvm.func_type = func_type;
}

// Attach sanitize_address to the function when -san is enabled.
// Emit a non-proto function's body in order: sanitizer attribute, debug-info
// metadata, parameter allocas, optional alloca hoist, and codegen of every
// child statement (stopping at the first terminator).
void emit_func_body(Node *fdec) {
	Token *token = fdec->token;

	if (enable_san) {
		unsigned int kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
		AttributeRef attr = LLVMCreateEnumAttribute(context, kind, 0);
		LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex, attr);
	}

	char           *fname   = token->name;
	LLVMMetadataRef di_type = LLVMDIBuilderCreateSubroutineType(debug_builder, debug_file, NULL, 0,
	                                                            LLVMDIFlagZero);
	LLVMMetadataRef di_func =
	    LLVMDIBuilderCreateFunction(debug_builder, debug_compile_unit, fname, strlen(fname), fname,
	                                strlen(fname), debug_file, token->line, di_type, 0, 1,
	                                token->line, LLVMDIFlagZero, 0);
	LLVMSetSubprogram(token->llvm.elem, di_func);
	debug_scope = di_func;
	_entry(token);
	LLVMSetCurrentDebugLocation2(
	    builder, LLVMDIBuilderCreateDebugLocation(context, token->line, 0, di_func, NULL));

	for (int i = 0; i < fdec->left->children_count; i++) {
		Token *param_token = fdec->left->children[i]->token;
		Value  param       = LLVMGetParam(token->llvm.elem, i);
		LLVMSetValueName(param, param_token->name);
		_alloca(param_token);
		param_token->is_dec = false;
		LLVMBuildStore(builder, param, param_token->llvm.elem);
	}

#if USING_HOIST
	for (int i = 0; i < fdec->children_count; i++)
		hoist_allocas(fdec->children[i]);
#endif
	for (int i = 0; i < fdec->children_count; i++) {
		gen_asm(fdec->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
}

// Walk the scope chain (innermost-out) for the nearest enclosing scope of `type`.
Node *find_enclosing(Type type) {
	for (int i = scopes_count; i >= 0; i--)
		if (scopes[i] && scopes[i]->token->type == type) return scopes[i];
	return NULL;
}

// STRUCT_CALL declaration: alloca/AddGlobal + zero-init, ref skipped, used==0 globals dropped.
void gen_struct_declaration(Token *token) {
	if (!token->is_dec) return;
	if (token->is_global) {
		if (token->used == 0) {
			token->is_dec = false;
			return;
		}
		TypeRef type     = get_llvm_type(token);
		token->llvm.elem = LLVMAddGlobal(module, type, token->name);
		LLVMSetInitializer(token->llvm.elem, LLVMConstNull(type));
	} else if (token->is_ref) {
		// ref struct binding is materialized later by the assignment
	} else {
		_alloca(token);
		TypeRef type = get_llvm_type(token);
		LLVMBuildStore(builder, LLVMConstNull(type), token->llvm.elem);
	}
	token->is_dec = false;
}

// Primitive (INT/LONG/SHORT/CHARS/CHAR/BOOL/ARRAY_TYPE/FLOAT) declaration or constant.
void gen_primitive_declaration(Token *token) {
	if (token->is_dec) {
		if (token->is_global) {
			if (token->used == 0) {
				token->is_dec = false;
				return;
			}
			TypeRef type     = get_llvm_type(token);
			token->llvm.elem = LLVMAddGlobal(module, type, token->name);
			LLVMSetInitializer(token->llvm.elem, _get_default_value(token));
		} else {
			_alloca(token);
			LLVMBuildStore(builder, _get_default_value(token), token->llvm.elem);
		}
		token->is_dec = false;
		return;
	}
	if (!token->name) _const_value(token);
}

// Spill an FCALL/operator-call result into a tracked temp so the scope can clean it up.
void track_temp_struct(Token *token) {
	if (token->ret_type != STRUCT_CALL || !token->Struct.ptr) return;
	TypeRef st_type = get_llvm_type(token);
	Value   tmp     = LLVMBuildAlloca(builder, st_type, "tmp_struct");
	LLVMBuildStore(builder, token->llvm.elem, tmp);
	if (scope->temps == NULL || scope->temps_count + 5 >= scope->temps_size) {
		int    new_size = scope->temps == NULL ? 10 : scope->temps_size * 2;
		Value *nt       = allocate(new_size, sizeof(Value));
		Node **nd       = allocate(new_size, sizeof(Node *));
		if (scope->temps) {
			memcpy(nt, scope->temps, scope->temps_count * sizeof(Value));
			memcpy(nd, scope->temp_defs, scope->temps_count * sizeof(Node *));
			free(scope->temps);
			free(scope->temp_defs);
		}
		scope->temps      = nt;
		scope->temp_defs  = nd;
		scope->temps_size = new_size;
	}
	scope->temps[scope->temps_count]     = tmp;
	scope->temp_defs[scope->temps_count] = token->Struct.ptr;
	scope->temps_count++;
}

// If `param` is a non-ref struct and a copy operator (.operator.ASSIGN.<Self>) exists,
// alloca + invoke it on `arg`, returning the copied value. Returns NULL otherwise so
// the caller can fall back to a plain load.
Value emit_copy_construct(Token *param, Token *arg) {
	if (!param || param->type != STRUCT_CALL) return NULL;
	bool arg_is_struct = arg->type == STRUCT_CALL || arg->ret_type == STRUCT_CALL;
	if (!arg_is_struct) return NULL;

	Node *st_node = param->Struct.ptr;
	if (!st_node) return NULL;
	char *cp_name = strjoin(st_node->token->name, ".operator.ASSIGN.", st_node->token->name);
	Value copy_op = LLVMGetNamedFunction(module, cp_name);
	free(cp_name);
	if (!copy_op) return NULL;

	TypeRef st_type = get_llvm_type(param);
	Value   tmp     = LLVMBuildAlloca(builder, st_type, "copy");
	LLVMBuildStore(builder, LLVMConstNull(st_type), tmp);

	Value src_ptr;
	if (LLVMGetTypeKind(LLVMTypeOf(arg->llvm.elem)) == LLVMPointerTypeKind) {
		src_ptr = arg->llvm.elem;
	} else {
		Value spill = LLVMBuildAlloca(builder, st_type, "spill");
		LLVMBuildStore(builder, arg->llvm.elem, spill);
		src_ptr = spill;
	}
	Value cargs[] = {src_ptr, tmp};
	LLVMBuildCall2(builder, LLVMGlobalGetValueType(copy_op), copy_op, cargs, 2, "");
	return LLVMBuildLoad2(builder, st_type, tmp, "copy_val");
}

// Marshal a single operator-overload arg according to the param's ref-ness.
// Used by ASSIGN, compound-assign, and binary-op operator dispatches (NOT FCALL —
// FCALL has a 4-way matrix that this 3-way helper doesn't cover).
Value marshal_arg_for_op(Token *param, Node *arg) {
	bool param_is_ref = param ? param->is_ref : false;
	bool arg_is_ref   = arg->token->is_ref;

	if (param_is_ref && arg_is_ref) {
		TypeRef type = get_llvm_type(arg->token);
		return LLVMBuildLoad2(builder, LLVMPointerType(type, 0), arg->token->llvm.elem, "ref_arg");
	}
	if (param_is_ref && !arg_is_ref) {
		if (LLVMGetTypeKind(LLVMTypeOf(arg->token->llvm.elem)) == LLVMPointerTypeKind)
			return arg->token->llvm.elem;
		TypeRef type = get_llvm_type(arg->token);
		Value   tmp  = LLVMBuildAlloca(builder, type, "tmp_op");
		LLVMBuildStore(builder, arg->token->llvm.elem, tmp);
		return tmp;
	}
	ensure_loaded(arg);
	return arg->token->llvm.elem;
}

// Propagate multi-dim array info from a stack/heap allocation to the LHS variable
// (and, when assigning to a struct field, to the field definition too).
void propagate_dims(Token *dst, Token *src, Node *lhs_node) {
	dst->llvm.dims_count = 0;
	for (int i = 0; i < src->llvm.dims_count; i++) {
		resize_array(dst->llvm.dims, Value, dst->llvm.dims_size, dst->llvm.dims_count);
		dst->llvm.dims[dst->llvm.dims_count++] = src->llvm.dims[i];
	}
	if (lhs_node->token->type != DOT || !lhs_node->right) return;

	Node *sd = lhs_node->left->token->Struct.ptr;
	if (!sd) return;
	int idx = lhs_node->right->token->Struct.index;
	if (idx < 0 || idx >= sd->children_count) return;

	Token *field           = sd->children[idx]->token;
	field->llvm.dims_count = 0;
	for (int i = 0; i < src->llvm.dims_count; i++) {
		resize_array(field->llvm.dims, Value, field->llvm.dims_size, field->llvm.dims_count);
		field->llvm.dims[field->llvm.dims_count++] = src->llvm.dims[i];
	}
}

// Dispatch a struct operator overload. Returns the call's return Value, or NULL on
// missing srcFunc metadata (caller should `break` from its case).
// `try_copy_ctor` enables the copy-constructor fallback in the !param_ref/!arg_ref
// branch — pass true for binary ops, false for plain ASSIGN/compound-assign.
Value gen_operator_call(Node *node, Node *left, Node *right, bool try_copy_ctor) {
	Node *func    = node->token->Fcall.ptr;
	LLVM  srcFunc = func->token->llvm;
	if (CHECK(!srcFunc.func_type, "operator: func_type NULL for '%s'", node->token->name))
		return NULL;
	if (CHECK(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name)) return NULL;

	gen_asm(left);
	gen_asm(right);

	Token *param =
	    func->left->children_count >= 2 ? func->left->children[0]->token : NULL;

	Value rhs_val = NULL;
	bool  param_is_ref = param ? param->is_ref : false;
	if (try_copy_ctor && !param_is_ref && !right->token->is_ref)
		rhs_val = emit_copy_construct(param, right->token);
	if (!rhs_val) rhs_val = marshal_arg_for_op(param, right);

	Value args[2] = {rhs_val, left->token->llvm.elem};
	return LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, 2, "");
}

// Marshal a single FCALL argument according to the param's ref-ness, with the
// 4-way ref/non-ref matrix, copy-ctor fallback, and ARM64 proto-ABI promotion
// of structs to i64 via memcpy.
Value marshal_fcall_arg(Token *param, Node *arg, bool is_proto_call) {
	bool param_is_ref = param ? param->is_ref : false;
	bool arg_is_ref   = arg->token->is_ref;

	if (param_is_ref && arg_is_ref) {
		TypeRef type = get_llvm_type(arg->token);
		return LLVMBuildLoad2(builder, LLVMPointerType(type, 0), arg->token->llvm.elem, "ref_arg");
	}
	if (param_is_ref && !arg_is_ref) return arg->token->llvm.elem;
	if (!param_is_ref && arg_is_ref) return read_value(arg->token);

	// non-ref → non-ref: try copy-constructor, else ensure_loaded
	Value val = emit_copy_construct(param, arg->token);
	if (!val) {
		ensure_loaded(arg);
		val = arg->token->llvm.elem;
	}

	// ARM64 ABI: promote structs to i64 for proto calls
	bool param_is_struct = param && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0);
	if (is_proto_call && param_is_struct) {
		TypeRef st_type = get_llvm_type(param);
		Value   st_ptr  = LLVMBuildAlloca(builder, st_type, "st_slot");
		LLVMBuildStore(builder, val, st_ptr);
		Value i64p = LLVMBuildAlloca(builder, i64, "i64_slot");
		LLVMBuildMemCpy(builder, i64p, 0, st_ptr, 0, LLVMConstInt(i64, 4, 0));
		val = LLVMBuildLoad2(builder, i64, i64p, "i64_arg");
	}
	return val;
}

// Static call: Foo::method(args) — no `self`, explicit args only.
void gen_fcall_static(Node *node) {
	Node *func   = node->token->Fcall.ptr;
	LLVM  srcFn  = func->token->llvm;
	int   argc   = node->left->children_count;
	Value *args  = allocate(argc + 1, sizeof(Value));
	for (int i = 0; i < argc; i++) {
		gen_asm(node->left->children[i]);
		ensure_loaded(node->left->children[i]);
		args[i] = node->left->children[i]->token->llvm.elem;
	}
	node->token->llvm.elem = LLVMBuildCall2(builder, srcFn.func_type, srcFn.elem, args, argc, "");
	if (node->token->ret_type != VOID) node->token->llvm.is_loaded = true;
	free(args);
	track_temp_struct(node->token);
}

// Recurse into nested STRUCT_CALL fields (and array[StructType] fields) so their
// LLVM types are emitted before the parent struct type is built.
void gen_struct_emit_nested(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Token *ft = node->children[i]->token;
		if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0) {
			ft->Struct.ptr->token->used++;
			gen_asm(ft->Struct.ptr);
		}
		if (includes(ft->type, ARRAY_TYPE, ARRAY, 0) && ft->Array.sub_type == STRUCT_CALL &&
		    ft->Array.struct_ptr && ft->Array.struct_ptr->token->used == 0) {
			ft->Array.struct_ptr->token->used++;
			gen_asm(ft->Array.struct_ptr);
		}
	}
}

// Build the named LLVM struct type from this node's child fields.
void gen_struct_build_type(Node *node) {
	int      pos   = node->children_count;
	TypeRef *types = allocate(pos + 1, sizeof(TypeRef));
	for (int i = 0; i < pos; i++) types[i] = get_llvm_type(node->children[i]->token);
	char *struct_name             = strjoin("struct.", node->token->name, NULL);
	node->token->llvm.struct_type = _named_struct_type(struct_name, types, pos, 0);
	free(struct_name);
	free(types);
}

// Auto-generate the `<Type>.delete(self)` function, which calls field-level
// `.delete` for each STRUCT_CALL field. Idempotent via has_clean.
void gen_struct_emit_delete(Node *node) {
	if (node->token->has_clean) return;
	TypeRef st_type     = node->token->llvm.struct_type;
	TypeRef ptr_type    = LLVMPointerType(st_type, 0);
	TypeRef lc_params[] = {ptr_type};
	TypeRef lc_fn_type  = LLVMFunctionType(vd, lc_params, 1, 0);

	char *fname = strjoin(node->token->name, ".delete", NULL);
	Value fn    = _add_function(fname, lc_fn_type);
	free(fname);

	Block entry = LLVMAppendBasicBlockInContext(context, fn, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	LLVMSetCurrentDebugLocation2(builder, NULL);
	Value self = LLVMGetParam(fn, 0);

	for (int i = 0; i < node->children_count; i++) {
		Token *field = node->children[i]->token;
		if (field->type != STRUCT_CALL) continue;
		Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
		Value field_ptr = LLVMBuildGEP2(builder, st_type, self, indices, 2, field->name);
		call_delete(field->Struct.ptr->token->name, field_ptr);
	}
	LLVMBuildRetVoid(builder);
	node->token->has_clean = true;
}

// Pre-declare LLVM signatures for every method on this struct so the bodies
// can reference each other regardless of declaration order.
void gen_struct_predeclare_methods(Node *node) {
	for (int i = 0; i < node->functions_count; i++)
		resolve_func_type(node->functions[i]);
}

// Walk an IF / ELIF / ELSE chain, emitting condition branches into named blocks
// and a single shared join block. Caller has already entered the scope and
// stashed the start/end blocks on `node->token->llvm`.
void gen_if_chain(Node *node, Block if_start, Block end) {
	Node *curr = node;
	while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
		if (includes(curr->token->type, IF, ELIF, 0)) {
			Block start = curr->token->type == IF ? if_start : curr->token->llvm.bloc;
			Block then  = _append_block(curr->token->type == IF ? "if.then" : "elif.then");
			Block next;
			if (curr->right) {
				next = _append_block(curr->right->token->type == ELSE ? "if.else" : "elif.start");
			} else next = end;

			curr->token->llvm.then = then;

			LLVMPositionBuilderAtEnd(builder, start);
			gen_asm(curr->left);
			ensure_loaded(curr->left);
			LLVMBuildCondBr(builder, curr->left->token->llvm.elem, then, next);

			LLVMPositionBuilderAtEnd(builder, then);
			for (int i = 0; i < curr->children_count; i++) {
				gen_asm(curr->children[i]);
				if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
			}
			_branch(end);

			if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
				curr->right->token->llvm.bloc = next;
		} else if (curr->token->type == ELSE) {
			LLVMPositionBuilderAtEnd(builder, curr->token->llvm.bloc);
			for (int i = 0; i < curr->children_count; i++) {
				gen_asm(curr->children[i]);
				if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
			}
			_branch(end);
		}
		curr = curr->right;
	}
}

// Struct field access: emit GEP to the field. Element load is deferred to ensure_loaded.
void gen_access_struct_field(Node *node) {
	Token *struct_tok = node->left->token;
	int    field_idx  = node->right->token->Struct.index;
	node->token->llvm.elem =
	    struct_field_ptr(struct_tok, field_idx, node->right->token->name);
}

// Multi-dim flat array access: stride * index → GEP, propagating the remaining dims.
// Returns true if a multi-dim row was emitted.
bool gen_access_multidim(Node *node, Value leftValue, Value rightRef) {
	int left_depth = node->left->token->llvm.dims_count;
	if (left_depth <= 1) return false;

	Value stride = LLVMConstInt(i32, 1, 0);
	for (int d = 1; d < left_depth; d++)
		stride = LLVMBuildMul(builder, stride, node->left->token->llvm.dims[d], "stride");
	Value flat_idx = LLVMBuildMul(builder, rightRef, stride, "flat_idx");

	TypeRef base_t;
	if (node->left->token->Array.sub_type == STRUCT_CALL && node->left->token->Array.struct_ptr) {
		base_t = get_llvm_type(node->left->token->Array.struct_ptr->token);
	} else {
		Token tmp = {.type = node->left->token->Array.sub_type};
		base_t    = get_llvm_type(&tmp);
	}
	Value indices[]               = {flat_idx};
	node->token->llvm.elem        = LLVMBuildGEP2(builder, base_t, leftValue, indices, 1, "row");
	node->token->llvm.is_loaded   = true;
	node->token->ret_type         = ARRAY;
	node->token->Array.sub_type   = node->left->token->Array.sub_type;
	node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
	node->token->Array.depth      = left_depth - 1;
	node->token->llvm.dims_count  = 0;
	for (int d = 1; d < left_depth; d++) {
		resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
		             node->token->llvm.dims_count);
		node->token->llvm.dims[node->token->llvm.dims_count++] = node->left->token->llvm.dims[d];
	}
	return true;
}

// Resolve the element type for an ACCESS node and update its ret_type accordingly.
TypeRef resolve_access_element_type(Node *node) {
	Type left_elem =
	    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;

	if (left_elem == CHARS) {
		node->token->ret_type = CHAR;
		return i8;
	}
	if (left_elem == ARRAY_TYPE || left_elem == ARRAY ||
	    node->left->token->type == HEAP || node->left->token->type == ARRAY) {
		Type    et = node->left->token->Array.sub_type;
		TypeRef element_type;
		if (et == STRUCT_CALL && node->left->token->Array.struct_ptr) {
			element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
			node->token->Struct.ptr = node->left->token->Array.struct_ptr;
		} else {
			Token tmp    = {.type = et};
			element_type = get_llvm_type(&tmp);
		}
		node->token->ret_type = et;
		return element_type;
	}
	node->token->ret_type = node->left->token->type;
	return get_llvm_type(node->left->token);
}

// Regular/instance call: marshal args (with ref matrix + copy-ctor + proto ABI),
// emit the call, unpack proto i64-returned structs back to native struct layout,
// and track the temp result for scope cleanup.
void gen_fcall_instance(Node *node) {
	LLVM    srcFunc     = node->token->Fcall.ptr->token->llvm;
	bool    is_proto    = node->token->Fcall.ptr->token->is_proto;
	bool    is_variadic = node->token->Fcall.ptr->token->is_variadic;
	int     count       = node->left->children_count;
	Node  **argNodes    = node->left->children;
	Node   *dec_args    = node->token->Fcall.ptr->left;
	Value  *args        = NULL;

	if (count) {
		args = allocate(count + 3, sizeof(Value));
		int fixed_params = is_variadic ? dec_args->children_count : count;

		for (int i = 0; i < fixed_params; i++) {
			gen_asm(argNodes[i]);
			Token *param = (i < dec_args->children_count) ? dec_args->children[i]->token : NULL;
			args[i]      = marshal_fcall_arg(param, argNodes[i], is_proto);
		}

		if (is_variadic) {
			int variadic_count = count - fixed_params;
			// Ura variadic convention: count precedes the variadic args
			args[fixed_params] = LLVMConstInt(i32, variadic_count, 0);
			for (int i = fixed_params; i < count; i++) {
				gen_asm(argNodes[i]);
				ensure_loaded(argNodes[i]);
				args[i + 1] = argNodes[i]->token->llvm.elem;
			}
			count++;
		}
	}

	if (CHECK(!srcFunc.func_type, "FCALL: func_type is NULL for '%s'", node->token->name)) {
		free(args);
		return;
	}
	if (CHECK(!srcFunc.elem, "FCALL: elem is NULL for '%s'", node->token->name)) {
		free(args);
		return;
	}
	char *name = node->token->Fcall.ptr->token->ret_type != VOID ? node->token->name : "";
	node->token->llvm.elem =
	    LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, count, name);
	free(args);

	// ARM64 ABI: proto returning struct actually returns i64 — unpack it
	if (is_proto && node->token->Fcall.ptr->token->ret_type == STRUCT_CALL) {
		Value   i64_ret = node->token->llvm.elem;
		TypeRef st_type = get_llvm_type(node->token);
		Value   i64p    = LLVMBuildAlloca(builder, i64, "ret_i64");
		LLVMBuildStore(builder, i64_ret, i64p);
		Value st_ptr = LLVMBuildAlloca(builder, st_type, "ret_struct");
		LLVMBuildMemCpy(builder, st_ptr, 0, i64p, 0, LLVMConstInt(i64, 4, 0));
		node->token->llvm.elem = LLVMBuildLoad2(builder, st_type, st_ptr, "ret_struct_val");
	}
	track_temp_struct(node->token);
}

void gen_asm(Node *node) {
	set_debug_location(node->token);
	Node *left  = node->left;
	Node *right = node->right;

	if (CHECK(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type) {
	case STRUCT_CALL: {
		gen_struct_declaration(node->token);
		break;
	}
	case NULLABLE: {
		node->token->llvm.elem = LLVMConstNull(p8);
		break;
	}
	case INT:        case LONG: case SHORT: case CHARS: case CHAR: case BOOL:
	case ARRAY_TYPE: case FLOAT: {
		gen_primitive_declaration(node->token);
		break;
	}
	case ASSIGN: {
		// Struct operator-overload assignment (=)
		if (node->token->Fcall.ptr) {
			Value result = gen_operator_call(node, left, right, false);
			if (result) node->token->llvm.elem = result;
			break;
		}
		// Global variable initialization with a constant — set initializer directly
		if (left->token->is_global && left->token->is_dec) {
			gen_asm(left);
			gen_asm(right);
			if (left->token->llvm.elem && right->token->llvm.elem &&
			    LLVMIsConstant(right->token->llvm.elem))
				LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
			return;
		}
		// Reference binding: a = ref b → store b's address into a
		if (left->token->is_ref && right->token->type == REF) {
			if (!left->token->llvm.elem) _alloca(left->token);
			gen_asm(right);
			LLVMBuildStore(builder, right->token->llvm.elem, left->token->llvm.elem);
			left->token->is_dec = false;
			break;
		}

		gen_asm(left);
		gen_asm(right);

		if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dims_count > 1)
			propagate_dims(left->token, right->token, left);

		Value val;
		if (right->token->is_ref) val = read_value(right->token);
		else {
			ensure_loaded(right);
			val = right->token->llvm.elem;
		}

		// Cast heap/stack result to match a struct field type
		if (includes(right->token->type, STACK, HEAP, 0) && left->token->type == DOT) {
			TypeRef field_type = get_llvm_type(left->token);
			val                = LLVMBuildBitCast(builder, val, field_type, "field_cast");
		}
		// Cast i8* to array[T] (T*) when needed
		if (val && LLVMGetTypeKind(LLVMTypeOf(val)) == PointerType) {
			TypeRef dest_type = get_llvm_type(left->token);
			if (dest_type && LLVMGetTypeKind(dest_type) == PointerType && LLVMTypeOf(val) != dest_type)
				val = LLVMBuildBitCast(builder, val, dest_type, "ptr_cast");
		}

		write_value(left->token, val);
		node->token->llvm.elem = val;
		break;
	}
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: {
		// Struct compound-assign operator overload (e.g. operator+=)
		if (node->token->Fcall.ptr) {
			Value result = gen_operator_call(node, left, right, false);
			if (result) node->token->llvm.elem = result;
			break;
		}
		gen_asm(left);
		gen_asm(right);

		Value current_val = read_value(left->token);
		Value right_val;
		if (right->token->is_ref) right_val = read_value(right->token);
		else {
			ensure_loaded(right);
			right_val = right->token->llvm.elem;
		}

		Value result = build_binary_op(assign_base_op(node->token->type), current_val, right_val);

		write_value(left->token, result);
		node->token->llvm.elem = result;
		break;
	}
	case NOT: case BNOT: {
		gen_asm(left);
		ensure_loaded(left);
		if (left->token->is_ref) {
			left->token->llvm.elem      = read_value(left->token);
			left->token->llvm.is_loaded = true;
		}
		node->token->llvm.elem =
		    LLVMBuildNot(builder, left->token->llvm.elem, to_string(node->token->type));
		break;
	}
	case ADD:    case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
	case LESS:   case GREAT: case LESS_EQUAL: case GREAT_EQUAL: case MOD:
	case AND:    case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT: {
		// Struct binary operator overload (e.g. operator+, operator==)
		if (node->token->Fcall.ptr) {
			Value result = gen_operator_call(node, left, right, true);
			if (result) {
				node->token->llvm.elem = result;
				if (node->token->Fcall.ptr->token->ret_type != STRUCT_CALL &&
				    node->token->Fcall.ptr->token->ret_type != VOID)
					node->token->llvm.is_loaded = true;
			}
			break;
		}
		gen_asm(left);
		gen_asm(right);

		ensure_loaded(left);
		ensure_loaded(right);

		Value lref = left->token->llvm.elem;
		Value rref = right->token->llvm.elem;

		if (left->token->is_ref && !left->token->llvm.is_loaded)   lref = read_value(left->token);
		if (right->token->is_ref && !right->token->llvm.is_loaded) rref = read_value(right->token);

		// Pointer arithmetic: chars/ptr + int → GEP instead of add
		if (node->token->type == ADD && LLVMGetTypeKind(LLVMTypeOf(lref)) == LLVMPointerTypeKind) {
			Value idx = LLVMGetTypeKind(LLVMTypeOf(rref)) == LLVMIntegerTypeKind &&
			                    LLVMGetIntTypeWidth(LLVMTypeOf(rref)) < 64
			                ? LLVMBuildSExt(builder, rref, i64, "idx")
			                : rref;
			node->token->llvm.elem = LLVMBuildGEP2(builder, i8, lref, &idx, 1, "ADD");
		} else {
			node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
		}
		if (includes(node->token->type, COMPARISON_OPS, 0)) node->token->ret_type = BOOL;
		else
			node->token->ret_type = left->token->ret_type ? left->token->ret_type : left->token->type;
		break;
	}
	case STACK: {
		Token   tmp    = {.type = node->token->Array.sub_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int     depth  = node->token->Array.depth;

		// evaluate each dimension, compute total element count, store dims
		Value total = LLVMConstInt(i32, 1, 0);
		for (int i = 0; i < depth; i++) {
			gen_asm(node->children[i]);
			ensure_loaded(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
			             node->token->llvm.dims_count);
			node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
			total = LLVMBuildMul(builder, total, dv, "dim");
		}

		TargetData td        = LLVMGetModuleDataLayout(module);
		size_t     elem_size = LLVMABISizeOfType(td, elem_t);
		Value      total_bytes =
		    LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_stack(total_bytes, elem_t, "stack");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case HEAP: {
		Token   tmp    = {.type = node->token->Array.sub_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int     depth  = node->token->Array.depth;

		Value   total = LLVMConstInt(i32, 1, 0);
		for (int i = 0; i < depth; i++) {
			gen_asm(node->children[i]);
			ensure_loaded(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
			             node->token->llvm.dims_count);
			node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
			total = LLVMBuildMul(builder, total, dv, "dim");
		}

		TargetData td        = LLVMGetModuleDataLayout(module);
		size_t     elem_size = LLVMABISizeOfType(td, elem_t);
		Value      total_bytes =
		    LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_heap(total_bytes, elem_t, "heap");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case ARRAY_LIT: {
		int count = node->children_count;
		// Resolve element LLVM type
		TypeRef elem_t;
		Type    et = node->token->Array.sub_type;
		if (et == STRUCT_CALL && node->token->Array.struct_ptr)
			elem_t = get_llvm_type(node->token->Array.struct_ptr->token);
		else {
			Token tmp = {.type = et};
			elem_t    = get_llvm_type(&tmp);
		}
		// For depth > 1 (nested arrays), element is a pointer
		for (int d = 1; d < node->token->Array.depth; d++)
			elem_t = LLVMPointerType(elem_t, 0);

		// Allocate: calloc(count, sizeof(elem))
		Value cnt      = LLVMConstInt(i32, count, 0);
		Value raw_data = allocate_heap(cnt, elem_t, "array_lit");
		Value data     = LLVMBuildBitCast(builder, raw_data, LLVMPointerType(elem_t, 0), "lit_data");

		// Generate and store each element
		for (int i = 0; i < count; i++) {
			gen_asm(node->children[i]);
			ensure_loaded(node->children[i]);
			Value idx       = LLVMConstInt(i32, i, 0);
			Value indices[] = {idx};
			Value gep       = LLVMBuildGEP2(builder, elem_t, data, indices, 1, "lit_elem");
			LLVMBuildStore(builder, node->children[i]->token->llvm.elem, gep);
		}

		node->token->llvm.elem       = data;
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = cnt;
		break;
	}
	case FCALL: {
		if (node->token->is_static_call) gen_fcall_static(node);
		else                             gen_fcall_instance(node);
		break;
	}
	case FDEC: {
		resolve_func_type(node);

		enter_scope(node);
		if (!node->token->is_proto) emit_func_body(node);
		exit_scope();
		debug_scope = debug_compile_unit;
		break;
	}
	case TUPLE_UNPACK: {
		// Evaluate RHS (function call returning a tuple/aggregate)
		gen_asm(node->left);
		Value tuple_val = node->left->token->llvm.elem;
		// Alloca each LHS variable and extract the corresponding field
		for (int i = 0; i < node->children_count; i++) {
			Node *lhs = node->children[i];
			gen_asm(lhs); // handles is_dec → _alloca, sets is_dec = false
			Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
			LLVMBuildStore(builder, field, lhs->token->llvm.elem);
		}
		break;
	}
	case WHILE: {
		enter_scope(node);
		Block start = _append_block("while.start");
		Block then  = _append_block("while.then");
		Block end   = _append_block("while.end");

		// Store blocks for break/continue
		node->token->llvm.start = start;
		node->token->llvm.then  = then;
		node->token->llvm.end   = end;

		_branch(start);

		LLVMPositionBuilderAtEnd(builder, start);
		gen_asm(node->left); // condition
		ensure_loaded(node->left);
		LLVMBuildCondBr(builder, node->left->token->llvm.elem, then, end);

		LLVMPositionBuilderAtEnd(builder, then);
		for (int i = 0; i < node->children_count; i++) {
			if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
			gen_asm(node->children[i]);
		}
		_branch(start);

		LLVMPositionBuilderAtEnd(builder, end);
		exit_scope();
		break;
	}
	case IF: {
		enter_scope(node);

		Block if_start          = _append_block("if.start");
		Block end               = _append_block("if.end");
		node->token->llvm.start = if_start;
		node->token->llvm.end   = end;

		_branch(if_start);
		gen_if_chain(node, if_start, end);

		LLVMPositionBuilderAtEnd(builder, end);
		exit_scope();
		break;
	}
	case REF: {
		gen_asm(node->left);
		// Return the alloca pointer — don't load
		node->token->llvm.elem = node->left->token->llvm.elem;
		break;
	}
	case BREAK: {
		for (int i = scopes_count; i > 0; i--) {
			if (scopes[i]->token->type == WHILE) {
				_branch(scopes[i]->token->llvm.end);
				return;
			}
		}
		CHECK(1, "break outside loop");
		break;
	}
	case CONTINUE: {
		for (int i = scopes_count; i > 0; i--) {
			if (scopes[i]->token->type == WHILE) {
				_branch(scopes[i]->token->llvm.start);
				return;
			}
		}
		CHECK(1, "continue outside loop");
		break;
	}
	case RETURN: {
		Token *ret_skip =
		    (node->left && node->left->token->type == STRUCT_CALL && !node->left->token->is_ref)
		        ? node->left->token
		        : NULL;
		emit_scope_clean(scope, 0, ret_skip);

		Node *fdec = find_enclosing(FDEC);
		if(CHECK(!fdec, "can not return outside function"))
			break;

		// Inside a `.delete` method, chain field-level `.delete` calls on each STRUCT_CALL field of self.
		char *fn_name = fdec->token->name;
		int   fn_len  = strlen(fn_name);
		if (fn_len > 7 && strcmp(fn_name + fn_len - 7, ".delete") == 0 &&
		    fdec->left->children_count >= 1) {
			Token *self_tok = fdec->left->children[0]->token;
			if (self_tok->type == STRUCT_CALL && self_tok->is_ref) {
				Node   *sd       = self_tok->Struct.ptr;
				TypeRef st_type  = sd->token->llvm.struct_type;
				TypeRef ptr_type = LLVMPointerType(st_type, 0);
				Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
				for (int i = 0; i < sd->children_count; i++) {
					Token *field = sd->children[i]->token;
					if (field->type != STRUCT_CALL) continue;
					Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
					Value field_ptr = LLVMBuildGEP2(builder, st_type, self_ptr, indices, 2, field->name);
					call_delete(field->Struct.ptr->token->name, field_ptr);
				}
			}
		}

		// Before main() returns, call `.delete` for every used global struct.
		if (strcmp(fdec->token->name, "main") == 0) {
			for (int i = 0; i < ura_scope->children_count; i++) {
				Node *child = ura_scope->children[i];
				if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
				if (child->token->used == 0)                                       continue;
				call_delete(child->token->Struct.ptr->token->name, child->token->llvm.elem);
			}
		}

		// Tuple return: pack values into the anonymous return struct.
		if (fdec->token->ret_type == TUPLE && node->children_count > 0) {
			TypeRef tuple_type = fdec->token->llvm.struct_type;
			Value   agg        = LLVMGetUndef(tuple_type);
			for (int i = 0; i < node->children_count; i++) {
				gen_asm(node->children[i]);
				ensure_loaded(node->children[i]);
				agg = LLVMBuildInsertValue(builder, agg, node->children[i]->token->llvm.elem, i, "");
			}
			LLVMBuildRet(builder, agg);
			break;
		}

		if (!node->left) break;

		Token *ret_tok = node->left->token;
		if (ret_tok->type == VOID) {
			LLVMBuildRetVoid(builder);
			break;
		}

		gen_asm(node->left);

		// Struct-by-value: full load
		if (fdec->token->ret_type == STRUCT_CALL && !fdec->token->is_ref) {
			LLVMBuildRet(builder, read_value(ret_tok));
			break;
		}
		// Struct-by-ref: return the held pointer
		if (fdec->token->ret_type == STRUCT_CALL && fdec->token->is_ref) {
			TypeRef st_type  = get_llvm_type(ret_tok);
			TypeRef ptr_type = LLVMPointerType(st_type, 0);
			Value   ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
			LLVMBuildRet(builder, ptr);
			break;
		}

		// Scalar — ensure_loaded may swap node->left->token, so re-read after.
		ensure_loaded(node->left);
		LLVMBuildRet(builder, node->left->token->llvm.elem);
		break;
	}
	case AS: {
		gen_asm(left);
		ensure_loaded(left);

		Value    source     = left->token->llvm.elem;
		TypeRef  sourceType = LLVMTypeOf(source);
		TypeRef  targetType = get_llvm_type(node->right->token);

		TypeKind sourceKind = LLVMGetTypeKind(sourceType);
		TypeKind targetKind = LLVMGetTypeKind(targetType);

		Value    result;

		bool     srcIsFloat = (sourceKind == FloatType || sourceKind == DoubleType);
		bool     tgtIsFloat = (targetKind == FloatType || targetKind == DoubleType);

		// Same type — no-op
		if (sourceType == targetType) result = source;
		// Integer → Integer (trunc / sign-extend / same)
		else if (sourceKind == IntegerType && targetKind == IntegerType) {
			unsigned srcBits = LLVMGetIntTypeWidth(sourceType);
			unsigned tgtBits = LLVMGetIntTypeWidth(targetType);
			if (srcBits > tgtBits)      result = LLVMBuildTrunc(builder, source, targetType, "as");
			else if (srcBits < tgtBits) result = LLVMBuildSExt(builder, source, targetType, "as");
			else result = source;
		}
		// Integer → Float / Double  (signed)
		else if (sourceKind == IntegerType && tgtIsFloat)
			result = LLVMBuildSIToFP(builder, source, targetType, "as");
		// Float / Double → Integer  (signed, truncating toward zero)
		else if (srcIsFloat && targetKind == IntegerType)
			result = LLVMBuildFPToSI(builder, source, targetType, "as");
		// Float ↔ Double
		else if (srcIsFloat && tgtIsFloat) {
			if (sourceKind == FloatType) result = LLVMBuildFPExt(builder, source, targetType, "as");
			else result = LLVMBuildFPTrunc(builder, source, targetType, "as");
		}
		// Integer → Pointer
		else if (sourceKind == IntegerType && targetKind == PointerType)
			result = LLVMBuildIntToPtr(builder, source, targetType, "as");
		// Pointer → Integer
		else if (sourceKind == PointerType && targetKind == IntegerType)
			result = LLVMBuildPtrToInt(builder, source, targetType, "as");
		// Pointer → Pointer  (bitcast)
		else if (sourceKind == PointerType && targetKind == PointerType)
			result = LLVMBuildBitCast(builder, source, targetType, "as");
		else {
			CHECK(1, "unsupported cast from %d to %d", sourceKind, targetKind);
			result = source;
		}

		node->token->llvm.elem      = result;
		node->token->llvm.is_loaded = true;
		break;
	}
	case ENUM_DEF: {
		for (int i = 0; i < node->children_count; i++) {
			Token *var = node->children[i]->token;
			Value  g   = LLVMAddGlobal(module, i32, var->name);
			LLVMSetInitializer(g, LLVMConstInt(i32, (unsigned)var->Int.value, 0));
			LLVMSetGlobalConstant(g, 1);
			var->llvm.elem = g;
			// var->llvm.is_set = true;
			var->is_dec = false;
		}
		break;
	}
	case MODULE: {
		for (int i = 0; i < node->functions_count; i++)
			gen_asm(node->functions[i]);
		break;
	}
	case STRUCT_DEF: {
		if (node->token->used == 0) return;
		gen_struct_emit_nested(node);
		gen_struct_build_type(node);
		gen_struct_emit_delete(node);
		gen_struct_predeclare_methods(node);
		for (int i = 0; i < node->functions_count; i++) gen_asm(node->functions[i]);
		break;
	}
	case DOT: {
		gen_asm(node->left);

		Token *struct_tok  = node->left->token;
		int    field_index = node->right->token->Struct.index;

		// If the struct is a ref, we need to load the pointer first
		// before indexing into it, otherwise we GEP into the alloca
		// that holds the pointer, not into the struct itself.
		if (struct_tok->is_ref) {
			TypeRef struct_type = get_llvm_type(struct_tok);
			Value   struct_ptr  = LLVMBuildLoad2(builder, LLVMPointerType(struct_type, 0),
			                                     struct_tok->llvm.elem, struct_tok->name);
			Value   indices[]   = {
			        LLVMConstInt(i32, 0, 0),
			        LLVMConstInt(i32, field_index, 0),
         };
			node->token->llvm.elem =
			    LLVMBuildGEP2(builder, struct_type, struct_ptr, indices, 2, node->right->token->name);
		} else {
			Value field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);
			node->token->llvm.elem = field_ptr;
		}
		// Propagate multi-dim info from struct field definition
		if (struct_tok->Struct.ptr) {
			Node *sd = struct_tok->Struct.ptr;
			if (field_index >= 0 && field_index < sd->children_count) {
				Token *field_def = sd->children[field_index]->token;
				if (field_def->llvm.dims_count > 0) {
					node->token->llvm.dims_count = 0;
					for (int d = 0; d < field_def->llvm.dims_count; d++) {
						resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
						             node->token->llvm.dims_count);
						node->token->llvm.dims[node->token->llvm.dims_count++] = field_def->llvm.dims[d];
					}
				}
			}
		}
		break;
	}
	case ACCESS: {
		gen_asm(node->left);

		if (node->left->token->type == STRUCT_CALL) {
			gen_access_struct_field(node);
			break;
		}

		gen_asm(node->right);

		Value leftValue;
		if (node->left->token->is_ref) {
			TODO(1, "stop");
			leftValue = read_value(node->left->token);
		} else {
			ensure_loaded(node->left);
			leftValue     = node->left->token->llvm.elem;
			TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
			if (kind == PointerType && node->left->token->name && !node->left->token->llvm.is_loaded &&
			    node->left->token->type != STACK && node->left->token->type != DOT)
				leftValue = LLVMBuildLoad2(builder, p8, leftValue, "ptr_load");
		}

		ensure_loaded(node->right);
		Value rightRef = node->right->token->llvm.elem;

		if (gen_access_multidim(node, leftValue, rightRef)) break;

		TypeRef element_type    = resolve_access_element_type(node);
		Value   indices[]       = {rightRef};
		node->token->llvm.elem  =
		    LLVMBuildGEP2(builder, element_type, leftValue, indices, 1, "ACCESS");
		break;
	}
	case TYPEOF: case SIZEOF: {
		Token *type_tok = node->left->token;
		if (node->token->type == TYPEOF) {
			node->token->llvm.elem =
			    LLVMBuildGlobalStringPtr(builder, node->token->Chars.value, "typeof");
			node->token->llvm.is_loaded = true;
		} else {
			TypeRef    type             = get_llvm_type(type_tok);
			TargetData td               = LLVMGetModuleDataLayout(module);
			size_t     size             = LLVMABISizeOfType(td, type);
			node->token->Int.value      = (long long)size;
			node->token->llvm.elem      = LLVMConstInt(i32, size, 0);
			node->token->llvm.is_loaded = true;
		}
		break;
	}
	case OUTPUT: {
		int    argc = node->left->children_count;
		Node **argv = node->left->children;

		// Compute format string capacity: literals + format specifiers + struct expansion
		int fmt_cap = 64;
		for (int i = 0; i < argc; i++)
			if (argv[i]->token->type == CHARS && !argv[i]->token->name)
				fmt_cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
			else fmt_cap += 128;

		char  *fmt             = allocate(fmt_cap, 1);
		Value *args            = allocate(256, sizeof(Value));
		int    functions_count = 0;
		int    nargs           = 0;

		for (int i = 0; i < argc; i++) {
			gen_asm(argv[i]);
			append_output_arg(argv[i]->token, fmt, &functions_count, args, &nargs);
		}
		fmt[functions_count] = '\0';

		Value printf_fn = LLVMGetNamedFunction(module, "printf");
		if (!printf_fn) {
			TypeRef params[]  = {p8, i32}; // fmt, nargs (language variadic convention)
			TypeRef printf_ft = LLVMFunctionType(i32, params, 2, 1);
			printf_fn         = _add_function("printf", printf_ft);
		}
		TypeRef printf_ft = LLVMGlobalGetValueType(printf_fn);
		// Follow the language's variadic calling convention: insert arg count between
		// fixed params and variadic args — same as FCALL does for variadic functions
		Value *call_args = allocate(nargs + 3, sizeof(Value));
		call_args[0]     = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
		call_args[1]     = LLVMConstInt(i32, nargs, 0);
		memcpy(call_args + 2, args, nargs * sizeof(Value));
		LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 2, "");
		free(fmt);
		free(args);
		free(call_args);
		break;
	}
	default:
		pnode(node, "");
		TODO(1, "handle this case %s", to_string(node->token->type));
		// SEG();
		break;
	}
}

// ============================================================================
// Preprocessed source output (-prep flag)
// ============================================================================

static void emit_indent(File f, int depth) {
	for (int i = 0; i < depth * TAB; i++)
		fputc(' ', f);
}

static const char *op_symbol(Type type) {
	switch (type) {
	case ADD:         return "+";
	case SUB:         return "-";
	case MUL:         return "*";
	case DIV:         return "/";
	case MOD:         return "%";
	case EQUAL:       return "==";
	case NOT_EQUAL:   return "!=";
	case LESS:        return "<";
	case GREAT:       return ">";
	case LESS_EQUAL:  return "<=";
	case GREAT_EQUAL: return ">=";
	case AND:         return "and";
	case OR:          return "or";
	case NOT:         return "not";
	case BAND:        return "&";
	case BOR:         return "|";
	case BXOR:        return "^";
	case BNOT:        return "~";
	case LSHIFT:      return "<<";
	case RSHIFT:      return ">>";
	case ASSIGN:      return "=";
	case ADD_ASSIGN:  return "+=";
	case SUB_ASSIGN:  return "-=";
	case MUL_ASSIGN:  return "*=";
	case DIV_ASSIGN:  return "/=";
	case MOD_ASSIGN:  return "%=";
	default:          return "?";
	}
}

static void emit_expr(File f, Node *node);

static void emit_type_name(File f, Token *tok) {
	Type t = tok->ret_type ? tok->ret_type : tok->type;
	switch (t) {
	case INT:   fprintf(f, "int"); break;
	case LONG:  fprintf(f, "long"); break;
	case SHORT: fprintf(f, "short"); break;
	case CHAR:  fprintf(f, "char"); break;
	case CHARS: fprintf(f, "chars"); break;
	case BOOL:  fprintf(f, "bool"); break;
	case FLOAT: fprintf(f, "float"); break;
	case VOID:  fprintf(f, "void"); break;
	case PTR:   fprintf(f, "pointer"); break;
	case STRUCT_CALL:
		if (tok->Struct.ptr) fprintf(f, "%s", tok->Struct.ptr->token->name);
		else fprintf(f, "%s", tok->name ? tok->name : "?");
		break;
	case ARRAY_TYPE: case ARRAY: {
		fprintf(f, "array[");
		const char *en = type_to_ura_name(tok->Array.sub_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.sub_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		fprintf(f, "]");
		break;
	}
	default: fprintf(f, "%s", to_string(t)); break;
	}
}

static void emit_expr(File f, Node *node) {
	if (!node || !node->token) return;
	Token *tok = node->token;

	switch (tok->type) {
	case INT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%ld", tok->Int.value);
		break;
	case LONG:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%lld", tok->Long.value);
		break;
	case SHORT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%d", tok->Short.value);
		break;
	case FLOAT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%f", tok->Float.value);
		break;
	case BOOL:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%s", tok->Bool.value ? "true" : "false");
		break;
	case CHAR:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "'%c'", tok->Char.value);
		break;
	case CHARS:
		if (tok->name) fprintf(f, "%s", tok->name);
		else {
			fprintf(f, "\"");
			const char *s = tok->Chars.value ? tok->Chars.value : "";
			for (int ci = 0; s[ci]; ci++) {
				switch (s[ci]) {
				case '\n': fprintf(f, "\\n"); break;
				case '\t': fprintf(f, "\\t"); break;
				case '\r': fprintf(f, "\\r"); break;
				case '\\': fprintf(f, "\\\\"); break;
				case '"':  fprintf(f, "\\\""); break;
				default:   fputc(s[ci], f); break;
				}
			}
			fprintf(f, "\"");
		}
		break;
	case NULLABLE:    fprintf(f, "null"); break;
	case VOID:        fprintf(f, "void"); break;
	case STRUCT_CALL: fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case ARRAY_TYPE:  if (tok->name) fprintf(f, "%s", tok->name); break;
	case ID:          fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case DOT:
		emit_expr(f, node->left);
		fprintf(f, ".");
		if (node->right && node->right->token->name) fprintf(f, "%s", node->right->token->name);
		break;
	case ACCESS:
		emit_expr(f, node->left);
		fprintf(f, "[");
		emit_expr(f, node->right);
		fprintf(f, "]");
		break;
	case DOUBLE_DOTS:
		emit_expr(f, node->left);
		fprintf(f, "::");
		emit_expr(f, node->right);
		break;
	case AS:
		emit_expr(f, node->left);
		fprintf(f, " as ");
		emit_type_name(f, node->right->token);
		break;
	case NOT:  fprintf(f, "not "); emit_expr(f, node->left); break;
	case BNOT: fprintf(f, "~"); emit_expr(f, node->left); break;
	case TYPEOF:
		fprintf(f, "typeof(");
		emit_expr(f, node->left);
		fprintf(f, ")");
		break;
	case SIZEOF:
		fprintf(f, "sizeof(");
		// sizeof argument is a type, not a value — print type name
		if (node->left && node->left->token) {
			const char *tn = type_to_ura_name(node->left->token->type);
			if (tn) fprintf(f, "%s", tn);
			else emit_expr(f, node->left);
		}
		fprintf(f, ")");
		break;
	case REF: fprintf(f, "ref "); emit_expr(f, node->left); break;
	// Binary operators
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:
		if (tok->Fcall.ptr) {
			// Operator overload — emit as Struct::method(args)
			char *oname = tok->Fcall.ptr->token->name;
			char *dot   = strchr(oname, '.');
			if (dot) {
				int plen = (int)(dot - oname);
				fprintf(f, "%.*s::%s(", plen, oname, dot + 1);
			} else fprintf(f, "%s(", oname);
			emit_expr(f, node->right);
			fprintf(f, ", ");
			emit_expr(f, node->left);
			fprintf(f, ")");
		} else {
			emit_expr(f, node->left);
			fprintf(f, " %s ", op_symbol(tok->type));
			emit_expr(f, node->right);
		}
		break;
	// Function calls
	case FCALL: {
		// Detect resolved method calls: qualified name "Struct.method" + last arg is self
		bool is_resolved_method = false;
		if (node->left && tok->name && strchr(tok->name, '.') && !tok->is_static_call) {
			int last = node->left->children_count - 1;
			if (last >= 0 && node->left->children[last]->token->type == STRUCT_CALL)
				is_resolved_method = true;
		}
		if (is_resolved_method) {
			// Method call: obj.method(args) — self is last child
			int last = node->left->children_count - 1;
			emit_expr(f, node->left->children[last]); // object
			char *dot = strrchr(tok->name, '.');
			fprintf(f, ".%s(", dot ? dot + 1 : tok->name);
			for (int i = 0; i < last; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
			fprintf(f, ")");
		} else if (tok->is_static_call && tok->name && strchr(tok->name, '.')) {
			// Static call: Struct::method(args)
			char *dotpos     = strchr(tok->name, '.');
			int   prefix_len = (int)(dotpos - tok->name);
			fprintf(f, "%.*s::%s(", prefix_len, tok->name, dotpos + 1);
			if (node->left) {
				for (int i = 0; i < node->left->children_count; i++) {
					if (i > 0) fprintf(f, ", ");
					emit_expr(f, node->left->children[i]);
				}
			}
			fprintf(f, ")");
		} else {
			fprintf(f, "%s(", tok->name);
			if (node->left) {
				for (int i = 0; i < node->left->children_count; i++) {
					if (i > 0) fprintf(f, ", ");
					emit_expr(f, node->left->children[i]);
				}
			}
			fprintf(f, ")");
		}
		break;
	}
	case HEAP: case STACK: {
		fprintf(f, "%s[", tok->type == HEAP ? "heap" : "stack");
		emit_type_name(f, tok);
		fprintf(f, "](");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, ")");
		break;
	}
	case ARRAY_LIT: {
		fprintf(f, "[");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, "]");
		break;
	}
	default: fprintf(f, "/* ? %s */", to_string(tok->type)); break;
	}
}

static void emit_node(File f, Node *node, int depth) {
	if (!node || !node->token) return;
	Token *tok = node->token;

	switch (tok->type) {
	case PROTO: {
		// proto is on the child FDEC
		if (node->left && node->left->token->type == FDEC) emit_node(f, node->left, depth);
		break;
	}
	case FDEC: {
		emit_indent(f, depth);
		if (tok->is_proto) fprintf(f, "proto ");
		// Detect if this is a struct method (has self as last param)
		bool has_self = false;
		if (node->left && node->left->children_count > 0) {
			Token *last = node->left->children[node->left->children_count - 1]->token;
			if (last->type == STRUCT_CALL && last->name && strcmp(last->name, "self") == 0)
				has_self = true;
		}
		// Use short name for struct methods (strip "Struct." prefix)
		const char *fn_name = tok->name;
		if (has_self) {
			char *dot = strrchr(tok->name, '.');
			if (dot) fn_name = dot + 1;
		}
		// Detect operator methods: delete, operator.*, output
		bool is_operator = false;
		if (has_self && fn_name) {
			if (strcmp(fn_name, "delete") == 0 || strcmp(fn_name, "output") == 0 ||
			    strncmp(fn_name, "operator", 8) == 0)
				is_operator = true;
		}
		if (is_operator) {
			fprintf(f, "operator %s(", fn_name);
		} else {
			if (tok->is_pub) fprintf(f, "pub ");
			fprintf(f, "fn %s(", fn_name);
		}
		if (node->left) {
			int end = node->left->children_count;
			// Skip self param (implicit for all struct methods)
			if (has_self && end > 0) end--;
			int printed = 0;
			for (int i = 0; i < end; i++) {
				Token *p = node->left->children[i]->token;
				if (printed > 0) fprintf(f, ", ");
				fprintf(f, "%s ", p->name);
				emit_type_name(f, p);
				if (p->is_ref) fprintf(f, " ref");
				printed++;
			}
		}
		fprintf(f, ") ");
		// Return type
		if (tok->ret_type == STRUCT_CALL && tok->Struct.ptr)
			fprintf(f, "%s", tok->Struct.ptr->token->name);
		else if (tok->ret_type)
			fprintf(f, "%s",
			        type_to_ura_name(tok->ret_type) ? type_to_ura_name(tok->ret_type)
			                                        : to_string(tok->ret_type));
		if (tok->is_proto) {
			fprintf(f, "\n");
			break;
		}
		fprintf(f, ":\n");
		// Body
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		fprintf(f, "\n");
		break;
	}
	case STRUCT_DEF: {
		emit_indent(f, depth);
		fprintf(f, "struct %s:\n", tok->name);
		// Fields
		for (int i = 0; i < node->children_count; i++) {
			Token *field = node->children[i]->token;
			emit_indent(f, depth + 1);
			fprintf(f, "%s ", field->name);
			emit_type_name(f, field);
			fprintf(f, "\n");
		}
		fprintf(f, "\n");
		// Methods
		for (int i = 0; i < node->functions_count; i++)
			emit_node(f, node->functions[i], depth + 1);
		fprintf(f, "\n");
		break;
	}
	case STRUCT_CALL: {
		if (tok->is_dec) {
			emit_indent(f, depth);
			fprintf(f, "%s %s", tok->name, tok->Struct.ptr->token->name);
			if (tok->is_ref) fprintf(f, " ref");
			fprintf(f, "\n");
		}
		break;
	}
	case ASSIGN:     case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
	case DIV_ASSIGN: case MOD_ASSIGN: {
		emit_indent(f, depth);
		if (tok->Fcall.ptr) {
			// Operator overload — Struct::operator.OP.T(args)
			char *oname = tok->Fcall.ptr->token->name;
			char *dot   = strchr(oname, '.');
			if (dot) {
				int plen = (int)(dot - oname);
				fprintf(f, "%.*s::%s(", plen, oname, dot + 1);
			} else fprintf(f, "%s(", oname);
			emit_expr(f, node->right);
			fprintf(f, ", ");
			emit_expr(f, node->left);
			fprintf(f, ")\n");
		} else {
			emit_expr(f, node->left);
			fprintf(f, " %s ", op_symbol(tok->type));
			emit_expr(f, node->right);
			fprintf(f, "\n");
		}
		break;
	}
	case INT:   case LONG: case SHORT: case FLOAT: case BOOL: case CHAR:
	case CHARS: case ARRAY_TYPE: case PTR: {
		if (tok->is_dec) {
			emit_indent(f, depth);
			fprintf(f, "%s ", tok->name);
			emit_type_name(f, tok);
			if (tok->is_ref) fprintf(f, " ref");
			fprintf(f, "\n");
		}
		break;
	}
	case RETURN: {
		emit_indent(f, depth);
		// Skip implicit "return void" — the compiler auto-adds it
		if (node->left && node->left->token && node->left->token->type == VOID) break;
		fprintf(f, "return ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	}
	case IF: case ELIF: {
		emit_indent(f, depth);
		fprintf(f, "%s ", tok->type == IF ? "if" : "elif");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		if (node->right) emit_node(f, node->right, depth);
		break;
	}
	case ELSE: {
		emit_indent(f, depth);
		fprintf(f, "else:\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case WHILE: {
		emit_indent(f, depth);
		fprintf(f, "while ");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case BREAK: {
		emit_indent(f, depth);
		fprintf(f, "break\n");
		break;
	}
	case CONTINUE: {
		emit_indent(f, depth);
		fprintf(f, "continue\n");
		break;
	}
	case FCALL: {
		emit_indent(f, depth);
		emit_expr(f, node);
		fprintf(f, "\n");
		break;
	}
	case OUTPUT: {
		emit_indent(f, depth);
		fprintf(f, "output(");
		if (node->left) {
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
		}
		fprintf(f, ")\n");
		break;
	}
	default:
		emit_indent(f, depth);
		fprintf(f, "// ? %s\n", to_string(tok->type));
		break;
	}
}

void emit_prep_file(Node *scope, char *path) {
	File f = fopen(path, "w");
	if (!f) {
		CHECK(1, "cannot open %s for writing", path);
		return;
	}
	fprintf(f, "// Preprocessed output — generated by ura -prep\n\n");
	for (int i = 0; i < scope->children_count; i++)
		emit_node(f, scope->children[i], 0);
	fclose(f);
}

// List type temp file paths (set by instantiate_list_types, used by compile)
static char list_tmp_files[64][URA_MAX_SIZE];
static int  list_tmp_count = 0;

void        instantiate_list_types(void) {
	       // Scan token stream for LIST_TYPE, collect unique (elem_type, depth) pairs
   typedef struct {
		       const char *base_name;
		       int         depth;
   } ListEntry;
   ListEntry entries[64];
   int       entry_count = 0;

   for (int i = 0; i < tokens_count; i++) {
		       if (tokens[i]->type != LIST_TYPE) continue;
      int j = i + 1;
		       if (j >= tokens_count || tokens[j]->type != LBRA) continue;
      int depth = 0;
		       while (j < tokens_count && tokens[j]->type == LBRA) {
			       depth++;
			       j++;
      }
		       if (j >= tokens_count) continue;
      const char *base_name = tokens[j]->name ? tokens[j]->name : type_to_ura_name(tokens[j]->type);
		       if (!base_name) continue;
      bool found = false;
		       for (int k = 0; k < entry_count; k++)
         if (strcmp(entries[k].base_name, base_name) == 0 && entries[k].depth == depth) {
				       found = true;
				       break;
         }
      if (!found && entry_count < 64) {
			       entries[entry_count].base_name = base_name;
			       entries[entry_count].depth     = depth;
			       entry_count++;
			       // For depth > 1, ensure all intermediate depths exist
			       for (int d = 1; d < depth; d++) {
				       bool has = false;
				       for (int k = 0; k < entry_count; k++)
               if (strcmp(entries[k].base_name, base_name) == 0 && entries[k].depth == d) {
						       has = true;
						       break;
               }
            if (!has && entry_count < 64) {
					       entries[entry_count].base_name = base_name;
					       entries[entry_count].depth     = d;
					       entry_count++;
            }
         }
      }
   }
   list_tmp_count = 0;
   if (entry_count == 0) return;

   // Sort entries by depth ascending
   for (int i = 0; i < entry_count - 1; i++)
      for (int j = i + 1; j < entry_count; j++)
         if (entries[j].depth < entries[i].depth) {
				       ListEntry tmp = entries[i];
				       entries[i]    = entries[j];
				       entries[j]    = tmp;
         }

   // Generate source and write to temp files
   for (int i = 0; i < entry_count; i++) {
		       const char *base = entries[i].base_name;
		       int         d    = entries[i].depth;
		       char        elem_name[256];
		       char        sname[256];
		       if (d == 1) {
			       snprintf(elem_name, sizeof(elem_name), "%s", base);
			       snprintf(sname, sizeof(sname), "__list_%s", base);
      } else {
			       snprintf(elem_name, sizeof(elem_name), "%s", base);
			       for (int dd = 0; dd < d; dd++) {
				       if (dd == 0) {
					       snprintf(sname, sizeof(sname), "__list_%s", base);
            } else {
					       snprintf(elem_name, sizeof(elem_name), "%s", sname);
					       char tn[256];
					       snprintf(tn, sizeof(tn), "__list_%s", sname);
					       snprintf(sname, sizeof(sname), "%s", tn);
            }
         }
      }

		       char *src = generate_list_source(elem_name, sname);
		       debug(RED("GENERATED LIST SOURCE:\n%s\n"), src);

		       // Prepend memory protos to the first file
		       char tmppath[URA_MAX_SIZE];
		       snprintf(tmppath, sizeof(tmppath), "/tmp/__ura_%s.ura", sname);
		       File tmpf = fopen(tmppath, "w");
		       if (tmpf) {
			       if (i == 0) fputs("use \"@/memory\"\n\n", tmpf);
         fputs(src, tmpf);
			       fclose(tmpf);
			       snprintf(list_tmp_files[list_tmp_count++], URA_MAX_SIZE, "%s", tmppath);
      }
		       free(src);
   }
}

char *compile(char *path_name) {
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));
	// reset ura_scope state for each file
	found_error  = false;
	tokens_count = 0;
	exe_count    = 0;
	scopes_count = 0;
	scope        = NULL;
	ura_scope    = NULL;

	char *file_name = realpath(path_name, NULL);
	if (CHECK(file_name == NULL, "error resolving %s\n", path_name)) return NULL;
	// Two-pass: first tokenize to scan for list types, then retokenize with generated structs
	tokenize(file_name, 0);
	instantiate_list_types();
	if (list_tmp_count > 0) {
		// Reset token state and re-tokenize with generated structs first
		free_local_memory();
		ucount = 0;
		// Tokenize generated list struct files (includes memory.ura via use)
		for (int i = 0; i < list_tmp_count; i++) {
			calling_use++;
			tokenize(list_tmp_files[i], 0);
			calling_use--;
		}
		// Re-tokenize user file
		tokenize(file_name, 0);
	}

	for (int i = 0; tokens[i]; i++)
		debug(GREEN("%k\n"), tokens[i]);

#if AST
	if (found_error) return NULL;
	ura_scope = new_node(new_token(ID, -TAB));
	setName(ura_scope->token, "ura-scope");
	enter_scope(ura_scope);
	while (!find(END, 0) && !found_error)
		add_child(ura_scope, expr_node());
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
	if (found_error) return NULL;
#endif

#if IR
	if (found_error) return NULL;
	// Pre-register all top-level functions for forward references
	for (int i = 0; !found_error && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == FDEC && !find_function(child->token->name))
			add_function(scope, child);
	}
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_ir(ura_scope->children[i]);
	if (found_error) return NULL;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER IR\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#if OPTIMIZE
	bool changed = true;
	int  op      = 0;
	while (changed) {
		changed = false;
		optimize_ir(ura_scope, &changed);
		if (changed) op++;
	}
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER OPTMIZING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; op && !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#endif
#endif

	// build/  directory next to the source file
	char *dir  = dirname(strdup(path_name));
	char *base = basename(strdup(path_name));
	// strip .ura extension
	char *dot_ext = strrchr(base, '.');
	if (dot_ext) *dot_ext = '\0';

	// Preprocessed output mode
	if (enable_prep && !found_error) {
		char *build_dir = strjoin(dir, "/build", NULL);
		mkdir(build_dir, 0755);
		char *prep_path = strjoin(build_dir, "/", strjoin(base, ".prep.ura", NULL));
		emit_prep_file(ura_scope, prep_path);
		printf("Wrote %s\n", prep_path);
		return NULL;
	}

#if ASM
	if (found_error) return NULL;

	char *build_dir = strjoin(dir, "/build", NULL);
	mkdir(build_dir, 0755);
	char *ll_path = strjoin(build_dir, "/", strjoin(base, ".ll", NULL));

	char *moduleName = resolve_path(path_name);
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

int main(int argc, char **argv) {
	ura_lib = getenv("URA_LIB");
	if (CHECK(!ura_lib, "URA_LIB not set")) return 1;
	argv0 = argv[0];
	if (CHECK(argc < 2,
	          "usage: ura <file.ura> [file2.ura ...] [-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
		return 1;

	char *output  = NULL;
	bool  no_exec = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-O0") == 0)            flags = PASSES_O0;
		else if (strcmp(argv[i], "-O1") == 0)       flags = PASSES_O1;
		else if (strcmp(argv[i], "-O2") == 0)       flags = PASSES_O2;
		else if (strcmp(argv[i], "-O3") == 0)       flags = PASSES_O3;
		else if (strcmp(argv[i], "-Os") == 0)       flags = PASSES_Os;
		else if (strcmp(argv[i], "-Oz") == 0)       flags = PASSES_Oz;
		else if (strcmp(argv[i], "-san") == 0)      enable_san = true;
		else if (strcmp(argv[i], "-no-exec") == 0)  no_exec = true;
		else if (strcmp(argv[i], "-no-debug") == 0) enable_debug = false;
		else if (strcmp(argv[i], "-prep") == 0)     enable_prep = true;
		else if (strcmp(argv[i], "-o") == 0)        {
			CHECK(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		} else if (argv[i][0] != '-' && strlen(argv[i]) > 4 &&
		           strcmp(argv[i] + strlen(argv[i]) - 4, ".ura") == 0) {
			resize_array(files, char *, files_size, files_count);
			files[files_count++] = argv[i];
		} else {
			CHECK(argv[i][0] == '-', "unknown flag: %s\n", argv[i]);
			CHECK(argv[i][0] != '-', "unvalid file: %s\n", argv[i]);
		}
	}

	char default_output[URA_MAX_SIZE];
	if (!output && files_count > 0) {
		char *base = basename(files[0]);
		snprintf(default_output, sizeof(default_output), "%s", base);
		char *dot = strrchr(default_output, '.');
		if (dot) *dot = '\0';
		output = default_output;
	}
	if (!output) output = "exe.out";

#if ASM
	char final_cmd[URA_MAX_SIZE];
	int  pos = 0;

	pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
	if (enable_san)
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos,
		                " -fsanitize=address,undefined -fno-omit-frame-pointer -g");
#endif

	for (int i = 0; i < files_count && !found_error; i++) {
		char *ll = compile(files[i]);
		if (enable_prep) continue;
#if ASM
		if (!no_exec && !found_error) {
			if (enable_san) pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
			else {
				char s[URA_MAX_SIZE];
				snprintf(s, sizeof(s), "%s", ll);
				char *dot = strrchr(s, '.');
				if (dot) strcpy(dot, ".s");

				char llc_cmd[URA_MAX_SIZE];
				snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
				if (CHECK(system(llc_cmd) != 0, "llc failed for %s\n", ll)) {
					free(ll);
					break;
				}
				pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", s);
			}
		}
		free(ll);
#endif
		free_local_memory();
	}
#if ASM
	for (int i = 0; i < lcount && !found_error; i++) {
		char env_name[URA_MAX_SIZE];
		snprintf(env_name, sizeof(env_name), "URA_LINK_%s", links[i]);
		char *flags = getenv(env_name);
		if (flags) pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
	}

	if (!no_exec && !found_error && !enable_prep) {
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
		if (!CHECK(system(final_cmd) != 0, "linking failed\n")) {
			char full[URA_MAX_SIZE];
			realpath(output, full);

			if (enable_san) {
				char *asan_file = getenv("ASAN_FILE");
				if (asan_file) {
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
					setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
				} else setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
			}
		}
	}
#endif
	free(files);
	free_global_memory();
	return found_error;
}
