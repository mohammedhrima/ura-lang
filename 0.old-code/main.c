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

#define ERR_MISSING_O_ARG "Missing argument for '-o'"

#define ERR_NO_INPUT \
	"No input file (usage: ura <file.ura> [-o out] [-O0..-Oz]" \
	" [-san] [-debug] [-tree])"

#define ERR_CASE_TYPE_MISMATCH \
	"This case value is %s but the subject is %s;" \
	" they must be the same type"

#define ERR_ARG_TYPE_MISMATCH \
	"Argument %d type mismatch in call to '%s'"

#define ERR_ARG_NEEDS_REF \
	"Argument %d to '%s' must be passed by reference (ref x)"

#define ERR_ARG_NO_REF \
	"Argument %d to '%s' does not take a reference"

#define ERR_REF_TYPE_MISMATCH \
	"Reference type mismatch: expected %s, got %s"

#define ERR_BITWISE_NEEDS_INT \
	"Bitwise and shift operators require integer operands"

#define ERR_NOT_AN_ARRAY \
	"Cannot index '%s', it is not an array"

#define ERR_INDEX_NOT_INT \
	"Array index must be an integer, got %s"

#define ERR_BY_NEEDS_RANGE \
	"'by' sets the step of a range (a..b), so it needs one on its left"

#define ERR_BY_NOT_INT \
	"The 'by' step must be an integer, got %s"

#define ERR_BY_NOT_POSITIVE \
	"The 'by' step must be positive;" \
	" a range counts down when its start is greater than its end"

#define ERR_LITERAL_OUT_OF_RANGE \
	"%lld does not fit in '%s' (range %lld to %lld);" \
	" widen the type, or convert explicitly with 'as'"

#define ERR_FOR_NOT_ITERABLE \
	"'for %s in ...' expects a range (a..b) or an array"

#define ERR_FOR_REF_NEEDS_ARRAY \
	"'for ref' needs an array; a range yields values, not storage"

#define ERR_RETURN_TYPE_MISMATCH \
	"'%s' returns %s, but this returns %s"

#define ERR_RETURN_NEEDS_REF \
	"'%s' returns a reference; return `ref x` (or null), not a value"

#define ERR_RETURN_NO_REF \
	"'%s' returns a value, not a reference; drop the `ref`"

#define ERR_CANNOT_CAST "Cannot cast %s to %s"

#define ERR_LEN_NOT_ARRAY \
	"'.len' is only valid on an array, not %s"

#define ERR_UNTERM_BLOCK_COMMENT \
	"Unterminated block comment, expected '*/'"

#define ERR_UNTERM_STRING_LITERAL \
	"Unterminated string literal, expected '\"'"

#define ERR_UNTERM_CHAR_LITERAL \
	"Unterminated character literal, expected \"'\""

#define ERR_MISSING_LIB_FN \
	"'%s' is not in scope; the standard library failed to load" \
	" (it is declared in ura-lib/common.ura)"

#define ERR_NO_STDLIB \
	"Cannot find the ura standard library;" \
	" set URA_LIB or put ura-lib next to the ura binary"

#define ERR_NO_URA_LIB \
	"No ura standard library found for 'use \"@/...\"'\n" \
	"  tried: %s\n" \
	"         %s\n" \
	"         %s\n" \
	"  set URA_LIB, or put ura-lib next to the ura binary"

#define ERR_UNTERM_USE_PATH \
	"Unterminated 'use' path, expected closing '\"'"

#define ERR_UNTERM_LINK_PATH \
	"Unterminated 'link' path, expected closing '\"'"

#define ERR_UNKNOWN_ESCAPE "Unknown escape character: \\%c"

#define ERR_FN_EXPECTED_LPAREN "Expected '(' after function %s"

#define ERR_FN_EXPECTED_RPAREN "Expected ')' after function %s"

#define ERR_FN_EXPECTED_COLON "Expected ':' after function %s"

#define ERR_FN_EXPECTED_PARAM_NAME \
	"Expected parameter name in function %s"

#define ERR_FN_EXPECTED_RET_TYPE \
	"Expected <data type> after function %s"

#define ERR_EXPECTED_BODY_COLON "Expected ':' to open the '%s' body"

#define ERR_CALL_EXPECTED_RPAREN "Expected ')' after %s arguments"

#define ERR_STRUCT_EXPECTED_COLON "Expected ':' after struct %s"

#define ERR_UNKNOWN_TYPE "Unknown type '%s'"

#define ERR_STRUCT_RECURSIVE \
	"Struct %s cannot contain itself by value;" \
	" use an array or a 'ref?' so the size stays finite"

#define ERR_STRUCT_EMPTY \
	"Struct %s must declare at least one field"

#define ERR_STRUCT_DUP_FIELD \
	"Field '%s' is already declared in this struct;" \
	" rename it or remove the duplicate"

#define ERR_ENUM_EXPECTED_COLON "Expected ':' after enum %s"

#define ERR_ENUM_EMPTY \
	"Enum %s must declare at least one variant"

#define ERR_ENUM_VALUE_INT \
	"Enum variant '%s' needs an integer value after '='"

#define ERR_REDECL_VARIABLE "Redeclaration of variable '%s'"

#define ERR_REF_MUST_BE_BOUND \
	"A reference must be bound when declared" \
	" (use 'ref?' for an optional reference)"

#define ERR_NEW_EXPECTED_ARRAY_TYPE \
	"Expected an array type after 'new' (e.g. new int[n])"

#define ERR_DROP_NO_PARAMS \
	"Destructor 'drop' takes no parameters; write 'operator drop:'"

#define ERR_DROP_NO_RET \
	"Destructor 'drop' returns void; drop the return type"

#define ERR_ASSIGN_OPTIONAL \
	"'%s' may be null; unwrap it with '?? <default>'" \
	" or declare '%s' as optional"

#define ERR_FALLBACK_RHS \
	"The right side of '?\?' may itself be null; it must not be"

#define ERR_FALLBACK_LHS \
	"The left side of '?\?' is never null, so '?\?' does nothing"

#define ERR_NULL_NO_TARGET \
	"'%s' can never be null; declare it optional to assign null to it"

#define ERR_COMPARE_NON_NULL \
	"'%s' is never null, so this comparison is always the same"

#define ERR_NULL_ON_VALUE \
	"Only arrays and pointers can be null, not %s"

#define ERR_REDECL_FUNCTION "Redeclaration of function '%s'"

#define ERR_SIG_CONFLICT \
	"Conflicting declaration of '%s': %s"

#define NOTE_PREV_DECL "Previous declaration of '%s' is here"

#define ERR_UNDECLARED_VARIABLE "Undeclared variable '%s'"

#define ERR_BARE_FIELD \
	"Field '%s' must be accessed through self; write 'self.%s'"

#define ERR_REF_TO_NON_VARIABLE \
	"Cannot take a reference to a non-variable"

#define ERR_CAPTURE_NOT_ALLOWED \
	"Cannot use '%s' from an enclosing function" \
	" - pass it as a parameter"

#define ERR_WRONG_ARG_COUNT "Wrong number of arguments to '%s'"

#define ERR_REF_NEEDS_VARIABLE \
	"A reference must be bound to a variable (ref x)"

#define ERR_REF_FIELD_WRITE \
	"'%s' is a reference field; bind it with '= ref', not a value"

#define ERR_THROW_NEEDS_ERROR "'throw' expects an Error, got %s"

#define ERR_MOD_EXPECTED_COLON \
	"Expected ':' to open the body of module '%s'"

#define ERR_MOD_EMPTY \
	"Module '%s' is empty; add a declaration or remove it"

#define ERR_MOD_BODY \
	"Only 'fn', 'struct', 'enum' and 'mod' can be declared in a module"

#define ERR_UNKNOWN_MODULE "Unknown module '%s'"

#define ERR_MOD_NO_MEMBER "Module '%s' has no member '%s'"

#define ERR_MOD_DOT_CALL \
	"'%s' is a module; reach its members with '::' instead of '.'"

#define ERR_ARRAY_SIZE_NOT_INT "Array size must be an integer"

#define ERR_ARRAY_SIZE_IN_TYPE \
	"An array type carries no size;" \
	" ex. 'arr int[] = int[3]' to make one"

#define ERR_UNKNOWN_MEMBER "Unknown member '.%s'"

#define ERR_UNKNOWN_FIELD \
	"Struct %s has no field '%s';" \
	" check the spelling or declare it in the struct"

#define ERR_UNKNOWN_METHOD \
	"Struct %s has no method '%s';" \
	" check the spelling or declare it in the struct"

#define ERR_CANNOT_ASSIGN "Cannot assign %s to %s"

#define ERR_BINOP_TYPE_MISMATCH \
	"Cannot use '%s' with %s and %s"

#define ERR_NO_OPERATOR \
	"Struct %s has no 'operator %s' taking %s;" \
	" declare one inside the struct"

#define ERR_ARG_TYPE_MISMATCH_STRUCT \
	"Cannot assign %s to %s; they are different structs"

#define ERR_OPERATOR_OUTSIDE_STRUCT \
	"'operator' may only be declared inside a struct"

#define ERR_PUB_OUTSIDE_STRUCT \
	"'pub' marks a static method; it may only be used inside a struct"

#define ERR_OPERATOR_EXPECTED \
	"Expected an operator after 'operator'" \
	" (one of + - * / %% & | ^ << >> == != < > <= >= =" \
	" += -= *= /= %%= &= |= ^= <<= >>= drop)"

#define ERR_METHOD_NEEDS_RECEIVER \
	"'%s::%s' needs a receiver;" \
	" call it on a value, or declare it 'pub fn' to make it static"

#define ERR_PUB_IS_STATIC \
	"'%s.%s' is a 'pub fn', so it is static and takes no receiver;" \
	" call it as '%s::%s()'"

#define ERR_METHOD_ON_NON_STRUCT \
	"Cannot call '.%s()' on %s; only a struct has methods"

#define ERR_TYPE_HAS_NO_FIELDS \
	"Cannot read '.%s' from %s;" \
	" only a struct has fields"

#define WARN_DROP_NEEDS_ASSIGN \
	"Struct %s has 'operator drop' but no 'operator =';" \
	" copying it makes two owners of the same memory," \
	" and both will be destroyed"

#define ERR_CANNOT_CALL_DROP \
	"Cannot call '%s.drop()' yourself;" \
	" the compiler runs 'operator drop' when the value leaves scope"

#define ERR_CLEAN_NEEDS_ARRAY \
	"'clean' frees a heap array, not %s"

#define ERR_CLEAN_ON_STRUCT \
	"'clean' frees a heap array, not the struct %s;" \
	" a struct is destroyed at end of scope by its 'operator drop'"

#define ASSERT_TYPECHECK_NODE "type_check: unhandled node '%s'"

#define ASSERT_CODEGEN_NODE "code_gen: unhandled node '%s'"


#if defined(__APPLE__)
#include <mach-o/dyld.h>
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
#define YELLOW(fmt) BOLD "\033[0;33m" fmt RESET

#define CARET_ERR  "\033[1;31m"
#define CARET_WARN "\033[1;33m"

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

#define SIGNED_TYPES   I8, I16, I32, I64
#define UNSIGNED_TYPES U8, U16, U32, U64
#define DATA_TYPES     SIGNED_TYPES, UNSIGNED_TYPES, F32, F64, BOOL, CHAR, \
                       CHARS, VOID, PTR, ARRAY_TYPE, LIST_TYPE, FN_TYPE
#define POINTER_TYPES  PTR, ARRAY_TYPE
#define LOGIC_TYPE     AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define BITWISE_TYPE   BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS

#define NUMERIC_TYPES  SIGNED_TYPES, UNSIGNED_TYPES, BOOL, CHAR
#define FLOAT_TYPES    F32, F64
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, \
                       MOD_ASSIGN, BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN, \
                       LSHIFT_ASSIGN, RSHIFT_ASSIGN

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
typedef LLVMTargetRef     Target;
typedef LLVMTargetMachineRef TargetMachine;
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
	VOID, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64,
	BOOL, CHAR, CHARS, PTR, VARIADIC, REF,
	ARRAY, ARRAY_TYPE, ARRAY_LIT, LIST, LIST_TYPE,
	FN_TYPE,
	STRUCT_DEF, STRUCT_CALL, // Structures
	ENUM_DEF, ENUM_CALL, // Enums
	TUPLE, TUPLE_UNPACK, // Tuples
	ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
	BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN,
	EQUAL, NOT_EQUAL, LESS_EQUAL, GREAT, GREAT_EQUAL, LESS, // Comparison
	ADD, SUB, MUL, DIV, MOD, // Arithmetic
	AND, OR, NOT, // Logical
	LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, RANGE, ACCESS, AS, // Punctuation and Syntax
	RETURN, IF, ELIF, ELSE, WHILE, CONTINUE, BREAK, MATCH, CASE, DEFAULT, // Control Flow
	FOR, BY, IN, LOOP,
	TRY, CATCH, THROW,
	AT_IF, AT_ELIF, AT_ELSE, AT_NO_WARN,
	FDEC, FCALL, PROTO, ARGS, CHILDREN, // Functions
	TYPEOF, SIZEOF, OUTPUT, ERRPUT, SYNTAX_ERROR, // Built-ins
	BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT, // Bitwise
	NULL_LIT, OPTIONAL, FALLBACK, // Literals
	MODULE, // Modules
	OPERATOR, // Operator overloading keyword
	PUB, DOUBLE_DOTS, CLEAN, NEW, // Static dispatch / pub
	CAT_VALUE, CAT_SLICE, CAT_REF,
	VAL_STORE, REF_WRITE, REF_REBIND,
	CMP_VALUE, CMP_SLICE, CMP_REF,
	RET_VALUE, RET_REF,
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
	char *pathname;
	bool  loading;
};

struct LLVM {
	bool    is_set;
	bool    is_loaded;
	Value   array_size;
	Value   elem;
	Value   err_out;
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

struct Token {
	Type    type;
	Type    ret_type;
	Type    kind;    
	Source *source;

	char   *name;
	int     indent;

	int     used;
	int     start_index;
	int     end_index; 

	bool    is_ref;
	bool    is_nullable;
	bool    is_optional;
	bool    is_dec;
	bool    is_global;
	bool    is_param;
	bool    is_variadic;
	bool    is_proto;
	bool    has_drop;
	bool    no_warn;   
	bool    is_method_call;
	bool    is_pub;
	bool    is_static_call;
	bool    is_heap;

	int     line;

	LLVM    llvm;

	struct {
		struct { long value; } Int;
		struct { int value; } Short;
		struct { long long value; } Long;
		struct { double value; } Float;
		struct { bool value; } Bool;
		struct { char *value; } Chars;
		struct { char value; } Char;
		struct { int index; Node *ptr; char *name; } Struct;
		struct { Type  sub_type; int   depth; Node *struct_ptr; } Array;
		struct { EXPAND(Token**, types); } Tuple;
		struct { Node *ptr; Token *var; } Fcall;
		struct { Token *ptr; } Decl;
		struct { Type  type; char *name; } Catch;
		struct { EXPAND(Token**, params); Token *ret; } Fn;
	};
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

struct UraGlobal {
	bool             found_error;
	bool             uses_exceptions;
	char            *output;
	int              error_count;
	int              max_errors;
	bool             enable_debug;
	bool             enable_exec;
	bool             enable_san;
	bool             enable_tree;
	bool             enable_ll;
	bool             no_color;
	// bool             enable_prep;
	char            *flags;
	char            *lib;
	double           time_start;
	// const char      *ura_target_os;

	char            *dir;
	char            *base;
	char            *build_dir;
	char            *ll_path;

	// TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
	LLVMDIBuilderRef debug_builder;
	MetadataRef      debug_compile_unit;
	MetadataRef      debug_file;
	MetadataRef      debug_scope;
	EXPAND(Node **, scopes);
	EXPAND(Node **, try_nodes);
	EXPAND(Token **, tokens);
	EXPAND(Token **, temps);
	EXPAND(Source **, sources);
	Value            err_flag;
	Value            err_value;
	Node            *error_def;
	char           **platform;
	Token           *fn_ret;
	char            *current_module;
	Source          *current;
	int              calling_use;
	Node            *scope;
	Node            *head;
	int              exe_pos;

	Context          context;
	Module           module;
	Builder          builder;
	TypeRef          vd, f32, f64, i1, i8, i16, i32, i64;
};

extern UraGlobal ura;

/* ---- frontend prototypes ---- */
void tokenize(int base);
int parse_escape_seq(char *input, int s, int e, char *buf, int *ptr);
bool lex_spaces(char *src, int *i, int *line, int *indent, int base);
bool lex_multi_comment(char *src, int *i, int *line);
bool lex_comment(char *src, int *i);
bool lex_chars(char *src, int *i, int line, int indent);
bool lex_char(char *src, int *i, int line, int indent);
bool lex_number(char *src, int *i, int line, int indent);
bool lex_use(char *src, int *i, int s, int line);
bool lex_link(char *src, int *i, int s, int line);
bool lex_identifier(char *src, int *i, int line, int indent, int base);
bool platform_has(char *name);
void preprocess();
bool lex_symbol(char *src, int *i, int line, int *indent);
Token *new_token(Type type, int indent);
Token *parse_token(int line, int s, int e, Type type, int indent);
void set_name(Token *token, char *name);
Token *next();
Token *peek(int index);
Token *find(Type type, ...);
int get_operation_precedence(Type type);
bool within(int indent);
void parser_recover(int indent);
Node *new_node(Token *token);
Node *syntax_error();
void parse_type(Token *target);
void parse_block(Node *node, int indent);
Node *match_node(Node *node);
Node *if_node(Node *node);
Node *while_node(Node *node);
Node *for_node(Node *node);
Node *ref_node(Node *node);
Node *id_node(Node *node);
Node *fdec_node(Node *node);
Node *struct_node(Node *node);
Node *enum_node(Node *node);
Node *fcall_node(Node *node);
Node *output_node(Node *node);
Node *access_node(Node *node);
Node *array_lit_node(Node *node);
Node *array_ctor_node(Node *node);
Node *prime_node();
Node *expr_node(int min_op);
void inject_self(Node *fn, Node *owner);
Node *drop_node(Node *node, Node *owner);

/* ---- backend prototypes ---- */
void enter_scope(Node *node);
void exit_scope();
Node *enclosing_continue();
Node *enclosing_break();
void analyze_block(Node *node);
void analyze_match(Node *node);
void declare_variable(Token *token);
Token *find_variable(char *name, bool *captured, bool *from_field);
bool same_signature(Token *a, Token *b);
void declare_function(Node *fn);
void declare_struct(Node *node);
void declare_structs(Node *node);
void declare_enum(Node *node);
Node *find_function(char *name);
void analyze_binop(Node *node);
void analyze_fdec(Node *node);
void resolve_struct_type(Token *token);
bool rewrite_struct_ctor(Node *node);
bool has_assign(Node *def);
void analyze_struct(Node *node);
Token *global_decl(Node *child);
void analyze_id(Node *node);
void analyze_fcall(Node *node);
void analyze_for(Node *node);
void analyze(Node *node);
void type_check_block(Node *node);
void type_check_match(Node *node);
bool is_float(Type type);
bool is_unsigned(Type type);
bool is_pointer(Type type);
bool same_or_pointer(Type a, Type b);
void set_string_type(Token *token);
bool is_string(Token *token);
bool is_null(Token *token);
bool is_nullable(Token *token);
bool is_assignable(Token *lhs, Token *rhs);
bool is_castable(Type type);
bool is_data_type(Token *token);
TypeRef to_llvm_type(Type type);
TypeRef elem_type(Token *arr, int depth);
TypeRef array_type(Token *arr, int depth);
TypeRef struct_type_of(Node *def);
TypeRef llvm_type_of(Token *token);
Value default_value(Token *token);
Node *find_method(Node *def, char *name);
void analyze_method_call(Node *node);
void type_check_method_call(Node *node);
void type_check_static_call(Node *node);
void type_check_fcall(Node *node);
void type_check_return(Node *node);
void type_check_fdec(Node *node);
bool struct_contains(Node *def, Node *target, int depth);
bool is_field(Token *token);
Token *find_field(Node *def, char *name);
void type_check_dot(Node *node);
void type_check_struct(Node *node);
bool is_untyped_literal(Node *node);
bool same_family(Type a, Type b);
void adopt_literal(Node *node, Type target);
void retype_literal(Node *node, Type target);
void unify_literals(Node *left, Node *right);
bool int_range(Type type, long *lo, long *hi);
bool literal_value(Node *node, long *out);
void check_int_range(Node *node, Type target);
void type_check_binop(Node *node);
void type_check_array_lit(Node *node);
void type_check_access(Node *node);
void type_check_array_ctor(Node *node);
void type_check_for(Node *node);
void type_check(Node *node);
void setup_paths(char *path_name);
void init_module(char *name);
void finalize_module(char *ll_path);
void debug_enter_function(Token *token);
void debug_exit_function(Token *token);
void set_debug_location(Token *token);
void guard(Token *op, Value is_bad, char *what);
void guard_nonzero(Token *op, Value divisor);
void guard_nonnull(Token *op, Value ptr);
void guard_bound(Token *op, Value ptr);
void guard_index(Token *op, Value idx, Value slice);
void guard_slice(Token *op, Value start, Value end, Value len);
Block here_block();
Value here_func();
void llvm_at(Block block);
Block llvm_block(Value fn, char *name);
Value llvm_alloca(TypeRef type, char *name);
Value llvm_load(TypeRef type, Value ptr, char *name);
void llvm_store(Value value, Value ptr);
Value llvm_gep(TypeRef type, Value ptr, Value *idx, int n, char *name);
Value llvm_icmp(LLVMIntPredicate pred, Value l, Value r, char *name);
Value llvm_fcmp(LLVMRealPredicate pred, Value l, Value r, char *name);
void llvm_br(Block dest);
void llvm_cond_br(Value cond, Block yes, Block no);
Value llvm_call(TypeRef type, Value fn, Value *args, int n, char *name);
Value llvm_extract(Value agg, unsigned index, char *name);
Value decay_ptr(Type from, Type to, Value v);
Value llvm_insert(Value agg, Value elem, unsigned index, char *name);
Value llvm_string(char *text, char *name);
Value const_int(TypeRef type, long long value);
Value const_i32(long long value);
Value const_i64(long long value);
TypeRef pointer_to(TypeRef type);
Value llvm_binop(LLVMOpcode op, Value l, Value r, char *name);
Value llvm_ret(Value value);
Value llvm_not(Value value);
Value llvm_int_cast(Value value, TypeRef type);
Value llvm_num_cast(Value value, Type src, Type dst);
Value llvm_global(Token *token);
void llvm_sanitize(Value fn);
MetadataRef llvm_di_subroutine();
MetadataRef llvm_di_function(char *name, int line, MetadataRef type);
MetadataRef llvm_di_location(int line, MetadataRef scope);
MetadataRef llvm_get_location();
void llvm_set_location(MetadataRef loc);

void emit_signature(Node *fn);
Value lib_fn(char *name, TypeRef *type);
Value emit_printf_fd(int fd, char *fmt, Value *args, int n);
Value emit_printf(char *fmt, Value *args, int n);
Value print_adapt(Type type, Value v, char **spec);
TypeRef out_frame_type();
void emit_out_call(Node *def, Value ptr, Value frame, int fd);
void emit_printer_call(Node *printer, Value self);
int  type_id(Node *def);
void emit_out_scalar(Token *info, Value slot);
void emit_out_array(Token *field, Value slot, Value frame, int depth,
                    int fd);
void emit_out_field(Token *field, Value slot, Value frame);
Value struct_printer(Node *def);
Value struct_arg_ptr(Node *arg);
Value field_ptr(Node *node);
Type category(Node *node);
Value emit_value(Node *node);
Value emit_place(Node *node);
Value emit_ref(Node *node);
Value emit_slot(Node *node);
Value access_ptr(Node *node);
void code_gen_slice(Node *node);
void code_gen_dot(Node *node);
void code_gen_access(Node *node);
void code_gen_array_lit(Node *node);
Value make_slice(Token *arr, int depth, Value data, Value len);
Value array_calloc(TypeRef elem, Value count, Value esz);
Value build_array(Token *arr, Value *dims, int depth, bool heap);
void code_gen_array_ctor(Node *node);
void free_array(Token *arr, Value slice, int depth);
void code_gen_typeof(Node *node);
void code_gen_sizeof(Node *node);
void code_gen_clean(Node *node);
void code_gen_literal(Node *node);
bool is_main(Token *token);
void init_globals();
void code_gen_fdec(Node *node);
void code_gen_id(Node *node);
void code_gen_fcall(Node *node);
void code_gen_body(Node *node);
void code_gen_loop(Node *node);
void code_gen_for_array(Node *node);
void code_gen_for(Node *node);
void code_gen_while(Node *node);
void code_gen_match(Node *node);
void code_gen_if(Node *node);
void code_gen_assign(Node *node);
void code_gen_binop(Node *node);
void code_gen_compound(Node *node);
void code_gen_output(Node *node);
void code_gen(Node *node);
char *struct_name_of(Token *token);
bool find_operator(Node *node);
void code_gen_operator(Node *node);
Node *find_destructor(Node *def);
Node *find_printer(Node *def);
bool needs_drop(Node *def);
Node *drop_target(Token *var);
void emit_drop_value(Value ptr, Node *def);
void push_temp(Value ptr, Token *of);
void drop_temps();
void emit_drops(Node *scope, Token *keep);
void emit_unwind(Node *stop, Token *keep);
void scope_out();

/* ---- utils prototypes ---- */
double clock_now();
char *signal_name(int sig);
char *format(const char *fmt, ...);
void *allocate(int len, int size);
void new_source(char *file_name);
bool includes(Type to_find, ...);
void free_token(Token *token);
void free_node(Node *node);
void free_memory();
char *to_string(Type type);
char *type_name(Type type);
int _vprint(File out, const char *conv, va_list args);
int _debug(char *conv, ...);
void fprint_escaped(File out, char c);
void print_escaped(char c);
char *array_type_label(Token *token);
char *spell(Type type);
void print_node_label(Node *node);
void print_subtree(Node *node, char *prefix, bool is_last, char *role);
void print_children(Node *node, char *prefix);
void print_ast(Node *head);
void pnode(Node *node, char *indent);
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void decolor(char *s);
void render_caret(File out, Token *token, const char *color);
bool is_dir(char *path);
char *exe_dir();
char *find_ura_lib();
void ura_lib_missing(int line, int s, int e);
void parse_error(Token *token, const char *fmt, ...);
void parse_note(Token *token, const char *fmt, ...);
void parse_warn(Token *token, const char *fmt, ...);
void tokenize_error(int line, int s, int e, const char *fmt, ...);

/* ---- utils/memory.c ---- */
double clock_now() {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   return ts.tv_sec + ts.tv_nsec / 1e9;
}

char *signal_name(int sig) {
   switch (sig) {
      case SIGSEGV: return "SIGSEGV";
      case SIGABRT: return "SIGABRT";
      case SIGTRAP: return "SIGTRAP";
      case SIGILL:  return "SIGILL";
      case SIGFPE:  return "SIGFPE";
      case SIGBUS:  return "SIGBUS";
      default:      return "signal";
   }
}

char *format(const char *fmt, ...) {
	char  *buf  = NULL;
	size_t size = 0;
	File   out  = open_memstream(&buf, &size);
	if (CHECK(!out, "format: open_memstream failed")) return NULL;

	va_list ap;
	va_start(ap, fmt);
	_vprint(out, fmt, ap);
	va_end(ap);
	fclose(out);
	return buf;
}

void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

bool is_dir(char *path) {
	struct stat st;
	return path && stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

char *exe_dir() {
	char buf[PATH_MAX] = {0};
#if defined(__APPLE__)
	uint32_t size = sizeof(buf);
	if (_NSGetExecutablePath(buf, &size) != 0) return NULL;
#elif defined(__linux__)
	ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (n <= 0) return NULL;
	buf[n] = '\0';
#endif
	char *full = realpath(buf, NULL);
	if (!full) return NULL;
	char *copy = strdup(full);
	char *dir  = strdup(dirname(copy));
	free(copy);
	free(full);
	return dir;
}

char *find_ura_lib() {
	char *dir = exe_dir();
	if (!dir) return NULL;
	char *beside = format("%s/ura-lib", dir);
	if (is_dir(beside)) { free(dir); return beside; }
	free(beside);
	char *above = format("%s/../ura-lib", dir);
	free(dir);
	if (is_dir(above)) return above;
	free(above);
	return NULL;
}

void ura_lib_missing(int line, int s, int e) {
	char *env   = getenv("URA_LIB");
	char *dir   = exe_dir();
	char *one   = env ? strdup(env) : strdup("$URA_LIB (not set)");
	char *two   = dir ? format("%s/ura-lib", dir) : strdup("<unknown>");
	char *three = dir ? format("%s/../ura-lib", dir) : strdup("<unknown>");
	tokenize_error(line, s, e, ERR_NO_URA_LIB, one, two, three);
	free(dir);
	free(one);
	free(two);
	free(three);
}

void new_source(char *file_name) {
	char *full = realpath(file_name, NULL);
	if (!full) {
		parse_error(NULL, "Cannot find file '%s'", file_name);
		return;
	}
	for (int i = 0; i < ura.sources_count; i++)
		if (strcmp(ura.sources[i]->pathname, full) == 0) {
			if (ura.sources[i]->loading)
				parse_warn(NULL, "import cycle: '%s' imports itself through"
				                 " '%s'", file_name, ura.current->filename);
			free(full);
			return;
		}
	File file = fopen(full, "r");
	if (!file) {
		parse_error(NULL, "Cannot open file '%s'", file_name);
		free(full);
		return;
	}
	char   *dir   = strdup(full);
	Source *src   = allocate(1, sizeof(Source));
	src->pathname = full;
	src->filename = strdup(file_name);
	src->dirname  = strdup(dirname(dir));
	free(dir);

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	src->content = allocate((size + 1), sizeof(char));
	fread(src->content, size, sizeof(char), file);
	fclose(file);

	resize_array(ura.sources, Source *);
	ura.sources[ura.sources_count++] = src;
}

bool includes(Type to_find, ...) {
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

void free_node(Node *node) {
   if (!node) return;
   if (!includes(node->token->type, BREAK, CONTINUE, 0))
      free_node(node->left);
   free_node(node->right);
   for (int i = 0; i < node->children_count; i++)
      free_node(node->children[i]);
   free(node->children);
   free(node->variables);
   free(node->functions);
   free(node->structs);
   free(node->modules);
   free(node);
}

void free_memory() {
   free_node(ura.head);
   for (int i = 0; i < ura.tokens_count; i++) {
      Token *token = ura.tokens[i];
      if (token) {
         free(token->name);
         free(token->Chars.value);
         free(token->llvm.dims);
         free(token);
      }
   }
   free(ura.tokens);
   for (int i = 0; i < ura.sources_count; i++) {
      free(ura.sources[i]->content);
      free(ura.sources[i]->filename);
      free(ura.sources[i]->dirname);
      free(ura.sources[i]->pathname);
      free(ura.sources[i]);
   }
   free(ura.sources);
   free(ura.scopes);
   if (ura.context) {
      LLVMDisposeBuilder(ura.builder);
      LLVMDisposeModule(ura.module);
      LLVMContextDispose(ura.context);
   }
}

/* ---- utils/diagnostics.c ---- */
char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID",              [CHAR] = "CHAR",             [CHARS] = "CHARS",
	    [PTR] = "PTR",
	    [I8] = "I8",              [I16] = "I16",               [I32] = "I32",
	    [I64] = "I64",            [U8] = "U8",                 [U16] = "U16",
	    [U32] = "U32",            [U64] = "U64",               [F32] = "F32",
	    [F64] = "F64",            [VOID] = "VOID",             [BOOL] = "BOOL",
	    [FDEC] = "FDEC",          [END] = "END",               [LPAR] = "LPAR",
	    [FCALL] = "CALL",
	    [IF] = "IF",              [RPAR] = "RPAR",             [ELIF] = "ELIF",
	    [FOR] = "FOR",            [ELSE] = "ELSE",             [WHILE] = "WHILE",
	    [BY] = "BY",              [LOOP] = "LOOP",
	    [IN] = "IN",              [BREAK] = "BRK",             [CONTINUE] = "CONT",
	    [CASE] = "CASE",          [MATCH] = "MATCH",           [DEFAULT] = "DEFAULT",
	    [BAND] = "BAND",          [RETURN] = "RET",            [BOR] = "BOR",
	    [BXOR] = "BXOR",          [BNOT] = "BNOT",
	    [ADD] = "ADD",            [LSHIFT] = "LSHIFT",         [RSHIFT] = "RSHIFT",
	    [SUB] = "SUB",            [MUL] = "MUL",               [DIV] = "DIV",
	    [ASSIGN] = "ASSIGN",      [ADD_ASSIGN] = "ADD_ASS",    [SUB_ASSIGN] = "SUB_ASS",
	    [MUL_ASSIGN] = "MUL_ASS", [DIV_ASSIGN] = "DIV_ASS",    [MOD_ASSIGN] = "MOD_ASS",
	    [BAND_ASSIGN] = "AND_ASS", [BOR_ASSIGN] = "OR_ASS",   [BXOR_ASSIGN] = "XOR_ASS",
	    [LSHIFT_ASSIGN] = "SHL_ASS", [RSHIFT_ASSIGN] = "SHR_ASS",
	    [MOD] = "MOD",            [COMA] = "COMA",             [ACCESS] = "ACC",
	    [REF] = "REF",            [EQUAL] = "EQ",              [NOT_EQUAL] = "NEQ",
	    [LESS] = "LT",            [GREAT] = "GT",              [LESS_EQUAL] = "LE",
	    [NOT] = "NOT",            [AND] = "AND",               [GREAT_EQUAL] = "GE",
	    [OR] = "OR",              [DOTS] = "DOTS",             [PROTO] = "PROT",
	    [VARIADIC] = "VAR",       [TYPEOF] = "TYPEOF",         [SIZEOF] = "SIZEOF",
	    [ARGS] = "ARGS",          [OUTPUT] = "OUTPUT",         [ERRPUT] = "ERRPUT",
	    [CHILDREN] = "CHILDREN",
	    [AS] = "AS",              [NULL_LIT] = "NULL_LIT",    [FALLBACK] = "FALLBACK",     [ARRAY_LIT] = "ARRAY_LIT",
	    [ARRAY_TYPE] = "ARRAY_TYPE",
	    [OPTIONAL] = "OPTIONAL",  [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [TUPLE] = "TUPLE",        [ENUM_DEF] = "ENUM_DEF",     [ENUM_CALL] = "ENUM_CALL",
	    [LBRA] = "LBRA",          [RBRA] = "RBRA",             [TUPLE_UNPACK] = "TUPLE_UNPACK",
	    [LIST] = "LIST",          [ARRAY] = "ARRAY",           [LIST_TYPE] = "LIST_TYPE",
	    [DOT] = "DOT",            [RANGE] = "RANGE",           [FN_TYPE] = "FN_TYPE",
	    [MODULE] = "MODULE",      [OPERATOR] = "OPERATOR_KW",  [SYNTAX_ERROR] = "SYNTAX_ERROR",
	    [PUB] = "PUB",            [CLEAN] = "CLEAN",           [DOUBLE_DOTS] = "DOUBLE_DOTS",
	    [NEW] = "NEW",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	};

	TODO(!res[type], "handle this case %d\n", type);
	return res[type];
}

char *type_name(Type type) {
	switch (type) {
	case I8:         return "i8";
	case I16:        return "i16";
	case I32:        return "i32";
	case I64:        return "i64";
	case U8:         return "u8";
	case U16:        return "u16";
	case U32:        return "u32";
	case U64:        return "u64";
	case CHAR:       return "char";
	case PTR:        return "pointer";
	case BOOL:       return "bool";
	case F32:        return "f32";
	case F64:        return "f64";
	case VOID:       return "void";
	case ARRAY_TYPE: return "array";
	case FN_TYPE:    return "fn";
	default:         return type ? to_string(type) : "unknown";
	}
}

int _vprint(File out, const char *conv, va_list args) {
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
			case 'x': {
				unsigned int value = va_arg(args, unsigned int);
				res += (prec >= 0) ? fprintf(out, "%.*x", prec, value)
				                   : fprintf(out, "%x", value);
				break;
			}
			case 'X': {
				unsigned int value = va_arg(args, unsigned int);
				res += (prec >= 0) ? fprintf(out, "%.*X", prec, value)
				                   : fprintf(out, "%X", value);
				break;
			}
			case 'd': {
				int value = va_arg(args, int);
				res += (prec >= 0) ? fprintf(out, "%.*d", prec, value)
				                   : fprintf(out, "%d", value);
				break;
			}
			case 'f': {
				double value = va_arg(args, double);
				res += (prec >= 0) ? fprintf(out, "%.*f", prec, value)
				                   : fprintf(out, "%f", value);
				break;
			}
			case '%': res += fprintf(out, "%%"); break;
			case 't': {
				Type type = (Type)va_arg(args, Type);
				res += fprintf(out, "%s", to_string(type));
				break;
			}
			case 'k': {
				Token *token = va_arg(args, Token *);
				if (!token) {
					fprintf(out, "(null)");
					break;
				}

				fprintf(out, "[%s] ", to_string(token->type));

				switch (token->type) {
				case VOID: case CHARS: case CHAR: case I32: case BOOL: case F32:
				case I64: {
					if (token->name) {
						fprintf(out, "%s ", token->name);
						break;
					}
					if (token->type == VOID) break;
					switch (token->type) {
					case I32:   fprintf(out, "[%lld] ", (long long)token->Int.value);
						break;
					case I64:  fprintf(out, "[%lld] ", token->Long.value); break;
					case BOOL: {
						char *text = token->Bool.value ? "True" : "False";
						fprintf(out, "[%s] ", text);
						break;
					}
					case F32: fprintf(out, "[%f] ", token->Float.value); break;
					case CHAR:  {
						fprintf(out, "[");
						fprint_escaped(out, token->Char.value);
						fprintf(out, "] ");
						break;
					}
					case CHARS: {
						fprintf(out, "[\"");
						char *str = token->Chars.value;
						if (str)
							for (int j = 0; str[j]; j++)
								fprint_escaped(out, str[j]);
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

int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = _vprint(stdout, conv, args);
	va_end(args);
	return res;
}

void fprint_escaped(File out, char c) {
	switch (c) {
	case '\n': fputs("\\n", out); break;
	case '\t': fputs("\\t", out); break;
	case '\r': fputs("\\r", out); break;
	case '\\': fputs("\\\\", out); break;
	case '"':  fputs("\\\"", out); break;
	default:   fputc(c, out);
	}
}

void print_escaped(char c) {
	fprint_escaped(stdout, c);
}

static char *spelling[END + 1] = {
		[I8] = "i8",         [I16] = "i16",         [I32] = "i32",
		[I64] = "i64",       [U8] = "u8",           [U16] = "u16",
		[U32] = "u32",       [U64] = "u64",         [F32] = "f32",
		[F64] = "f64",       [BOOL] = "bool",       [CHAR] = "char",
		[PTR] = "pointer",   [VOID] = "void",       [ADD] = "+",
		[SUB] = "-",         [MUL] = "*",           [DIV] = "/",
		[MOD] = "%",         [EQUAL] = "==",        [NOT_EQUAL] = "!=",
		[LESS] = "<",        [GREAT] = ">",         [LESS_EQUAL] = "<=",
		[OR] = "or",         [AND] = "and",         [GREAT_EQUAL] = ">=",
		[BOR] = "|",         [BAND] = "&",          [NOT] = "not",
		[BXOR] = "^",        [BNOT] = "~",          [LSHIFT] = "<<",
		[ASSIGN] = "=",      [RSHIFT] = ">>",       [ADD_ASSIGN] = "+=",
		[SUB_ASSIGN] = "-=", [MUL_ASSIGN] = "*=",   [DIV_ASSIGN] = "/=",
		[MOD_ASSIGN] = "%=", [BAND_ASSIGN] = "&=",  [BOR_ASSIGN] = "|=",
		[BXOR_ASSIGN] = "^=", [LSHIFT_ASSIGN] = "<<=", [RSHIFT_ASSIGN] = ">>=",
		[IF] = "if",         [ELIF] = "elif",
		[ELSE] = "else",     [WHILE] = "while",     [MATCH] = "match",
		[CASE] = "case",     [BREAK] = "break",     [DEFAULT] = "default",
		[RETURN] = "return", [OUTPUT] = "output",   [ERRPUT] = "errput",
		[CONTINUE] = "continue",
		[REF] = "ref",       [AS] = "cast",         [FOR] = "for",
		[LOOP] = "loop",     [RANGE] = "range",     [ACCESS] = "index",
		[ARRAY] = "array",   [ARRAY_LIT] = "array", [ARRAY_TYPE] = "array",
		[NEW] = "new",       [TYPEOF] = "typeof",   [SIZEOF] = "sizeof",
		[CLEAN] = "clean",     [TRY] = "try",         [CATCH] = "catch",
		[THROW] = "throw",     [MODULE] = "mod",
};

char *spell(Type type) {
	return spelling[type] ? spelling[type] : to_string(type);
}

void print_node_label(Node *node) {
	Token *token = node->token;
	switch (token->type) {
	case I32:   printf("int %lld", (long long)token->Int.value); return;
	case I64:  printf("long %lld", token->Long.value); return;
	case I16: printf("short %d", token->Short.value); return;
	case F32: printf("float %g", token->Float.value); return;
	case BOOL:  printf("bool %s", token->Bool.value ? "True" : "False"); return;
	case CHAR: {
		printf("char '");
		print_escaped(token->Char.value);
		printf("'");
		return;
	}
	case CHARS: {
		printf("char[] \"");
		char *value = token->Chars.value;
		for (int i = 0; value[i]; i++)
			print_escaped(value[i]);
		printf("\"");
		return;
	}
	case ID:    printf("%s", token->name); break;
	case FCALL: printf("call %s", token->name); break;
	case FDEC: {
		printf("%sfn %s(", token->is_proto ? "proto " : "", token->name);
		for (int i = 0; i < token->Fn.params_count; i++) {
			Token *param = token->Fn.params[i];
			printf("%s%s", i ? ", " : "", param->name);
			if (param->ret_type)
				printf(" : %s", spell(param->ret_type));
		}
		if (token->is_variadic)
			printf("%s...", token->Fn.params_count ? ", " : "");
		printf(")");
		break;
	}
	case STRUCT_DEF: printf("struct %s", token->name); return;
	case MODULE:     printf("mod %s", token->name); return;
	case NEW:        printf("new %s", token->Struct.name); return;
	case DOT:   printf(".%s", token->name); break;
	default:    printf("%s", spell(token->type));
	}
	if (token->ret_type == ARRAY_TYPE && token->Array.sub_type) {
		char *t = strdup(type_name(token->Array.sub_type));
		for (int i = 0; i < token->Array.depth; i++) {
			char *n = format("%s[]", t);
			free(t);
			t = n;
		}
		printf(" : %s", t);
		free(t);
	} else if (token->ret_type)
		printf(" : %s", spell(token->ret_type));
}

void print_children(Node *node, char *prefix) {
	int    capacity = node->children_count + (node->left ? node->left->children_count : 0) + 2;
	Node  *edge_node[capacity];
	char  *edge_role[capacity];
	int    count = 0;
	Token *token = node->token;
	switch (token->type) {
	case IF:
	case ELIF: {
		edge_role[count] = "condition";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		if (node->right) {
			edge_role[count] = NULL;
			edge_node[count++] = node->right;
		}
		break;
	}
	case WHILE: {
		edge_role[count] = "condition";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case MATCH: {
		edge_role[count] = "subject";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case CASE: {
		for (int i = 0; node->left && i < node->left->children_count; i++) {
			edge_role[count] = "value";
			edge_node[count++] = node->left->children[i];
		}
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case TRY: {
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		if (node->right) {
			edge_role[count] = NULL;
			edge_node[count++] = node->right;
		}
		break;
	}
	case CATCH: {
		if (node->left) {
			edge_role[count] = "binds";
			edge_node[count++] = node->left;
		}
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case BREAK:
	case CONTINUE:
		break;
	case AS: {
		edge_role[count] = NULL;
		edge_node[count++] = node->left;
		break;
	}
	default: {
		if (node->left) {
			edge_role[count] = NULL;
			edge_node[count++] = node->left;
		}
		if (node->right) {
			edge_role[count] = NULL;
			edge_node[count++] = node->right;
		}
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
	}
	}
	for (int i = 0; i < count; i++) {
		Node *node = edge_node[i];
		bool is_last = i == count - 1;
		char *role = edge_role[i];
		printf("%s%s", prefix, is_last ? "└─ " : "├─ ");
		if (role) printf("%s ", role);
		print_node_label(node);
		putchar('\n');
		char *child_prefix = format("%s%s", prefix, is_last ? "   " : "│  ");
		print_children(node, child_prefix);
		free(child_prefix);
	}
}

void print_ast(Node *head) {
	for (int i = 0; i < head->children_count; i++) {
		print_node_label(head->children[i]);
		putchar('\n');
		print_children(head->children[i], "");
		if (i < head->children_count - 1)
			putchar('\n');
	}
}

void pnode(Node *node, char *indent) {
	if (!node || !node->token || !ura.enable_debug) return;
	Node **subs     = NULL;
	int    subs_count    = 0;
	int    subs_size = 0;

#define push(n)                                                             \
	do {                                                                     \
		resize_array(subs, Node *);                                           \
		subs[subs_count++] = (n);                                             \
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
	for (int i = 0; i < subs_count; i++) {
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int         is_last = (i == subs_count - 1);
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

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	ura.error_count++;
	fprintf(stderr, RED("ura error: %s:%s:%d "), filename, funcname, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void decolor(char *text) {
	int write = 0;
	for (int read = 0; text[read]; ) {
		bool is_escape = text[read] == '\033' && text[read + 1] == '[';
		if (!is_escape) {
			text[write++] = text[read++];
			continue;
		}
		read += 2;
		while (text[read] && text[read] != 'm') read++;
		if (text[read] == 'm') read++;
	}
	text[write] = '\0';
}

void render_caret(File out, Token *token, const char *color) {
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
	for (int i = line_no; i >= 10; i /= 10)
		gutter++;

	char *name = token->source->filename;
	char *lib  = strstr(name, "/ura-lib/");
	if (lib) name = lib + 1;
	fprintf(out, "%*s \033[2m%s:%d:%d\033[0m\n", gutter, "", name, line_no, col);
	fprintf(out, "%*s " BLUE("|") "\n", gutter, "");
	fprintf(out, BLUE("%*d |") " %.*s\n", gutter, line_no, line_end - line_start, content + line_start);
	fprintf(out, "%*s " BLUE("|") " ", gutter, "");
	for (int i = 0; i < col - 1; i++)
		fputc(content[line_start + i] == '\t' ? '\t' : ' ', out);
	fprintf(out, "%s", color);
	for (int i = 0; i < span; i++)
		fputc('^', out);
	fprintf(out, RESET "\n");
}

void parse_error(Token *token, const char *fmt, ...) {
	ura.error_count++;
	ura.found_error = true;
	if (ura.error_count > ura.max_errors) {
		fprintf(stderr, RED("error: ") "Too many errors, stopping\n");
		return;
	}

	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, RED("error: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token, CARET_ERR);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void parse_warn(Token *token, const char *fmt, ...) {
	if (token && token->no_warn) return;
	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, YELLOW("warning: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token, CARET_WARN);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void parse_note(Token *token, const char *fmt, ...) {
	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, BLUE("note: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token, CARET_WARN);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void tokenize_error(int line, int s, int e, const char *fmt, ...) {
	Source *src = ura.current;
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

/* ---- frontend/lexer.c ---- */
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
	case '\\': buf[j++] = '\\'; break; 	// backslash
	case '"':  buf[j++] = '"' ; break;  // double quote
	case '\'': buf[j++] = '\''; break; 	// single quote
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
	case 'x': { // Hexadecimal: \xFF
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
	case 'u': { // \uXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	}
	case 'U': { // \UXXXXXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	}
	default: { // unknown escape, keep backslash
		buf[j++] = input[s]; 
		break;
	}
	}
	*ptr = j;
	return ret;
}

bool lex_spaces(char *src, int *i, int *line, int *indent, int base)
{
	char c = src[*i];
	if (!isspace(c)) 
	 	return false;
	 
	if (c == '\n') {
		(*line)++;
		(*indent)    = base;
	}
	else {
		int j = *i;
		while (j > 0 && (src[j - 1] == ' ' || src[j - 1] == '\t'))
			j--;
		if (j == 0 || src[j - 1] == '\n')
			(*indent) += (c == '\t') ? TAB : 1;
	}
	(*i)++;

	return true;
}

bool lex_multi_comment(char *src, int *i, int *line)
{
	int s = *i;
	if (strncmp(src + (*i), "/*", 2) != 0)
		return false;
	
	(*i) += 2;
	while (src[(*i)] && src[(*i) + 1] && strncmp(src + (*i), "*/", 2)) {
		if (src[(*i)] == '\n') (*line)++;
		(*i)++;
	}
	if (strncmp(src + (*i), "*/", 2) != 0) {
		tokenize_error((*line), s, s + 2, ERR_UNTERM_BLOCK_COMMENT);
		return true;
	}
	(*i) += 2;
	return true;
}

bool lex_comment(char *src, int *i)
{
	if (strncmp(src + (*i), "//", 2) != 0)
		return false;

	while (src[(*i)] && src[(*i)] != '\n')
		(*i)++;
	return true;
}

bool lex_chars(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (src[(*i)] != '\"')
		return false;

	(*i)++;
	while (src[(*i)] && src[(*i)] != '\"') {
		if (src[(*i)] == '\\' && src[(*i) + 1]) (*i)++;
		(*i)++;
	}
	if (src[(*i)] != '\"') {
		tokenize_error(line, s, s + 1, ERR_UNTERM_STRING_LITERAL);
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHARS, indent);
	return true;
}

bool lex_char(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (src[(*i)] != '\'')
		return false;

	(*i)++;
	if (src[(*i)] == '\\' && src[(*i) + 1]) (*i)++;
	if (src[(*i)] && src[(*i)] != '\'')     (*i)++;
	if (src[(*i)] != '\'')                 {
		tokenize_error(line, s, s + 1, ERR_UNTERM_CHAR_LITERAL);
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHAR, indent);
	return true;
}

bool lex_number(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (!isdigit(src[(*i)])) return false;
	while (isdigit(src[(*i)])) (*i)++;
	if (src[(*i)] == '.' && isdigit(src[(*i) + 1])) {
		(*i)++;
		while (isdigit(src[(*i)])) (*i)++;
		parse_token(line, s, *i, F32, indent);
	}
	else
		parse_token(line, s, *i, I32, indent);
	return true;
}

bool platform_has(char *name) {
	for (int i = 0; ura.platform && ura.platform[i]; i++)
		if (strcmp(ura.platform[i], name) == 0) return true;
	return false;
}

void preprocess() {
	int  gate   = -1;
	bool active = false;
	bool taken  = false;
	bool mute   = false;
	int  write  = 0;
	for (int i = 0; i < ura.tokens_count; i++) {
		Token *token = ura.tokens[i];
		Type   type  = token->type;
		if (type == AT_NO_WARN) { 
			mute = true;
			continue;
		}
		if (type == AT_IF || type == AT_ELIF || type == AT_ELSE) {
			char *name = type == AT_ELSE ? NULL : ura.tokens[++i]->name;
			i++;
			if (type == AT_IF) { gate = token->indent; taken = false; }
			active = name ? (!taken && platform_has(name)) : !taken;
			taken  = taken || active;
			continue;
		}
		if (gate >= 0 && token->indent <= gate) gate = -1;
		if (gate < 0 || active) {
			if (mute) { token->no_warn = true; mute = false; }
			ura.tokens[write++] = token;
		}
	}
	ura.tokens_count = write;
}

bool lex_use(char *src, int *i, int s, int line)
{
	bool is_len   = (*i) - s == 3;
	bool is_word  = strncmp(src + s, "use", 3) == 0;
	bool is_space = isspace(src[(*i)]);
	if (!is_len || !is_word || !is_space) return false;
	while (isspace(src[(*i)]))
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'use'");
		return true;
	}
	(*i)++;
	{
		int start = (*i);
		while (src[(*i)] && src[(*i)] != '\"' && src[(*i)] != '\n')
			(*i)++;
		if (src[(*i)] != '\"') {
			tokenize_error(line, start - 1, start, ERR_UNTERM_USE_PATH);
			return true;
		}
		(*i)++;
		char *use = strndup(src + start, (*i) - start - 1);
		char *tmp = format("%s.ura", use);
		free(use);
		use = tmp;
		if (use[0] == '@') {
			char *rest = use + 1;
			if (*rest == '/') rest++;
			if (!ura.lib) {
				ura_lib_missing(line, start - 1, start);
				free(use);
				return true;
			}
			tmp = format("%s/%s", ura.lib, rest);
			free(use);
			use = tmp;
		} else if (use[0] != '/') {
			tmp            = format("%s/%s", ura.current->dirname, use);
			free(use);
			use = tmp;
		}
		new_source(use);
	}
	return true;
}

bool lex_link(char *src, int *i, int s, int line)
{
	bool is_len   = (*i) - s == 4;
	bool is_word  = strncmp(src + s, "link", 4) == 0;
	bool is_space = isspace(src[(*i)]);
	if (!is_len || !is_word || !is_space) return false;
	while (isspace(src[(*i)]))
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'link'");
		return true;
	}
	(*i)++;
	int link_s = (*i);
	while (src[(*i)] && src[(*i)] != '\"' && src[(*i)] != '\n')
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, link_s - 1, link_s, ERR_UNTERM_LINK_PATH);
		return true;
	}
	(*i)++;
	return true;
}

bool lex_identifier(char *src, int *i, int line, int indent, int base)
{
	int s = *i;
	if (!(isalpha(src[*i]) || strchr("@$_", src[*i])))
		return false;
	bool at_dir = src[s] == '@';
	while (src[*i] && (isalnum(src[*i]) || strchr("@$_", src[*i]) || (at_dir && src[*i] == '-')))
		(*i)++;
	int loaded = ura.sources_count;
	if (lex_use(src, i, s, line)) {
		if (ura.error_count || ura.sources_count == loaded) return true;
		ura.calling_use++;
		tokenize(indent);
		ura.calling_use--;
	} else if (!lex_link(src, i, s, line)) {
		parse_token(line, s, *i, ID, indent);
	}
	return true;
}

bool lex_symbol(char *src, int *i, int line, int *indent)
{
	int s = *i;
	static const Keyword specials[] = {
		{"...", VARIADIC, 0, 0},   {"..", RANGE, 0, 0},       {".", DOT, 0, 0},
		{"::", DOUBLE_DOTS, 0, 0}, {":", DOTS, 0, 0},         {"+=", ADD_ASSIGN, 0, 0},
		{"-=", SUB_ASSIGN, 0, 0},  {"*=", MUL_ASSIGN, 0, 0},  {"/=", DIV_ASSIGN, 0, 0},
		{"%=", MOD_ASSIGN, 0, 0},  {"!=", NOT_EQUAL, 0, 0},   {"!", NOT, 0, 0},
		{"==", EQUAL, 0, 0},		 	{"<<=", LSHIFT_ASSIGN, 0, 0}, {">>=", RSHIFT_ASSIGN, 0, 0},
		{"&=", BAND_ASSIGN, 0, 0}, {"|=", BOR_ASSIGN, 0, 0},  {"^=", BXOR_ASSIGN, 0, 0},
		{"<<", LSHIFT, 0, 0},      {">>", RSHIFT, 0, 0},
		{"<=", LESS_EQUAL, 0, 0},  {">=", GREAT_EQUAL, 0, 0}, {"<", LESS, 0, 0},
		{">", GREAT, 0, 0},        {"=", ASSIGN, 0, 0},       {"+", ADD, 0, 0},
		{"-", SUB, 0, 0},          {"*", MUL, 0, 0},          {"/", DIV, 0, 0},
		{"%", MOD, 0, 0},          {"(", LPAR, 0, 0},         {")", RPAR, 0, 0},
		{"[", LBRA, 0, 0},         {"]", RBRA, 0, 0},         {",", COMA, 0, 0},
		{"&&", AND, 0, 0},         {"||", OR, 0, 0},          {"&", BAND, 0, 0},
		{"|", BOR, 0, 0},          {"^", BXOR, 0, 0},         {"~", BNOT, 0, 0},
		{"??", FALLBACK, 0, 0},    {"?", OPTIONAL, 0, 0},
	};
	for (size_t j = 0; j < sizeof(specials) / sizeof(*specials); j++) {
		size_t len = strlen(specials[j].name);
		if (strncmp(specials[j].name, src + (*i), len) == 0) {
			(*i) += len;
			parse_token(line, s, (*i), specials[j].type, *indent);
			if (includes(specials[j].type, DOTS, 0)) (*indent) += TAB;
			return true;
		}
	}
	return false;
}

Token *new_token(Type type, int indent) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->indent = ((indent + TAB / 2) / TAB) * TAB;
	resize_array(ura.tokens, Token *);
	ura.tokens[ura.tokens_count++] = token;
	return token;
}

Token *parse_token(int line, int s, int e, Type type, int indent) {
	Source *src = ura.current;
	Token *new       = new_token(type, indent);
	char *input      = src->content;
	new->line        = line;
	new->source      = src;
	new->start_index = s;
	new->end_index   = e;

	switch (type) {
	case I32: {
		while (s < e) new->Int.value = new->Int.value * 10 + (input[s++] - '0');
		new->ret_type = new->Int.value > INT32_MAX ? I64 : I32;
		break;
	}
	case F32: {
		// TODO: to be checked
		char buf[64] = {0};
		int  len     = e - s;
		if (len > 63) len = 63;
		strncpy(buf, input + s, len);
		new->Float.value = atof(buf);
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

		static const Keyword keywords[] = {
			{"bool", BOOL, 1, 1},     {"char", CHAR, 1, 1},
			{"void", VOID, 1, 1},     {"pointer", PTR, 1, 1},
			{"i8", I8, 1, 1},         {"i16", I16, 1, 1},         {"i32", I32, 1, 1},
			{"i64", I64, 1, 1},       {"u8", U8, 1, 1},           {"u16", U16, 1, 1},
			{"u32", U32, 1, 1},       {"u64", U64, 1, 1},         {"f32", F32, 1, 1},
			{"f64", F64, 1, 1},
			{"if", IF, 0, 0},         {"elif", ELIF, 0, 0},       {"else", ELSE, 0, 0},
			{"@if", AT_IF, 0, 0},     {"@elif", AT_ELIF, 0, 0},   {"@else", AT_ELSE, 0, 0},
			{"@no-warn", AT_NO_WARN, 0, 0},
			{"for", FOR, 0, 0},       {"loop", LOOP, 0, 0},       {"while", WHILE, 0, 0},
			{"by", BY, 0, 0},         {"in", IN, 0, 0},
			{"fn", FDEC, 0, 0},       {"break", BREAK, 0, 0},     {"return", RETURN, 0, 0},
			{"case", CASE, 0, 0},     {"match", MATCH, 0, 0},     {"continue", CONTINUE, 0, 0},
			{"try", TRY, 0, 0},       {"catch", CATCH, 0, 0},     {"throw", THROW, 0, 0},
			{"ret", RETURN, 0, 0},
			{"ref", REF, 0, 0},       {"default", DEFAULT, 0, 0}, {"struct", STRUCT_DEF, 0, 0},
			{"mod", MODULE, 0, 0},    {"proto", PROTO, 0, 0},     {"enum", ENUM_DEF, 0, 0},
			{"as", AS, 0, 0},         {"pub", PUB, 0, 0},         {"operator", OPERATOR, 0, 0},
			{"new", NEW, 0, 0},       {"and", AND, 0, 1},         {"clean", CLEAN, 0, 0},
			{"or", OR, 0, 1},         {"not", NOT, 0, 1},         {"is", EQUAL, 0, 1},
			{"typeof", TYPEOF, 0, 1}, {"sizeof", SIZEOF, 0, 1},   {"List", LIST_TYPE, 0, 1},
			{"array", ARRAY_TYPE, 0, 1}, {"null", NULL_LIT, 0, 1},
		};

		for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
			if (strcmp(new->name, keywords[i].name) != 0) continue;
			new->type   = keywords[i].type;
			new->is_dec = keywords[i].is_dec;
			if (keywords[i].is_dec || keywords[i].clear_name) 
				set_name(new, NULL);
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
			bool is_escape = s + 1 < e && input[s] == '\\';
			if (is_escape) s = parse_escape_seq(input, s, e, value, &j);
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
				tokenize_error(line, s, e, ERR_UNKNOWN_ESCAPE, input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}
	default: break;
	}
	return new;
}

/* ---- frontend/parser.c ---- */
void set_name(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

Token *next() {
	Token *token = peek(0);
	if (ura.exe_pos + 1 < ura.tokens_count) ura.exe_pos++;
	return token;
}

Token *peek(int index) {
	int pos = ura.exe_pos + index;
	if (pos >= ura.tokens_count) pos = ura.tokens_count - 1;
	return ura.tokens[pos];
}

Token *find(Type type, ...) {
	va_list ap;
	va_start(ap, type);
	while (type && ura.tokens[ura.exe_pos]) {
		if (type == ura.tokens[ura.exe_pos]->type) return ura.tokens[ura.exe_pos++];
		type = va_arg(ap, Type);
	}
	return NULL;
}

bool within(int indent) {
	Token *curr = ura.tokens[ura.exe_pos];
	return !ura.found_error && curr->indent > indent && curr->type != END;
}

void parser_recover(int indent) {
	int start_line = ura.exe_pos < ura.tokens_count ? ura.tokens[ura.exe_pos]->line : -1;
	while (ura.exe_pos < ura.tokens_count && ura.tokens[ura.exe_pos]->type != END) {
		Token *t = ura.tokens[ura.exe_pos];
		if (t->indent <= indent && t->line != start_line) break;
		ura.exe_pos++;
	}
	ura.found_error = false;
}

Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *syntax_error() {
	 return new_node(new_token(SYNTAX_ERROR, -1));
}

void parse_type(Token *target) {
	if (peek(0)->type == FDEC) {
		next();
		target->ret_type = FN_TYPE;
		if (!find(LPAR, 0)) {
			parse_error(peek(0), "Expected '(' in function type");
			return;
		}
		while (!ura.found_error && peek(0)->type != RPAR) {
			Token *param = new_token(ID, 0);
			parse_type(param);
			resize_array(target->Fn.params, Token *);
			target->Fn.params[target->Fn.params_count++] = param;
			while (find(COMA, 0));
		}
		if (!find(RPAR, 0))
			parse_error(peek(0), "Expected ')' in function type");
		target->Fn.ret = new_token(ID, 0);
		parse_type(target->Fn.ret);
	} else if (is_data_type(peek(0)) || peek(0)->type == ID) {
		Token *base = next();
		if (base->type == ID) {
			target->ret_type = STRUCT_CALL;
			char *tname = strdup(base->name);
			while (find(DOUBLE_DOTS, 0)) {
				Token *seg = find(ID, 0);
				if (!seg) {
					parse_error(base, "Expected a type name after '::'");
					break;
				}
				char *j = format("%s.%s", tname, seg->name);
				free(tname);
				tname = j;
			}
			target->Struct.name = tname;
		} else
			target->ret_type = base->type;
		int depth = 0;
		while (peek(0)->type == LBRA && peek(1)->type == RBRA) {
			find(LBRA, 0);
			find(RBRA, 0);
			depth++;
		}
		if (peek(0)->type == LBRA)
			parse_error(peek(0), ERR_ARRAY_SIZE_IN_TYPE);
		if (depth > 0) {
			target->Array.sub_type = target->ret_type;
			target->Array.depth    = depth;
			target->ret_type       = ARRAY_TYPE;
		}
		if (find(OPTIONAL, 0)) target->is_optional = true;
		if (target->ret_type == PTR) target->is_optional = true;
	} else {
		parse_error(peek(0), "Expected a type");
	}
}

void parse_block(Node *node, int indent) {
	while (within(indent)) {
		resize_array(node->children, Node*);
		node->children[node->children_count++] = expr_node(0);
	}
}

Node *postfix(Node *node) {
	if (peek(0)->type == LBRA || peek(0)->type == DOT)
		return access_node(node);
	return node;
}

void inject_self(Node *fn, Node *owner) {
	Token *self = new_token(ID, 0);
	set_name(self, "self");
	self->ret_type    = STRUCT_CALL;
	self->Struct.name = owner->token->name;
	self->Struct.ptr  = owner;
	self->is_dec      = true;
	self->is_ref      = true;
	self->is_param    = true;
	resize_array(fn->token->Fn.params, Token *);
	fn->token->Fn.params[fn->token->Fn.params_count++] = self;
}

Node *fdec_node(Node *node) {
	node->token->type = FDEC;
	Node *owner  = ura.scope;
	bool  method = owner && owner->token->type == STRUCT_DEF;
	enter_scope(node);
	if (!find(LPAR, 0))
		parse_error(node->token, ERR_FN_EXPECTED_LPAREN, node->token->name);
	if (method && !node->token->is_pub) inject_self(node, owner);
	while (!ura.found_error && peek(0)->type != RPAR) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			break;
		}
		bool   is_ref  = find(REF, 0) != NULL;
		bool   is_null = is_ref && find(OPTIONAL, 0) != NULL;
		Token *param   = find(ID, 0);
		if (!param) {
			parse_error(node->token, ERR_FN_EXPECTED_PARAM_NAME, node->token->name);
			break;
		}
		parse_type(param);
		param->is_param    = true;
		param->is_dec      = true;
		param->is_ref      = is_ref;
		param->is_nullable = is_null;
		resize_array(node->token->Fn.params, Token *);
		node->token->Fn.params[node->token->Fn.params_count++] = param;
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, ERR_FN_EXPECTED_RPAREN, node->token->name);

	if(strcmp(node->token->name, "main") == 0) {
		node->token->ret_type = I32;
	}
	else {
		if (find(REF, 0)) {
			node->token->is_ref = true;
			if (find(OPTIONAL, 0)) node->token->is_nullable = true;
		}
		if (is_data_type(peek(0)) || peek(0)->type == ID)
			parse_type(node->token);
		else
			parse_error(node->token, ERR_FN_EXPECTED_RET_TYPE, node->token->name);
	}
	if (!node->token->is_proto) {
		if (!find(DOTS, 0))
			parse_error(node->token, ERR_FN_EXPECTED_COLON, node->token->name);
		parse_block(node, node->token->indent);
	}
	exit_scope();
	return node;
}

Node *fcall_node(Node *node) {
	node->token->type = FCALL;
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after %s", node->token->name);
	while (!ura.found_error && peek(0)->type != RPAR) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, ERR_CALL_EXPECTED_RPAREN, node->token->name);
	return node;
}

Node *access_node(Node *node) {
	while (peek(0)->type == LBRA || peek(0)->type == DOT) {
		if (peek(0)->type == DOT) {
			Token *dot    = next();
			Token *member = find(ID, 0);
			if (!member) {
				parse_error(dot, "Expected a member name after '.'");
				return syntax_error();
			}
			Node *dnode = new_node(dot);
			dnode->left = node;
			set_name(dnode->token, member->name);
			if (peek(0)->type == LPAR) {
				dnode = fcall_node(dnode);
				dnode->token->is_method_call = true;
			}
			if (find(OPTIONAL, 0))
				dnode->token->is_nullable = true;
			node = dnode;
			continue;
		}
		Token *bracket = next();
		bracket->type  = ACCESS;
		Node  *access  = new_node(bracket);
		access->left   = node;
		access->right  = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(bracket, "Expected ']' after array index");
		if (find(OPTIONAL, 0))
			bracket->is_nullable = true;
		node = access;
	}
	return node;
}

Node *array_ctor_node(Node *node) {
	Token *token   = node->token;
	bool   is_name = token->type == ID;
	Type   sub     = is_name ? STRUCT_CALL : token->type;
	if (is_name) token->Struct.name = token->name;
	token->type           = ARRAY;
	token->is_dec         = false;
	token->ret_type       = ARRAY_TYPE;
	token->Array.sub_type = sub;
	int depth = 0;
	while (peek(0)->type == LBRA) {
		find(LBRA, 0);
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(node->token, "Expected ']' after array size");
		depth++;
	}
	node->token->Array.depth = depth;
	return node;
}

void qualify_decl(Node *n, char *prefix) {
   Token *t = n->token;
   char  *q = format("%s.%s", prefix, t->name);
   set_name(t, q);
   free(q);
   if (!includes(t->type, STRUCT_DEF, ENUM_DEF, MODULE, 0)) return;
   for (int i = 0; i < n->children_count; i++) {
      if (t->type == STRUCT_DEF && n->children[i]->token->type != FDEC)
         continue;
      qualify_decl(n->children[i], prefix);
   }
}

Node *prime_node() {
   Token *token = next();
   switch(token->type)
   {
   case I32: case BOOL: case CHARS: case CHAR: case F32:
   case I64: case I16:
      if (token->is_dec && peek(0)->type == LBRA)
         return array_ctor_node(new_node(token));
      return new_node(token);
   case NULL_LIT: return new_node(token);
   case LBRA: {
      Node *node = new_node(token);
      node->token->type = ARRAY_LIT;
      while (!ura.found_error && peek(0)->type != RBRA) {
         resize_array(node->children, Node *);
         node->children[node->children_count++] = expr_node(0);
         while (find(COMA, 0));
      }
      if (!find(RBRA, 0))
         parse_error(node->token, "Expected ']' to close array literal");
      return postfix(node);
   }
   case LPAR: {
      Node *node = expr_node(0);
      if (!find(RPAR, 0))
         parse_error(token, "Expected ')' after expression");
      return postfix(node);
   }
   case PROTO: {
		if (peek(0)->type != FDEC) {
			parse_error(token, "Expected 'fn' after 'proto'");
         return syntax_error();
      }
      peek(0)->is_proto = true;
      return prime_node();
	}
   case OPERATOR: {
      Node *owner = ura.scope;
      if (!owner || owner->token->type != STRUCT_DEF) {
         parse_error(token, ERR_OPERATOR_OUTSIDE_STRUCT);
         return syntax_error();
      }
      if (peek(0)->type == ID && strcmp(peek(0)->name, "drop") == 0) {
         next();
         Node *node = new_node(token);
         node->token->type     = FDEC;
         node->token->ret_type = VOID;
         set_name(node->token, "drop");
         enter_scope(node);
         inject_self(node, owner);
         if (peek(0)->type == LPAR)
            parse_error(peek(0), ERR_DROP_NO_PARAMS);
         else if (!find(DOTS, 0) && peek(0)->type != DOTS)
            parse_error(peek(0), ERR_DROP_NO_RET);
         parse_block(node, node->token->indent);
         exit_scope();
         owner->token->has_drop = true;
         return node;
      }
      if (peek(0)->type == ID && strcmp(peek(0)->name, "output") == 0) {
         next();
         Node *node = new_node(token);
         set_name(node->token, "output");
         fdec_node(node);
         return node;
      }
      Token *op = find(ASSIGN, ADD, SUB, MUL, DIV, MOD, EQUAL, NOT_EQUAL,
                       LESS, GREAT, LESS_EQUAL, GREAT_EQUAL, ADD_ASSIGN,
                       SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
                       BAND, BOR, BXOR, LSHIFT, RSHIFT, BAND_ASSIGN,
                       BOR_ASSIGN, BXOR_ASSIGN, LSHIFT_ASSIGN,
                       RSHIFT_ASSIGN, 0);
      if (!op) {
         parse_error(token, ERR_OPERATOR_EXPECTED);
         return syntax_error();
      }
      Node *node = new_node(token);
      set_name(node->token, spell(op->type));
      fdec_node(node);
      Token *fn = node->token;
      if (fn->Fn.params_count > 1) {
         Token *rhs  = fn->Fn.params[1];
         char  *sub  = rhs->ret_type == STRUCT_CALL ? rhs->Struct.name
                                                    : type_name(rhs->ret_type);
 
         char  *pre  = rhs->is_ref ? "ref." : "";
         char  *full = format("%s.%s%s", fn->name, pre, sub);
         set_name(fn, full);
         free(full);
      }
      return node;
   }
   case PUB: {
      Node *owner = ura.scope;
      if (!owner || owner->token->type != STRUCT_DEF) {
         parse_error(token, ERR_PUB_OUTSIDE_STRUCT);
         return syntax_error();
      }
		if (peek(0)->type != FDEC) {
			parse_error(token, "Expected 'fn' after 'pub'");
         return syntax_error();
      }
      peek(0)->is_pub = true;
      return prime_node();
	}
   case FDEC: {
      Node *node = new_node(token);
      Token *fname = find(ID, 0);
      if (!fname) {
         parse_error(token, "Expected a function name after 'fn'");
         return syntax_error();
      }
      set_name(node->token, fname->name);
      return fdec_node(node);
   }
   case STRUCT_DEF: {
      Node *node = new_node(token);
      Token *sname = find(ID, 0);
      if (!sname) {
         parse_error(token, "Expected a struct name after 'struct'");
         return syntax_error();
      }
      set_name(node->token, sname->name);
      node->token->type = STRUCT_DEF;
      enter_scope(node);
      if (!find(DOTS, 0))
         parse_error(node->token, ERR_STRUCT_EXPECTED_COLON, node->token->name);
      parse_block(node, node->token->indent);
      if (!node->children_count)
         parse_error(node->token, ERR_STRUCT_EMPTY, node->token->name);
      for (int i = 0; i < node->children_count; i++) {
         Token *child = node->children[i]->token;
         if (child->type != FDEC) continue;
         char *qualified = format("%s.%s", node->token->name, child->name);
         set_name(child, qualified);
         free(qualified);
      }
      exit_scope();
      return node;
   }
   case ENUM_DEF: {
      Node *node = new_node(token);
      Token *ename = find(ID, 0);
      if (!ename) {
         parse_error(token, "Expected an enum name after 'enum'");
         return syntax_error();
      }
      set_name(node->token, ename->name);
      node->token->type = ENUM_DEF;
      if (!find(DOTS, 0))
         parse_error(node->token, ERR_ENUM_EXPECTED_COLON, node->token->name);
      int  line   = node->token->line;
      int  indent = node->token->indent;
      long value  = 0;
      while (!ura.found_error) {
         Token *peeked = peek(0);
         bool   inside = peeked->line == line || peeked->indent > indent;
         if (peeked->type != ID || !inside) break;
         Token *var     = find(ID, 0);
         var->is_dec    = true;
         var->is_global = true;
         var->type      = ENUM_CALL;
         if (find(ASSIGN, 0)) {
            bool   neg = find(SUB, 0) != NULL;
            Token *lit = find(I32, 0);
            if (!lit) {
               parse_error(var, ERR_ENUM_VALUE_INT, var->name);
               break;
            }
            value = neg ? -lit->Int.value : lit->Int.value;
         }
         var->Int.value = value;
         value++;
         resize_array(node->children, Node *);
         node->children[node->children_count++] = new_node(var);
         if (!find(COMA, 0)) break;
      }
      if (!node->children_count)
         parse_error(node->token, ERR_ENUM_EMPTY, node->token->name);
      return node;
   }
   case MODULE: {
      Node  *node  = new_node(token);
      Token *mname = find(ID, 0);
      if (!mname) {
         parse_error(token, "Expected a module name after 'mod'");
         return syntax_error();
      }
      set_name(node->token, mname->name);
      node->token->type = MODULE;
      enter_scope(node);
      if (!find(DOTS, 0))
         parse_error(node->token, ERR_MOD_EXPECTED_COLON, node->token->name);
      parse_block(node, node->token->indent);
      exit_scope();
      if (!node->children_count)
         parse_error(node->token, ERR_MOD_EMPTY, node->token->name);
      for (int i = 0; i < node->children_count; i++) {
         Token *ct = node->children[i]->token;
         if (!includes(ct->type, FDEC, STRUCT_DEF, ENUM_DEF, MODULE, 0)) {
            parse_error(ct, ERR_MOD_BODY);
            continue;
         }
         qualify_decl(node->children[i], node->token->name);
      }
      return node;
   }
   case ID: {
		Node *node = new_node(token);
		Token *token = node->token;
		if (peek(0)->type == LPAR) {
			if (strcmp(token->name, "main") == 0)
				return fdec_node(node);
			bool is_err = strcmp(token->name, "errput") == 0;
			if (is_err || strcmp(token->name, "output") == 0) {
				char *who = token->name;
				node->token->type     = is_err ? ERRPUT : OUTPUT;
				node->token->ret_type = VOID;
				if (!find(LPAR, 0))
					parse_error(node->token, "Expected '(' after %s", who);
				while (!ura.found_error && peek(0)->type != RPAR) {
					resize_array(node->children, Node *);
					node->children[node->children_count++] = expr_node(0);
					while (find(COMA, 0));
				}
				if (!find(RPAR, 0))
					parse_error(node->token, "Expected ')' after %s arguments", who);
				return node;
			}
			return postfix(fcall_node(node));
		}
		if (peek(0)->type == DOUBLE_DOTS) {
			char  *path = strdup(token->name);
			Token *sep  = NULL;
			while ((sep = find(DOUBLE_DOTS, 0))) {
				Token *member = find(ID, 0);
				if (!member) {
					parse_error(sep, "Expected a name after '::'");
					free(path);
					return syntax_error();
				}
				char *joined = format("%s.%s", path, member->name);
				free(path);
				path = joined;
			}
			if (peek(0)->type != LPAR) {
				set_name(token, path);
				free(path);
				return postfix(node);
			}
			char *dot = strrchr(path, '.');
			*dot = '\0';
			token->Struct.name    = strdup(path);
			token->is_static_call = true;
			set_name(token, dot + 1);
			free(path);
			return postfix(fcall_node(node));
		}
		if (peek(0)->type == LBRA || peek(0)->type == DOT)
			return access_node(node);
		bool named   = peek(0)->type == ID && peek(0)->line == token->line;
		bool fn_type = peek(0)->type == FDEC && peek(1)->type == LPAR;
		if (is_data_type(peek(0)) || named || fn_type) {
			token->is_dec = true;
			parse_type(token);
		}
		if (!token->is_dec && peek(0)->type == OPTIONAL) {
			find(OPTIONAL, 0);
			token->is_nullable = true;
			return postfix(node);
		}
		return node;
   }
   case NEW: {
      Token *type = next();
      bool   ok   = is_data_type(type) || type->type == ID;
      if (!ok) {
         parse_error(token, ERR_NEW_EXPECTED_ARRAY_TYPE);
         return syntax_error();
      }
      if (peek(0)->type != LBRA) {
         if (type->type != ID) {
            parse_error(token, ERR_NEW_EXPECTED_ARRAY_TYPE);
            return syntax_error();
         }
         type->type        = NEW;
         type->ret_type    = STRUCT_CALL;
         type->Struct.name = type->name;
         type->is_dec      = false;
         type->is_heap     = true;
         return postfix(new_node(type));
      }
      Node *arr = array_ctor_node(new_node(type));
      arr->token->is_heap = true;
      return arr;
   }
   case CLEAN: {
      Node *clean = new_node(token);
      clean->left = expr_node(0);
      return clean;
   }
   case TYPEOF: case SIZEOF: {
      char *kw   = token->type == TYPEOF ? "typeof" : "sizeof";
      Node *node = new_node(token);
      if (!find(LPAR, 0))
         parse_error(token, "Expected '(' after '%s'", kw);
      if (is_data_type(peek(0))) {
         Token *arg    = next();
         arg->ret_type = arg->type;
         node->left    = new_node(arg);
      } else
         node->left = expr_node(0);
      if (!find(RPAR, 0))
         parse_error(token, "Expected ')' to close '%s'", kw);
      return node;
   }
   case RETURN: {
      Node *node = new_node(token);
      if (peek(0)->line == token->line)
         node->left = expr_node(0);
      return node;
   }
   case NOT: case BNOT: {           
      Node *node = new_node(token);
      node->left = prime_node();
      return node;
   }
   case SUB: {
      Node *node   = new_node(token);
      node->right  = prime_node();
      Type kind    = node->right->token->type == F32 ? F32 : I32;
      node->left   = new_node(new_token(kind, token->indent));
      return node;
   }
   case REF: {
		Node *node = new_node(token);
		Token *token = node->token;
		bool nullable = peek(0)->type == OPTIONAL;
		if (nullable) find(OPTIONAL, 0);
		Token *name = peek(0);
		if (!name || name->type != ID) {
			parse_error(token, "Expected a variable after 'ref'");
			return syntax_error();
		}
		bool fn_type = peek(1)->type == FDEC && peek(2)->type == LPAR;
		bool named   = peek(1)->type == ID && peek(1)->line == name->line;
		if (is_data_type(peek(1)) || named || fn_type) {
			find(ID, 0);
			name->is_dec      = true;
			name->is_ref      = true;
			name->is_nullable = nullable;
			parse_type(name);
			if (!nullable && peek(0)->type != ASSIGN) {
				parse_error(name, ERR_REF_MUST_BE_BOUND);
				return syntax_error();
			}
			node->token = name;
			return node;
		}
		node->left = prime_node();
		return node;
   }
   case IF: {
		Node *node = new_node(token);
		Token *token = node->token;
		node->left = expr_node(0);
		if (!find(DOTS, 0))
			parse_error(token, "Expected ':' to open the 'if' body");
		parse_block(node, token->indent);
		Node *tail = node;
		while (includes(peek(0)->type, ELIF, ELSE, 0) && peek(0)->indent == token->indent) {
			Token *keyword = next();
			Node  *branch  = new_node(keyword);
			if (keyword->type == ELIF)
				branch->left = expr_node(0);
			if (!find(DOTS, 0))
				parse_error(keyword, ERR_EXPECTED_BODY_COLON, keyword->name);
			parse_block(branch, keyword->indent);
			tail->right = branch;
			tail        = branch;
			if (keyword->type == ELSE) break;
		}
		return node;
   }
   case ELIF: case ELSE:
      parse_error(token, "'%s' without a matching 'if'", token->name);
      return syntax_error();
   case MATCH: {
		Node *node = new_node(token);
		Token *token = node->token;
		node->left = expr_node(0);
		if (!find(DOTS, 0))
			parse_error(token, "Expected ':' to open the 'match' body");
		while (within(token->indent) && includes(peek(0)->type, CASE, DEFAULT, 0)) {
			Token *keyword = next();
			Node  *branch  = new_node(keyword);
			if (keyword->type == CASE) {
				Node *values = new_node(keyword);
				while (!ura.found_error && peek(0)->type != DOTS) {
					resize_array(values->children, Node *);
					values->children[values->children_count++] = expr_node(0);
					if (!find(COMA, 0)) break;
				}
				if (values->children_count == 0)
					parse_error(keyword, "Expected an expression after 'case'");
				branch->left = values;
			}
			if (!find(DOTS, 0))
				parse_error(keyword, ERR_EXPECTED_BODY_COLON, keyword->name);
			parse_block(branch, keyword->indent);
			resize_array(node->children, Node *);
			node->children[node->children_count++] = branch;
			if (keyword->type == DEFAULT) break;
		}
		return node;
   }
   case CASE: case DEFAULT:
      parse_error(token, "'%s' without a matching 'match'", token->name);
      return syntax_error();
   case WHILE: {
		Node *node = new_node(token);
		Token *token = node->token;
		node->left = expr_node(0);
		if (!find(DOTS, 0))
			parse_error(token, "Expected ':' to open the 'while' body");
		parse_block(node, token->indent);
		return node;
   }
   case LOOP: {
      Node *node = new_node(token);
      if (!find(DOTS, 0))
         parse_error(token, "Expected ':' to open the 'loop' body");
      parse_block(node, token->indent);
      return node;
   }
   case FOR: {
		Node *node = new_node(token);
		Token *token = node->token;
		bool   ref   = find(REF, 0) != NULL;
		Token *iter  = find(ID, 0);
		if (!iter) {
			parse_error(token, "Expected a loop variable after 'for'");
			return syntax_error();
		}
		iter->is_dec  = true;
		token->is_ref = ref;
		node->left    = new_node(iter);
		if (!find(IN, 0))
			parse_error(token, "Expected 'in' after 'for %s'", iter->name);
		node->right = expr_node(0);
		if (find(BY, 0)) {
			Node *by = expr_node(0);
			if (node->right->token->type != RANGE)
				parse_error(token, ERR_BY_NEEDS_RANGE);
			else {
				resize_array(node->right->children, Node *);
				node->right->children[node->right->children_count++] = by;
			}
		}
		if (!find(DOTS, 0))
			parse_error(token, "Expected ':' to open the 'for' body");
		parse_block(node, token->indent);
		return node;
   }
   case TRY: {
		Node *node = new_node(token);
		Token *token = node->token;
		if (!find(DOTS, 0))
			parse_error(token, "Expected ':' to open the 'try' body");
		parse_block(node, token->indent);
		if (peek(0)->type != CATCH || peek(0)->indent != token->indent) {
			parse_error(token, "Expected 'catch' after 'try'");
			return node;
		}
		Token *ckw   = next();
		Node  *cnode = new_node(ckw);
		Token *bind  = find(ID, 0);
		if (!bind) {
			parse_error(ckw, "Expected a variable after 'catch'");
			return node;
		}
		bind->is_dec = true;
		cnode->left  = new_node(bind);
		if (!find(DOTS, 0))
			parse_error(ckw, "Expected ':' to open the 'catch' body");
		parse_block(cnode, ckw->indent);
		node->right = cnode;
		return node;
   }
   case CATCH:
      parse_error(token, "'catch' without a matching 'try'");
      return syntax_error();
   case THROW: {
		Node *node = new_node(token);
		node->left = expr_node(0);
		return node;
   }
   case BREAK: case CONTINUE:
      return new_node(token);
   default:
      break;
   }
   parse_error(token, "Unexpected token");
   return syntax_error();
}

Node *expr_node(int min_op) {
   Node *left = prime_node();

   while (true)
   {
      static const int res[END + 1] = {
         [ASSIGN] = 1,     [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
         [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1, [MOD_ASSIGN] = 1,
         [BAND_ASSIGN] = 1, [BOR_ASSIGN] = 1, [BXOR_ASSIGN] = 1,
         [LSHIFT_ASSIGN] = 1, [RSHIFT_ASSIGN] = 1,
         [FALLBACK] = 2,   [OR] = 2,         [AND] = 3,
         [RANGE] = 2,
         [BOR] = 4,        [BXOR] = 5,       [BAND] = 6,
         [LESS] = 8,       [EQUAL] = 7,      [NOT_EQUAL] = 7,
         [GREAT] = 8,      [LESS_EQUAL] = 8, [GREAT_EQUAL] = 8,
         [ADD] = 10,       [LSHIFT] = 9,     [RSHIFT] = 9,
         [SUB] = 10,       [MUL] = 11,       [DIV] = 11,
         [AS] = 12,        [MOD] = 11,
      };
      int op = res[ura.tokens[ura.exe_pos]->type];
      if(op <= min_op) break;
      Node *node = new_node(next());
      node->left = left;
      bool is_cast  = node->token->type == AS;
      bool is_chain = node->token->type == FALLBACK;
      if (is_cast) {
         Token *type_tok = new_token(ID, 0);
         parse_type(type_tok);
         node->right = new_node(type_tok);
      } else
         node->right = expr_node(is_chain ? op - 1 : op);
      left = node;
   }
   return left;
}

/* ---- backend/analyze.c ---- */
void enter_scope(Node *node) {
	ura.scopes_count++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, ura.scopes_count);
	resize_array(ura.scopes, Node *);
	ura.scopes[ura.scopes_count] = node;
	ura.scope                = ura.scopes[ura.scopes_count];
}

void exit_scope() {
	if (CHECK(ura.scopes_count < 0, "No active scope to exit")) return;
	debug(CYAN("Exit Scope: %k index %d\n"), ura.scopes[ura.scopes_count]->token, ura.scopes_count);
	ura.scopes[ura.scopes_count] = NULL;
	ura.scopes_count--;
	ura.scope = ura.scopes[ura.scopes_count];
}

void declare_structs(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		if (node->children[i]->token->type == STRUCT_DEF)
			declare_struct(node->children[i]);
}

void declare_variable(Token *token) {
	Node *scope     = ura.scope;
	bool  in_struct = scope->token->type == STRUCT_DEF;
	char *err = in_struct ? ERR_STRUCT_DUP_FIELD : ERR_REDECL_VARIABLE;
	for (int i = 0; i < scope->variables_count; i++)
		if (strcmp(scope->variables[i]->name, token->name) == 0) {
			parse_error(token, err, token->name);
			return;
		}
	resize_array(scope->variables, Token *);
	scope->variables[scope->variables_count++] = token;
}

Token *find_variable(char *name, bool *captured, bool *from_field) {
	bool passed_function = false;
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->variables_count; j++) {
			char *current = scope->variables[j]->name;
			if (!current || strcmp(current, name) != 0) continue;
			bool from_outer_fn = passed_function && scope->token->type == FDEC;
			if (captured)   *captured   = from_outer_fn;
			if (from_field) *from_field = scope->token->type == STRUCT_DEF;
			return scope->variables[j];
		}
		if (scope->token->type == FDEC) passed_function = true;
	}
	if (captured)   *captured   = false;
	if (from_field) *from_field = false;
	return NULL;
}

char *signature_diff(Token *old, Token *new) {
	if (old->Fn.params_count != new->Fn.params_count)
		return format("expected %d parameter(s), found %d",
		              old->Fn.params_count, new->Fn.params_count);
	for (int i = 0; i < old->Fn.params_count; i++) {
		Type a = old->Fn.params[i]->ret_type;
		Type b = new->Fn.params[i]->ret_type;
		if (!same_or_pointer(a, b))
			return format("parameter %d is %s, expected %s", i + 1, type_name(b), type_name(a));
	}
	if (!same_or_pointer(old->ret_type, new->ret_type))
		return format("returns %s, expected %s", type_name(new->ret_type), type_name(old->ret_type));
	return format("one is variadic, the other is not");
}

bool same_signature(Token *a, Token *b) {
	if (!same_or_pointer(a->ret_type, b->ret_type)) return false;
	if (a->is_variadic != b->is_variadic) return false;
	if (a->Fn.params_count != b->Fn.params_count) return false;
	for (int i = 0; i < a->Fn.params_count; i++)
		if (!same_or_pointer(a->Fn.params[i]->ret_type,  b->Fn.params[i]->ret_type))
			return false;
	return true;
}

void declare_function(Node *fn) {
	Token *new = fn->token;
	for (int i = 0; i < ura.scope->functions_count; i++) {
		Token *old = ura.scope->functions[i]->token;
		if (strcmp(old->name, new->name) != 0) continue;
		if (!old->is_proto && !new->is_proto) {
			parse_error(new, ERR_REDECL_FUNCTION, new->name);
			parse_note(old, NOTE_PREV_DECL, new->name);
			return;
		}
		if (!same_signature(old, new)) {
			parse_error(new, ERR_SIG_CONFLICT, new->name, signature_diff(old, new));
			parse_note(old, NOTE_PREV_DECL, new->name);
			return;
		}
		if (old->is_proto && !new->is_proto)
			ura.scope->functions[i] = fn;
		return;
	}
	resize_array(ura.scope->functions, Node *);
	ura.scope->functions[ura.scope->functions_count++] = fn;
}

void declare_struct(Node *node) {
	Token *token = node->token;
	for (int i = 0; i < ura.scope->structs_count; i++)
		if (strcmp(ura.scope->structs[i]->token->name, token->name) == 0) {
			parse_error(token, "Redeclaration of struct '%s'", token->name);
			return;
		}
	resize_array(ura.scope->structs, Node *);
	ura.scope->structs[ura.scope->structs_count++] = node;
}

void declare_enum(Node *node) {
	Token *token = node->token;
	for (int i = 0; i < ura.scope->structs_count; i++)
		if (strcmp(ura.scope->structs[i]->token->name, token->name) == 0) {
			parse_error(token, "Redeclaration of type '%s'", token->name);
			return;
		}
	resize_array(ura.scope->structs, Node *);
	ura.scope->structs[ura.scope->structs_count++] = node;
	for (int i = 0; i < node->children_count; i++)
		declare_variable(node->children[i]->token);
}

Node *lookup_struct(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->structs_count; j++) {
			char *current = scope->structs[j]->token->name;
			if (current && strcmp(current, name) == 0)
				return scope->structs[j];
		}
	}
	return NULL;
}

Node *lookup_function(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->functions_count; j++) {
			char *current = scope->functions[j]->token->name;
			if (current && strcmp(current, name) == 0)
				return scope->functions[j];
		}
	}
	return NULL;
}

Node *find_struct(char *name) {
	if (ura.current_module) {
		char *pre = strdup(ura.current_module);
		while (1) {
			char *q   = format("%s.%s", pre, name);
			Node *hit = lookup_struct(q);
			free(q);
			if (hit) { free(pre); return hit; }
			char *dot = strrchr(pre, '.');
			if (!dot) break;
			*dot = '\0';
		}
		free(pre);
	}
	return lookup_struct(name);
}

Node *find_function(char *name) {
	if (ura.current_module) {
		char *pre = strdup(ura.current_module);
		while (1) {
			char *q   = format("%s.%s", pre, name);
			Node *hit = lookup_function(q);
			free(q);
			if (hit) { free(pre); return hit; }
			char *dot = strrchr(pre, '.');
			if (!dot) break;
			*dot = '\0';
		}
		free(pre);
	}
	return lookup_function(name);
}

Node *find_module_in(Node *parent, char *name) {
	for (int i = 0; i < parent->children_count; i++) {
		Node *c = parent->children[i];
		if (c->token->type != MODULE) continue;
		if (strcmp(c->token->name, name) == 0) return c;
		Node *hit = find_module_in(c, name);
		if (hit) return hit;
	}
	return NULL;
}

Node *find_module(char *name) {
	return ura.head ? find_module_in(ura.head, name) : NULL;
}

void analyze_binop(Node *node) {
   analyze(node->left);
   analyze(node->right);
}

void resolve_struct_type(Token *token) {
   bool array = token->ret_type == ARRAY_TYPE;
   Type type  = array ? token->Array.sub_type : token->ret_type;
   if (type != STRUCT_CALL) return;
   Node *def = find_struct(token->Struct.name);
   if (!def) {
      parse_error(token, ERR_UNKNOWN_TYPE, token->Struct.name);
      return;
   }
   if (def->token->type == ENUM_DEF) {
      if (array) token->Array.sub_type = I32;
      else       token->ret_type       = I32;
      return;
   }
   if (array) token->Array.struct_ptr = def;
   else       token->Struct.ptr       = def;
}

bool rewrite_struct_ctor(Node *node) {
   Node *base  = node;
   int   depth = 0;
   while (base->token->type == ACCESS) { base = base->left; depth++; }
   Token *name = base->token;
   if (name->type != ID || name->is_dec) return false;
   if (find_variable(name->name, NULL, NULL)) return false;
   Node *def = find_struct(name->name);
   if (!def) return false;
   Node **dims = allocate(depth, sizeof(Node *));
   Node  *cur  = node;
   for (int i = depth - 1; i >= 0; i--) {
      dims[i] = cur->right;
      cur     = cur->left;
   }
   Token *token           = node->token;
   token->type            = ARRAY;
   token->ret_type        = ARRAY_TYPE;
   token->Struct.name     = name->name;
   token->Array.sub_type  = STRUCT_CALL;
   token->Array.depth     = depth;
   token->Array.struct_ptr = def;
   node->left             = NULL;
   node->right            = NULL;
   node->children         = dims;
   node->children_count   = depth;
   node->children_size    = depth;
   return true;
}

Token *global_decl(Node *child) {
   Token *token = child->token;
   if (token->type == ID && token->is_dec) return token;
   if (token->type == ASSIGN && child->left->token->is_dec)
      return child->left->token;
   return NULL;
}

Node *find_method(Node *def, char *name) {
   for (int i = 0; i < def->children_count; i++) {
      Node *child = def->children[i];
      if (child->token->type != FDEC) continue;
      if (strcmp(child->token->name, name) == 0) return child;
   }
   return NULL;
}

void analyze(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         Token *token = node->token;
         resolve_struct_type(token);
         enter_scope(node);
         for (int i = 0; i < token->Fn.params_count; i++) {
            declare_variable(token->Fn.params[i]);
            resolve_struct_type(token->Fn.params[i]);
         }
         for (int i = 0; i < node->children_count; i++)
            if (node->children[i]->token->type == FDEC)
               declare_function(node->children[i]);
         declare_structs(node);
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         exit_scope();
         break;
      }
      case STRUCT_DEF: {
         if (token->has_drop) {
            char *prefix = format("%s.=", token->name);
            size_t n     = strlen(prefix);
            bool  found  = false;
            for (int i = 0; i < node->children_count && !found; i++) {
               Token *child = node->children[i]->token;
               if (child->type != FDEC || !child->name) continue;
               found = strncmp(child->name, prefix, n) == 0;
            }
            free(prefix);
            char *dname = format("%s.drop", token->name);
            bool  muted = false;
            for (int i = 0; i < node->children_count && !muted; i++) {
               Token *child = node->children[i]->token;
               if (child->type != FDEC || !child->name) continue;
               if (strcmp(child->name, dname) == 0) muted = child->no_warn;
            }
            free(dname);
            if (!found && !muted)
               parse_warn(token, WARN_DROP_NEEDS_ASSIGN, token->name);
         }
         enter_scope(node);
         for (int i = 0; i < node->children_count; i++)
            if (node->children[i]->token->type == FDEC)
               declare_function(node->children[i]);
         for (int i = 0; i < node->children_count; i++) {
            Node *field = node->children[i];
            if (field->token->type == STRUCT_DEF) declare_struct(field);
            analyze(field);
         }
         exit_scope();
         break;
      }
      case ENUM_DEF: break;
      case ID: {
         Token *token = node->token;
         if (token->is_dec) {
            if (!token->is_global) declare_variable(token);
            resolve_struct_type(token);
            return;
         }
         bool   captured = false;
         bool   member   = false;
         Token *decl     = find_variable(token->name, &captured, &member);
         if (decl && decl->type == ENUM_CALL) {
            token->type      = I32;
            token->ret_type  = I32;
            token->Int.value = decl->Int.value;
            decl->used++;
            return;
         }
         if (decl && member) {
            char *nm = token->name;
            if (find_variable("self", NULL, NULL))
               parse_error(token, ERR_BARE_FIELD, nm, nm);
            else
               parse_error(token, ERR_UNDECLARED_VARIABLE, nm);
            return;
         }
         if (decl) {
            if (captured) {
               parse_error(token, ERR_CAPTURE_NOT_ALLOWED, token->name);
               return;
            }
            decl->used++;
            token->Decl.ptr    = decl;
            token->ret_type    = decl->ret_type;
            token->is_optional = decl->is_optional;
            token->is_ref      = decl->is_ref;
            if (decl->ret_type == FN_TYPE) token->Fn = decl->Fn;
            if (decl->ret_type == ARRAY_TYPE) token->Array = decl->Array;
            if (decl->ret_type == STRUCT_CALL)
               token->Struct = decl->Struct;
            return;
         }
         Node *fn = find_function(token->name);
         if (!fn) {
            if (find_module(token->name))
               parse_error(token, ERR_MOD_DOT_CALL, token->name);
            else
               parse_error(token, ERR_UNDECLARED_VARIABLE, token->name);
            return;
         }
         token->type             = FN_TYPE;
         token->ret_type         = FN_TYPE;
         token->Fcall.ptr        = fn;
         token->Fn.params        = fn->token->Fn.params;
         token->Fn.params_count  = fn->token->Fn.params_count;
         token->Fn.ret           = new_token(ID, 0);
         token->Fn.ret->ret_type = fn->token->ret_type;
         break;
      }
      case I32: case BOOL: case CHARS: case CHAR: case F32: break;
      case NULL_LIT: break;
      case FALLBACK: analyze_binop(node); break;
      case RETURN: analyze(node->left); break;
      case FCALL: {
         Token *token = node->token;
         if (token->is_method_call) {
            analyze(node->left);
            for (int i = 0; i < node->children_count; i++)
               analyze(node->children[i]);
            return;
         }
         if (token->is_static_call) {
            for (int i = 0; i < node->children_count; i++)
               analyze(node->children[i]);
            return;
         }
         Token *var = find_variable(token->name, NULL, NULL);
         if (var && var->ret_type == FN_TYPE) {
            token->Fcall.var = var;
            for (int i = 0; i < node->children_count; i++)
               analyze(node->children[i]);
            token->ret_type = var->Fn.ret->ret_type;
            return;
         }
         Node *fn = find_function(token->name);
         if (!fn) {
            parse_error(token, "Undeclared function '%s'", token->name);
            return;
         }
         token->Fcall.ptr = fn;
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         token->ret_type = fn->token->ret_type;
         if (token->ret_type == STRUCT_CALL)
            token->Struct = fn->token->Struct;
         break;
      }
      case NOT: case BNOT: analyze(node->left); break;
      case AS: analyze(node->left); break;
      case REF: {
         analyze(node->left);
         Type k = node->left->token->type;
         if (!includes(k, ID, ACCESS, DOT, 0))
            parse_error(node->token, ERR_REF_TO_NON_VARIABLE);
         break;
      }
      case IF: case ELIF: case ELSE: case WHILE: case LOOP: {
         analyze(node->left);
         enter_scope(node);
         declare_structs(node);
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         exit_scope();
         analyze(node->right);
         break;
      }
      case FOR: {
         enter_scope(node);
         analyze(node->right);
         Token *var = node->left->token;
         Token *it  = node->right->token;
         if (it->type != RANGE && !it->ret_type) type_check(node->right);
         for (int i = 0; i < node->right->children_count; i++)
            analyze(node->right->children[i]);
         if (it->type == RANGE)
            var->ret_type = I32;
         else if (it->ret_type == ARRAY_TYPE) {
            var->Array       = it->Array;
            var->Array.depth = it->Array.depth - 1;
            if (it->Array.depth > 1) var->ret_type = ARRAY_TYPE;
            else var->ret_type = it->Array.sub_type;
            if (var->ret_type == STRUCT_CALL)
               var->Struct.ptr = it->Array.struct_ptr;
         }
         declare_variable(var);
         declare_structs(node);
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         exit_scope();
         break;
      }
      case MATCH: {
         analyze(node->left);
         enter_scope(node);
         for (int i = 0; i < node->children_count; i++) {
            Node *branch = node->children[i];
            if (branch->left)
               for (int j = 0; j < branch->left->children_count; j++)
                  analyze(branch->left->children[j]);
            enter_scope(branch);
            declare_structs(branch);
            for (int j = 0; j < branch->children_count; j++)
               analyze(branch->children[j]);
            exit_scope();
         }
         exit_scope();
         break;
      }
      case MODULE: {
         char *prev = ura.current_module;
         ura.current_module = node->token->name;
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         ura.current_module = prev;
         break;
      }
      case TRY: {
         enter_scope(node);
         declare_structs(node);
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         exit_scope();
         Node  *cnode = node->right;
         Token *bind  = cnode->left->token;
         bind->ret_type    = STRUCT_CALL;
         bind->Struct.name = "Error";
         resolve_struct_type(bind);
         enter_scope(cnode);
         declare_variable(bind);
         declare_structs(cnode);
         for (int i = 0; i < cnode->children_count; i++)
            analyze(cnode->children[i]);
         exit_scope();
         break;
      }
      case THROW: analyze(node->left); break;
      case BREAK: {
         node->left = NULL;
         for (int i = ura.scopes_count; i >= 1; i--) {
            Node *loop = ura.scopes[i];
            if (!loop) continue;
            if (includes(loop->token->type, WHILE, LOOP, FOR, MATCH, 0)) {
               node->left = loop;
               break;
            }
            if (loop->token->type == FDEC) break;
         }
         if (!node->left)
         parse_error(node->token, "'break' outside a loop or match");
         break;
      }
      case CONTINUE: {
         node->left = NULL;
         for (int i = ura.scopes_count; i >= 1; i--) {
            Node *loop = ura.scopes[i];
            if (!loop) continue;
            if (includes(loop->token->type, WHILE, LOOP, FOR, 0)) {
               node->left = loop;
               break;
            }
            if (loop->token->type == FDEC) break;
         }
         if (!node->left)
         parse_error(node->token, "'continue' outside a loop");
         break;
      }
      case ERRPUT:
      case OUTPUT: {
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case ARRAY_LIT: {
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case ACCESS: {
         if (!rewrite_struct_ctor(node)) { 
            analyze_binop(node); 
            break; 
         }
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case RANGE:  analyze_binop(node); break;
      case DOT:    analyze(node->left); break;
      case TYPEOF: case SIZEOF:
         if (!is_data_type(node->left->token)) analyze(node->left);
         break;
      case ARRAY: {
         resolve_struct_type(token);
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case CLEAN: analyze(node->left); break;
      case NEW: resolve_struct_type(token); break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
      case MOD_ASSIGN: case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
      case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         analyze_binop(node);
         break;
      default:
         CHECK(1, "analyze: unhandled node '%s'", to_string(token->type));
         break;
   }
}

/* ---- backend/typecheck.c ---- */
bool is_float(Type type) {
	return includes(type, FLOAT_TYPES, 0);
}

bool is_unsigned(Type type) {
	return includes(type, UNSIGNED_TYPES, BOOL, 0);
}

bool is_pointer(Type type) {
	return includes(type, POINTER_TYPES, 0);
}

bool same_or_pointer(Type a, Type b) {
	return a == b || (is_pointer(a) && is_pointer(b));
}

void set_string_type(Token *token) {
	token->ret_type       = ARRAY_TYPE;
	token->Array.sub_type = CHAR;
	token->Array.depth    = 1;
}

bool is_null(Token *token) {
	return token->type == NULL_LIT;
}

bool is_nullable(Token *t) {
	return t->is_optional || t->is_ref;
}

bool is_assignable(Token *lhs, Token *rhs) {
	return is_nullable(lhs) || !rhs->is_optional;
}

bool is_string(Token *token) {
	return token->ret_type == ARRAY_TYPE && token->Array.depth == 1
	       && token->Array.sub_type == CHAR;
}

bool is_castable(Type type) {
	return is_float(type) || includes(type, NUMERIC_TYPES, 0);
}

bool is_data_type(Token *token) {
	return token->is_dec && includes(token->type, DATA_TYPES, 0); 
}

TypeRef to_llvm_type(Type type) {
   switch (type) {
   case I8:  case U8:  return ura.i8;
   case I16: case U16: return ura.i16;
   case I32: case U32: return ura.i32;
   case I64: case U64: return ura.i64;
   case CHAR:          return ura.i8;
   case F32:           return ura.f32;
   case F64:           return ura.f64;
   case BOOL:          return ura.i1;
   case VOID:          return ura.vd;
   case PTR:           return LLVMPointerType(ura.i8, 0);
   default: TODO(1, "to_llvm_type: unhandled type %t", type); return NULL;
   }
}

TypeRef elem_type(Token *arr, int depth) {
   if (depth > 1) return array_type(arr, depth - 1);
   if (arr->Array.sub_type == STRUCT_CALL)
      return struct_type_of(arr->Array.struct_ptr);
   return to_llvm_type(arr->Array.sub_type);
}

TypeRef array_type(Token *arr, int depth) {
   TypeRef fields[2] = { pointer_to(elem_type(arr, depth)), ura.i64 };
   return LLVMStructTypeInContext(ura.context, fields, 2, 0);
}

TypeRef struct_type_of(Node *def) {
   if (!def) return NULL;
   Token *token = def->token;
   if (token->llvm.struct_type) return token->llvm.struct_type;
   TypeRef type = LLVMStructCreateNamed(ura.context, token->name);
   token->llvm.struct_type = type;
   TypeRef *fields = allocate(def->children_count, sizeof(TypeRef));
   int      n      = 0;
   for (int i = 0; i < def->children_count; i++) {
      Node  *child = def->children[i];
      Token *field = child->token;
      if (field->type == STRUCT_DEF) { struct_type_of(child); continue; }
      if (!is_field(field)) continue;
      TypeRef t = llvm_type_of(field);
      fields[field->Struct.index] = field->is_ref ? pointer_to(t) : t;
      n++;
   }
   LLVMStructSetBody(type, fields, n, 0);
   free(fields);
   return type;
}

TypeRef llvm_type_of(Token *token) {
   if (token->ret_type == STRUCT_CALL)
      return struct_type_of(token->Struct.ptr);
   if (token->ret_type == ARRAY_TYPE)
      return array_type(token, token->Array.depth);
   if (token->ret_type != FN_TYPE) 
      return to_llvm_type(token->ret_type);
   int      n      = token->Fn.params_count;
   TypeRef *params = n ? allocate(n, sizeof(TypeRef)) : NULL;
   for (int i = 0; i < n; i++) params[i] = llvm_type_of(token->Fn.params[i]);
   TypeRef ft = LLVMFunctionType(llvm_type_of(token->Fn.ret), params, n, 0);
   free(params);
   return LLVMPointerType(ft, 0);
}

Value default_value(Token *token) {
   if (includes(token->ret_type, NUMERIC_TYPES, 0)) 
      return LLVMConstInt(llvm_type_of(token), 0, false);
   if (is_float(token->ret_type))
      return LLVMConstReal(llvm_type_of(token), 0.0);
   return LLVMConstNull(llvm_type_of(token));
}

void type_check_fcall(Node *node) {
   Token *token = node->token;
   if (token->is_method_call && !token->Fcall.ptr) {
      Token *token = node->token;
      type_check(node->left);
      Token *recv = node->left->token;
      if (recv->ret_type != STRUCT_CALL || !recv->Struct.ptr) {
         parse_error(token, ERR_METHOD_ON_NON_STRUCT, token->name, type_name(recv->ret_type));
         return;
      }
      Node *def       = recv->Struct.ptr;
      char *qualified = format("%s.%s", def->token->name, token->name);
      Node *fn        = find_method(def, qualified);
      free(qualified);
      if (!fn) {
         parse_error(token, ERR_UNKNOWN_METHOD, def->token->name, token->name);
         return;
      }
      if (def->token->has_drop && fn == find_destructor(def)) {
         parse_error(token, ERR_CANNOT_CALL_DROP, def->token->name);
         return;
      }
      if (fn->token->is_pub) {
         char *self = recv->name ? recv->name : def->token->name;
         parse_error(token, ERR_PUB_IS_STATIC, self, token->name, def->token->name, token->name);
         return;
      }
      token->Fcall.ptr = fn;
      type_check_fcall(node);
      if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
      return;
   }
   if (token->is_static_call && !token->Fcall.ptr) {
      Token *token = node->token;
      char *mq = format("%s.%s", token->Struct.name, token->name);
      Node *mf = find_function(mq);
      free(mq);
      if (mf) {
         token->Fcall.ptr = mf;
         type_check_fcall(node);
         if (token->ret_type == STRUCT_CALL) token->Struct = mf->token->Struct;
         return;
      }
      Node  *def   = find_struct(token->Struct.name);
      if (!def) {
         if (find_module(token->Struct.name))
            parse_error(token, ERR_MOD_NO_MEMBER, token->Struct.name,
                        token->name);
         else
            parse_error(token, ERR_UNKNOWN_TYPE, token->Struct.name);
         return;
      }
      char *qualified = format("%s.%s", def->token->name, token->name);
      Node *fn        = find_method(def, qualified);
      free(qualified);
      if (!fn) {
         parse_error(token, ERR_UNKNOWN_METHOD, def->token->name, token->name);
         return;
      }
      if (!fn->token->is_pub) {
         parse_error(token, ERR_METHOD_NEEDS_RECEIVER, def->token->name, token->name);
         return;
      }
      token->Fcall.ptr = fn;
      type_check_fcall(node);
      if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
      return;
   }
   bool indirect = token->Fcall.var != NULL;
   if (!indirect && !token->Fcall.ptr) return;
   Token *fn = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   int  self      = token->is_method_call ? 1 : 0;
   int  wanted    = fn->Fn.params_count - self;
   bool bad_count = fn->is_variadic ? node->children_count < wanted
                                    : node->children_count != wanted;
   if (bad_count) {
      parse_error(token, ERR_WRONG_ARG_COUNT, token->name);
      return;
   }
   for (int i = 0; i < wanted; i++) {
      Token *param    = fn->Fn.params[i + self];
      adopt_literal(node->children[i], param->ret_type);
      Type arg_type   = node->children[i]->token->ret_type;
      Type param_type = param->ret_type;
      if (arg_type && !same_or_pointer(arg_type, param_type)) {
         parse_error(node->children[i]->token, ERR_ARG_TYPE_MISMATCH, i + 1, token->name);
         return;
      }
      bool want_ref = param->is_ref;
      bool got_ref  = category(node->children[i]) == CAT_REF;
      if (want_ref && !got_ref) {
         parse_error(node->children[i]->token, ERR_ARG_NEEDS_REF, i + 1, token->name);
         return;
      }
      if (!want_ref && got_ref) {
         parse_error(node->children[i]->token, ERR_ARG_NO_REF, i + 1, token->name);
         return;
      }
   }
   Token *ret         = indirect ? fn->Fn.ret : fn;
   token->ret_type    = ret->ret_type;
   token->is_optional = ret->is_optional;
   token->is_ref      = ret->is_ref;
   if (ret->ret_type == ARRAY_TYPE) token->Array = ret->Array;
}

bool struct_contains(Node *def, Node *target, int depth) {
   if (!def || depth > 64) return false;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      Node *sub = field->Struct.ptr;
      if (!sub) continue;
      if (sub == target) return true;
      if (struct_contains(sub, target, depth + 1)) return true;
   }
   return false;
}

bool is_field(Token *token) {
   return token->name && token->type != STRUCT_DEF && token->type != FDEC;
}

char *struct_name_of(Token *token) {
   if (token->ret_type == STRUCT_CALL && token->Struct.name)
      return token->Struct.name;
   return type_name(token->ret_type);
}

Node *find_destructor(Node *def) {
   char *qualified = format("%s.drop", def->token->name);
   Node *fn        = find_method(def, qualified);
   free(qualified);
   return fn;
}

Node *find_printer(Node *def) {
   if (!def) return NULL;
   char *qualified = format("%s.output", def->token->name);
   Node *fn        = find_method(def, qualified);
   free(qualified);
   return fn;
}

Token *operand_of(Node *rhs) {
   bool is_ref = rhs->token->type == REF;
   return is_ref && rhs->left ? rhs->left->token : rhs->token;
}

Node *operator_named(Node *node, bool as_ref) {
   Node *def = node->left->token->Struct.ptr;
   if (!def) return NULL;
   char *q  = format("%s.%s.%s%s", def->token->name, spell(node->token->type),
                     as_ref ? "ref." : "",
                     struct_name_of(operand_of(node->right)));
   Node *fn = find_method(def, q);
   free(q);
   return fn;
}

bool find_operator(Node *node) {
   Token *token = node->token;
   Node  *def   = node->left->token->Struct.ptr;
   if (!def) return false;

   bool  spelled = node->right->token->type == REF;
   Node *fn      = operator_named(node, spelled);
   if (!fn) fn   = operator_named(node, !spelled);
   if (!fn) return false;
   token->Fcall.ptr = fn;
   token->ret_type  = fn->token->ret_type;
   if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
   return true;
}

bool is_untyped_literal(Node *node) {
   if (!node) return false;
   if (includes(node->token->type, I32, F32, 0)) return true;
   if (!includes(node->token->type, MATH_TYPE, 0)) return false;
   return is_untyped_literal(node->left) && is_untyped_literal(node->right);
}

bool int_range(Type type, long *lo, long *hi) {
   switch (type) {
      case I8:   *lo = INT8_MIN;  *hi = INT8_MAX;  return true;
      case I16:  *lo = INT16_MIN; *hi = INT16_MAX; return true;
      case I32:  *lo = INT32_MIN; *hi = INT32_MAX; return true;
      case I64:  *lo = INT64_MIN; *hi = INT64_MAX; return true;
      case U8:   *lo = 0; *hi = UINT8_MAX;  return true;
      case U16:  *lo = 0; *hi = UINT16_MAX; return true;
      case U32:  *lo = 0; *hi = UINT32_MAX; return true;
      case U64:  *lo = 0; *hi = INT64_MAX;  return true;
      default:   return false;
   }
}

bool literal_value(Node *node, long *out) {
   if (!node || !node->token) return false;
   if (node->token->type == I32) {
      *out = node->token->Int.value;
      return true;
   }
   long rhs;
   if (node->token->type == SUB && node->left && node->right
       && node->left->token->type == I32 && node->left->token->Int.value == 0
       && literal_value(node->right, &rhs)) {
      *out = -rhs;
      return true;
   }
   return false;
}

void check_int_range(Node *node, Type target) {
   long value, lo, hi;
   if (!literal_value(node, &value)) return;
   if (!int_range(target, &lo, &hi)) return;
   if (value >= lo && value <= hi) return;
   parse_error(node->token, ERR_LITERAL_OUT_OF_RANGE, (long long)value,
               type_name(target), (long long)lo, (long long)hi);
}

void retype_literal(Node *node, Type target) {
   if (!is_untyped_literal(node)) return;
   node->token->ret_type = target;
   retype_literal(node->left, target);
   retype_literal(node->right, target);
}

void adopt_literal(Node *node, Type target) {
   if (!is_untyped_literal(node)) return;
   Type a = node->token->ret_type;
   Type b = target;
   bool fam;
   if (b == BOOL) fam = false;
   else if (is_float(a)) fam = is_float(b);
   else fam = includes(a, NUMERIC_TYPES, 0) && includes(b, NUMERIC_TYPES, 0);
   if (!fam) return;
   if (!is_float(a)) check_int_range(node, target);
   retype_literal(node, target);
}

void unify_literals(Node *left, Node *right) {
   Type lt = left->token->ret_type;
   Type rt = right->token->ret_type;
   long llo, lhi, rlo, rhi;
   if (is_untyped_literal(left) && is_untyped_literal(right)
       && int_range(lt, &llo, &lhi) && int_range(rt, &rlo, &rhi)) {
      Type wide = lhi >= rhi ? lt : rt;
      adopt_literal(left, wide);
      adopt_literal(right, wide);
      return;
   }
   adopt_literal(left, rt);
   adopt_literal(right, lt);
}

void type_check(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         Token *prev = ura.fn_ret;
         ura.fn_ret  = node->token;
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         ura.fn_ret = prev;
         break;
      }
      case STRUCT_DEF: {
         node->token->ret_type = STRUCT_DEF;
         int n = 0;
         for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            type_check(child);
            if (is_field(child->token))
               child->token->Struct.index = n++;
         }
         if (struct_contains(node, node, 0))
            parse_error(node->token, ERR_STRUCT_RECURSIVE, node->token->name);
         break;
      }
      case ENUM_DEF: break;
      case I32:     if (!token->ret_type) token->ret_type = I32; break;
      case BOOL:    token->ret_type = BOOL; break;
      case CHARS:   set_string_type(token); break;
      case CHAR:    token->ret_type = CHAR; break;
      case F32:   token->ret_type = F32; break;
      case ID:      break;
      case FN_TYPE: break;
      case RETURN: {
         type_check(node->left);
         if (!ura.fn_ret || !node->left) return;
         Type   wanted = ura.fn_ret->ret_type;
         Token *value  = node->left->token;
         adopt_literal(node->left, wanted);
         if (is_null(value)) {
            value->ret_type    = wanted;
            value->Array       = ura.fn_ret->Array;
            value->Struct      = ura.fn_ret->Struct;
            value->is_optional = true;
         }
         bool want_ref = ura.fn_ret->is_ref;
         node->token->kind = want_ref ? RET_REF : RET_VALUE;
         if (want_ref && category(node->left) != CAT_REF) {
            parse_error(node->token, ERR_RETURN_NEEDS_REF, ura.fn_ret->name);
            return;
         }
         if (!want_ref && value->type == REF) {
            parse_error(node->token, ERR_RETURN_NO_REF, ura.fn_ret->name);
            return;
         }
         if (!is_assignable(ura.fn_ret, value)) {
            parse_error(node->token, ERR_ASSIGN_OPTIONAL,
                        value->name ? value->name : "the value", ura.fn_ret->name);
            return;
         }
         Type got = node->left->token->ret_type;
         if (got && got != wanted)
            parse_error(node->token, ERR_RETURN_TYPE_MISMATCH, ura.fn_ret->name, type_name(wanted), type_name(got));
         break;
      }
      case FCALL:   type_check_fcall(node); break;
      case ERRPUT:
      case OUTPUT: {
         for (int i = 0; i < node->children_count; i++)
         type_check(node->children[i]);
         token->ret_type = VOID;
         break;
      }
      case NOT: case BNOT:
         type_check(node->left);
         if (token->type == NOT && node->left->token->ret_type != BOOL)
            parse_error(token, "Operator 'not' needs a bool operand");
         token->ret_type = (token->type == NOT) ? BOOL : node->left->token->ret_type;
         break;
      case AS: {
         type_check(node->left);
         Type src = node->left->token->ret_type;
         Type dst = node->right->token->ret_type;
         bool bad_src = src && !is_castable(src);
         bool bad_dst = !is_castable(dst);
         if (bad_src || bad_dst)
            parse_error(token, ERR_CANNOT_CAST,
                        struct_name_of(node->left->token),
                        struct_name_of(node->right->token));
         token->ret_type = dst;
         break;
      }
      case REF: {
         type_check(node->left);
         token->ret_type = node->left->token->ret_type;
         break;
      }
      case IF: case ELIF: case ELSE: case WHILE: case LOOP: {
         type_check(node->left);
         if (node->left && node->left->token->ret_type != BOOL)
            parse_error(node->token, "The '%s' condition must be a bool, got %s",
                        node->token->name, type_name(node->left->token->ret_type));
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         type_check(node->right);
         break;
      }
      case FOR: {
         type_check(node->right);
         Token *iter = node->right->token;
         if (iter->type != RANGE && iter->ret_type != ARRAY_TYPE)
            parse_error(node->token, ERR_FOR_NOT_ITERABLE, node->left->token->name);
         if (node->token->is_ref && iter->type == RANGE)
            parse_error(node->token, ERR_FOR_REF_NEEDS_ARRAY);
         if (iter->type == RANGE && node->right->children_count) {
            Node  *by = node->right->children[0];
            type_check(by);
            Token *bt = by->token;
            if (!includes(bt->ret_type, NUMERIC_TYPES, 0))
               parse_error(bt, ERR_BY_NOT_INT, type_name(bt->ret_type));
            else if (bt->type == I32 && bt->Int.value <= 0)
               parse_error(bt, ERR_BY_NOT_POSITIVE);
         }
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         break;
      }
      case MATCH: {
         type_check(node->left);
         Type subject = node->left->token->ret_type;
         for (int i = 0; i < node->children_count; i++) {
            Node *branch = node->children[i];
            if (branch->left)
               for (int j = 0; j < branch->left->children_count; j++) {
                  Node *value = branch->left->children[j];
                  type_check(value);
                  Type ret_type = value->token->ret_type;
                  if (subject && ret_type && ret_type != subject)
                     parse_error(value->token, ERR_CASE_TYPE_MISMATCH, type_name(ret_type), type_name(subject));
               }
            for (int j = 0; j < branch->children_count; j++)
               type_check(branch->children[j]);
         }
         break;
      }
      case MODULE: {
         char *prev = ura.current_module;
         ura.current_module = node->token->name;
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         ura.current_module = prev;
         break;
      }
      case TRY: {
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         Node *cnode = node->right;
         for (int i = 0; i < cnode->children_count; i++)
            type_check(cnode->children[i]);
         break;
      }
      case THROW: {
         type_check(node->left);
         Token *v      = node->left->token;
         bool   is_err = v->ret_type == STRUCT_CALL && v->Struct.ptr
                         && strcmp(v->Struct.ptr->token->name, "Error") == 0;
         if (!is_err)
            parse_error(node->token, ERR_THROW_NEEDS_ERROR,
                        type_name(v->ret_type));
         break;
      }
      case ARRAY_LIT: {
         if (node->children_count == 0) {
            parse_error(node->token, "Empty array literal has no element type");
            return;
         }
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         Token *first = node->children[0]->token;
         for (int i = 1; i < node->children_count; i++) {
            Token *elem     = node->children[i]->token;
            bool   is_array = first->ret_type == ARRAY_TYPE;
            bool   same_type  = elem->ret_type == first->ret_type;
            bool   same_sub   = elem->Array.sub_type == first->Array.sub_type;
            bool   same_depth = elem->Array.depth == first->Array.depth;
            bool   same_array = !is_array || (same_sub && same_depth);
            if (!same_type || !same_array)
               parse_error(elem, "Array elements must all be the same type");
         }
         node->token->ret_type = ARRAY_TYPE;
         if (first->ret_type == ARRAY_TYPE) {
            node->token->Array.sub_type = first->Array.sub_type;
            node->token->Array.depth    = first->Array.depth + 1;
         } else {
            node->token->Array.sub_type = first->ret_type;
            node->token->Array.depth    = 1;
         }
         break;
      }
      case ACCESS: {
         type_check(node->left);
         type_check(node->right);
         Token *arr = node->left->token;
         if (arr->ret_type != ARRAY_TYPE) {
            parse_error(node->token, ERR_NOT_AN_ARRAY, type_name(arr->ret_type));
            return;
         }
         if (node->right->token->type == RANGE) {
            node->token->ret_type = ARRAY_TYPE;
            node->token->Array    = arr->Array;
            return;
         }
         if (!includes(node->right->token->ret_type, NUMERIC_TYPES, 0))
            parse_error(node->token, ERR_INDEX_NOT_INT, type_name(node->right->token->ret_type));
         node->token->Array       = arr->Array;
         node->token->Array.depth = arr->Array.depth - 1;
         bool elem = node->token->Array.depth == 0;
         node->token->ret_type = elem ? arr->Array.sub_type : ARRAY_TYPE;
         if (elem && arr->Array.sub_type == STRUCT_CALL)
            node->token->Struct.ptr = arr->Array.struct_ptr;
         break;
      }
      case DOT: {
         type_check(node->left);
         Token *token = node->token;
         Token *left  = node->left->token;
         if (left->ret_type == ARRAY_TYPE) {
            if (strcmp(token->name, "len") != 0)
               parse_error(token, ERR_UNKNOWN_MEMBER, token->name);
            else
               token->ret_type = U64;
            return;
         }
         if (left->ret_type != STRUCT_CALL) {
            char *tname = type_name(left->ret_type);
            if (strcmp(token->name, "len") == 0)
               parse_error(token, ERR_LEN_NOT_ARRAY, tname);
            else
               parse_error(token, ERR_TYPE_HAS_NO_FIELDS, token->name, tname);
            return;
         }
         Node  *def   = left->Struct.ptr;
         Token *field = NULL;
         for (int i = 0; def && i < def->children_count; i++) {
            Token *f = def->children[i]->token;
            if (!is_field(f)) continue;
            if (strcmp(f->name, token->name) == 0) { field = f; break; }
         }
         if (!field) {
            if (!def) return;
            parse_error(token, ERR_UNKNOWN_FIELD, def->token->name, token->name);
            return;
         }
         token->ret_type    = field->ret_type;
         token->is_ref      = field->is_ref;
         token->is_optional = field->is_optional;
         token->Struct      = field->Struct;
         token->Array       = field->Array;
         break;
      }
      case RANGE: {
         type_check(node->left);
         type_check(node->right);
         bool left_num  = includes(node->left->token->ret_type, NUMERIC_TYPES, 0);
         bool right_num = includes(node->right->token->ret_type, NUMERIC_TYPES, 0);
         if (!left_num || !right_num)
         parse_error(node->token, "Range bounds must be integers");
         node->token->ret_type = I32;
         break;
      }
      case ARRAY: {
         for (int i = 0; i < node->children_count; i++) {
            type_check(node->children[i]);
            if (!includes(node->children[i]->token->ret_type, NUMERIC_TYPES, 0))
               parse_error(node->children[i]->token, ERR_ARRAY_SIZE_NOT_INT);
         }
         break;
      }
      case TYPEOF: case SIZEOF:
         if (!is_data_type(node->left->token)) type_check(node->left);
         if (token->type == TYPEOF) set_string_type(node->token);
         else node->token->ret_type = U64;
         break;
      case CLEAN: {
         type_check(node->left);
         Token *target = node->left->token;
         node->token->ret_type = VOID;
         if (target->ret_type == ARRAY_TYPE) break;
         if (target->ret_type == STRUCT_CALL && target->is_ref) break;
         char *name = struct_name_of(target);
         if (target->ret_type == STRUCT_CALL)
            parse_error(node->token, ERR_CLEAN_ON_STRUCT, name);
         else
            parse_error(node->token, ERR_CLEAN_NEEDS_ARRAY, name);
         break;
      }
      case NEW: break;
      case BREAK: case CONTINUE: case NULL_LIT: break;
      case FALLBACK:
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
      case MOD_ASSIGN: case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
      case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: {
         Token *token = node->token;
         type_check(node->left);
         type_check(node->right);
         unify_literals(node->left, node->right);
         Type   lt  = node->left->token->ret_type;
         Type   rt  = node->right->token->ret_type;
         Token *lhs = node->left->token;
         Token *rhs = node->right->token;
         if (includes(token->type, EQUAL, NOT_EQUAL, 0)) {
            bool l_cref = category(node->left) == CAT_REF;
            bool r_cref = category(node->right) == CAT_REF;
            bool l_ok   = l_cref || lhs->is_ref;
            bool r_ok   = r_cref || rhs->is_ref;
            bool slice  = lt == ARRAY_TYPE || rt == ARRAY_TYPE;
            if ((l_cref || r_cref) && l_ok && r_ok) token->kind = CMP_REF;
            else if (slice)                         token->kind = CMP_SLICE;
            else                                    token->kind = CMP_VALUE;
         }
         if (token->type == ASSIGN) {
            bool lhs_ref = lhs->is_ref && !lhs->is_dec;
            bool rebind  = lhs->is_ref && category(node->right) == CAT_REF;
            if (rebind)       token->kind = REF_REBIND;
            else if (lhs_ref) {
               token->kind = REF_WRITE;
               if (node->left->token->type == DOT) {
                  parse_error(token, ERR_REF_FIELD_WRITE, node->left->token->name);
                  return;
               }
            }
            else              token->kind = VAL_STORE;
         }
         if (token->type == FALLBACK) {
            if (!lhs->is_optional) {
               parse_error(token, ERR_FALLBACK_LHS);
               return;
            }
            if (rhs->is_optional) {
               parse_error(token, ERR_FALLBACK_RHS);
               return;
            }
            token->ret_type    = rt;
            token->Array       = rhs->Array;
            token->is_optional = false;
            return;
         }
         if (is_null(lhs) || is_null(rhs)) {
            Token *nul   = is_null(lhs) ? lhs : rhs;
            Token *other = is_null(lhs) ? rhs : lhs;
            bool   ptr_like = is_pointer(other->ret_type) || other->is_ref;
            if (!ptr_like) {
               parse_error(nul, ERR_NULL_ON_VALUE, type_name(other->ret_type));
               return;
            }
            nul->ret_type    = other->ret_type;
            nul->Array       = other->Array;
            nul->Struct      = other->Struct;
            nul->is_ref      = other->is_ref;
            nul->is_optional = true;
            lt = lhs->ret_type;
            rt = rhs->ret_type;
            if (includes(token->type, EQUAL, NOT_EQUAL, 0)) {
               if (!is_nullable(other)) {
                  parse_error(token, ERR_COMPARE_NON_NULL, other->name);
                  return;
               }
               token->ret_type = BOOL;
               return;
            }
         }
         if (token->type == ASSIGN && !is_assignable(lhs, rhs)) {
            if (is_null(rhs)) parse_error(token, ERR_NULL_NO_TARGET, lhs->name);
            else parse_error(token, ERR_ASSIGN_OPTIONAL, rhs->name, lhs->name);
            return;
         }
         if (node->left->token->is_dec && node->left->token->is_ref) {
            if (category(node->right) != CAT_REF)
               parse_error(token, ERR_REF_NEEDS_VARIABLE);
            else if (lt && rt && lt != rt)
               parse_error(token, ERR_REF_TYPE_MISMATCH, type_name(lt), type_name(rt));
            token->ret_type = lt;
            return;
         }
         if (token->kind == CMP_REF) {
            token->ret_type = BOOL;
            return;
         }
         if (lt == STRUCT_CALL) {
            bool assign = token->type == ASSIGN;
            bool is_init = assign && node->left->token->is_dec;
            bool binding = assign && node->left->token->is_ref
                                  && node->right->token->type == REF;
            if (!is_init && !binding && find_operator(node)) return;

            if (!assign) {
               parse_error(token, ERR_NO_OPERATOR, struct_name_of(node->left->token),
                           spell(token->type), struct_name_of(operand_of(node->right)));
               return;
            }
            Node *lhs_def = node->left->token->Struct.ptr;
            Node *rhs_def = node->right->token->Struct.ptr;
            if (rt == STRUCT_CALL && lhs_def && rhs_def && lhs_def != rhs_def) {
               parse_error(token, ERR_ARG_TYPE_MISMATCH_STRUCT, struct_name_of(node->right->token),
                           struct_name_of(node->left->token));
               return;
            }
         }
         if (lt && rt && lt != rt) {
            if (token->type == ASSIGN)
               parse_error(token, ERR_CANNOT_ASSIGN, struct_name_of(node->right->token),
                           struct_name_of(node->left->token));
            else
               parse_error(token, ERR_BINOP_TYPE_MISMATCH, spell(token->type),
                           struct_name_of(node->left->token),
                           struct_name_of(node->right->token));
            return;
         }
         if (is_float(lt) && includes(token->type, BITWISE_TYPE, 0)) {
            parse_error(token, ERR_BITWISE_NEEDS_INT);
            return;
         }
         if (includes(token->type, LOGIC_TYPE, 0)) {
            if (lt != BOOL || rt != BOOL) {
               parse_error(token, "Operators 'and'/'or' need bool operands");
               return;
            }
            token->ret_type = BOOL;
         } else
            token->ret_type = includes(token->type, COMPARISON_OPS, 0) ? BOOL : lt;
         break;
      }
      default:
         CHECK(1, ASSERT_TYPECHECK_NODE, to_string(token->type));
         break;
   }
}

/* ---- backend/codegen.c ---- */
Block here_block() {
   return LLVMGetInsertBlock(ura.builder);
}

Value here_func() {
   return LLVMGetBasicBlockParent(here_block());
}

void llvm_at(Block block) {
   LLVMPositionBuilderAtEnd(ura.builder, block);
}

Block llvm_block(Value fn, char *name) {
   return LLVMAppendBasicBlockInContext(ura.context, fn, name);
}

Value llvm_alloca(TypeRef type, char *name) {
   return LLVMBuildAlloca(ura.builder, type, name);
}

Value llvm_load(TypeRef type, Value ptr, char *name) {
   return LLVMBuildLoad2(ura.builder, type, ptr, name);
}

void llvm_store(Value value, Value ptr) {
   LLVMBuildStore(ura.builder, value, ptr);
}

Value llvm_gep(TypeRef type, Value ptr, Value *idx, int n, char *name) {
   return LLVMBuildGEP2(ura.builder, type, ptr, idx, n, name);
}

Value llvm_icmp(LLVMIntPredicate pred, Value l, Value r, char *name) {
   return LLVMBuildICmp(ura.builder, pred, l, r, name);
}

Value llvm_fcmp(LLVMRealPredicate pred, Value l, Value r, char *name) {
   return LLVMBuildFCmp(ura.builder, pred, l, r, name);
}

void llvm_br(Block dest) {
   LLVMBuildBr(ura.builder, dest);
}

void llvm_cond_br(Value cond, Block yes, Block no) {
   LLVMBuildCondBr(ura.builder, cond, yes, no);
}

Value llvm_call(TypeRef type, Value fn, Value *args, int n, char *name) {
   return LLVMBuildCall2(ura.builder, type, fn, args, n, name);
}

Value llvm_extract(Value agg, unsigned index, char *name) {
   return LLVMBuildExtractValue(ura.builder, agg, index, name);
}

Value llvm_insert(Value agg, Value elem, unsigned index, char *name) {
   return LLVMBuildInsertValue(ura.builder, agg, elem, index, name);
}

Value llvm_string(char *text, char *name) {
   return LLVMBuildGlobalStringPtr(ura.builder, text, name);
}

Value const_int(TypeRef type, long long value) {
   return LLVMConstInt(type, value, 0);
}

Value const_i32(long long value) {
   return LLVMConstInt(ura.i32, value, 0);
}

Value const_i64(long long value) {
   return LLVMConstInt(ura.i64, value, 0);
}

TypeRef pointer_to(TypeRef type) {
   return LLVMPointerType(type, 0);
}

Value llvm_binop(LLVMOpcode op, Value l, Value r, char *name) {
   return LLVMBuildBinOp(ura.builder, op, l, r, name);
}

Value llvm_int_cast(Value value, TypeRef type) {
   return LLVMBuildIntCast2(ura.builder, value, type, 1, "cast");
}

Value llvm_global(Token *token) {
   TypeRef type = llvm_type_of(token);
   Value   glob = LLVMAddGlobal(ura.module, type, token->name);
   LLVMSetInitializer(glob, LLVMConstNull(type));
   LLVMSetLinkage(glob, LLVMInternalLinkage);
   return token->llvm.elem = glob;
}

MetadataRef llvm_di_location(int line, MetadataRef scope) {
   return LLVMDIBuilderCreateDebugLocation(ura.context, line, 0, scope, NULL);
}

void llvm_set_location(MetadataRef loc) {
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

void setup_paths(char *path_name) {
   char *slash = strrchr(path_name, '/');
   if (slash) {
      ura.dir = strdup(path_name);
      ura.dir[slash - path_name] = '\0';
      ura.base = strdup(slash + 1);
   } else {
      ura.dir = strdup(".");
      ura.base = strdup(path_name);
   }
   char *dot = strrchr(ura.base, '.');
   if (dot) *dot = '\0';
   ura.build_dir = format("%s/build", ura.dir);
   mkdir(ura.build_dir, 0755);
   ura.ll_path = format("%s/%s.ll", ura.build_dir, ura.base);
}

void init_module(char *name) {
   ura.context = LLVMContextCreate();
   ura.module  = LLVMModuleCreateWithNameInContext(name, ura.context);
   ura.builder = LLVMCreateBuilderInContext(ura.context);
   ura.vd  = LLVMVoidTypeInContext(ura.context);
   ura.i1  = LLVMInt1TypeInContext(ura.context);
   ura.i8  = LLVMInt8TypeInContext(ura.context);
   ura.i16 = LLVMInt16TypeInContext(ura.context);
   ura.i32 = LLVMInt32TypeInContext(ura.context);
   ura.i64 = LLVMInt64TypeInContext(ura.context);
   ura.f32 = LLVMFloatTypeInContext(ura.context);
   ura.f64 = LLVMDoubleTypeInContext(ura.context);
   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   char *triple = LLVMGetDefaultTargetTriple();
   LLVMSetTarget(ura.module, triple);
   Target target;
   if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
      TargetMachine machine = LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault);
      TargetData layout = LLVMCreateTargetDataLayout(machine);
      LLVMSetModuleDataLayout(ura.module, layout);
      LLVMDisposeTargetData(layout);
      LLVMDisposeTargetMachine(machine);
   }
   LLVMDisposeMessage(triple);

   if (!ura.enable_san) return;
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18, LLVMValueAsMetadata(const_i32(3)));
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13, LLVMValueAsMetadata(const_i32(4)));
   ura.debug_builder = LLVMCreateDIBuilder(ura.module);
   char *src   = ura.sources[0]->filename;
   char *slash = strrchr(src, '/');
   char *file  = slash ? slash + 1 : src;
   ura.debug_file = LLVMDIBuilderCreateFile(ura.debug_builder, file, strlen(file),
                                            ura.dir, strlen(ura.dir));
   ura.debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
       ura.debug_builder, LLVMDWARFSourceLanguageC, ura.debug_file, "ura", 3, 0, "", 0, 0, "", 0,
       LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);
   ura.debug_scope = ura.debug_compile_unit;
}

void finalize_module(char *ll_path) {
   char *error = NULL;
   PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
   if (ura.flags) {
      Error err = LLVMRunPasses(ura.module, ura.flags, NULL, opts);
      if (err) {
         char *msg = LLVMGetErrorMessage(err);
         CHECK(1, "optimizer error: %s", msg);
         LLVMDisposeErrorMessage(msg);
      }
   }
   if (ura.debug_builder) {
      LLVMDIBuilderFinalize(ura.debug_builder);
      LLVMDisposeDIBuilder(ura.debug_builder);
      ura.debug_builder = NULL;
   }
   if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
      CHECK(1, "module verification failed:\n%s", error);
   LLVMDisposeMessage(error);
   LLVMDisposePassBuilderOptions(opts);
   LLVMPrintModuleToFile(ura.module, ll_path, NULL);
   if (ura.enable_ll) {
      char *ir = LLVMPrintModuleToString(ura.module);
      printf("%s", ir);
      LLVMDisposeMessage(ir);
   }
}

Value lib_fn(char *name, TypeRef *type) {
   Node *fn = find_function(name);
   if (!fn) {
      parse_error(NULL, ERR_MISSING_LIB_FN, name);
      return NULL;
   }
   emit_signature(fn);
   *type = fn->token->llvm.func_type;
   return fn->token->llvm.elem;
}

void guard(Token *op, Value is_bad, char *what) {
   Value fn   = here_func();
   Block trap = llvm_block(fn, "trap");
   Block cont = llvm_block(fn, "cont");
   llvm_cond_br(is_bad, trap, cont);

   llvm_at(trap);
   char  *text = NULL;
   size_t tlen = 0;
   File   ms   = open_memstream(&text, &tlen);
   fprintf(ms, RED("runtime error: ") "%s\n", what);
   render_caret(ms, op, CARET_ERR);
   fclose(ms);
   if (ura.no_color) decolor(text);
   Value   msg  = llvm_string(text, "trap_msg");
   TypeRef i8p  = pointer_to(ura.i8);

   TypeRef write_ty      = NULL;
   Value   write_fn      = lib_fn("write", &write_ty);
   Value   write_args[3] = { const_i32(2), msg, LLVMConstInt(ura.i64, strlen(text), 0) };
   if (write_fn) llvm_call(write_ty, write_fn, write_args, 3, "");
   free(text);

   TypeRef exit_ty      = NULL;
   Value   exit_fn      = lib_fn("exit", &exit_ty);
   Value   exit_args[1] = { const_i32(1) };
   if (exit_fn) llvm_call(exit_ty, exit_fn, exit_args, 1, "");

   LLVMBuildUnreachable(ura.builder);
   llvm_at(cont);
}

void guard_nonzero(Token *op, Value divisor) {
   TypeRef ty     = LLVMTypeOf(divisor);
   bool    fp     = LLVMGetTypeKind(ty) == FloatType || LLVMGetTypeKind(ty) == DoubleType;
   Value   iszero = fp
      ? llvm_fcmp(LLVMRealOEQ, divisor, LLVMConstReal(ty, 0.0), "iszero")
      : llvm_icmp(LLVMIntEQ,   divisor, LLVMConstInt(ty, 0, 0), "iszero");
   guard(op, iszero, includes(op->type, MOD, MOD_ASSIGN, 0) ? "Modulo by zero" : "Division by zero");
}

void guard_bound(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = llvm_icmp(LLVMIntEQ, ptr, null, "unbound");
   char *msg    = format("reference '%s' used before it was bound - assign '%s = ref <target>' first", op->name, op->name);
   guard(op, isnull, msg);
   free(msg);
}

void emit_signature(Node *fn) {
   Token *token = fn->token;
   if (token->llvm.func_type) return;
   int      n      = token->Fn.params_count;
   TypeRef *params = NULL;
   if (n > 0) {
      params = allocate(n, sizeof(TypeRef));
      for (int i = 0; i < n; i++) {
         TypeRef pt = llvm_type_of(token->Fn.params[i]);
         params[i] = token->Fn.params[i]->is_ref ? pointer_to(pt) : pt;
      }
   }
   TypeRef ret;
   if (token->ret_type == STRUCT_CALL)
      ret = struct_type_of(token->Struct.ptr);
   else if (token->ret_type == ARRAY_TYPE)
      ret = array_type(token, token->Array.depth);
   else
      ret = to_llvm_type(token->ret_type);
   if (token->is_ref) ret = pointer_to(ret);
   if (is_main(token)) {
      TypeRef argv = pointer_to(pointer_to(ura.i8));
      TypeRef mp[2] = { ura.i32, argv };
      token->llvm.func_type = LLVMFunctionType(ret, mp, 2, 0);
   } else
      token->llvm.func_type = LLVMFunctionType(ret, params, n, token->is_variadic);
   token->llvm.elem      = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
   free(params);
}

Value field_ptr(Node *node) {
   Token  *token  = node->token;
   Token  *left   = node->left->token;
   Value   base   = struct_arg_ptr(node->left);
   TypeRef sty    = struct_type_of(left->Struct.ptr);
   Value   idx[2] = { const_i32(0), const_i32(token->Struct.index) };
   return llvm_gep(sty, base, idx, 2, token->name);
}

Value access_ptr(Node *node) {
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = llvm_extract(slice, 0, "arr.data");
   code_gen(node->right);
   Value   idx   = node->right->token->llvm.elem;
   if (node->token->is_nullable) {
      Value len   = llvm_extract(slice, 1, "arr.len");
      Value idx64 = LLVMBuildIntCast2(ura.builder, idx, ura.i64, 1, "idx");
      Value low   = llvm_icmp(LLVMIntSLT, idx64, const_i64(0), "oob.low");
      Value high  = llvm_icmp(LLVMIntSGE, idx64, len, "oob.high");
      guard(node->token, LLVMBuildOr(ura.builder, low, high, "oob"),
            "array index out of bounds");
   }
   Token  *arr   = node->left->token;
   TypeRef elem  = elem_type(arr, arr->Array.depth);
   return llvm_gep(elem, data, &idx, 1, "arr.at");
}

Value string_slice(Token *token, char *text) {
   Value str = llvm_string(text, "str");
   Value len = LLVMConstInt(ura.i64, strlen(text), 0);
   return make_slice(token, 1, str, len);
}

Value make_slice(Token *arr, int depth, Value data, Value len) {
   Value agg = LLVMGetUndef(array_type(arr, depth));
   agg = llvm_insert(agg, data, 0, "arr.ptr");
   agg = llvm_insert(agg, len,  1, "arr.len");
   return agg;
}

Value array_calloc(TypeRef elem, Value count, Value esz) {
   TypeRef cty = NULL;
   Value   fn  = lib_fn("calloc", &cty);
   if (!fn) return LLVMConstNull(pointer_to(elem));
   Value   mem = llvm_call(cty, fn, (Value[]){ count, esz }, 2, "heap");
   return LLVMBuildBitCast(ura.builder, mem, pointer_to(elem), "arr");
}

Value build_array(Token *arr, Value *dims, int depth, bool heap) {
   Value   n    = dims[0];
   TypeRef elem = elem_type(arr, depth);
   Value   esz  = LLVMConstInt(ura.i64, LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), elem), 0);
   if (depth == 1) {
      if (heap) return make_slice(arr, 1, array_calloc(elem, n, esz), n);
      Value data  = LLVMBuildArrayAlloca(ura.builder, elem, n, "arr");
      Value bytes = LLVMBuildMul(ura.builder, n, esz, "bytes");
      LLVMBuildMemSet(ura.builder, data, LLVMConstInt(ura.i8, 0, 0), bytes, 0);
      return make_slice(arr, 1, data, n);
   }
   TypeRef inner = elem;
   Value   data  = heap ? array_calloc(inner, n, esz)
                        : LLVMBuildArrayAlloca(ura.builder, inner, n, "arr");
   Value   fn    = here_func();
   Value   slot  = llvm_alloca(ura.i64, "i");
   llvm_store(const_i64(0), slot);
   Block   cond  = llvm_block(fn, "arr.cond");
   Block   body  = llvm_block(fn, "arr.body");
   Block   end   = llvm_block(fn, "arr.end");
   llvm_br(cond);
   llvm_at(cond);
   Value   i     = llvm_load(ura.i64, slot, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, n, "more"), body, end);
   llvm_at(body);
   Value   sub_arr = build_array(arr, dims + 1, depth - 1, heap);
   i = llvm_load(ura.i64, slot, "i");
   Value   gep = llvm_gep(inner, data, &i, 1, "arr.slot");
   llvm_store(sub_arr, gep);
   llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
   llvm_br(cond);
   llvm_at(end);
   return make_slice(arr, depth, data, n);
}

void free_array(Token *arr, Value slice, int depth) {
   Value data = llvm_extract(slice, 0, "arr.data");
   if (depth > 1) {
      Value   len   = llvm_extract(slice, 1, "arr.len");
      TypeRef inner = array_type(arr, depth - 1);
      Value   fn    = here_func();
      Value   slot  = llvm_alloca(ura.i64, "i");
      llvm_store(const_i64(0), slot);
      Block   cond  = llvm_block(fn, "free.cond");
      Block   body  = llvm_block(fn, "free.body");
      Block   end   = llvm_block(fn, "free.end");
      llvm_br(cond);
      llvm_at(cond);
      Value   i     = llvm_load(ura.i64, slot, "i");
      llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
      llvm_at(body);
      Value   gep   = llvm_gep(inner, data, &i, 1, "free.slot");
      free_array(arr, llvm_load(inner, gep, "inner"), depth - 1);
      i = llvm_load(ura.i64, slot, "i");
      llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
      llvm_br(cond);
      llvm_at(end);
   }
   Value   ptr = LLVMBuildBitCast(ura.builder, data, pointer_to(ura.i8), "free.ptr");
   TypeRef fty = NULL;
   Value   ffn = lib_fn("free", &fty);
   if (ffn) llvm_call(fty, ffn, (Value[]){ ptr }, 1, "");
}

bool needs_drop(Node *def) {
   if (!def) return false;
   if (def->token->has_drop) return true;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      if (needs_drop(field->Struct.ptr)) return true;
   }
   return false;
}

void emit_drop_value(Value ptr, Node *def) {
   if (!def) return;
   if (def->token->has_drop) {
      Node  *fn   = find_destructor(def);
      emit_signature(fn);
      Token *impl = fn->token;
      llvm_call(impl->llvm.func_type, impl->llvm.elem, &ptr, 1, "");
   }
   TypeRef sty = struct_type_of(def);
   for (int i = def->children_count - 1; i >= 0; i--) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      Node *sub = field->Struct.ptr;
      if (!needs_drop(sub)) continue;
      Value idx[2] = { const_i32(0), const_i32(field->Struct.index) };
      Value slot   = llvm_gep(sty, ptr, idx, 2, field->name);
      emit_drop_value(slot, sub);
   }
}

void emit_drops(Node *scope, Token *keep) {
   if (!scope || LLVMGetBasicBlockTerminator(here_block())) return;
   for (int i = scope->variables_count - 1; i >= 0; i--) {
      Token *var = scope->variables[i];
      bool  skip = var == keep || var->is_ref || var->is_param
                   || !var->llvm.elem || var->ret_type != STRUCT_CALL;
      Node  *def = NULL;
      if (!skip && needs_drop(var->Struct.ptr)) def = var->Struct.ptr;
      if (def) emit_drop_value(var->llvm.elem, def);
   }
}

void emit_unwind(Node *stop, Token *keep) {
   for (int i = ura.scopes_count; i >= 1; i--) {
      Node *scope = ura.scopes[i];
      if (!scope) continue;
      emit_drops(scope, keep);
      bool at_fdec = scope->token->type == FDEC;
      if (stop ? scope == stop : at_fdec && !is_main(scope->token)) return;
   }
}

void scope_out() {
   emit_drops(ura.scope, NULL);
   exit_scope();
}

bool is_main(Token *token) {
   return token->type == FDEC && strcmp(token->name, "main") == 0;
}

void drop_temps() {
   if (!LLVMGetBasicBlockTerminator(here_block()))
      for (int i = ura.temps_count - 1; i >= 0; i--)
         emit_drop_value(ura.temps[i]->llvm.elem, ura.temps[i]->Struct.ptr);
   ura.temps_count = 0;
}

void code_gen_body(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      code_gen(node->children[i]);
      drop_temps();
      if (LLVMGetBasicBlockTerminator(here_block())) break;
   }
}

Type category(Node *n) {
   Token *t = n->token;
   if (t->ret_type == ARRAY_TYPE)             return CAT_SLICE;
   if (t->type == REF || t->type == NULL_LIT) return CAT_REF;
   if (t->type == NEW)                        return CAT_REF;
   if (t->type == FCALL && t->is_ref)         return CAT_REF;
   return CAT_VALUE;
}

Value emit_value(Node *n) {
   code_gen(n);
   return n->token->llvm.elem;
}

Value emit_place(Node *n) {
   Token *t = n->token;
   if (t->type == ACCESS) return access_ptr(n);
   if (t->type == DOT)    return field_ptr(n);
   if (t->is_dec) {
      if (t->is_global) return t->llvm.elem;
      TypeRef ty = llvm_type_of(t);
      if (t->is_ref) ty = pointer_to(ty);
      return t->llvm.elem = llvm_alloca(ty, t->name);
   }
   Token *decl = t->Decl.ptr;
   if (decl->is_ref) {
      TypeRef pty = pointer_to(llvm_type_of(decl));
      Value   ptr = llvm_load(pty, decl->llvm.elem, "ref");
      if (t->is_nullable) guard_bound(t, ptr);
      return ptr;
   }
   return decl->llvm.elem;
}

Value emit_ref(Node *n) {
   Token *t = n->token;
   if (t->type == REF)      return emit_ref(n->left);
   if (t->type == NULL_LIT) return LLVMConstNull(pointer_to(llvm_type_of(t)));
   if (t->type == FCALL)  { code_gen(n); return t->llvm.elem; }
   if (t->type == NEW)    { code_gen(n); return t->llvm.elem; }
   if (t->type == DOT && t->is_ref) {
      TypeRef pty = pointer_to(llvm_type_of(t));
      return llvm_load(pty, field_ptr(n), "ref");
   }
   bool ref_var = t->type == ID && t->Decl.ptr && t->Decl.ptr->is_ref;
   if (ref_var) {
      TypeRef pty = pointer_to(llvm_type_of(t->Decl.ptr));
      return llvm_load(pty, t->Decl.ptr->llvm.elem, "ref");
   }
   return emit_place(n);
}

void code_gen_operator(Node *node) {
   Token *token = node->token;
   Node  *fn    = token->Fcall.ptr;
   emit_signature(fn);
   Value  self   = struct_arg_ptr(node->left);
   Token *param  = fn->token->Fn.params_count > 1 ? fn->token->Fn.params[1] : NULL;
   bool   by_ref = param && param->is_ref;
   Type   kind     = node->right->token->type;
   bool   spelled  = kind == REF;                 
   bool   has_addr = includes(kind, ID, DOT, ACCESS, 0);
   Value  arg;
   if (spelled) {
      code_gen(node->right);
      Value addr = node->right->token->llvm.elem;
      arg = by_ref ? addr : llvm_load(llvm_type_of(param), addr, "deref");
   } else if (by_ref && has_addr) {
      arg = emit_place(node->right);
   } else {
      code_gen(node->right);
      arg = node->right->token->llvm.elem;
      if (by_ref) {
         Value slot = llvm_alloca(LLVMTypeOf(arg), "op.tmp");
         llvm_store(arg, slot);
         arg = slot;
      }
   }
   Value args[2] = { self, arg };
   char *name = fn->token->ret_type == VOID ? "" : "op";
   token->llvm.elem = llvm_call(fn->token->llvm.func_type, fn->token->llvm.elem, args, 2, name);
}

Value opt_ptr(Token *token, Value v) {
   return token->ret_type == ARRAY_TYPE ? llvm_extract(v, 0, "opt.ptr") : v;
}

Value emit_printf_fd(int fd, char *fmt, Value *args, int n) {
   bool    err   = fd != 1;
   int     lead  = err ? 2 : 1;
   TypeRef ty    = NULL;
   Value   fn    = lib_fn(err ? "dprintf" : "printf", &ty);
   if (!fn) return NULL;
   Value  *call  = allocate(n + lead, sizeof(Value));
   if (err) call[0] = const_i32(fd);
   call[lead - 1] = llvm_string(fmt, "fmt");
   if (n) memcpy(call + lead, args, n * sizeof(Value));
   Value res = llvm_call(ty, fn, call, n + lead, "");
   free(call);
   return res;
}

Value emit_printf(char *fmt, Value *args, int n) {
   return emit_printf_fd(1, fmt, args, n);
}

Value print_adapt(Type type, Value v, char **spec) {
   switch (type) {
      case I32:   *spec = "%d";   return v;
      case I64:   *spec = "%lld"; return v;
      case U32:   *spec = "%u";   return v;
      case U64:   *spec = "%llu"; return v;
      case PTR:   *spec = "%s";   return v;
      case I8: case I16:
         *spec = "%d";
         return LLVMBuildSExt(ura.builder, v, ura.i32, "s2i");
      case U8: case U16:
         *spec = "%u";
         return LLVMBuildZExt(ura.builder, v, ura.i32, "u2i");
      case CHAR:
         *spec = "%c";
         return LLVMBuildSExt(ura.builder, v, ura.i32, "c2i");
      case F32:
         *spec = "%f";
         return LLVMBuildFPExt(ura.builder, v, ura.f64, "f2d");
      case F64: *spec = "%f"; return v;
      case BOOL: {
         *spec    = "%s";
         Value ts = llvm_string("True", "true_str");
         Value fs = llvm_string("False", "false_str");
         return LLVMBuildSelect(ura.builder, v, ts, fs, "bool_str");
      }
      default: *spec = "?"; return NULL;
   }
}

TypeRef out_frame_type() {
   TypeRef t = LLVMGetTypeByName(ura.module, "__out_frame");
   if (t) return t;
   t = LLVMStructCreateNamed(ura.context, "__out_frame");
   TypeRef body[3] = { pointer_to(ura.i8), ura.i32, pointer_to(t) };
   LLVMStructSetBody(t, body, 3, 0);
   return t;
}

void emit_out_call(Node *def, Value ptr, Value frame, int fd) {
   Node *printer = find_printer(def);
   if (printer) {
      Token *fn = printer->token;
      emit_signature(printer);
      Value self  = ptr;
      Value slice = llvm_call(fn->llvm.func_type, fn->llvm.elem, &self, 1, "out");
      Value len   = llvm_extract(slice, 1, "out.len");
      Value data  = llvm_extract(slice, 0, "out.data");
      Value n     = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
      emit_printf_fd(fd, "%.*s", (Value[]){ n, data }, 2);
      return;
   }
   // The generated printer has its stream baked into its body, so stdout and
   // stderr need two distinct functions -- reusing __out_T for errput would
   // silently write struct dumps to fd 1.
   Token *token = def->token;
   Value *memo  = fd == 1 ? &token->llvm.elem : &token->llvm.err_out;
   if (!*memo) {
      TypeRef sty  = struct_type_of(def);
      TypeRef frt  = out_frame_type();
      TypeRef fty  = LLVMFunctionType(ura.vd,
                        (TypeRef[]){ pointer_to(sty), pointer_to(frt) }, 2, 0);
      char   *name = format(fd == 1 ? "__out_%s" : "__err_%s", token->name);
      Value   fn   = LLVMAddFunction(ura.module, name, fty);
      *memo                 = fn;
      token->llvm.func_type = fty;
      free(name);

      Block prev   = LLVMGetInsertBlock(ura.builder);
      Block entry  = llvm_block(fn, "entry");
      llvm_at(entry);
      Value self   = LLVMGetParam(fn, 0);
      Value parent = LLVMGetParam(fn, 1);
      Value me     = LLVMBuildBitCast(ura.builder, self, pointer_to(ura.i8), "me");

      Value walk  = llvm_alloca(pointer_to(frt), "walk");
      llvm_store(parent, walk);
      Block sc    = llvm_block(fn, "seen.cond");
      Block sb    = llvm_block(fn, "seen.body");
      Block hit   = llvm_block(fn, "seen.hit");
      Block miss  = llvm_block(fn, "seen.next");
      Block fresh = llvm_block(fn, "seen.fresh");
      llvm_br(sc);
      llvm_at(sc);
      Value node = llvm_load(pointer_to(frt), walk, "q");
      Value done = llvm_icmp(LLVMIntEQ,
                      LLVMBuildPtrToInt(ura.builder, node, ura.i64, "q2i"),
                      const_i64(0), "atroot");
      llvm_cond_br(done, fresh, sb);
      llvm_at(sb);
      Value pidx[2] = { const_i32(0), const_i32(0) };
      Value tidx[2] = { const_i32(0), const_i32(1) };
      Value nidx[2] = { const_i32(0), const_i32(2) };
      static int next = 1;
      if (!token->Struct.index) token->Struct.index = next++;
      Value myid = const_i32(token->Struct.index);
      Value held = llvm_load(pointer_to(ura.i8),
                      llvm_gep(frt, node, pidx, 2, "q.ptr"), "held");
      Value hid  = llvm_load(ura.i32,
                      llvm_gep(frt, node, tidx, 2, "q.ty"), "heldty");
      Value hit_ptr = llvm_icmp(LLVMIntEQ,
                      LLVMBuildPtrToInt(ura.builder, held, ura.i64, "h2i"),
                      LLVMBuildPtrToInt(ura.builder, me, ura.i64, "m2i"), "sameptr");
      Value hit_ty  = llvm_icmp(LLVMIntEQ, hid, myid, "samety");
      Value same    = llvm_binop(LLVMAnd, hit_ptr, hit_ty, "same");
      llvm_cond_br(same, hit, miss);
      llvm_at(hit);
      emit_printf_fd(fd, "[Circular]", NULL, 0);
      LLVMBuildRetVoid(ura.builder);
      llvm_at(miss);
      llvm_store(llvm_load(pointer_to(frt),
                    llvm_gep(frt, node, nidx, 2, "q.prev"), "up"), walk);
      llvm_br(sc);

      llvm_at(fresh);
      Value frame = llvm_alloca(frt, "frame");
      llvm_store(me,     llvm_gep(frt, frame, pidx, 2, "f.ptr"));
      llvm_store(myid,   llvm_gep(frt, frame, tidx, 2, "f.ty"));
      llvm_store(parent, llvm_gep(frt, frame, nidx, 2, "f.prev"));

      char *open = format("%s{", token->name);
      emit_printf_fd(fd, open, NULL, 0);
      free(open);
      int shown = 0;
      for (int i = 0; i < def->children_count; i++) {
         Token *field = def->children[i]->token;
         if (!is_field(field)) continue;
         char *label = format(shown ? ", %s: " : "%s: ", field->name);
         emit_printf_fd(fd, label, NULL, 0);
         free(label);
         shown++;
         Value idx[2] = { const_i32(0), const_i32(field->Struct.index) };
         Value slot = llvm_gep(sty, self, idx, 2, field->name);
         if (field->is_ref) {
            TypeRef tty = struct_type_of(field->Struct.ptr);
            Value   ptr = llvm_load(pointer_to(tty), slot, "ref");
            Value   fn  = here_func();
            Block   nb  = llvm_block(fn, "out.null");
            Block   vb  = llvm_block(fn, "out.ref");
            Block   cb  = llvm_block(fn, "out.refend");
            Value   iv  = LLVMBuildPtrToInt(ura.builder, ptr, ura.i64, "p2i");
            llvm_cond_br(llvm_icmp(LLVMIntEQ, iv, const_i64(0), "isnull"), nb, vb);
            llvm_at(nb);
            emit_printf_fd(fd, "null", NULL, 0);
            llvm_br(cb);
            llvm_at(vb);
            emit_printf_fd(fd, "ref ", NULL, 0);
            emit_out_call(field->Struct.ptr, ptr, frame, fd);
            llvm_br(cb);
            llvm_at(cb);
         } else if (field->ret_type == STRUCT_CALL) {
            emit_out_call(field->Struct.ptr, slot, frame, fd);
         } else if (field->ret_type == ARRAY_TYPE) {
            emit_out_array(field, slot, frame, field->Array.depth, fd);
         } else {
            char *spec = NULL;
            Value v    = llvm_load(llvm_type_of(field), slot, "f");
            v          = print_adapt(field->ret_type, v, &spec);
            if (!v) emit_printf_fd(fd, "?", NULL, 0);
            else    emit_printf_fd(fd, spec, (Value[]){ v }, 1);
         }
      }
      emit_printf_fd(fd, "}", NULL, 0);
      LLVMBuildRetVoid(ura.builder);
      if (prev) llvm_at(prev);
   }
   TypeRef fty = def->token->llvm.func_type;
   llvm_call(fty, *memo, (Value[]){ ptr, frame }, 2, "");
}

void emit_out_array(Token *field, Value slot, Value frame, int depth,
                    int fd) {
   Value   slice = llvm_load(array_type(field, depth), slot, "arr");
   Value   data  = llvm_extract(slice, 0, "arr.data");
   Value   len   = llvm_extract(slice, 1, "arr.len");
   if (depth == 1 && field->Array.sub_type == CHAR) {
      Value n = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
      emit_printf_fd(fd, "%.*s", (Value[]){ n, data }, 2);
      return;
   }
   TypeRef ety   = elem_type(field, depth);
   Value   fn    = here_func();
   Value   slot_i = llvm_alloca(ura.i64, "oi");
   llvm_store(const_i64(0), slot_i);
   emit_printf_fd(fd, "[", NULL, 0);
   Block cond = llvm_block(fn, "out.arr.cond");
   Block body = llvm_block(fn, "out.arr.body");
   Block sep  = llvm_block(fn, "out.arr.sep");
   Block item = llvm_block(fn, "out.arr.item");
   Block end  = llvm_block(fn, "out.arr.end");
   llvm_br(cond);
   llvm_at(cond);
   Value i = llvm_load(ura.i64, slot_i, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
   llvm_at(body);
   llvm_cond_br(llvm_icmp(LLVMIntSGT, i, const_i64(0), "notfirst"), sep, item);
   llvm_at(sep);
   emit_printf_fd(fd, ", ", NULL, 0);
   llvm_br(item);
   llvm_at(item);
   Value at = llvm_gep(ety, data, &i, 1, "at");
   if (depth > 1)
      emit_out_array(field, at, frame, depth - 1, fd);
   else if (field->Array.sub_type == STRUCT_CALL)
      emit_out_call(field->Array.struct_ptr, at, frame, fd);
   else {
      char *spec = NULL;
      Value v    = print_adapt(field->Array.sub_type,
                               llvm_load(ety, at, "e"), &spec);
      if (v) emit_printf_fd(fd, spec, (Value[]){ v }, 1);
      else   emit_printf_fd(fd, "?", NULL, 0);
   }
   llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "n"), slot_i);
   llvm_br(cond);
   llvm_at(end);
   emit_printf_fd(fd, "]", NULL, 0);
}

Value struct_arg_ptr(Node *arg) {
   Token *token = arg->token;
   if (includes(token->type, ID, ACCESS, DOT, 0) && !token->is_dec) {
      Value slot = emit_place(arg);
      if (token->type != DOT || !token->is_ref) return slot;
      TypeRef sty = pointer_to(struct_type_of(token->Struct.ptr));
      Value   ptr = llvm_load(sty, slot, "ref");
      if (token->is_nullable) guard_bound(token, ptr);
      return ptr;
   }
   code_gen(arg);
   if (category(arg) == CAT_REF)
      return token->llvm.elem;
   Value tmp = llvm_alloca(struct_type_of(token->Struct.ptr), "out.tmp");
   llvm_store(token->llvm.elem, tmp);
   if (needs_drop(token->Struct.ptr)) {
      Token *temp     = new_token(ID, token->indent);
      temp->ret_type  = STRUCT_CALL;
      temp->Struct    = token->Struct;
      temp->llvm.elem = tmp;
      resize_array(ura.temps, Token *);
      ura.temps[ura.temps_count++] = temp;
   }
   return tmp;
}

bool program_throws(Node *n) {
   if (!n) return false;
   if (includes(n->token->type, THROW, TRY, 0)) return true;
   // break/continue->left is a back-edge to the loop; recursing loops forever
   if (includes(n->token->type, BREAK, CONTINUE, 0)) return false;
   if (program_throws(n->left) || program_throws(n->right)) return true;
   for (int i = 0; i < n->children_count; i++)
      if (program_throws(n->children[i])) return true;
   return false;
}

void ensure_err_globals() {
   if (ura.err_flag) return;
   ura.error_def = find_struct("Error");
   ura.err_flag  = LLVMAddGlobal(ura.module, ura.i1, "ura.err_flag");
   LLVMSetInitializer(ura.err_flag, LLVMConstInt(ura.i1, 0, 0));
   LLVMSetLinkage(ura.err_flag, LLVMInternalLinkage);
   TypeRef et    = struct_type_of(ura.error_def);
   ura.err_value = LLVMAddGlobal(ura.module, et, "ura.err_value");
   LLVMSetInitializer(ura.err_value, LLVMConstNull(et));
   LLVMSetLinkage(ura.err_value, LLVMInternalLinkage);
}

void emit_uncaught() {
   TypeRef et     = struct_type_of(ura.error_def);
   TypeRef mt     = LLVMStructGetTypeAtIndex(et, 0);
   Value   idx[2] = { const_i32(0), const_i32(0) };
   Value   msgp   = llvm_gep(et, ura.err_value, idx, 2, "err.msg");
   Value   msg    = llvm_load(mt, msgp, "msg");
   Value   data   = llvm_extract(msg, 0, "msg.data");
   Value   len    = llvm_extract(msg, 1, "msg.len");
   TypeRef wty    = NULL;
   Value   wfn    = lib_fn("write", &wty);
   if (wfn) llvm_call(wty, wfn, (Value[]){ const_i32(2), data, len }, 3, "");
   TypeRef xty    = NULL;
   Value   xfn    = lib_fn("exit", &xty);
   if (xfn) llvm_call(xty, xfn, (Value[]){ const_i32(1) }, 1, "");
   LLVMBuildUnreachable(ura.builder);
}

Node *current_fdec() {
   for (int i = ura.scopes_count; i >= 1; i--)
      if (ura.scopes[i] && ura.scopes[i]->token->type == FDEC)
         return ura.scopes[i];
   return NULL;
}

void emit_throw_branch() {
   // 1. enclosing try in THIS fn -> drop its scope, jump to the catch
   if (ura.try_nodes_count > 0) {
      Node *tn = ura.try_nodes[ura.try_nodes_count - 1];
      emit_unwind(tn, NULL);
      llvm_br(tn->token->llvm._catch);
      return;
   }
   // 2. no try + we are main -> nothing above catches it: print + exit
   Node *fdec = current_fdec();
   if (!fdec || is_main(fdec->token)) {
      emit_uncaught();
      return;
   }
   // 3. no try in a callee -> ret (err_flag stays set; the caller's
   //    post-call check picks it up and keeps unwinding)  deep()->middle()->main
   emit_unwind(NULL, NULL);
   TypeRef rt = LLVMGetReturnType(fdec->token->llvm.func_type);
   if (LLVMGetTypeKind(rt) == LLVMVoidTypeKind)
      LLVMBuildRetVoid(ura.builder);
   else
      LLVMBuildRet(ura.builder, LLVMConstNull(rt));
}

void code_gen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         Token *token = node->token;
         if (token->is_proto) return;
         Token *prev_ret = ura.fn_ret;
         ura.fn_ret = token;
         emit_signature(node);
         token->llvm.prev_block = here_block();
         token->llvm.prev_scope = ura.debug_scope;
         token->llvm.prev_loc   = NULL;
         if (ura.debug_builder)
            token->llvm.prev_loc = LLVMGetCurrentDebugLocation2(ura.builder);
         llvm_at(llvm_block(token->llvm.elem, "entry"));
         if (ura.enable_san) {
            unsigned     kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
            AttributeRef attr = LLVMCreateEnumAttribute(ura.context, kind, 0);
            LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex, attr);
         }
         if (ura.debug_builder) {
            MetadataRef di_type = LLVMDIBuilderCreateSubroutineType(
               ura.debug_builder, ura.debug_file, NULL, 0, LLVMDIFlagZero);
            size_t len = strlen(token->name);
            MetadataRef di_func = LLVMDIBuilderCreateFunction(ura.debug_builder,
               ura.debug_compile_unit, token->name, len, token->name, len,
               ura.debug_file, token->line, di_type, 0, 1, token->line,
               LLVMDIFlagZero, 0);
            LLVMSetSubprogram(token->llvm.elem, di_func);
            ura.debug_scope = di_func;
            llvm_set_location(llvm_di_location(token->line, di_func));
         }
         enter_scope(node);
         for (int i = 0; i < token->Fn.params_count; i++) {
            Token  *param = token->Fn.params[i];
            TypeRef pt    = llvm_type_of(param);
            if (param->is_ref) pt = pointer_to(pt);
            param->llvm.elem = llvm_alloca(pt, param->name);
            llvm_store(LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
         }
         if (is_main(token)) {
            for (int i = 0; i < ura.head->children_count; i++) {
               Token *os = global_decl(ura.head->children[i]);
               if (!os || !os->name || strcmp(os->name, "os") != 0) continue;
               if (os->ret_type != STRUCT_CALL || !os->Struct.ptr) break;
               if (!os->used) break;
               Value fn = here_func();
               {
                  Value   argc  = LLVMGetParam(fn, 0);
                  Value   argv  = LLVMGetParam(fn, 1);
                  TypeRef sty   = struct_type_of(os->Struct.ptr);
                  Value   base  = os->llvm.elem;
                  llvm_store(argc, LLVMBuildStructGEP2(ura.builder, sty, base, 0, "os.argc"));

                  TypeRef outer = LLVMStructGetTypeAtIndex(sty, 1);
                  TypeRef strt  = LLVMGetElementType(LLVMStructGetTypeAtIndex(outer, 0));
                  Value   n64   = LLVMBuildSExt(ura.builder, argc, ura.i64, "argc64");
                  TypeRef cty   = NULL;
                  Value   cfn   = lib_fn("calloc", &cty);
                  Value   raw   = llvm_call(cty, cfn, (Value[]){ n64, const_i64(16) }, 2, "argvbuf");
                  Value   buf   = LLVMBuildBitCast(ura.builder, raw, pointer_to(strt), "argv.buf");

                  TypeRef lty   = NULL;
                  Value   lfn   = lib_fn("strlen", &lty);
                  Block   cond  = llvm_block(fn, "os.cond");
                  Block   body  = llvm_block(fn, "os.body");
                  Block   end   = llvm_block(fn, "os.end");
                  Value   slot  = llvm_alloca(ura.i64, "oi");
                  llvm_store(const_i64(0), slot);
                  llvm_br(cond);
                  llvm_at(cond);
                  Value i = llvm_load(ura.i64, slot, "i");
                  llvm_cond_br(llvm_icmp(LLVMIntSLT, i, n64, "more"), body, end);
                  llvm_at(body);
                  TypeRef i8p = pointer_to(ura.i8);
                  Value   src = llvm_load(i8p, LLVMBuildGEP2(ura.builder, i8p, argv, &i, 1, "ap"), "arg");
                  Value   len = llvm_call(lty, lfn, (Value[]){ src }, 1, "alen");
                  Value   sl  = llvm_insert(LLVMGetUndef(strt), src, 0, "a.ptr");
                  sl          = llvm_insert(sl, len, 1, "a.len");
                  llvm_store(sl, LLVMBuildGEP2(ura.builder, strt, buf, &i, 1, "slot"));
                  llvm_store(llvm_binop(LLVMAdd, i, const_i64(1), "inc"), slot);
                  llvm_br(cond);
                  llvm_at(end);
                  Value agg = llvm_insert(LLVMGetUndef(outer), buf, 0, "argv.ptr");
                  agg       = llvm_insert(agg, n64, 1, "argv.len");
                  llvm_store(agg, LLVMBuildStructGEP2(ura.builder, sty, base, 1, "os.argv"));
               }
               break;
            }
            for (int i = 0; i < ura.head->children_count; i++) {
               Node *child = ura.head->children[i];
               if (child->token->type != ASSIGN || !global_decl(child)) continue;
               code_gen(child);
               drop_temps();
            }
         }
         for (int i = 0; i < node->children_count; i++) {
            Token *st = node->children[i]->token;
            if (ura.debug_builder && ura.debug_scope)
               llvm_set_location(llvm_di_location(st->line, ura.debug_scope));
            code_gen(node->children[i]);
            drop_temps();
            // throw/return closes the block; stop or the next stmt is dead code
            //   throw Error::make(..)  \n  output(..)  <- unreachable
            if (LLVMGetBasicBlockTerminator(here_block())) break;
         }
         scope_out();
         if (is_main(token)) emit_drops(ura.head, NULL);
         if (!LLVMGetBasicBlockTerminator(here_block())) {
            if (token->ret_type == VOID) LLVMBuildRetVoid(ura.builder);
            else LLVMBuildRet(ura.builder, default_value(token));
         }
         if (token->llvm.prev_block) llvm_at(token->llvm.prev_block);
         if (ura.debug_builder) {
            ura.debug_scope = token->llvm.prev_scope;
            llvm_set_location(token->llvm.prev_loc);
         }
         ura.fn_ret = prev_ret;
         break;
      }
      case ID: {
         Token *token = node->token;
         if (token->is_dec) {
            if (token->is_global) return;
            TypeRef t = llvm_type_of(token);
            if (token->is_ref) t = pointer_to(t);
            token->llvm.elem = llvm_alloca(t, token->name);
            Value init = token->is_ref ? LLVMConstNull(t) : default_value(token);
            llvm_store(init, token->llvm.elem);
            return;
         }
         Token  *decl = token->Decl.ptr;
         TypeRef t    = llvm_type_of(decl);
         if (!decl->is_ref) {
            token->llvm.elem = llvm_load(t, decl->llvm.elem, token->name);
            return;
         }
         Value ptr = llvm_load(pointer_to(t), decl->llvm.elem, "ref");
         if (token->is_nullable) guard_bound(token, ptr);
         token->llvm.elem = llvm_load(t, ptr, token->name);
         break;
      }
      case FCALL: {
         Token *token    = node->token;
         bool   indirect = token->Fcall.var != NULL;
         Token *fn       = indirect ? token->Fcall.var : token->Fcall.ptr->token;
         if (!indirect) emit_signature(token->Fcall.ptr);
         int    self = token->is_method_call ? 1 : 0;
         int    n    = node->children_count + self;
         Value *args = NULL;
         if (n > 0) {
            args = allocate(n, sizeof(Value));
            if (self) args[0] = struct_arg_ptr(node->left);
            for (int i = 0; i < node->children_count; i++) {
               Token *arg = node->children[i]->token;
               code_gen(node->children[i]);
               args[i + self] = arg->llvm.elem;
               bool extra = fn->is_variadic && i + self >= fn->Fn.params_count;
               Type want  = extra ? PTR : fn->Fn.params[i + self]->ret_type;
               Value dv   = args[i + self];
               bool  keep = arg->ret_type != ARRAY_TYPE || want == ARRAY_TYPE
                            || !is_pointer(want);
               args[i + self] = keep ? dv : llvm_extract(dv, 0, "arr.data");
               if (extra && arg->ret_type != ARRAY_TYPE) {
                  Value pv = args[i + self];
                  Value pr = pv;
                  Type  pt = arg->ret_type;
                  if (pt == F32)
                     pr = LLVMBuildFPExt(ura.builder, pv, ura.f64, "f2d");
                  else if (pt == I8 || pt == I16 || pt == CHAR)
                     pr = LLVMBuildSExt(ura.builder, pv, ura.i32, "i2i");
                  else if (pt == BOOL || pt == U8 || pt == U16)
                     pr = LLVMBuildZExt(ura.builder, pv, ura.i32, "u2i");
                  args[i + self] = pr;
               }
            }
         }
         if (indirect) {
            TypeRef ptr_type = llvm_type_of(fn);
            Value   fn_ptr   = llvm_load(ptr_type, fn->llvm.elem, "fn");
            Value   null     = LLVMConstNull(LLVMTypeOf(fn_ptr));
            Value   isnull   = llvm_icmp(LLVMIntEQ, fn_ptr, null, "isnull");
            guard(token, isnull, "Call to a null function value");
            char   *name     = fn->Fn.ret->ret_type == VOID ? "" : "call";
            token->llvm.elem = llvm_call(LLVMGetElementType(ptr_type), fn_ptr, args, n, name);
         } else {
            char *name       = fn->ret_type == VOID ? "" : "call";
            token->llvm.elem = llvm_call(fn->llvm.func_type, fn->llvm.elem, args, n, name);
         }
         free(args);
         // after any call, a callee may have thrown; check + unwind. gated so
         // non-throwing programs pay nothing:  f()  ->  if err_flag: unwind
         if (ura.uses_exceptions) {
            ensure_err_globals();
            Value fnv    = here_func();
            Block unwind = llvm_block(fnv, "throw.unwind");
            Block cont   = llvm_block(fnv, "throw.cont");
            Value flag   = llvm_load(ura.i1, ura.err_flag, "eflag");
            llvm_cond_br(flag, unwind, cont);
            llvm_at(unwind);
            emit_throw_branch();
            llvm_at(cont);
         }
         break;
      }
      case ERRPUT:
      case OUTPUT: {
         int    fd    = node->token->type == ERRPUT ? 2 : 1;
         char  *fmt   = allocate(node->children_count * 8 + 16, 1);
         int    fc    = 0;
         Value *args  = allocate(node->children_count * 2 + 1, sizeof(Value));
         int    nargs = 0;
         Value  last  = NULL;

         if (fd == 2) emit_printf_fd(fd, "\033[0;31m", NULL, 0);
         for (int i = 0; i < node->children_count; i++) {
            Node  *arg   = node->children[i];
            Token *token = arg->token;
            if (token->ret_type == STRUCT_CALL) {
               if (fc) { fmt[fc] = 0; last = emit_printf_fd(fd, fmt, args, nargs); }
               fc    = 0;
               nargs = 0;
               emit_out_call(token->Struct.ptr, struct_arg_ptr(arg),
                             LLVMConstNull(pointer_to(out_frame_type())), fd);
               continue;
            }
            code_gen(arg);
            if (is_string(token)) {
               Value len = llvm_extract(token->llvm.elem, 1, "str.len");
               args[nargs++] = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
               args[nargs++] = llvm_extract(token->llvm.elem, 0, "str.data");
               for (char *s = "%.*s"; *s; s++) fmt[fc++] = *s;
               continue;
            }
            if (token->ret_type == ARRAY_TYPE) {
               if (fc) { fmt[fc] = 0; last = emit_printf_fd(fd, fmt, args, nargs); }
               fc    = 0;
               nargs = 0;
               int     depth = token->Array.depth;
               TypeRef at    = array_type(token, depth);
               Value   slot  = LLVMBuildAlloca(ura.builder, at, "arr.tmp");
               llvm_store(token->llvm.elem, slot);
               emit_out_array(token, slot,
                              LLVMConstNull(pointer_to(out_frame_type())), depth, fd);
               continue;
            }
            char *spec = NULL;
            Value v    = print_adapt(token->ret_type, token->llvm.elem, &spec);
            if (!v) { fmt[fc++] = '?'; continue; }
            for (char *s = spec; *s; s++) fmt[fc++] = *s;
            args[nargs++] = v;
         }
         if (fc) { fmt[fc] = 0; last = emit_printf_fd(fd, fmt, args, nargs); }
         if (fd == 2) last = emit_printf_fd(fd, "\033[0m", NULL, 0);
         node->token->llvm.elem = last;
         free(fmt);
         free(args);
         break;
      }
      case ARRAY_LIT: {
         Token  *token = node->token;
         int     n     = node->children_count;
         int     depth = token->Array.depth;
         TypeRef elem  = elem_type(token, depth);
         Value   len   = const_i64(n);
         Value   data  = LLVMBuildArrayAlloca(ura.builder, elem, len, "arr");
         for (int i = 0; i < n; i++) {
            code_gen(node->children[i]);
            Value idx = const_i64(i);
            Value gep = llvm_gep(elem, data, &idx, 1, "arr.init");
            llvm_store(node->children[i]->token->llvm.elem, gep);
         }
         TypeRef slice = array_type(token, depth);
         Value   agg   = LLVMGetUndef(slice);
         agg = llvm_insert(agg, data, 0, "arr.ptr");
         agg = llvm_insert(agg, len,  1, "arr.len");
         token->llvm.elem = agg;
         break;
      }
      case ARRAY: {
         Token *token = node->token;
         int    depth = token->Array.depth;
         Value *dims  = allocate(depth, sizeof(Value));
         for (int i = 0; i < depth; i++) {
            code_gen(node->children[i]);
            dims[i] = LLVMBuildIntCast2(ura.builder, node->children[i]->token->llvm.elem, ura.i64, 1, "n");
         }
         token->llvm.elem = build_array(token, dims, depth, token->is_heap);
         free(dims);
         break;
      }
      case NEW: {
         TypeRef sty = struct_type_of(token->Struct.ptr);
         Value   esz = LLVMConstInt(ura.i64,
            LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), sty), 0);
         token->llvm.elem = array_calloc(sty, const_i64(1), esz);
         break;
      }
      case ACCESS: {
         if (node->right->token->type == RANGE) {
            Token *arr   = node->left->token;
            Node  *range = node->right;
            code_gen(node->left);
            Value   slice = node->left->token->llvm.elem;
            Value   data  = llvm_extract(slice, 0, "arr.data");
            code_gen(range->left);
            code_gen(range->right);
            Value   start = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i64, 1, "start");
            Value   end   = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i64, 1, "end");
            if (node->token->is_nullable) {
               Value len = llvm_extract(slice, 1, "arr.len");
               Value lo  = llvm_icmp(LLVMIntSLT, start, const_i64(0), "s.lo");
               Value hi  = llvm_icmp(LLVMIntSGT, end, len, "e.hi");
               Value ord = llvm_icmp(LLVMIntSGT, start, end, "s.gt");
               Value bad = LLVMBuildOr(ura.builder, LLVMBuildOr(ura.builder, lo, hi, "b"), ord, "bad");
               guard(node->token, bad, "slice range out of bounds");
            }
            TypeRef elem  = elem_type(arr, arr->Array.depth);
            Value   ptr   = llvm_gep(elem, data, &start, 1, "slice.data");
            Value   len   = LLVMBuildSub(ura.builder, end, start, "slice.len");
            node->token->llvm.elem = make_slice(arr, arr->Array.depth, ptr, len);
            return;
         }
         Value   ptr  = access_ptr(node);
         TypeRef elem = llvm_type_of(node->token);
         node->token->llvm.elem = llvm_load(elem, ptr, "idx");
         break;
      }
      case DOT: {
         Token *token = node->token;
         if (node->left->token->ret_type == STRUCT_CALL) {
            Value ptr = field_ptr(node);
            token->llvm.elem = llvm_load(llvm_type_of(token), ptr, token->name);
            return;
         }
         code_gen(node->left);
         token->llvm.elem = llvm_extract(node->left->token->llvm.elem, 1, "len");
         break;
      }
      case TYPEOF: {
         Token *arg  = node->left->token;
         char  *name = arg->ret_type == STRUCT_CALL ? struct_name_of(arg)
                     : type_name(arg->ret_type);
         node->token->llvm.elem = string_slice(node->token, name);
         break;
      }
      case SIZEOF: {
         TypeRef t = llvm_type_of(node->left->token);
         unsigned long long sz = LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), t);
         node->token->llvm.elem = const_i64(sz);
         break;
      }
      case CLEAN: {
         Token *tgt = node->left->token;
         if (tgt->ret_type == STRUCT_CALL && tgt->is_ref) {
            Value   ptr  = emit_ref(node->left);
            Value   slot;
            if (tgt->type == DOT) slot = field_ptr(node->left);
            else {
               Token *decl = tgt->is_dec ? tgt : tgt->Decl.ptr;
               slot = decl->llvm.elem;
            }
            TypeRef pty  = LLVMTypeOf(ptr);
            Value   fnv  = here_func();
            Block   live = llvm_block(fnv, "clean.live");
            Block   done = llvm_block(fnv, "clean.done");
            Value   nil  = llvm_icmp(LLVMIntEQ, ptr, LLVMConstNull(pty), "isnull");
            llvm_cond_br(nil, done, live);
            llvm_at(live);
            emit_drop_value(ptr, tgt->Struct.ptr);
            TypeRef fty = NULL;
            Value   ffn = lib_fn("free", &fty);
            Value   raw = LLVMBuildBitCast(ura.builder, ptr, pointer_to(ura.i8), "free.ptr");
            if (ffn) llvm_call(fty, ffn, (Value[]){ raw }, 1, "");
            llvm_br(done);
            llvm_at(done);
            llvm_store(LLVMConstNull(pty), slot);
            break;
         }
         Value   slot  = emit_place(node->left);
         TypeRef sty   = array_type(tgt, tgt->Array.depth);
         Value   slice = llvm_load(sty, slot, "arr");
         free_array(tgt, slice, tgt->Array.depth);
         llvm_store(LLVMConstNull(sty), slot);
         break;
      }
      case IF: {
         Value fn  = here_func();
         Block end = llvm_block(fn, "endif");
         for (Node *cur = node; cur; cur = cur->right) {
            if (cur->token->type == ELSE) {
               enter_scope(cur);
               code_gen_body(cur);
               scope_out();
               if (!LLVMGetBasicBlockTerminator(here_block()))
                  llvm_br(end);
               break;
            }
            Block body = llvm_block(fn, "then");
            Block next = cur->right ? llvm_block(fn, "next") : end;
            code_gen(cur->left);
            llvm_cond_br(cur->left->token->llvm.elem, body, next);
            llvm_at(body);
            enter_scope(cur);
            code_gen_body(cur);
            scope_out();
            if (!LLVMGetBasicBlockTerminator(here_block()))
               llvm_br(end);
            llvm_at(next);
         }
         llvm_at(end);
         break;
      }
      case WHILE: {
         Value fn   = here_func();
         Block cond = llvm_block(fn, "while.cond");
         Block body = llvm_block(fn, "while.body");
         Block end  = llvm_block(fn, "while.end");
         node->token->llvm.start = cond;
         node->token->llvm.end   = end;
         llvm_br(cond);
         llvm_at(cond);
         code_gen(node->left);
         llvm_cond_br(node->left->token->llvm.elem, body, end);
         llvm_at(body);
         enter_scope(node);
         code_gen_body(node);
         scope_out();
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(cond);
         llvm_at(end);
         break;
      }
      case LOOP: {
         Value fn   = here_func();
         Block body = llvm_block(fn, "loop.body");
         Block end  = llvm_block(fn, "loop.end");
         node->token->llvm.start = body;
         node->token->llvm.end   = end;
         llvm_br(body);
         llvm_at(body);
         enter_scope(node);
         code_gen_body(node);
         scope_out();
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(body);
         llvm_at(end);
         break;
      }
      case FOR: {
         if (node->right->token->type != RANGE) {
            Token  *var  = node->left->token;
            bool    ref  = node->token->is_ref;
            Token  *arr  = node->right->token;
            code_gen(node->right);
            Value   slice = arr->llvm.elem;
            Value   data  = llvm_extract(slice, 0, "arr.data");
            Value   len   = llvm_extract(slice, 1, "arr.len");
            TypeRef elem  = elem_type(arr, arr->Array.depth);
            Value   fn    = here_func();
            Value   idx   = llvm_alloca(ura.i64, "idx");
            llvm_store(const_i64(0), idx);
            Value   xslot = ref ? NULL : llvm_alloca(elem, var->name);
            if (!ref) var->llvm.elem = xslot;
            Block   cond  = llvm_block(fn, "for.cond");
            Block   body  = llvm_block(fn, "for.body");
            Block   inc   = llvm_block(fn, "for.inc");
            Block   end   = llvm_block(fn, "for.end");
            node->token->llvm.start = inc;
            node->token->llvm.end   = end;
            llvm_br(cond);
            llvm_at(cond);
            Value   i = llvm_load(ura.i64, idx, "i");
            llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
            llvm_at(body);
            Value   gep = llvm_gep(elem, data, &i, 1, "elem");
            if (ref) var->llvm.elem = gep;
            else     llvm_store(llvm_load(elem, gep, "x"), xslot);
            enter_scope(node);
            code_gen_body(node);
            scope_out();
            if (!LLVMGetBasicBlockTerminator(here_block()))
               llvm_br(inc);
            llvm_at(inc);
            Value   iv = llvm_load(ura.i64, idx, "i");
            llvm_store(LLVMBuildAdd(ura.builder, iv, const_i64(1), "next"), idx);
            llvm_br(cond);
            llvm_at(end);
            return;
         }
         Token *var   = node->left->token;
         Node  *range = node->right;
         code_gen(range->left);
         code_gen(range->right);
         Value a    = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i32, 1, "a");
         Value b    = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i32, 1, "b");
         Value asc  = llvm_icmp(LLVMIntSLT, a, b, "asc");
         Value mag  = const_i32(1);
         if (range->children_count) {
            code_gen(range->children[0]);
            Value raw = range->children[0]->token->llvm.elem;
            mag = LLVMBuildIntCast2(ura.builder, raw, ura.i32, 1, "by");
         }
         Value down = LLVMBuildNeg(ura.builder, mag, "by.neg");
         Value step = LLVMBuildSelect(ura.builder, asc, mag, down, "step");
         Value fn   = here_func();
         Value slot = llvm_alloca(ura.i32, var->name);
         llvm_store(a, slot);
         var->llvm.elem = slot;
         Block cond = llvm_block(fn, "for.cond");
         Block body = llvm_block(fn, "for.body");
         Block inc  = llvm_block(fn, "for.inc");
         Block end  = llvm_block(fn, "for.end");
         node->token->llvm.start = inc;
         node->token->llvm.end   = end;
         llvm_br(cond);
         llvm_at(cond);
         Value i = llvm_load(ura.i32, slot, var->name);
         Value more = LLVMBuildSelect(ura.builder, asc,
                                      llvm_icmp(LLVMIntSLT, i, b, "lt"),
                                      llvm_icmp(LLVMIntSGT, i, b, "gt"), "more");
         llvm_cond_br(more, body, end);
         llvm_at(body);
         enter_scope(node);
         code_gen_body(node);
         scope_out();
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(inc);
         llvm_at(inc);
         Value iv = llvm_load(ura.i32, slot, var->name);
         llvm_store(LLVMBuildAdd(ura.builder, iv, step, "next"), slot);
         llvm_br(cond);
         llvm_at(end);
         break;
      }
      case MATCH: {
         Value fn   = here_func();
         Block end  = llvm_block(fn, "match.end");
         node->token->llvm.end = end;
         code_gen(node->left);
         Value subject = node->left->token->llvm.elem;
         bool  fp      = is_float(node->left->token->ret_type);
         enter_scope(node);
         for (int i = 0; i < node->children_count; i++) {
            Node *branch = node->children[i];
            if (branch->token->type == DEFAULT) {
               enter_scope(branch); code_gen_body(branch); scope_out();
               if (!LLVMGetBasicBlockTerminator(here_block()))
                  llvm_br(end);
               break;
            }
            Block body = llvm_block(fn, "case.body");
            Block next = i + 1 < node->children_count ? llvm_block(fn, "case.next") : end;
            Value cond = NULL;
            for (int j = 0; j < branch->left->children_count; j++) {
               code_gen(branch->left->children[j]);
               Value val = branch->left->children[j]->token->llvm.elem;
               Value eq  = fp ? llvm_fcmp(LLVMRealOEQ, subject, val, "feq")
                              : llvm_icmp(LLVMIntEQ, subject, val, "eq");
               cond = cond ? LLVMBuildOr(ura.builder, cond, eq, "case.or") : eq;
            }
            llvm_cond_br(cond, body, next);
            llvm_at(body);
            enter_scope(branch); code_gen_body(branch); scope_out();
            if (!LLVMGetBasicBlockTerminator(here_block()))
               llvm_br(end);
            llvm_at(next);
         }
         scope_out();
         llvm_at(end);
         break;
      }
      case ASSIGN: {
         Token *token = node->token;
         if (token->Fcall.ptr) {
            code_gen_operator(node);
            return;
         }
         if (token->kind == REF_REBIND) {
            Node *left = node->left;
            if (left->token->is_dec) {
               Value dest = emit_place(left);
               token->llvm.elem = emit_ref(node->right);
               llvm_store(token->llvm.elem, dest);
               return;
            }
            Value ptr = emit_ref(node->right);
            Token *lt = left->token;
            Value slot;
            if (lt->type == DOT) slot = field_ptr(left);
            else {
               Token *decl = lt->is_dec ? lt : lt->Decl.ptr;
               slot = decl->llvm.elem;
            }
            llvm_store(ptr, slot);
            token->llvm.elem = ptr;
            return;
         }
         Value dest = emit_place(node->left);
         token->llvm.elem = emit_value(node->right);
         llvm_store(token->llvm.elem, dest);
         break;
      }
      case STRUCT_DEF:
         struct_type_of(node);
         for (int i = 0; i < node->children_count; i++)
            if (node->children[i]->token->type == FDEC)
               code_gen(node->children[i]);
         break;
      case MODULE:
         for (int i = 0; i < node->children_count; i++)
            code_gen(node->children[i]);
         break;
      case ENUM_DEF: break;

      case REF:      token->llvm.elem = emit_ref(node->left);   break;
      case THROW: {
         ensure_err_globals();
         code_gen(node->left);
         llvm_store(node->left->token->llvm.elem, ura.err_value);
         llvm_store(LLVMConstInt(ura.i1, 1, 0), ura.err_flag);
         drop_temps();
         emit_throw_branch();
         break;
      }
      case TRY: {
         ensure_err_globals();
         Value fn   = here_func();
         Block cblk = llvm_block(fn, "catch");
         Block end  = llvm_block(fn, "try.end");
         node->token->llvm._catch = cblk;
         // push while emitting the body so a throw inside it targets THIS catch
         resize_array(ura.try_nodes, Node *);
         ura.try_nodes[ura.try_nodes_count++] = node;
         enter_scope(node);
         code_gen_body(node);
         scope_out();
         ura.try_nodes_count--;
         // no br if the body already threw/returned (would double-terminate)
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(end);
         llvm_at(cblk);
         Node   *cnode = node->right;
         enter_scope(cnode);
         Value   slot  = emit_place(cnode->left);
         TypeRef et    = struct_type_of(ura.error_def);
         llvm_store(llvm_load(et, ura.err_value, "ev"), slot);
         llvm_store(LLVMConstInt(ura.i1, 0, 0), ura.err_flag);
         code_gen_body(cnode);
         scope_out();
         // catch may itself rethrow/return; only br when it fell through
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(end);
         llvm_at(end);
         break;
      }
      case BREAK: {
         drop_temps();
         emit_unwind(node->left, NULL);
         llvm_br(node->left->token->llvm.end);
         break;
      }
      case CONTINUE: {
         drop_temps();
         emit_unwind(node->left, NULL);
         llvm_br(node->left->token->llvm.start);
         break;
      }
      case I32: case BOOL: case CHARS:
      case CHAR: case F32: case NULL_LIT: {
         Token *token = node->token;
         switch (token->type) {
            case I32:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
            case BOOL:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
            case CHARS:
               token->llvm.elem = string_slice(token, token->Chars.value);
               break;
            case NULL_LIT:
               token->llvm.elem = LLVMConstNull(llvm_type_of(token));
               break;
            case CHAR:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Char.value, 0); break;
            case F32: token->llvm.elem = LLVMConstReal(to_llvm_type(token->ret_type), token->Float.value); break;
            default: break;
         }
         break;
      }
      case FN_TYPE: {
         emit_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;
      }
      case RETURN: {
         if (!node->left) {
            drop_temps();
            emit_unwind(NULL, NULL);
            token->llvm.elem = LLVMBuildRetVoid(ura.builder);
            break;
         }
         Token *out  = node->left->token;
         bool   ref  = token->kind == RET_REF;
         Value  val  = ref ? emit_ref(node->left) : emit_value(node->left);
         Token *keep = out->type == ID ? find_variable(out->name, NULL, NULL) : NULL;
         drop_temps();
         emit_unwind(NULL, keep);
         token->llvm.elem = LLVMBuildRet(ura.builder, val);
         break;
      }
      case NOT: case BNOT: {
         code_gen(node->left);
         Value nv = node->left->token->llvm.elem;
         token->llvm.elem = LLVMBuildNot(ura.builder, nv, "not");
         break;
      }
      case AS: {
         code_gen(node->left);
         Value value = node->left->token->llvm.elem;
         Type  src   = node->left->token->ret_type;
         Type  dst   = token->ret_type;
         if (src == dst) { token->llvm.elem = value; break; }
         TypeRef to = to_llvm_type(dst);
         if (is_float(src) && is_float(dst))
            token->llvm.elem = dst == F64
               ? LLVMBuildFPExt(ura.builder, value, to, "cast")
               : LLVMBuildFPTrunc(ura.builder, value, to, "cast");
         else if (is_float(src))
            token->llvm.elem = is_unsigned(dst)
               ? LLVMBuildFPToUI(ura.builder, value, to, "cast")
               : LLVMBuildFPToSI(ura.builder, value, to, "cast");
         else if (is_float(dst))
            token->llvm.elem = is_unsigned(src)
               ? LLVMBuildUIToFP(ura.builder, value, to, "cast")
               : LLVMBuildSIToFP(ura.builder, value, to, "cast");
         else
            token->llvm.elem = LLVMBuildIntCast2(ura.builder, value, to,
               !is_unsigned(src), "cast");
         break;
      }
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
      case DIV_ASSIGN: case MOD_ASSIGN: case BAND_ASSIGN:
      case BOR_ASSIGN: case BXOR_ASSIGN: case LSHIFT_ASSIGN:
      case RSHIFT_ASSIGN: {
         if (node->token->Fcall.ptr) {
            code_gen_operator(node);
            return;
         }
         Value   dest = emit_place(node->left);
         code_gen(node->right);
         Type    op      = node->token->type;
         Value   right   = node->right->token->llvm.elem;
         TypeRef type    = to_llvm_type(node->left->token->ret_type);
         Value   current = llvm_load(type, dest, "cur");
         Value   res     = right;
         Type    lt      = node->left->token->ret_type;
         bool    fp      = is_float(lt);
         bool    un      = is_unsigned(lt);
         bool    divides = includes(op, DIV_ASSIGN, MOD_ASSIGN, 0);
         if (divides) guard_nonzero(node->token, right);
#define ARITH(fop, sop, uop, fname, iname) res = fp \
   ? llvm_binop(fop, current, right, fname) \
   : llvm_binop(un ? uop : sop, current, right, iname)
         switch (op) {
            case ADD_ASSIGN: ARITH(LLVMFAdd, LLVMAdd,  LLVMAdd,  "fadd", "add"); break;
            case SUB_ASSIGN: ARITH(LLVMFSub, LLVMSub,  LLVMSub,  "fsub", "sub"); break;
            case MUL_ASSIGN: ARITH(LLVMFMul, LLVMMul,  LLVMMul,  "fmul", "mul"); break;
            case DIV_ASSIGN: ARITH(LLVMFDiv, LLVMSDiv, LLVMUDiv, "fdiv", "div"); break;
            case MOD_ASSIGN: ARITH(LLVMFRem, LLVMSRem, LLVMURem, "frem", "mod"); break;

            case BAND_ASSIGN:   res = llvm_binop(LLVMAnd, current, right, "band"); break;
            case BOR_ASSIGN:    res = llvm_binop(LLVMOr,  current, right, "bor");  break;
            case BXOR_ASSIGN:   res = llvm_binop(LLVMXor, current, right, "bxor"); break;
            case LSHIFT_ASSIGN: res = llvm_binop(LLVMShl, current, right, "shl");  break;
            case RSHIFT_ASSIGN: res = llvm_binop(un ? LLVMLShr : LLVMAShr, current, right, "shr"); break;
            default: break;
         }
#undef ARITH
         llvm_store(res, dest);
         node->token->llvm.elem = res;
         break;
      }
      case FALLBACK: {
         code_gen(node->left);
         code_gen(node->right);
         Value left   = node->left->token->llvm.elem;
         Value right  = node->right->token->llvm.elem;
         Value ptr    = opt_ptr(node->left->token, left);
         Value null   = LLVMConstNull(LLVMTypeOf(ptr));
         Value isnull = llvm_icmp(LLVMIntEQ, ptr, null, "isnull");
         node->token->llvm.elem = LLVMBuildSelect(ura.builder, isnull, right, left, "fallback");
         break;
      }
      case ADD: case SUB: case MUL: case DIV: case MOD:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT:
      case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: {
         Token *token = node->token;
         if (token->Fcall.ptr) {
            code_gen_operator(node);
            return;
         }
         int p = token->type == EQUAL ? LLVMIntEQ : LLVMIntNE;
         if (token->kind == CMP_REF) {
            Value l = emit_ref(node->left);
            Value r = emit_ref(node->right);
            token->llvm.elem = llvm_icmp(p, l, r, "refcmp");
            return;
         }
         code_gen(node->left);
         code_gen(node->right);
         Value left  = node->left->token->llvm.elem;
         Value right = node->right->token->llvm.elem;
         Value res   = NULL;
         if (token->kind == CMP_SLICE) {
            Value l = opt_ptr(node->left->token, left);
            Value r = opt_ptr(node->right->token, right);
            token->llvm.elem = llvm_icmp(p, l, r, "nullcmp");
            return;
         }
         Type  lt    = node->left->token->ret_type;
         bool  fp    = is_float(lt);
         bool  un    = is_unsigned(lt);
         if (includes(token->type, DIV, MOD, 0)) guard_nonzero(token, right);
#define ARITH(fop, sop, uop, fname, iname) res = fp \
   ? llvm_binop(fop, left, right, fname) \
   : llvm_binop(un ? uop : sop, left, right, iname)
#define CMP(fpred, spred, upred, fname, iname) res = fp \
   ? llvm_fcmp(fpred, left, right, fname) \
   : llvm_icmp(un ? upred : spred, left, right, iname)
         switch (token->type) {
            case ADD: ARITH(LLVMFAdd, LLVMAdd,  LLVMAdd,  "fadd", "add"); break;
            case SUB: ARITH(LLVMFSub, LLVMSub,  LLVMSub,  "fsub", "sub"); break;
            case MUL: ARITH(LLVMFMul, LLVMMul,  LLVMMul,  "fmul", "mul"); break;
            case DIV: ARITH(LLVMFDiv, LLVMSDiv, LLVMUDiv, "fdiv", "div"); break;
            case MOD: ARITH(LLVMFRem, LLVMSRem, LLVMURem, "frem", "mod"); break;

            case EQUAL:       CMP(LLVMRealOEQ, LLVMIntEQ,  LLVMIntEQ,  "feq", "eq"); break;
            case NOT_EQUAL:   CMP(LLVMRealUNE, LLVMIntNE,  LLVMIntNE,  "fne", "ne"); break;
            case LESS:        CMP(LLVMRealOLT, LLVMIntSLT, LLVMIntULT, "flt", "lt"); break;
            case GREAT:       CMP(LLVMRealOGT, LLVMIntSGT, LLVMIntUGT, "fgt", "gt"); break;
            case LESS_EQUAL:  CMP(LLVMRealOLE, LLVMIntSLE, LLVMIntULE, "fle", "le"); break;
            case GREAT_EQUAL: CMP(LLVMRealOGE, LLVMIntSGE, LLVMIntUGE, "fge", "ge"); break;

            case AND:    res = llvm_binop(LLVMAnd,  left, right, "and");  break;
            case OR:     res = llvm_binop(LLVMOr,   left, right, "or");   break;
            case BAND:   res = llvm_binop(LLVMAnd,  left, right, "band"); break;
            case BOR:    res = llvm_binop(LLVMOr,   left, right, "bor");  break;
            case BXOR:   res = llvm_binop(LLVMXor,  left, right, "bxor"); break;
            case LSHIFT: res = llvm_binop(LLVMShl,  left, right, "shl");  break;
            case RSHIFT: res = llvm_binop(un ? LLVMLShr : LLVMAShr, left, right, "shr"); break;
            default: break;
         }
#undef ARITH
#undef CMP
         token->llvm.elem = res;
         break;
      }
      default:
         CHECK(1, ASSERT_CODEGEN_NODE, to_string(token->type));
         break;
   }
}

UraGlobal ura;

void parse_arguments(int argc, char **argv)
{
   ura.lib = getenv("URA_LIB");
   if (!is_dir(ura.lib)) ura.lib = find_ura_lib();
   ura.output = "exe.out";
   
   for (int i = 1; i < argc && !ura.error_count; i++) {
      char *arg = argv[i];
#define MATCH(name, field, val) if (strcmp(arg, name) == 0) { field = val; continue; }
      MATCH("-debug", ura.enable_debug, true);
      MATCH("-exec",  ura.enable_exec,  true);
      MATCH("-san",   ura.enable_san,   true);
      MATCH("-testing", ura.no_color,   true);
      MATCH("-tree",  ura.enable_tree,  true);
      MATCH("-ll",    ura.enable_ll,    true);
      MATCH("-O0", ura.flags, PASSES_O0);   
      MATCH("-O1", ura.flags, PASSES_O1);
      MATCH("-O2", ura.flags, PASSES_O2);   
      MATCH("-O3", ura.flags, PASSES_O3);
      MATCH("-Os", ura.flags, PASSES_Os);   
      MATCH("-Oz", ura.flags, PASSES_Oz);
#undef MATCH
      if (strcmp(arg, "-o") == 0) {
         if (i + 1 >= argc) {
            parse_error(NULL, ERR_MISSING_O_ARG);
            return;
         }
         ura.output = argv[++i];
      } else if (arg[0] == '-') {
         parse_error(NULL, "Unknown flag '%s'", arg);
      } else {
         size_t n = strlen(arg);
         bool is_ura = n > 4 && strcmp(arg + n - 4, ".ura") == 0;
         if (!is_ura) parse_error(NULL, "Invalid file '%s'", arg);
         else new_source(arg);
      }
   }
   if (ura.error_count) return;
   if (!ura.sources) parse_error(NULL, ERR_NO_INPUT);
}

void load_common() {
   if (!ura.lib) {
      parse_error(NULL, ERR_NO_STDLIB);
      return;
   }
   char *path   = format("%s/common.ura", ura.lib);
   int   before = ura.sources_count;
   new_source(path);
   free(path);
   if (ura.error_count || ura.sources_count == before) return;
   ura.calling_use++;
   tokenize(0);
   ura.calling_use--;
}

void tokenize(int default_indent) {
   if (ura.error_count || !ura.sources) return;
   Source *prev = ura.current;
   ura.current  = ura.sources[ura.sources_count - 1];
   ura.current->loading = true;
   if (!ura.calling_use) load_common();
   char *content = ura.current->content;
   int line = 1;
   int indent = default_indent;
   
   int s = 0;
   int i = 0;
   while (content && content[i] && !ura.error_count) {
      s = i;
      char c = content[i];
      if (lex_spaces(content, &i, &line, &indent, default_indent)) continue;
      if (lex_multi_comment(content, &i, &line)) continue;
      if (lex_comment(content, &i)) continue;
      if (lex_chars(content, &i, line, indent)) continue;
      if (lex_char(content, &i, line, indent)) continue;
      if (lex_number(content, &i, line, indent)) continue;
      if (lex_identifier(content, &i, line, indent, default_indent)) continue;
      if (lex_symbol(content, &i, line, &indent)) continue;
      tokenize_error(line, i, i + 1, "Unexpected character '%c'", c);
   }
   if (!ura.calling_use)
   {
      parse_token(line, s, i, END, -1);
      for(int i = 0; i < ura.tokens_count && ura.enable_debug; i++)
         debug("token %k\n", ura.tokens[i]);
   }
   ura.current->loading = false;
   ura.current = prev;
}

void generate_ast() {
   if(ura.error_count) return;
   Node *head = new_node(new_token(ID, -TAB));
   ura.head = head;
   enter_scope(head);
   while (!find(END, 0)) {
      int before = ura.error_count;
      Node *child = expr_node(0);
      if (ura.error_count > before) {
         parser_recover(0);
         if (ura.error_count > ura.max_errors) break;
         continue;
      }
      resize_array(head->children, Node *);
      head->children[head->children_count++] = child;
      // TODO: only function declarations and
      // struct declaration are allowed here
   }
   if (ura.error_count > 0 || !ura.enable_debug) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < head->children_count; i++)
		pnode(head->children[i], "");
}

void declare_module_members(Node *m) {
   for (int i = 0; i < m->children_count; i++)
      if (m->children[i]->token->type == STRUCT_DEF)
         declare_struct(m->children[i]);
   for (int i = 0; i < m->children_count; i++)
      if (m->children[i]->token->type == ENUM_DEF)
         declare_enum(m->children[i]);
   for (int i = 0; i < m->children_count; i++)
      if (m->children[i]->token->type == FDEC)
         declare_function(m->children[i]);
   for (int i = 0; i < m->children_count; i++)
      if (m->children[i]->token->type == MODULE)
         declare_module_members(m->children[i]);
}

void generate_ir() {
   if(ura.error_count || !ura.head) return;
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == STRUCT_DEF)
         declare_struct(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == ENUM_DEF)
         declare_enum(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == FDEC)
         declare_function(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++) {
      Token *global = global_decl(ura.head->children[i]);
      if (!global) continue;
      global->is_global = true;
      declare_variable(global);
   }
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == MODULE)
         declare_module_members(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      analyze(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      type_check(ura.head->children[i]);
   if (!ura.enable_tree) return;
   print_ast(ura.head);
}

void generate_asm() {
   if (ura.error_count || !ura.head) return;
   setup_paths(ura.sources[0]->filename);
   init_module(ura.output);
   for (int i = 0; i < ura.head->children_count; i++)
      if (program_throws(ura.head->children[i])) {
         ura.uses_exceptions = true;
         break;
      }
   for (int i = 0; i < ura.head->children_count; i++) {
      Token *global = global_decl(ura.head->children[i]);
      if (global) llvm_global(global);
   }
   for (int i = 0; i < ura.head->children_count; i++)
      if (!global_decl(ura.head->children[i]))
         code_gen(ura.head->children[i]);
   finalize_module(ura.ll_path);
}

void run_linker(char **argv) {
   pid_t pid = fork();
   if (pid < 0) return;
   if (pid == 0) {
      int devnull = open("/dev/null", O_WRONLY);
      if (devnull >= 0) {
         dup2(devnull, STDERR_FILENO);
         close(devnull);
      }
      execvp(argv[0], argv);
      _exit(127);
   }
   int status = 0;
   waitpid(pid, &status, 0);
}

void compile_executable() {
   if (ura.error_count || !ura.head) return;

   fprintf(stderr, CYAN("%-9s") " " BLUE("%s (%s)") "\n", 
         "Compiling", ura.base, ura.sources[0]->filename);
   char *cc = ura.enable_san ? "/usr/bin/clang" : "clang";
   char *argv[12];
   int   n = 0;
   argv[n++] = cc;
   if (ura.enable_san) {
      argv[n++] = "-fsanitize=address,undefined";
      argv[n++] = "-fno-omit-frame-pointer";
      argv[n++] = "-g";
   }
   argv[n++] = ura.ll_path;
   argv[n++] = "-o";
   argv[n++] = ura.output;
   argv[n]   = NULL;
   run_linker(argv);
   if (!ura.enable_exec) return;
   char *opt = ura.flags ? "optimized" : "unoptimized";
   char *tag = ura.enable_san ? " + sanitized" : "";
   fprintf(stderr, CYAN("%-9s") " \033[2m[%s%s]\033[0m in %.2fs\n",
           "Finished", opt, tag, clock_now() - ura.time_start);
}

void run_executable() {
   if (!ura.enable_exec || ura.error_count || !ura.head) return;
   char  *run = strchr(ura.output, '/') ? ura.output : format("./%s", ura.output);
   fprintf(stderr, CYAN("%-9s") " " BLUE("%s") "\n", "Running", run);
   double start = clock_now();
   pid_t  pid = fork();
   if (pid == 0) {
      execl(run, run, NULL);
      _exit(127);
   }
   if (pid < 0) return;
   int status = 0;
   waitpid(pid, &status, 0);
   double exec = clock_now() - start;
   if (WIFSIGNALED(status))
      fprintf(stderr, RED("%-9s") " %s (signal %d) in %.2fs\n",
              "\nCrashed", signal_name(WTERMSIG(status)), WTERMSIG(status), exec);
   else
      fprintf(stderr, CYAN("%-9s") " with code %d in %.2fs\n",
              "\nExited", WEXITSTATUS(status), exec);
}

void detect_platform() {
   static char *macos_aliases[]   = { "macos", "darwin", "unix", 0 };
   static char *linux_aliases[]   = { "linux", "unix", 0 };
   static char *windows_aliases[] = { "windows", 0 };
   static char *no_platform[]     = { 0 };
   char *triple = LLVMGetDefaultTargetTriple();
   if (strstr(triple, "darwin") || strstr(triple, "apple"))
      ura.platform = macos_aliases;
   else if (strstr(triple, "linux"))
      ura.platform = linux_aliases;
   else if (strstr(triple, "windows") || strstr(triple, "win32"))
      ura.platform = windows_aliases;
   else
      ura.platform = no_platform;
   LLVMDisposeMessage(triple);
}

int main(int argc, char**argv) {
   ura.time_start = clock_now();
   ura.max_errors = 20;
   ura.enable_debug = false;
   parse_arguments(argc, argv);
   detect_platform();
#if TOKENIZE
   tokenize(0);
   preprocess();
#endif
#if AST
   generate_ast();
#endif
#if IR
   generate_ir();
#endif
#if ASM
   generate_asm();
   compile_executable();
   run_executable();
#endif
   free_memory();
   return ura.error_count != 0;
}
