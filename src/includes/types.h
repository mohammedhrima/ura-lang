#pragma once

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

#define PointerTypeKind LLVMPointerTypeKind
#define IntegerTypeKind LLVMIntegerTypeKind
#define VoidTypeKind    LLVMVoidTypeKind
#define FunctionTypeKind LLVMFunctionTypeKind
#define StructTypeKind  LLVMStructTypeKind

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
   RETURN, IF, ELIF, ELSE, END_IF,
   WHILE, CONTINUE, BREAK, END_BLOC,
   // Functions
   FDEC, FCALL, PROTO, ARGS, CHILDREN,
   // Built-ins
   STACK, HEAP, TYPEOF, SIZEOF, OUTPUT, DEFAULT, SYNTAX_ERROR,
   // Bitwise
   BAND, BOR, BXOR, BNOT, LSHIFT, RSHIFT,
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

struct ExcepCTX
{
   Block lpad;
   Block Catch;
   Block end;
   Value storage;
   Type type;
   bool in_catch;
};

struct Token
{
   Type type;
   Type retType;
   Type assign_type;

   char *name;
   int space;
   bool remove;

   int used;
   int pos;

   bool is_cond;
   bool is_ref;
   bool is_dec;
   bool is_global;
   bool is_param;
   bool is_cast;
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
      struct { Type elem_type; int depth; } Array;
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
   int cpos;
   int clen;

   Token **variables;
   int vpos;
   int vlen;

   Node **functions;
   int fpos;
   int flen;

   Node **structs;
   int spos;
   int slen;

   Node **methods;
   int mlen;
   int mpos;
};
