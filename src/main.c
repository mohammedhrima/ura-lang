// ============================================================
// HEADERS
// ============================================================
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

// ============================================================
// MACROS
// ============================================================
#define LINE __LINE__
#define FUNC (char *)__func__

#if defined(__APPLE__)
typedef struct __sFILE *File;
#define FILE strrchr(__FILE__, '/') + 1
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#define FILE strrchr(__FILE__, '/') + 1
#endif

#define TOKENIZE 1
#define AST 0
#define IR 0
#define OPTIMIZE 0
#define ASM 0
#define TAB 3

#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#define debug(fmt, ...) _debug(fmt, ## __VA_ARGS__)
#else
#define debug(fmt, ...) do { } while (0)
#endif

#define SPLIT "=================================================\n"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED(fmt) BOLD "\033[0;31m" fmt RESET
#define GREEN(fmt) "\033[0;32m" fmt RESET
#define BLUE(fmt) BOLD "\033[0;34m" fmt RESET
#define CYAN(fmt) BOLD "\033[0;36m" fmt RESET

#define allocate(count, size) _allocate(FILE, LINE, count, size)
#define check(cond, fmt, ...) _check(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)
#define todo(cond, fmt, ...) if (_check(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)) exit(1)
#define seg() raise(SIGSEGV)

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, SHORT, ARRAY_TYPE
#define LOGIC_TYPES AND, OR
#define MATH_TYPES ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define NUMERIC_TYPES INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN

#define PASSES_O0 "default<O0>" /* debug mode */
#define PASSES_O1 "default<O1>" /* memory to register */
#define PASSES_O2 "default<O2>" /* loop unrolling, inline operations */
#define PASSES_O3 "default<O3>" /* O2++ */
#define PASSES_Os "default<Os>" /* reduce build size */
#define PASSES_Oz "default<Oz>" /* Os++ */

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

#define resize_array(array, type, size, count){     \
		if(array == NULL) {                               \
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
// ============================================================
// TYPEDEFS
// ============================================================
typedef enum Type Type;
typedef struct Types Types;
typedef struct Token Token;
typedef struct Node Node;
typedef struct LLVM LLVM;
typedef enum LogType LogType;

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Block;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTypeKind TypeKind;
typedef LLVMDIBuilderRef DIBuilder;
typedef LLVMMetadataRef Metadata;

#define PointerType LLVMPointerTypeKind
#define IntegerType LLVMIntegerTypeKind
#define VoidType    LLVMVoidTypeKind
#define FunctionType LLVMFunctionTypeKind
#define StructType  LLVMStructTypeKind

// ============================================================
// STRUCTS / ENUMS
// ============================================================

enum Type
{
	ID = 1, // Identifier
	VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, VARIADIC, REF, ARRAY, ARRAY_TYPE, // Data types
	STRUCT_DEF, STRUCT_CALL, // Structures
	ENUM_DEF, ENUM_CALL, // Enums
	TUPLE, TUPLE_UNPACK, // Tuples
	ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, // Assignment
	EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREAT, GREAT_EQUAL, // Comparison
	ADD, SUB, MUL, DIV, MOD, // Arithmetic
	AND, OR, NOT, // Logical
	LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS, AS, // Punctuation and Syntax
	RETURN, IF, ELIF, ELSE, WHILE, CONTINUE, BREAK, // Control Flow
	FDEC, FCALL, PROTO, ARGS, // Functions
	STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, DEFAULT, SYNTAX_ERROR, // Built-ins
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT, // Bitwise
	NULLABLE, // Literals
	END, // Tokens terminator
};

struct Types
{
	char *name;
	Type type;
};
struct Token
{
	Type type;
	char *filename;
	int line;

	Type retType;
	char *name;
	int space;
	int used;

	bool is_ref;
	bool is_dec;
	bool is_proto;
	bool is_global;
	bool is_param;
	bool is_variadic;
	bool is_init;
	bool is_clean;
	bool is_method_call;
	bool ir_bound;
	bool asm_bound;

	struct
	{
		bool is_set;
		bool is_loaded;
		Value array_size;
		Value elem;

		Value *dims;
		int dcount;
		int dsize;

		Block bloc;
		TypeRef funcType;
		TypeRef stType;

		// try / catch
		// Value error_flag;
		// Value error_value;
		// Block Catch;
		// Block lpad;

		// statements / loops
		Block start;
		Block then;
		Block end;
	} llvm;

	struct // TODO: replace struct with union
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
		struct { Token **types; int tsize; int tcount; } Tuple;
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
	int variabales_size;

	Node **functions; // functions / methods for struct
	int functions_count;
	int functions_size;

	Node **structs;
	int structs_count;
	int structs_size;
};

// ============================================================
// FUNCTIONS PROTOTYPES
// ============================================================
// ----------------------------------------------------------------------------
// File Management
// ----------------------------------------------------------------------------
char *resolve_path(char *path);
void  add_link_key(char *key);
// ----------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------
void   free_token(Token *token);
void   free_node(Node *node);
Token *copy_token(Token *token);
Node  *copy_node(Node *node);
char *strjoin(char *str0, char *str1, char *str2);
// ----------------------------------------------------------------------------
// Debugin / Error Handling
// ----------------------------------------------------------------------------
int  _debug(char *conv, ...);
void pnode(Node *node, char *indent);
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
Node *syntax_error();
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
Value   allocate_stack(Value size, TypeRef elementType, char *name);
Value   allocate_heap(Value count, TypeRef elementType, char *name);
void   set_ret_type(Node *node);

// ============================================================
// GLOBALS
// ============================================================
bool found_error;

Token **tokens;
int tcount;              // tokens count
int tsize;               // tokens size
int ecount;              // execution index

Node   *ura_scope;            // tree origin parent
Node   *scope;                // current scope
Node  **level_scope;          // array of scopes
int sccount;                  // scope count
int scsize;                   // scope size

char  **used_files;           // imported files
int ucount;                   // files count
int usize;                    // files size

char   *argv0;
char  **links;
int lcount;
int lsize;
char  **src_files;
int src_count;
int src_size;

// llvm
Context context;
Module module;
Builder builder;
DIBuilder debug_builder;
Metadata debug_compile_unit;
Metadata debug_file;
Metadata debug_scope;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
char     *op_flags;          // optimization flags

int calling_use;               // tokenizing an imported file
bool enable_san;      // sanitizer
char     *ura_lib;

// ============================================================
// UTILS / DEBUGING / LOGS
// ============================================================
void *_allocate(char *filename, int line, int count, int size)
{
	void *res = calloc(count, size);
	check(!res, "allocate did failed in %s:%d", filename, line);
	return res;
}

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

void free_token(Token *token)
{
	free(token->name);
	free(token->llvm.dims);
	free(token->Chars.value);
	free(token->Tuple.types);
	free(token);
}

void free_node(Node* node)
{
	if (!node) return;
	for (int i = 0; i < node->children_count; i++) free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->variables);
	free(node->functions);
	free(node->structs);
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

void free_global_memory()
{
	for (int i = 0; links && links[i]; i++)
	{
		free(links[i]);
		links[i] = NULL;
	}
	lcount = 0;
}

int vprint_(File out, char *conv, va_list args)
{
	int res = 0;

	for (int i = 0; conv[i]; i++)
	{
		if (conv[i] != '%') { res += fprintf(out, "%c", conv[i]); continue; }
		i++;
		int left_align = 0;
		if (conv[i] == '-') { left_align = 1; i++; }
		int width      = 0;
		while (isdigit(conv[i])) { width = width * 10 + (conv[i] - '0'); i++; }
		int prec       = -1;
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
						fprintf(out, "[");
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
						fprintf(out, "] ");
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

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...)
{
	if (!cond) return cond;
	found_error = true;
	fprintf(stderr, RED("ura_error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	vprint_(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

int _debug(char *conv, ...)
{
	va_list args;
	va_start(args, conv);
	int res = vprint_(stdout, conv, args);
	va_end(args);
	return res;
}

void pnode(Node *node, char *indent)
{
#if AST
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
		for (int i = 0; i < node->ccount; i++) push(node->children[i]);
		if (node->right) push(node->right);
	}
	else
	{
		if (node->left)  push(node->left);
		if (node->right) push(node->right);
		for (int i = 0; i < node->ccount; i++) push(node->children[i]);
		if (node->token->type == STRUCT_DEF)
		{
			for (int i = 0; i < node->functions_count; i++)
				push(node->functions[i]);
		}
	}
	for (int i = 0; i < count; i++)
	{
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int is_last   = (i == count - 1);
		const char *connector = is_last ? "└──" : "├──";
		const char *bar       = is_last ? "   " : "│  ";

		char new_indent[4096];
		snprintf(new_indent, sizeof(new_indent), "%s%s", indent, bar);

		debug("%s%s", indent, connector);
		pnode(child, new_indent);
	}
	free(subs);
#undef push
#endif
}

void setName(Token *token, char *name)
{
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

bool includes(Type to_find, ...)
{
	if (found_error) return false;
	va_list ap; Type current; va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
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

// ============================================================
// PARSING
// ============================================================
// ----------------------------------------------------------------------------
// Tokenizer
// ----------------------------------------------------------------------------
static bool match_table(Token *new, char *name, Types *table, bool is_dec)
{
	for (int i = 0; table[i].name; i++)
	{
		if (strcmp(table[i].name, name) == 0)
		{
			new->type   = table[i].type;
			new->is_dec = is_dec;
			if (is_dec) setName(new, NULL);
			return true;
		}
	}
	return false;
}

static int parse_escape_seq(char *input, int s, int e, char *buf, int *j)
{
	switch (input[s + 1])
	{
	case 'n':  buf[(*j)++] = '\n'; return s + 1;
	case 't':  buf[(*j)++] = '\t'; return s + 1;
	case 'r':  buf[(*j)++] = '\r'; return s + 1;
	case 'b':  buf[(*j)++] = '\b'; return s + 1;
	case 'f':  buf[(*j)++] = '\f'; return s + 1;
	case 'v':  buf[(*j)++] = '\v'; return s + 1;
	case 'a':  buf[(*j)++] = '\a'; return s + 1;
	case '\\': buf[(*j)++] = '\\'; return s + 1;
	case '"':  buf[(*j)++] = '"';  return s + 1;
	case '\'': buf[(*j)++] = '\''; return s + 1;
	case '?':  buf[(*j)++] = '\?'; return s + 1;
	case '0':
	{
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
		{
			int oct = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3]- '0');
			if (oct <= 255) { buf[(*j)++] = (char)oct; return s + 3; }
			buf[(*j)++] = '\0';
			return s + 1;
		}
		if (s + 1 < e && isdigit(input[s + 2]))
		{
			buf[(*j)++] = (char)((input[s + 1] - '0') * 8 + (input[s + 2] - '0'));
			return s + 2;
		}
		buf[(*j)++] = '\0';
		return s + 1;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7':
	{
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
		{
			int oct = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (oct <= 255) { buf[(*j)++] = (char)oct; return s + 3; }
			buf[(*j)++] = input[s];
			return s + 1;
		}
		if (s + 2 < e && isdigit(input[s + 2]))
		{
			buf[(*j)++] = (char)((input[s + 1] - '0') * 8 + (input[s + 2] - '0'));
			return s + 2;
		}
		buf[(*j)++] = (char)(input[s + 1] - '0');
		return s + 1;
	}
	case 'x':
	{
		if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
		{
			int hex = 0;
			char c1 = input[s + 2], c2 = input[s + 3];
			hex += (c1 >= '0'&& c1 <= '9') ? (c1 - '0') * 16 : (tolower(c1) - 'a' + 10) * 16;
			hex += (c2 >= '0'&& c2 <= '9') ? (c2 - '0') : (tolower(c2) - 'a' + 10);
			buf[(*j)++] = (char)hex;
			return s + 3;
		}
		buf[(*j)++] = input[s];
		return s + 1;
	}
	default: 
   {
		buf[(*j)++] = input[s];
		return s + 1;
	}
	}
}

inline void add_token(Token *token)
{
	resize_array(tokens, Token*, tsize, tcount);
	tokens[tcount++] = token;
}

Token *new_token(Type type, int space)
{
	Token* token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space)
{
	Token *new = new_token(type, space);
	new->line     = line;
	new->filename = filename;

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
		int len     = e - s > 63 ? 63 : e - s;
		strncpy(buf, input + s, len);
		new->Float.value = (float)atof(buf);
		break;
	}
	case ID:
	{
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);

		struct { char *name; bool value; } bools[] = {
			{"True", true}, {"False", false}, {0, 0}
		};
		for (int i = 0; bools[i].name; i++)
			if (strcmp(new->name, bools[i].name) == 0)
			{
				setName(new, NULL);
				new->type       = BOOL;
				new->Bool.value = bools[i].value;
				goto id_done;
			}

		Types data_types[] = {
			{"int", INT}, {"bool", BOOL}, {"chars", CHARS}, {"char", CHAR},
			{"float", FLOAT}, {"void", VOID}, {"long", LONG}, {"pointer", CHARS},
			{"short", SHORT}, {0, 0}
		};
		if (match_table(new, new->name, (Types*)data_types, true)) goto id_done;

		Types keywords[] = {
			{"if", IF}, {"elif", ELIF}, {"else", ELSE}, {"while", WHILE},
			{"fn", FDEC}, {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
			{"ref", REF}, {"struct", STRUCT_DEF}, {"enum", ENUM_DEF},
			{"proto", PROTO}, {"as", AS}, {0, 0}
		};
		if (match_table(new, new->name, (Types*)keywords, false)) goto id_done;

		Types keywords2[] = {
			{"and", AND}, {"or", OR}, {"is", EQUAL}, {"not", NOT},
			{"typeof", TYPEOF}, {"sizeof", SIZEOF}, {"output", OUTPUT},
			{"stack", STACK}, {"heap", HEAP}, {"array", ARRAY_TYPE},
			{"null", NULLABLE}, {0, 0}
		};
		if (match_table(new, new->name, (Types*)keywords2, false)) setName(new, NULL);

id_done:        break;
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
		new->Chars.value = allocate(e - s + 2, sizeof(char));
		int j = 0;
		for (; s < e; s++)
		{
			if (s + 1 < e && input[s] == '\\')
				s = parse_escape_seq(input, s, e, new->Chars.value, &j);
			else
				new->Chars.value[j++] = input[s];
		}
		break;
	}
	case CHAR:
	{
		if (e == s + 1) { new->Char.value = input[s]; break; }
		char escape_map[256] = {
			['n'] ='\n',['t']='\t',['r']='\r',['b']='\b',
			['f'] ='\f',['v']='\v',['a']='\a',['0']='\0',
			['\\']='\\',['"']='\"',['\'']='\'',['?']='\?',
		};
		char c = escape_map[(unsigned char)input[s + 1]];
		check(!c && input[s + 1] != '0', "unknown escape character");
		new->Char.value = c;
		break;
	}
	default: break;
	}
	return new;
}

char *open_file(char* file_name)
{
	if (found_error) return NULL;
	char *filename = realpath(file_name, NULL);
	File file     = fopen(filename, "r");
	char *input    = NULL;
	if (!check(!file, "openning %s", filename))
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fseek(file, 0, SEEK_SET);
		input = allocate((size + 1), sizeof(char));
		fread(input, size, sizeof(char), file);
		fclose(file);
	}
	free(filename);
	return input;
}

void tokenize(char *file_name)
{
	if(found_error) return;
	char *filename = realpath(file_name, NULL);
	File file     = fopen(filename, "r");
	char *input    = NULL;
	if (check(!file, "openning %s", filename))
		return;
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	input = allocate((size + 1), sizeof(char));
	fread(input, size, sizeof(char), file);
	fclose(file);

	debug(GREEN("TOKENIZE: [%s]\n"), file_name);

	struct { char *value; Type type; } specials[] =
	{
		{"...", VARIADIC}, {".",  DOT}, {":", DOTS}, {"+=", ADD_ASSIGN},
		{"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN},
		{"!=", NOT_EQUAL}, {"!", NOT}, {"==", EQUAL}, {"<<", LSHIFT},
		{">>", RSHIFT}, {"<=", LESS_EQUAL}, {">=", GREAT_EQUAL},
		{"<", LESS}, {">", GREAT}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
		{"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
		{"[", LBRA}, {"]", RBRA}, {", ", COMA}, {"&&", AND}, {"||", OR},
		{"&", BAND}, {"|", BOR}, {"^", BXOR}, {"~", BNOT}, {0, (Type)0},
	};

	int space    = 0;
	int line     = 1;
	bool new_line = true;

	resize_array(used_files, char*, usize, ucount);
	for (int i = 0; i < ucount; i++)
	{
		if (strcmp(file_name, used_files[i]) == 0)
			return;
	}
	used_files[ucount++] = file_name;

	for (int i = 0; input[i] && !found_error; )
	{
		int s = i;
		char c = input[i];

		if (isspace(c))
		{
			if (c == '\n') { line++; new_line = true; space = 0; }
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
		{ while (input[i] && input[i] != '\n') i++; continue; }

		new_line = false;
		if (c == '\"')
		{
			i++;
			while (input[i] && input[i] != '\"')
			{
				if (input[i] == '\\') i++;
				i++;
			}
			check(input[i++] != '\"', "expected '\"'");
			parse_token(file_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'')
		{
			i++;
			if (input[i] == '\\') i++;
			if (input[i] && input[i] != '\'') i++;
			check(input[i++] != '\'', "expected '\''");
			parse_token(file_name, line, input, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c))
		{
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i]))) i++;
			if (i - s == 3 && strncmp(input + s, "use",  3) == 0 && isspace(input[i]))
			{
				while (isspace(input[i])) i++;
				check(input[i++] != '\"', "expected \" after use");
				int s = i;
				while (input[i] && input[i] != '\"') i++;
				check(input[i++] != '\"', "expected \" after use <filename>");

				char *use = strndup(input + s, i - s - 1);
				char *tmp = strjoin(use, ".ura", NULL);
				free(use);
				use = tmp;
				if (use[0] == '@') // module from ura-lib
				{
					tmp = strjoin(ura_lib, "/", use + 1); // skip @
					free(use);
					use = tmp;
				}
				else if (use[0] != '/')
				{
					char *dir_name = dirname(file_name);
					tmp = strjoin(dir_name, "/", use);
					free(use);
					use = tmp;
				}
				calling_use++;
				tokenize(use);
				calling_use--;
			}
			else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i]))
			{
				while (isspace(input[i])) i++;
				check(input[i++] != '\"', "expected \" after link");
				int s = i;
				while (input[i] && input[i] != '\"') i++;
				check(input[i++] != '\"', "expected \" after link <filename>");
				char *raw = strndup(input + s, i - s);
				for (int i = 0; i < lcount; i++)
					if (strcmp(links[i], raw) == 0)
						goto _continue;
				resize_array(links, char*, lsize, lcount);
				links[lcount++] = raw;
			}
			else
				parse_token(file_name, line, input, s, i, ID, space);
_continue:
			continue;
		}
		if (isdigit(c))
		{
			while (isdigit(input[i])) i++;
			if (input[i] == '.' && isdigit(input[i + 1]))
			{
				i++;
				while (isdigit(input[i])) i++;
				parse_token(file_name, line, input, s, i, FLOAT, space);
			}
			else
				parse_token(file_name, line, input, s, i, INT, space);
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++)
		{
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0)
			{
				parse_token(file_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true; break;
			}
		}
		if (found) continue;
		check(1, "Syntax error <%c>", c);
	}
	if (!calling_use) new_token(END, -1);
	free(input);
}

// ------------------------------------------------------------
// ABSTRACT SYNTAX TREE
// ------------------------------------------------------------
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

Node *as_node() // as (casting)
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
		Node *node  = new_node(token);
		Node *left  = node->left  = unary_node();
		Node *right = node->right = new_node(new_token(INT, token->space));
		right->token->filename  = token->filename;
		right->token->line      = token->line;
		right->token->Int.value = token->type == SUB ? -1 : 1;
		node->token->type       = MUL;
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


// ============================================================
// GENERATE INTERMEDIATE REPRESENTATION
// ============================================================

// ============================================================
// GENERATE ASSEMBLY
// ============================================================

// ============================================================
// START POINT
// ============================================================
void compile(char *path_name)
{
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));

	// reset ura_scope state for each file
	found_error = false;
	tcount      = 0; ecount = 0;
	sccount     = 0; scope = NULL;
	ura_scope   = NULL;


	char* file_name = realpath(path_name, NULL);
	if (check(file_name == NULL, "error resolving %s\n", path_name)) return;
	tokenize(file_name);

	for(int i = 0; i < tokens[i]; i++) debug(GREEN("%k\n"), tokens[i]);

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
	for (int i = 0; !found_error && i < ura_scope->ccount; i++)
		pnode(ura_scope->children[i], "");
	if (found_error) return NULL;
#endif

#if IR
	if (found_error) return NULL;
	for (int i = 0; !found_error && i < ura_scope->ccount; i++)
		gen_ir(ura_scope->children[i]);
	if (found_error) return NULL;
	debug("%s===========================================\n", GREEN);
	debug("AFTER IR\n");
	debug("===========================================\n%s",RESET);
	for (int i = 0; !found_error && i < ura_scope->ccount; i++)
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
	for (int i = 0; op && !found_error && i < ura_scope->ccount; i++)
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

	for (int i = 0; !found_error && i < ura_scope->ccount; i++)
		gen_asm(ura_scope->children[i]);

	finalize(ll_path);
	free(build_dir);
#endif
}

int main(int argc, char **argv)
{
	ura_lib = getenv("URA_LIB");
	if (check(!ura_lib, "URA_LIB not set")) return 1;
	argv0   = argv[0];
	if(check(argc < 2, "usage: ura <file.ura> [file2.ura ...] [-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
		return 1;

	char *output       = "exe.out";
	bool testing_mode = false;

	for (int i = 1; i < argc; i++)
	{
		if      (strcmp(argv[i], "-O0")     == 0) op_flags = PASSES_O0;
		else if (strcmp(argv[i], "-O1")     == 0) op_flags = PASSES_O1;
		else if (strcmp(argv[i], "-O2")     == 0) op_flags = PASSES_O2;
		else if (strcmp(argv[i], "-O3")     == 0) op_flags = PASSES_O3;
		else if (strcmp(argv[i], "-Os")     == 0) op_flags = PASSES_Os;
		else if (strcmp(argv[i], "-Oz")     == 0) op_flags = PASSES_Oz;
		else if (strcmp(argv[i], "-san")    == 0) enable_san = true;
		else if (strcmp(argv[i], "-testing") == 0) testing_mode = true;
		else if (strcmp(argv[i], "-o")      == 0)
		{
			check(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		}
		else if (argv[i][0] != '-' && strlen(argv[i]) > 4 && strcmp(argv[i] + strlen(argv[i]) - 4, ".ura") == 0)
		{
			resize_array(src_files, char*, src_size, src_count);
			src_files[src_count++] = argv[i];
		}
		else
		{
			check(argv[i][0] == '-', "unknown flag: %s\n", argv[i]);
			check(argv[i][0] != '-', "unvalid file: %s\n", argv[i]);
		}
	}
#if ASM
	char final_cmd[99999999999999];
	int pos = 0;

	pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
	if (enable_san)
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -fsanitize=address,undefined -fno-omit-frame-pointer -g");
#endif

	for (int i = 0; i < src_count && !found_error; i++)
	{
		compile(src_files[i]);
#if ASM
		if(!testing_mode)
		{
			if (enable_san)
				pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
			else
			{
				char s[SIZE_MAX];
				snprintf(s, sizeof(s), "%s", ll);
				char *dot = strrchr(s, '.');
				if (dot) strcpy(dot, ".s");

				char llc_cmd[SIZE_MAX];
				snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
				if (check(system(llc_cmd) != 0, "llc failed for %s\n", ll))
				{
					link_ok = false;
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
	for (int i = 0; i < lcount; i++)
	{
		char env_name[SIZE_MAX];
		snprintf(env_name, sizeof(env_name), "URA_LINK_%s", links[i]);
		char *flags = getenv(env_name);
		if (flags)
			pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
	}

	if (!testing_mode && link_ok)
	{
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
		if (!check(system(final_cmd) != 0, "linking failed\n"))
		{
			char full[SIZE_MAX];
			realpath(output, full);

			if (enable_san)
			{
				char *asan_file = getenv("ASAN_FILE");
				if (asan_file)
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1),
					setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
				else
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
			}
		}
	}
#endif
	free(src_files);
	free_global_memory();
	return found_error;
}