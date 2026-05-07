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

#define URA_MAX_SIZE       999999
#define LIST_NAME_MAX      256
#define LIST_SOURCE_MAX    4096
#define LIST_STRUCT_PREFIX "__list_"
#define OP_PREFIX          ".operator."

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
#define EXPECT_TOKEN(anchor, type, fmt, ...)                                                       \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			parse_error(anchor, fmt, ##__VA_ARGS__);                                                  \
			return syntax_error();                                                                    \
		}                                                                                            \
	}

#define EXPECT_TOKEN_VOID(anchor, type, fmt, ...)                                                  \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			parse_error(anchor, fmt, ##__VA_ARGS__);                                                  \
			syntax_error();                                                                           \
			return;                                                                                   \
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
typedef struct AutoClean  AutoClean;
typedef enum Type         Type;
typedef struct Source     Source;

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
};

struct AutoClean {
	Value value;
	Node *type;
};

struct Source {
	char *filename;
	char *content;
};

struct Token {
	Type    type;
	Type    ret_type;
	Source *source;

	char   *name;
	int     space;

	int     used;
	int     start_index;
	int     end_index; // TODO: meybe it needs to be removed

	bool    is_ref;
	bool    is_dec;
	bool    is_global;
	bool    is_param;
	bool    is_variadic;
	bool    is_proto;
	bool    has_clean;
	bool    is_method_call;
	bool    is_pub;
	bool    is_static_call;

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
	EXPAND(AutoClean *, auto_cleans);
};

// ----------------------------------------------------------------------------
// Globals (defined in utils.c)
// ----------------------------------------------------------------------------
extern bool             found_error;
extern bool             enable_debug;
extern bool             enable_san;
extern bool             enable_prep;
extern char            *flags;
extern char            *ura_lib;

extern Source         **sources;
extern int              sources_count;
extern int              sources_size;
extern Token          **tokens;
extern int              tokens_count;
extern int              tokens_size;
extern int              exe_count;
extern char            *synth_list_paths[];
extern int              synth_list_count;
extern Node           **scopes;
extern int              scopes_count;
extern int              scopes_size;
extern Node            *scope;
extern Node            *ura_scope;
extern char            *current_gen_module;

extern char            *dir;
extern char            *base;
extern char            *build_dir;
extern char            *ll_path;

extern Context          context;
extern Module           module;
extern Builder          builder;
extern TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
extern LLVMDIBuilderRef debug_builder;
extern LLVMMetadataRef  debug_compile_unit;
extern LLVMMetadataRef  debug_file;
extern LLVMMetadataRef  debug_scope;

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
// memory
void  *_allocate(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);
void   unuse(Node *node);

// tokenizer / parser helpers
Source *new_source(char *file_name);
void    tokenize(char *filename, int default_space);
void    tokenize_string(char *input, char *fake_name, int default_space);
void    instantiate_list_types(void);
char   *generate_list_source(const char *elem_type_name, const char *struct_name);
Token  *new_token(Type type, int space);
Token  *parse_token(Source *src, int line, int s, int e, Type type, int space);
void    add_token(Token *token);
Node   *new_node(Token *token);
Node   *add_child(Node *node, Node *child);
void    enter_scope(Node *node);
void    exit_scope();
Token  *find(Type type, ...);
bool    within(int space);
bool    includes(Type to_find, ...);
void    setName(Token *token, char *name);
char   *to_string(Type type);
bool    is_data_type(Token *token);
char   *resolve_path(char *path);
Node   *syntax_error();

// variables / functions / structs
Token      *new_variable(Token *token);
void        add_variable(Node *b, Token *token);
Token      *get_variable(char *name);
Node       *new_function(Node *node);
Node       *find_function(char *name);
void        add_function(Node *b, Node *node);
Node       *get_function(char *name);
Node       *new_struct(Node *node);
void        add_struct(Node *b, Node *node);
Node       *get_struct(char *name);
const char *type_to_ura_name(Type type);

// parser nodes — precedence chain
Node *expr_node(), *assign_node(), *and_node(), *or_node();
Node *bitor_node(), *bitxor_node(), *bitnot_node(), *bitand_node();
Node *equality_node(), *comparison_node();
Node *shift_node();
Node *add_sub_node(), *mul_div_node();
Node *as_node();
Node *unary_node();
Node *access_node();
Node *keyword_node();
Node *prime_node();

// parser nodes — statement / value builders
Node *fdec_node(Token *token);
Node *return_node(Token *token);
Node *if_node(Token *token);
Node *while_node(Token *token);
Node *struct_def_node(Token *token);
Node *enum_def_node(Token *token);
Node *module_node(Token *token);
Node *tuple_unpack_node(Token *token);
Node *array_lit_node(Token *token);

// parser nodes — struct internals
Node *pub_node(Node *struct_node, Token *pub_tok);
Node *operator_node(Node *struct_node, Token *op_kw);
Node *delete_node(Node *struct_node, Token *op_kw);

// parser nodes — helpers / atom dispatch
void  params_node(Node *fdec);
void  array_list_type_setup(Token *data_type);
void  synth_list_structs();
char *generate_list_source(const char *elem, const char *sname);

Node *main_node(Token *token);
Node *output_node(Token *token);
Node *fcall_node(Token *token);

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
Type    get_ret_type(Node *node);
void    set_ret_type(Node *node);

// IR/ASM support helpers (defined in utils.c, called from gen.c case dispatchers)
Node   *find_op_overload(Token *left, Token *right, Type op);
void    ir_access_list_struct(Node *node, Node *st);
void    ir_method_call_args(Node *node, Node *func);
void    ir_method_call(Node *node);
void    ir_regular_call_args(Node *node, Node *func);
void    ir_regular_call(Node *node);
void    ir_static_call(Node *node);
bool    try_module_call(Node *node);
void    gen_struct_declaration(Token *token);
void    gen_primitive_declaration(Token *token);
void    propagate_dims(Token *dst, Token *src, Node *lhs_node);
Value   emit_copy_construct(Token *param, Token *arg);
Value   marshal_arg_for_op(Token *param, Node *arg);
Value   gen_operator_call(Node *node, Node *left, Node *right, bool try_copy_ctor);
Value   asm_assign_cast(Node *left, Node *right, Value val);
Value   asm_as_int_to_int(Value src, TypeRef stype, TypeRef ttype);
Value   asm_collect_dims(Node *node);
Value   asm_total_bytes(Value total, TypeRef elem_t);
TypeRef asm_array_lit_elem_type(Node *node);
void    asm_dot_propagate_field_dims(Token *struct_tok, int field_index, Token *target);
void    asm_access_struct_field(Node *node);
bool    asm_access_multidim(Node *node, Value left_value, Value right_ref);
TypeRef asm_access_element_type(Node *node);
Value   asm_access_left_value(Node *node);
Value   marshal_fcall_arg(Token *param, Node *arg, bool is_proto_call);
void    schedule_temp_cleanup(Token *token);
void    call_delete(char *type_name, Value self_ptr);
void    emit_scope_clean(Node *scope_node, int from, Token *skip);
void    asm_fcall_static(Node *node);
void    asm_fcall_marshal_args(Node *node, Value *args, int *count_out, bool is_proto);
void    asm_fcall_unpack_proto_struct(Node *node);
void    asm_fcall_instance(Node *node);
void    append_string_literal_to_fmt(const char *s, char *fmt, int *fc);
void append_struct_with_output_op(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd,
                                  Value out_fn);
void append_struct_default_fmt(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd);
Type append_resolve_type(Token *tok);
void append_output_arg(Token *tok, char *fmt, int *fc, Value *args, int *nargs);
int  output_format_capacity(int argc, Node **argv);
void if_chain_branch(Node *curr, Block if_start, Block end);
void if_chain_else(Node *curr, Block end);
void gen_if_chain(Node *node, Block if_start, Block end);
void asm_return_delete_chain(Node *fdec);
void asm_return_main_globals(void);
void asm_return_tuple(Node *node, Node *fdec);
void asm_return_value(Node *node, Node *fdec);
void gen_struct_emit_nested(Node *node);
void gen_struct_build_type(Node *node);
void gen_struct_emit_delete(Node *node);
void gen_struct_predeclare_methods(Node *node);
void emit_prep_file(Node *scope_node, char *path);
void _branch(Block bloc);
Block   _append_block(char *name);
Value   _add_function(char *name, TypeRef function_type);
Value   build_binary_op(Type op, Value l, Value r);
void    add_auto_clean(Node *parent, Value value, Node *type);
void    schedule_temp_cleanup(Token *token);
void    call_delete(char *type_name, Value self_ptr);
void    emit_scope_clean(Node *scope_node, int from, Token *skip);
TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed);
void    set_debug_location(Token *token);

// LLVM helpers
LLVMIntPredicate  icmp_predicate(Type op);
LLVMRealPredicate fcmp_predicate(Type op);
int               is_float_value(Value v);
Type              assign_base_op(Type assign_op);

// debug / utils
bool  _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void  parse_error(Token *token, const char *fmt, ...);
void  tokenize_error(Source *src, int line, int s, int e, const char *fmt, ...);
int   _debug(char *conv, ...);
void  pnode(Node *node, char *indent);
char *format(const char *fmt, ...);
int   vprint_(File out, char *conv, va_list args);
void *allocate(int len, int size);
char *open_file(char *path_name);
int   parse_escape_seq(char *input, int s, int e, char *buf, int *j);
void  setup_paths(char *path_name);

// codegen helpers (gated callers — declarations always available)
void  resolve_func_type(Node *fdec);
void  emit_func_body(Node *fdec);
Node *find_enclosing(Type type);

// keyword sub-helpers in parse.c
Node *stack_heap_node(Token *token);
Node *typeof_sizeof_node(Token *token);

#endif