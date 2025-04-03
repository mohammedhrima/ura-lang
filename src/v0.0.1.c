#define EXIT_STATUS 0
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
#define BUILTINS 1
#ifndef OPTIMIZE
#define OPTIMIZE 0
#endif

#define ASM 1
#else
#define ASM 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#define TREE 0

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) check_error(FILE, FUNC, LINE, cond, fmt, ##__VA_ARGS__)
#define to_string(type) to_string_(FILE, LINE, type)
// #define new_token(input, s, e, type, space) new_token_(FILE, LINE, input, s, e, type, space)
#define ptoken(token) ptoken_(FILE, LINE, token)

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
Token* new_token(char *input, int s, int e, Type type, int space);

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

// TOKENIZE
Specials *dataTypes = (Specials[]) { {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
   {"char", CHAR}, {"float", FLOAT}, {"void", VOID}, {0, (Type)0}
};

Token* new_token(char *input, int s, int e, Type type, int space)
{
   // debug("new token in %s:%d\n", filename, line);
  // filename = NULL, line = 0;
   debug("new token ");
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   if (type)
      switch (type)
      {
      case INT: while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0'; break;
      case BLOC: case ID: case JMP: case JE: case JNE: case FDEC: case END_BLOC:
      {
         if (e <= s) break;
         new->name = allocate(e - s + 1, sizeof(char));
         strncpy(new->name, input + s, e - s);
         // if (type != ID) break;
         if (strcmp(new->name, "True") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = true;
            break;
         }
         else if (strcmp(new->name, "False") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = false;
            break;
         }
         for (int j = 0; dataTypes[j].value; j++)
         {
            if (strncmp(dataTypes[j].value, new->name, strlen(dataTypes[j].value)) == 0)
            {
               new->type = dataTypes[j].type;
               new->declare = true;
               free(new->name);
               new->name = NULL;
               break;
            }
         }
         break;
      }
      case CHARS:
      {
         if (e <= s) break;
         new->Chars.value = allocate(e - s + 1, sizeof(char));
         strncpy(new->Chars.value, input + s, e - s);
         break;
      }
      case CHAR: if (e > s) new->Char.value = input[s]; break;
      default: check(e > s, "implement adding name for this one %s", to_string(type)); break;
      }
   add_token(new);
   debug("new %k\n", new);
   return new;
}

void add_token(Token *token)
{
   static int pos;
   static int len;
   if (len == 0)
   {
      len = 10;
      tokens = allocate(len, sizeof(Token *));
   }
   else if (pos + 1 == len)
   {
      Token **tmp = allocate(len * 2, sizeof(Token *));
      memcpy(tmp, tokens, len * sizeof(Token *));
      free(tokens);
      tokens = tmp;
      len *= 2;
   }
   tokens[pos++] = token;
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   // TODO: check all values that can be copied example: name ...
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   if (token->creg) new->creg = strdup(token->creg);
   if (token->Struct.attrs)
   {
      new->Struct.attrs = allocate(token->Struct.len, sizeof(Token*));
      for (int i = 0; i < new->Struct.pos; i++) new->Struct.attrs[i] = copy_token(token->Struct.attrs[i]);
   }
   add_token(new);
   return new;
}

// ABSTRACT SYNTAX TREE
Node *new_node(Token *token)
{
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

Token *find(Type type, ...)
{
   if (found_error) return NULL;
   va_list ap;
   va_start(ap, type);
   while (type)
   {
      if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};


bool within_space(int space)
{
   return tokens[exe_pos]->space > space && tokens[exe_pos]->type != END && !found_error;
}

void add_attribute(Token *obj, Token *attr)
{
   if (obj->Struct.attrs == NULL)
   {
      obj->Struct.len = 10;
      obj->Struct.attrs = allocate(obj->Struct.len, sizeof(Token *));
   }
   else if (obj->Struct.pos + 1 == obj->Struct.len)
   {
      Token **tmp = allocate((obj->Struct.len *= 2), sizeof(Token *));
      memcpy(tmp, obj->Struct.attrs, obj->Struct.pos * sizeof(Token *));
      free(obj->Struct.attrs);
      obj->Struct.attrs = tmp;
   }
   attr->space = obj->space + TAB;
   obj->Struct.attrs[obj->Struct.pos++] = attr;
}

// INTERMEDIATE REPRESENTATION
void setName(Token *token, char *name)
{
   if (token->name)
   {
      free(token->name);
      token->name = NULL;
   }
   if (name) token->name = strdup(name);
   else token->name = NULL;
}

void setReg(Token *token, char *creg)
{
   if (token->creg)
   {
      free(token->creg);
      token->creg = NULL;
   }
   if (creg) token->creg = strdup(creg);
   else token->creg = NULL;
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

int sizeofToken(Token *token)
{
   switch (token->type)
   {
   case INT: return sizeof(int);
   case FLOAT: return sizeof(float);
   case CHARS: return sizeof(char *);
   case CHAR: return sizeof(char);
   case BOOL: return sizeof(bool);
   case STRUCT_DEF:
      return token->offset;
   case STRUCT_CALL:
      return token->offset;
   default: check(1, "add this type [%s]\n", to_string(token->type));
   }
   return 0;
}

bool includes(Type *types, Type type)
{
   for (int i = 0; types[i]; i++) if (types[i] == type) return true;
   return false;
}

int ptr;
Inst *new_inst(Token *token)
{
   static int reg;

   Inst *new = allocate(1, sizeof(Inst));
   new->token = token;
   if (token->is_ref && token->ptr)
      // I added this line for this case
      // func int m(ref int a)
      // the code increment rsp and
      // it become for example sub rsp, 16
      // even though I'm not declaring any variable inside
      // function declaration
   {
      token->ptr = (ptr += 8);
   }
   else if (token->name && token->declare)
   {
      if (token->type == STRUCT_CALL)
      {
         debug("found"); exit(1);
      }
      else
      {
         // new_variable(token);
         token->ptr = (ptr += sizeofToken(token));
         // token->reg = ++reg;
      }
   }
   else if (includes((Type[]) {ADD, SUB, MUL, DIV, 0}, token->type))
   {
      // token->reg = ++reg;
   }
   else if (includes((Type[]) {EQUAL, NOT_EQUAL, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0}, token->type))
   {
      // token->reg = ++reg;
   }
   else if (includes((Type[]) {FCALL, 0}, token->type))
   {
   }
   token->reg = ++reg;
   debug("inst: %k\n", new->token);
   add_inst(new);
   return new;
}

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos = -1;

void enter_scoop(Node *node)
{
   debug(CYAN "Enter Scoop: %k index %d\n" RESET, node->token, scoopPos + 1);
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
   debug(CYAN "Exit Scoop: %k index %d\n" RESET, Gscoop[scoopPos]->token, scoopPos);
   Gscoop[scoopPos] = NULL;
   scoopPos--;
   if (scoopPos >= 0) scoop = Gscoop[scoopPos];
}

void clone_insts()
{
   int pos = 0;
   int len = 100;
   free(insts);
   insts = allocate(len, sizeof(Inst *));

   for (int i = 0; OrgInsts[i]; i++)
   {
      if (!OrgInsts[i]->token->remove)
      {
         insts[pos++] = OrgInsts[i];
         if (pos + 1 == len)
         {
            Inst **tmp = allocate((len *= 2), sizeof(Inst *));
            memcpy(tmp, insts, pos * sizeof(Inst *));
            free(insts);
            insts = tmp;
         }
      }
   }
}

/*
get_func m in scoop [F_DEC ] name [main] ret [INT] space [0]
[F_DEC ] name [m] PTR [8] ret [INT] space [0]
   L: [CHILDREN  ] space [0]
   children:
      [CHARS ] name [e] space [4]
children:
   [ASSIGN] ret [CHARS] space [4]
      L: [CHARS ] name [t] [declare] PTR [8] space [4]
      R: [ID    ] name [e] space [4]
   [RETURN] space [4]
      L: [INT   ] value [1] space [4]
*/
Node **builtins_functions;
int builtins_pos;
int builtins_size;
void create_builtin(char *name, Type *params, Type retType)
{
   if (found_error) return;
   Node *node = new_node(new_token(name, 0, strlen(name), FDEC, 0));
   node->token->retType = retType;
   switch (retType)
   {
   case INT:   setReg(node->token, "eax"); break;
   case CHAR:  setReg(node->token, "al"); break;
   case CHARS: setReg(node->token, "rax"); break;
   case VOID:  setReg(node->token, "rax"); break;
   default: check(1, "handle this case %s", to_string(retType)) ;
   }
   node->left = new_node(new_token(0, 0, 0, CHILDREN, node->token->space));
   Node *args = node->left;
   int i = 0;
   char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
   char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
   while (params[i] && !found_error)
   {

      Token *arg_token = new_token(NULL, 0, 0, params[i], node->token->space + TAB);
      // arg_token->declare = true;
      if (i < (int)(sizeof(eregs) / sizeof(eregs[0])))
      {
         // if (name->is_ref) setReg(name, rregs[i]);
         // else
         {
            // check(1, "set reg");
            // TODO: add other data type and math operations
            switch (arg_token->type)
            {
            case CHARS: setReg(arg_token, rregs[i]); break;
            case INT:   setReg(arg_token, eregs[i]); break;
            case CHAR:  setReg(arg_token, eregs[i]); break;
            case FLOAT: setReg(arg_token, rregs[i]); break; // TODO: to be checked
            case BOOL:  setReg(arg_token, eregs[i]); break;
            default: check(1, "set reg for %s", to_string(arg_token->type));
            };
         }
      }
      else
      {
         // TODO:
         check(1, "implement assigning function argument using PTR");
      }
      add_child(args, new_node(arg_token));
      i++;
   }

   new_function(node);
}

Node *new_function(Node *node)
{
   debug("new_func %s in scoop %k that return %t\n", node->token->name, scoop->token, node->token->retType);
   for (int i = 0; i < scoop->fpos; i++)
   {
      Node *func = scoop->functions[i];
      bool cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s\n", node->token->name);
   }
   if (scoop->functions == NULL)
   {
      scoop->fsize = 10;
      scoop->functions = allocate(scoop->fsize, sizeof(Node *));
   }
   else if (scoop->fpos + 1 == scoop->fsize)
   {
      scoop->fsize *= 2;
      Node **tmp = allocate(scoop->fsize, sizeof(Node *));
      memcpy(tmp, scoop->functions, scoop->fpos * sizeof(Node *));
      free(scoop->functions);
      scoop->functions = tmp;
   }
   scoop->functions[scoop->fpos++] = node;
   return node;
}

Node *get_function(char *name)
{
   // TODO: remove output from here
   debug("get_func %s in scoop %k\n", name, scoop->token);
#if 1
   for (int i = 0; i < builtins_pos; i++)
      if (strcmp(name, builtins_functions[i]->token->name) == 0) return builtins_functions[i];
#endif
   for (int j = scoopPos; j >= 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->fpos; i++)
         if (strcmp(scoop->functions[i]->token->name, name) == 0) return scoop->functions[i];
   }
   check(1, "'%s' Not found\n", name);
   return NULL;
}

Token *new_variable(Token *token)
{
   debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET, token->name, to_string(token->type), scoop->token);
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      if (strcmp(curr->name, token->name) == 0) check(1, "Redefinition of %s\n", token->name);
   }
   if (scoop->vars == NULL)
   {
      scoop->vsize = 10;
      scoop->vars = allocate(scoop->vsize, sizeof(Token *));
   }
   else if (scoop->vpos + 1 == scoop->vsize)
   {
      Token **tmp = allocate(scoop->vsize *= 2, sizeof(Token *));
      memcpy(tmp, scoop->vars, scoop->vpos * sizeof(Token *));
      free(scoop->vars);
      scoop->vars = tmp;
   }
   scoop->vars[scoop->vpos++] = token;
   return token;
}

Token *get_variable(char *name)
{
   debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
   for (int j = scoopPos; j >= 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->vars[i]->name, name) == 0) return scoop->vars[i];
   }
   check(1, "%s not found\n", name);
   return NULL;
}

Token *new_struct(Token *token)
{
   static int structs_ids;
   if (!token->struct_id) token->struct_id = (structs_ids += 1);
   // check(1, "implement this one");
   debug(CYAN "new struct [%s] with id [%d] offset [%d] ", token->name, token->struct_id, token->offset);
   if (scoop && scoop->token) debug("in scoop %k", scoop->token);
   else if (scoop && !scoop->token) debug("in scoop with NULL token");
   else debug("in NULL scoop");
   debug("\n" RESET);
   for (int i = 0; i < scoop->spos; i++)
   {
      Token *curr = scoop->structs[i];
      if (strcmp(curr->name, token->name) == 0) check(1, "Redefinition of %s\n", token->name);
   }
   if (scoop->structs == NULL)
   {
      scoop->ssize = 10;
      scoop->structs = allocate(scoop->ssize, sizeof(Token *));
   }
   else if (scoop->spos + 1 == scoop->ssize)
   {
      Token **tmp = allocate(scoop->ssize *= 2, sizeof(Token *));
      memcpy(tmp, scoop->structs, scoop->spos * sizeof(Token *));
      free(scoop->structs);
      scoop->structs = tmp;
   }
   scoop->structs[scoop->spos++] = token;
   return token;
}

Token *get_struct(char *name)
{
   debug(CYAN "get struct [%s] ", name);
   if (scoop && scoop->token) debug("from scoop %k", scoop->token);
   else if (scoop && !scoop->token) debug("from scoop with NULL token");
   else debug("from NULL scoop");
   debug("\n" RESET);

   for (int j = scoopPos; j >= 0; j--)
   {
      Node *node = Gscoop[j];
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->name, name) == 0) return node->structs[i];
   }
   // check(1, "%s not found\n", name);
   return NULL;
}

Token *get_struct_by_id(int id)
{
   debug(CYAN "get struct with id [%d] from scoop %k\n", id, scoop->token);
   for (int j = scoopPos; j >= 0; j--)
   {
      Node *node = Gscoop[j];
      debug("[%d] scoop [%s] has %d structs\n", j, scoop->token->name, node->spos);
      for (int i = 0; i < node->spos; i++)
      {
         debug(GREEN"struct has [%d]\n"RESET, node->structs[i]->struct_id);
         if (node->structs[i]->struct_id == id) return node->structs[i];
      }
   }
   // check(1, "%s not found\n", name);
   return NULL;
}


// TODO: implement it
bool compatible(Token *left, Token *right)
{
   Type ltype = left->type;
   Type lrtype = left->retType;
   Type rtype = right->type;
   Type rrtype = right->retType;
   if (ltype == CHARS && (rtype == VOID || rrtype == VOID)) return true;
   return (ltype == rtype || ltype == rrtype || lrtype == rtype || lrtype == rrtype);
}

// UTILS
const char *to_string_(const char *filename, const int line, Type type) {
   const char *arr[] = {
      [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD ASSIGN", [SUB_ASSIGN] = "SUB ASSIGN",
      [MUL_ASSIGN] = "MUL ASSIGN", [DIV_ASSIGN] = "DIV ASSIGN", [MOD_ASSIGN] = "MOD_ASSIGN",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT EQUAL", [LESS_EQUAL] = "LESS THAN OR EQUAL",
      [MORE_EQUAL] = "MORE THAN OR EQUAL", [LESS] = "LESS THAN", [MORE] = "MORE THAN",
      [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [MOD] = "MOD",
      [AND] = "AND", [OR]  = "OR", [RPAR] = "R_PAR", [LPAR] = "L_PAR", [COMA] = "COMMA",
      [DOTS] = "DOTS", [DOT] = "DOT", [RETURN] = "RETURN", [IF] = "IF", [ELIF] = "ELIF",
      [ELSE] = "ELSE", [WHILE] = "HILE", [CONTINUE] = "continue", [BREAK] = "break", [REF] = "REF",
      [FDEC] = "F_DEC", [FCALL] = "F_CALL", [INT] = "INT", [VOID] = "VOID", [CHARS] = "CHARS",
      [CHAR] = "CHAR", [BOOL] = "BOOL", [FLOAT] = "FLOAT", [STRUCT_CALL] = "STRUCT CALL",
      [STRUCT_DEF] = "STRUCT DEF", [ID] = "ID", [END_BLOC] = "END_BLOC", [BLOC] = "BLOC",
      [JNE] = "JNE", [JE] = "JE", [JMP] = "JMP", [LBRA] = "L_BRA", [RBRA] = "R_BRA",
      [END] = "END", [CHILDREN] = "CHILDREN",
   };
   if (type >= 1 && type < sizeof(arr) / sizeof(arr[0]) && arr[type] != NULL) return arr[type];
   check(1, "Unknown type [%d] in %s:%d\n", type, filename, line);
   return NULL;
}

void *allocate_func(int line, int len, int size)
{
   void *ptr = calloc(len, size);
   check(!ptr, "allocate did failed in line %d\n", line);
   return ptr;
}

void open_file(char *filename)
{
   if (found_error) return;
   struct _IO_FILE *file = fopen(filename, "r");
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
}

bool check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   found_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "%sError:%s:%s:%d %s", RED, filename, funcname, line, RESET);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   return cond;
}

// ASSEMBLY
void pasm(char *fmt, ...)
{
   did_pasm = true;
   int i = 0;
   va_list args;
   va_start(args, fmt);

   while (fmt[i])
   {
      if (fmt[i] == '%')
      {
         i++;
         if (fmt[i] == 'i')
         {
            i++;
            fprintf(asm_fd, "%-4s ", va_arg(args, char *));
         }
         else if (strncmp(fmt + i, "ra", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "%s", token->creg); // TODO: those lines are bad
            else
            {
               Type type = token->retType ? token->retType : token->type;
               switch (type)
               {
               case CHARS: fputs("rax", asm_fd); break;
               case INT: fputs("eax", asm_fd); break;
               case BOOL: case CHAR: fputs("al", asm_fd); break;
               case FLOAT: fputs("xmm0", asm_fd); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
            }
         }
         else if (strncmp(fmt + i, "rb", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "%s", token->creg);
            else
            {
               Type type = token->retType ? token->retType : token->type;
               switch (type)
               {
               case CHARS: fputs("rbx", asm_fd); break;
               case INT: fputs("ebx", asm_fd); break;
               case BOOL: case CHAR: fputs("bl", asm_fd); break;
               case FLOAT: fputs("xmm1", asm_fd); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
            }
         }
         else if (strncmp(fmt + i, "rd", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "%s", token->creg);
            else
            {
               Type type = token->retType ? token->retType : token->type;
               switch (type)
               {
               case CHARS: fputs("edx", asm_fd); break;
               case INT: fputs("edx", asm_fd); break;
               case BOOL: case CHAR: fputs("dl", asm_fd); break;
               case FLOAT: fputs("xmm2", asm_fd); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
            }
         }
         // else if (fmt[i] == 'r')
         // {
         //    i++;
         //    Token *token = va_arg(args, Token *);
         //    if (token->creg)
         //       fprintf(asm_fd, "%s", token->creg);
         //    else
         //    {
         //       Type type = token->retType ? token->retType : token->type;
         //       switch (type)
         //       {
         //       case CHARS: fputs("rax", asm_fd); break;
         //       case INT: fputs("eax", asm_fd); break;
         //       case BOOL: case CHAR: fputs("al", asm_fd); break;
         //       case FLOAT: fputs("xmm0", asm_fd); break;
         //       default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
         //       }
         //    }
         // }
         else if (fmt[i] == 'a')
         {
            i++;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "%s", token->creg);
            else if (token->is_ref) fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr);
            else
               switch (token->type)
               {
               case CHARS: fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr); break;
               case INT: fprintf(asm_fd, "DWORD PTR -%d[rbp]", token->ptr); break;
               case CHAR: fprintf(asm_fd, "BYTE PTR -%d[rbp]", token->ptr); break;
               case BOOL: fprintf(asm_fd, "BYTE PTR -%d[rbp]", token->ptr); break;
               case FLOAT: fprintf(asm_fd, "DWORD PTR -%d[rbp]", token->ptr); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
         }
         else if (strncmp(fmt + i, "ma", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            switch (token->type)
            {
            case CHARS: fprintf(asm_fd, "QWORD PTR [rax]"); break;
            case INT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
            case CHAR: fprintf(asm_fd, "BYTE PTR [rax]"); break;
            case BOOL: fprintf(asm_fd, "BYTE PTR [rax]"); break;
            case FLOAT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         else if (strncmp(fmt + i, "mb", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            switch (token->type)
            {
            case CHARS: fprintf(asm_fd, "QWORD PTR [rbx]"); break;
            case INT: fprintf(asm_fd, "DWORD PTR [rbx]"); break;
            case CHAR: fprintf(asm_fd, "BYTE PTR [rbx]"); break;
            case BOOL: fprintf(asm_fd, "BYTE PTR [rbx]"); break;
            case FLOAT: fprintf(asm_fd, "DWORD PTR [rbx]"); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         // else if (fmt[i] == 'm')
         // {
         //    i++;
         //    Token *token = va_arg(args, Token *);
         //    switch (token->type)
         //    {
         //    case CHARS: fprintf(asm_fd, "QWORD PTR [rax]"); break;
         //    case INT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
         //    case CHAR: fprintf(asm_fd, "BYTE PTR [rax]"); break;
         //    case BOOL: fprintf(asm_fd, "BYTE PTR [rax]"); break;
         //    case FLOAT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
         //    default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
         //    }
         // }
         else if (fmt[i] == 'v')
         {
            i++;
            Token *token = va_arg(args, Token *);
            switch (token->type)
            {
            case INT: fprintf(asm_fd, "%lld", token->Int.value); break;
            case BOOL: fprintf(asm_fd, "%d", token->Bool.value); break;
            case CHAR: fprintf(asm_fd, "%d", token->Char.value); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         else
         {
            int handled = 0;
#define check_format(string, type)                     \
               do                                                   \
               {                                                    \
                  if (strncmp(fmt + i, string, strlen(string)) == 0) \
                  {                                                  \
                     handled = 1;                                     \
                     i += strlen(string);                             \
                     fprintf(asm_fd, "%" string, va_arg(args, type)); \
                  }                                                  \
               } while (0)
            check_format("d", int);
            check_format("ld", long);
            check_format("s", char *);
            check_format("zu", unsigned long);
            check_format("f", double);
            check(!handled, "handle this case [%s]\n", fmt + i);
         }
      }
      else
      {
         fputc(fmt[i], asm_fd);
         i++;
      }
   }
   va_end(args);
}

void initialize()
{
   pasm(".intel_syntax noprefix\n");
   pasm(".include \"/import/header.s\"\n");
   pasm(".text\n");
   pasm(".globl	main\n");
}

void finalize()
{
#if TOKENIZE
   for (int i = 0; tokens[i]; i++)
   {
      Token *curr = tokens[i];
      // test char variable before making any modification
      if (curr->type == CHARS && !curr->name && !curr->ptr && curr->index)
         pasm(".STR%d: .string %s\n", curr->index, curr->Chars.value ? curr->Chars.value : "\"\"");
      if (curr->type == FLOAT && !curr->name && !curr->ptr && curr->index)
         pasm(".FLT%d: .long %d /* %f */\n", curr->index, *((uint32_t *)(&curr->Float.value)), curr->Float.value);
   }
   pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
#endif
}

bool did_pasm;
void asm_space(int space)
{
   if (did_pasm)
   {
      pasm("\n");
      for (int i = 0; i < space; i++) pasm(" ");
      did_pasm = false;
   }
}

// LOGGING
void print_value(Token *token)
{
   switch (token->type)
   {  // TODO: handle the other cases
   case INT: debug("%lld", token->Int.value); break;
   case BOOL: debug("%s", token->Bool.value ? "True" : "False"); break;
   case FLOAT: debug("%f", token->Float.value); break;
   case CHAR: debug("%c", token->Char.value); break;
   case CHARS: debug("%s", token->Chars.value); break;
   default: check(1, "handle this case [%s]\n", to_string(token->type)); break;
   }
}
void print_ir()
{
   if (!DEBUG) return;
   clone_insts();
   debug(GREEN "==========   PRINT IR  =========\n" RESET);
   int i = 0;
   for (i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      curr->reg ? debug("r%.2d:", curr->reg) : debug("rxx:");
      int k = 0;
      while (k < curr->space) k += printf(" ");
      switch (curr->type)
      {
      case ADD_ASSIGN:
      case ASSIGN:
      {
         // debug("%s\n", right->reg ? "has reg" : "no reg");
         debug("[%-6s] ", to_string(curr->type));
         if (left->creg) debug("r%.2d (%s) = ", left->reg, left->creg);
         else debug("r%.2d (%s) = ", left->reg, left->name);

         if (right->reg) debug("r%.2d (%s)", right->reg, right->name ? right->name : "");
         else if (right->creg) debug("[%s]", right->creg);
         else print_value(right);
         break;
      }
      case ADD: case SUB: case MUL: case DIV:
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
      {
         if (!left || !right)
         {
            if (!left) check(1, " left NULL\n");
            if (!right) check(1, " right NULL\n");
            exit(1);
         }
         debug("[%-6s] ", to_string(curr->type));
         if (left->reg) debug("r%.2d", left->reg);
         else if (left->creg)
         {
            check(1, "handle this case");
         }
         else print_value(left);
         if (left->name) debug(" (%s)", left->name);
         debug(" to ");
         if (right->reg) debug("r%.2d", right->reg);
         else print_value(right);
         if (right->name) debug(" (%s)", right->name);
         break;
      }
      case INT: case BOOL: case CHARS: case CHAR:
      {
         debug("[%-6s] ", to_string(curr->type));
         if (curr->declare) debug("declare [%s] PTR=[%d]", curr->name, curr->ptr);
         else if (curr->name) debug("variable %s ", curr->name);
         else if (curr->creg) debug("creg %s ", curr->creg);
         else if (curr->type == INT) debug("value %lld ", curr->Int.value);
         else if (curr->type == BOOL) debug("value %s ", curr->Bool.value ? "True" : "False");
         else if (curr->type == CHAR) debug("value %c ", curr->Char.value);
         // else if(curr->type == BOOL) debug("value %s ", curr->Bool.value ? "True" : "False");
         // else if(curr->type == FLOAT)
         // {
         //     curr->index = ++float_index;
         //     debug("value %f ", curr->Float.value);
         // }
         else if (curr->type == CHARS)
         {
            if (curr->index) debug("value %s in STR%d", curr->Chars.value, curr->index);
            else debug("in %s", curr->creg);
         }
         else check(1, "handle this case in generate ir\n", "");
         break;
      }
      case DOT:
      {
         debug("[%-6s] ", to_string(curr->type));
         debug("access [%s] in %k", right->name, left);
         break;
      }
      case JMP: debug("jmp to [%s]", curr->name); break;
      case JNE: debug("jne to [%s]", curr->name); break;
      case FCALL: debug("call [%s]", curr->name); break;
      case BLOC: case FDEC: debug("[%s] bloc", curr->name); break;
      case END_BLOC: debug("[%s] endbloc", curr->name); break;
      case RETURN: case CONTINUE: case BREAK: debug("[%s]", to_string(curr->type)); break;
      default: debug(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
      }
      // if(curr->remove) debug(" remove");
      if (curr->is_ref) debug(" is_ref");
      debug(" space (%d)", curr->space);
      debug("\n");
   }
   debug("total instructions [%d]\n", i);
}

int ptoken_(const char*filename, int line, Token *token)
{
   int res = 0;
   if (!token) return debug("null token");
   res += debug("[%-6s] ", to_string_(filename, line, token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:// case STRUCT_CALL:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      if (token->declare) res += debug("[declare] ");
      if (!token->name && !token->declare)
      {
         if (token->creg) res += debug("creg [%s] ", token->creg);
         else
            switch (token->type)
            {
            case INT: res += debug("value [%lld] ", token->Int.value); break;
            case CHARS: res += debug("value [%s] ", token->Chars.value); break;
            case CHAR: res += debug("value [%c] ", token->Char.value); break;
            case BOOL: res += debug("value [%d] ", token->Bool.value); break;
            case FLOAT: res += debug("value [%f] ", token->Float.value); break;
            default: break;
            }
      }
      break;
   }
   case STRUCT_CALL:
   case STRUCT_DEF:
   {
      res += debug("name [%s] ", token->name);
      res += debug("attributes: ");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
#if 0
         res += ptoken(attr) + debug(", ");
#else
         res += debug("%s %t [%d], ", attr->name, attr->type, attr->ptr);
#endif
      }
      break;
   }
   case FCALL:
   case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   //if (token->ptr)
   res += debug("PTR [%d] ", token->ptr);
   if (token->remove) res += debug("[remove] ");
   if (token->retType) res += debug("ret [%t] ", token->retType);
   if (token->struct_id) res += debug("struct_id [%d] ", token->struct_id);
   res += debug("space [%d] ", token->space);
   return res;
}

int pnode(Node *node, char *side, int space)
{
   if (!node) return 0;

   int res = 0;
   for (int i = 0; i < space; i++) res += debug(" ");
   if (side) res += debug("%s", side);
   res += debug("%k\n", node->token);


   res += pnode(node->left, "L: ", space + TAB);
   res += pnode(node->right, "R: ", space + TAB);
   if (node->children)
   {
      for (int i = 0; i < space; i++) res += debug(" ");
      res += debug("children: \n");
      //for (int i = 0; i < space; i++) res += debug(" ");
      for (int i = 0; i < node->cpos; i++) pnode(node->children[i], NULL, space + TAB);
   }

   return res;
}

int debug(char *conv, ...)
{
   if (!DEBUG) return 0;
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
         while (conv[i] >= '0' && conv[i] <= '9') { width = width * 10 + (conv[i] - '0'); i++; }
         int precision = -1;
         if (conv[i] == '.')
         {
            i++;
            precision = 0;
            while (conv[i] >= '0' && conv[i] <= '9')
            {
               precision = precision * 10 + (conv[i] - '0');
               i++;
            }
         }
         if (strncmp(conv + i, "zu", 2) == 0) {res += fprintf(stdout, "%d", va_arg(args, int)); i++; }
         else if (strncmp(conv + i, "lld", 3) == 0) { res += fprintf(stdout, "%lld", va_arg(args, long long)); i += 2; }
         else
         {
            switch (conv[i])
            {
            case 'c': res += fprintf(stdout, "%c", va_arg(args, int)); break;
            case 's':
            {
               char *str = va_arg(args, char *);
               if (left_align)
               {
                  if (precision >= 0) res += fprintf(stdout, "%-*.*s", width, precision, str);
                  else res += fprintf(stdout, "%-*s", width, str);
               }
               else
               {
                  if (precision >= 0) res += fprintf(stdout, "%*.*s", width, precision, str);
                  else res += fprintf(stdout, "%*s", width, str);
               }
               break;
            }
            case 'p': res += fprintf(stdout, "%p", (void *)(va_arg(args, void *))); break;
            case 'x':
               if (precision >= 0) res += fprintf(stdout, "%.*x", precision, va_arg(args, unsigned int));
               else  res += fprintf(stdout, "%x", va_arg(args, unsigned int));
               break;
            case 'X':
               if (precision >= 0) res += fprintf(stdout, "%.*X", precision, va_arg(args, unsigned int));
               else res += fprintf(stdout, "%X", va_arg(args, unsigned int));
               break;
            case 'd':
               if (precision >= 0) res += fprintf(stdout, "%.*d", precision, va_arg(args, int));
               else res += fprintf(stdout, "%d", va_arg(args, int));
               break;
            case 'f':
               if (precision >= 0) res += fprintf(stdout, "%.*f", precision, va_arg(args, double));
               else res += fprintf(stdout, "%f", va_arg(args, double));
               break;
            case '%': res += fprintf(stdout, "%%"); break;
            case 't': res += fprintf(stdout, "%s", to_string((Type)va_arg(args, Type))); break;
            case 'k':
            {
               Token *token = va_arg(args, Token *);
               res += token ? ptoken(token) : fprintf(stdout, "(null)");
               break;
            }
            case 'n':
            {
               Node *node = (Node *)va_arg(args, Node *);
               res += node ? pnode(node, NULL, node->token->space) : fprintf(stdout, "(null)");
               break;
            }
            default: check(1, "invalid format specifier [%c]\n", conv[i]); exit(1); break;
            }
         }
      }
      else res += fprintf(stdout, "%c", conv[i]);
   }
   va_end(args);
   return res;
}
// CLEAR MEMORY
void free_token(Token *token)
{
   free(token->name);
   free(token->creg);
   free(token->Chars.value);
   free(token->Struct.attrs);
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
   free(input);
   for (int i = 0; tokens && tokens[i]; i++) free_token(tokens[i]);
   free_node(head);
}

char *strjoin(char *str0, char *str1, char *str2)
{
   int len0 = strlen(str0);
   int len1 = strlen(str1);
   int len2 = strlen(str2);

   char *res = allocate(len0 + len1 + len2 + 1, 1);
   strcpy(res, str0);
   strcpy(res + len0, str1);
   strcpy(res + len0 + len1, str2);
   return res;
}

// GLOBALS
bool found_error = false;
Inst **OrgInsts;
Inst **insts;
Token **tokens;
Node *head;
int exe_pos;
char *input;

// TOKENIZE
void tokenize()
{
   if (found_error) return;
   debug(GREEN BOLD"TOKENIZE:\n" RESET);
   Specials *specials = (Specials[]) {
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"is", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR}, {"[", LBRA}, {"]", RBRA},
      {",", COMA}, {"if", IF}, {"elif", ELIF}, {"else", ELSE},
      {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
      {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF},
      {"and", AND}, {"&&", AND}, {"or", OR}, {"||", OR}, {"struct", STRUCT_DEF},
      {0, (Type)0}
   };

   int space = 0;
   bool inc_space = true;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            inc_space = true;
            space = 0;
         }
         else if (inc_space)
         {
            if (input[i] == '\t') space += TAB;
            else space++;
         }
         i++;
         continue;
      }
      // TODO: handle new lines inside comment
      else if (strncmp(input + i, "/*", 2) == 0)
      {
         i += 2;
         while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2)) i++;
         check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'\n");
         i += 2;
         continue;
      }
      else if (strncmp(input + i, "//", 2) == 0)
      {
         while (input[i] && input[i] != '\n') i++;
         continue;
      }
      inc_space = false;
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
         {
            new_token(NULL, 0, 0, specials[j].type, space);
            found = true;
            i += strlen(specials[j].value);
            if (strcmp(specials[j].value, ":") == 0) space += TAB;
            break;
         }
      }
      if (found) continue;
      if (input[i] && input[i] == '\"')
      {
         i++;
         while (input[i] && input[i] != '\"') i++;
         check(input[i] != '\"', "Expected '\"'");
         i++;
         new_token(input, s, i, CHARS, space);
         continue;
      }
      if (input[i] && input[i] == '\'')
      {
         i++;
         if (input[i] && input[i] != '\'') i++;
         check(input[i] != '\'', "Expected '\''");
         i++;
         new_token(input, s + 1, i, CHAR, space);
         continue;
      }
      if (isalpha(input[i]))
      {
         while (isalnum(input[i])) i++;
         new_token(input, s, i, ID, space);
         continue;
      }
      if (isdigit(input[i]))
      {
         while (isdigit(input[i])) i++;
         new_token(input, s, i, INT, space);
         continue;
      }
      check(input[i], "Syntax error <%c>\n", input[i]);
   }
   new_token(input, 0, 0, END, -1);
}

// ABSTRACT SYNTAX TREE
Type getRetType(Node *node)
{
   if (!node || !node->token) return 0;
   if (includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0},
node->token->type)) return node->token->type;
   if (includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0},
node->token->retType)) return node->token->retType;
   Type left = 0, right = 0;
   if (node->left) left = getRetType(node->left);
   if (node->right) right = getRetType(node->right);
   if (left) return left;
   if (right) return right;
   return 0;
}

Node *expr()
{
   return assign();
}

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = logic();
      node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *logic()
{
   Node *left = equality();
   Token *token;
   while ((token = find(AND, OR, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = equality();
      left = node;
   }
   return left;
}

// TODO: handle NOT operator
Node *equality()
{
   Node *left = comparison();
   Token *token;
   while ((token = find(EQUAL, NOT_EQUAL, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = comparison();
      left = node;
   }
   return left;
}

Node *comparison()
{
   Node *left = add_sub();
   Token *token;
   while ((token = find(LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = add_sub();
      left = node;
   }
   return left;
}


Node *add_sub()
{
   Node *left = mul_div();
   Token *token;
   while ((token = find(ADD, SUB, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = mul_div();
      node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *mul_div()
{
   Node *left = dot();
   Token *token;
   while ((token = find(MUL, DIV, 0))) // TODO: handle modulo
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = dot();
      // node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *dot()
{
   Node *left = brackets();
   Token *token;
   while ((token = find(DOT, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      token = find(ID, 0);
      if (check(!token, "error, expected id after dot")) exit(1);
      node->right = new_node(token);
      node->token->retType = getRetType(node);
      // TODO: check righ should be an identifier
      node->right->token->isattr = true;
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

Node *sign()
{
   return prime();
}

Node *func_dec(Node *node)
{
   /*
   function declaration:
      LEFT : array of argument
      array of childen
   */
   Token *typeName = find(INT, CHARS, CHAR, FLOAT, BOOL, VOID, 0);
   Token *fname = find(ID, 0);
   check(!typeName || !fname, "expected data type and identifier after func declaration");

   node->token->retType = typeName->type;
   node->token->name = fname->name;
   fname->name = NULL;

   switch (node->token->retType)
   {
   case INT:   setReg(node->token, "eax"); break;
   case CHAR:  setReg(node->token, "al"); break;
   case CHARS: setReg(node->token, "rax"); break;
   case VOID:  setReg(node->token, "rax"); break;
   default: check(1, "handle this case %s", to_string(node->token->retType)) ;
   }

   enter_scoop(node);
   char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
   char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
   int i = 0;
   Token *arg = NULL;

   check(!find(LPAR, 0), "expected ( after function declaration");

   node->left = new_node(new_token(0, 0, 0, CHILDREN, node->token->space));
   Node *args = node->left;
   Token *last;
   while (!found_error && !(last = find(RPAR, END, 0)))
   {
      bool is_ref = find(REF, 0) != NULL;
      arg = find(INT, CHARS, CHAR, FLOAT, BOOL, 0);
      if (check(!arg, "expected data type in function argument")) break;
      Token *name = find(ID, 0);
      if (check(!name, "expected identifier in function argument")) break;
      name->is_ref = is_ref;
      name->type = arg->type;
      if (i < (int)(sizeof(eregs) / sizeof(eregs[0])))
      {
         if (name->is_ref) setReg(name, rregs[i]);
         else
         {
            // TODO: add other data type and math operations
            switch (name->type)
            {
            case CHARS: setReg(name, rregs[i]); break;
            case INT:   setReg(name, eregs[i]); break;
            case CHAR:  setReg(name, eregs[i]); break;
            case FLOAT: setReg(name, rregs[i]); break; // TODO: to be checked
            case BOOL:  setReg(name, eregs[i]); break;
            default: check(1, "set reg for %s", to_string(name->type));
            };
            i++;
         }
      }
      else
      {
         // TODO:
         check(1, "implement assigning function argument using PTR");
      }
      add_child(args, new_node(name));
      find(COMA, 0); // TODO: check this later
   }
   check((!found_error && last->type != RPAR), "expected ) after function declaration");
   check((!found_error && !find(DOTS, 0)), "Expected : after function declaration");

   for (int i = 0; i < args->cpos; i++)
   {
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space + TAB));
      Node *left = copy_node(args->children[i]);
      left->token->declare = true;
      Node *right = copy_node(args->children[i]);
      if (right->token->is_ref)
      {
         right->token->has_ref = true;
         right->token->ptr = 0;
      }
      setName(right->token, NULL);
      setReg(left->token, NULL);
      left->token->ptr += (ptr += sizeofToken(left->token));
      new_variable(left->token);
      assign->left = left;
      assign->right = right;
      add_child(node, assign);
   }
   Node *child = NULL;
   while (within_space(node->token->space))
   {
      child = expr();
      add_child(node, child);
   }
   // TODO: expect return only for none VOID functions
   if (node->token->retType != VOID)
      check(!child || child->token->type != RETURN, "expected return statment");
   else
   {
      Node *ret = new_node(new_token(0, 0, 0, RETURN, node->token->space + TAB));
      ret->left = new_node(new_token(0, 0, 0, INT, node->token->space + TAB));
      add_child(node, ret);
   }
   exit_scoop();
   return node;
}

// TODO: to be used when settign struct as function parameter
void add_param()
{

}

Type get_attr_type(Node *src, Node *attr)
{
   debug("access %s in %s \n", attr->token->name, src->token->name);
   Type src_type = 0;
   if (src->token->type == DOT) src_type = get_attr_type(src->left, src->right);
   else if (src->token->type == ID)
   {
      Token *var = get_variable(src->token->name);
      if (!var) return 0;
      // TODO: check if it's a struct call
      for (int i = 0; i < var->Struct.pos; i++)
      {
         Token *curr = var->Struct.attrs[i];
         if (strcmp(curr->name,  attr->token->name) == 0)
         {
            // debug("found has type %s\n", to_string(curr->type));
            return curr->type;
         }
      }
      // TODO: if not found return some error
   }

   return src->token->type;
}

Node *func_call(Node *node)
{
   node->token->type = FCALL;
   Token *arg = NULL;
   Token *token = node->token;

   char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
   char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
   int i = 0;
   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      // TODO: this approach need to be modified
      // doesn't work well if function takes references
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space));
      assign->right = expr();
      assign->right->token->space = token->space;
      assign->left = new_node(NULL);
      if (assign->right->token->type == ID)
      {
         Token *var = get_variable(assign->right->token->name);
         if (var)
         {
            assign->left->token = copy_token(var);
            setName(assign->left->token, NULL);
            setReg(assign->left->token, NULL);
            assign->left->token->space = token->space;
            assign->left->token->declare = false;
         }
         else check(1, "variable no found %s\n", assign->right->token->name);
      }
      else
      {
         assign->left->token = copy_token(assign->right->token);
         assign->left->token->space = token->space;
         // keep it commented it causes isue here
         // putstr(user.name)
         //assign->left->token->type = getRetType(assign->right);
      }
      // check(assign->left->token->type == 0, "found another stupid error in function call");
      if (eregs[i])
      {
         if (!found_error)
         {
            Type type = assign->left->token->type;
            if (type == DOT)
            {
               type =  get_attr_type(assign->right->left, assign->right->right);
               assign->left->token->type = type;
            }

            Type retType = assign->left->token->retType;
            switch (type)
            {
            case CHARS: setReg(assign->left->token, rregs[i]); break;
            case ADD: case SUB: case MUL: case DIV:
            case INT: setReg(assign->left->token, eregs[i]); break;
            case DOT:
            {
               debug("found DOT");
               pnode(assign->right, NULL, 0);

               //debug("struct id: %d", assign->right->token->struct_id);
               exit(1);
               break;
            }
            default:
            {
               pnode(assign->left, NULL, 0);
               check(1, "handle this case [%s] [%s]", to_string(type), to_string(retType));
               exit(1);
            }
            }
         }
         i++;
      }
      else
      {
         check(1, "implemnt PTR");
      }
      add_child(node, assign);
      assign->token->space = node->token->space;
      find(COMA, 0);
   }
   check(!found_error && arg->type != RPAR, "expected ) after function call");
   return node;
}

Node *func_main(Node *node)
{
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
      last = new_node(new_token(NULL, 0, 0, RETURN, node->token->space + TAB));
      last->left = new_node(new_token(NULL, 0, 0, INT, node->token->space + TAB));
      add_child(node, last);
   }
   exit_scoop();
   return node;
}

Node *if_node(Node *node)
{
   enter_scoop(node);
   debug(GREEN "Enter if\n" RESET);
   /*
   if:
      left: condition
      children: code bloc
      right:
         children: elif, else
   */

   node->left = expr();  // condition
   node->left->token->is_cond = true;
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(NULL, 0, 0, CHILDREN, node->token->space));

   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());
   while (!found_error && includes((Type[]) {ELSE, ELIF, 0}, tokens[exe_pos]->type) &&
node->token->space == tokens[exe_pos]->space)
   {
      Token *token = find(ELSE, ELIF, 0);
      Node *curr = add_child(node->right, new_node(token));
      token->space -= TAB;
      if (token->type == ELIF)
      {
         // debug(GREEN "found elif\n" RESET);
         curr->left = expr();
         curr->left->token->is_cond = true;
         //curr->left->token->space = node->token->space;
         check(!find(DOTS, 0), "expected : after elif condition");
         // debug("within %k, current %k\n", token, tokens[exe_pos]);
         while (within_space(token->space))
         {
            // debug("add %k\n", tokens[exe_pos]);
            add_child(curr, expr());
         }
         // debug("current is %k\n", tokens[exe_pos]);
         // exit(1);
      }
      else if (token->type == ELSE)
      {
         // debug(GREEN "found else\n" RESET);
         check(!find(DOTS, 0), "expected : after else");
         while (within_space(token->space)) add_child(curr, expr());
         break;
      }
   }
   exit_scoop();
   //exit(1);
   return node;
}

Node *while_node(Node *node)
{
   /*
   while:
      left: condition
      children: code bloc
   */
   enter_scoop(node);
   node->left = expr();
   node->left->token->is_cond = true;
   node->left->token->space = node->token->space;

   check(!find(DOTS, 0), "Expected : after while condition\n", "");

   while (within_space(node->token->space))
   {
      Token *token = find(CONTINUE, BREAK, 0);
      Node *child = NULL;
      if (token) child = new_node(token);
      else child = expr();
      add_child(node, child);
   }
   exit_scoop();
   return node;
}

Token *is_struct(Token *token)
{
   Token *res = get_struct(token->name);
   if (res) return copy_token(res);
   return NULL;
}

int calculate_padding(int offset, int alignment) {
   if (check(!alignment, "invalid alignment")) return 0;
   return (alignment - (offset % alignment)) % alignment;
}
/*
16
8
4
*/
void set_struct_size(Token *token)
{
   int offset = 0;
   Token *attr;
   for (int i = 0; i < token->Struct.pos; i++)
   {
      attr = token->Struct.attrs[i];
      int size = sizeofToken(attr);
      int padding = calculate_padding(offset, size);
      offset += padding;
      attr->offset = offset;
      offset += size;
   }
   int max_alignment = 8; // For Id struct, this would be alignof(char*)
   int end_padding = calculate_padding(offset, max_alignment);
   int total_size = offset + end_padding;
   token->offset = offset;
   // token->ptr = total_size;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(STRUCT_DEF)))
   {
      if (check(!(token = find(ID, 0)), "expected identifier after struct definition")) return NULL;
      if (check(!find(DOTS, 0), "expected dots after struct definition")) return NULL;
      node = new_node(token);
      node->token->type = STRUCT_DEF;
      bool block = false;
      while (within_space(token->space))
      {
         Token *attr = find(INT, CHARS, CHAR, FLOAT, BOOL, ID, 0);
         Token *id = find(ID, 0);
         //if (attr->type == ID) attr = get_struct(attr->name);
         if (check(!attr, "expected data type followed by id")) break;
         if (check(!id, "expected id after data type")) break;

         if (attr->type == ID)
         {
            attr = get_struct(attr->name);
            if (check(!attr, "")) exit(1);
            attr = copy_token(attr);
            char *name = id->name;
            id = attr;
            setName(id, name);
            id->type = STRUCT_CALL;
            block = true;
         }
         else
            id->type = attr->type;
         // attr->declare = false;
         add_attribute(token, id);
      }
      set_struct_size(node->token);
      new_struct(node->token);
      for (int i = 0; i < node->token->Struct.pos; i++)
      {
         Token *attr = node->token->Struct.attrs[i];
         debug(GREEN"<%k offset %d>\n" RESET, attr, attr->offset);
      }
      debug(GREEN"<struct offset %d>\n" RESET, node->token->offset);
      // if(block) exit(1);
      return node;
   }
   else if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, 0)))
   {
      Token *struct_token = NULL;
      debug("token: %k \n", token);
      if (token->type == ID && token->name && (struct_token =  is_struct(token)))
      {
         struct_token = copy_token(struct_token);
         Node *struct_node = new_node(struct_token);
         // token = tmp_node->token;
         if (!struct_node->token->struct_id)
         {
            check(1, "expected struct id\n");
            exit(1);
         }
         token = find(ID, 0);
         (check(!token, "Expected variable name after struct declaration\n"));
         setName(struct_node->token, token->name);
         struct_node->token->type = STRUCT_CALL;

         debug(RED"==========================================\n");
         debug("offset %d\n", struct_node->token->offset);
         ptr += struct_node->token->offset;
         for (int i = 0; i < struct_node->token->Struct.pos; i++)
         {
            Token *attr = struct_node->token->Struct.attrs[i];
            attr->ptr = ptr - attr->offset;
            debug("<%k PTR %d>\n", attr, attr->ptr);
         }
         debug("==========================================\n"RESET);
         return struct_node;
      }
      else if (token->declare)
      {
         Token *tmp = find(ID, 0);
         check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
         setName(token, tmp->name);
         new_variable(token);
         return new_node(token);
      }
      else if (token->type == ID && token->name && find(LPAR, 0))
      {
         node = new_node(token);
         if (strcmp(token->name, "main") == 0) return func_main(node);
         return func_call(node);
      }
      return new_node(token);
   }
   else if ((token = find(REF, 0)))
   {
      node = prime(); // TODO: check it
      check(!node->token->declare, "must be variable declaration after ref");
      node->token->is_ref = true;
      return node;
   }
   else if ((token = find(FDEC, 0)))
      return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
      node = new_node(token);
      node->left = expr();
      return node;
   }
   else if ((token = find(IF, 0))) return if_node(new_node(token));
   else if ((token = find(WHILE, 0))) return while_node(new_node(token));
   else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
   else if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr();
      check(!find(RPAR, 0), "expected right par");
      return node;
   }
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}

Node *global;
void generate_ast()
{
   if (found_error) return;
   global = new_node(new_token(".global", 0, strlen(".global"), ID, 0));
   enter_scoop(global);

   debug(BLUE BOLD"AST:\n" RESET);
   head = new_node(NULL);
   Node *curr = head;
   curr->left = expr();
   while (tokens[exe_pos]->type != END && !found_error)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   if (found_error) return;
   debug(BLUE BOLD"PRINT AST:\n" RESET);
   curr = head;
   while (curr && !found_error)
   {
      debug("%n\n", curr->left);
      curr = curr->right;
   }
   //exit_scoop();
   //free_node(global);
}

// INTERMEDIATE REPRESENTATION
bool optimize_ir()
{

   static int op = 0;
   static bool did_optimize = false;
   bool did_something = false;
   switch (op)
   {
   case 0:
   {
      debug(CYAN "OP[%d] calculate operations on values\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         Token *left = insts[i]->left;
         Token *right = insts[i]->right;

         Type types[] = {INT, FLOAT, CHAR, 0};
         Type ops[] = {ADD, SUB, MUL, DIV, 0};
         // TODO: check if left and right are compatible
         // test if left is function, and right is number ...
         if (
            includes(ops, token->type) && includes(types, left->type) &&
            compatible(left, right) && !left->name && !right->name)
         {
            did_something = true;
            did_optimize = true;
            switch (left->type)
            {
            case INT:
               switch (token->type)
               {
               case ADD: token->Int.value = left->Int.value + right->Int.value; break;
               case SUB: token->Int.value = left->Int.value - right->Int.value; break;
               case MUL: token->Int.value = left->Int.value * right->Int.value; break;
               case DIV: token->Int.value = left->Int.value / right->Int.value; break;
               default: break;
               }
               break;
            default:
               check(1, "handle this case\n", "");
               break;
            }
            token->type = left->type;
            token->retType = 0;
            insts[i]->left = NULL;
            insts[i]->right = NULL;
            left->remove = true;
            right->remove = true;
            token->reg = 0;
            setReg(token, NULL);
            if (i > 0) i -= 2;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 1:
   {
      debug(CYAN "OP[%d] calculate operations on constants\n" RESET, op);
      for (int i = 1; insts && insts[i]; i++)
      {
         Token *curr_token = insts[i]->token;
         Token *curr_left = insts[i]->left;
         Token *curr_right = insts[i]->right;

         Token *prev_token = insts[i - 1]->token;
         // Token *prev_left = insts[i - 1]->left;
         Token *prev_right = insts[i - 1]->right;

         //  TODO: handle string also here X'D ma fiyach daba
         if (curr_token->type == ADD && prev_token->type == ADD)
         {
            if (curr_left == prev_token && !prev_right->name && !curr_right->name)
            {
               // prev_right->type = INT;
               curr_token->remove = true;
               prev_right->Int.value += curr_right->Int.value;
               i = 1;
               clone_insts();
               did_something = true;
               did_optimize = true;
               continue;
            }
            // else
            // if(curr_right == prev_token && !prev_left->name && !curr_left->name)
            // {
            //   // prev_r->type = INT;
            //   // curr_token->remove = true;
            //   prev_left->Int.value += curr_left->Int.value;
            //   i = 1;
            //   clone_insts();
            //   optimize = true;
            //   continue;
            // }
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 2:
   {
      debug(CYAN "OP[%d] remove reassigned variables\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         if (!token->ptr) continue;
         if (token->declare)
         {
            for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
            {
               if (insts[j]->token->type == ASSIGN && insts[j]->left->ptr == token->ptr /*&& !token->is_ref*/)
               {
                  // debug(RED"1. remove r%d %k\n"RESET, token->reg, token);
                  token->declare = false;
                  token->remove = true;
                  did_optimize = true;
                  did_something = true;
                  insts[j]->left->is_ref = token->is_ref;
                  break;
               }
               if ((insts[j]->left && insts[j]->left->reg == token->reg) ||
                     (insts[j]->right && insts[j]->right->reg == token->reg)) {
                  break;
               }
            }
         }
         else if (token->type == ASSIGN)
         {
            for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
            {
               if (!insts[j]->left || !insts[j]->right) continue;
               // TODO: to be checked
               // I replaced insts[j]->left == insts[i]->left with insts[j]->left->ptr == insts[i]->left->ptr
               if (insts[j]->token->type == ASSIGN && insts[j]->left->ptr == token->ptr && !insts[i]->left->is_ref)
               {
                  // debug(RED"2. remove r%d %k\n"RESET, token->reg, token);
                  token->remove = true;
                  did_optimize = true;
                  did_something = true;
                  break;
               }
               if (insts[j]->left->reg == token->reg || insts[j]->right->reg == token->reg)
                  break;
            }
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 3:
   {
      debug(CYAN"OP[%d] remove followed return instructions\n"RESET, op);
      for (int i = 1; insts[i]; i++)
      {
         if (insts[i]->token->type == RETURN && insts[i - 1]->token->type == RETURN)
         {
            did_optimize = true;
            did_something = true;
            insts[i]->token->remove = true;
            clone_insts();
            i = 1;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 4:
   {
      // TODO: be carefull this one remove anything that don't have reg
      debug(CYAN "OP[%d] remove unused instructions\n"RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *curr = insts[i]->token;
         if (!curr->ptr && !curr->reg && includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0}, curr->type))
         {
            curr->remove = true;
            did_something = true;
            did_optimize = true;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 5:
   {

      break;
   }
   default:
   {
      op = 0;
      if (!did_optimize) return false;
      did_optimize = false;
      return true;
      break;
   }
   }
   op++;
   return true;
}

int str_index;
int bloc_index;
Token *if_ir(Node *node)
{
   enter_scoop(node);

   Inst *inst = new_inst(node->token);
   setName(inst->token, "if");
   inst->token->type =  BLOC;
   inst->token->index = ++bloc_index;

   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   setName(cond, "endif");
   cond->index = inst->token->index;

   Token *lastInst = cond;
   // code bloc
   for (int i = 0; i < node->cpos; i++) generate_ir(node->children[i]);

   Inst *endInst = NULL;
   if (node->right->cpos)
   {
      endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space + TAB));
      endInst->token->index = node->token->index;
   }
#if 1
   Node *subs = node->right;
   for (int i = 0; i < subs->cpos; i++)
   {
      Node *curr = subs->children[i];
      if (curr->token->type == ELIF)
      {
         curr->token->index = ++bloc_index;
         curr->token->type = BLOC;
         setName(curr->token, "elif");
         char *name = strdup(lastInst->name);
         int index = lastInst->index;
         {
            setName(lastInst, "elif");
            lastInst->index = curr->token->index;
            lastInst = copy_token(lastInst);
         }
         new_inst(curr->token); // elif bloc
         setName(curr->left->token, name);
         free(name);
         generate_ir(curr->left); // elif condition, TODO: check is boolean
         curr->left->token->index = index;
         lastInst = curr->left->token;
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space + TAB));
         endInst->token->index = node->token->index;
      }
      else if (curr->token->type == ELSE)
      {
         curr->token->index = ++bloc_index;
         curr->token->type = BLOC;
         setName(curr->token, "else");
         new_inst(curr->token);
         {
            setName(lastInst, "else");
            lastInst->index = curr->token->index;
            lastInst = copy_token(lastInst);
         }
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         break;
      }

   }
#endif
   Token *new = new_token("endif", 0, 5, BLOC, node->token->space);
   new->index = node->token->index;
   new_inst(new);
   // free_token(lastInst);
   exit_scoop();
   return node->left->token;
}

Token *while_ir(Node *node)
{
   enter_scoop(node);
   node->token->type = BLOC;
   setName(node->token, "while");
   node->token->index = ++bloc_index;
   Inst *inst = new_inst(node->token);

   setName(node->left->token, "endwhile");
   generate_ir(node->left); // TODO: check if it's boolean
   node->left->token->index = node->token->index;

   for (int i = 0; i < node->cpos; i++) // while code bloc
   {
      Node *curr = node->children[i];
      Token *lastInst;
      switch (curr->token->type)
      {
      case BREAK:
         lastInst = copy_token(node->left->token);
         lastInst->type = JMP;
         setName(lastInst, "endwhile");
         new_inst(lastInst); // jmp back to while loop
         break;
      case CONTINUE:
         lastInst = copy_token(node->token);
         lastInst->type = JMP;
         setName(lastInst, "while");
         new_inst(lastInst); // jmp back to while loop
         break;
      default:
         generate_ir(curr); break;
      }
   }

   Token *lastInst = copy_token(node->token);
   lastInst->type = JMP;
   setName(lastInst, "while");
   new_inst(lastInst); // jmp back to while loop

   lastInst = copy_token(node->token);
   lastInst->type = BLOC;
   setName(lastInst, "endwhile");
   new_inst(lastInst); // end while bloc
   exit_scoop();
   return inst->token;
}

Token *func_dec_ir(Node *node)
{
   new_function(node);
   enter_scoop(node);
   int tmp_ptr = ptr;
   ptr = 0;
   Inst* inst = new_inst(node->token);

   // arguments
   Node *curr = node->left;
   for (int i = 0; curr && i < curr->cpos && !found_error; i++)
   {
      Node *child = curr->children[i];
      generate_ir(child);
      //new_variable(child->token);
      if (child->token->is_ref) child->token->has_ref = true;
   }
   pnode(node, NULL, 0);

   // code bloc
   for (int i = 0; i < node->cpos; i++)
   {
      Node *child = node->children[i];
      generate_ir(child);
   }

   // TODO: if RETURN not found add it
   Token *new = new_token(NULL, 0, 0, END_BLOC, node->token->space);
   new->name = strdup(node->token->name);
   new_inst(new);
   node->token->ptr = ptr;
   ptr = tmp_ptr;
   exit_scoop();
   return inst->token;
}

Token *func_call_ir(Node *node)
{
   if (strcmp(node->token->name, "output") == 0)
   {
      check(1, "handle this case"); // TODO
   }
   else
   {
      Node *dec = get_function(node->token->name);
      if (!dec) return NULL;
      node->token->retType = dec->token->retType;
      setReg(node->token, dec->token->creg);
      dec = dec->left;

      Node *call = node;
#if 1
      debug("call: cpos [%d]\n", call->cpos);
      pnode(call, NULL, 0);
      debug("dec: cpos [%d]\n", dec->cpos);
      pnode(dec, NULL, 0);
#endif

      for (int i = 0 ; !found_error && i < call->cpos && i < dec->cpos; i++)
      {
         Node *carg = call->children[i];
         Node *darg = dec->children[i];
         if (darg->token->is_ref)
         {
            generate_ir(carg);
            carg->left->token->is_ref = true;
            carg->left->token->has_ref = 0;
            carg->left->token->ptr = 0;
            carg->token->ptr = 0;
         }
         else
         {
            if (carg->token->type == DOT)
            {
               debug("found");
               exit(1);
            }
            generate_ir(carg);
         }

      }
   }
   Inst* inst = new_inst(node->token);
   return inst->token;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   check(!compatible(left, right), "invalid [%s] op between %s and %s \n",
         to_string(node->token->type), to_string(left->type), to_string(right->type));

   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   switch (node->token->type)
   {
   case ADD_ASSIGN: case ASSIGN:
      node->token->reg = left->reg;
      node->token->retType = getRetType(node);
      break;
   case ADD: case SUB: case MUL: case DIV:
   {
      node->token->retType = getRetType(node);
      if (node->token->retType  == INT) setReg(node->token, "eax");
      else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
      else check(1, "handle this case");
      break;
   }
   case NOT_EQUAL: case EQUAL: case LESS:
   case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      node->token->retType = BOOL;
      node->token->index = ++bloc_index;
      // debug(RED"===========================");
      // debug("> %k\n", inst->token);
      // debug("left : %k\n", inst->left);
      // debug("right: %k\n", inst->right);
      // debug("===========================\n"RESET);
      return inst->token;
      // debug(RED "this is [%d]\n" RESET, node->token->reg);
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   return inst->token;
}


Token* generate_ir(Node *node)
{
   if (found_error) return NULL;
   Inst *inst = NULL;
   switch (node->token->type)
   {
   case ID:
   {
      if (node->token->isattr) return node->token;
      return get_variable(node->token->name);
   }
   case INT: case BOOL: case CHAR: case FLOAT:
   {
      inst = new_inst(node->token);
      // if (inst->token->name && inst->token->declare) new_variable(inst->token);
      break;
   }
   case CHARS:
   {
      inst = new_inst(node->token);
      //if (inst->token->name && inst->token->declare) new_variable(inst->token);
      //else
      if (!node->token->creg && !node->token->index) node->token->index = ++str_index;
      break;
   }
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
      return op_ir(node);
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FDEC:  return func_dec_ir(node);
   case FCALL: return func_call_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      break;
   }
   case BREAK:
   {
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            Token *token = copy_token(node->token);
            token->type = JMP;
            token->index = scoop->token->index;
            setName(token, "endwhile");
            inst = new_inst(token);
            break;
         }
      }
      // TODO: invalid syntax, break should be inside whie loop
      break;
   }
   case CONTINUE:
   {
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            Token *token = copy_token(node->token);
            token->type = JMP;
            token->index = scoop->token->index;
            setName(token, "while");
            inst = new_inst(token);
            break;
         }
      }
      // TODO: invalid syntax, break should be inside whie loop
      break;
   }
   case STRUCT_CALL:
   {
      pnode(node, NULL, 0);
      if (!node->token->struct_id)
      {
         check(1, "expected struct id\n");
         exit(1);
      }
      for (int i = 0; i < node->token->Struct.pos; i++)
      {
         Token *attr = node->token->Struct.attrs[i];
         attr->declare = false;
         debug(CYAN"line %d: IR for %k\n"RESET, LINE, attr);
         Node *tmp = new_node(attr);
         if (attr->type == STRUCT_CALL)
         {
            Token *curr = attr;
            for (int i = 0; i < curr->Struct.pos; i++)
            {
               Token *attr = curr->Struct.attrs[i];
               attr->declare = false;
               debug(CYAN"line %d: IR for %k\n"RESET, LINE, attr);
               Node *tmp = new_node(attr);
               // generate_ir(tmp);
               free_node(tmp);

               attr->declare = true;
            }
         }
         else
            generate_ir(tmp);
         free_node(tmp);

         attr->declare = true;
      }

      return NULL;
      break;
   }
   case DOT:
   {
      //(node, NULL, 0);
      // TODO: left and right both must have name
      Token *left =  generate_ir(node->left);
      Token *right =  generate_ir(node->right);
      //debug("access %k from %k \n", right, left);
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         ptoken(attr);
         if (strcmp(right->name, attr->name) == 0)
         {
            attr = copy_token(attr);
            char *name = strjoin(left->name, ".", attr->name);
            setName(attr, name);
            free(name);
            debug(RED"\n=>found %k\n"RESET, attr);
            ptoken(attr);
            // exit(1);
            return attr;
         }
      }

      check(1, "%s has no attribute %s", left->name, right->name);
      exit(1);
      return right;
      break;
   }
   case LBRA:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      debug("access %k in %k\n", right, left);
      inst = new_inst(node->token);
      break;
   }
   case STRUCT_DEF: return node->token;
   default: check(1, "handle this case %s", to_string(node->token->type)); return NULL;
   }
   return inst->token;
}

// ASSMBLY GENERATION
struct _IO_FILE *asm_fd;

void generate_asm(char *name)
{
   if (found_error) return;
   char *outfile = strdup(name);
   outfile[strlen(outfile) - 1] = 's';
   asm_fd = fopen(outfile, "w+");
   check(!asm_fd, "openning %s\n", outfile);
   if (found_error) return;
   free(outfile);
   initialize();
   clone_insts();
   for (int i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      asm_space(curr->space);
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT:
      {
         if (curr->declare)
         {
            pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         }
         break;
      }
      case ADD_ASSIGN:
      {
         // TODO: check this
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); asm_space(curr->space);}
         break;
      }
      case ASSIGN:
      {
         // char *inst = left->type == FLOAT ? "movss " : "mov ";
         check(right->is_ref && !right->has_ref, "can't assign from reference that don't point to anything");
         check(left->is_ref && !left->has_ref && (!right->ptr && !right->creg), "first assignment for ref must have have ptr");
         //check(left->is_ref && right->is_ref, "assignment between two references is forbidden");

         if (left->is_ref) debug("left is ref\n");
         if (left->has_ref) debug("left has ref\n");
         // if(left->isarg) debug("left is arg\n");

         if (right->is_ref) debug("right is ref\n");
         if (right->has_ref) debug("right has ref\n");
         // if(right->isarg) debug("right is arg\n");

         if (left->is_ref && !left->has_ref)
         {
            left->has_ref = true;
            if (right->ptr)
            {
               //check(1, "found"); exit(1);
               // int a = 1 ref int b = a
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
               else pasm("%irdi, rax", "mov"); // is function argument
            }
            else if (right->creg) // I added for function arguments that are references
            {
               /*
                  func int m(ref int a):
                     a = 1
                     return 1
                  main():
                     int x = 2
                     m(x)
                     putnbr(x)
               */
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], %ra", "mov", left->ptr, right);
               else check(1, "handle this case");
            }
            else
            {
               check(1, "handle this case");
            }
         }
         else if (left->is_ref && left->has_ref)
         {
            if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 int c = 2 ref int b = a b = c putnbr(a)
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%rd, %a", "mov", left, right); asm_space(curr->space);
               pasm("%i%ma, %rd", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "handle this case");
               // int a = 1 + 2 ref int b = a b = 2 + a putnbr(a)
               pasm("%i%rb, %ra", "mov", left, right); asm_space(curr->space);
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%ma, %rb", "mov", left, left);
            }
            // else if(right->type == CHARS)
            // {
            //    check(1, "found"); exit(1);
            // }
            else // right is value
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a b = 3
               pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
               pasm("%i%ma, %v", "mov", left,  right);
            }
         }
         else if (left->ptr && !left->is_ref)
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 + 2 ref int b = a int c = b putnbr(c)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 int b = a
               pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               // int a = 1
               pasm("%i%a, %v", "mov", left, right, left->name);
            }
         }
         else if (left->creg) // function parameter
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a putnbr(a)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a b = 3 putnbr(a)
               pasm("%i%ra, %a", "mov", left, right); //asm_space(curr->space);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               // putnbr(1 + 2)
               pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               // putstr("cond 1\n")
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, %v", "mov", left, right, left->name);
            }
         }
         else
         {
            check(1, "handle this case");
            debug("left:%k, right:%k\n", left, right);
            // check(1, "handle this case left:%k, right:%k", left, right);
         }
         if (left->name) {pasm(" ;// assign [%s]", left->name); }
         else if (left->creg) {pasm(" ;// assign [%s]", left->creg);}
         if (left->is_ref) {pasm(" is_ref"); }
         asm_space(curr->space);
         break;
      }
      case ADD: case SUB: case MUL: case DIV: // TODO: check all math_op operations
      {
         // TODO: use rax for long etc...
         // TODO: something wrong here, fix it
         // Type type = curr->type;
         char *inst = left->type == FLOAT ? "movss" : "mov";
         char *inst2 = NULL;
         switch (curr->type)
         {
         case ADD: inst2 = left->type == FLOAT ? "addss " : "add "; break;
         case SUB: inst2 = left->type == FLOAT ? "subss " : "sub "; break;
         case MUL: inst2 = left->type == FLOAT ? "imulss " : "imul "; break;
         case DIV: inst2 = left->type == FLOAT ? "divss " : "div "; break;
         default: break;
         }
         pasm("%i%ra, ", inst, left);
         if (left->ptr) pasm("%a", left);
         else if (left->creg) pasm("%ra", left) ;
         else pasm("%v", left);
         asm_space(curr->space);
         pasm("%i%ra, ", inst2, right);
         if (right->ptr) pasm("%a", right);
         else if (right->creg) pasm("%ra", right) ;
         else pasm("%v", right);
         // curr->type = getTypeleft->type;
         break;
      }
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case MORE_EQUAL:
      {
         if (curr->is_cond)
         {
            char *inst = NULL;
            switch (curr->type)
            {
            case EQUAL: inst = "jne"; break;
            case NOT_EQUAL: inst = "je"; break;
            case LESS: inst = "jge"; break;
            case LESS_EQUAL: inst = "jg"; break;
            case MORE: inst = "jle"; break;
            case MORE_EQUAL: inst = "jl"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space); pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%ra", left);
            else if (!left->creg) pasm("%v", left);

            // asm_space(curr->space);
            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %ra", right);
            else if (!right->creg) pasm(", %v", right);
            asm_space(curr->space); pasm("%i .%s%d", inst, curr->name ? curr->name : "(null)", curr->index);
         }
         else
         {
            char *inst = left->type == FLOAT ? "movss" : "mov";
            if (left->ptr) pasm("%i%ra, %a", inst, left, left);
            else if (left->creg /*&& strcmp(left->creg, r->creg)*/) pasm("%i%ra, %ra", inst, left, left);
            else if (!left->creg) pasm("%i%ra, %v", inst, left, left);

            char *reg = NULL;
            switch (left->type)
            {
            case INT: reg = "ebx"; break;
            case FLOAT: reg = "xmm1"; break;
            case CHAR: reg = "bl"; break;
            case BOOL: reg = "ebx"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space);
            if (right->ptr) pasm("%i%s, %a", inst, reg, right);
            else if (right->creg) pasm("%i%s, %ra", inst, reg, right);
            else if (!right->creg) pasm("%i%s, %v", inst, reg, right);

            inst = left->type == FLOAT ? "ucomiss" : "cmp";
            asm_space(curr->space);
            pasm("%i%ra, %s", inst, left, reg);
            switch (curr->type)
            {
            case EQUAL: inst = "sete"; break;
            case NOT_EQUAL: inst = "setne"; break;
            case LESS: inst = "setl"; break;
            case LESS_EQUAL: inst = "setle"; break;
            case MORE: inst = "setg"; break;
            case MORE_EQUAL: inst = "setge"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            curr->retType = BOOL;
            setReg(curr, "al");
            asm_space(curr->space); pasm("%i%ra", inst, curr);
         }
         break;
      }
      case FDEC:
      {
         pasm("%s:", curr->name);
         asm_space(curr->space + TAB); pasm("%irbp", "push");
         asm_space(curr->space + TAB); pasm("%irbp, rsp", "mov");
         asm_space(curr->space + TAB); pasm("%irsp, %d", "sub", (((curr->ptr) + 15) / 16) * 16);
         break;
      }
      case RETURN:
      {
         /*
         TODO: handle reference return
         func int m(ref int a):
            a = 1
            return a
         */
         if (left->ptr) pasm("%i%ra, %a", "mov", left, left);
         else if (left->creg)
         {
            // TODO: check the type
            if (strcmp(left->creg, "eax")) pasm("%i%ra, %a", "mov", left, left);
         }
         else
         {
            switch (left->type)
            {
            case INT: pasm("%i%ra, %ld", "mov", left, left->Int.value); break;
            case VOID: pasm("%ieax, 0", "mov"); break;
            default: check(1, "handle this case [%s]\n", to_string(left->type)); break;
            }
         }
         asm_space(curr->space); pasm("%i", "leave");
         asm_space(curr->space); pasm("%i", "ret");
         break;
      }
      case JE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "je", curr->name, curr->index); break;
      case JNE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "jne", curr->name, curr->index); break;
      case JMP: pasm("%i.%s%d", "jmp", curr->name, curr->index); break;
      case FCALL: pasm("%i%s", "call", curr->name); break;
      case BLOC: pasm(".%s%d:", (curr->name ? curr->name : "(null)"), curr->index); break;
      case END_BLOC: pasm(".end%s:", curr->name); break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
}

void add_builtins()
{
   if (found_error) return;
   create_builtin("putnbr", (Type[]) {INT, 0}, INT);
   create_builtin("write", (Type[]) {INT, CHARS, INT, 0}, INT);
   create_builtin("read", (Type[]) {INT, CHARS, INT, 0}, INT);
   create_builtin("exit", (Type[]) {INT, 0}, INT);
   create_builtin("malloc", (Type[]) {INT, 0}, VOID);
   create_builtin("calloc", (Type[]) {INT, INT, 0}, VOID);
   create_builtin("strdup", (Type[]) {CHARS, 0}, CHARS);
   create_builtin("strlen", (Type[]) {CHARS, 0}, INT);
   // create_builtin("free", (Type[]) {VOID, 0}, VOID);
   create_builtin("strcpy", (Type[]) {CHARS, CHARS, 0}, CHARS);
   create_builtin("strncpy", (Type[]) {CHARS, CHARS, INT, 0}, CHARS);
   create_builtin("puts", (Type[]) {CHARS, 0}, INT);
   create_builtin("putstr", (Type[]) {CHARS, 0}, INT);
   create_builtin("putchar", (Type[]) {CHAR, 0}, INT);
   create_builtin("putbool", (Type[]) {BOOL, 0}, INT);
   create_builtin("putfloat", (Type[]) {FLOAT, 0}, INT);
   create_builtin("strcmp", (Type[]) {CHARS, CHARS, 0}, INT);
}

void generate(char *name)
{
   if (found_error) return;
#if IR
   debug(BLUE BOLD"GENERATE IR:\n" RESET);
   Node *curr = head;
   add_builtins();
   while (curr && !found_error)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }
   if (found_error) return;
   debug(BLUE BOLD"PRINT IR:\n" RESET);
   print_ir();
   debug(BLUE BOLD"OPTIMIZE IR:\n" RESET);
   clone_insts();
#endif
#if OPTIMIZE
   while (OPTIMIZE && !found_error && optimize_ir()) clone_insts();
#endif
#if ASM
   debug(BLUE BOLD"GENERATE ASM:\n" RESET);
   generate_asm(name);
#endif
}

int main(int argc, char **argv)
{
   check(argc < 2, "Invalid arguments");
   open_file(argv[1]);
   tokenize();
   generate_ast();
   generate(argv[1]);
   free_memory();
}

/*
2090 + 1528
1819 + 1179

*/