#pragma once

// HEADERS
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <libgen.h>
#include <limits.h>
#include <sys/types.h>
#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>

// MACROS
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1
#define TAB 3
#define AST 1
#define IR 1
#define OPTIMIZE 0
#define ASM 1

#ifndef DEBUG
#define DEBUG 1
#endif

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define to_string(type) to_string_(FILE, LINE, type)
#define todo(cond, fmt, ...) if (check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define stop(cond, fmt, ...) if (check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)) exit(1);
#define seg() raise(SIGSEGV);

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, LONG, PTR, SHORT

#define AST_NODE(name, child_func, ...) \
Node *name() { \
   Node *left = child_func(); \
   Token *token; \
   while ((token = find(__VA_ARGS__, 0))) { \
      Node *node = new_node(token); \
      node->left = left; \
      node->right = child_func(); \
      left = node; \
   } \
   return left; \
}

// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct Inst Inst;
typedef struct LLvm LLvm;
typedef enum Type Type;

// STRUCTS
enum Type
{
   // General / Special
   TMP = 1, CHILDREN, DEFAULT, COMMENT, END,
   // Identifiers & References
   ID, REF,
   REF_ID, REF_HOLD_ID, REF_VAL, REF_HOLD_REF, REF_REF,
   ID_ID, ID_REF, ID_VAL,

   // Types
   VOID, INT, FLOAT, LONG, SHORT, BOOL, CHAR, CHARS, PTR,
   ARRAY, CAST,

   // Struct Usage
   STRUCT_DEF, STRUCT_BODY, STRUCT_ATTR,
   STRUCT_ALLOC, STRUCT_CALL,

   // Operators
   // Assignment
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
   // Comparison
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   // Arithmetic
   ADD, SUB, MUL, DIV, MOD,
   // Logical
   AND, OR, NOT,

   // Punctuation & Syntax
   LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS, ACCESS,

   // Control Flow
   RETURN, ARROW,
   IF, ELIF, ELSE, END_IF, BUILD_COND,
   WHILE, CONTINUE, BREAK, END_WHILE,
   BLOC, END_BLOC, END_COND, APPEND_BLOC, SET_POS,
   BUILD_BR,

   // Functions
   FDEC, FCALL, PROTO_FUNC,

   // Jumps
   JMP, JE, JNE,

   // Functions params
   PUSH, POP
};


struct LLvm
{
   bool is_set;  // is LLVM block set
   LLVMTypeRef funcType;
   LLVMValueRef element;
   LLVMValueRef ptr;  // ADD THIS LINE - pointer for lvalue assignments
   LLVMBasicBlockRef bloc;
   LLVMTypeRef structType;
};

struct Token
{
   Type type;
   Type retType; // return type
   Type assign_type;

   char *name;
   int space; // indentation
   bool remove;
   int index;
   int size;

   // Intermediate Representation
   int ir_reg;
   int used;

   bool is_cond;
   bool is_ref;
   bool has_ref;
   bool is_declare;
   bool is_attr;
   bool is_proto;
   bool is_arg;
   bool is_param;
   bool is_cast;

   // display errors, file and line
   char *filename;
   int line;

   LLvm llvm;
   struct
   {
      // integer
      struct { long value; } Int;
      // short
      struct { int value; } Short;
      // long
      struct { long long value; } Long;
      // float
      struct { float value; } Float;
      // boolean
      struct { bool value; } Bool;
      // chars
      struct { char *value; } Chars;
      // char
      struct { char value; } Char;
      // structure
      struct
      {
         Token *ptr;
         char *name;
         Token **attrs;
         int size;
         int pos;
         int attr_index;
      } Struct;
      struct
      {
         Token *func_ptr;
         int index;
      } Param;
      // function call
      struct
      {
         Token *ptr;
         Token **args;
         int size;
         int pos;
      } Fcall;
      // function declaration
      struct
      {
         Token **args;
         int size;
         int pos;
      } Fdec;
      // statement
      struct
      {
         Token *ptr;
         Token *start;
         Token *end;
      } Statement;
   };
};

struct Node
{
   Node *left;
   Node *right;
   Token *token;

   struct Node **children;
   int cpos; // children pos
   int csize; // children size

   // bloc Infos
   struct
   {
      struct Node **functions;
      int fpos;
      int fsize;

      Token **structs;
      int spos;
      int ssize;

      Token **vars;
      int vpos;
      int vsize;
   };
};

struct Inst
{
   Token *token;
   Token *left;
   Token *right;
};

// GLOBAL
extern bool found_error;

extern Token **tokens;
extern int tk_pos;
extern int tk_len;

extern char *input;
extern Node *global;
extern int exe_pos;
extern Inst **OrgInsts;
extern Inst **insts;

extern Node **Gscoop;
extern Node *scoop;
extern int scoopSize;
extern int scoopPos;

extern int ptr;
#if defined(__APPLE__)
extern struct __sFILE *asm_fd;
#elif defined(__linux__)
extern struct _IO_FILE *asm_fd;
#endif

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------

Token* new_token(Type type, int space);
void parse_token(char *filename, int line, char *input, int s, int e, Type type, int space)
void add_token(Token *token);
Node *expr();
Node *assign();
Node *logic();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *dot();
Node *sign();
Node *brackets();
Node *prime();
Node *new_node(Token *token);
bool includes(Type to_find, ...);
Token *find(Type type, ...);
Node *new_function(Node *node);
Node *get_function(char *name);
Token *get_variable(char *name);
Token *new_variable(Token *token);
void free_node(Node *node);
Token *copy_token(Token *token);
Node *copy_node(Node *node);
Token *new_struct(Token *token);
Token *get_struct(char *name);
Token *is_struct(Token *token);
void add_attribute(Token *obj, Token *attr);
Node* add_child(Node *node, Node *child);
void add_variable(Node *bloc, Token *token);

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
Inst *new_inst(Token *token);
void add_inst(Inst *inst);

void enter_scoop(Node *node);
void exit_scoop();
void copy_insts();
bool compatible(Token *left, Token *right);
Token *generate_ir(Node *node);
void generate_asm(char *name);

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------
char* open_file(char *filename);
char *to_string_(char *filename, int line, Type type);
void setName(Token *token, char *name);
bool within_space(int space);
bool check_error(const char *filename, const char *funcname, int line,
                 bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(int line, int len, int size);
char *strjoin(char *str0, char *str1, char *str2);
Type getRetType(Node *node);
bool optimize_ir();
char* resolve_path(char* path);

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
int print(char *conv, ...);
int pnode(Node *node, char *side, int space);
int ptoken(Token *token);
void print_ast(Node *head);
void print_ir();
int print_value(Token *token);

// LLVM utilities
LLVMModuleRef mod;
LLVMBuilderRef builder;
LLVMContextRef context;

// TODO: this approach need to be customized
LLVMValueRef farr[100];
static int fpos = 0;
LLVMValueRef cfunc;

void enter_func(LLVMValueRef func)
{
   farr[fpos] = func;
   cfunc = farr[fpos];
   fpos++;
}

void exit_func()
{
   fpos--;
   cfunc = farr[fpos];
}


LLVMTypeRef get_llvm_type(Token *token)
{
   static LLVMTypeRef vd = LLVMVoidTypeInContext(context);
   static LLVMTypeRef f32 = LLVMFloatTypeInContext(context);
   static LLVMTypeRef i1 = LLVMInt1TypeInContext(context); // bool (i1)
   static LLVMTypeRef i8 = LLVMInt8TypeInContext(context); // char (i8)
   static LLVMTypeRef i16 = LLVMInt16TypeInContext(context);
   static LLVMTypeRef i32 = LLVMInt32TypeInContext(context);
   static LLVMTypeRef i64 = LLVMInt64TypeInContext(context);
   static LLVMTypeRef p8 = LLVMPointerType(i8, 0); // char* (pointer to i8)
   static LLVMTypeRef p32 = LLVMPointerType(i32, 0); // i32*

   Type type = token->retType ? token->retType : token->type;
   switch (type)
   {
   case VOID: return vd;
   case INT: return i32;
   case FLOAT: return f32;
   case LONG: return i64;
   case SHORT: return i16;
   case BOOL: return i1;
   case CHAR: return i8;
   case CHARS: return p8;
   case PTR: return p32;
   default: todo(1, "handle this case %s", to_string(type)); break;
   }
   return NULL;
}

LLVMValueRef get_value(Token *token)
{
   LLVMTypeRef llvmType = get_llvm_type(token);
   switch (token->type)
   {
   case INT: return LLVMConstInt(llvmType, token->Int.value, 0);
   case FLOAT: return LLVMConstReal(llvmType, token->Float.value);
   case BOOL: return LLVMConstInt(llvmType, token->Bool.value ? 1 : 0, 0);
   case LONG: return LLVMConstInt(llvmType, token->Long.value, 0);
   case SHORT: return LLVMConstInt(llvmType, token->Short.value, 0);
   case CHAR: return LLVMConstInt(llvmType, token->Char.value, 0);
   case CHARS:
   {
      LLVMValueRef str_constant = LLVMConstStringInContext(context, token->Chars.value,
                                  strlen(token->Chars.value), 0);
      LLVMValueRef global_str = LLVMAddGlobal(mod, LLVMTypeOf(str_constant), "str_literal");
      LLVMSetInitializer(global_str, str_constant);
      LLVMSetLinkage(global_str, LLVMPrivateLinkage);
      LLVMSetGlobalConstant(global_str, 1);
      return global_str;
   }
   default: todo(1, "handle this literal case %s", to_string(token->type));
   }
   return (LLVMValueRef) {};
}

LLVMValueRef get_llvm_op(Token *token, Token* left, Token* right)
{
   LLVMValueRef leftRef = NULL, rightRef = NULL;
   if (left->name && !left->is_param && !includes(left->type, FCALL, AND, OR))
      leftRef = LLVMBuildLoad2(builder, get_llvm_type(left), left->llvm.element, left->name);
   else leftRef = left->llvm.element;

   if (right->name && !right->is_param && !includes(right->type, FCALL, AND, OR))
      rightRef = LLVMBuildLoad2(builder, get_llvm_type(right), right->llvm.element, right->name);
   else rightRef = right->llvm.element;

   char* op = to_string(token->type);
   Type type = token->type == FCALL ? token->retType : token->type;
   switch (type)
   {
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, op); break;
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, op); break;
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, op); break;
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, op); break;
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, op); break;
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, op); break;
   case ADD: return LLVMBuildAdd(builder, leftRef, rightRef, op); break;
   case SUB: return LLVMBuildSub(builder, leftRef, rightRef, op); break;
   case MUL: return LLVMBuildMul(builder, leftRef, rightRef, op); break;
   case DIV: return LLVMBuildSDiv(builder, leftRef, rightRef, op); break;
   case MOD: return LLVMBuildSRem(builder, leftRef, rightRef, op); break;
   case AND: return LLVMBuildAnd(builder, leftRef, rightRef, op); break;
   case OR: return LLVMBuildOr(builder, leftRef, rightRef, op); break;
   default: todo(1, "handle this %s", op);
   }
   return (LLVMValueRef) {};
}

void handle_asm(Inst *inst)
{
   if (DEBUG) print("%k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   LLVMValueRef ret = NULL;

   if (curr->llvm.is_set)
   {
      print(RED"%k already set\n"RESET, curr);
      exit(1);
      return;
   }

   switch (curr->type)
   {
   case VOID:
   {
      curr->llvm.is_set = true;
      break;
   }
   case INT: case BOOL: case LONG: case SHORT: case CHAR: case CHARS: case PTR:
   {
      if (curr->name) print(CYAN"call %s\n"RESET, curr->name);
      if (curr->is_param)
      {
         ret = LLVMGetParam(curr->Param.func_ptr->llvm.element, curr->Param.index);
         LLVMSetValueName(ret, curr->name);
      }
      else if (curr->is_declare)
      {
         ret = LLVMBuildAlloca(builder, get_llvm_type(curr), curr->name);
      }
      else if (curr->name)
      {
         ret = curr->llvm.element;
      }
      else ret = get_value(curr);

      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case STRUCT_DEF:
   {
      curr->llvm.is_set = true;
      // CREATE STRUCT TYPE
      // SET STRUCT BODY
      curr->llvm.structType = LLVMStructCreateNamed(LLVMGetGlobalContext(), curr->Struct.name);
      int pos = curr->Struct.pos;
      LLVMTypeRef *attrs = allocate(pos, sizeof(LLVMTypeRef));
      for (int i = 0; i < pos; i++)
      {
         Token *attr = curr->Struct.attrs[i];
         stop(!attr, "attribite is NULL\n");
         attrs[i] = get_llvm_type(attr);
      }
      // SET STRUCT BODY
      LLVMStructSetBody(curr->llvm.structType, attrs, pos, 0);
      free(attrs);
      break;
   }
   case STRUCT_CALL:
   {
      curr->llvm.element = LLVMBuildAlloca(builder, curr->Struct.ptr->llvm.structType, curr->name);
      curr->llvm.is_set = true;
      break;
   }
   case DOT:
   {
      LLVMValueRef st_call = left->llvm.element;
      LLVMTypeRef st_type = left->Struct.ptr->llvm.structType;
      int index = right->Struct.attr_index; // attribute position
      curr->llvm.element = LLVMBuildStructGEP2(builder, st_type, st_call, index, right->name);
      curr->llvm.is_set = true;
      curr->type = right->type;
      curr->name = strdup(right->name);
      break;
   }
   case ASSIGN:
   {
      check(!left->llvm.is_set, "assign, left is not set");
      check(!right->llvm.is_set, "assign, right is not set");

      LLVMValueRef target = left->llvm.ptr ? left->llvm.ptr : left->llvm.element;
      LLVMBuildStore(builder, right->llvm.element, target);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   case AND: case OR:
   {
      if (check(!left->llvm.is_set, "left is not set\n")) break;
      if (check(!right->llvm.is_set, "right is not set\n")) break;
      ret = get_llvm_op(curr, left, right);
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case FCALL:
   {
      LLvm srcFunc = curr->Fcall.ptr->llvm;
      LLVMValueRef *args = NULL;
      if (curr->Fcall.pos)
      {
         args = allocate(curr->Fcall.pos, sizeof(LLVMValueRef));
         for (int i = 0; i < curr->Fcall.pos; i++)
         {
            Token *arg = curr->Fcall.args[i];
            check(!arg->llvm.is_set, "llvm is not set");

            if (arg->name && !arg->is_param && arg->type != FCALL)
               args[i] = LLVMBuildLoad2(builder, get_llvm_type(arg), arg->llvm.element, arg->name);
            else
               args[i] = arg->llvm.element;
         }
         curr->llvm.element = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.element, args,
                                             curr->Fcall.pos, curr->name);
         free(args);
      }
      else
         curr->llvm.element = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.element,
                                             NULL, 0, curr->name);

      curr->llvm.is_set = true;
      break;
   }
   case FDEC:
   {
      // print("FDEC: ", curr->name);
      LLVMTypeRef *args = NULL;

      if (curr->Fdec.pos)
      {
         args = allocate(curr->Fdec.pos + 1, sizeof(LLVMTypeRef));
         for (int i = 0; i < curr->Fdec.pos; i++)
            args[i] = get_llvm_type(curr->Fdec.args[i]);
         curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), args, curr->Fdec.pos, 0);
         free(args);
      }
      else curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), NULL, 0, 0);

      curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);

      if (!curr->is_proto)
      {
         LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element, "entry");
         LLVMPositionBuilderAtEnd(builder, funcEntry);
         LLVMPositionBuilderAtEnd(builder, funcEntry);
      }

      enter_func(curr->llvm.element);
      curr->llvm.is_set = true;
      break;
   }
   case END_BLOC:
   {
      exit_func();
      break;
   }
   case RETURN:
   {
      if (check(!left->llvm.is_set, "return result is not set\n")) break;
      switch (left->type)
      {
      case FCALL: case ADD: case SUB: case MUL: case DIV: case MOD:
      case LESS: case LESS_EQUAL: case MORE:
      case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
      case AND: case OR:
      {
         ret = LLVMBuildRet(builder, left->llvm.element);
         break;
      }
      case INT: case BOOL: case LONG: case SHORT: case CHAR: case FLOAT:
      {
         if (left->name || left->is_attr)
         {
            LLVMValueRef loaded = LLVMBuildLoad2(builder, get_llvm_type(left),
                                                 left->llvm.element, left->name);
            ret = LLVMBuildRet(builder, loaded);
         }
         else
            ret = LLVMBuildRet(builder, get_value(left));
         break;
      }
      case VOID:
      {
         ret = LLVMBuildRetVoid(builder);
         break;
      }
      default:
         todo(1, "handle this case %s\n", to_string(left->type));
         break;
      }
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case APPEND_BLOC:
   {
      check(!left->name, "APPEND BLOC require a name");
      left->llvm.bloc = LLVMAppendBasicBlockInContext(context, cfunc, left->name);
      curr->llvm.is_set = true;
      break;
   }
   case BUILD_COND:
   {
      LLVMValueRef cond = curr->Statement.ptr->llvm.element;
      LLVMBasicBlockRef start = left->llvm.bloc;
      LLVMBasicBlockRef end = right->llvm.bloc;
      curr->llvm.element = LLVMBuildCondBr(builder, cond, start, end);
      curr->llvm.is_set = true;
      break;
   }
   case ACCESS:
   {
      check(!left->llvm.is_set, "left is not set");
      check(!right->llvm.is_set, "right is not set");

      LLVMValueRef leftRef = NULL, rightRef = NULL;
      if (left->name && !left->is_param && left->type != FCALL)
         leftRef = LLVMBuildLoad2(builder, get_llvm_type(left), left->llvm.element, left->name);
      else leftRef = left->llvm.element;

      if (right->name && !right->is_param && right->type != FCALL)
         rightRef = LLVMBuildLoad2(builder, get_llvm_type(right), right->llvm.element, right->name);
      else rightRef = right->llvm.element;

      LLVMValueRef indices[] = { rightRef };
      LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, i8, leftRef, indices, 1, "access");

      curr->llvm.ptr = elem_ptr;

      curr->llvm.element = LLVMBuildLoad2(builder, i8, elem_ptr, "access_val");
      curr->llvm.is_set = true;
      curr->type = CHAR;
      curr->name = NULL;
      break;
   }
   case SET_POS:
   {
      LLVMPositionBuilderAtEnd(builder, left->llvm.bloc);
      break;
   }
   case BUILD_BR:
   {
      check(!left->name, "BUILD BR require a name");
      LLVMBuildBr(builder, left->llvm.bloc);
      break;
   }
   default: todo(1, "handle this case (%s)\n", to_string(curr->type)); break;
   }
}

void generate_asm(char *name)
{
#if ASM
   if (found_error) return;

   char *moduleName = resolve_path(name);
   context = LLVMContextCreate();
   mod = LLVMModuleCreateWithName(moduleName);
   builder = LLVMCreateBuilder();

   for (int i = 0; insts[i]; i++) handle_asm(insts[i]);

   // save to file
   int len = strlen(moduleName);
   strcpy(moduleName + len - 3, "ll");
   moduleName[len - 1] = '\0';
   LLVMPrintModuleToFile(mod, moduleName, NULL);

   // free llvm context
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   LLVMContextDispose(context);
   free(moduleName);
#endif
}

/*
SECTIONS:
   + LOGGING:
   + MEMMORY
   + PARSING
   + IR
   + UTILS
*/

// LOGGING
int print(char *conv, ...)
{
   int res = 0;
   va_list args;
   va_start(args, conv);

   for (int i = 0; conv[i]; i++)
   {
      if (conv[i] == '%')
      {
         i++;
         int left_align = 0;
         if (conv[i] == '-') { left_align = 1; i++; }
         int width = 0;
         while (isdigit(conv[i])) { width = width * 10 + (conv[i] - '0'); i++; }
         int prec = -1;
         if (conv[i] == '.')
         {
            i++;
            prec = 0;
            while (conv[i] >= '0' && conv[i] <= '9')
            {
               prec = prec * 10 + (conv[i] - '0');
               i++;
            }
         }
         if (strncmp(conv + i, "zu", 2) == 0) {res += fprintf(stdout, "%d", va_arg(args, int)); i++;}
         else if (strncmp(conv + i, "lld", 3) == 0)
         {res += fprintf(stdout, "%lld", va_arg(args, long long)); i += 2;}
         else
         {
            switch (conv[i])
            {
            case 'c': res += fprintf(stdout, "%c", va_arg(args, int)); break;
            case 's':
            {
               char *str = va_arg(args, char *);
               if (!str) str = "(null_str)";
               if (left_align)
               {
                  if (prec >= 0) res += fprintf(stdout, "%-*.*s", width, prec, str);
                  else res += fprintf(stdout, "%-*s", width, str);
               }
               else
               {
                  if (prec >= 0) res += fprintf(stdout, "%*.*s", width, prec, str);
                  else res += fprintf(stdout, "%*s", width, str);
               }
               break;
            }
            case 'p':
               res += fprintf(stdout, "%p", (void *)(va_arg(args, void *))); break;
            case 'x':
               if (prec >= 0)
                  res += fprintf(stdout, "%.*x", prec, va_arg(args, unsigned int));
               else
                  res += fprintf(stdout, "%x", va_arg(args, unsigned int));
               break;
            case 'X':
               if (prec >= 0) res += fprintf(stdout, "%.*X", prec, va_arg(args, unsigned int));
               else res += fprintf(stdout, "%X", va_arg(args, unsigned int));
               break;
            case 'd':
               if (prec >= 0) res += fprintf(stdout, "%.*d", prec, va_arg(args, int));
               else res += fprintf(stdout, "%d", va_arg(args, int));
               break;
            case 'f':
               if (prec >= 0) res += fprintf(stdout, "%.*f", prec, va_arg(args, double));
               else res += fprintf(stdout, "%f", va_arg(args, double));
               break;
            case '%': res += fprintf(stdout, "%%"); break;
            case 't':
               res += fprintf(stdout, "%s", to_string((Type)va_arg(args, Type))); break;
            case 'k':
            {
               Token *token = va_arg(args, Token *);
               res += token ? ptoken(token) : fprintf(stdout, "(null)");
               break;
            }
            case 'n':
            {
               Node *node = (Node *)va_arg(args, Node *);
               res += print("node: ") + (node ?
                                         pnode(node, NULL, node->token->space) :
                                         fprintf(stdout, "(null)"));
               break;
            }
            default: todo(1, "invalid format specifier [%c]\n", conv[i]);
               break;
            }
         }
      }
      else res += fprintf(stdout, "%c", conv[i]);
   }
   va_end(args);
   return res;
}

int ptoken(Token *token)
{
   int res = 0;
   if (!token) return print("null token");
   res += print("[%-8s] ", to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT:
   case BOOL: case FLOAT: case LONG:
   {
      if (token->name) res += print("name [%s] ", token->name);
      else if (token->type != VOID) print_value(token);
      break;
   }
   case STRUCT_CALL: case STRUCT_DEF:
   {
      if (token->name) res += print("name [%s] ", token->name);
      res += print("st_name [%s] ", token->Struct.name);
      res += print("attributes:\n");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
         for (int j = 0; j < token->space + TAB; ) j += print(" ");
         res += ptoken(attr);
         print("\n");
      }
      break;
   }
   case FCALL: case FDEC: case ID: res += print("name [%s] ", token->name); break;
   default: break;
   }
   if (token->ir_reg) res += print("ir_reg [%d] ", token->ir_reg);
   if (token->is_ref) print("ref ");
   if (token->has_ref) print("has_ref ");
   if (token->remove) res += print("[remove] ");
   if (token->retType) res += print("ret [%t] ", token->retType);
   return res;
}

int pnode(Node *node, char *side, int space)
{
   if (!node) return 0;
   int res = 0;
   for (int i = 0; i < space; i++) res += print(" ");
   if (side) res += print("%s", side);
   res += print("%k\n", node->token);
   res += pnode(node->left, "L: ", space + TAB);
   res += pnode(node->right, "R: ", space + TAB);
   if (node->children)
   {
      for (int i = 0; i < space; i++) res += print(" ");
      res += print("children: \n");
      for (int i = 0; i < node->cpos; i++)
         pnode(node->children[i], NULL, space + TAB);
   }
   return res;
}

int print_value(Token *token)
{
   // TODO: handle the other cases
   switch (token->type)
   {
   case INT: return print("value [%lld] ", token->Int.value);
   case LONG: return print("value [%lld] ", token->Long.value);
   case BOOL: return print("value [%s] ", token->Bool.value ? "True" : "False");
   case FLOAT: return print("value [%f] ", token->Float.value);
   case CHAR: return print("value [%c] ", token->Char.value);
   case CHARS: return print("value [%s] index [%d] ", token->Chars.value, token->index);
   case STRUCT_CALL: return print("has [%d] attrs ", token->Struct.pos);
   case DEFAULT: return print("default value ");
   case ADD: case SUB: case NOT_EQUAL: return print("%t ", token->type); break;
   case VOID: break;
   default: check(1, "handle this case [%s]\n", to_string(token->type));
   }
   return 0;
}

void print_ast(Node *head)
{
   if (!DEBUG) return;
   print(GREEN BOLD SPLIT RESET);
   print(GREEN BOLD"PRINT AST:\n" RESET);
   for (int i = 0; !found_error && i < head->cpos; i++)
      print("%n\n", head->children[i]);
   print(GREEN BOLD SPLIT RESET);
}

void print_inst(Inst *inst)
{
   if (!DEBUG) return;
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   curr->ir_reg ? print("r%.2d:", curr->ir_reg) : print("rxx:");
   int k = 0;
   while (!TESTING && k < curr->space) k += print(" ");
   print("[%-6s] ", to_string(curr->type));

   switch (curr->type)
   {
   case ADD_ASSIGN: case ASSIGN:
   {
      print("[%s] ", curr->assign_type ? to_string(curr->assign_type) : "");

      if (left->name) print("r%.2d (%s) = ", left->ir_reg, left->name);
      else print("r%.2d = ", left->ir_reg);

      if (right->ir_reg) print("r%.2d (%s) ", right->ir_reg, right->name ? right->name : "");
      else print_value(right);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case EQUAL: case NOT_EQUAL: case LESS: case MORE:
   case LESS_EQUAL: case MORE_EQUAL:
   case AND: case OR:
   {
      if (left->ir_reg) print("r%.2d ", left->ir_reg);
      else print_value(left);
      if (left->name) print("(%s)", left->name);

      print(" to ");
      if (right->ir_reg) print("r%.2d ", right->ir_reg);
      else print_value(right);
      if (right->name) print("(%s)", right->name);

      // print(" ret (%t)", to_string(curr->retType));
      break;
   }
   case INT: case BOOL: case CHARS: case CHAR: case LONG: case VOID:
   {
      if (curr->name) print("name %s ", curr->name);
      if (curr->type == CHARS && !curr->name)
         print("value \"%s\" in STR%d ", curr->Chars.value, curr->index);
      else if (!curr->name) print_value(curr);
      break;
   }
   case DOT: print("get attribute [%s] in %k", right->name, left); break;
   case JMP: print("jmp to [%s] ", curr->name); break;
   case JNE: print("jne to [%s] ", curr->name); break;
   case FCALL: print("call [%s] ", curr->name); break;
   case BLOC: case FDEC: print("[%s] bloc ", curr->name); break;
   case END_BLOC: print("[%s] endbloc ", curr->name); break;
   case STRUCT_CALL: print("%s ", curr->name); break;
   case STRUCT_DEF: print("%s ", curr->Struct.name); break;
   case BUILD_COND: print("%s ", curr->name); break;
   case SET_POS: case APPEND_BLOC: case BUILD_BR:
      print("%s ", left->name); break;
   case ACCESS:
   {
      if (left->ir_reg) print("r%.2d ", left->ir_reg);
      if (left->name) print("(%s) ", left->name);
      print("in ");
      if (right->ir_reg) print("r%.2d ", right->ir_reg);
      else print_value(right);
      if (right->name) print("(%s) ", right->name);
   }
   case IF: case ELIF: case END_IF: case ELSE: case END_COND:
   case STRUCT_ALLOC: case STRUCT_BODY:
   case RETURN: case CONTINUE: case BREAK: break;
   default: print(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
   }
   print("\n");
}

void print_ir()
{
   if (!DEBUG) return;
   copy_insts();
   print(GREEN BOLD SPLIT RESET);
   print(GREEN BOLD"PRINT IR:\n" RESET);
   int i;
   for (i = 0; insts && insts[i]; i++) print_inst(insts[i]);
   print("total instructions [%d]\n", i);
   print(GREEN BOLD SPLIT RESET);
}

// MEMMORY
void *allocate_func(int line, int len, int size)
{
   void *res = calloc(len, size);
   check(!res, "allocate did failed in line %d\n", line);
   return res;
}

void free_token(Token *token)
{
   free(token->name);
   // free(token->creg);
   free(token->Chars.value);
   free(token->Struct.attrs);
   free(token->Struct.name);
   free(token);
}

void free_node(Node *node)
{
   if (!node) return;
   for (int i = 0; i < node->cpos; i++) free_node(node->children[i]);
   free_node(node->left);
   free_node(node->right);
   free(node->children);
   free(node->functions);
   free(node->vars);
   free(node->structs);
   free(node);
}

void free_memory()
{
   for (int i = 0; tokens && tokens[i]; i++)
   {
      free_token(tokens[i]);
      tokens[i] = NULL;
   }
   tk_pos = 0;
   exe_pos = 0;
}

// PARSING
Token* new_token(Type type, int space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(new);
   return new;
}

void parse_token(char *filename, int line, char *input, int s, int e,
                 Type type, int space)
{
   Token *new = new_token(type, space);
   new->line = line;
   new->filename = filename;
   switch (type)
   {
   case INT:
   {
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   case ID:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      int i = 0;

      struct { char *name; bool value; } bools[] = {{"True", true}, {"False", false}, {0, 0}};
      for (i = 0; bools[i].name; i++)
      {
         if (strcmp(new->name, bools[i].name) == 0)
         {
            setName(new, NULL);
            new->type = BOOL;
            new->Bool.value = bools[i].value;
            break;
         }
      }
      if (new->name == NULL) break;

      struct { char *name; Type type; } dataTypes [] =
      {
         {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
         {"char", CHAR}, {"float", FLOAT}, {"void", VOID},
         {"long", LONG}, {"pointer", PTR}, {"array", ARRAY},
         {"short", SHORT}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->is_declare = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {{"if", IF}, {"elif", ELIF},
         {"else", ELSE}, {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
         {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF}, {"and", AND},
         {"or", OR}, {"struct", STRUCT_DEF}, {"is", EQUAL}, {"protoFunc", PROTO_FUNC},
         {"cast", CAST}, {0, 0},
      };
      for (i = 0; keywords[i].name; i++)
      {
         if (strcmp(keywords[i].name, new->name) == 0)
         {
            new->type = keywords[i].type;
            break;
         }
      }
      break;
   }
   case BLOC: case JMP: case JE: case JNE: case FDEC:
   case END_BLOC:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      if (e <= s) break;
      int len = e - s;
      new->Chars.value = allocate(len + 1, sizeof(char));
      char *value = new->Chars.value;
      int j = 0;

      for (int i = 0; i < len && s < e; i++, s++)
      {
         if (s + 1 < e && input[s] == '\\')
         {
            switch (input[s + 1])
            {
            case 'n': value[j++] = '\n'; s++; break;  // newline
            case 't': value[j++] = '\t'; s++; break;  // tab
            case 'r': value[j++] = '\r'; s++; break;  // carriage return
            case 'b': value[j++] = '\b'; s++; break;  // backspace
            case 'f': value[j++] = '\f'; s++; break;  // form feed
            case 'v': value[j++] = '\v'; s++; break;  // vertical tab
            case 'a': value[j++] = '\a'; s++; break;  // alert (bell)
            case '0': value[j++] = '\0'; s++; break;  // null character
            case '\\': value[j++] = '\\'; s++; break; // backslash
            case '"': value[j++] = '"'; s++; break;   // double quote
            case '\'': value[j++] = '\''; s++; break; // single quote
            case '?': value[j++] = '\?'; s++; break;  // question mark (for trigraphs)
            // Three digit octal
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
               if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
               {
                  int octal = (input[s + 1] - '0') * 64 +
                              (input[s + 2] - '0') * 8 +
                              (input[s + 3] - '0');
                  if (octal <= 255) { value[j++] = (char)octal; s += 3; }
                  // Invalid octal, keep backslash
                  else value[j++] = input[s];
               }
               // Two digit octal
               else if (s + 2 < e && isdigit(input[s + 2]))
               {
                  int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
                  value[j++] = (char)octal;
                  s += 2;
               }
               // Single digit octal
               else { value[j++] = (char)(input[s + 1] - '0'); s++; }
               break;
            case 'x':
               // Hexadecimal escape sequences (\x00 to \xFF)
               if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
               {
                  int hex = 0;
                  char c1 = input[s + 2];
                  char c2 = input[s + 3];

                  // Convert first hex digit
                  if (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
                  else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
                  else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;

                  // Convert second hex digit
                  if (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
                  else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
                  else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);

                  value[j++] = (char)hex;
                  s += 3;
               }
               else value[j++] = input[s]; // Invalid hex escape, keep backslash
               break;
            case 'u':
               // Unicode escape sequences (basic support)
               // \uXXXX - 4 hex digits for Unicode, basic implementation
               if (s + 5 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;
            case 'U':
               // \UXXXXXXXX - 8 hex digits for Unicode, basic implementation
               if (s + 9 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;

            default:
               // Unknown escape sequence, keep the backslash
               value[j++] = input[s];
               break;
            }
         }
         else value[j++] = input[s];
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
      if (e == s + 1) new->Char.value = input[s];
      else // is special character
      {
         switch (input[s + 1])
         {
         case 'n':  new->Char.value = '\n';  break;
         case 't':  new->Char.value = '\t';  break;
         case 'r':  new->Char.value = '\r';  break;
         case 'b':  new->Char.value = '\b';  break;
         case 'f':  new->Char.value = '\f';  break;
         case 'v':  new->Char.value = '\v';  break;
         case 'a':  new->Char.value = '\a';  break;
         case '0':  new->Char.value = '\0';  break;
         case '\\': new->Char.value = '\\';  break;
         case '"':  new->Char.value = '"'; break;
         case '\'': new->Char.value = '\'';  break;
         case '?':  new->Char.value = '\?';  break;
         default: break;
         }
      }
      break;
   }
   default:
      check(e > s, "implement adding name for this one %s", to_string(type));
      break;
   }
#if DEBUG
   print("new %k\n", new);
#endif
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   if (token->Struct.attrs)
   {
      new->Struct.attrs = allocate(token->Struct.size, sizeof(Token*));
      for (int i = 0; i < new->Struct.pos; i++)
         new->Struct.attrs[i] = copy_token(token->Struct.attrs[i]);
   }
   if (token->Struct.name) new->Struct.name = strdup(token->Struct.name);
   add_token(new);
   return new;
}

// IR
void copy_insts()
{
#if IR
   int pos = 0;
   int len = 100;
   free(insts);
   insts = allocate(len, sizeof(Inst *));

   for (int i = 0; OrgInsts && OrgInsts[i]; i++)
   {
      if (OrgInsts[i]->token->remove) continue;
      insts[pos++] = OrgInsts[i];
      if (pos + 1 == len)
      {
         Inst **tmp = allocate((len *= 2), sizeof(Inst *));
         memcpy(tmp, insts, pos * sizeof(Inst *));
         free(insts);
         insts = tmp;
      }
   }
#endif
}

void add_inst(Inst *inst)
{
   static int pos;
   static int len;
   if (len == 0)
   {
      len = 10;
      OrgInsts = allocate(len, sizeof(Inst *));
   }
   else if (pos + 2 == len)
   {
      Inst **tmp = allocate(len * 2, sizeof(Inst *));
      memcpy(tmp, OrgInsts, len * sizeof(Inst *));
      free(OrgInsts);
      OrgInsts = tmp;
      len *= 2;
   }
   OrgInsts[pos++] = inst;
}

Inst *new_inst(Token *token)
{
   static int ir_reg;

   Inst *new = allocate(1, sizeof(Inst));
   new->token = token;

   if (includes(token->type, DATA_TYPES) && token->name) token->ir_reg = ++ir_reg;
   switch (token->type)
   {
   case STRUCT_CALL:
   {
      for (int i = 0; i < token->Struct.pos; i++) {
         Token *attr = token->Struct.attrs[i];
         attr->ir_reg = ++ir_reg;
      }
      break;
   }
   default:
      token->ir_reg = ++ir_reg;
      break;
   }
#if DEBUG
   print("new inst: %k%c", new->token, token->type != STRUCT_CALL ? '\n' : '\0');
#endif
   add_inst(new);
   return new;
}

Token *new_variable(Token *token)
{
#if DEBUG
   print(CYAN "new variable [%s] [%s] in scoop %k\n" RESET,
         token->name, to_string(token->type), scoop->token);
#endif
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      check(strcmp(curr->name, token->name) == 0,
            "Redefinition of %s\n", token->name);
   }
   // if (token->ptr)
   // {
   //     print(RED"%k already has ptr"RESET, token);
   //     todo(1, "");
   // }
   // if (token->type == STRUCT_CALL)
   // {
   //     token->ptr = ptr;
   //     if (token->is_arg) token->ptr += sizeofToken(token->Struct.attrs[0]);
   //     setAttrName(NULL, token);
   //     inc_ptr(sizeofToken(token)); // TODO: each struct must have attributes
   // }
   // else
   // {
   //     inc_ptr(sizeofToken(token));
   //     token->ptr = ptr;
   // }
   // new_inst(token);
   add_variable(scoop, token);
   return token;
}

// UTILS
char* open_file(char *filename)
{
   if (found_error) return NULL;

#if defined(__APPLE__)
   struct __sFILE *file;
#elif defined(__linux__)
   struct _IO_FILE *file;
#endif
   file = fopen(filename, "r");
   if (check(!file, "openning %s", filename)) return NULL;
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

// Token names table (static, global to this translation unit)
static const char *type_names[] = {
   [TMP] = "TMP", [CHILDREN] = "CHILD", [DEFAULT] = "DEF",
   [COMMENT] = "COMMENT", [END] = "END",

   [ID] = "ID", [REF] = "REF",
   [REF_ID] = "REF_ID", [REF_HOLD_ID] = "REF_HID",
   [REF_VAL] = "REF_VAL", [REF_HOLD_REF] = "REF_HRF",
   [REF_REF] = "REF_REF", [ID_ID] = "ID_ID",
   [ID_REF] = "ID_REF", [ID_VAL] = "ID_VAL",

   [VOID] = "VOID", [INT] = "INT",
   [FLOAT] = "FLOAT", [LONG] = "LONG",
   [SHORT] = "SHORT", [BOOL] = "BOOL",
   [CHAR] = "CHAR", [CHARS] = "CHARS",
   [PTR] = "PTR",
   [STRUCT_DEF] = "ST_DEF", [STRUCT_BODY] = "ST_BODY",
   [STRUCT_ALLOC] = "ST_ALLOC", [STRUCT_CALL] = "ST_CALL",
   [STRUCT_ATTR] = "ST_ATTR",
   [ARRAY] = "ARRAY", [CAST] = "CAST",


   [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD_ASGN",
   [SUB_ASSIGN] = "SUB_ASGN", [MUL_ASSIGN] = "MUL_ASGN",
   [DIV_ASSIGN] = "DIV_ASGN", [MOD_ASSIGN] = "MOD_ASGN",
   [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQ",
   [LESS_EQUAL] = "LE_EQ", [MORE_EQUAL] = "MO_EQ",
   [LESS] = "LESS", [MORE] = "MORE", [ADD] = "ADD",
   [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV",
   [MOD] = "MOD", [AND] = "AND", [OR] = "OR", [NOT] = "NOT",

   [LPAR] = "LPAR", [RPAR] = "RPAR", [LBRA] = "LBRA",
   [RBRA] = "RBRA", [COMA] = "COMA", [DOT] = "DOT",
   [DOTS] = "DOTS", [ACCESS] = "ACCESS",

   [RETURN] = "RETURN", [ARROW] = "ARROW",
   [IF] = "IF", [ELIF] = "ELIF",
   [ELSE] = "ELSE", [END_IF] = "END_IF",
   [BUILD_COND] = "BLD_COND", [WHILE] = "WHILE",
   [CONTINUE] = "CONT", [BREAK] = "BREAK",
   [END_WHILE] = "END_WHILE", [BLOC] = "BLOC",
   [END_BLOC] = "ENDBLOC", [END_COND] = "ENDCOND",
   [APPEND_BLOC] = "APP_BLC", [SET_POS] = "SET_POS",
   [BUILD_BR] = "BLD_BR",

   [FDEC] = "FDEC", [FCALL] = "FCALL", [PROTO_FUNC] = "PROTO_FUNC",
   [JMP] = "JMP", [JE] = "JE", [JNE] = "JNE",

   [PUSH] = "PUSH", [POP] = "POP"
};


char *to_string_(char *filename, int line, Type type)
{
   int size = (int)(sizeof(type_names) / sizeof(type_names[0]));
   if (type > 0 && (int)type < size && type_names[type])
      return (char *)type_names[type];
   check(1, "Unknown type [%d] in %s:%d\n", type, filename, line);
   // seg();
   return "";
}

bool check_error(const char *filename, const char *funcname,
                 int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   found_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, BOLD RED"ura_error:%s:%s:%d "RESET,
           filename, funcname, line);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   print("\n");
   va_end(ap);
   return cond;
}

void add_token(Token *token)
{
   if (tk_len == 0)
   {
      tk_len = 10;
      tokens = allocate(tk_len, sizeof(Token *));
   }
   else if (tk_pos + 5 >= tk_len)
   {
      Token **tmp = allocate(tk_len * 2, sizeof(Token *));
      memcpy(tmp, tokens, tk_len * sizeof(Token *));
      free(tokens);
      tokens = tmp;
      tk_len *= 2;
   }
   tokens[tk_pos++] = token;
}

bool includes(Type to_find, ...)
{
   if (found_error) return false;
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0)
      if (current == to_find) return true;
   return false;
}

Token *find(Type type, ...)
{
   if (found_error) return NULL;
   va_list ap; va_start(ap, type);
   while (type)
   {
      // print("search for %t, current %t\n", type, tokens[exe_pos]->type);
      if (type == tokens[exe_pos]->type)
         return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};

bool within_space(int space)
{
   return tokens[exe_pos]->space > space &&
          tokens[exe_pos]->type != END && !found_error;
}

char *strjoin(char *str0, char *str1, char *str2)
{
   int len0 = str0 ? strlen(str0) : 0;
   int len1 = str1 ? strlen(str1) : 0;
   int len2 = str2 ? strlen(str2) : 0;
   char *res = allocate(len0 + len1 + len2 + 1, 1);
   if (str0) strcpy(res, str0);
   if (str1) strcpy(res + len0, str1);
   if (str2) strcpy(res + len0 + len1, str2);
   return res;
}

void setName(Token *token, char *name)
{
   if (token->name) free(token->name);
   token->name = name ? strdup(name) : NULL;
}

void enter_scoop(Node *node)
{
#if DEBUG
   debug(CYAN "Enter Scoop: %k index %d\n" RESET,
         node->token, scoopPos + 1);
#endif
   if (Gscoop == NULL)
   {
      scoopSize = 10;
      Gscoop = allocate(scoopSize, sizeof(Node*));
   }
   else if (scoopPos + 1 >= scoopSize)
   {
      Node **tmp = allocate(scoopSize * 2, sizeof(Node*));
      memcpy(tmp, Gscoop, scoopPos * sizeof(Node*));
      scoopSize *= 2;
      free(Gscoop);
      Gscoop = tmp;
   }
   scoopPos++;
   Gscoop[scoopPos] = node;
   scoop = Gscoop[scoopPos];
}

void exit_scoop()
{
   if (check(scoopPos < 0, "No active scoop to exit\n")) return;
#if DEBUG
   print(CYAN "Exit Scoop: %k index %d\n" RESET,
         Gscoop[scoopPos]->token, scoopPos);
#endif
   Gscoop[scoopPos] = NULL;
   scoopPos--;
   if (scoopPos >= 0) scoop = Gscoop[scoopPos];
}

void add_struct(Node *bloc, Token *token)
{
   if (bloc->structs == NULL)
   {
      bloc->ssize = 10;
      bloc->structs = allocate(bloc->ssize, sizeof(Token *));
   }
   else if (bloc->spos + 1 == bloc->ssize)
   {
      Token **tmp = allocate(bloc->ssize *= 2, sizeof(Token *));
      memcpy(tmp, bloc->structs, bloc->spos * sizeof(Token *));
      free(bloc->structs);
      bloc->structs = tmp;
   }
   bloc->structs[bloc->spos++] = token;
}

Token *new_struct(Token *token)
{
#if DEBUG
   print(CYAN "in scoop %k, new struct [%k]\n" RESET,
         scoop->token, token);
#endif
   for (int i = 0; i < scoop->spos; i++)
   {
      print(GREEN"loop [%d]\n"RESET, i);
      Token *curr = scoop->structs[i];
      check(strcmp(curr->Struct.name, token->Struct.name) == 0,
            "Redefinition of %s\n", token->Struct.name);
   }
   add_struct(scoop, token);
   return token;
}

Token *get_struct(char *name)
{
#if DEBUG
   print(CYAN "get struct [%s] from scoop %k\n"RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *node = Gscoop[j];
      todo(node == NULL, RED"Error accessing NULL, %d\n"RESET, j);
#if DEBUG
      print("[%d] scoop [%s] has %d structs\n", j,
            node->token->name, node->spos);
#endif
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->Struct.name, name) == 0)
            return node->structs[i];
   }
   // check(1, "%s not found\n", name);
   return NULL;
}

Token *is_struct(Token *token)
{
   Token *res = get_struct(token->name);
   if (res) return res;
   return NULL;
}

Node *new_node(Token *token)
{
   print("new node: %k\n", token);
   Node *new = allocate(1, sizeof(Node));
   new->token = token;
   return new;
}

Node *copy_node(Node *node)
{
   Node *new = allocate(1, sizeof(Node));
   new->token = copy_token(node->token);
   if (node->left) new->left = copy_node(node->left);
   if (node->right) new->right = copy_node(node->right);
   for (int i = 0; i < node->cpos; i++)
      add_child(new, copy_node(node->children[i]));
   for (int i = 0; i < node->spos; i++)
      add_struct(new, copy_token(node->structs[i]));
   for (int i = 0; i < node->vpos; i++)
      add_variable(new, copy_token(node->vars[i]));
   return new;
}

Node* add_child(Node *node, Node *child)
{
   if (node->csize == 0)
   {
      node->csize = 10;
      node->children = allocate(node->csize, sizeof(Node *));
   }
   else if (node->cpos + 1 == node->csize)
   {
      Node **tmp = allocate(node->csize * 2, sizeof(Node *));
      memcpy(tmp, node->children, node->csize * sizeof(Node *));
      free(node->children);
      node->children = tmp;
      node->csize *= 2;
   }
   child->token->space = node->token->space + TAB;
   node->children[node->cpos++] = child;
   return child;
}

void add_attribute(Token *obj, Token *attr)
{
   if (obj->Struct.attrs == NULL)
   {
      obj->Struct.size = 10;
      obj->Struct.attrs = allocate(obj->Struct.size, sizeof(Token *));
   }
   else if (obj->Struct.pos + 1 == obj->Struct.size)
   {
      Token **tmp = allocate((obj->Struct.size *= 2), sizeof(Token *));
      memcpy(tmp, obj->Struct.attrs, obj->Struct.pos * sizeof(Token *));
      free(obj->Struct.attrs);
      obj->Struct.attrs = tmp;
   }
   attr->space = obj->space + TAB;
   obj->Struct.attrs[obj->Struct.pos++] = attr;
}

void add_variable(Node *bloc, Token *token)
{
   if (bloc->vars == NULL)
   {
      bloc->vsize = 10;
      bloc->vars = allocate(bloc->vsize, sizeof(Token *));
   }
   else if (bloc->vpos + 1 == bloc->vsize)
   {
      Token **tmp = allocate(bloc->vsize *= 2, sizeof(Token *));
      memcpy(tmp, bloc->vars, bloc->vpos * sizeof(Token *));
      free(bloc->vars);
      bloc->vars = tmp;
   }
   bloc->vars[bloc->vpos++] = token;
}

Token *get_variable(char *name)
{
#if DEBUG
   print(CYAN "get variable [%s] from scoop %k\n"
         RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->vars[i]->name, name) == 0)
            return scoop->vars[i];
   }
   check(1, "%s not found\n", name);
   return NULL;
}

void add_function(Node *bloc, Node *node)
{
   if (bloc->functions == NULL)
   {
      bloc->fsize = 10;
      bloc->functions = allocate(bloc->fsize, sizeof(Node *));
   }
   else if (bloc->fpos + 1 == bloc->fsize)
   {
      bloc->fsize *= 2;
      Node **tmp = allocate(bloc->fsize, sizeof(Node *));
      memcpy(tmp, bloc->functions, bloc->fpos * sizeof(Node *));
      free(bloc->functions);
      bloc->functions = tmp;
   }
   bloc->functions[bloc->fpos++] = node;
}

Node *new_function(Node *node)
{
#if DEBUG
   // print("new_func %s in scoop %kthat return %t\n", node->token->name, scoop->token, node->token->retType);
#endif
   for (int i = 0; i < scoop->fpos; i++)
   {
      Node *func = scoop->functions[i];
      bool cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s\n", node->token->name);
   }
   // switch (node->token->retType)
   // {
   // case CHAR: case INT: case VOID:
   //     setReg(node->token, "eax"); break;
   // case LONG: case PTR: case CHARS:
   //     setReg(node->token, "rax"); break;
   // default:
   //     todo(1, "handle this case %s\n", to_string(node->token->retType));
   //     break;
   // }
   add_function(scoop, node);
   return node;
}

Node *get_function(char *name)
{
#if DEBUG
   print("get_func %s in scoop %k\n", name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->fpos; i++)
         if (strcmp(scoop->functions[i]->token->name, name) == 0)
            return scoop->functions[i];
   }
   check(1, "'%s' Not found\n", name);
   return NULL;
}

bool compatible(Token *left, Token *right)
{
   Type left_type = left->type;
   Type right_type = right->type;
   Type left_ret_type = left->retType;
   Type right_ret_type = right->retType;
   if (left_type == CHARS && (right_type == PTR || right_ret_type == PTR))
      return true;
   return (left_type == right_type || left_type == right_ret_type ||
           left_ret_type == right_type || left_ret_type == right_ret_type);
}

Type getRetType(Node *node)
{
   if (!node || !node->token) return 0;
   if (includes(node->token->type, INT, CHARS, CHAR, FLOAT, BOOL, 0))
      return node->token->type;
   if (includes(node->token->retType, INT, CHARS, CHAR, FLOAT, BOOL, 0))
      return node->token->retType;

   Type left = 0, right = 0;
   if (node->left) left = getRetType(node->left);
   if (node->right) right = getRetType(node->right);
   if (left) return left;
   if (right) return right;
   return 0;
}

char* resolve_path(char* path)
{
   if (path == NULL) return NULL;

   char* cleaned = allocate(strlen(path) + 5, 1);
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

bool found_error;

Token **tokens;
int tk_pos;
int tk_len;

char *input;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

int ptr;
#if defined(__APPLE__)
struct __sFILE *asm_fd;
#elif defined(__linux__)
struct _IO_FILE *asm_fd;
#endif


void tokenize(char *filename)
{
   if (found_error) return;
#if DEBUG
   print(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);
#endif
   char *input = open_file(filename);
   if (!input) return;

   struct { char *value; Type type; } specials[] = {
      {"->", ARROW},
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
      {"[", LBRA}, {"]", RBRA}, {",", COMA}, {"&&", AND}, {"||", OR},
      {0, (Type)0}
   };

   int space = 0;
   int line = 0;
   bool new_line = true;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            line++;
            new_line = true;
            space = 0;
         }
         else if (new_line)
         {
            if (input[i] == '\t') space += TAB;
            else space++;
         }
         i++;
         continue;
      }
      else if (strncmp(input + i, "/*", 2) == 0)
      {
         i += 2;
         while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2))
         {
            if (input[i] == '\n') line++;
            i++;
         }
         check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'\n");
         i += 2;
         continue;
      }
      else if (strncmp(input + i, "//", 2) == 0)
      {
         while (input[i] && input[i] != '\n') i++;
         continue;
      }
      new_line = false;
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
         {
            parse_token(filename, line, NULL, 0, 0, specials[j].type, space);
            found = true;
            i += strlen(specials[j].value);
            if (includes(specials[j].type, ARROW, DOTS, 0)) space += TAB;
            break;
         }
      }
      if (found) continue;
      if (input[i] && input[i] == '\"')
      {
         i++;
         while (input[i] && input[i] != '\"') i++;
         check(input[i] != '\"', "Expected '\"'");
         parse_token(filename, line, input, s + 1, i, CHARS, space);
         i++;
         continue;
      }
      if (input[i] && input[i] == '\'')
      {
         i++;
         if (input[i] == '\\') i++;
         if (input[i] && input[i] != '\'')
            i++;
         check(input[i] != '\'', "Expected '\''");
         parse_token(filename, line, input, s + 1, i, CHAR, space);
         i++;
         continue;
      }
      if (isalpha(input[i]) || strchr("@$-_", input[i]))
      {
         while (input[i] && (isalnum(input[i]) || strchr("@$-_", input[i]))) i++;
         if (strncmp(input + s, "use ", 4) == 0)
         {
            while (isspace(input[i])) i++;
            check(input[i] != '\"', "expected \" after use"); i++;
            s = i;
            while (input[i] && !isspace(input[i]) && input[i] != '\"') i++;
            check(input[i] != '\"', "expected \""); i++;

            char *use = allocate(i - s, sizeof(char));
            strncpy(use, input + s, i - s - 1);

            char *use_filename = strjoin(dirname(filename), "/", use);
            free(use);
            use = strjoin(use_filename, ".ura", NULL);
            tokenize(use);
            free(use_filename);
         }
         else parse_token(filename, line, input, s, i, ID, space);
         continue;
      }
      if (isdigit(input[i]))
      {
         while (isdigit(input[i])) i++;
         parse_token(filename, line, input, s, i, INT, space);
         continue;
      }
      check(input[i], "Syntax error <%c>\n", input[i]);
   }
   free(input);
}

Node *expr() {
   return assign();
}

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, 0)))
   {
      Node *node = new_node(token);

      if (token->type == ASSIGN)
      {
         node->left = left;
         node->right = logic();
      }
      else
      {
         node->left = left;
         node->right = new_node(copy_token(token));
         node->right->left = new_node(left->token);
         node->right->right = logic();
         switch (token->type)
         {
         case ADD_ASSIGN: node->right->token->type = ADD; break;
         case SUB_ASSIGN: node->right->token->type = SUB; break;
         case MUL_ASSIGN: node->right->token->type = MUL; break;
         case DIV_ASSIGN: node->right->token->type = DIV; break;
         case MOD_ASSIGN: node->right->token->type = MOD; break;
         default: break;
         }
         node->token->type = ASSIGN;
      }
      left = node;
   }
   return left;
}
AST_NODE(logic, equality, AND, OR, 0);
// TODO: handle ! operator
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison, add_sub, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub, mul_div, ADD, SUB, 0);
AST_NODE(mul_div, dot, MUL, DIV, MOD, 0); // TODO: handle modulo %

Node *dot()
{
   Node *left = brackets();
   Token *token;
   while ((token = find(DOT, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      token = find(ID, 0);
      todo(!token, "error, expected id after dot");
      node->right = new_node(token);
      left = node;
   }
   return left;
}

Node *brackets()
{
   Node *left = sign();
   Token *token;
   if ((token = find(LBRA, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = brackets();
      check(!find(RBRA, 0), "expected right bracket");
      return node;
   }
   return left;
}

Node *sign() // TODO: implement it
{
   Token *token;
   if ((token = find(SUB, 0)))
   {
      // TODO: left should be a number
      Node *node = new_node(token);
      token->type = MUL;
      node->left = prime();

      node->right = new_node(new_token(INT, node->left->token->space));
      node->right->token->Int.value = -1;
      return node;
   }
   return prime();
}

Node *func_dec(Node *node)
{
   // Function Declaration:
   //    + left children: arguments
   //    + children     : code block
   Token *typeName = find(DATA_TYPES, 0);
   check(!typeName, "Expected data type after func declaration");
   if (typeName->type == ID)
   {
      typeName = get_struct(typeName->name);
      todo(1, "handle function return struct properly");
   }
   Token *fname = find(ID, 0);
   if (check(!typeName || !fname,
             "expected data type and identifier after func declaration"))
      return node;
   node->token->retType = typeName->type;
   node->token->is_proto = (node->token->type == PROTO_FUNC);
   node->token->type = FDEC;
   setName(node->token, fname->name);
   enter_scoop(node);

   check(!find(LPAR, 0), "expected ( after function declaration");
   node->left = new_node(new_token(CHILDREN, node->token->space));
   Token *last;
   while (!found_error && !(last = find(RPAR, END, 0)))
   {
      bool is_ref = find(REF, 0) != NULL;
      Token* data_type = find(DATA_TYPES, ID, 0);
      if (data_type && data_type->type == ID)
      {
         data_type = get_struct(data_type->name);
         if (data_type) data_type->type = STRUCT_CALL;
      }
      if (check(!data_type, "expected data type in function argument")) break;
      Token *name = find(ID, 0);
      if (check(!name, "expected identifier in function argument")) break;
      Node *curr;
      if (data_type->type == STRUCT_CALL)
      {
         curr = new_node(data_type);
         data_type->is_ref = is_ref;
         setName(data_type, name->name);
      }
      else
      {
         curr = new_node(name);
         name->is_ref = is_ref;
         name->type = data_type->type;
      }
      curr->token->is_declare = true;
      add_child(node->left, curr);
      find(COMA, 0); // TODO: check this later
   }
   check(!found_error && last->type != RPAR, "expected ) after function declaration");

   if (!node->token->is_proto)
   {
      Token *next = find(DOTS, ARROW, 0);
      check(!found_error && !next, "Expected : after function declaration");
      Node *child = NULL;

      if (next->type == DOTS) while (within_space(node->token->space)) child = add_child(node, expr());
      else
      {
         todo(1, "stop");
         Token *retToken = copy_token(next);
         retToken->type = RETURN;
         Node *retNode = new_node(retToken);
         retNode->left = expr();
         child = add_child(node, retNode);
      }
      if (next->type == DOTS)
      {
         if (node->token->retType != VOID)
            check(!child || child->token->type != RETURN, "expected return statment");
         else
         {
            Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
            ret->left = new_node(new_token(VOID, node->token->space + TAB));
            add_child(node, ret);
         }
      }
   }
   exit_scoop();
   return node;
}

Node *func_call(Node *node)
{
   // Function call:
   //    + children: Parameters
   node->token->type = FCALL;
   Token *arg = NULL;
   Token *token = node->token;

   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      Node *curr = expr();
      curr->token->space = token->space;
      add_child(node, curr);
      find(COMA, 0);
   }
   check(!found_error && arg->type != RPAR, "expected ) after function call");
   return node;
}

Node *func_main(Node *node)
{
   // Function main:
   //    + children: code bloc
   check(!find(RPAR, 0), "expected ) after main declaration");
   check(!find(DOTS, 0), "expected : after main() declaration");

   enter_scoop(node);
   node->token->type = FDEC;
   node->token->retType = INT;

   Node *last = NULL;
   while (within_space(node->token->space))
   {
      last = expr();
      add_child(node, last);
   }
   if (!last || last->token->type != RETURN)
   {
      last = new_node(new_token(RETURN, node->token->space + TAB));
      last->left = new_node(new_token(INT, node->token->space + TAB));
      add_child(node, last);
   }
   exit_scoop();
   return node;
}

Node *symbol(Token *token)
{
   // Token *struct_token = NULL;
   Node *node;
   Token *st_dec = NULL;
   if (token->is_declare)
   {
      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n",
            to_string(token->type));
      setName(token, tmp->name);
      return new_node(token);
   }
   else if (token->type == ID && find(LPAR, 0))
   {
      node = new_node(token);
      if (strcmp(token->name, "main") == 0) return func_main(node);
      return func_call(node);
   }
   else if (token->type == ID && (st_dec = get_struct(token->name)))
   {
      token = copy_token(st_dec);
      token->type = STRUCT_CALL;
      token->Struct.ptr = st_dec;
      token->is_declare = true;

      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n",
            to_string(token->type));
      setName(token, tmp->name);
      return new_node(token);
   }
   else if (token->type == ID && find(LBRA, 0))
   {
      node = new_node(copy_token(token));
      node->token->type = ACCESS;
      Node *index = prime();
      check(!index || !index->token, "expected index after left bracket\n");
      check(!find(RBRA, 0), "expected right bracket\n");
      node->left = new_node(token);
      node->right = index;
      return node;
   }
   return new_node(token);
}

Node *struct_def(Node *node)
{
   // Struct def Layout:
   //    + children: attributes
   Token *st_name;
   if (check(!(st_name = find(ID, 0)),
             "expected identifier after struct definition"))
      return NULL;
   if (check(!find(DOTS, 0), "expected dots after struct definition"))
      return NULL;

   setName(node->token, NULL);
   node->token->Struct.name = strdup(st_name->name);
   while (within_space(node->token->space))
   {
      Token *attr = find(DATA_TYPES, ID, 0);
      Token *id = find(ID, 0);
      if (check(!attr, "expected data type followed by id"))
      {
         ptoken(tokens[exe_pos]);
         break;
      }
      if (check(!id, "expected id after data type")) break;
      id->type = attr->type;
      id->is_attr = true; // TODO: check if is-attr is useful
      add_attribute(node->token, id);
   }
   // set_struct_size(node->token);
   new_struct(node->token);
   return node;
}

Node *if_node(Node *node)
{
   // if Layout:
   //    + left    : condition
   //    + children: code bloc
   //    + right   : elif/else Layout
   enter_scoop(node);

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(CHILDREN, node->token->space));

   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());

   while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) &&
          within_space(node->token->space - TAB))
   {
      Token *token = find(ELSE, ELIF, 0);
      Node *curr = add_child(node->right, new_node(token));
      token->space -= TAB;
      if (token->type == ELIF)
      {
         enter_scoop(curr);
         curr->left = expr();
         Node *bloc = add_child(node->right, new_node(copy_token(token)));
         setName(bloc->token, "bloc");
         bloc->token->space -= TAB;
         check(!find(DOTS, 0), "expected : after elif condition");
         while (within_space(token->space)) add_child(curr, expr());
         exit_scoop();
      }
      else if (token->type == ELSE)
      {
         enter_scoop(curr);
         check(!find(DOTS, 0), "expected : after else");
         while (within_space(token->space)) add_child(curr, expr());
         exit_scoop();
         break;
      }
   }

   Node *end = add_child(node->right, new_node(new_token(END_IF, 0)));
   end->token->space = node->right->token->space;
   setName(end->token, "end_if");

   exit_scoop();
   return node;
}

Node *while_node(Node *node)
{
   // while Layout:
   //    + left    : condition
   //    + children: code bloc
   enter_scoop(node);

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->space = node->token->space;
   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());

   exit_scoop();
   return node;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, LONG, SHORT, 0)))
      return symbol(token);
   else if ((token = find(STRUCT_DEF, 0)))
      return struct_def(new_node(token));
   // else if ((token = find(REF, 0)))
   // {
   //     node = prime(); // TODO: check it
   //     check(!node->token->is_declare, "must be variable declaration after ref");
   //     node->token->is_ref = true;
   //     return node;
   // }
   else if ((token = find(FDEC, PROTO_FUNC, 0))) return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
      // in current scoop
      node = new_node(token);
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *curr = Gscoop[i];
         print(">> %k\n", curr->token);
         if (curr->token->type == FDEC)
         {
            // TODO: check return type here
            if (curr->token->retType == VOID)
            {
               node->left = copy_node(node);
               node->left->token->type = VOID;
            }
            else node->left = expr();
            break;
         }
      }
      return node;
   }
   else if ((token = find(CAST, 0)))
   {
      node = new_node(token);
      Token *tkType = find(DATA_TYPES);
      node->right = new_node(tkType);
      node->left = expr();
      return node;
   }
   else if ((token = find(IF, 0))) return if_node(new_node(token));
   else if ((token = find(WHILE, 0))) return while_node(new_node(token));
   else if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr();
      check(!find(RPAR, 0), "expected right par");
      return node;
   }
   else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}

Token *func_dec_ir(Node *node)
{
   new_function(node);
   enter_scoop(node);
   // int tmp_ptr = ptr;
   // ptr = 0;

   Inst* inst = NULL;
   // if (!node->token->is_proto)
   inst = new_inst(node->token);

   // parameters
   Node **params = (node->left ? node->left->children : NULL);
   Token *token = node->token;
   for (int i = 0; params && i < node->left->cpos && !found_error; i++)
   {
      Node *child = params[i];
      generate_ir(child);
      if (token->Fdec.args == NULL)
      {
         token->Fdec.size = 10;
         token->Fdec.args = allocate(token->Fdec.size, sizeof(Token*));
      }
      else if (token->Fdec.pos + 1 == token->Fdec.size)
      {
         Token **tmp = allocate(token->Fdec.size *= 2, sizeof(Token*));
         memcpy(tmp, token->Fdec.args, token->Fdec.pos * sizeof(Token*));
         free(token->Fdec.args);
         token->Fdec.args = tmp;
      }
      child->token->is_param = true;
      child->token->Param.index = i;
      child->token->Param.func_ptr = node->token;
      child->token->is_declare = false;
      token->Fdec.args[token->Fdec.pos++] = child->token;
   }

   // if (node->token->is_proto) set_remove(node);
   // code bloc
   for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
   {
      Node *child = node->children[i];
      generate_ir(child);
   }

   if (!node->token->is_proto)
   {
      // TODO: if RETURN not found add it
      Token *new = new_token(END_BLOC, node->token->space);
      new->name = strdup(node->token->name);
      new_inst(new);
   }
   exit_scoop();
   // if (!node->token->is_proto)
   return inst->token;
}

Token *func_call_ir(Node *node)
{
   Inst* inst = NULL;
   // static Node *printf_func = NULL;

   if (strcmp(node->token->name, "output") == 0)
   {
#if 1
      todo(1, "handle this\n");
#else
      // if (!printf_func)
      // {

      // }

      node->token->Fcall.ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID

         Token *src = generate_ir(carg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;

         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      inst = new_inst(node->token);
#endif
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      node->token->Fcall.ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;

      func = copy_node(func);
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      if (check(fcall->cpos != fdec->cpos,
                "Incompatible number of arguments %s", func->token->name))
         return NULL;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID
         Node *darg = fdec->children[i];

         Token *src = generate_ir(carg);
         Token *dist = generate_ir(darg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;
         if (check(!compatible(src, dist), "Incompatible type arg %s",
                   func->token->name)) break;
         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      free_node(func);
      inst = new_inst(node->token);
   }
   return inst->token;
}

Token *if_ir(Node *node)
{
   enter_scoop(node);
   Node **children = node->right->children;
   int cpos = node->right->cpos;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (!cond) return NULL;

   // APPEND BLOC
   Token *start = copy_token(node->token);
   Inst *bloc = new_inst(copy_token(start));
   bloc->token->type = APPEND_BLOC;
   bloc->left = start;

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Inst *bloc = new_inst(copy_token(children[i]->token));
      bloc->token->type = APPEND_BLOC;
      bloc->left = children[i]->token;
   }

   // BUILD CONDITION
   // cond ? go to left : go to right
   Inst *inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = start;
   inst->right = children[0]->token;

   // SET POSITION start
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = start;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = children[cpos - 1]->token;

   // SET POSITION next

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Node *curr = children[i];
      inst = new_inst(copy_token(node->token));
      inst->token->type = SET_POS;
      inst->left = children[i]->token;
      switch (curr->token->type)
      {
      case ELSE:
      {
         enter_scoop(curr);
         for (int j = 0; j < curr->cpos; j++)
            generate_ir(curr->children[j]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(curr->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case ELIF:
      {
         enter_scoop(curr);
         Token *cond = generate_ir(curr->left); // TODO: check if it's boolean
         if (!cond) return NULL;
         i++;

         // BUILD CONDITION
         // cond ? go to left : go to right
         Inst *inst = new_inst(curr->token);
         inst->token->type = BUILD_COND;
         inst->token->Statement.ptr = cond;
         inst->left = children[i]->token;
         inst->right = children[i + 1]->token;


         // SET POSITION next
         inst = new_inst(copy_token(node->token));
         inst->token->type = SET_POS;
         inst->left = children[i]->token;

         for (int i = 0; i < curr->cpos && !found_error; i++)
            generate_ir(curr->children[i]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(node->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case END_IF: break;
      default: break;
      }
   }
   exit_scoop();
   return NULL;
}

Token *while_ir(Node *node)
{
   enter_scoop(node);
   // APPEND BLOC
   Token *loop_cond = copy_token(node->token);
   setName(loop_cond, "while");
   Inst *bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_cond;

   Token *loop_body = copy_token(node->token);
   setName(loop_body, "while_bloc");
   bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_body;

   Token *loop_end = copy_token(node->token);
   setName(loop_end, "end_while");
   bloc = new_inst(copy_token(loop_end));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_end;

   node->token->Statement.start = loop_cond;
   node->token->Statement.end = loop_end;

   // BUILD BR
   Inst *inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_cond;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (!cond) return NULL;

   // BUILD CONDITION
   // cond ? go to left : go to right
   inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = loop_body;
   inst->right = loop_end;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_body;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_end;

   exit_scoop();
   return NULL;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   if (!right || !right) return NULL;

   // TODO: fix the check later
   // check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
   //       to_string(node->token->type), to_string(left->type), to_string(right->type));
   switch (node->token->type)
   {
   case ASSIGN:
   {
      node->token->ir_reg = left->ir_reg;
      node->token->retType = getRetType(node);
#if 0
      // if (left->is_ref) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
      //         else node->token->assign_type = REF_HOLD_REF;
      //     }
      //     else if (right->ptr || right->creg) // ptr
      //     {
      //         if (left->has_ref) node->token->assign_type = REF_ID;
      //         else node->token->assign_type = REF_HOLD_ID;
      //     }
      //     else // ir_reg, value
      //     {
      //         print("line %d: %n\n", LINE, node);
      //         // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
      //         node->token->assign_type = REF_VAL;
      //     }
      //     left->has_ref = true;
      // }
      // else if (left->ptr || left->creg) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         node->token->assign_type = ID_REF;
      //     }
      //     else if (right->ptr) // ptr
      //         node->token->assign_type = ID_ID;
      //     else // ir_reg, value
      //         node->token->assign_type = ID_VAL;
      // }
      // else if (left->type == STRUCT_CALL)
      // {
      //     stop(1, "check this");
      //     print(">> %k\n", left);
      //     print(">> %k\n", right);
      //     // TODO: check compatibility
      //     for (int i = 0; i < left->Struct.pos; i++)
      //     {
      //         Node *tmp = new_node(new_token(ASSIGN, node->token->space));
      //         tmp->left = new_node(left->Struct.attrs[i]);
      //         tmp->right = new_node(right->Struct.attrs[i]);
      //         op_ir(tmp);
      //         free_node(tmp);
      //     }
      //     return NULL;
      //     // exit(1);
      // }
      // else
      // {
      //     pnode(node, NULL, 0);
      //     print("<%k>\n", left);
      //     print("<%k>\n", right);
      //     // todo(1, "Invalid assignment");
      // }
#endif
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   {
      // TODO: to be checked
      // node->token->retType = INT;
      // if (node->token->retType  == INT) setReg(node->token, "eax");
      // else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
      // else
      // check(1, "handle this case");
      break;
   }
   case AND: case OR:
   {
      node->token->retType = BOOL;
      break;
   }
   case NOT_EQUAL: case EQUAL: case LESS:
   case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      node->token->retType = BOOL;
      // node->token->index = ++bloc_index;
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   return node->token;
}

Token *struct_def_ir(Node *node)
{
   new_inst(node->token);
   return node->token;
}

Token *struct_call_ir(Node *node)
{
   // ALLOCATE STRUCT
   // ADD ATTRIBUTE
#if 0
   Inst *inst = NULL;
   Token **attrs = node->token->Struct.attrs;
   int attrs_size = node->token->Struct.pos;
   // node->token->Struct.attrs = NULL;
   // node->token->Struct.pos = 0;

   // set struct body
   // Token *body = copy_token(node->token);
   // body->type = STRUCT_BODY;
   // inst = new_inst(body);
   // inst->left = node->token;

   // allocate struct
   // Token *alloca_st = copy_token(node->token);
   // alloca_st->type = STRUCT_ALLOC;
   // inst = new_inst(alloca_st);
   // inst->left = node->token;

   // TODO: handle children if they are struct type
   // ...
   for (int i = 0; i < attrs_size; i++)
   {

   }
#endif
   return node->token;
}

Token *generate_ir(Node *node)
{
   Inst *inst = NULL;
   if (found_error) return NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find) return find;
      return node->token;
   }
   case CAST:
   {

      break;
   }
   // case VOID: return node->token;
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      inst = new_inst(node->token);
      if (node->token->type == STRUCT_CALL) struct_call_ir(node);
      if (node->token->is_declare) new_variable(node->token);
      return node->token;
   }
   case STRUCT_DEF: return struct_def_ir(node);
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
   case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL: case MOD:
   {
      // TODO: check if right is DEFAULT, then initlize it, and return left
      return op_ir(node);
   }
   case AND: case OR:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(!(left->type == BOOL || left->retType == BOOL),
            "left should be boolean but instead recieved (%s) (%s)", to_string(left->type),
            left->retType ? to_string(left->retType) : "");
      check(!(right->type == BOOL || right->retType == BOOL),
            "right should be boolean but instead recieved (%s) (%s)", to_string(right->type),
            right->retType ? to_string(right->retType) : "");

      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      inst->token->retType = BOOL;
      return inst->token;
   }
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FCALL: return func_call_ir(node);
   case FDEC:  return func_dec_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      return node->token;
   }
   case BREAK: case CONTINUE:
   {
      print(RED"handle BREAK/CONTINUE\n");
      bool found = false;
      for (int i = scoopPos; i >= 0 && !found; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            // BUILD BR
            found = true;
            Inst *inst = new_inst(node->token);
            if (node->token->type == BREAK)
            {
               inst->left = scoop->token->Statement.end;
               // print(RED "break to %k\n" RESET, scoop->token->Statement.end);
            }
            else inst->left = scoop->token->Statement.start;
            inst->token->type = BUILD_BR;
         }
      }
      todo(!found, "Invalid syntax\n");
      break;
   }
   case DOT:
   {
      Token *left = generate_ir(node->left); // struct name
      Token *right = node->right->token; // attribute
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(left->type != STRUCT_CALL,
                "%s should be a struct call", left->name)) break;
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         // print("compare %s == %s\n", attr->name, right->name);
         if (strcmp(attr->name, right->name) == 0)
         {
            attr->Struct.attr_index = i;
            attr->Struct.ptr = left->Statement.ptr;
            Inst *inst =  new_inst(node->token);

            inst->left = left;
            inst->right = attr;
            inst->token->is_attr = true;
            return inst->token;
         }
      }

      check(1, "%s has no attribute %s", left->name, right->name);
      break;
   }
   case ACCESS:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(right->type != INT, "should be int")) break;

      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      switch (inst->left->type)
      {
      case CHARS: inst->token->retType = CHAR; break;
      default: check(1, "handle this case %s\n", to_string(inst->left->type)); break;
         break;
      }
      return node->token;
      break;
   }
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
   return NULL;
}

void compile(char *filename)
{
   tokenize(filename);
   new_token(END, -1);
   if (found_error) return;

   Node *global = new_node(new_token(ID, -TAB - 1));
   setName(global->token, ".global");
   enter_scoop(global);

   if (DEBUG) print(GREEN BOLD"AST:\n" RESET);
   while (tokens[exe_pos]->type != END && !found_error)
      add_child(global, expr());
   print_ast(global);
   if (found_error) return;

#if IR
   if (DEBUG) print(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      generate_ir(global->children[i]);
   if (found_error) return;
   if (DEBUG) print_ir();
#endif

#if OPTIMIZE
   print(GREEN BOLD"OPTIMIZE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   copy_insts();
   while (OPTIMIZE && !found_error && optimize_ir()) copy_insts();
#if !DEUBG
   print_ir();
#endif
#endif

#if ASM
   if (DEBUG) print(GREEN BOLD"GENERATE ASSEMBLY CODE:\n" RESET);
   copy_insts();
   generate_asm(filename);
#endif

   free_node(global);
   if (DEBUG) print(BLUE BOLD"FINISH COMPILATION:\n" RESET);
}

int main(int argc, char **argv)
{
   check(argc < 2, "require argument, usage ura <file.ura>");
   compile(argv[1]);
   free_memory();
#if !IR
   print(RED"NO IR GENERATION\n"RESET);
#endif
#if !ASM
   print(RED"NO ASSEMBLY GENERATION\n"RESET);
#endif
   if (found_error || !ASM || !IR) exit(1);
}


