#pragma once

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

#include "errors.h"

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

#define DATA_TYPES     I32, BOOL, CHARS, CHAR, F32, F64, VOID, I64, PTR, I16, ARRAY_TYPE, LIST_TYPE, FN_TYPE
#define LOGIC_TYPE     AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define BITWISE_TYPE   BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  I32, I64, I16, BOOL, CHAR
#define FLOAT_TYPES    F32, F64
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
	VOID, I32, F32, F64, I64, I16, BOOL, CHAR, CHARS, PTR, VARIADIC, REF,
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
	FOR, BY, IN, LOOP,
	FDEC, FCALL, PROTO, ARGS, CHILDREN, // Functions
	TYPEOF, SIZEOF, OUTPUT, SYNTAX_ERROR, // Built-ins
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
	char *pathname;
	bool  loading;
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
	bool    has_drop;
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
	EXPAND(Token **, tokens);
	EXPAND(Token **, temps);
	EXPAND(Source **, sources);
	Token           *fn_ret;
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

#include "frontend/header.h"
#include "backend/header.h"
#include "utils/header.h"