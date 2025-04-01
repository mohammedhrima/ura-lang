#pragma once

// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "macros.h"

// STRUCTS
typedef enum
{
   START = 1, CHILDREN,
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   ADD, SUB, MUL, DIV, MOD,
   AND, OR, NOT,
   LPAR, RPAR, LBRA, RBRA, COMA, DOT, DOTS,
   RETURN,
   IF, ELIF, ELSE, 
   WHILE, CONTINUE, BREAK,
   FDEC, FCALL,
   VOID, INT, CHARS, CHAR, BOOL, FLOAT,
   STRUCT_DEF, STRUCT_CALL, ID, REF,
   JNE, JE, JMP, BLOC, END_BLOC,
   PUSH, POP,
   END
} Type;

typedef struct Token
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
   bool isref;
   bool hasref;
   // size_t rsp;
   bool isarg;
   size_t struct_id;
   bool isattr;
   size_t offset;

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
      struct
      {
         struct Token **attrs;
         size_t pos;
         size_t len;
      } Struct;
   };
} Token;

typedef struct Node
{
   struct Node *left;
   struct Node *right;
   Token *token;

   struct Node **children;
   size_t cpos; // children pos
   size_t csize; // children size

} Node;

typedef struct
{
   char *value;
   Type type;
} Specials;

typedef struct
{
   Token *token;

   Node **functions;
   size_t fpos;
   size_t fsize;

   Token **structs;
   size_t spos;
   size_t ssize;

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

// GLOBAL
extern bool found_error;
extern bool did_pasm;
extern char *input;
extern Token **tokens;
extern Node *head;
extern size_t exe_pos;
extern Inst **OrgInsts;
extern Inst **insts;
extern Scoop *Gscoop;
extern ssize_t scoopSize;
extern ssize_t scoopPos;
extern Scoop *scoop;
extern size_t ptr;
extern struct _IO_FILE *asm_fd;

void open_file(char *filename);
Token* new_token_(char *filename, int line, char *input, size_t s, size_t e, Type type, size_t space);

void tokenize();
void generate_ast();
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
void generate(char *name);
Inst *new_inst(Token *token);
void setName(Token *token, char *name);
void setReg(Token *token, char *creg);
bool within_space(size_t space);
void add_token(Token *token);
Node *new_node(Token *token);
Token *find(Type type, ...);
const char *to_string_(const char *filename, const int line, Type type);
void enter_scoop(Token *token);
bool check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(size_t line, size_t len, size_t size);
int debug(char *conv, ...);
int pnode(Node *node, char *side, size_t space);
int ptoken_(const char*filename, int line, Token *token);
void exit_scoop();
void clone_insts();
Node *new_function(Node *node);
Node *get_function(char *name);
Token *get_variable(char *name);
Token *new_variable(Token *token);
void print_ir();
bool compatible(Token *left, Token *right);
void initialize();
void finalize();
void asm_space(int space);
void pasm(char *fmt, ...);
void free_node(Node *node);
Token *copy_token(Token *token);
Node *copy_node(Node *node);
void create_builtin(char *name, Type *params, Type retType);
bool includes(Type *types, Type type);
Token* generate_ir(Node *node);
Token *new_struct(Token *token);
Token *get_struct(char *name);
Token *get_struct_by_id(size_t id);
void add_attribute(Token *obj, Token *attr);
char *strjoin(char *str0, char *str1, char *str2);
int sizeofToken(Token *token);
Node* add_child(Node *node, Node *child);