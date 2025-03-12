// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define EXIT_STATUS 0
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1
#define TAB 4

#if TOKENIZE
   #define AST 1
#endif

#if AST
   #define IR 1
#else
   #define IR 0
#endif

#define WITH_COMMENTS 1

#if IR
   #define BUILTINS 0
   #ifndef OPTIMIZE
      #define OPTIMIZE 1
   #endif

   #define ASM 1
#else
   #define ASM 0
#endif

#ifndef DEBUG
   #define DEBUG 1
#endif

#define check(cond, fmt, ...) check_error(__FILE__, __func__, __LINE__, cond, fmt, ##__VA_ARGS__)

// STRUCTS
typedef enum
{
   START = 1,
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   ADD, SUB, MUL, DIV, MOD,
   AND, OR,
   RPAR, LPAR, COMA, DOT, DOTS,
   RETURN,
   IF, ELIF, ELSE, WHILE,
   FDEC, FCALL,
   VOID, INT, CHARS, CHAR, BOOL, FLOAT,
   STRUCT, ID, BLOC, END_BLOC,
   JNE, JE, JMP,
   PUSH, POP,
   END
} Type;

typedef struct
{
   Type type;
   Type retType;
   char *name;
   size_t ptr;
   bool declare;
   size_t space;
   bool remove;
   size_t reg;
   char *creg;
   size_t index;
   bool isCond;
   // size_t rsp;
   // bool isarg;

   struct
   {
      // integer
      struct
      {
         long long value;
         int power;
         struct Int *next;
      } Int;
      // float
      struct
      {
         float value;
      } Float;
      // boolean
      struct
      {
         bool value;
         char c;
      } Bool;
      // chars
      struct
      {
         char *value;
      } Chars;
      // char
      struct
      {
         char value;
      } Char;
   };
} Token;

typedef struct Node
{
   struct Node *left;
   struct Node *right;
   Token *token;
} Node;

typedef struct
{
   char *value;
   Type type;
} Specials;

typedef struct
{
   char *name;

   Node **functions;
   size_t fpos;
   size_t fsize;

   Token **vars;
   size_t vpos;
   size_t vsize;

} Scoop;

typedef struct
{
   Token *token;
   Token *left;
   Token *right;
} Inst;

// PARSE
void tokenize();
void generate_ast();
void setName(Token *token, char *name);
void setReg(Token *token, char *creg);
void add_token(Token *token);
Node *new_node(Token *token);
Token *find(Type type, ...);
void parse();

// AST
Node *expr();
Node *assign();
Node *logic();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *dot();
Node *sign();
Node *prime();

// GENERATE
Token* generate_ir();
bool optimize_ir();
void generate_asm();
void generate();

// UTILS
const char *to_string(Type type);
char* open_file();
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(size_t line, size_t len, size_t size);
int debug(char *conv, ...);
bool includes(Type *types, Type type);
void pnode(Node *node, char *side, int space);
