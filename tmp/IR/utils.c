#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <fcntl.h>

#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define FUNC __func__
#define LINE __LINE__
#define FILE __FILE__

#define TOKENIZE 1

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#endif

#define MAX_OPTIMIZATION 4
#define WITH_COMMENTS 1

#if IR
#define BUILTINS 1
#ifndef OPTIMIZE
#define OPTIMIZE 1
#endif
#define ASM 1
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

int debug(char *fmt, ...)
{
  int res = 0;
#if DEBUG
  va_list ap;
  va_start(ap, fmt);
  res = vprintf(fmt, ap);
  va_end(ap);
#endif
  return res;
}

bool found_error;
int error(char *fmt, ...)
{
  found_error = true;
  va_list ap;
  va_start(ap, fmt);
  int res = fprintf(stderr, "%sError:%s ", RED, RESET);
  res += vfprintf(stderr, fmt, ap);
  va_end(ap);
  return res;
}

#define RLOG(log, msg, ...)              \
  {                                      \
    debug("%s%8s%s: ", RED, log, RESET); \
    debug(msg, ##__VA_ARGS__);           \
  }

#define CLOG(log, msg, ...)               \
  {                                       \
    debug("%s%8s%s: ", CYAN, log, RESET); \
    debug(msg, ##__VA_ARGS__);            \
  }

#define GLOG(log, msg, ...)                \
  {                                        \
    debug("%s%8s%s: ", GREEN, log, RESET); \
    debug(msg, ##__VA_ARGS__);             \
  }

typedef enum
{
  assign_ = 11,
  add_assign_,
  sub_assign_,
  mul_assign_,
  div_assign_,
  lpar_,
  rpar_,
  mul_,
  add_,
  sub_,
  div_,
  mod_,
  equal_,
  not_equal_,
  less_,
  more_,
  less_equal_,
  more_equal_,
  and_,
  or_,
  id_,
  int_,
  bool_,
  char_,
  chars_,
  void_,
  float_,
  ptr_,
  struct_,
  end_struct_,
  coma_,
  fcall_,
  fdec_,
  ret_,
  if_,
  elif_,
  else_,
  while_,
  dots_,
  dot_,
  module_,

  cmp_,
  jne_,
  je_,
  jmp_,
  bloc_,
  end_bloc_,
  mov_,
  push_,
  pop_,
  end_,
} Type;

char *to_string(Type type)
{
  switch (type)
  {
  case add_:
    return "ADD";
  case sub_:
    return "SUB";
  case mul_:
    return "MUL";
  case div_:
    return "DIV";
  case mod_:
    return "MOD";

  case equal_:
    return "EQUAL";
  case not_equal_:
    return "NOT EQUAL";
  case less_:
    return "LESS THAN";
  case more_:
    return "MORE THAN";
  case more_equal_:
    return "MORE THAN OR EQUAL";
  case less_equal_:
    return "LESS THAN OR EQUAL";
  case and_:
    return "AND";
  case or_:
    return "OR";

  case float_:
    return "FLOAT";
  case int_:
    return "INT";
  case char_:
    return "CHAR";
  case chars_:
    return "CHARS";
  case bool_:
    return "BOOL";
  case void_:
    return "VOID";
  case ptr_:
    return "PTR";
  case struct_:
    return "STRUCT";
  case end_struct_:
    return "END STRUCT";
  case id_:
    return "ID";
  case coma_:
    return "COMA";

  case assign_:
    return "ASSIGN";
  case add_assign_:
    return "ADD ASSIGN";
  case sub_assign_:
    return "SUB ASSIGN";
  case mul_assign_:
    return "MUL ASSIGN";
  case div_assign_:
    return "DIV ASSIGN";

  case lpar_:
    return "LPARENT";
  case rpar_:
    return "RPARENT";
  case fcall_:
    return "FUNC CALL";
  case fdec_:
    return "FUNC DECL";

  case jne_:
    return "JNE";
  case je_:
    return "JE";
  case jmp_:
    return "JMP";
  case cmp_:
    return "CMP";
  case mov_:
    return "MOV";
  case push_:
    return "PUSH";
  case pop_:
    return "POP";
    // case arg_: return "ARG";

  case while_:
    return "WHILE";
  case if_:
    return "IF";
  case elif_:
    return "ELIF";
  case else_:
    return "ELSE";
  case dots_:
    return "DOTS";
  case dot_:
    return "DOT";
  case module_:
    return "MODULE";
  case bloc_:
    return "BLOC";
  case end_bloc_:
    return "ENDBLOC";
  case ret_:
    return "RETURN";
  case end_:
    return "END";
  }
  return NULL;
}

typedef struct
{
  char *value;
  Type type;
} Specials;

extern Specials *specials;

typedef struct
{
  // TODO: remove unused attributes
  Type type;
  Type retType;

  char *creg;
  int sreg;
  // bool hasreg;
  // char c;
  long ptr;

  size_t size;
  int offset;
  size_t allign;

  size_t index;
  int space;

  char *name;
  bool declare;
  bool remove;
  bool isbuiltin;
  bool isarg;
  char *arg_reg;

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
  Token *token;
  Token *left;
  Token *right;
} Inst;

typedef struct
{
  char *name;
  Type type;
  Node **functions;
  size_t func_size;
  size_t func_pos;

  Token **variables;
  size_t var_size;
  size_t var_pos;

  Node **structs;
  size_t struct_size;
  size_t struct_pos;
} Scoop;

// DEBUG
Specials *specials = (Specials[]){
    {".", dot_},
    {":", dots_},
    {"+=", add_assign_},
    {"-=", sub_assign_},
    {"*=", mul_assign_},
    {"/=", div_assign_},
    {"!=", not_equal_},
    {"==", equal_},
    {"<=", less_equal_},
    {">=", more_equal_},
    {"<", less_},
    {">", more_},
    {"=", assign_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {"%", mod_},
    {"(", lpar_},
    {")", rpar_},
    {",", coma_},
    {"if", if_},
    {"elif", elif_},
    {"else", else_},
    {"while", while_},
    {"func", fdec_},
    {"return", ret_},
    {"and", and_},
    {"&&", and_},
    {"or", or_},
    {"||", or_},
    {0, (Type)0},
};

void ptoken(Token *token)
{
  debug("token ");
  switch (token->type)
  {
  case void_:
  case chars_:
  case char_:
  case int_:
  case bool_:
  case float_:
  {
    Type type = token->type;
    debug("[%s]", (type == chars_ ? "chars" : type == int_ ? "int"
                                          : type == bool_  ? "bool"
                                          : type == void_  ? "void"
                                          : type == char_  ? "char"
                                          : type == float_ ? "float"
                                                           : NULL));
    if (token->name)
    {
      debug(" name [%s]", token->name);
      if (token->declare)
        debug(" [declare]");
    }
    else if (token->type == int_)
      debug(" value [%lld]", token->Int.value);
    else if (token->type == chars_)
      debug(" value [%s]", token->Chars.value);
    else if (token->type == char_)
      debug(" value [%c]", token->Char.value);
    else if (token->type == bool_)
      debug(" value [%d]", token->Bool.value);
    else if (token->type == float_)
      debug(" value [%f]", token->Float.value);
    // else if(token->type == chars_)
    //     debug(" value [%s]", token->Chars.value);
    // else if(token->type == bool_)
    //     debug(" value [%d]", token->Bool.value);
    break;
  }
  case fcall_:
    debug("[func call] name [%s]", token->name);
    break;
  case fdec_:
    debug("[func dec] name [%s]", token->name);
    break;
  case id_:
    debug("[id] name [%s]", token->name);
    break;
  case struct_:
    debug("[struct]");
    break;
  case end_:
    debug("[end]");
    break;
  default:
  {
    for (int i = 0; specials[i].value; i++)
    {
      if (specials[i].type == token->type)
      {
        debug("[%s]", specials[i].value);
        break;
      }
    }
    break;
  }
  }
  debug(" space [%d]\n", token->space);
}

void pnode(Node *node, char *side, int space)
{
  if (node)
  {
    int i = 0;
    while (i < space)
    {
      i++;
      debug(" ");
    }
    if (side)
      debug("%s: ", side);
    if (node->token)
    {
      debug("node: ");
      ptoken(node->token);
    }
    else
      debug("\n");
#if 1
    if (node->token && node->token->type == fdec_)
    {
      node = node->right;
      while (node)
      {
        pnode(node->left, NULL, space + 4);
        node = node->right;
      }
    }
    else if (node->token && node->token->type == struct_)
    {
      i = 0;
      while (i < space)
      {
        i++;
        debug(" ");
      }
      debug("attributes:\n");
      node = node->left;
      while (node)
      {
        pnode(node->left, NULL, space + 4);
        node = node->right;
      }
    }
    else
#endif
    {
      pnode(node->left, "LEFT ", space + 2);
      pnode(node->right, "RIGHT", space + 2);
    }
  }
}

char *open_file(char *filename)
{
  struct _IO_FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    debug("Failed to open file %s\n", filename);
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *input = calloc(size + 1, sizeof(char));
  fread(input, size, sizeof(char), file);
  fclose(file);
  return input;
}

bool check_type(Type *types, Type type)
{
  for (int i = 0; types[i]; i++)
  {
    if (types[i] == type)
      return true;
  }
  return false;
}

Inst **copy_insts(Inst **src, Inst **dest, size_t pos, size_t size)
{
  // debug("%scheck %zu instuction%s\n", RED, pos, RESET);
  if (!size)
    return NULL;
  if (dest)
    free(dest);
  dest = calloc(size, sizeof(Inst *));
  size_t j = 0;
  for (size_t i = 0; i < pos; i++)
  {
    if (!src[i]->token->remove)
      dest[j++] = src[i];
  }
  // debug("%sdid Copy %d instuction%s\n", RED, j, RESET);
  return dest;
}

void pscoop(Scoop *scoop)
{
  debug("Scoop %s\n", scoop->name);
  debug("    variables:\n");
  for (size_t i = 0; i < scoop->var_pos; i++)
  {
    debug("        ");
    ptoken(scoop->variables[i]);
  }
  debug("    functions:\n");
  for (size_t i = 0; i < scoop->func_pos; i++)
  {
    debug("        ");
    debug("%s\n", scoop->functions[i]->token->name);
  }
}

struct _IO_FILE *asm_fd;
void pasm(char *fmt, ...)
{
  int i = 0;
  va_list args;
  va_start(args, fmt);
#define isInstruction(inst)                        \
  do                                               \
  {                                                \
    if (strncmp(fmt + i, inst, strlen(inst)) == 0) \
    {                                              \
      i += strlen(inst);                           \
      fprintf(asm_fd, "\t%-8s", inst);             \
    }                                              \
  } while (0)
  isInstruction("movss ");
  isInstruction("mov ");
  isInstruction("sub ");
  isInstruction("lea ");
  isInstruction("cmp ");
  isInstruction("push ");
  isInstruction("call ");
  isInstruction("leave");
  isInstruction("ret");

#if WITH_COMMENTS
  isInstruction("//");
#else
  if (strncmp(fmt + i, "//", 2) == 0)
    return;
#endif

  while (fmt[i])
  {
    if (fmt[i] == '%')
    {
      i++;
      if (fmt[i] == 'i')
      {
        i++;
        fprintf(asm_fd, "\t%-8s", va_arg(args, char *));
      }
      else if (fmt[i] == 'r')
      {
        i++;
        Token *token = va_arg(args, Token *);
        if (token->creg)
        {
          // printf("%s:%dhas been used\n",FUNC, LINE);
          // exit(1);
          fprintf(asm_fd, "%s", token->creg);
        }
        else
        {
          Type type = token->retType ? token->retType : token->type;
          switch (type)
          {
          case chars_:
            fputs("rax", asm_fd);
            break;
          case int_:
            fputs("eax", asm_fd);
            break;
          case bool_:
          case char_:
            fputs("al", asm_fd);
            break;
          case float_:
            fputs("xmm0", asm_fd);
            break;
          default:
            error("%s:%d Unkown type [%s]\n", FUNC, LINE, to_string(token->type));
            break;
          }
        }
      }
      else if (fmt[i] == 'a')
      {
        i++;
        Token *token = va_arg(args, Token *);
        switch (token->type)
        {
        case chars_:
          fprintf(asm_fd, "QWORD PTR -%ld[rbp]", token->ptr);
          break;
        case int_:
          fprintf(asm_fd, "DWORD PTR -%ld[rbp]", token->ptr);
          break;
        case char_:
          fprintf(asm_fd, "BYTE PTR -%ld[rbp]", token->ptr);
          break;
        case bool_:
          fprintf(asm_fd, "BYTE PTR -%ld[rbp]", token->ptr);
          break;
        case float_:
          fprintf(asm_fd, "DWORD PTR -%ld[rbp]", token->ptr);
          break;
        default:
          error("%s:%d Unkown type [%s]\n", FUNC, LINE, to_string(token->type));
          break;
        }
      }
      else if (fmt[i] == 'v')
      {
        i++;
        Token *token = va_arg(args, Token *);
        switch (token->type)
        {
        case int_:
          fprintf(asm_fd, "%lld", token->Int.value);
          break;
        case bool_:
          fprintf(asm_fd, "%d", token->Bool.value);
          break;
        case char_:
          fprintf(asm_fd, "%d", token->Char.value);
          break;
        default:
          error("%s:%d Unkown type [%s]\n", FUNC, LINE, to_string(token->type));
          break;
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
        if (!handled)
        {
          error("%s:%d handle this case [%s]\n", FUNC, LINE, fmt + i);
          exit(1);
        }
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

// #define math_op(op, fmt, ...) \
// do { \
//     switch(op) { \
//         case add_: pasm("    add     " fmt, __VA_ARGS__); break; \
//         case sub_: pasm("    sub     " fmt, __VA_ARGS__); break; \
//         case mul_: pasm("    mul     " fmt, __VA_ARGS__); break; \
//         case div_: pasm("    div     " fmt, __VA_ARGS__); break; \
//         default: break; \
//     } \
// } while (0)

// #define relational_op(op, fmt, ...) \
// do { \
//     switch(op) { \
//         case equal_:      pasm("    sete    " fmt, __VA_ARGS__); break; \
//         case not_equal_:  pasm("    setne   " fmt, __VA_ARGS__); break; \
//         case less_:       pasm("    setl    " fmt, __VA_ARGS__); break; \
//         case less_equal_: pasm("    setle   " fmt, __VA_ARGS__); break; \
//         case more_:       pasm("    setg    " fmt, __VA_ARGS__); break; \
//         case more_equal_: pasm("    setge   " fmt, __VA_ARGS__); break; \
//         default: break; \
//     } \
// } while (0)

// CLEAR
void free_node(Node *node)
{
  if (node)
  {
    free_node(node->left);
    free_node(node->right);
    free(node);
  }
}

void free_token(Token *token)
{
  if (token->name)
    free(token->name);
  if (token->Chars.value)
    free(token->Chars.value);
  if (token->arg_reg)
    free(token->arg_reg);
  free(token);
}

void clear(Node *head, Token **tokens, Inst **first_insts, char *input)
{
#if AST
  free_node(head);
#endif

#if TOKENIZE
  for (int i = 0; tokens && tokens[i]; i++)
    free_token(tokens[i]);
#endif

#if IR
  for (int i = 0; first_insts && first_insts[i]; i++)
    free(first_insts[i]);
  free(first_insts);
  // free(regs);
#endif
  free(input);
}

char *strjoin(char *left, char *right)
{
  size_t len = strlen(left) + strlen(right);
  char *res = calloc(len + 1, sizeof(char));
  strcpy(res, left);
  strcpy(res + strlen(res), right);
  return res;
}
