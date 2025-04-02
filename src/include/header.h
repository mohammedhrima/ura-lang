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
   int ptr;
   bool declare;
   int space;
   bool remove;
   int reg;
   char *creg;
   int index;
   bool is_cond;
   bool is_ref;
   bool has_ref;
   // int rsp;
   bool isarg;
   int struct_id;
   bool isattr;
   int offset;

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
         int pos;
         int len;
      } Struct;
   };
} Token;

typedef struct Node
{
   struct Node *left;
   struct Node *right;
   Token *token;

   struct Node **children;
   int cpos; // children pos
   int csize; // children size

   struct {
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

} Node;

typedef struct
{
   char *value;
   Type type;
} Specials;

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
extern Node *global;
extern int exe_pos;
extern Inst **OrgInsts;
extern Inst **insts;

extern Node **Gscoop;
extern Node *scoop;
extern int scoopSize;
extern int scoopPos;

extern int ptr;
extern struct _IO_FILE *asm_fd;

void open_file(char *filename);
Token* new_token_(char *filename, int line, char *input, int s, int e, Type type, int space);

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
bool within_space(int space);
void add_token(Token *token);
Node *new_node(Token *token);
Token *find(Type type, ...);
const char *to_string_(const char *filename, const int line, Type type);
void enter_scoop(Node *node);
bool check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(int line, int len, int size);
int debug(char *conv, ...);
int pnode(Node *node, char *side, int space);
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
Token *get_struct_by_id(int id);
void add_attribute(Token *obj, Token *attr);
char *strjoin(char *str0, char *str1, char *str2);
int sizeofToken(Token *token);
Node* add_child(Node *node, Node *child);