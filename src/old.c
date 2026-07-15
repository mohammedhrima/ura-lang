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
			parser_recover((anchor)->space);                                                          \
			return syntax_error();                                                                    \
		}                                                                                            \
	}

#define EXPECT_TOKEN_VOID(anchor, type, fmt, ...)                                                  \
	{                                                                                               \
		Token *find_token = find(type, 0);                                                           \
		if (!find_token) {                                                                           \
			parse_error(anchor, fmt, ##__VA_ARGS__);                                                  \
			parser_recover((anchor)->space);                                                          \
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

#define DATA_TYPES     INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT, ARRAY_TYPE, LIST_TYPE, FN_TYPE
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
	LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS, AS, // Punctuation and Syntax
	RETURN, IF, ELIF, ELSE, WHILE, CONTINUE, BREAK, // Control Flow
	FOR, TO, STEP, IN,
	FDEC, FCALL, PROTO, ARGS, CHILDREN, // Functions
	STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, SYNTAX_ERROR, // Built-ins
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT, // Bitwise
	NULLABLE, // Literals
	MODULE, // Modules
	OPERATOR, // Operator overloading keyword
	PUB, DOUBLE_DOTS, DELETE, // Static dispatch / pub
	END, // end
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
	int     end_index; // TODO: maybe it needs to be removed

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
	EXPAND(AutoClean *, auto_cleans);
};

// ----------------------------------------------------------------------------
// Globals (defined in utils.c)
// ----------------------------------------------------------------------------
extern bool             found_error;
extern int              error_count;
extern int              max_errors;
extern Node            *syntax_error_node;
extern bool             enable_debug;
extern bool             enable_san;
extern bool             enable_prep;
extern char            *flags;
extern char            *ura_lib;
extern const char      *ura_target_os;

extern Source         **sources;
extern int              sources_count;
extern int              sources_size;
extern Token          **tokens;
extern int              tokens_count;
extern int              tokens_size;
extern int              exe_pos;
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
void    set_name(Token *token, char *name);
char   *to_string(Type type);
bool    is_data_type(Token *token);
char   *resolve_path(char *path);
Node   *syntax_error();
void    parser_recover(int indent);

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
Node *for_node(Token *token);
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
void    ir_gen(Node *node);
void    asm_gen(Node *node);
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
void    ir_indirect_call(Node *node, Token *fn_var);
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
void    asm_fcall_indirect(Node *node);
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
void compile(char *path_name) {
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));

	tokenize(path_name, 0);
	if (error_count > 0) return;

	synth_list_structs();
	if (synth_list_count > 0) {
		Token **user_tokens = tokens;
		int     user_count  = tokens_count;
		tokens              = NULL;
		tokens_count        = 0;
		tokens_size         = 0;
		for (int i = 0; i < synth_list_count; i++)
			tokenize(synth_list_paths[i], 0);
		if (tokens_count > 0 && tokens[tokens_count - 1]->type == END) tokens_count--;
		for (int i = 0; i < user_count; i++) {
			resize_array(tokens, Token *, tokens_size, tokens_count);
			tokens[tokens_count++] = user_tokens[i];
		}
		free(user_tokens);
	}

	for (int i = 0; tokens[i]; i++)
		debug(GREEN("%k\n"), tokens[i]);

#if AST
	ura_scope = new_node(new_token(ID, -TAB));
	set_name(ura_scope->token, "ura-scope");
	enter_scope(ura_scope);
	while (!find(END, 0)) {
		Node *child = expr_node();
		// after expr_node, EXPECT_TOKEN/parse_error sites have already
		// called parser_recover(), so found_error is back to false here.
		// We just check whether the node we got is the SYNTAX_ERROR
		// sentinel — if so, skip it and keep parsing the next statement.
		if (!child || child == syntax_error_node) continue;
		Token *t      = child->token;
		Token *anchor = (t->type == ASSIGN && child->left) ? child->left->token : t;
		bool   ok = includes(anchor->type, FDEC, STRUCT_DEF, ENUM_DEF, MODULE, PROTO, 0) || anchor->is_dec;
		if (!ok) {
			parse_error(t, "only declarations are allowed at top level");
			parser_recover(t->space);
			continue;
		}
		add_child(ura_scope, child);
	}

	if (error_count > 0) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
#endif

#if IR
	debug(GREEN("===========================================\n"));
	debug(GREEN("GENERATE INTERMEDIATE REPRESENTATION\n"));
	debug(GREEN("===========================================\n"));
	// Pre-register every top-level FDEC so forward references work across
	// flat top-level functions (already worked for struct methods via
	// ir_struct_def's pre-pass; same idea, top-level scope).
	for (int i = 0; i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		Node *fn    = NULL;
		if (child->token->type == FDEC)                                  fn = child;
		else if (child->token->type == PROTO && child->left
		         && child->left->token->type == FDEC)                    fn = child->left;
		if (fn && fn->token->name && !find_function(fn->token->name))
			add_function(scope, fn);
	}
	for (int i = 0; i < ura_scope->children_count; i++)
		ir_gen(ura_scope->children[i]);
#endif

	if (error_count > 0) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER IR GENERATION\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

	setup_paths(path_name);

	if (enable_prep) {
		char *prep_path = format("%s/%s", build_dir, base);
		char *full_path = format("%s.prep.ura", prep_path);
		free(prep_path);
		emit_prep_file(ura_scope, full_path);
		free(full_path);
	}

#if ASM
	init(path_name);
	// Two passes so calls reference fully-typed callees:
	// 1) Generate STRUCT_DEFs first (so STRUCT_CALL params have llvm.struct_type).
	// 2) Pre-declare every top-level FDEC's signature.
	// 3) Emit the rest of the top-level tree (fn bodies + globals).
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == STRUCT_DEF)
			asm_gen(child);
	}
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		Node *fn    = NULL;
		if (child->token->type == FDEC)                                  fn = child;
		else if (child->token->type == PROTO && child->left
		         && child->left->token->type == FDEC)                    fn = child->left;
		if (fn && fn->token->name) {
			bool is_method = strchr(fn->token->name, '.') != NULL;
			bool is_main   = strcmp(fn->token->name, "main") == 0;
			if (fn->token->used > 0 || is_method || is_main)
				resolve_func_type(fn);
		}
	}
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == STRUCT_DEF) continue;
		asm_gen(child);
	}
	if (error_count == 0) finalize(ll_path);
#endif
}

// ============================================================================
// MAIN
// ============================================================================
int main(int argc, char **argv) {
	ura_lib = getenv("URA_LIB");
	if (CHECK(!ura_lib, "URA_LIB not set")) return 1;
	if (CHECK(argc < 2, "usage: ura <file.ura> [-O0|-O1|-O2|-O3|-Os|-Oz] [-san]"
	                    " [-o output]"))
		return 1;

	char *output  = NULL;
	char *entry   = NULL;
	bool  no_exec = false;
	(void)no_exec;
	for (int i = 1; i < argc && !found_error; i++) {
		char *a = argv[i];
#define MATCH(name, var, val)                                                                      \
	if (strcmp(a, name) == 0) {                                                                     \
		var = val;                                                                                   \
		continue;                                                                                    \
	}
		MATCH("-san", enable_san, true);
		MATCH("-O0", flags, PASSES_O0);
		MATCH("-no-exec", no_exec, true) MATCH("-O1", flags, PASSES_O1);
		MATCH("-no-debug", enable_debug, false);
		MATCH("-O2", flags, PASSES_O2);
		MATCH("-O3", flags, PASSES_O3);
		MATCH("-Os", flags, PASSES_Os);
		MATCH("-Oz", flags, PASSES_Oz);
		MATCH("-prep", enable_prep, true);
#undef MATCH
		if (strcmp(a, "-o") == 0) {
			CHECK(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		} else if (a[0] != '-') {
			size_t n = strlen(a);
			CHECK(n <= 4 || strcmp(a + n - 4, ".ura") != 0, "invalid file: %s\n", a);
			CHECK(entry != NULL, "you can compile only one file "
			                     "try importing them in one files");
			entry = a;
		} else {
			CHECK(true, "unknown flag: %s\n", a);
		}
	}
	if (CHECK(!entry, "required .ura files as arguments to compile")) return 1;
	if (found_error)                                                  return 1;

	if (!output) output = "exe.out";

	compile(entry);

#if ASM
	if (ll_path && !no_exec && !found_error) {
		char *san = enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
		char *cmd = format("clang%s \"%s\" -o \"%s\"", san, ll_path, output);
		CHECK(system(cmd) != 0, "linking failed\n");
		free(cmd);
	}
#endif
	free_memory();
	return error_count > 0 ? 1 : 0;
}

const Keyword keywords[] = {
    {"int", INT, 1, 1},           {"bool", BOOL, 1, 1},
    {"chars", CHARS, 1, 1},       {"char", CHAR, 1, 1},
    {"float", FLOAT, 1, 1},       {"void", VOID, 1, 1},
    {"long", LONG, 1, 1},         {"pointer", CHARS, 1, 1},
    {"short", SHORT, 1, 1},       {"if", IF, 0, 0},
    {"elif", ELIF, 0, 0},         {"else", ELSE, 0, 0},
    {"while", WHILE, 0, 0},       {"for", FOR, 0, 0},
    {"to", TO, 0, 0},             {"step", STEP, 0, 0},
    {"in", IN, 0, 0},             {"fn", FDEC, 0, 0},
    {"return", RETURN, 0, 0},     {"break", BREAK, 0, 0},
    {"continue", CONTINUE, 0, 0}, {"ref", REF, 0, 0},
    {"struct", STRUCT_DEF, 0, 0}, {"enum", ENUM_DEF, 0, 0},
    {"proto", PROTO, 0, 0},       {"mod", MODULE, 0, 0},
    {"operator", OPERATOR, 0, 0}, {"as", AS, 0, 0},
    {"pub", PUB, 0, 0},           {"delete", DELETE, 0, 0},
    {"and", AND, 0, 1},           {"or", OR, 0, 1},
    {"is", EQUAL, 0, 1},          {"not", NOT, 0, 1},
    {"typeof", TYPEOF, 0, 1},     {"sizeof", SIZEOF, 0, 1},
    {"stack", STACK, 0, 1},       {"heap", HEAP, 0, 1},
    {"array", ARRAY_TYPE, 0, 1},  {"List", LIST_TYPE, 0, 1},
    {"null", NULLABLE, 0, 1},
};

const Keyword specials[] = {
    {"...", VARIADIC, 0, 0},   {".", DOT, 0, 0},         {"::", DOUBLE_DOTS, 0, 0},
    {":", DOTS, 0, 0},         {"+=", ADD_ASSIGN, 0, 0}, {"-=", SUB_ASSIGN, 0, 0},
    {"*=", MUL_ASSIGN, 0, 0},  {"/=", DIV_ASSIGN, 0, 0}, {"%=", MOD_ASSIGN, 0, 0},
    {"!=", NOT_EQUAL, 0, 0},   {"!", NOT, 0, 0},         {"==", EQUAL, 0, 0},
    {"<<", LSHIFT, 0, 0},      {">>", RSHIFT, 0, 0},     {"<=", LESS_EQUAL, 0, 0},
    {">=", GREAT_EQUAL, 0, 0}, {"<", LESS, 0, 0},        {">", GREAT, 0, 0},
    {"=", ASSIGN, 0, 0},       {"+", ADD, 0, 0},         {"-", SUB, 0, 0},
    {"*", MUL, 0, 0},          {"/", DIV, 0, 0},         {"%", MOD, 0, 0},
    {"(", LPAR, 0, 0},         {")", RPAR, 0, 0},        {"[", LBRA, 0, 0},
    {"]", RBRA, 0, 0},         {",", COMA, 0, 0},        {"&&", AND, 0, 0},
    {"||", OR, 0, 0},          {"&", BAND, 0, 0},        {"|", BOR, 0, 0},
    {"^", BXOR, 0, 0},         {"~", BNOT, 0, 0},
};

// TODO: use default_space in int s, int e
Token *parse_token(Source *src, int line, int s, int e, Type type, int space) {
	Token *new       = new_token(type, space);
	char *input      = src->content;
	new->line        = line;
	new->source      = src;
	new->start_index = s;
	new->end_index   = e;

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

		if (strcmp(new->name, "True") == 0 || strcmp(new->name, "False") == 0) {
			new->type       = BOOL;
			new->Bool.value = (new->name[0] == 'T');
			set_name(new, NULL);
			break;
		}

		for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
			if (strcmp(new->name, keywords[i].name) != 0) continue;
			new->type   = keywords[i].type;
			new->is_dec = keywords[i].is_dec;
			if (keywords[i].is_dec || keywords[i].clear_name) set_name(new, NULL);
			break;
		}
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
		char buf[4] = {0};
		int  j      = 0;
		if (e == s + 1) {
			buf[j++] = input[s];
		} else {
			parse_escape_seq(input, s, e, buf, &j);
			if (buf[0] == '\\' && input[s + 1] != '\\')
				tokenize_error(src, line, s, e, "unknown escape character: \\%c", input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}
	default: break;
	}
	return new;
}

// Conditional-compile frame stack. `@if linux:` pushes; `@elif`/`@else` updates
// the top frame; a non-directive line at indent <= the frame's @if-indent pops.
// `active` means tokens inside this frame are emitted; `any_matched` records
// whether any branch in the chain has already been chosen (so later branches
// don't double-fire even if their condition is true).
typedef struct CondFrame {
	int  indent;
	bool active;
	bool any_matched;
} CondFrame;

#define MAX_COND_DEPTH 32
static CondFrame cond_stack[MAX_COND_DEPTH];
static int       cond_top = -1;

static bool cond_emitting(void) {
	for (int j = 0; j <= cond_top; j++)
		if (!cond_stack[j].active) return false;
	return true;
}
static bool cond_outer_active(void) {
	for (int j = 0; j < cond_top; j++)
		if (!cond_stack[j].active) return false;
	return true;
}

void tokenize(char *file_name, int default_space) {
	debug(GREEN("TOKENIZE: [%s]\n"), file_name);
	Source *src = new_source(file_name);
	if (found_error) return;
	char      *input = src->content;

	int        space    = default_space;
	int        line     = 1;
	bool       new_line = true;
	static int calling_use;

	int        i = 0;
	int        s = i;
	for (; input[i] && !found_error;) {
		s      = i;
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
			if (strncmp(input + i, "*/", 2) != 0) {
				tokenize_error(src, line, s, s + 2, "unterminated block comment, expected '*/'");
				break;
			}
			i += 2;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}

		// === Conditional compilation: `@if name:` / `@elif name:` / `@else:`
		// Runs once per line at the first non-whitespace char. `space` is the
		// current line's indent. Three jobs:
		//   1. Pop any frames we've dedented out of (unless the current line is
		//      `@elif`/`@else` matching the top frame's indent — that's a
		//      continuation, not an exit).
		//   2. Handle a directive line: push (@if) / update (@elif, @else) the
		//      top frame, then consume the rest of the line.
		//   3. If we're inside an inactive branch, skip the line.
		if (new_line) {
			bool starts_elif_or_else =
			    input[i] == '@'
			    && (strncmp(input + i, "@elif", 5) == 0 || strncmp(input + i, "@else", 5) == 0);
			while (cond_top >= 0 && space <= cond_stack[cond_top].indent
			       && !(starts_elif_or_else && space == cond_stack[cond_top].indent))
				cond_top--;

			if (input[i] == '@' && strncmp(input + i, "@if", 3) == 0
			    && (input[i + 3] == ' ' || input[i + 3] == '\t')) {
				i += 3;
				while (input[i] == ' ' || input[i] == '\t') i++;
				int ns = i;
				while (isalnum(input[i]) || input[i] == '_') i++;
				char *name = strndup(input + ns, i - ns);
				while (input[i] == ' ' || input[i] == '\t') i++;
				if (input[i] != ':') {
					tokenize_error(src, line, ns, i, "expected ':' after @if name");
					free(name);
					break;
				}
				i++;
				while (input[i] && input[i] != '\n') i++;
				if (cond_top + 1 >= MAX_COND_DEPTH) {
					tokenize_error(src, line, ns, i, "too many nested @if directives");
					free(name);
					break;
				}
				bool match = (ura_target_os && strcmp(name, ura_target_os) == 0);
				free(name);
				cond_top++;
				cond_stack[cond_top].indent      = space;
				cond_stack[cond_top].active      = match && cond_outer_active();
				cond_stack[cond_top].any_matched = match;
				continue;
			}
			if (input[i] == '@' && strncmp(input + i, "@elif", 5) == 0
			    && (input[i + 5] == ' ' || input[i + 5] == '\t')) {
				if (cond_top < 0 || space != cond_stack[cond_top].indent) {
					tokenize_error(src, line, i, i + 5, "@elif without matching @if at same indent");
					break;
				}
				i += 5;
				while (input[i] == ' ' || input[i] == '\t') i++;
				int ns = i;
				while (isalnum(input[i]) || input[i] == '_') i++;
				char *name = strndup(input + ns, i - ns);
				while (input[i] == ' ' || input[i] == '\t') i++;
				if (input[i] != ':') {
					tokenize_error(src, line, ns, i, "expected ':' after @elif name");
					free(name);
					break;
				}
				i++;
				while (input[i] && input[i] != '\n') i++;
				bool match = (ura_target_os && strcmp(name, ura_target_os) == 0);
				free(name);
				if (cond_stack[cond_top].any_matched) {
					cond_stack[cond_top].active = false;
				} else {
					cond_stack[cond_top].active      = match && cond_outer_active();
					cond_stack[cond_top].any_matched = match;
				}
				continue;
			}
			if (input[i] == '@' && strncmp(input + i, "@else", 5) == 0
			    && (input[i + 5] == ':' || input[i + 5] == ' ' || input[i + 5] == '\t')) {
				if (cond_top < 0 || space != cond_stack[cond_top].indent) {
					tokenize_error(src, line, i, i + 5, "@else without matching @if at same indent");
					break;
				}
				while (input[i] && input[i] != ':' && input[i] != '\n') i++;
				if (input[i] == ':') i++;
				while (input[i] && input[i] != '\n') i++;
				if (cond_stack[cond_top].any_matched) cond_stack[cond_top].active = false;
				else {
					cond_stack[cond_top].active      = cond_outer_active();
					cond_stack[cond_top].any_matched = true;
				}
				continue;
			}

			if (!cond_emitting()) {
				while (input[i] && input[i] != '\n') i++;
				continue;
			}
		}

		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\' && input[i + 1]) i++;
				i++;
			}
			if (input[i] != '\"') {
				tokenize_error(src, line, s, s + 1, "unterminated string literal, expected '\"'");
				break;
			}
			i++;
			parse_token(src, line, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\' && input[i + 1]) i++;
			if (input[i] && input[i] != '\'')     i++;
			if (input[i] != '\'')                 {
				tokenize_error(src, line, s, s + 1, "unterminated character literal, expected \"'\"");
				break;
			}
			i++;
			parse_token(src, line, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(src, line, s, i, FLOAT, space);
			} else parse_token(src, line, s, i, INT, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, i, i + 1, "expected '\"' after `use`");
					break;
				}
				i++;
				int s = i;
				while (input[i] && input[i] != '\"' && input[i] != '\n')
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, s - 1, s,
					               "unterminated `use` path, expected closing '\"'");
					break;
				}
				i++;
				char *use = strndup(input + s, i - s - 1);
				char *tmp = format("%s.ura", use);
				free(use);
				use = tmp;
				if (use[0] == '@') {
					char *rest = use + 1;
					if (*rest == '/') rest++;
					tmp = format("%s/%s", ura_lib, rest);
					free(use);
					use = tmp;
				} else if (use[0] != '/') {
					char *dir_copy = strdup(file_name);
					char *dir_name = dirname(dir_copy);
					tmp            = format("%s/%s", dir_name, use);
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
				if (input[i] != '\"') {
					tokenize_error(src, line, i, i + 1, "expected '\"' after `link`");
					break;
				}
				i++;
				int link_s = i;
				while (input[i] && input[i] != '\"' && input[i] != '\n')
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, link_s - 1, link_s,
					               "unterminated `link` path, expected closing '\"'");
					break;
				}
				i++;
			} else parse_token(src, line, s, i, ID, space);
			continue;
		}

		bool found = false;
		for (size_t j = 0; j < sizeof(specials) / sizeof(*specials); j++) {
			size_t len = strlen(specials[j].name);
			if (strncmp(specials[j].name, input + i, len) == 0) {
				i += len;
				parse_token(src, line, s, i, specials[j].type, space);
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		tokenize_error(src, line, i, i + 1, "unexpected character '%c'", c);
		break;
	}
	if (!calling_use) parse_token(src, line, s, i, END, -1);
}

Token *new_token(Type type, int space) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

Token *copy_token(Token *token) {
	if (token == NULL) return NULL;
	Token *new = allocate(1, sizeof(Token));
	memcpy(new, token, sizeof(Token));
	new->name = NULL;
	if (token->name)        set_name(new, token->name);
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

void add_token(Token *token) {
	resize_array(tokens, Token *, tokens_size, tokens_count);
	tokens[tokens_count++] = token;
}

void set_name(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *add_child(Node *node, Node *child) {
	// Skip the SYNTAX_ERROR sentinel — it's a single static node returned
	// by every failed parse, so adding it as a child more than once would
	// UAF on free_memory (same pointer freed twice).
	if (child == syntax_error_node) return child;
	if (child) {
		resize_array(node->children, Node *, node->children_size, node->children_count);
		child->token->space                    = node->token->space + TAB;
		node->children[node->children_count++] = child;
	}
	return child;
}

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

Node *syntax_error_node;

Node *syntax_error() {
	// Note: do NOT set found_error here. parse_error() already records the
	// error in error_count and sets found_error; the caller has typically
	// already invoked parser_recover() to clear that flag and skip past
	// the bad tokens. Setting found_error again here would re-block
	// find()/within() and prevent further error reporting.
	if (syntax_error_node == NULL)
		syntax_error_node = new_node(new_token(SYNTAX_ERROR, -1));
	if (exe_pos >= 0 && exe_pos < tokens_count && tokens[exe_pos] && tokens[exe_pos]->source)
		syntax_error_node->token->source = tokens[exe_pos]->source;
	return syntax_error_node;
}

// Advance to a sync point and clear the scoped-error flag.
// A sync point is the next token that is BOTH at-or-shallower than
// `indent` AND on a different line from where we currently are. This
// guarantees we always cross at least one newline and skip the broken
// remainder of the current statement (otherwise we'd loop forever
// when the error fires mid-line on a token at the target indent —
// e.g. `operator () Vec:` where `(` shares the indent with `operator`).
void parser_recover(int indent) {
	int start_line = (exe_pos < tokens_count) ? tokens[exe_pos]->line : -1;
	while (exe_pos < tokens_count && tokens[exe_pos]->type != END) {
		Token *t = tokens[exe_pos];
		if (t->space <= indent && t->line != start_line) break;
		exe_pos++;
	}
	found_error = false;
}

Token *find(Type type, ...) {
	if (found_error) return NULL;
	// Past-the-end: treat as if we hit END so callers don't deref a NULL slot
	// and the top-level `while (!find(END, 0))` loop terminates.
	if (exe_pos >= tokens_count) {
		va_list ap;
		va_start(ap, type);
		bool wants_end = (type == END);
		while (!wants_end && type) {
			type      = va_arg(ap, Type);
			wants_end = (type == END);
		}
		if (wants_end && tokens_count > 0) return tokens[tokens_count - 1];
		return NULL;
	}
	va_list ap;
	va_start(ap, type);
	while (type) {
		if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
		type = va_arg(ap, Type);
	}
	return NULL;
}

bool within(int space) {
	Token *curr = tokens[exe_pos];
	return !found_error && curr->space > space && curr->type != END;
}

bool is_data_type(Token *token) { return includes(token->type, DATA_TYPES, 0); }

bool is_type_starter(Token *token) {
	return is_data_type(token) || token->type == FDEC;
}

Token *parse_type_in_signature(int anchor_space);

Token *parse_fn_type(int anchor_space) {
	Token *fn_kw = find(FDEC, 0);
	if (!fn_kw) return NULL;

	Token *ftype       = new_token(FN_TYPE, fn_kw->space);
	ftype->source      = fn_kw->source;
	ftype->line        = fn_kw->line;
	ftype->start_index = fn_kw->start_index;
	ftype->end_index   = fn_kw->end_index;

	if (!find(LPAR, 0)) {
		parse_error(fn_kw, "expected '(' after `fn` in type");
		parser_recover(anchor_space);
		return NULL;
	}

	// Parse comma-separated parameter types.
	while (tokens[exe_pos]->type != RPAR && tokens[exe_pos]->type != END) {
		Token *param_t = parse_type_in_signature(anchor_space);
		if (!param_t) return NULL;
		resize_array(ftype->Fn.params, Token *, ftype->Fn.params_size, ftype->Fn.params_count);
		ftype->Fn.params[ftype->Fn.params_count++] = param_t;
		if (tokens[exe_pos]->type == COMA) exe_pos++;
		else if (tokens[exe_pos]->type != RPAR) {
			parse_error(fn_kw, "expected ',' or ')' in fn-type parameter list");
			parser_recover(anchor_space);
			return NULL;
		}
	}
	if (!find(RPAR, 0)) {
		parse_error(fn_kw, "expected ')' to close fn-type parameter list");
		parser_recover(anchor_space);
		return NULL;
	}

	ftype->Fn.ret = parse_type_in_signature(anchor_space);
	if (!ftype->Fn.ret) return NULL;
	return ftype;
}

Token *parse_type_in_signature(int anchor_space) {
	if (tokens[exe_pos]->type == FDEC) return parse_fn_type(anchor_space);
	Token *t = find(DATA_TYPES, ID, 0);
	if (!t) {
		parse_error(tokens[exe_pos], "expected type");
		parser_recover(anchor_space);
		return NULL;
	}
	if (includes(t->type, ARRAY_TYPE, LIST_TYPE, 0)) array_list_type_setup(t);
	return t;
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

// ============================================================================
// AST BUILDERS
// ============================================================================
Node *expr_node() { return assign_node(); }

// Hand-rolled equivalent of AST_NODE for assign_node, with one extra step:
// when the LHS is a primitive/struct declaration (is_dec on the operand), we
// stamp `is_dec` on the ASSIGN token itself so the -prep emitter can later
// distinguish a typed declaration (`x int = 1`) from a re-assignment (`x = 1`).
// ir_id replaces the LHS node->token with the canonical variable Token, which
// preserves is_dec on the operand — so we can't tell decl vs use from the
// operand alone after IR.
Node *assign_node() {
	Node  *left = or_node();
	Token *token;
	while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
	                     DIV_ASSIGN, MOD_ASSIGN, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = or_node();
		if (token->type == ASSIGN && left && left->token && left->token->is_dec)
			token->is_dec = true;
		left = node;
	}
	return left;
}
AST_NODE(or_node, and_node, OR)
AST_NODE(and_node, bitor_node, AND)
AST_NODE(bitor_node, bitxor_node, BOR)
AST_NODE(bitxor_node, bitand_node, BXOR)
AST_NODE(bitand_node, equality_node, BAND)
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL)
AST_NODE(comparison_node, shift_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL)
AST_NODE(shift_node, add_sub_node, LSHIFT, RSHIFT)
AST_NODE(add_sub_node, mul_div_node, ADD, SUB)
AST_NODE(mul_div_node, as_node, MUL, DIV, MOD)

Node *as_node() {
	Node  *left = unary_node();
	Token *token;
	while ((token = find(AS, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = unary_node();
		Token *to   = node->right->token;
		// TODO: casting to struct type
		if (!to->is_dec) {
			parse_error(token, "expected data type after 'as'");
			parser_recover(token->space);
			return syntax_error();
		}
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
		node->right->token->source    = token->source;
		node->right->token->line      = token->line;
		node->right->token->Int.value = token->type == SUB ? -1 : 1;
		node->token->type             = MUL;
		return node;
	}
	return access_node();
}

Node *access_node() // . [] ::
{
	Node  *left = keyword_node();
	Token *token;
	while ((token = find(DOT, LBRA, DOUBLE_DOTS, 0))) {
		Node *node = new_node(token);
		switch (token->type) {
		case DOT: {
			node->left  = left;
			node->right = keyword_node();

			if (!node->right) {
				parse_error(token, "expected attribute or method name after '.'");
				parser_recover(token->space);
				return syntax_error();
			}
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
			if (!node->right) {
				parse_error(token, "expected expression between '[' and ']'");
				parser_recover(token->space);
				return syntax_error();
			}
			EXPECT_TOKEN(token, RBRA, "expected ']' to close index access");
			break;
		}
		case DOUBLE_DOTS: {
			Node *call = keyword_node();
			if (!call || call->token->type != FCALL) {
				parse_error(token, "expected function call after '::'");
				parser_recover(token->space);
				return syntax_error();
			}
			char *name = format("%s.%s", left->token->name, call->token->name);
			set_name(call->token, name);
			free(name);
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

Node *keyword_node() {
	Token *token = NULL;
	Node  *node;

	if ((token = find(PUB, 0))) {
		parse_error(token, "'pub' is only valid inside struct definitions");
		parser_recover(token->space);
		return syntax_error();
	}

	if ((token = find(MODULE, 0)))                    return module_node(token);
	if ((token = find(STRUCT_DEF, 0)))                return struct_def_node(token);
	if ((token = find(ENUM_DEF, 0)))                  return enum_def_node(token);
	if ((token = find(IF, 0)))                        return if_node(token);
	if ((token = find(WHILE, 0)))                     return while_node(token);
	if ((token = find(FOR, 0)))                       return for_node(token);
	if ((token = find(FDEC, 0)))                      return fdec_node(token);
	if ((token = find(RETURN, 0)))                    return return_node(token);
	if ((token = find(BREAK, CONTINUE, NULLABLE, 0))) return new_node(token);
	if ((token = find(STACK, HEAP, 0)))               return stack_heap_node(token);
	if ((token = find(TYPEOF, SIZEOF, 0)))            return typeof_sizeof_node(token);
	if ((token = find(LBRA, 0)))                      return array_lit_node(token);

	if ((token = find(REF, 0))) {
		// Leading-ref declaration: `ref name type [= init]` for primitives,
		// `ref name Struct = init` for structs, `ref name fn(...) ret` for fn-types.
		// Anything else after REF is the unary ref-of expression form (`p = ref x`).
		bool is_decl = false;
		if (exe_pos + 1 < tokens_count
		    && tokens[exe_pos]->type == ID
		    && tokens[exe_pos]->line == token->line
		    && (is_data_type(tokens[exe_pos + 1])
		        || tokens[exe_pos + 1]->type == FDEC
		        || (tokens[exe_pos + 1]->type == ID
		            && get_struct(tokens[exe_pos + 1]->name)))) {
			is_decl = true;
		}
		if (is_decl) {
			Node *decl = prime_node();
			if (decl && decl != syntax_error_node && decl->token) {
				decl->token->is_ref = true;
				if (decl->token->type == STRUCT_CALL) {
					EXPECT_TOKEN(token, ASSIGN,
					    "'%s': ref must be initialized at declaration",
					    decl->token->name);
					exe_pos--;
				}
			}
			return decl;
		}
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
	if ((token = find(LPAR, 0))) {
		node = (tokens[exe_pos]->type != RPAR) ? expr_node() : NULL;
		EXPECT_TOKEN(token, RPAR, "expected ')' to close parenthesized expression");
		return node;
	}
	if ((token = find(PROTO, 0))) {
		if (includes(tokens[exe_pos]->type, FDEC, STRUCT_DEF, 0))
			tokens[exe_pos]->is_proto = true;
		else {
			parse_error(token, "expected `fn` or `struct` after `proto`");
			parser_recover(token->space);
			return syntax_error();
		}
		return expr_node();
	}
	return prime_node();
}

Node *prime_node() {
	Token *token = find(ID, DATA_TYPES, 0);
	if (!token) {
		Token *cur = tokens[exe_pos];
		parse_error(cur, "unexpected token in expression");
		parser_recover(cur->space);
		return syntax_error();
	}

	if (includes(token->type, DATA_TYPES) && !token->name && !token->is_dec) return new_node(token);
	if (token->is_dec)                                                       return new_node(token);

	if (token->type == ID && is_type_starter(tokens[exe_pos]) &&
	    tokens[exe_pos]->line == token->line) {
		Token *data_type;
		if (tokens[exe_pos]->type == FDEC) {
			data_type = parse_fn_type(token->space);
			if (!data_type) return syntax_error();
		} else {
			data_type = tokens[exe_pos++];
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0))
				array_list_type_setup(data_type);
		}
		if (tokens[exe_pos]->type == REF) {
			parse_error(tokens[exe_pos],
			    "trailing `ref` is no longer supported; write `ref %s %s` instead",
			    token->name,
			    data_type->type == STRUCT_CALL && data_type->Struct.ptr
			        ? data_type->Struct.ptr->token->name
			        : to_string(data_type->type));
			parser_recover(token->space);
			return syntax_error();
		}
		set_name(data_type, token->name);
		data_type->is_dec = true;
		if (tokens[exe_pos]->type == COMA && exe_pos + 2 < tokens_count &&
		    tokens[exe_pos + 1]->type == ID && is_data_type(tokens[exe_pos + 2]))
			return tuple_unpack_node(data_type);
		return new_node(data_type);
	}

	Node *st_dec = NULL;
	if (token->type == ID && tokens[exe_pos]->type == ID &&
	    (st_dec = get_struct(tokens[exe_pos]->name))) {
		find(ID, 0);
		if (tokens[exe_pos]->type == REF) {
			parse_error(tokens[exe_pos],
			    "trailing `ref` is no longer supported; write `ref %s %s` instead",
			    token->name, st_dec->token->name);
			parser_recover(token->space);
			return syntax_error();
		}
		token->type       = STRUCT_CALL;
		token->is_dec     = true;
		token->Struct.ptr = st_dec;
		return new_node(token);
	}

	if (token->type == ID && find(LPAR, 0)) {
		if (strcmp(token->name, "main") == 0)        return main_node(token);
		else if (strcmp(token->name, "output") == 0) return output_node(token);
		return fcall_node(token);
	}

	if (token->type == ID) return new_node(token);
	// CHECK(1, "Unexpected token %k line %d", tokens[exe_pos], tokens[exe_pos]->line);
	return syntax_error();
}

void fdec_tuple_elem(Node *node) {
	Token *token2 = NULL;
	if (includes(tokens[exe_pos]->type, ARRAY_TYPE, LIST_TYPE, 0)) {
		token2         = tokens[exe_pos++];
		Type container = token2->type;
		EXPECT_TOKEN_VOID(token2, LBRA, "expected '[' after %s in tuple type",
		                  container == LIST_TYPE ? "list" : "array");
		Token *et = find(DATA_TYPES, ID, 0);
		if (!et) {
			parse_error(token2, "expected element type in tuple %s",
			            container == LIST_TYPE ? "list" : "array");
			syntax_error();
			return;
		}
		EXPECT_TOKEN_VOID(token2, RBRA, "expected ']' to close tuple %s type",
		                  container == LIST_TYPE ? "list" : "array");
		if (container == LIST_TYPE) {
			const char *ename = et->type == ID ? et->name : type_to_ura_name(et->type);
			char        sname[LIST_NAME_MAX];
			snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
			Node *list_st = get_struct(sname);
			if (!list_st) {
				parse_error(token2, "list struct '%s' not found", sname);
				syntax_error();
				return;
			}
			token2->type       = STRUCT_CALL;
			token2->Struct.ptr = list_st;
		} else {
			token2->Array.sub_type = et->type;
			token2->ret_type       = container;
			if (et->type == ID) {
				Node *sd = get_struct(et->name);
				if (sd) {
					token2->Array.sub_type   = STRUCT_CALL;
					token2->Array.struct_ptr = sd;
				}
			}
		}
	} else if (tokens[exe_pos]->type == ID) {
		token2   = tokens[exe_pos++];
		Node *st = get_struct(token2->name);
		if (!st) {
			parse_error(token2, "unknown type '%s' in tuple", token2->name);
			syntax_error();
			return;
		}
		token2->type       = STRUCT_CALL;
		token2->Struct.ptr = st;
	} else if (is_data_type(tokens[exe_pos])) {
		token2 = tokens[exe_pos++];
	} else {
		parse_error(tokens[exe_pos], "unexpected token in tuple return type");
		syntax_error();
		return;
	}
	resize_array(node->token->Tuple.types, Token *, node->token->Tuple.types_size,
	             node->token->Tuple.types_count);
	node->token->Tuple.types[node->token->Tuple.types_count++] = token2;
}

void fdec_tuple_ret(Node *node) {
	exe_pos++;
	node->token->ret_type = TUPLE;
	while (!found_error && tokens[exe_pos]->type != RPAR) {
		fdec_tuple_elem(node);
		if (found_error)                     return;
		if (tokens[exe_pos]->type == COMA) exe_pos++;
	}
	EXPECT_TOKEN_VOID(node->token, RPAR, "expected ')' to close tuple return type list");
}

void fdec_single_ret(Node *node) {
	bool is_ref_ret = false;
	if (tokens[exe_pos]->type == REF) {
		is_ref_ret = true;
		exe_pos++;
	}
	if (!is_data_type(tokens[exe_pos]) && tokens[exe_pos]->type != ID) {
		parse_error(node->token, "expected return type after function declaration");
		syntax_error();
		return;
	}
	Token *ret_tok = tokens[exe_pos++];
	if (ret_tok->type == ID) {
		Node *st = get_struct(ret_tok->name);
		if (!st) {
			parse_error(ret_tok, "unknown return type '%s'", ret_tok->name);
			syntax_error();
			return;
		}
		node->token->ret_type   = STRUCT_CALL;
		node->token->Struct.ptr = st;
	} else node->token->ret_type = ret_tok->type;
	node->token->is_ref = is_ref_ret;
}

void fdec_body(Node *node) {
	Token *next = find(DOTS, 0);
	if (!found_error && !next) {
		parse_error(node->token, "expected ':' after function declaration");
		syntax_error();
		return;
	}
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
		if (node->token->ret_type != VOID) {
			if (!child || child->token->type != RETURN) {
				parse_error(node->token, "expected return statement in function '%s'",
				            node->token->name);
				syntax_error();
				return;
			}
		} else {
			Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
			ret->left = new_node(new_token(VOID, node->token->space + TAB));
			add_child(node, ret);
		}
	}
}

Node *fdec_node(Token *token) {
	Node *node = new_node(token);
	if (!node->token->name || strcmp(node->token->name, "fn") == 0) {
		Token *fname = find(ID, 0);
		if (!fname) {
			parse_error(token, "expected identifier after `fn` keyword");
			parser_recover(token->space);
			return syntax_error();
		}
		set_name(node->token, fname->name);
	}

	Node *struct_owner = NULL;
	Node *parent       = scopes[scopes_count];
	if (parent && parent->token->type == STRUCT_DEF) struct_owner = parent;

	enter_scope(node);
	EXPECT_TOKEN(token, LPAR, "expected '(' after function declaration");
	params_node(node);
	if (found_error) return syntax_error();

	if (struct_owner && !node->token->is_pub) {
		Token *self_tok = new_token(STRUCT_CALL, node->token->space);
		set_name(self_tok, "self");
		self_tok->Struct.ptr = struct_owner;
		self_tok->is_dec     = true;
		self_tok->is_ref     = true;
		add_child(node->left, new_node(self_tok));
	}

	if (tokens[exe_pos]->type == LPAR) fdec_tuple_ret(node);
	else fdec_single_ret(node);
	if (found_error) return syntax_error();

	if (tokens[exe_pos]->type == REF) {
		parse_error(tokens[exe_pos],
		    "trailing `ref` on return type is no longer supported; write `ref <type>` before the type");
		syntax_error();
		return node;
	}
	if (!node->token->is_proto) fdec_body(node);
	if (found_error)            return syntax_error();
	exit_scope();
	return node;
}

Node *return_node(Token *token) {
	Node *node = new_node(token);
	for (int i = scopes_count; i >= 0; i--) {
		Node *curr = scopes[i];
		if (curr->token->type == FDEC) {
			if (curr->token->ret_type == VOID) {
				node->left              = copy_node(node);
				node->left->token->type = VOID;
			} else if (curr->token->ret_type == TUPLE) {
				add_child(node, expr_node());
				while (tokens[exe_pos]->type == COMA) {
					exe_pos++;
					add_child(node, expr_node());
				}
			} else node->left = expr_node();
			break;
		}
	}
	return node;
}

Node *stack_heap_node(Token *token) {
	Node *node = new_node(token);
	EXPECT_TOKEN(token, LBRA, "expected '[' after %s", to_string(token->type));
	int depth = 1;
	while (find(LBRA, 0))
		depth++;
	Token *elem_type = find(DATA_TYPES, ID, 0);
	if (!elem_type) {
		parse_error(token, "expected element type after %s[", to_string(token->type));
		parser_recover(token->space);
		return syntax_error();
	}
	for (int i = 0; i < depth; i++)
		EXPECT_TOKEN(token, RBRA, "expected ']' in %s type", to_string(token->type));
	EXPECT_TOKEN(token, LPAR, "expected '(' after %s[type]", to_string(token->type));
	node->token->ret_type       = ARRAY;
	node->token->Array.sub_type = elem_type->type;
	node->token->Array.depth    = depth;
	if (elem_type->type == ID) {
		Node *sd = get_struct(elem_type->name);
		if (sd) {
			node->token->Array.sub_type   = STRUCT_CALL;
			node->token->Array.struct_ptr = sd;
		}
		// TODO: check if struct doesn not exists
	}
	for (int i = 0; i < depth; i++) {
		add_child(node, expr_node());
		if (i < depth - 1) EXPECT_TOKEN(token, COMA, "expected ',' between dimensions");
	}
	EXPECT_TOKEN(token, RPAR, "expected ')' after %s size", to_string(token->type));
	return node;
}

Node *typeof_sizeof_node(Token *token) {
	Type  ret  = token->type == TYPEOF ? CHARS : INT;
	Node *node = new_node(token);
	EXPECT_TOKEN(token, LPAR, "expected '(' after %s", to_string(token->type));
	node->left = prime_node();
	EXPECT_TOKEN(token, RPAR, "expected ')' to close %s", to_string(token->type));
	node->token->ret_type = ret;
	return node;
}

Node *if_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	node->left = expr_node();
	EXPECT_TOKEN(token, DOTS, "expected ':' after `if` condition");

	while (within(node->token->space))
		add_child(node, expr_node());

	Node *curr = node;
	while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within(node->token->space - TAB)) {
		Token *tok = find(ELSE, ELIF, 0);

		curr->right = new_node(tok);
		curr        = curr->right;

		if (tok->type == ELIF) {
			enter_scope(curr);
			curr->left = expr_node();
			EXPECT_TOKEN(tok, DOTS, "expected ':' after `elif` condition");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
		} else if (tok->type == ELSE) {
			enter_scope(curr);
			EXPECT_TOKEN(tok, DOTS, "expected ':' after `else`");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
			break;
		}
	}
	exit_scope();
	return node;
}

Node *struct_def_node(Token *token) {
	Node  *node = new_node(token);
	Token *st_name;
	if (!(st_name = find(ID, 0))) {
		parse_error(token, "expected identifier after `struct`");
		parser_recover(token->space);
		return syntax_error();
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after struct name");

	set_name(node->token, st_name->name);
	node->token->type       = STRUCT_DEF;
	node->token->Struct.ptr = node;
	new_struct(node);
	enter_scope(node);

	while (within(node->token->space)) {
		Token *pub_tok = find(PUB, 0);
		if (pub_tok) {
			pub_node(node, pub_tok);
			if (found_error) {
				exit_scope();
				return syntax_error();
			}
			continue;
		}
		Token *op_kw;
		if ((op_kw = find(OPERATOR, 0))) {
			operator_node(node, op_kw);
			if (found_error) {
				exit_scope();
				return syntax_error();
			}
			continue;
		}
		Node *child = keyword_node();
		if (found_error) {
			exit_scope();
			return syntax_error();
		}
		if (child->token->type == FDEC) {
			resize_array(child->functions, Node *, child->functions_size, child->functions_count);
			char *qualified = format("%s.%s", node->token->name, child->token->name);
			set_name(child->token, qualified);
			free(qualified);
			new_function(child);
		} else {
			if (!child->token->is_dec) {
				parse_error(child->token, "invalid struct attribute");
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

Node *enum_def_node(Token *token) {
	Token *ename;
	if (!(ename = find(ID, 0))) {
		parse_error(token, "expected identifier after `enum`");
		parser_recover(token->space);
		return syntax_error();
	}
	if (!find(DOTS, 0)) {
		parse_error(ename, "expected ':' after enum name");
		parser_recover(token->space);
		return syntax_error();
	}

	set_name(token, ename->name);
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

	Token *next = tokens[exe_pos];
	if (next->type == ID && within(next->space)) {
		parse_error(next, "enum '%s': expected ',' before '%s'", token->name, next->name);
		syntax_error();
	}

	return node;
}

void array_list_type_setup(Token *data_type) {
	Type container = data_type->type;
	EXPECT_TOKEN_VOID(data_type, LBRA, "expected '[' after %s",
	                  container == LIST_TYPE ? "list" : "array");
	int depth = 1;
	while (find(LBRA, 0))
		depth++;
	Token *elem_type = find(DATA_TYPES, ID, 0);
	if (!elem_type) {
		parse_error(data_type, "expected element type in %s type",
		            container == LIST_TYPE ? "list" : "array");
		syntax_error();
		return;
	}
	for (int i = 0; i < depth; i++)
		EXPECT_TOKEN_VOID(data_type, RBRA, "expected ']' to close %s type",
		                  container == LIST_TYPE ? "list" : "array");
	if (container == LIST_TYPE) {
		const char *ename =
		    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
		char sname[LIST_NAME_MAX];
		snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
		for (int d = 1; d < depth; d++) {
			char tn[LIST_NAME_MAX];
			snprintf(tn, sizeof(tn), LIST_STRUCT_PREFIX "%s", sname);
			snprintf(sname, sizeof(sname), "%s", tn);
		}
		Node *list_st = get_struct(sname);
		if (!list_st) {
			parse_error(elem_type, "unknown type '%s' in list element type", ename);
			syntax_error();
			return;
		}
		data_type->type       = STRUCT_CALL;
		data_type->Struct.ptr = list_st;
	} else {
		data_type->Array.sub_type = elem_type->type;
		data_type->Array.depth    = depth;
		data_type->ret_type       = container;
		if (elem_type->type == ID) {
			Node *sd = get_struct(elem_type->name);
			if (!sd) {
				parse_error(elem_type, "unknown type '%s' in array element type", elem_type->name);
				syntax_error();
				return;
			}
			data_type->Array.sub_type   = STRUCT_CALL;
			data_type->Array.struct_ptr = sd;
		}
	}
}

#define MAX_LIST_SYNTHS 64

char       *synth_list_paths[MAX_LIST_SYNTHS];
int         synth_list_count;

static void list_struct_names(const char *base, int depth, char *elem, char *sname) {
	snprintf(elem, LIST_NAME_MAX, "%s", base);
	snprintf(sname, LIST_NAME_MAX, LIST_STRUCT_PREFIX "%s", base);
	for (int d = 1; d < depth; d++) {
		snprintf(elem, LIST_NAME_MAX, "%s", sname);
		char tmp[LIST_NAME_MAX];
		snprintf(tmp, sizeof tmp, LIST_STRUCT_PREFIX "%s", sname);
		snprintf(sname, LIST_NAME_MAX, "%s", tmp);
	}
}

void synth_list_structs() {
	struct {
		const char *base;
		int         max_depth;
	} specs[MAX_LIST_SYNTHS];
	int n = 0;
	for (int i = 0; i < tokens_count; i++) {
		if (tokens[i]->type != LIST_TYPE) continue;
		int j = i + 1, depth = 0;
		while (j < tokens_count && tokens[j]->type == LBRA) {
			depth++;
			j++;
		}
		if (j >= tokens_count) continue;
		const char *name = tokens[j]->name ? tokens[j]->name : type_to_ura_name(tokens[j]->type);
		if (!name) continue;
		int idx = -1;
		for (int k = 0; k < n; k++)
			if (strcmp(specs[k].base, name) == 0) {
				idx = k;
				break;
			}
		if (idx == -1) {
			if (n < MAX_LIST_SYNTHS) {
				specs[n].base      = name;
				specs[n].max_depth = depth;
				n++;
			}
		} else if (depth > specs[idx].max_depth) specs[idx].max_depth = depth;
	}

	synth_list_count = 0;
	for (int b = 0; b < n; b++) {
		for (int d = 1; d <= specs[b].max_depth; d++) {
			char elem[LIST_NAME_MAX], sname[LIST_NAME_MAX];
			list_struct_names(specs[b].base, d, elem, sname);
			char *src  = generate_list_source(elem, sname);
			char *path = format("/tmp/__ura_%d_%s.ura", (int)getpid(), sname);
			File  f    = fopen(path, "w");
			if (!f) {
				free(path);
				free(src);
				continue;
			}
			if (synth_list_count == 0) fputs("use \"@/memory\"\n\n", f);
			fputs(src, f);
			fclose(f);
			free(src);
			synth_list_paths[synth_list_count++] = path;
		}
	}
}

void params_node(Node *node) {
	node->left = new_node(new_token(ARGS, node->token->space));

	Token *last;
	while (!found_error && !(last = find(RPAR, END, 0))) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			last                     = find(RPAR, 0);
			if (!last) {
				parse_error(node->token, "expected ')' after `...` in variadic function");
				syntax_error();
				return;
			}
			break;
		}

		bool is_ref = false;
		if (tokens[exe_pos]->type == REF) {
			is_ref = true;
			exe_pos++;
		}

		Token *name = find(ID, 0);
		if (!name) {
			parse_error(node->token, "expected parameter name in function '%s'", node->token->name);
			syntax_error();
			return;
		}

		Token *data_type;
		if (tokens[exe_pos]->type == FDEC) {
			data_type = parse_fn_type(name->space);
			if (!data_type) break;
		} else {
			data_type = find(DATA_TYPES, ID, 0);
			if (!data_type) {
				parse_error(name, "expected type for parameter '%s'", name->name);
				syntax_error();
				break;
			}
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				array_list_type_setup(data_type);
				if (found_error) break;
			}
		}
		if (tokens[exe_pos]->type == REF) {
			parse_error(tokens[exe_pos],
			    "trailing `ref` is no longer supported; write `ref %s <type>` before the name",
			    name->name);
			syntax_error();
			break;
		}
		if (data_type->type == ID) {
			Node *to_find = get_struct(data_type->name);
			if (to_find) data_type->type = STRUCT_CALL;
			else {
				parse_error(data_type, "unknown type '%s'", data_type->name);
				syntax_error();
				break;
			}
		}

		Node *curr;
		if (data_type->type == STRUCT_CALL) {
			Node *st = get_struct(data_type->name);
			if (!st) {
				parse_error(data_type, "unknown struct type '%s'", data_type->name);
				syntax_error();
				break;
			}
			data_type->Struct.ptr = st;
			curr                  = new_node(data_type);
			data_type->is_ref     = is_ref;
			set_name(data_type, name->name);
		} else {
			curr         = new_node(name);
			name->is_ref = is_ref;
			name->type   = data_type->type;
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				name->ret_type         = data_type->type;
				name->Array.sub_type   = data_type->Array.sub_type;
				name->Array.depth      = data_type->Array.depth;
				name->Array.struct_ptr = data_type->Array.struct_ptr;
			} else if (data_type->type == FN_TYPE) {
				name->ret_type     = FN_TYPE;
				name->Fn.params    = data_type->Fn.params;
				name->Fn.params_count = data_type->Fn.params_count;
				name->Fn.params_size  = data_type->Fn.params_size;
				name->Fn.ret       = data_type->Fn.ret;
			}
		}
		curr->token->is_dec   = true;
		curr->token->is_param = true;
		add_child(node->left, curr);

		if (tokens[exe_pos]->type != RPAR)
			EXPECT_TOKEN_VOID(name, COMA, "expected ',' between function parameters");
	}
	if (!found_error && last && last->type != RPAR) {
		parse_error(node->token, "expected ')' to close function parameter list");
		syntax_error();
	}
}

Node *while_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);
	node->left = expr_node();
	EXPECT_TOKEN(token, DOTS, "expected ':' after `while` condition");
	while (within(node->token->space))
		add_child(node, expr_node());
	exit_scope();
	return node;
}

Node *for_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	Token *iter = find(ID, 0);
	if (!iter) {
		parse_error(token, "expected loop variable name after `for`");
		parser_recover(token->space);
		return syntax_error();
	}
	iter->type   = INT;
	iter->is_dec = true;
	node->left   = new_node(iter);

	Token *from_kw = find(ID, 0);
	if (!from_kw || !from_kw->name || strcmp(from_kw->name, "from") != 0) {
		parse_error(token, "expected `from` after `for %s`", iter->name);
		parser_recover(token->space);
		return syntax_error();
	}

	Node *from_expr = expr_node();
	EXPECT_TOKEN(token, TO, "expected `to` after `for %s from <expr>`", iter->name);
	Node *to_expr   = expr_node();
	Node *step_expr = NULL;
	if (find(STEP, 0)) step_expr = expr_node();
	if (!step_expr) {
		Token *one     = new_token(INT, token->space);
		one->Int.value = 1;
		step_expr      = new_node(one);
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after `for ... to <expr>`");

	add_child(node, from_expr);
	add_child(node, to_expr);
	add_child(node, step_expr);

	while (within(node->token->space))
		add_child(node, expr_node());
	exit_scope();
	return node;
}

Node *module_node(Token *token) {
	Token *id = find(ID, 0);
	if (!id) {
		parse_error(token, "expected module identifier after `mod`");
		parser_recover(token->space);
		return syntax_error();
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after `mod` name");
	set_name(token, id->name);
	Node *node = new_node(token);
	enter_scope(node);
	while (within(token->space)) {
		Node  *curr   = expr_node();
		Token *ctoken = curr->token;
		// `assign_node` stamps is_dec onto the ASSIGN itself when the LHS is
		// a typed declaration — so we must look through ASSIGN to find the
		// real anchor (mirrors the top-level guard in main.c).
		Token *anchor =
		    (ctoken->type == ASSIGN && curr->left) ? curr->left->token : ctoken;
		if (!includes(anchor->type, STRUCT_DEF, FDEC, 0)) {
			parse_error(ctoken, "only function/struct declarations are allowed inside `mod`");
			syntax_error();
			break;
		}
		if (ctoken->type == FDEC) add_function(node, curr);
		else add_child(node, curr);
	}
	exit_scope();
	return node;
}

Node *array_lit_node(Token *token) {
	token->type = ARRAY_LIT;
	Node *node  = new_node(token);
	if (tokens[exe_pos]->type != RBRA) {
		add_child(node, expr_node());
		while (find(COMA, 0))
			add_child(node, expr_node());
	}
	EXPECT_TOKEN(token, RBRA, "expected ']' to close array literal");
	return node;
}

Node *tuple_unpack_node(Token *first_decl) {
	Token *tu_tok = new_token(TUPLE_UNPACK, first_decl->space);
	Node  *tu     = new_node(tu_tok);
	add_child(tu, new_node(first_decl));
	while (tokens[exe_pos]->type == COMA) {
		exe_pos++;
		Token *nname = find(ID, 0);
		Token *ntype = find(DATA_TYPES, 0);
		if (includes(ntype->type, ARRAY_TYPE, LIST_TYPE, 0)) {
			Type container = ntype->type;
			EXPECT_TOKEN(ntype, LBRA, "expected '[' after %s",
			             container == LIST_TYPE ? "list" : "array");
			int depth = 1;
			while (find(LBRA, 0))
				depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			if (!elem_type) {
				parse_error(ntype, "expected element type in %s type",
				            container == LIST_TYPE ? "list" : "array");
				return syntax_error();
			}
			for (int i = 0; i < depth; i++)
				EXPECT_TOKEN(ntype, RBRA, "expected ']' to close %s type",
				             container == LIST_TYPE ? "list" : "array");
			if (container == LIST_TYPE) {
				const char *ename =
				    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
				char sname[LIST_NAME_MAX];
				snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
				for (int d = 1; d < depth; d++) {
					char tn[LIST_NAME_MAX];
					snprintf(tn, sizeof(tn), LIST_STRUCT_PREFIX "%s", sname);
					snprintf(sname, sizeof(sname), "%s", tn);
				}
				Node *list_st = get_struct(sname);
				if (!list_st) {
					parse_error(ntype, "list struct '%s' not found", sname);
					parser_recover(ntype->space);
					return syntax_error();
				}
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
		set_name(ntype, nname->name);
		ntype->is_dec = true;
		add_child(tu, new_node(ntype));
	}
	EXPECT_TOKEN(first_decl, ASSIGN, "expected '=' in tuple unpack");
	tu->left = expr_node();
	return tu;
}

Node *pub_node(Node *struct_node, Token *pub_tok) {
	Token *fdec_tok = find(FDEC, 0);
	if (!fdec_tok) {
		parse_error(pub_tok, "struct '%s': expected `fn` after `pub`", struct_node->token->name);
		parser_recover(pub_tok->space);
		return syntax_error();
	}
	fdec_tok->is_pub = true;
	Node *child      = fdec_node(fdec_tok);
	if (!child || child->token->type != FDEC) {
		parse_error(pub_tok, "struct '%s': expected `fn` after `pub`", struct_node->token->name);
		parser_recover(pub_tok->space);
		return syntax_error();
	}
	child->token->is_pub = true;
	char *qualified      = format("%s.%s", struct_node->token->name, child->token->name);
	set_name(child->token, qualified);
	free(qualified);
	new_function(child);
	return child;
}

Node *delete_node(Node *struct_node, Token *op_kw) {
	Token *fdec_tok = new_token(FDEC, op_kw->space);
	set_name(fdec_tok, "delete");
	Node *child = fdec_node(fdec_tok);
	if (child->token->ret_type != VOID) {
		parse_error(op_kw, "struct '%s': operator delete must return void", struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}
	if (child->left->children_count != 1) {
		parse_error(op_kw, "struct '%s': operator delete takes no parameters",
		            struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}
	char *qualified = format("%s.delete", struct_node->token->name);
	set_name(child->token, qualified);
	free(qualified);
	struct_node->token->has_clean = true;
	new_function(child);
	return child;
}

Node *operator_node(Node *struct_node, Token *op_kw) {
	Token *op_tok = find(ASSIGN, ADD, SUB, MUL, DIV, MOD, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
	                     DIV_ASSIGN, MOD_ASSIGN, EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL,
	                     GREAT_EQUAL, AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, DELETE, 0);
	if (!op_tok && tokens[exe_pos]->type == ID && strcmp(tokens[exe_pos]->name, "output") == 0) {
		op_tok       = tokens[exe_pos++];
		op_tok->type = OUTPUT;
	}
	if (!op_tok) {
		parse_error(op_kw, "struct '%s': expected operator symbol after `operator`",
		            struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}

	if (op_tok->type == DELETE) return delete_node(struct_node, op_kw);

	if (op_tok->type == OUTPUT) {
		Token *fdec_tok = new_token(FDEC, op_kw->space);
		set_name(fdec_tok, "output");
		Node *child     = fdec_node(fdec_tok);
		char *qualified = format("%s" OP_PREFIX "output", struct_node->token->name);
		set_name(child->token, qualified);
		free(qualified);
		new_function(child);
		return child;
	}

	char  *op_base  = format("operator.%s", to_string(op_tok->type));
	Token *fdec_tok = new_token(FDEC, op_kw->space);
	set_name(fdec_tok, op_base);
	free(op_base);
	Node *child = fdec_node(fdec_tok);
	char *op_named;
	if (child->left && child->left->children_count >= 2) {
		Token *p      = child->left->children[0]->token;
		char  *suffix = (p->type == STRUCT_CALL && p->Struct.ptr) ? p->Struct.ptr->token->name
		                                                          : to_string(p->type);
		op_named      = format("%s.%s", child->token->name, suffix);
	} else {
		op_named = format("%s", child->token->name);
	}
	char *qualified = format("%s.%s", struct_node->token->name, op_named);
	free(op_named);
	set_name(child->token, qualified);
	free(qualified);
	new_function(child);
	return child;
}

Node *main_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);
	node->token->type     = FDEC;
	node->token->ret_type = INT;

	params_node(node);
	if (found_error) return syntax_error();

	EXPECT_TOKEN(token, DOTS, "expected ':' after `main()` declaration");

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
}

Node *output_node(Token *token) {
	Node *node        = new_node(token);
	node->token->type = OUTPUT;
	node->left        = new_node(new_token(ARGS, token->space));
	Token *end        = NULL;
	while (!found_error && !(end = find(RPAR, END, 0))) {
		add_child(node->left, expr_node());
		find(COMA, 0);
	}
	if (!found_error && end->type != RPAR) {
		parse_error(token, "expected ')' to close `output(...)` call");
		parser_recover(token->space);
		return syntax_error();
	}
	node->token->ret_type = VOID;
	return node;
}

Node *fcall_node(Token *token) {
	Node *node        = new_node(token);
	node->token->type = FCALL;
	node->left        = new_node(new_token(ARGS, node->token->space));
	Token *end        = NULL;
	while (!found_error && !(end = find(RPAR, END, 0))) {
		add_child(node->left, expr_node());
		find(COMA, 0);
	}
	if (!found_error && end->type != RPAR) {
		parse_error(token, "expected ')' to close function call");
		syntax_error();
	}
	return node;
}

// ============================================================================
// INTERMEDIATE REPRESENTATION
// ============================================================================

void ir_id(Node *node) {
	// Variable in any active scope (preferred — masks fn name in inner scope).
	Token *find = get_variable(node->token->name);
	if (find) {
		node->token = find;
		return;
	}
	// Fall back: bare ID resolves to a top-level function → function pointer value.
	Node *func = find_function(node->token->name);
	if (func) {
		Token *t      = node->token;
		t->type       = FN_TYPE;
		t->ret_type   = FN_TYPE;
		t->Fcall.ptr  = func;
		// Mark the referenced function as used so asm_fdec emits it.
		func->token->used++;
		// Build the Fn signature on this token from the function's declaration.
		Node *params  = func->left;
		if (params) {
			for (int i = 0; i < params->children_count; i++) {
				Token *p = params->children[i]->token;
				resize_array(t->Fn.params, Token *, t->Fn.params_size, t->Fn.params_count);
				t->Fn.params[t->Fn.params_count++] = p;
			}
		}
		Token *ret_t = new_token(func->token->ret_type, 0);
		if (func->token->ret_type == STRUCT_CALL)
			ret_t->Struct.ptr = func->token->Struct.ptr;
		t->Fn.ret = ret_t;
		return;
	}
	parse_error(node->token, "'%s' not found", node->token->name);
}

void ir_struct_call(Node *node) {
	Node *src               = get_struct(node->token->Struct.ptr->token->name);
	node->token->Struct.ptr = src;
	if (node->token->is_dec) {
		if (scopes_count == 1) node->token->is_global = true;
		else if (scope && scope->token && !scope->token->is_proto) src->token->used++;
		new_variable(node->token);
	}
}

void ir_nullable(Node *node) {
	node->token->ret_type = CHARS;
	node->token->used++;
}

void ir_primitive(Node *node) {
	if (node->token->is_dec) {
		if (scopes_count == 1) node->token->is_global = true;
		new_variable(node->token);
	} else node->token->used++;
}

void ir_assign(Node *node) {
	ir_gen(node->left);
	if (found_error) return;
	ir_gen(node->right);
	if (found_error) return;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, ASSIGN);
		if (func) {
			node->token->Fcall.ptr = func;
			func->token->used++;
			return;
		}
	}
	if (!compatible(node->left->token, node->right->token)) {
		Type rt =
		    node->right->token->ret_type ? node->right->token->ret_type : node->right->token->type;
		parse_error(node->token, "type mismatch: cannot assign %s to %s", to_string(rt),
		            to_string(lt));
		return;
	}
	node->token->ret_type = node->left->token->ret_type;
}

void ir_compound_assign(Node *node) {
	ir_gen(node->left);
	ir_gen(node->right);
	node->token->ret_type = node->left->token->ret_type;
	node->token->used++;
	node->left->token->used++;
	node->right->token->used++;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, node->token->type);
		if (func) {
			node->token->Fcall.ptr = func;
			func->token->used++;
			node->token->ret_type = func->token->ret_type;
		}
	}
}

void ir_unary_op(Node *node) {
	ir_gen(node->left);
	node->token->used++;
	node->left->token->used++;
	if (node->token->type == NOT) node->token->ret_type = BOOL;
	else
		node->token->ret_type =
		    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
}

void ir_binary_op(Node *node) {
	ir_gen(node->left);
	ir_gen(node->right);
	CHECK(!node->left || !node->left->token, "left is NULL");
	CHECK(!node->right || !node->right->token, "right is NULL");
	if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
		node->token->ret_type = node->left->token->ret_type;
	else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0)) node->token->ret_type = BOOL;
	node->token->used++;
	node->left->token->used++;
	node->right->token->used++;
	set_ret_type(node);
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, node->token->type);
		if (!func) {
			parse_error(
			    node->token, "no `operator %s` overload for struct '%s'", to_string(node->token->type),
			    node->left->token->Struct.ptr ? node->left->token->Struct.ptr->token->name : "?");
			return;
		}
		node->token->Fcall.ptr = func;
		func->token->used++;
		if (func->token->ret_type == STRUCT_CALL) {
			node->token->ret_type   = STRUCT_CALL;
			node->token->Struct.ptr = func->token->Struct.ptr;
		} else node->token->ret_type = func->token->ret_type;
	}
}

void ir_as(Node *node) {
	ir_gen(node->left);
	if (found_error) return;
	Type src = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	Type dst = node->right->token->type;
	if (src == STRUCT_CALL || dst == STRUCT_CALL) {
		parse_error(node->token, "cannot cast %s to %s", to_string(src), to_string(dst));
		return;
	}
	node->token->ret_type = dst;
}

void ir_ref(Node *node) {
	ir_gen(node->left);
	node->token->is_ref = true;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	node->token->ret_type = lt;
	if (lt == STRUCT_CALL) node->token->Struct.ptr = node->left->token->Struct.ptr;
}

void ir_dot(Node *node) {
	ir_gen(node->left);
	if (found_error) return;
	node->left->token->used++;
	if (node->left->token->Struct.ptr) node->left->token->Struct.ptr->token->used++;

	Node *src = NULL;
	if (node->left->token->type == STRUCT_CALL)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else if (node->left->token->ret_type == STRUCT_CALL && node->left->token->Struct.ptr)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else {
		parse_error(node->left->token, "cannot access field of non-struct value (%s)",
		            to_string(node->left->token->type));
		return;
	}

	Type ret_type = 0;
	bool found    = false;
	for (int i = 0; src && i < src->children_count; i++) {
		Node *src_child = src->children[i];
		if (strcmp(src_child->token->name, node->right->token->name) != 0) continue;
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
	if (!found) {
		parse_error(node->right->token, "'%s' has no field '%s'",
		            src ? src->token->name : "(unknown)",
		            node->right->token->name ? node->right->token->name : "?");
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

void ir_access(Node *node) {
	ir_gen(node->left);
	ir_gen(node->right);
	if (found_error) return;
	node->right->token->used++;
	Type type  = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	int  depth = node->left->token->Array.depth;
	Type ret_type = 0;
	switch (type) {
	case CHARS: ret_type = CHAR; break;
	case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
		if (depth > 1) {
			ret_type                      = ARRAY;
			node->token->Array.sub_type   = node->left->token->Array.sub_type;
			node->token->Array.depth      = depth - 1;
			node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
		} else {
			ret_type = node->left->token->Array.sub_type;
			if (ret_type == STRUCT_CALL) node->token->Struct.ptr = node->left->token->Array.struct_ptr;
		}
		break;
	case STRUCT_CALL: {
		Node *st = node->left->token->Struct.ptr;
		if (st && strncmp(st->token->name, LIST_STRUCT_PREFIX, sizeof(LIST_STRUCT_PREFIX) - 1) == 0) {
			ir_access_list_struct(node, st);
			return;
		}
		CHECK(1, "struct '%s' does not support [] access", st ? st->token->name : "(null)");
		break;
	}
	default: CHECK(1, "handle this case %s", to_string(node->left->token->type)); break;
	}
	node->token->ret_type = ret_type;
}

void ir_fcall(Node *node) {
	if (try_module_call(node))                          return;
	if (node->token->is_static_call)                    ir_static_call(node);
	else if (node->token->is_method_call && node->left) ir_method_call(node);
	else {
		Token *var = get_variable(node->token->name);
		if (var && (var->ret_type == FN_TYPE || var->type == FN_TYPE)) {
			ir_indirect_call(node, var);
			return;
		}
		ir_regular_call(node);
	}
}

void ir_output(Node *node) {
	for (int i = 0; i < node->left->children_count; i++) {
		ir_gen(node->left->children[i]);
		Token *arg = node->left->children[i]->token;
		arg->used++;
		// For struct args, the printf formatter dispatches to a `<Struct>_output`
		// flat function (or `<Struct>.operator.output`). Mark it used so the
		// codegen actually emits the body.
		Type t = arg->ret_type ? arg->ret_type : arg->type;
		if (t == STRUCT_CALL && arg->Struct.ptr) {
			const char *sname = arg->Struct.ptr->token->name;
			char *qualified   = format("%s" OP_PREFIX "output", sname);
			Node *fn          = find_function(qualified);
			free(qualified);
			if (!fn) {
				char *flat = format("%s_output", sname);
				fn         = find_function(flat);
				free(flat);
			}
			if (fn) fn->token->used++;
		}
	}
	node->token->ret_type = VOID;
}

void ir_typeof_sizeof(Node *node) {
	if (node->left && node->left->token->type == ID && node->left->token->name) {
		Node *st = get_struct(node->left->token->name);
		if (st) {
			node->left->token->type       = STRUCT_CALL;
			node->left->token->Struct.ptr = st;
		}
	}
	// Bare type-name forms like `sizeof(chars)` come from the prep emitter.
	// Primitive type keywords tokenize with is_dec=1, which would cause
	// ir_primitive to register them as variables — but we just want the type.
	if (node->left && node->left->token && node->left->token->is_dec
	    && !node->left->token->name
	    && includes(node->left->token->type, DATA_TYPES, 0))
		node->left->token->is_dec = false;
	ir_gen(node->left);
	Token *type_tok = node->left->token;
	Type   type     = type_tok->type ? type_tok->type : type_tok->ret_type;
	if (node->token->type == TYPEOF) {
		node->token->ret_type    = CHARS;
		node->token->Chars.value = strdup(to_string(type));
	} else {
		node->token->ret_type  = INT;
		node->token->Int.value = 0;
	}
	node->token->used++;
}

void ir_stack_heap(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		ir_gen(node->children[i]);
		node->children[i]->token->used++;
	}
	node->token->used++;
}

void ir_tuple_unpack(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Node *lhs = node->children[i];
		if (lhs->token->is_dec) {
			if (scopes_count == 1) lhs->token->is_global = true;
			new_variable(lhs->token);
		}
	}
	ir_gen(node->left);
	if (found_error) return;

	Token *rhs  = node->left->token;
	Node  *func = rhs->Fcall.ptr;
	if (!func || func->token->ret_type != TUPLE) {
		parse_error(node->token, "tuple unpack requires a tuple-returning function call");
		return;
	}
	int got = node->children_count;
	int exp = func->token->Tuple.types_count;
	if (got != exp) {
		parse_error(node->token, "tuple unpack expects %d values, got %d names", exp, got);
		return;
	}
	for (int i = 0; i < got; i++) {
		Token *lhs_tok = node->children[i]->token;
		Token *rhs_tok = func->token->Tuple.types[i];
		Type   lt      = lhs_tok->ret_type ? lhs_tok->ret_type : lhs_tok->type;
		Type   rt      = rhs_tok->ret_type ? rhs_tok->ret_type : rhs_tok->type;
		if (lt != rt) {
			parse_error(lhs_tok, "tuple unpack element %d: expected %s, got %s", i + 1, to_string(rt),
			            to_string(lt));
			return;
		}
	}
}

void ir_return(Node *node) {
	if (node->children_count > 0) {
		for (int i = 0; i < node->children_count; i++)
			ir_gen(node->children[i]);
		node->token->ret_type = TUPLE;
		return;
	}
	if (!node->left) return;
	ir_gen(node->left);
	if (found_error) return;
	node->token->ret_type = node->left->token->type;
	if (node->left->token->type == STRUCT_CALL)
		node->token->Struct.ptr = node->left->token->Struct.ptr;
	node->left->token->used++;

	Node *fdec = NULL;
	for (int j = scopes_count; j > 0; j--) {
		if (scopes[j]->token->type == FDEC) {
			fdec = scopes[j];
			break;
		}
	}
	if (fdec && !compatible(fdec->token, node->left->token)) {
		Type got =
		    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
		parse_error(node->left->token, "return type mismatch in '%s': expected %s, got %s",
		            fdec->token->name, to_string(fdec->token->ret_type), to_string(got));
	}
}

void ir_if(Node *node) {
	enter_scope(node);
	Node *curr = node;
	while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
		if (includes(curr->token->type, IF, ELIF, 0)) {
			CHECK(curr->left == NULL, "error");
			ir_gen(curr->left);
			curr->left->token->used++;
		}
		for (int i = 0; i < curr->children_count; i++)
			ir_gen(curr->children[i]);
		curr = curr->right;
	}
	exit_scope();
	node->token->used++;
}

void ir_while(Node *node) {
	enter_scope(node);
	ir_gen(node->left);
	node->left->token->used++;
	for (int i = 0; i < node->children_count; i++)
		ir_gen(node->children[i]);
	exit_scope();
	node->token->used++;
}

void ir_for(Node *node) {
	enter_scope(node);
	if (scopes_count == 1) node->left->token->is_global = true;
	new_variable(node->left->token);
	node->token->used++;
	node->left->token->used++;
	for (int i = 0; i < node->children_count; i++)
		ir_gen(node->children[i]);
	exit_scope();
}

void ir_fdec(Node *node) {
	Node *existing = find_function(node->token->name);
	if (existing && existing != node) {
		// Two protos for the same C symbol (e.g. malloc) are always
		// interchangeable — accept silently even across different source files.
		bool both_proto = existing->token->is_proto && node->token->is_proto;
		bool same_loc =
		    existing->token->source && node->token->source
		    && existing->token->source->filename && node->token->source->filename
		    && strcmp(existing->token->source->filename, node->token->source->filename) == 0
		    && existing->token->line == node->token->line;
		// A user-defined fn is allowed to shadow a stdlib `proto fn` with
		// the same name (common case: tests defining a local `log`/`open`
		// wrapper that happens to match a libc symbol). Swap the entry in
		// the function table so callers resolve to the user definition.
		bool shadows_proto = existing->token->is_proto && !node->token->is_proto;
		if (shadows_proto) {
			for (int j = scopes_count; j > 0; j--) {
				Node *sc = scopes[j];
				for (int k = 0; k < sc->functions_count; k++)
					if (sc->functions[k] == existing) { sc->functions[k] = node; break; }
			}
			// Fall through and re-process body so locals/params register.
		} else {
			if (!both_proto && !same_loc)
				parse_error(node->token, "redefinition of function '%s'", node->token->name);
			return;
		}
	}
	if (!existing) new_function(node);
	enter_scope(node);
	Node **params = (node->left ? node->left->children : NULL);
	for (int i = 0; params && i < node->left->children_count && !found_error; i++) {
		ir_gen(params[i]);
		params[i]->token->used++;
	}
	for (int i = 0; !node->token->is_proto && i < node->children_count; i++)
		ir_gen(node->children[i]);
	exit_scope();
}

void ir_struct_def(Node *node) {
	for (int i = 0; node && i < node->functions_count; i++) {
		Node *fn = node->functions[i];
		if (!find_function(fn->token->name)) add_function(scope, fn);
	}
	for (int i = 0; node && i < node->functions_count; i++)
		ir_gen(node->functions[i]);
}

void ir_enum_def(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		node->children[i]->token->used++;
}

void ir_module(Node *node) {
	char *mname        = node->token->name;
	char *saved        = current_gen_module;
	current_gen_module = mname;
	for (int i = 0; i < node->functions_count; i++) {
		Node *fn    = node->functions[i];
		char *qname = format("%s.%s", mname, fn->token->name);
		set_name(fn->token, qname);
		free(qname);
		fn->token->is_pub = true;
	}
	for (int i = 0; i < node->functions_count; i++)
		ir_gen(node->functions[i]);
	current_gen_module = saved;
}

void ir_array_lit(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		ir_gen(node->children[i]);
	if (node->children_count == 0) return;
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

void ir_gen(Node *node) {
	if (found_error) return;
	switch (node->token->type) {
	case ID:           ir_id(node); break;
	case STRUCT_CALL:  ir_struct_call(node); break;
	case NULLABLE:     ir_nullable(node); break;
	case INT:          case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
	case CHARS:        case PTR: case VOID:
	case SHORT:        ir_primitive(node); break;
	case FN_TYPE:      ir_primitive(node); break;
	case TUPLE_UNPACK: ir_tuple_unpack(node); break;
	case ASSIGN:       ir_assign(node); break;
	case ADD_ASSIGN:   case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN:   ir_compound_assign(node); break;
	case NOT:
	case BNOT:      ir_unary_op(node); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    ir_binary_op(node); break;
	case AS:        ir_as(node); break;
	case REF:       ir_ref(node); break;
	case DOT:       ir_dot(node); break;
	case ACCESS:    ir_access(node); break;
	case FCALL:     ir_fcall(node); break;
	case OUTPUT:    ir_output(node); break;
	case TYPEOF:
	case SIZEOF: ir_typeof_sizeof(node); break;
	case STACK:
	case HEAP:       ir_stack_heap(node); break;
	case RETURN:     ir_return(node); break;
	case IF:         ir_if(node); break;
	case WHILE:      ir_while(node); break;
	case FOR:        ir_for(node); break;
	case FDEC:       ir_fdec(node); break;
	case STRUCT_DEF: ir_struct_def(node); break;
	case ENUM_DEF:   ir_enum_def(node); break;
	case MODULE:     ir_module(node); break;
	case ARRAY_LIT:  ir_array_lit(node); break;
	case BREAK:
	case CONTINUE: break;
	default:       TODO(1, "handle this case %s", to_string(node->token->type)); break;
	}
}

// ============================================================================
// ASSEMBLY GENERATION
// ============================================================================

void asm_struct_call(Node *node) { gen_struct_declaration(node->token); }

void asm_nullable(Node *node) { node->token->llvm.elem = LLVMConstNull(p8); }

void asm_primitive(Node *node) { gen_primitive_declaration(node->token); }

// Function reference: a bare top-level fn name used as a value.
// Produces a function pointer that can be stored, passed, or invoked.
void asm_fn_ref(Node *node) {
	if (node->token->is_dec) {
		gen_primitive_declaration(node->token);
		return;
	}
	Node *func = node->token->Fcall.ptr;
	if (!func) return;
	if (CHECK(!func->token->name, "asm_fn_ref: resolved fn has no name")) return;
	Value fn_val = LLVMGetNamedFunction(module, func->token->name);
	if (CHECK(!fn_val, "asm_fn_ref: '%s' not found in module", func->token->name)) return;
	node->token->llvm.elem      = fn_val;
	node->token->llvm.is_loaded = true; // pointer constant — no load required
}

void asm_assign(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, false);
		if (result) node->token->llvm.elem = result;
		return;
	}
	if (left->token->is_global && left->token->is_dec) {
		asm_gen(left);
		asm_gen(right);
		if (left->token->llvm.elem && right->token->llvm.elem &&
		    LLVMIsConstant(right->token->llvm.elem))
			LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
		return;
	}
	if (left->token->is_ref && right->token->type == REF) {
		if (!left->token->llvm.elem) _alloca(left->token);
		asm_gen(right);
		LLVMBuildStore(builder, right->token->llvm.elem, left->token->llvm.elem);
		left->token->is_dec = false;
		return;
	}
	// `ref b T = a` — declaration form takes the address of the RHS implicitly.
	// Without this we'd fall through to a value-store that writes a's int into
	// the null pointer currently held in b's slot (segv writing to 0x0).
	if (left->token->is_ref && left->token->is_dec) {
		if (!left->token->llvm.elem) _alloca(left->token);
		asm_gen(right);
		Value addr = right->token->is_ref
		                 ? read_value(right->token)   // collapse ref-of-ref to underlying target
		                 : right->token->llvm.elem;   // alloca of the named/field/index lvalue
		LLVMBuildStore(builder, addr, left->token->llvm.elem);
		left->token->is_dec = false;
		return;
	}
	asm_gen(left);
	asm_gen(right);
	if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dims_count > 1)
		propagate_dims(left->token, right->token, left);
	Value val;
	if (right->token->is_ref) val = read_value(right->token);
	else {
		ensure_loaded(right);
		val = right->token->llvm.elem;
	}
	val = asm_assign_cast(left, right, val);
	write_value(left->token, val);
	node->token->llvm.elem = val;
}

void asm_compound_assign(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, false);
		if (result) node->token->llvm.elem = result;
		return;
	}
	asm_gen(left);
	asm_gen(right);
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
}

void asm_unary_op(Node *node) {
	Node *left = node->left;
	asm_gen(left);
	ensure_loaded(left);
	if (left->token->is_ref) {
		left->token->llvm.elem      = read_value(left->token);
		left->token->llvm.is_loaded = true;
	}
	node->token->llvm.elem =
	    LLVMBuildNot(builder, left->token->llvm.elem, to_string(node->token->type));
}

void asm_binary_op(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, true);
		if (result) {
			node->token->llvm.elem = result;
			if (node->token->Fcall.ptr->token->ret_type != STRUCT_CALL &&
			    node->token->Fcall.ptr->token->ret_type != VOID)
				node->token->llvm.is_loaded = true;
		}
		return;
	}
	asm_gen(left);
	asm_gen(right);
	ensure_loaded(left);
	ensure_loaded(right);
	Value lref = left->token->llvm.elem;
	Value rref = right->token->llvm.elem;
	if (left->token->is_ref && !left->token->llvm.is_loaded)                                  lref = read_value(left->token);
	if (right->token->is_ref && !right->token->llvm.is_loaded)                                rref = read_value(right->token);
	if (node->token->type == ADD && LLVMGetTypeKind(LLVMTypeOf(lref)) == LLVMPointerTypeKind) {
		Value idx              = LLVMGetTypeKind(LLVMTypeOf(rref)) == LLVMIntegerTypeKind &&
                          LLVMGetIntTypeWidth(LLVMTypeOf(rref)) < 64
		                             ? LLVMBuildSExt(builder, rref, i64, "idx")
		                             : rref;
		node->token->llvm.elem = LLVMBuildGEP2(builder, i8, lref, &idx, 1, "ADD");
	} else node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
	if (includes(node->token->type, COMPARISON_OPS, 0)) node->token->ret_type = BOOL;
	else node->token->ret_type = left->token->ret_type ? left->token->ret_type : left->token->type;
}

void asm_as(Node *node) {
	Node *left = node->left;
	asm_gen(left);
	ensure_loaded(left);
	Value    source = left->token->llvm.elem;
	TypeRef  stype  = LLVMTypeOf(source);
	TypeRef  ttype  = get_llvm_type(node->right->token);
	TypeKind sk     = LLVMGetTypeKind(stype);
	TypeKind tk     = LLVMGetTypeKind(ttype);
	bool     sf     = (sk == FloatType || sk == DoubleType);
	bool     tf     = (tk == FloatType || tk == DoubleType);
	Value    result;
	if (stype == ttype) result = source;
	else if (sk == IntegerType && tk == IntegerType)
		result = asm_as_int_to_int(source, stype, ttype);
	else if (sk == IntegerType && tf) result = LLVMBuildSIToFP(builder, source, ttype, "as");
	else if (sf && tk == IntegerType) result = LLVMBuildFPToSI(builder, source, ttype, "as");
	else if (sf && tf)                {
		if (sk == FloatType) result = LLVMBuildFPExt(builder, source, ttype, "as");
		else result = LLVMBuildFPTrunc(builder, source, ttype, "as");
	} else if (sk == IntegerType && tk == PointerType)
		result = LLVMBuildIntToPtr(builder, source, ttype, "as");
	else if (sk == PointerType && tk == IntegerType)
		result = LLVMBuildPtrToInt(builder, source, ttype, "as");
	else if (sk == PointerType && tk == PointerType)
		result = LLVMBuildBitCast(builder, source, ttype, "as");
	else {
		CHECK(1, "unsupported cast from %d to %d", sk, tk);
		result = source;
	}
	node->token->llvm.elem      = result;
	node->token->llvm.is_loaded = true;
}

void asm_ref(Node *node) {
	asm_gen(node->left);
	node->token->llvm.elem = node->left->token->llvm.elem;
}

void asm_stack(Node *node) {
	Token   tmp                  = {.type = node->token->Array.sub_type};
	TypeRef elem_t               = get_llvm_type(&tmp);
	Value   total                = asm_collect_dims(node);
	Value   bytes                = asm_total_bytes(total, elem_t);
	node->token->llvm.elem       = allocate_stack(bytes, elem_t, "stack");
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = total;
}

void asm_heap(Node *node) {
	Token   tmp                  = {.type = node->token->Array.sub_type};
	TypeRef elem_t               = get_llvm_type(&tmp);
	Value   total                = asm_collect_dims(node);
	Value   bytes                = asm_total_bytes(total, elem_t);
	node->token->llvm.elem       = allocate_heap(bytes, elem_t, "heap");
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = total;
}

void asm_array_lit(Node *node) {
	int     count    = node->children_count;
	TypeRef elem_t   = asm_array_lit_elem_type(node);
	Value   cnt      = LLVMConstInt(i32, count, 0);
	Value   raw_data = allocate_heap(cnt, elem_t, "array_lit");
	Value   data     = LLVMBuildBitCast(builder, raw_data, LLVMPointerType(elem_t, 0), "lit_data");
	for (int i = 0; i < count; i++) {
		asm_gen(node->children[i]);
		ensure_loaded(node->children[i]);
		Value idx       = LLVMConstInt(i32, i, 0);
		Value indices[] = {idx};
		Value gep       = LLVMBuildGEP2(builder, elem_t, data, indices, 1, "lit_elem");
		LLVMBuildStore(builder, node->children[i]->token->llvm.elem, gep);
	}
	node->token->llvm.elem       = data;
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = cnt;
}

void asm_dot(Node *node) {
	asm_gen(node->left);
	Token *struct_tok  = node->left->token;
	int    field_index = node->right->token->Struct.index;
	if (struct_tok->is_ref) {
		TypeRef struct_type = get_llvm_type(struct_tok);
		Value   struct_ptr  = LLVMBuildLoad2(builder, LLVMPointerType(struct_type, 0),
		                                     struct_tok->llvm.elem, struct_tok->name);
		Value   indices[]   = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, field_index, 0)};
		node->token->llvm.elem =
		    LLVMBuildGEP2(builder, struct_type, struct_ptr, indices, 2, node->right->token->name);
	} else {
		node->token->llvm.elem = struct_field_ptr(struct_tok, field_index, node->right->token->name);
	}
	asm_dot_propagate_field_dims(struct_tok, field_index, node->token);
}

void asm_access(Node *node) {
	asm_gen(node->left);
	if (node->left->token->type == STRUCT_CALL) {
		asm_access_struct_field(node);
		return;
	}
	asm_gen(node->right);
	Value left_value = asm_access_left_value(node);
	ensure_loaded(node->right);
	Value right_ref = node->right->token->llvm.elem;
	if (asm_access_multidim(node, left_value, right_ref)) return;
	TypeRef element_type   = asm_access_element_type(node);
	Value   indices[]      = {right_ref};
	node->token->llvm.elem = LLVMBuildGEP2(builder, element_type, left_value, indices, 1, "ACCESS");
}

void asm_fcall(Node *node) {
	if (node->token->Statement.ptr) asm_fcall_indirect(node);
	else if (node->token->is_static_call) asm_fcall_static(node);
	else asm_fcall_instance(node);
}

void asm_typeof_sizeof(Node *node) {
	Token *type_tok = node->left->token;
	if (node->token->type == TYPEOF) {
		node->token->llvm.elem =
		    LLVMBuildGlobalStringPtr(builder, node->token->Chars.value, "typeof");
		node->token->llvm.is_loaded = true;
		return;
	}
	TypeRef    type             = get_llvm_type(type_tok);
	TargetData td               = LLVMGetModuleDataLayout(module);
	size_t     size             = LLVMABISizeOfType(td, type);
	node->token->Int.value      = (long long)size;
	node->token->llvm.elem      = LLVMConstInt(i32, size, 0);
	node->token->llvm.is_loaded = true;
}

void asm_output(Node *node) {
	int    argc  = node->left->children_count;
	Node **argv  = node->left->children;
	int    cap   = output_format_capacity(argc, argv);
	char  *fmt   = allocate(cap, 1);
	Value *args  = allocate(256, sizeof(Value));
	int    fc    = 0;
	int    nargs = 0;
	for (int i = 0; i < argc; i++) {
		asm_gen(argv[i]);
		append_output_arg(argv[i]->token, fmt, &fc, args, &nargs);
	}
	fmt[fc]         = '\0';
	// Declare printf with its real libc signature: (i8*, ...). Passing an extra
	// fixed arg before the varargs (the previous design) corrupted output on
	// SysV-ABI platforms (Linux x86_64) where varargs ride in registers and the
	// real printf reads %d from rsi — the bogus arg-count slot.
	Value printf_fn = LLVMGetNamedFunction(module, "printf");
	if (!printf_fn) {
		TypeRef params[]  = {p8};
		TypeRef printf_ft = LLVMFunctionType(i32, params, 1, 1);
		printf_fn         = _add_function("printf", printf_ft);
	}
	TypeRef printf_ft = LLVMGlobalGetValueType(printf_fn);
	Value  *call_args = allocate(nargs + 2, sizeof(Value));
	call_args[0]      = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
	memcpy(call_args + 1, args, nargs * sizeof(Value));
	LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 1, "");
	free(fmt);
	free(args);
	free(call_args);
}

void asm_tuple_unpack(Node *node) {
	asm_gen(node->left);
	Value tuple_val = node->left->token->llvm.elem;
	for (int i = 0; i < node->children_count; i++) {
		Node *lhs = node->children[i];
		asm_gen(lhs);
		Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
		LLVMBuildStore(builder, field, lhs->token->llvm.elem);
	}
}

void asm_while(Node *node) {
	enter_scope(node);
	Block start             = _append_block("while.start");
	Block then              = _append_block("while.then");
	Block end               = _append_block("while.end");
	node->token->llvm.start = start;
	node->token->llvm.then  = then;
	node->token->llvm.end   = end;
	_branch(start);
	LLVMPositionBuilderAtEnd(builder, start);
	asm_gen(node->left);
	ensure_loaded(node->left);
	LLVMBuildCondBr(builder, node->left->token->llvm.elem, then, end);
	LLVMPositionBuilderAtEnd(builder, then);
	for (int i = 0; i < node->children_count; i++) {
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
		asm_gen(node->children[i]);
	}
	_branch(start);
	LLVMPositionBuilderAtEnd(builder, end);
	exit_scope();
}

void asm_for(Node *node) {
	enter_scope(node);

	gen_primitive_declaration(node->left->token);
	Value i_ptr = node->left->token->llvm.elem;

	// from / to / step
	asm_gen(node->children[0]);
	ensure_loaded(node->children[0]);
	Value from_v = node->children[0]->token->llvm.elem;
	LLVMBuildStore(builder, from_v, i_ptr);

	asm_gen(node->children[1]);
	ensure_loaded(node->children[1]);
	Value to_v = node->children[1]->token->llvm.elem;

	asm_gen(node->children[2]);
	ensure_loaded(node->children[2]);
	Value step_v = node->children[2]->token->llvm.elem;

	// asc = from < to (i1)
	Value asc = LLVMBuildICmp(builder, LLVMIntSLT, from_v, to_v, "asc");
	// signed_step = asc ? step : -step
	Value zero       = LLVMConstInt(i32, 0, 1);
	Value neg_step   = LLVMBuildSub(builder, zero, step_v, "neg_step");
	Value sstep      = LLVMBuildSelect(builder, asc, step_v, neg_step, "sstep");

	Block cond_blk          = _append_block("for.cond");
	Block body_blk          = _append_block("for.body");
	Block step_blk          = _append_block("for.step");
	Block end_blk           = _append_block("for.end");
	node->token->llvm.start = step_blk; // continue → step
	node->token->llvm.then  = body_blk;
	node->token->llvm.end   = end_blk;  // break → end

	_branch(cond_blk);

	LLVMPositionBuilderAtEnd(builder, cond_blk);
	Value iv  = LLVMBuildLoad2(builder, i32, i_ptr, "iv");
	Value lt  = LLVMBuildICmp(builder, LLVMIntSLT, iv, to_v, "lt");
	Value gt  = LLVMBuildICmp(builder, LLVMIntSGT, iv, to_v, "gt");
	Value keep = LLVMBuildSelect(builder, asc, lt, gt, "keep");
	LLVMBuildCondBr(builder, keep, body_blk, end_blk);

	LLVMPositionBuilderAtEnd(builder, body_blk);
	for (int i = 3; i < node->children_count; i++) {
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
		asm_gen(node->children[i]);
	}
	_branch(step_blk);

	LLVMPositionBuilderAtEnd(builder, step_blk);
	Value cur  = LLVMBuildLoad2(builder, i32, i_ptr, "iv2");
	Value next = LLVMBuildAdd(builder, cur, sstep, "next");
	LLVMBuildStore(builder, next, i_ptr);
	_branch(cond_blk);

	LLVMPositionBuilderAtEnd(builder, end_blk);
	exit_scope();
}

void asm_if(Node *node) {
	enter_scope(node);
	Block if_start          = _append_block("if.start");
	Block end               = _append_block("if.end");
	node->token->llvm.start = if_start;
	node->token->llvm.end   = end;
	_branch(if_start);
	gen_if_chain(node, if_start, end);
	LLVMPositionBuilderAtEnd(builder, end);
	exit_scope();
}

void asm_break(void) {
	for (int i = scopes_count; i > 0; i--)
		if (scopes[i]->token->type == WHILE || scopes[i]->token->type == FOR) {
			_branch(scopes[i]->token->llvm.end);
			return;
		}
	CHECK(1, "break outside loop");
}

void asm_continue(void) {
	for (int i = scopes_count; i > 0; i--)
		if (scopes[i]->token->type == WHILE || scopes[i]->token->type == FOR) {
			_branch(scopes[i]->token->llvm.start);
			return;
		}
	CHECK(1, "continue outside loop");
}

void asm_return(Node *node) {
	Token *ret_skip =
	    (node->left && node->left->token->type == STRUCT_CALL && !node->left->token->is_ref)
	        ? node->left->token
	        : NULL;
	emit_scope_clean(scope, 0, ret_skip);
	Node *fdec = find_enclosing(FDEC);
	if (CHECK(!fdec, "can not return outside function")) return;
	char *fn_name = fdec->token->name;
	int   fn_len  = strlen(fn_name);
	if (fn_len > 7 && strcmp(fn_name + fn_len - 7, ".delete") == 0) asm_return_delete_chain(fdec);
	if (strcmp(fdec->token->name, "main") == 0)                     asm_return_main_globals();
	if (fdec->token->ret_type == TUPLE && node->children_count > 0) {
		asm_return_tuple(node, fdec);
		return;
	}
	asm_return_value(node, fdec);
}

void asm_fdec(Node *node) {
	Node *registered = find_function(node->token->name);
	if (registered && registered != node) return;
	char *name      = node->token->name;
	bool  is_method = name && strchr(name, '.');
	bool  is_main   = name && strcmp(name, "main") == 0;
	if (node->token->used == 0 && !is_method && !is_main) return;
	resolve_func_type(node);
	enter_scope(node);
	if (!node->token->is_proto) emit_func_body(node);
	exit_scope();
	debug_scope = debug_compile_unit;
}

void asm_enum_def(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Token *var = node->children[i]->token;
		Value  g   = LLVMAddGlobal(module, i32, var->name);
		LLVMSetInitializer(g, LLVMConstInt(i32, (unsigned)var->Int.value, 0));
		LLVMSetGlobalConstant(g, 1);
		var->llvm.elem = g;
		var->is_dec    = false;
	}
}

void asm_module(Node *node) {
	for (int i = 0; i < node->functions_count; i++)
		asm_gen(node->functions[i]);
}

void asm_struct_def(Node *node) {
	if (node->token->used == 0) return;
	gen_struct_emit_nested(node);
	gen_struct_build_type(node);
	gen_struct_emit_delete(node);
	gen_struct_predeclare_methods(node);
	for (int i = 0; i < node->functions_count; i++)
		asm_gen(node->functions[i]);
}

void asm_gen(Node *node) {
	set_debug_location(node->token);
	if (CHECK(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type) {
	case STRUCT_CALL: asm_struct_call(node); break;
	case NULLABLE:    asm_nullable(node); break;
	case INT:         case LONG: case SHORT: case CHARS: case CHAR: case BOOL:
	case ARRAY_TYPE:
	case FLOAT:      asm_primitive(node); break;
	case FN_TYPE:    asm_fn_ref(node); break;
	case ASSIGN:     asm_assign(node); break;
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: asm_compound_assign(node); break;
	case NOT:
	case BNOT:      asm_unary_op(node); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    asm_binary_op(node); break;
	case AS:        asm_as(node); break;
	case REF:       asm_ref(node); break;
	case STACK:     asm_stack(node); break;
	case HEAP:      asm_heap(node); break;
	case ARRAY_LIT: asm_array_lit(node); break;
	case DOT:       asm_dot(node); break;
	case ACCESS:    asm_access(node); break;
	case FCALL:     asm_fcall(node); break;
	case OUTPUT:    asm_output(node); break;
	case TYPEOF:
	case SIZEOF:       asm_typeof_sizeof(node); break;
	case TUPLE_UNPACK: asm_tuple_unpack(node); break;
	case RETURN:       asm_return(node); break;
	case IF:           asm_if(node); break;
	case WHILE:        asm_while(node); break;
	case FOR:          asm_for(node); break;
	case BREAK:        asm_break(); break;
	case CONTINUE:     asm_continue(); break;
	case FDEC:         asm_fdec(node); break;
	case STRUCT_DEF:   asm_struct_def(node); break;
	case ENUM_DEF:     asm_enum_def(node); break;
	case MODULE:       asm_module(node); break;
	default:           TODO(1, "handle this case %k", node->token); break;
	}
}

bool  found_error;        // scoped: "current parse path is broken" (cleared by parser_recover)
int   error_count;        // persistent: total errors reported across the run
int   max_errors = 25;    // cap before bailing entirely
bool  enable_debug = true;
bool  enable_san;
bool  enable_prep;
char *flags;
char *ura_lib;
const char *ura_target_os =
#if defined(__APPLE__)
    "macos";
#elif defined(__linux__)
    "linux";
#else
    "unknown";
#endif

EXPAND(Source **, sources);
EXPAND(Token **, tokens);
int exe_pos;
EXPAND(Node **, scopes);
Node *scope;

Node *ura_scope;
char *current_gen_module;

// Per-file path state, populated by setup_paths(), released by free_memory().
char            *dir;
char            *base;
char            *build_dir;
char            *ll_path;

Context          context;
Module           module;
Builder          builder;
TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;

LLVMDIBuilderRef debug_builder;
LLVMMetadataRef  debug_compile_unit;
LLVMMetadataRef  debug_file;
LLVMMetadataRef  debug_scope;

// ============================================================================
// ERROR CHECK, DEBUGGING, MEMORY
// ============================================================================
void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

char *generate_list_source(const char *elem, const char *sname) {
	char *src = allocate(LIST_SOURCE_MAX, sizeof(char));
	snprintf(src, LIST_SOURCE_MAX,
	         "struct %s:\n"
	         "   data array[%s]\n"
	         "   __len int\n"
	         "   __cap int\n"
	         "\n"
	         "   operator delete() void:\n"
	         "      free(self.data as pointer)\n"
	         "\n"
	         "   fn push(e %s) void:\n"
	         "      if self.__len >= self.__cap:\n"
	         "         if self.__cap == 0: self.__cap = 8\n"
	         "         else: self.__cap *= 2\n"
	         "         self.data = realloc(self.data as pointer, self.__cap * sizeof(%s))\n"
	         "      self.data[self.__len] = e\n"
	         "      self.__len += 1\n"
	         "\n"
	         "   fn pop() %s:\n"
	         "      self.__len -= 1\n"
	         "      return self.data[self.__len]\n"
	         "\n"
	         "   fn len() int:\n"
	         "      return self.__len\n"
	         "\n"
	         "   fn cap() int:\n"
	         "      return self.__cap\n"
	         "\n"
	         "   fn foreach(cb fn(%s) void) void:\n"
	         "      i int = 0\n"
	         "      while i < self.__len:\n"
	         "         cb(self.data[i])\n"
	         "         i += 1\n",
	         sname, elem, elem, elem, elem, elem);
	return src;
}

char *open_file(char *path_name) {
	char *file_name = realpath(path_name, NULL);
	if (!file_name) {
		parse_error(NULL, "cannot find file '%s'", path_name);
		return NULL;
	}
	File file = fopen(file_name, "r");
	if (!file) {
		parse_error(NULL, "cannot open file '%s'", file_name);
		free(file_name);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *input = allocate((size + 1), sizeof(char));
	fread(input, size, sizeof(char), file);
	fclose(file);
	free(file_name);
	return input;
}

Source *new_source(char *file_name) {
	Source *src   = allocate(1, sizeof(Source));
	src->filename = file_name;
	src->content  = open_file(file_name);
	resize_array(sources, Source *, sources_size, sources_count);
	sources[sources_count++] = src;
	return src;
}

void free_token(Token *token) {
	free(token->name);
	free(token->Chars.value);
	free(token->llvm.dims);
	free(token);
}

void free_node(Node *node) {
	if (!node) return;
	// SYNTAX_ERROR is a shared static sentinel — pointer-compare so we
	// don't dereference node->token, which has already been freed by
	// free_memory()'s earlier free_token() loop.
	if (node == syntax_error_node) return;
	for (int i = 0; i < node->children_count; i++)
		free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->functions);
	free(node->variables);
	free(node->structs);
	free(node->modules);
	free(node->auto_cleans);
	free(node);
}

void free_memory() {
	for (int i = 0; tokens && i < tokens_count; i++) {
		free_token(tokens[i]);
	}
	free(tokens);
	free(scopes);
	free_node(ura_scope);
	free(dir);
	free(base);
	free(build_dir);
	free(ll_path);
	for (int i = 0; sources && i < sources_count; i++) {
		free(sources[i]->content);
		free(sources[i]);
	}
	free(sources);
}

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

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	found_error = true;
	error_count++;
	fprintf(stderr, RED("ura_error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void parse_error(Token *token, const char *fmt, ...) {
	found_error = true;
	error_count++;
	if (error_count > max_errors) {
		fprintf(stderr, RED("error: ") "too many errors, stopping\n");
		exit(1);
	}

	fprintf(stderr, RED("error: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);

	if (!token || !token->source || !token->source->content) return;

	char *content = token->source->content;
	int   s       = token->start_index;
	int   e       = token->end_index;
	if (e <= s) e = s + 1;

	int line_start = s;
	while (line_start > 0 && content[line_start - 1] != '\n')
		line_start--;
	int line_end = e;
	while (content[line_end] && content[line_end] != '\n')
		line_end++;

	int col     = s - line_start + 1;
	int span    = e - s;
	int line_no = token->line;
	int gutter  = 1;
	for (int n = line_no; n >= 10; n /= 10)
		gutter++;

	fprintf(stderr, "%*s " BLUE("-->") " %s:%d:%d\n", gutter, "", token->source->filename, line_no,
	        col);
	fprintf(stderr, "%*s " BLUE("|") "\n", gutter, "");
	fprintf(stderr, BLUE("%*d |") " %.*s\n", gutter, line_no, line_end - line_start,
	        content + line_start);
	fprintf(stderr, "%*s " BLUE("|") " ", gutter, "");
	for (int i = 0; i < col - 1; i++)
		fputc(content[line_start + i] == '\t' ? '\t' : ' ', stderr);
	fprintf(stderr, "\033[1;31m");
	for (int i = 0; i < span; i++)
		fputc('^', stderr);
	fprintf(stderr, RESET "\n");
}

void tokenize_error(Source *src, int line, int s, int e, const char *fmt, ...) {
	Token tok       = {0};
	tok.source      = src;
	tok.line        = line;
	tok.start_index = s;
	tok.end_index   = e;

	va_list ap;
	va_start(ap, fmt);
	char *msg = NULL;
	int   len = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (len >= 0) {
		msg = allocate(len + 1, sizeof(char));
		va_start(ap, fmt);
		vsnprintf(msg, len + 1, fmt, ap);
		va_end(ap);
	}
	parse_error(&tok, "%s", msg ? msg : fmt);
	free(msg);
}

int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = _vprint(stdout, conv, args);
	va_end(args);
	return res;
}

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

		char       *new_indent = format("%s%s", indent, bar);

		char       *connector = is_last ? "└──" : "├──";
		debug("%s%s", indent, connector);
		pnode(child, new_indent);
		free(new_indent);
	}
	free(subs);
#undef push
}

// ============================================================================
// PARSING
// ============================================================================
int parse_escape_seq(char *input, int s, int e, char *buf, int *ptr) {
	int j   = *ptr;
	int ret = s + 1;
	switch (input[s + 1]) {
	case 'n':  buf[j++] = '\n'; break;  // newline
	case 't':  buf[j++] = '\t'; break;  // tab
	case 'r':  buf[j++] = '\r'; break;  // carriage return
	case 'b':  buf[j++] = '\b'; break;  // backspace
	case 'f':  buf[j++] = '\f'; break;  // form feed
	case 'v':  buf[j++] = '\v'; break;  // vertical tab
	case 'a':  buf[j++] = '\a'; break;  // alert (bell)
	case '\\': buf[j++] = '\\'; break; // backslash
	case '"':  buf[j++] = '"'; break;   // double quote
	case '\'': buf[j++] = '\''; break; // single quote
	case '?':  buf[j++] = '\?'; break;  // question mark (trigraph)
	case '0': {
		// three-digit octal: \0NN
		if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[j++] = (char)octal;
				ret      = s + 3;
				break;
			}
			buf[j++] = '\0';
			break;
		}
		// two-digit octal: \0N
		else if (s + 1 < e && isdigit(input[s + 2])) {
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[j++]  = (char)octal;
			ret       = s + 2;
			break;
		}
		// plain null
		buf[j++] = '\0';
		break;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
		// three-digit octal: \NNN
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[j++] = (char)octal;
				ret      = s + 3;
				break;
			}
			buf[j++] = input[s];
			break; // invalid, keep backslash
		}
		// two-digit octal
		else if (s + 2 < e && isdigit(input[s + 2])) {
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[j++]  = (char)octal;
			ret       = s + 2;
			break;
		}
		// single-digit octal
		buf[j++] = (char)(input[s + 1] - '0');
		break;
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
			buf[j++] = (char)hex;
			ret      = s + 3;
			break;
		}
		buf[j++] = input[s]; // invalid hex escape, keep backslash
		break;
	}
	case 'u': // \uXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	case 'U': // \UXXXXXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	default:
		buf[j++] = input[s]; // unknown escape, keep backslash
		break;
	}
	*ptr = j;
	return ret;
}

char *format(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (CHECK(len < 0, "format: vsnprintf measure failed")) return NULL;

	char *buf = allocate(len + 1, sizeof(char));
	va_start(ap, fmt);
	vsnprintf(buf, len + 1, fmt, ap);
	va_end(ap);
	return buf;
}

char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
	    [INT] = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
	    [FDEC] = "FDEC", [FCALL] = "CALL", [END] = "END", [LPAR] = "LPAR",
	    [RPAR] = "RPAR", [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
	    [WHILE] = "WHILE", [FOR] = "FOR", [TO] = "TO",
	    [STEP] = "STEP", [IN] = "IN", [BREAK] = "BRK", [CONTINUE] = "CONT",
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
	    [FN_TYPE] = "FN_TYPE",
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

bool includes(Type to_find, ...) {
	if (found_error) return false;
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

// ============================================================================
// INTERMEDIATE REPRESENTATION
// ============================================================================
void add_variable(Node *parent, Token *token) {
	resize_array(parent->variables, Token *, parent->variables_size, parent->variables_count);
	parent->variables[parent->variables_count++] = token;
}

Token *new_variable(Token *token) {
	debug(CYAN("new variable [%k] in scope %k\n"), token, scope->token);
	for (int i = 0; i < scope->variables_count; i++) {
		Token *curr = scope->variables[i];
		if (strcmp(curr->name, token->name) == 0) {
			parse_error(token, "redefinition of '%s'", token->name);
			return token;
		}
	}
	for (int j = scopes_count - 1; j > 0; j--) {
		Node *outer = scopes[j];
		for (int i = 0; i < outer->variables_count; i++) {
			Token *curr = outer->variables[i];
			if (curr->is_global && strcmp(curr->name, token->name) == 0) {
				parse_error(token, "'%s' shadows a global of the same name", token->name);
				return token;
			}
		}
	}
	if (scopes_count == 1) token->is_global = true;
	add_variable(scope, token);
	return token;
}

// Silent lookup — returns NULL on miss without firing an error.
// Caller (typically ir_id) decides whether to error or fall back to a
// function-reference lookup.
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
	return NULL;
}

void add_function(Node *parent, Node *node) {
	resize_array(parent->functions, Node *, parent->functions_size, parent->functions_count);
	parent->functions[parent->functions_count++] = node;
}

Node *new_function(Node *node) {
	for (int i = 0; i < scope->functions_count; i++) {
		Node *func = scope->functions[i];
		if (strcmp(func->token->name, node->token->name) == 0) {
			parse_error(node->token, "redefinition of function '%s'", node->token->name);
			return node;
		}
	}
	add_function(scope, node);
	return node;
}

Node *find_function(char *name) {
	for (int j = scopes_count; j > 0; j--) {
		Node *sc = scopes[j];
		for (int i = 0; i < sc->functions_count; i++)
			if (strcmp(sc->functions[i]->token->name, name) == 0) return sc->functions[i];
	}
	if (current_gen_module) {
		char *qname = format("%s.%s", current_gen_module, name);
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
	parse_error(NULL, "function '%s' not found", name);
	return NULL;
}

Node *new_struct(Node *node) {
	debug(CYAN("new struct [%s] in scope %k\n"), node->token->name, scope->token);
	for (int i = 0; i < scope->structs_count; i++) {
		Token *curr = scope->structs[i]->token;
		if (strcmp(curr->name, node->token->name) == 0) {
			parse_error(node->token, "redefinition of struct '%s'", node->token->name);
			return node;
		}
	}
	resize_array(scope->structs, Node *, scope->structs_size, scope->structs_count);
	scope->structs[scope->structs_count++] = node;
	return node;
}

void add_struct(Node *parent, Node *node) {
	resize_array(parent->structs, Node *, parent->structs_size, parent->structs_count);
	parent->structs[parent->structs_count++] = node;
}

void add_auto_clean(Node *parent, Value value, Node *type) {
	resize_array(parent->auto_cleans, AutoClean, parent->auto_cleans_size,
	             parent->auto_cleans_count);
	parent->auto_cleans[parent->auto_cleans_count].value = value;
	parent->auto_cleans[parent->auto_cleans_count].type  = type;
	parent->auto_cleans_count++;
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

Type get_ret_type(Node *node) {
	if (!node || !node->token) return 0;
	Token *token = node->token;
	Node  *left  = node->left;
	Node  *right = node->right;
	if (token->ret_type) return token->ret_type;
	if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
		return token->type;
	if (token->type == FCALL)                                         return token->Fcall.ptr ? token->Fcall.ptr->token->ret_type : 0;
	if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0))      return get_ret_type(left);
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
	if (node->token->type == FCALL && node->token->ret_type == STRUCT_CALL && node->token->Fcall.ptr)
		node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}

bool compatible(Token *left, Token *right) {
	Type lt = left->ret_type ? left->ret_type : left->type;
	Type rt = right->ret_type ? right->ret_type : right->type;

	if (lt == rt)                                                 return true;
	if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;
	if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.sub_type == CHAR)
		return true;
	if (rt == CHARS && includes(lt, ARRAY_TYPE, ARRAY, 0) && left->Array.sub_type == CHAR)
		return true;
	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY && rt == ARRAY_TYPE) return true;
	// pointer/chars <-> array[T]: all pointers at LLVM level
	// TODO: it should be checked, right might be array of something else than CHAR
	if ((includes(lt, PTR, CHARS, 0) && includes(rt, ARRAY_TYPE, ARRAY, 0)) ||
	    (includes(rt, PTR, CHARS, 0) && includes(lt, ARRAY_TYPE, ARRAY, 0)))
		return true;
	bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
	bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
	if (lt_numeric && rt_numeric && lt == rt)   return true;
	if (lt == STRUCT_CALL && rt == STRUCT_CALL) return left->Struct.ptr == right->Struct.ptr;
	if (lt == FN_TYPE && rt == FN_TYPE) {
		if (left->Fn.params_count != right->Fn.params_count) return false;
		for (int i = 0; i < left->Fn.params_count; i++)
			if (!compatible(left->Fn.params[i], right->Fn.params[i])) return false;
		// ret may be NULL if void; treat both-NULL as compatible
		if (!left->Fn.ret && !right->Fn.ret) return true;
		if (!left->Fn.ret || !right->Fn.ret) return false;
		return compatible(left->Fn.ret, right->Fn.ret);
	}
	return false;
}

// ============================================================================
// ASSEMBLY GENERATION
// ============================================================================

void setup_paths(char *path_name) {
	char *tmp_dir  = strdup(path_name);
	char *tmp_base = strdup(path_name);
	dir            = strdup(dirname(tmp_dir));
	base           = strdup(basename(tmp_base));
	free(tmp_dir);
	free(tmp_base);

	char *dot_ext = strrchr(base, '.');
	if (dot_ext) *dot_ext = '\0';

	build_dir = format("%s/build", dir);
	mkdir(build_dir, 0755);
	char *base_ll = format("%s.ll", base);
	ll_path       = format("%s/%s", build_dir, base_ll);
	free(base_ll);
}

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
	char *dir           = allocate(URA_MAX_SIZE, 1);
	strcpy(dir, ".");
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
	free(dir);
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
	} else {
		LLVMDisposePassBuilderOptions(options);
	}

	LLVMPrintModuleToFile(module, output, NULL);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);
}

void set_debug_location(Token *token) {
	if (!token || !debug_builder || !debug_scope)              return;
	if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
	LLVMMetadataRef loc =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, debug_scope, NULL);
	LLVMSetCurrentDebugLocation2(builder, loc);
}

TypeRef get_llvm_type(Token *token) {
	Type type = token->type;
	if (token->ret_type)     type = token->ret_type;
	if (type == STRUCT_DEF)  return token->llvm.struct_type;
	if (type == TUPLE)       return token->llvm.struct_type;
	if (type == STRUCT_CALL) {
		if (CHECK(!token->Struct.ptr, "STRUCT_CALL: Struct.ptr is NULL for %k", token))
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
		return LLVMPointerType(base, 0);
	}
	if (type == FN_TYPE) {
		TypeRef ret_t = token->Fn.ret ? get_llvm_type(token->Fn.ret) : vd;
		int n = token->Fn.params_count;
		TypeRef *ptypes = NULL;
		if (n > 0) {
			ptypes = allocate(n, sizeof(TypeRef));
			for (int i = 0; i < n; i++) ptypes[i] = get_llvm_type(token->Fn.params[i]);
		}
		TypeRef ft = LLVMFunctionType(ret_t, ptypes, n, 0);
		free(ptypes);
		return LLVMPointerType(ft, 0);
	}
	// if (type == FCALL) return get_llvm_type(token->Fcall.ptr->token);
	TypeRef res[END] = {
	    [INT] = i32,  [CHAR] = i8,   [CHARS] = p8,  [BOOL] = i1,  [VOID] = vd,
	    [LONG] = i64, [FLOAT] = f32, [ACCESS] = i8, [SHORT] = i16, [NULLABLE] = p8,
	};
	TODO(!res[type], "handle this case %k", token);
	return res[type];
}

Value _get_default_value(Token *token) {
	TypeRef type = get_llvm_type(token);

	if (token->is_ref) return LLVMConstNull(LLVMPointerType(type, 0));

	if (includes(token->type, NUMERIC_TYPES, 0))     return LLVMConstInt(type, 0, false);
	if (token->type == FLOAT)                        return LLVMConstReal(type, 0.0);
	if (includes(token->type, CHARS, ARRAY_TYPE, 0)) return LLVMConstNull(type);
	if (token->type == FN_TYPE)                      return LLVMConstNull(type);
	TODO(1, "handle this case %k", token);
	return NULL;
}

Value read_value(Token *token) {
	if (token->llvm.is_loaded) return token->llvm.elem;
	// Already computed values: function returns, literals, allocated buffers
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

void write_value(Token *token, Value val) {
	Value dest = token->llvm.elem;
	if (token->is_ref) {
		TypeRef type = get_llvm_type(token);
		dest         = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), token->llvm.elem, "ref_ptr");
	}
	LLVMBuildStore(builder, val, dest);
}

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
		// TODO: to be checked
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

TypeRef func_ret_type(Token *token) {
	if (token->ret_type == TUPLE) {
		int      n  = token->Tuple.types_count;
		TypeRef *ft = allocate(n, sizeof(TypeRef));
		for (int i = 0; i < n; i++)
			ft[i] = get_llvm_type(token->Tuple.types[i]);
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
	// Ura-defined variadics carry an extra i32 arg-count slot; libc protos
	// use their real signature (no count slot) because the count would be
	// consumed by the callee's first vararg slot under SysV ABI.
	bool   add_count   = token->is_variadic && !token->is_proto;
	int    total       = fixed_count + (add_count ? 1 : 0);
	*out_count         = total;
	if (fixed_count == 0 && !add_count) return NULL;

	TypeRef *types = allocate(total + 2, sizeof(TypeRef));
	for (int i = 0; i < fixed_count; i++) {
		Token *param = fdec->left->children[i]->token;
		if (token->is_proto && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0)) types[i] = i64;
		else if (param->is_ref)                                                   types[i] = LLVMPointerType(get_llvm_type(param), 0);
		else types[i] = get_llvm_type(param);
	}
	if (add_count) types[fixed_count] = i32;
	return types;
}

// TODO: delete this one because I'm already handling function existence
Value _add_function(char *name, TypeRef function_type) {
	Value f = LLVMGetNamedFunction(module, name);
	if (f) return f;
	return LLVMAddFunction(module, name, function_type);
}

LLVMIntPredicate icmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMIntSLT;
	case GREAT:       return LLVMIntSGT;
	case EQUAL:       return LLVMIntEQ;
	case LESS_EQUAL:  return LLVMIntSLE;
	case GREAT_EQUAL: return LLVMIntSGE;
	default:          return LLVMIntNE;
	}
}

LLVMRealPredicate fcmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMRealOLT;
	case GREAT:       return LLVMRealOGT;
	case EQUAL:       return LLVMRealOEQ;
	case LESS_EQUAL:  return LLVMRealOLE;
	case GREAT_EQUAL: return LLVMRealOGE;
	default:          return LLVMRealONE;
	}
}

int is_float_value(Value v) {
	LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
	return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

Type assign_base_op(Type assign_op) {
	switch (assign_op) {
	case ADD_ASSIGN: return ADD;
	case SUB_ASSIGN: return SUB;
	case MUL_ASSIGN: return MUL;
	case DIV_ASSIGN: return DIV;
	default:         return MOD;
	}
}

TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed) {
	TypeRef type = LLVMStructCreateNamed(context, name);
	LLVMStructSetBody(type, element_types, element_count, packed);
	return type;
}

void _branch(Block bloc) {
	if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) LLVMBuildBr(builder, bloc);
}

Block _append_block(char *name) {
	Value parent = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
	return LLVMAppendBasicBlockInContext(context, parent, name);
}

Value struct_field_ptr(Token *struct_tok, int field_index, char *name) {
	TypeRef struct_type = get_llvm_type(struct_tok);
	Value   indices[]   = {
	        LLVMConstInt(i32, 0, 0),
	        LLVMConstInt(i32, field_index, 0),
   };
	return LLVMBuildGEP2(builder, struct_type, struct_tok->llvm.elem, indices, 2, name);
}

Value allocate_stack(Value size, TypeRef element_type, char *name) {
	Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, 0, 0)};
	if (LLVMIsConstant(size)) {
		unsigned long long n     = LLVMConstIntGetZExtValue(size);
		TypeRef            atype = LLVMArrayType(element_type, n);
		Value              alc   = LLVMBuildAlloca(builder, atype, name);
		return LLVMBuildGEP2(builder, atype, alc, indices, 2, name);
	}
	Value alc = LLVMBuildArrayAlloca(builder, element_type, size, name);
	return LLVMBuildGEP2(builder, element_type, alc, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef element_type, char *name) {
	Value calloc_func = LLVMGetNamedFunction(module, "calloc");
	if (!calloc_func) {
		TypeRef params[]  = {i64, i64};
		TypeRef func_type = LLVMFunctionType(p8, params, 2, 0);
		calloc_func       = _add_function("calloc", func_type);
	}
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, element_type);
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

static LLVM ensure_func_resolved(Node *callee) {
	if (callee && callee->token && !callee->token->llvm.func_type)
		resolve_func_type(callee);
	return callee->token->llvm;
}

void resolve_func_type(Node *fdec) {
	Token *token = fdec->token;
	if (token->llvm.func_type) return;

	TypeRef  ret_type = func_ret_type(token);
	int      pcount;
	TypeRef *param_types = func_param_types(fdec, &pcount);
	TypeRef  func_type   = LLVMFunctionType(ret_type, param_types, pcount, token->is_variadic);

	Value    existing     = LLVMGetNamedFunction(module, token->name);
	token->llvm.elem      = existing ? existing : _add_function(token->name, func_type);
	token->llvm.func_type = func_type;
}

void emit_func_body(Node *fdec) {
	Token *token = fdec->token;
	if (token->is_proto) return;

	if (enable_san) {
		unsigned int kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
		AttributeRef attr = LLVMCreateEnumAttribute(context, kind, 0);
		LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex, attr);
	}

	char       *fname = token->name;
	MetadataRef di_type =
	    LLVMDIBuilderCreateSubroutineType(debug_builder, debug_file, NULL, 0, LLVMDIFlagZero);
	MetadataRef di_func = LLVMDIBuilderCreateFunction(
	    debug_builder, debug_compile_unit, fname, strlen(fname), fname, strlen(fname), debug_file,
	    token->line, di_type, 0, 1, token->line, LLVMDIFlagZero, 0);
	LLVMSetSubprogram(token->llvm.elem, di_func);
	debug_scope = di_func;
	Block entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	MetadataRef debug_location =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, di_func, NULL);
	LLVMSetCurrentDebugLocation2(builder, debug_location);

	for (int i = 0; i < fdec->left->children_count; i++) {
		Token *p     = fdec->left->children[i]->token;
		Value  param = LLVMGetParam(token->llvm.elem, i);
		LLVMSetValueName(param, p->name);
		_alloca(p);
		p->is_dec = false;
		LLVMBuildStore(builder, param, p->llvm.elem);
	}

#if USING_HOIST
	for (int i = 0; i < fdec->children_count; i++)
		hoist_allocas(fdec->children[i]);
#endif
	for (int i = 0; i < fdec->children_count; i++) {
		asm_gen(fdec->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
}

Node *find_enclosing(Type type) {
	for (int i = scopes_count; i >= 0; i--)
		if (scopes[i] && scopes[i]->token->type == type) return scopes[i];
	return NULL;
}

// ============================================================================
// IR / ASM SUPPORT HELPERS (called by gen.c case dispatchers)
// ============================================================================

Node *find_op_overload(Token *left, Token *right, Type op) {
	if (!left->Struct.ptr) return NULL;
	Type  rt = right->ret_type ? right->ret_type : right->type;
	char *suffix =
	    (rt == STRUCT_CALL && right->Struct.ptr) ? right->Struct.ptr->token->name : to_string(rt);
	char *base = format("%s" OP_PREFIX "%s", left->Struct.ptr->token->name, to_string(op));
	char *name = format("%s.%s", base, suffix);
	free(base);
	Node *func = find_function(name);
	free(name);
	return func;
}

void ir_access_list_struct(Node *node, Node *st) {
	Token *dot_tok               = new_token(DOT, node->left->token->space);
	Node  *dot                   = new_node(dot_tok);
	dot->left                    = node->left;
	Token *field_tok             = copy_token(st->children[0]->token);
	field_tok->Struct.index      = 0;
	dot->right                   = new_node(field_tok);
	dot->token->ret_type         = st->children[0]->token->ret_type;
	dot->token->type             = DOT;
	dot->token->Array.sub_type   = st->children[0]->token->Array.sub_type;
	dot->token->Array.depth      = st->children[0]->token->Array.depth;
	dot->token->Array.struct_ptr = st->children[0]->token->Array.struct_ptr;
	node->left->token->used++;
	node->left = dot;
	Type type  = dot->token->ret_type ? dot->token->ret_type : dot->token->type;
	int  depth = dot->token->Array.depth;
	if (!includes(type, ARRAY_TYPE, ARRAY, 0)) return;
	if (depth > 1)                             {
		node->token->ret_type         = ARRAY;
		node->token->Array.sub_type   = dot->token->Array.sub_type;
		node->token->Array.depth      = depth - 1;
		node->token->Array.struct_ptr = dot->token->Array.struct_ptr;
	} else {
		node->token->ret_type = dot->token->Array.sub_type;
		if (node->token->ret_type == STRUCT_CALL)
			node->token->Struct.ptr = dot->token->Array.struct_ptr;
	}
}

void ir_method_call_args(Node *node, Node *func) {
	Node *dec_args = func->left;
	if (dec_args && node->left) {
		int  got = node->left->children_count - 1; // exclude obj
		int  exp = dec_args->children_count - 1;   // exclude self
		bool ok  = func->token->is_variadic ? (got >= exp) : (got == exp);
		if (!ok) {
			parse_error(node->token,
			            func->token->is_variadic ? "'%s' expects at least %d arguments, got %d"
			                                     : "'%s' expects %d arguments, got %d",
			            node->token->name, exp, got);
			return;
		}
	}
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++) {
		Node *carg = node->left->children[i];
		ir_gen(carg);
		if (carg->token->type == ID) {
			parse_error(carg->token, "undeclared variable '%s'", carg->token->name);
			break;
		}
		carg->token->used++;
		Token *src = carg->token;
		if (i >= dec_args->children_count - 1) continue;
		Type param_type   = dec_args->children[i]->token->type;
		Type arg_type     = src->type;
		bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
		bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
		if (param_is_int && arg_is_int && param_type != arg_type) {
			Token *as_tok           = new_token(AS, src->space);
			as_tok->ret_type        = param_type;
			Node *tgt               = new_node(new_token(param_type, src->space));
			Node *as_n              = new_node(as_tok);
			as_n->left              = carg;
			as_n->right             = tgt;
			node->left->children[i] = as_n;
		} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
			parse_error(carg->token, "'%s' argument %d: cannot pass %s as %s", node->token->name,
			            i + 1, to_string(arg_type), to_string(param_type));
			break;
		}
	}
}

void ir_method_call(Node *node) {
	Node *obj_node = node->left->children[node->left->children_count - 1];
	ir_gen(obj_node);
	if (found_error) return;
	Token *obj = obj_node->token;
	obj->Struct.ptr->token->used++;
	char *struct_name = obj->Struct.ptr->token->name;
	char *qname       = format("%s.%s", struct_name, node->token->name);
	set_name(node->token, qname);
	free(qname);
	node->token->is_method_call = false;
	Node *func                  = get_function(node->token->name);
	if (!func)               return;
	if (func->token->is_pub) {
		parse_error(node->token, "'%s' is a `pub fn` — call it with '::' instead of '.'",
		            node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);
	obj_node->token->used++;
	ir_method_call_args(node, func);
	free_node(func);
	set_ret_type(node);
}

void ir_regular_call_args(Node *node, Node *func) {
	Node *call_args = node->left;
	Node *dec_args  = func->left;
	if (call_args && dec_args) {
		int  got = call_args->children_count;
		int  exp = dec_args->children_count;
		bool ok  = func->token->is_variadic ? (got >= exp) : (got == exp);
		if (!ok) {
			parse_error(node->token,
			            func->token->is_variadic ? "'%s' expects at least %d arguments, got %d"
			                                     : "'%s' expects %d arguments, got %d",
			            node->token->name, exp, got);
			return;
		}
	}
	for (int i = 0; !found_error && call_args && i < call_args->children_count; i++) {
		Node *carg = call_args->children[i];
		ir_gen(carg);
		carg->token->used++;
		Token *src = carg->token;
		if (src->type == ID) {
			parse_error(carg->token, "undeclared variable '%s'", carg->token->name);
			break;
		}
		if (i >= dec_args->children_count) continue;
		bool param_is_ref = dec_args->children[i]->token->is_ref;
		if (param_is_ref &&
		    (src->type == FCALL || (src->type != DOT && src->type != ACCESS && !src->name))) {
			parse_error(carg->token, "'%s': ref parameter requires a named variable",
			            dec_args->children[i]->token->name);
			break;
		}
		Type param_type   = dec_args->children[i]->token->type;
		Type arg_type     = src->type;
		bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
		bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
		if (param_is_int && arg_is_int && param_type != arg_type) {
			Token *as_tok          = new_token(AS, src->space);
			as_tok->ret_type       = param_type;
			Node *tgt              = new_node(new_token(param_type, src->space));
			Node *as_n             = new_node(as_tok);
			as_n->left             = carg;
			as_n->right            = tgt;
			call_args->children[i] = as_n;
		} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
			parse_error(carg->token, "'%s' argument %d: cannot pass %s as %s — use 'value as %s'",
			            node->token->name, i + 1, to_string(arg_type), to_string(param_type),
			            to_string(param_type));
			break;
		}
	}
}

void ir_regular_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);
	ir_regular_call_args(node, func);
	free_node(func);
	set_ret_type(node);
}

void ir_indirect_call(Node *node, Token *fn_var) {
	node->token->is_method_call = false;
	node->token->is_static_call = false;
	// Stash the fn variable so codegen can load it.
	node->token->Statement.ptr  = fn_var;

	int got = node->left ? node->left->children_count : 0;
	int exp = fn_var->Fn.params_count;
	if (got != exp) {
		parse_error(node->token, "'%s' expects %d arguments, got %d", node->token->name, exp, got);
		return;
	}

	for (int i = 0; i < got; i++) {
		Node  *carg = node->left->children[i];
		ir_gen(carg);
		if (found_error) return;
		Token *src   = carg->token;
		Token *param = fn_var->Fn.params[i];
		if (!compatible(param, src)) {
			parse_error(carg->token, "'%s' arg %d: cannot pass %s as %s", node->token->name, i + 1,
			            to_string(src->ret_type ? src->ret_type : src->type),
			            to_string(param->ret_type ? param->ret_type : param->type));
			return;
		}
	}

	// Set return type from signature
	if (fn_var->Fn.ret) {
		node->token->ret_type = fn_var->Fn.ret->ret_type ? fn_var->Fn.ret->ret_type
		                                                  : fn_var->Fn.ret->type;
	}
}

void ir_static_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func)                return;
	if (!func->token->is_pub) {
		parse_error(node->token, "'%s' is not a `pub fn` — call it with '.' instead of '::'",
		            node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	if (func->token->ret_type == STRUCT_CALL) {
		node->token->ret_type   = STRUCT_CALL;
		node->token->Struct.ptr = func->token->Struct.ptr;
	} else node->token->ret_type = func->token->ret_type;
	for (int i = 0; i < node->left->children_count; i++)
		ir_gen(node->left->children[i]);
}

bool try_module_call(Node *node) {
	if (!node->token->is_method_call || !node->left || node->left->children_count == 0) return false;
	Node *last = node->left->children[node->left->children_count - 1];
	if (last->token->type != ID || !last->token->name) return false;
	char *qname = format("%s.%s", last->token->name, node->token->name);
	Node *func  = find_function(qname);
	if (!func) {
		free(qname);
		return false;
	}
	// Module functions are accessed via `::`, not `.`. Same rule as
	// struct `pub fn`. Reject the dot form here so the user gets a
	// helpful message instead of a silent dispatch.
	parse_error(node->token,
	            "'%s' is a module function — call it with '::' instead of '.'", qname);
	free(qname);
	return true;
}

void gen_struct_declaration(Token *token) {
	if (!token->is_dec)   return;
	if (token->is_global) {
		if (token->used == 0) {
			token->is_dec = false;
			return;
		}
		TypeRef type     = get_llvm_type(token);
		token->llvm.elem = LLVMAddGlobal(module, type, token->name);
		LLVMSetInitializer(token->llvm.elem, LLVMConstNull(type));
	} else if (token->is_ref) {
	} else {
		_alloca(token);
		TypeRef type = get_llvm_type(token);
		LLVMBuildStore(builder, LLVMConstNull(type), token->llvm.elem);
	}
	token->is_dec = false;
}

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

Value emit_copy_construct(Token *param, Token *arg) {
	if (!param || param->type != STRUCT_CALL) return NULL;
	bool arg_is_struct = arg->type == STRUCT_CALL || arg->ret_type == STRUCT_CALL;
	if (!arg_is_struct) return NULL;
	Node *st_node = param->Struct.ptr;
	if (!st_node) return NULL;
	char *cp_name = format("%s" OP_PREFIX "ASSIGN.%s", st_node->token->name, st_node->token->name);
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

Value gen_operator_call(Node *node, Node *left, Node *right, bool try_copy_ctor) {
	Node *func    = node->token->Fcall.ptr;
	LLVM  srcFunc = ensure_func_resolved(func);
	if (CHECK(!srcFunc.func_type, "operator: func_type NULL for '%s'", node->token->name))
		return NULL;
	if (CHECK(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name)) return NULL;
	asm_gen(left);
	asm_gen(right);
	Token *param        = func->left->children_count >= 2 ? func->left->children[0]->token : NULL;
	Value  rhs_val      = NULL;
	bool   param_is_ref = param ? param->is_ref : false;
	if (try_copy_ctor && !param_is_ref && !right->token->is_ref)
		rhs_val = emit_copy_construct(param, right->token);
	if (!rhs_val) rhs_val = marshal_arg_for_op(param, right);
	Value args[2] = {rhs_val, left->token->llvm.elem};
	return LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, 2, "");
}

Value asm_assign_cast(Node *left, Node *right, Value val) {
	if (includes(right->token->type, STACK, HEAP, 0) && left->token->type == DOT) {
		TypeRef field_type = get_llvm_type(left->token);
		val                = LLVMBuildBitCast(builder, val, field_type, "field_cast");
	}
	if (val && LLVMGetTypeKind(LLVMTypeOf(val)) == PointerType) {
		TypeRef dest_type = get_llvm_type(left->token);
		if (dest_type && LLVMGetTypeKind(dest_type) == PointerType && LLVMTypeOf(val) != dest_type)
			val = LLVMBuildBitCast(builder, val, dest_type, "ptr_cast");
	}
	return val;
}

Value asm_as_int_to_int(Value src, TypeRef stype, TypeRef ttype) {
	unsigned sb = LLVMGetIntTypeWidth(stype);
	unsigned tb = LLVMGetIntTypeWidth(ttype);
	if (sb > tb) return LLVMBuildTrunc(builder, src, ttype, "as");
	if (sb < tb) return LLVMBuildSExt(builder, src, ttype, "as");
	return src;
}

Value asm_collect_dims(Node *node) {
	Value total = LLVMConstInt(i32, 1, 0);
	int   depth = node->token->Array.depth;
	for (int i = 0; i < depth; i++) {
		asm_gen(node->children[i]);
		ensure_loaded(node->children[i]);
		Value dv = node->children[i]->token->llvm.elem;
		resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
		             node->token->llvm.dims_count);
		node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
		total = LLVMBuildMul(builder, total, dv, "dim");
	}
	return total;
}

Value asm_total_bytes(Value total, TypeRef elem_t) {
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, elem_t);
	return LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");
}

TypeRef asm_array_lit_elem_type(Node *node) {
	TypeRef elem_t;
	Type    et = node->token->Array.sub_type;
	if (et == STRUCT_CALL && node->token->Array.struct_ptr)
		elem_t = get_llvm_type(node->token->Array.struct_ptr->token);
	else {
		Token tmp = {.type = et};
		elem_t    = get_llvm_type(&tmp);
	}
	for (int d = 1; d < node->token->Array.depth; d++)
		elem_t = LLVMPointerType(elem_t, 0);
	return elem_t;
}

void asm_dot_propagate_field_dims(Token *struct_tok, int field_index, Token *target) {
	if (!struct_tok->Struct.ptr) return;
	Node *sd = struct_tok->Struct.ptr;
	if (field_index < 0 || field_index >= sd->children_count) return;
	Token *field_def = sd->children[field_index]->token;
	if (field_def->llvm.dims_count == 0) return;
	target->llvm.dims_count = 0;
	for (int d = 0; d < field_def->llvm.dims_count; d++) {
		resize_array(target->llvm.dims, Value, target->llvm.dims_size, target->llvm.dims_count);
		target->llvm.dims[target->llvm.dims_count++] = field_def->llvm.dims[d];
	}
}

void asm_access_struct_field(Node *node) {
	Token *struct_tok      = node->left->token;
	int    field_idx       = node->right->token->Struct.index;
	node->token->llvm.elem = struct_field_ptr(struct_tok, field_idx, node->right->token->name);
}

bool asm_access_multidim(Node *node, Value left_value, Value right_ref) {
	int left_depth = node->left->token->llvm.dims_count;
	if (left_depth <= 1) return false;
	Value stride = LLVMConstInt(i32, 1, 0);
	for (int d = 1; d < left_depth; d++)
		stride = LLVMBuildMul(builder, stride, node->left->token->llvm.dims[d], "stride");
	Value   flat_idx = LLVMBuildMul(builder, right_ref, stride, "flat_idx");
	TypeRef base_t;
	if (node->left->token->Array.sub_type == STRUCT_CALL && node->left->token->Array.struct_ptr)
		base_t = get_llvm_type(node->left->token->Array.struct_ptr->token);
	else {
		Token tmp = {.type = node->left->token->Array.sub_type};
		base_t    = get_llvm_type(&tmp);
	}
	Value indices[]               = {flat_idx};
	node->token->llvm.elem        = LLVMBuildGEP2(builder, base_t, left_value, indices, 1, "row");
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

TypeRef asm_access_element_type(Node *node) {
	Type left_elem =
	    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (left_elem == CHARS) {
		node->token->ret_type = CHAR;
		return i8;
	}
	if (left_elem == ARRAY_TYPE || left_elem == ARRAY || node->left->token->type == HEAP ||
	    node->left->token->type == ARRAY) {
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

Value asm_access_left_value(Node *node) {
	Value lv;
	if (node->left->token->is_ref) {
		TODO(1, "stop");
		lv = read_value(node->left->token);
	} else {
		ensure_loaded(node->left);
		lv            = node->left->token->llvm.elem;
		TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(lv));
		if (kind == PointerType && node->left->token->name && !node->left->token->llvm.is_loaded &&
		    node->left->token->type != STACK && node->left->token->type != DOT)
			lv = LLVMBuildLoad2(builder, p8, lv, "ptr_load");
	}
	return lv;
}

Value marshal_fcall_arg(Token *param, Node *arg, bool is_proto_call) {
	bool param_is_ref = param ? param->is_ref : false;
	bool arg_is_ref   = arg->token->is_ref;
	if (param_is_ref && arg_is_ref) {
		TypeRef type = get_llvm_type(arg->token);
		return LLVMBuildLoad2(builder, LLVMPointerType(type, 0), arg->token->llvm.elem, "ref_arg");
	}
	if (param_is_ref && !arg_is_ref) return arg->token->llvm.elem;
	if (!param_is_ref && arg_is_ref) return read_value(arg->token);
	Value val = emit_copy_construct(param, arg->token);
	if (!val) {
		ensure_loaded(arg);
		val = arg->token->llvm.elem;
	}
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

void schedule_temp_cleanup(Token *token) {
	if (token->ret_type != STRUCT_CALL || !token->Struct.ptr) return;
	TypeRef st_type = get_llvm_type(token);
	Value   tmp     = LLVMBuildAlloca(builder, st_type, "tmp_struct");
	LLVMBuildStore(builder, token->llvm.elem, tmp);
	add_auto_clean(scope, tmp, token->Struct.ptr);
}

void call_delete(char *type_name, Value self_ptr) {
	char *qname = format("%s.delete", type_name);
	Value fn    = LLVMGetNamedFunction(module, qname);
	free(qname);
	if (!fn) return;
	Value args[] = {self_ptr};
	LLVMBuildCall2(builder, LLVMGlobalGetValueType(fn), fn, args, 1, "");
}

void emit_scope_clean(Node *scope_node, int from, Token *skip) {
	for (int i = from; i < scope_node->variables_count; i++) {
		Token *var = scope_node->variables[i];
		if (var == skip)              continue;
		if (!var->llvm.elem)          continue;
		if (var->is_ref)              continue;
		if (var->type == STRUCT_CALL) call_delete(var->Struct.ptr->token->name, var->llvm.elem);
	}
	for (int i = 0; i < scope_node->auto_cleans_count; i++)
		call_delete(scope_node->auto_cleans[i].type->token->name, scope_node->auto_cleans[i].value);
	scope_node->auto_cleans_count = 0;
}

void asm_fcall_static(Node *node) {
	Node  *func  = node->token->Fcall.ptr;
	LLVM   srcFn = ensure_func_resolved(func);
	int    argc  = node->left->children_count;
	Value *args  = allocate(argc + 1, sizeof(Value));
	for (int i = 0; i < argc; i++) {
		asm_gen(node->left->children[i]);
		ensure_loaded(node->left->children[i]);
		args[i] = node->left->children[i]->token->llvm.elem;
	}
	node->token->llvm.elem = LLVMBuildCall2(builder, srcFn.func_type, srcFn.elem, args, argc, "");
	if (node->token->ret_type != VOID) node->token->llvm.is_loaded = true;
	free(args);
	schedule_temp_cleanup(node->token);
}

void asm_fcall_marshal_args(Node *node, Value *args, int *count_out, bool is_proto) {
	bool   is_variadic = node->token->Fcall.ptr->token->is_variadic;
	int    count       = node->left->children_count;
	Node **argNodes    = node->left->children;
	Node  *dec_args    = node->token->Fcall.ptr->left;
	int    fixed       = is_variadic ? dec_args->children_count : count;
	for (int i = 0; i < fixed; i++) {
		asm_gen(argNodes[i]);
		Token *param = (i < dec_args->children_count) ? dec_args->children[i]->token : NULL;
		args[i]      = marshal_fcall_arg(param, argNodes[i], is_proto);
	}
	if (is_variadic) {
		// Ura-defined variadics: prepend the i32 arg-count so the callee can
		// walk its varargs. Libc protos (printf/scanf/etc.) use real C ABI —
		// no count, varargs go straight after the fixed args.
		bool add_count     = !is_proto;
		int  variadic_count = count - fixed;
		int  out_idx       = fixed;
		if (add_count) {
			args[out_idx++] = LLVMConstInt(i32, variadic_count, 0);
		}
		for (int i = fixed; i < count; i++) {
			asm_gen(argNodes[i]);
			ensure_loaded(argNodes[i]);
			args[out_idx++] = argNodes[i]->token->llvm.elem;
		}
		if (add_count) count++;
	}
	*count_out = count;
}

void asm_fcall_unpack_proto_struct(Node *node) {
	Value   i64_ret = node->token->llvm.elem;
	TypeRef st_type = get_llvm_type(node->token);
	Value   i64p    = LLVMBuildAlloca(builder, i64, "ret_i64");
	LLVMBuildStore(builder, i64_ret, i64p);
	Value st_ptr = LLVMBuildAlloca(builder, st_type, "ret_struct");
	LLVMBuildMemCpy(builder, st_ptr, 0, i64p, 0, LLVMConstInt(i64, 4, 0));
	node->token->llvm.elem = LLVMBuildLoad2(builder, st_type, st_ptr, "ret_struct_val");
}

// Indirect call: callee is a fn-typed local variable (Statement.ptr).
// Build the LLVM function type from the variable's FN_TYPE signature,
// load the stored function pointer, and dispatch.
void asm_fcall_indirect(Node *node) {
	Token *fn_var = node->token->Statement.ptr;
	if (!fn_var || !fn_var->llvm.elem) return;

	// Build LLVM function type from the signature.
	int      n      = fn_var->Fn.params_count;
	TypeRef  ret_t  = fn_var->Fn.ret ? get_llvm_type(fn_var->Fn.ret) : vd;
	TypeRef *ptypes = NULL;
	if (n > 0) {
		ptypes = allocate(n, sizeof(TypeRef));
		for (int i = 0; i < n; i++) ptypes[i] = get_llvm_type(fn_var->Fn.params[i]);
	}
	TypeRef ftype = LLVMFunctionType(ret_t, ptypes, n, 0);

	// Load fn pointer from the variable's storage.
	Value fn_ptr = LLVMBuildLoad2(builder, LLVMPointerType(ftype, 0), fn_var->llvm.elem, "fn_ptr");

	// Marshal args.
	int    arg_count = node->left ? node->left->children_count : 0;
	Value *args      = NULL;
	if (arg_count > 0) {
		args = allocate(arg_count, sizeof(Value));
		for (int i = 0; i < arg_count; i++) {
			Node *carg = node->left->children[i];
			asm_gen(carg);
			ensure_loaded(carg);
			args[i] = carg->token->llvm.elem;
		}
	}

	const char *name      = (ret_t != vd) ? "indirect_call" : "";
	node->token->llvm.elem = LLVMBuildCall2(builder, ftype, fn_ptr, args, arg_count, name);
	free(args);
	free(ptypes);
}

void asm_fcall_instance(Node *node) {
	LLVM   srcFunc  = ensure_func_resolved(node->token->Fcall.ptr);
	bool   is_proto = node->token->Fcall.ptr->token->is_proto;
	int    count    = node->left->children_count;
	Value *args     = NULL;
	if (count) {
		args = allocate(count + 3, sizeof(Value));
		asm_fcall_marshal_args(node, args, &count, is_proto);
	}
	if (CHECK(!srcFunc.func_type, "FCALL: func_type NULL '%s'", node->token->name)) {
		free(args);
		return;
	}
	if (CHECK(!srcFunc.elem, "FCALL: elem NULL '%s'", node->token->name)) {
		free(args);
		return;
	}
	char *name = node->token->Fcall.ptr->token->ret_type != VOID ? node->token->name : "";
	node->token->llvm.elem =
	    LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, count, name);
	free(args);
	if (is_proto && node->token->Fcall.ptr->token->ret_type == STRUCT_CALL)
		asm_fcall_unpack_proto_struct(node);
	schedule_temp_cleanup(node->token);
}

void append_string_literal_to_fmt(const char *s, char *fmt, int *fc) {
	for (int i = 0; s[i]; i++) {
		if (s[i] == '%') {
			fmt[(*fc)++] = '%';
			fmt[(*fc)++] = '%';
		} else if (s[i] == '\\' && s[i + 1]) {
			switch (s[++i]) {
			case 'n':  fmt[(*fc)++] = '\n'; break;
			case 't':  fmt[(*fc)++] = '\t'; break;
			case 'r':  fmt[(*fc)++] = '\r'; break;
			case '\\': fmt[(*fc)++] = '\\'; break;
			case '"':  fmt[(*fc)++] = '"'; break;
			default:
				fmt[(*fc)++] = '\\';
				fmt[(*fc)++] = s[i];
				break;
			}
		} else fmt[(*fc)++] = s[i];
	}
}

void append_struct_with_output_op(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd,
                                  Value out_fn) {
	// If `tok` came from an FCALL/op result, llvm.elem holds a struct VALUE
	// rather than a pointer. The output overload expects `ref self`, so we
	// need an addressable temp whenever the operand is a literal struct value.
	Value   self_ptr = tok->llvm.elem;
	if (self_ptr && LLVMGetTypeKind(LLVMTypeOf(self_ptr)) == StructType) {
		TypeRef val_type = LLVMTypeOf(self_ptr);
		Value   tmp      = LLVMBuildAlloca(builder, val_type, "out_arg_tmp");
		LLVMBuildStore(builder, self_ptr, tmp);
		self_ptr = tmp;
	}
	TypeRef fn_type  = LLVMGlobalGetValueType(out_fn);
	Value   result   = LLVMBuildCall2(builder, fn_type, out_fn, &self_ptr, 1, "output_op");
	TypeRef ret_type = LLVMGetReturnType(fn_type);
	if (LLVMGetTypeKind(ret_type) == PointerType) {
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = result;
		return;
	}
	if (LLVMGetTypeKind(ret_type) != StructType) return;
	Value tmp = LLVMBuildAlloca(builder, ret_type, "out_tmp");
	LLVMBuildStore(builder, result, tmp);
	Node *ret_sd = NULL;
	for (int si = scopes_count; si > 0 && !ret_sd; si--)
		for (int sj = 0; sj < scopes[si]->structs_count; sj++)
			if (scopes[si]->structs[sj]->token->llvm.struct_type == ret_type) {
				ret_sd = scopes[si]->structs[sj];
				break;
			}
	if (!ret_sd) return;
	add_auto_clean(scope, tmp, ret_sd);
	Token ftok      = *tok;
	ftok.Struct.ptr = ret_sd;
	ftok.llvm.elem  = tmp;
	ftok.type       = STRUCT_CALL;
	(void)sd;
	append_output_arg(&ftok, fmt, fc, args, nargs);
}

void append_struct_default_fmt(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd) {
	fmt[(*fc)++] = '{';
	fmt[(*fc)++] = ' ';
	for (int i = 0; i < sd->children_count; i++) {
		Token *field          = sd->children[i]->token;
		int    field_name_len = strlen(field->name);
		memcpy(fmt + *fc, field->name, field_name_len);
		*fc += field_name_len;
		fmt[(*fc)++]   = ':';
		fmt[(*fc)++]   = ' ';
		Token ftok     = *field;
		ftok.llvm.elem = struct_field_ptr(tok, i, field->name);
		append_output_arg(&ftok, fmt, fc, args, nargs);
		if (i < sd->children_count - 1) {
			fmt[(*fc)++] = ',';
			fmt[(*fc)++] = ' ';
		}
	}
	fmt[(*fc)++] = ' ';
	fmt[(*fc)++] = '}';
}

Type append_resolve_type(Token *tok) {
	Type type = tok->type ? tok->type : tok->ret_type;
	switch (type) {
	case INT: case LONG: case SHORT: case CHAR: case CHARS: case BOOL:
	case FLOAT:
	case STRUCT_CALL: return type;
	case FCALL:       return tok->ret_type;
	default:          return tok->ret_type ? tok->ret_type : type;
	}
}

void append_output_arg(Token *tok, char *fmt, int *fc, Value *args, int *nargs) {
	if (tok->type == CHARS && !tok->name) {
		append_string_literal_to_fmt(tok->Chars.value, fmt, fc);
		return;
	}
	Type type = append_resolve_type(tok);
	switch (type) {
	case INT: case SHORT:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'd';
		args[(*nargs)++] = read_value(tok);
		return;
	case BOOL: {
		Value bv         = read_value(tok);
		Value ts         = LLVMBuildGlobalStringPtr(builder, "True", "true_str");
		Value fs         = LLVMBuildGlobalStringPtr(builder, "False", "false_str");
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = LLVMBuildSelect(builder, bv, ts, fs, "bool_str");
		return;
	}
	case LONG:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'l';
		fmt[(*fc)++]     = 'l';
		fmt[(*fc)++]     = 'd';
		args[(*nargs)++] = read_value(tok);
		return;
	case CHAR:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'c';
		args[(*nargs)++] = read_value(tok);
		return;
	case CHARS:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = read_value(tok);
		return;
	case FLOAT:
		fmt[(*fc)++] = '%';
		fmt[(*fc)++] = 'f';
		args[(*nargs)++] =
		    LLVMBuildFPExt(builder, read_value(tok), LLVMDoubleTypeInContext(context), "f2d");
		return;
	case STRUCT_CALL: {
		Node *sd        = tok->Struct.ptr;
		char *out_name  = format("%s" OP_PREFIX "output", sd->token->name);
		Value out_fn    = LLVMGetNamedFunction(module, out_name);
		free(out_name);
		// Fallback: prep files emit the output overload as a flat `<Struct>_output`.
		// The flat fn may not be pre-declared in LLVM (used=0 at IR time since
		// the formatter is the only caller) — resolve it on demand from scope.
		if (!out_fn) {
			char *flat_name = format("%s_output", sd->token->name);
			out_fn          = LLVMGetNamedFunction(module, flat_name);
			if (!out_fn) {
				Node *fn = find_function(flat_name);
				if (fn) {
					resolve_func_type(fn);
					out_fn = LLVMGetNamedFunction(module, flat_name);
				}
			}
			free(flat_name);
		}
		if (out_fn) append_struct_with_output_op(tok, fmt, fc, args, nargs, sd, out_fn);
		else append_struct_default_fmt(tok, fmt, fc, args, nargs, sd);
		return;
	}
	default: fmt[(*fc)++] = '?'; return;
	}
}

int output_format_capacity(int argc, Node **argv) {
	int cap = 64;
	for (int i = 0; i < argc; i++) {
		if (argv[i]->token->type == CHARS && !argv[i]->token->name)
			cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
		else cap += 128;
	}
	return cap;
}

void if_chain_branch(Node *curr, Block if_start, Block end) {
	Block start = curr->token->type == IF ? if_start : curr->token->llvm.bloc;
	Block then  = _append_block(curr->token->type == IF ? "if.then" : "elif.then");
	Block next;
	if (curr->right)
		next = _append_block(curr->right->token->type == ELSE ? "if.else" : "elif.start");
	else next = end;
	curr->token->llvm.then = then;
	LLVMPositionBuilderAtEnd(builder, start);
	asm_gen(curr->left);
	ensure_loaded(curr->left);
	LLVMBuildCondBr(builder, curr->left->token->llvm.elem, then, next);
	LLVMPositionBuilderAtEnd(builder, then);
	for (int i = 0; i < curr->children_count; i++) {
		asm_gen(curr->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
	_branch(end);
	if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
		curr->right->token->llvm.bloc = next;
}

void if_chain_else(Node *curr, Block end) {
	LLVMPositionBuilderAtEnd(builder, curr->token->llvm.bloc);
	for (int i = 0; i < curr->children_count; i++) {
		asm_gen(curr->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
	_branch(end);
}

void gen_if_chain(Node *node, Block if_start, Block end) {
	Node *curr = node;
	while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
		if (includes(curr->token->type, IF, ELIF, 0)) if_chain_branch(curr, if_start, end);
		else if (curr->token->type == ELSE)           if_chain_else(curr, end);
		curr = curr->right;
	}
}

void asm_return_delete_chain(Node *fdec) {
	if (fdec->left->children_count < 1) return;
	Token *self_tok = fdec->left->children[0]->token;
	if (self_tok->type != STRUCT_CALL || !self_tok->is_ref) return;
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

void asm_return_main_globals(void) {
	for (int i = 0; i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
		if (child->token->used == 0)                                       continue;
		call_delete(child->token->Struct.ptr->token->name, child->token->llvm.elem);
	}
}

void asm_return_tuple(Node *node, Node *fdec) {
	TypeRef tuple_type = fdec->token->llvm.struct_type;
	Value   agg        = LLVMGetUndef(tuple_type);
	for (int i = 0; i < node->children_count; i++) {
		asm_gen(node->children[i]);
		ensure_loaded(node->children[i]);
		agg = LLVMBuildInsertValue(builder, agg, node->children[i]->token->llvm.elem, i, "");
	}
	LLVMBuildRet(builder, agg);
}

void asm_return_value(Node *node, Node *fdec) {
	if (!node->left) return;
	Token *ret_tok = node->left->token;
	if (ret_tok->type == VOID) {
		LLVMBuildRetVoid(builder);
		return;
	}
	asm_gen(node->left);
	if (fdec->token->ret_type == STRUCT_CALL && !fdec->token->is_ref) {
		LLVMBuildRet(builder, read_value(ret_tok));
		return;
	}
	if (fdec->token->ret_type == STRUCT_CALL && fdec->token->is_ref) {
		TypeRef st_type  = get_llvm_type(ret_tok);
		TypeRef ptr_type = LLVMPointerType(st_type, 0);
		Value   ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
		LLVMBuildRet(builder, ptr);
		return;
	}
	ensure_loaded(node->left);
	LLVMBuildRet(builder, node->left->token->llvm.elem);
}

void gen_struct_emit_nested(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Token *ft = node->children[i]->token;
		if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0) {
			ft->Struct.ptr->token->used++;
			asm_gen(ft->Struct.ptr);
		}
		if (includes(ft->type, ARRAY_TYPE, ARRAY, 0) && ft->Array.sub_type == STRUCT_CALL &&
		    ft->Array.struct_ptr && ft->Array.struct_ptr->token->used == 0) {
			ft->Array.struct_ptr->token->used++;
			asm_gen(ft->Array.struct_ptr);
		}
	}
}

void gen_struct_build_type(Node *node) {
	int      pos   = node->children_count;
	TypeRef *types = allocate(pos + 1, sizeof(TypeRef));
	for (int i = 0; i < pos; i++)
		types[i] = get_llvm_type(node->children[i]->token);
	char *struct_name             = format("struct.%s", node->token->name);
	node->token->llvm.struct_type = _named_struct_type(struct_name, types, pos, 0);
	free(struct_name);
	free(types);
}

void gen_struct_emit_delete(Node *node) {
	if (node->token->has_clean) return;
	TypeRef st_type     = node->token->llvm.struct_type;
	TypeRef ptr_type    = LLVMPointerType(st_type, 0);
	TypeRef lc_params[] = {ptr_type};
	TypeRef lc_fn_type  = LLVMFunctionType(vd, lc_params, 1, 0);
	char   *fname       = format("%s.delete", node->token->name);
	Value   fn          = _add_function(fname, lc_fn_type);
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

void gen_struct_predeclare_methods(Node *node) {
	for (int i = 0; i < node->functions_count; i++)
		resolve_func_type(node->functions[i]);
}

// ============================================================================
// PREPROCESSED SOURCE OUTPUT (-prep flag)
//
// The -prep dump is meant to be a flat, C-style, re-parseable Ura source.
// Conventions:
//   * Struct methods are HOISTED to free functions named `<Struct>_<method>`.
//   * Operator overloads become `<Struct>_<op>_<RHS>` with a uniform RHS suffix.
//   * Method calls are flattened: `s.assign(x)` -> `String_assign(ref s, x)`.
//   * Operator infix is flattened: `a + b` -> `String_add_String(ref a, ref b)`.
//   * `output(s)` for any struct with operator output is wrapped:
//       `output(String_output(ref s))`.
//   * Ref params/locals are written `ref name type`.
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

// Lowercase op suffix used in flat operator function names: ADD -> "add", LT -> "lt".
// Look up an operator overload registered on a struct by op-type and RHS-type-name.
// Returns the matching function node, or NULL if none.
static Node *prep_find_struct_op(Node *struct_def, const char *op_kw, const char *rhs_id) {
	if (!struct_def) return NULL;
	for (int i = 0; i < struct_def->functions_count; i++) {
		Node *fn = struct_def->functions[i];
		if (!fn->token->name) continue;
		if (op_kw && rhs_id) {
			char *want = format("%s" OP_PREFIX "%s.%s",
			    struct_def->token->name, op_kw, rhs_id);
			if (strcmp(fn->token->name, want) == 0) { free(want); return fn; }
			free(want);
		}
	}
	return NULL;
}

// Decode an IR-mangled fdec name into the prep flat name.
// Inputs like:
//   "String.assign"                      -> "String_assign"
//   "String.delete"                      -> "String_delete"
//   "String.operator.output"             -> "String_output"
//   "String.operator.ASSIGN.String"      -> "String_assign_String"
//   "String.operator.ADD.CHARS"          -> "String_add_chars"
// Caller frees the returned string.
static char *prep_fn_flat_name(const char *mangled) {
	if (!mangled) return strdup("?");
	// Find ".operator." substring.
	const char *opmark = strstr(mangled, OP_PREFIX);
	if (!opmark) {
		// Plain method: "<Struct>.<method>" -> "<Struct>_<method>"
		char *copy = strdup(mangled);
		for (char *p = copy; *p; p++) if (*p == '.') *p = '_';
		return copy;
	}
	// Split: <struct> ".operator." <op_kw> [ "." <rhs> ]
	int   struct_len = (int)(opmark - mangled);
	char *struct_part = strndup(mangled, struct_len);
	const char *after = opmark + strlen(OP_PREFIX);
	const char *dot   = strchr(after, '.');
	char *op_kw       = dot ? strndup(after, dot - after) : strdup(after);
	const char *rhs   = dot ? dot + 1 : NULL;

	// Lowercase op_kw to suffix form.
	// op_kw arrives as the to_string() name (e.g. "ASSIGN", "ADD", "LT", "EQ"...).
	const char *suffix = NULL;
	if      (strcmp(op_kw, "ASSIGN")  == 0) suffix = "assign";
	else if (strcmp(op_kw, "ADD")     == 0) suffix = "add";
	else if (strcmp(op_kw, "SUB")     == 0) suffix = "sub";
	else if (strcmp(op_kw, "MUL")     == 0) suffix = "mul";
	else if (strcmp(op_kw, "DIV")     == 0) suffix = "div";
	else if (strcmp(op_kw, "MOD")     == 0) suffix = "mod";
	else if (strcmp(op_kw, "EQ")      == 0) suffix = "eq";
	else if (strcmp(op_kw, "NEQ")     == 0) suffix = "ne";
	else if (strcmp(op_kw, "LT")      == 0) suffix = "lt";
	else if (strcmp(op_kw, "GT")      == 0) suffix = "gt";
	else if (strcmp(op_kw, "LE")      == 0) suffix = "le";
	else if (strcmp(op_kw, "GE")      == 0) suffix = "ge";
	else if (strcmp(op_kw, "BAND")    == 0) suffix = "band";
	else if (strcmp(op_kw, "BOR")     == 0) suffix = "bor";
	else if (strcmp(op_kw, "BXOR")    == 0) suffix = "bxor";
	else if (strcmp(op_kw, "LSHIFT")  == 0) suffix = "lshift";
	else if (strcmp(op_kw, "RSHIFT")  == 0) suffix = "rshift";
	else if (strcmp(op_kw, "ADD_ASS") == 0) suffix = "add_assign";
	else if (strcmp(op_kw, "SUB_ASS") == 0) suffix = "sub_assign";
	else if (strcmp(op_kw, "MUL_ASS") == 0) suffix = "mul_assign";
	else if (strcmp(op_kw, "DIV_ASS") == 0) suffix = "div_assign";
	else if (strcmp(op_kw, "MOD_ASS") == 0) suffix = "mod_assign";
	else if (strcmp(op_kw, "output")  == 0) suffix = "output";
	else if (strcmp(op_kw, "delete")  == 0) suffix = "delete";
	else                                    suffix = op_kw;

	// Lowercase RHS only for primitive type names; STRUCT_CALL stays original case.
	char *rhs_norm = NULL;
	if (rhs) {
		if      (strcmp(rhs, "INT")   == 0) rhs_norm = strdup("int");
		else if (strcmp(rhs, "LONG")  == 0) rhs_norm = strdup("long");
		else if (strcmp(rhs, "SHORT") == 0) rhs_norm = strdup("short");
		else if (strcmp(rhs, "CHAR")  == 0) rhs_norm = strdup("char");
		else if (strcmp(rhs, "CHARS") == 0) rhs_norm = strdup("chars");
		else if (strcmp(rhs, "BOOL")  == 0) rhs_norm = strdup("bool");
		else if (strcmp(rhs, "FLOAT") == 0) rhs_norm = strdup("float");
		else if (strcmp(rhs, "PTR")   == 0) rhs_norm = strdup("pointer");
		else                                rhs_norm = strdup(rhs);
	}

	char *out = rhs_norm
	    ? format("%s_%s_%s", struct_part, suffix, rhs_norm)
	    : format("%s_%s", struct_part, suffix);

	free(struct_part);
	free(op_kw);
	free(rhs_norm);
	return out;
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
		fprintf(f, "array");
		int depth = tok->Array.depth > 0 ? tok->Array.depth : 1;
		for (int i = 0; i < depth; i++) fputc('[', f);
		const char *en = type_to_ura_name(tok->Array.sub_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.sub_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		for (int i = 0; i < depth; i++) fputc(']', f);
		break;
	}
	case FN_TYPE: {
		fprintf(f, "fn(");
		for (int i = 0; i < tok->Fn.params_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_type_name(f, tok->Fn.params[i]);
		}
		fprintf(f, ") ");
		if (tok->Fn.ret) emit_type_name(f, tok->Fn.ret);
		else fprintf(f, "void");
		break;
	}
	default: fprintf(f, "%s", to_string(t)); break;
	}
}

static void emit_chars_literal(File f, const char *s) {
	fprintf(f, "\"");
	for (int i = 0; s[i]; i++) {
		switch (s[i]) {
		case '\n': fprintf(f, "\\n"); break;
		case '\t': fprintf(f, "\\t"); break;
		case '\r': fprintf(f, "\\r"); break;
		case '\\': fprintf(f, "\\\\"); break;
		case '"':  fprintf(f, "\\\""); break;
		default:   fputc(s[i], f); break;
		}
	}
	fprintf(f, "\"");
}

static void emit_char_literal(File f, int c) {
	fprintf(f, "'");
	switch (c) {
	case '\n': fprintf(f, "\\n"); break;
	case '\t': fprintf(f, "\\t"); break;
	case '\r': fprintf(f, "\\r"); break;
	case '\\': fprintf(f, "\\\\"); break;
	case '\'': fprintf(f, "\\'"); break;
	case '\0': fprintf(f, "\\0"); break;
	default:   fputc(c, f); break;
	}
	fprintf(f, "'");
}

static void emit_expr_call(File f, Node *node, Token *tok) {
	// Method call: `<recv>.method(args)` -> `<Struct>_method(ref <recv>, args...)`.
	// In the post-IR AST, the call's last argument is the receiver (STRUCT_CALL),
	// and tok->name is the IR-mangled `<Struct>.<method>` form.
	if (node->left && tok->name && strchr(tok->name, '.') && !tok->is_static_call) {
		int last = node->left->children_count - 1;
		if (last >= 0 && node->left->children[last]->token->type == STRUCT_CALL) {
			char *flat = prep_fn_flat_name(tok->name);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left->children[last]);
			for (int i = 0; i < last; i++) {
				fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
			fprintf(f, ")");
			free(flat);
			return;
		}
	}
	// Static dispatch: `Type::method(args)` -> `Type_method(args)`.
	if (tok->is_static_call && tok->name && strchr(tok->name, '.')) {
		char *flat = prep_fn_flat_name(tok->name);
		fprintf(f, "%s(", flat);
		if (node->left)
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
		fprintf(f, ")");
		free(flat);
		return;
	}
	fprintf(f, "%s(", tok->name);
	if (node->left)
		for (int i = 0; i < node->left->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->left->children[i]);
		}
	fprintf(f, ")");
}

// Always-parenthesize binary-op operand when it's itself a binary op. This
// preserves precedence after the prep round-trip without maintaining a full
// precedence table — slightly noisier output, but unambiguous.
static bool is_binop_token_type(Type t) {
	return includes(t,
	    ADD, SUB, MUL, DIV, MOD,
	    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
	    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, 0);
}

static void emit_binop_operand(File f, Node *child) {
	bool wrap = child && child->token && is_binop_token_type(child->token->type)
	    && !child->token->Fcall.ptr;
	if (wrap) fprintf(f, "(");
	emit_expr(f, child);
	if (wrap) fprintf(f, ")");
}

static void emit_expr_binop(File f, Node *node, Token *tok) {
	if (tok->Fcall.ptr) {
		// Operator overload resolved by typechecker.
		// Emit `<Struct>_<op>_<RHS>(ref lhs, rhs)`.
		char *flat = prep_fn_flat_name(tok->Fcall.ptr->token->name);
		fprintf(f, "%s(ref ", flat);
		emit_expr(f, node->left);
		fprintf(f, ", ");
		emit_expr(f, node->right);
		fprintf(f, ")");
		free(flat);
		return;
	}
	emit_binop_operand(f, node->left);
	fprintf(f, " %s ", op_symbol(tok->type));
	emit_binop_operand(f, node->right);
}

static void emit_expr(File f, Node *node) {
	if (!node || !node->token) return;
	Token *tok = node->token;
	switch (tok->type) {
	case INT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%lld", (long long)tok->Int.value);
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
		else fprintf(f, "%s", tok->Bool.value ? "True" : "False");
		break;
	case CHAR:
		if (tok->name) fprintf(f, "%s", tok->name);
		else emit_char_literal(f, (unsigned char)tok->Char.value);
		break;
	case CHARS:
		if (tok->name) fprintf(f, "%s", tok->name);
		else emit_chars_literal(f, tok->Chars.value ? tok->Chars.value : "");
		break;
	case NULLABLE:    fprintf(f, "null"); break;
	case VOID:        fprintf(f, "void"); break;
	case STRUCT_CALL: fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case ARRAY_TYPE:  if (tok->name) fprintf(f, "%s", tok->name); break;
	case ID:          fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case FN_TYPE:
		// Fn-typed value: either a local variable (use its name) or a top-level
		// function reference resolved by ir_id (use the resolved fn's name).
		if (tok->Fcall.ptr && tok->Fcall.ptr->token->name)
			fprintf(f, "%s", tok->Fcall.ptr->token->name);
		else if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "?");
		break;
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
		// `Type::method` becomes `Type_method` in the flat prep form.
		emit_expr(f, node->left);
		fprintf(f, "_");
		emit_expr(f, node->right);
		break;
	case AS: {
		// Parenthesize the value if it's any binary op so `(a+b) as long`
		// doesn't degrade into `a+b as long` (which would cast only `b`).
		bool needs_parens = node->left && node->left->token && includes(
		    node->left->token->type,
		    ADD, SUB, MUL, DIV, MOD,
		    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
		    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, 0);
		if (needs_parens) fprintf(f, "(");
		emit_expr(f, node->left);
		if (needs_parens) fprintf(f, ")");
		fprintf(f, " as ");
		emit_type_name(f, node->right->token);
		break;
	}
	case NOT:  fprintf(f, "not "); emit_expr(f, node->left); break;
	case BNOT: fprintf(f, "~"); emit_expr(f, node->left); break;
	case TYPEOF:
		fprintf(f, "typeof(");
		emit_expr(f, node->left);
		fprintf(f, ")");
		break;
	case SIZEOF:
		fprintf(f, "sizeof(");
		if (node->left && node->left->token) {
			const char *tn = type_to_ura_name(node->left->token->type);
			if (tn) fprintf(f, "%s", tn);
			else emit_expr(f, node->left);
		}
		fprintf(f, ")");
		break;
	case REF:       fprintf(f, "ref "); emit_expr(f, node->left); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    emit_expr_binop(f, node, tok); break;
	case FCALL:     emit_expr_call(f, node, tok); break;
	case HEAP:      case STACK: {
		// `stack[T](dims)` with `Array.depth` opening/closing brackets — preserves
		// the multi-dimensional form: depth=2 -> `stack[[T]](r, c)`.
		fprintf(f, "%s", tok->type == HEAP ? "heap" : "stack");
		int depth = tok->Array.depth > 0 ? tok->Array.depth : 1;
		for (int i = 0; i < depth; i++) fputc('[', f);
		const char *en = type_to_ura_name(tok->Array.sub_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.sub_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		else fprintf(f, "?");
		for (int i = 0; i < depth; i++) fputc(']', f);
		fprintf(f, "(");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, ")");
		break;
	}
	case ARRAY_LIT:
		fprintf(f, "[");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, "]");
		break;
	default: fprintf(f, "?"); break;
	}
}

static bool prep_fdec_has_self(Node *node) {
	if (!node->left || node->left->children_count == 0) return false;
	Token *last = node->left->children[node->left->children_count - 1]->token;
	return last->type == STRUCT_CALL && last->name && strcmp(last->name, "self") == 0;
}

// Print a parameter or local declaration name + type, using `ref name type` form
// when is_ref is set.
static void emit_param_decl(File f, Token *p) {
	if (p->is_ref) fprintf(f, "ref %s ", p->name);
	else fprintf(f, "%s ", p->name);
	emit_type_name(f, p);
}

// Emit destructor calls for every struct-typed local in `scope_node` that has a
// destructor defined. Skip the variable in `skip` (used when returning a
// struct so the caller owns the result). Mirrors emit_scope_clean in the asm
// path, but emitted as visible Ura source.
static void emit_struct_destructors(File f, Node *scope_node, int depth, Token *skip) {
	for (int i = 0; i < scope_node->variables_count; i++) {
		Token *v = scope_node->variables[i];
		if (!v || v == skip)                                     continue;
		if (v->is_ref || v->is_param)                            continue;
		if (v->type != STRUCT_CALL || !v->Struct.ptr)            continue;
		if (!v->Struct.ptr->token->has_clean)                    continue;
		emit_indent(f, depth);
		fprintf(f, "%s_delete(ref %s)\n", v->Struct.ptr->token->name, v->name);
	}
}

static void emit_node(File f, Node *node, int depth);

// Emit one fdec.
//   `flat_name`: when non-NULL, emit this flat name instead of decoding tok->name
//                (used when the caller is hoisting a struct method to top level).
//   `struct_owner`: when non-NULL, this fdec was a struct method. If it had self,
//                   inject `ref self <Struct>` as the first parameter.
static void emit_fdec(File f, Node *node, int depth,
                      const char *flat_name, Node *struct_owner) {
	Token *tok      = node->token;
	bool   has_self = prep_fdec_has_self(node);
	emit_indent(f, depth);
	if (tok->is_proto) fprintf(f, "proto ");

	const char *name_to_emit;
	char       *owned = NULL;
	if (flat_name) {
		name_to_emit = flat_name;
	} else if (struct_owner) {
		owned        = prep_fn_flat_name(tok->name);
		name_to_emit = owned;
	} else if (tok->name && strchr(tok->name, '.')) {
		owned        = prep_fn_flat_name(tok->name);
		name_to_emit = owned;
	} else {
		name_to_emit = tok->name ? tok->name : "?";
	}
	fprintf(f, "fn %s(", name_to_emit);

	int  printed = 0;
	bool need_self_inject = struct_owner && has_self;
	if (need_self_inject) {
		fprintf(f, "ref self %s", struct_owner->token->name);
		printed = 1;
	}
	if (node->left) {
		int end = node->left->children_count - (has_self ? 1 : 0);
		for (int i = 0; i < end; i++) {
			Token *p = node->left->children[i]->token;
			if (printed > 0) fprintf(f, ", ");
			emit_param_decl(f, p);
			printed++;
		}
	}
	if (tok->is_variadic) {
		if (printed > 0) fprintf(f, ", ");
		fprintf(f, "...");
	}
	fprintf(f, ") ");
	if (tok->is_ref) fprintf(f, "ref ");
	if (tok->ret_type == TUPLE) {
		fprintf(f, "(");
		for (int i = 0; i < tok->Tuple.types_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_type_name(f, tok->Tuple.types[i]);
		}
		fprintf(f, ")");
	} else if (tok->ret_type == STRUCT_CALL && tok->Struct.ptr)
		fprintf(f, "%s", tok->Struct.ptr->token->name);
	else if (tok->ret_type) {
		const char *tn = type_to_ura_name(tok->ret_type);
		fprintf(f, "%s", tn ? tn : to_string(tok->ret_type));
	}
	if (tok->is_proto) {
		fprintf(f, "\n");
		free(owned);
		return;
	}
	fprintf(f, ":\n");
	// Emit body. Inject struct-destructor calls right before any `return`
	// statement we encounter, and once more at the very end if the function
	// fell through (void / implicit return).
	bool ends_with_return = false;
	for (int i = 0; i < node->children_count; i++) {
		Node *child = node->children[i];
		if (child && child->token && child->token->type == RETURN) {
			// Determine which struct local (if any) is being returned — skip
			// its destructor since ownership transfers to the caller.
			Token *ret_skip = NULL;
			if (child->left && child->left->token
			    && child->left->token->type == STRUCT_CALL)
				ret_skip = child->left->token;
			emit_struct_destructors(f, node, depth + 1, ret_skip);
			ends_with_return = (i == node->children_count - 1);
		}
		emit_node(f, child, depth + 1);
	}
	if (!ends_with_return)
		emit_struct_destructors(f, node, depth + 1, NULL);
	fprintf(f, "\n");
	free(owned);
}

// Emit only struct fields. Methods are hoisted; the caller emits them after.
static void emit_struct_def(File f, Node *node, int depth) {
	emit_indent(f, depth);
	fprintf(f, "struct %s:\n", node->token->name);
	for (int i = 0; i < node->children_count; i++) {
		Token *field = node->children[i]->token;
		emit_indent(f, depth + 1);
		emit_param_decl(f, field);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}

// Emit hoisted methods (if any) for a struct as top-level free functions.
static void emit_struct_methods(File f, Node *struct_def) {
	for (int i = 0; i < struct_def->functions_count; i++)
		emit_fdec(f, struct_def->functions[i], 0, NULL, struct_def);
}

static void emit_assign_node(File f, Node *node, int depth) {
	Token *tok  = node->token;
	Node  *left = node->left;
	emit_indent(f, depth);

	// LHS is a declaration: emit `name type [= rhs]`.
	// `tok->is_dec` is stamped at parse time when this ASSIGN's LHS was a fresh
	// declaration (not a re-assignment). is_dec on the LHS token alone is
	// unreliable post-IR because ir_id re-points re-uses to the decl token.
	if (tok->type == ASSIGN && tok->is_dec && left && left->token) {
		Token *lt = left->token;
		if (lt->is_ref) fprintf(f, "ref %s ", lt->name);
		else fprintf(f, "%s ", lt->name);
		emit_type_name(f, lt);
		fprintf(f, " = ");
		emit_expr(f, node->right);
		fprintf(f, "\n");
		return;
	}

	// Operator-overloaded assign: always emit as a flat call. The op's RHS
	// param is ref-typed, and the parser rejects `ref <fcall_result>`. When
	// the RHS isn't a "simple" named lvalue (literal, ID, field, index), we
	// hoist it into a fresh `__prep_tmpN <type>` local first and pass the
	// temp by ref.
	Token *rt = node->right ? node->right->token : NULL;
	bool rhs_simple = rt && (
	    rt->name != NULL
	    || includes(rt->type,
	        INT, LONG, SHORT, FLOAT, BOOL, CHAR, CHARS, NULLABLE, DOT, ACCESS, 0));
	if (tok->Fcall.ptr && rt && rt->type == FCALL) rhs_simple = false;
	if (tok->Fcall.ptr && includes(rt ? rt->type : 0,
	    ADD, SUB, MUL, DIV, MOD,
	    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
	    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, AS, 0))
		rhs_simple = false;

	if (tok->Fcall.ptr) {
		char *flat = prep_fn_flat_name(tok->Fcall.ptr->token->name);
		if (rhs_simple) {
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left);
			fprintf(f, ", ");
			emit_expr(f, node->right);
			fprintf(f, ")\n");
		} else {
			// Resolve the RHS function's second-parameter type for the temp
			// decl. tok->Fcall.ptr->left->children = [rhs_param, self].
			Node  *op_fn   = tok->Fcall.ptr;
			Token *rhs_param = NULL;
			if (op_fn->left && op_fn->left->children_count >= 1)
				rhs_param = op_fn->left->children[0]->token;
			static int prep_tmp_id = 0;
			char *tmp_name = format("__prep_tmp%d", prep_tmp_id++);
			fprintf(f, "%s ", tmp_name);
			if (rhs_param) emit_type_name(f, rhs_param);
			else fprintf(f, "chars");
			fprintf(f, " = ");
			emit_expr(f, node->right);
			fprintf(f, "\n");
			emit_indent(f, depth);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left);
			fprintf(f, ", %s)\n", tmp_name);
			free(tmp_name);
		}
		free(flat);
		return;
	}

	// Plain assign / compound-assign on existing variables.
	emit_expr(f, node->left);
	fprintf(f, " %s ", op_symbol(tok->type));
	emit_expr(f, node->right);
	fprintf(f, "\n");
}

static void emit_output_arg(File f, Node *arg) {
	Token *t = arg->token;
	if (t->type == STRUCT_CALL && t->Struct.ptr) {
		Node *out_fn = prep_find_struct_op(t->Struct.ptr, "output", NULL);
		if (!out_fn) {
			// Try generic search by name suffix.
			char *want = format("%s" OP_PREFIX "output", t->Struct.ptr->token->name);
			for (int i = 0; i < t->Struct.ptr->functions_count; i++) {
				Node *fn = t->Struct.ptr->functions[i];
				if (fn->token->name && strcmp(fn->token->name, want) == 0) {
					out_fn = fn;
					break;
				}
			}
			free(want);
		}
		if (out_fn) {
			char *flat = prep_fn_flat_name(out_fn->token->name);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, arg);
			fprintf(f, ")");
			free(flat);
			return;
		}
	}
	emit_expr(f, arg);
}

static void emit_node(File f, Node *node, int depth) {
	if (!node || !node->token) return;
	Token *tok = node->token;
	switch (tok->type) {
	case PROTO:
		if (node->left && node->left->token->type == FDEC)
			emit_node(f, node->left, depth);
		break;
	case FDEC:       emit_fdec(f, node, depth, NULL, NULL); break;
	case STRUCT_DEF:
		emit_struct_def(f, node, depth);
		emit_struct_methods(f, node);
		break;
	case STRUCT_CALL:
		if (tok->is_dec) {
			emit_indent(f, depth);
			emit_param_decl(f, tok);
			fprintf(f, "\n");
		}
		break;
	case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
	case DIV_ASSIGN:
	case MOD_ASSIGN: emit_assign_node(f, node, depth); break;
	case INT:        case LONG: case SHORT: case FLOAT: case BOOL: case CHAR:
	case CHARS:      case ARRAY_TYPE: case PTR:
		if (tok->is_dec) {
			emit_indent(f, depth);
			emit_param_decl(f, tok);
			fprintf(f, "\n");
		}
		break;
	case RETURN:
		emit_indent(f, depth);
		// Tuple return: children hold each returned value.
		if (node->children_count > 0) {
			fprintf(f, "return ");
			for (int i = 0; i < node->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->children[i]);
			}
			fprintf(f, "\n");
			break;
		}
		if (node->left && node->left->token && node->left->token->type == VOID) {
			fprintf(f, "return\n");
			break;
		}
		fprintf(f, "return ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	case TUPLE_UNPACK:
		emit_indent(f, depth);
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_param_decl(f, node->children[i]->token);
		}
		fprintf(f, " = ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	case IF: case ELIF:
		emit_indent(f, depth);
		fprintf(f, "%s ", tok->type == IF ? "if" : "elif");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		if (node->right) emit_node(f, node->right, depth);
		break;
	case ELSE:
		emit_indent(f, depth);
		fprintf(f, "else:\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	case WHILE:
		emit_indent(f, depth);
		fprintf(f, "while ");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	case FOR: {
		// children[0..2]: from, to, step ; children[3..]: body
		emit_indent(f, depth);
		fprintf(f, "for %s from ", node->left->token->name);
		if (node->children_count > 0) emit_expr(f, node->children[0]);
		fprintf(f, " to ");
		if (node->children_count > 1) emit_expr(f, node->children[1]);
		bool default_step = node->children_count > 2
		    && node->children[2]->token->type == INT
		    && node->children[2]->token->name == NULL
		    && node->children[2]->token->Int.value == 1;
		if (node->children_count > 2 && !default_step) {
			fprintf(f, " step ");
			emit_expr(f, node->children[2]);
		}
		fprintf(f, ":\n");
		for (int i = 3; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case BREAK:    emit_indent(f, depth); fprintf(f, "break\n"); break;
	case CONTINUE: emit_indent(f, depth); fprintf(f, "continue\n"); break;
	case ENUM_DEF:
		emit_indent(f, depth);
		fprintf(f, "enum %s:\n", tok->name);
		emit_indent(f, depth + 1);
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			fprintf(f, "%s", node->children[i]->token->name);
		}
		fprintf(f, "\n");
		break;
	case MODULE:
		// Modules dissolve into flat top-level functions named `<mod>_<fn>`,
		// matching the call-site mangling in emit_expr_call. The `mod`
		// keyword itself disappears in the prep.
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth);
		for (int i = 0; i < node->functions_count; i++)
			emit_fdec(f, node->functions[i], depth, NULL, NULL);
		break;
	case FCALL:
		emit_indent(f, depth);
		emit_expr(f, node);
		fprintf(f, "\n");
		break;
	case OUTPUT:
		emit_indent(f, depth);
		fprintf(f, "output(");
		if (node->left) {
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_output_arg(f, node->left->children[i]);
			}
		}
		fprintf(f, ")\n");
		break;
	default:
		emit_indent(f, depth);
		fprintf(f, "?\n");
		break;
	}
}

void emit_prep_file(Node *scope_node, char *path) {
	File f = fopen(path, "w");
	if (CHECK(!f, "cannot open %s for writing", path)) return;
	for (int i = 0; i < scope_node->children_count; i++)
		emit_node(f, scope_node->children[i], 0);
	fclose(f);
}
