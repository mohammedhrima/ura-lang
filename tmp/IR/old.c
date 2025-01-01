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
  assign_ = 11, add_assign_, sub_assign_, mul_assign_, div_assign_, lpar_, rpar_, 
  mul_, add_, sub_, div_, mod_, equal_, not_equal_, less_, more_, less_equal_, more_equal_,
  and_, or_, id_, int_, bool_, char_, chars_, void_, float_, ptr_, struct_, end_struct_, 
  coma_, fcall_, fdec_, ret_, if_, elif_, else_, while_, dots_, dot_, module_ ,
  cmp_, jne_, je_, jmp_, bloc_, end_bloc_, mov_, push_, pop_, end_,
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

#if TOKENIZE
Token **tokens;
size_t tk_pos;
size_t tk_size;
size_t exe_pos;

void add_token(Token *token)
{
  if (tokens == NULL)
  {
    tk_size = 100;
    tokens = calloc(tk_size, sizeof(Token *));
  }
  else if (tk_pos + 1 == tk_size)
  {
    Token **tmp = calloc(tk_size * 2, sizeof(Token *));
    memcpy(tmp, tokens, tk_pos * sizeof(Token *));
    tk_size *= 2;
    free(tokens);
    tokens = tmp;
  }
  tokens[tk_pos++] = token;
}

Token *new_token(char *input, int s, int e, int space, Type type)
{
  Token *new = calloc(1, sizeof(Token));
  new->type = type;
  new->space = space;
  switch (type)
  {
  case int_:
  {
    while (s < e)
      new->Int.value = new->Int.value * 10 + input[s++] - '0';
    break;
  }
  case chars_:
  {
    new->Chars.value = calloc(e - s + 1, sizeof(char));
    strncpy(new->Chars.value, input + s, e - s);
    break;
  }
  case char_:
  {
    new->Char.value = input[s];
    break;
  }
  case float_:
  {
    while (s < e && input[s] != '.')
      new->Float.value = new->Float.value * 10 + (float)(input[s++] - '0');
    if (input[s] == '.')
      s++; // TODO: it shoul be always .
    float tmp = 0.0;
    float pres = 0.1;
    while (s < e)
    {
      tmp += (float)(input[s++] - '0') * pres;
      pres *= 0.1;
    }
    new->Float.value += tmp;
    break;
  }
  // case fcall_: case id_: case jmp_: case bloc_:
  default:
  {
    if (e > s)
    {
      new->name = calloc(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);

      if (strcmp(new->name, "True") == 0)
      {
        free(new->name);
        new->name = NULL;
        new->type = bool_;
        new->Bool.value = true;
      }
      else if (strcmp(new->name, "False") == 0)
      {
        free(new->name);
        new->name = NULL;
        new->type = bool_;
        new->Bool.value = false;
      }
      // else if (strcmp(new->name, "sys") == 0)
      // {
      //     new->type = module_;
      // }
      else if (strcmp(new->name, "struct") == 0)
      {
        free(new->name);
        new->name = NULL;
        new->type = struct_;
      }
    }
    break;
  }
  }
  add_token(new);
  return (new);
}

Token *copy_token(Token *token)
{
  if (token == NULL)
    return NULL;
  Token *new = calloc(1, sizeof(Token));
  memcpy(new, token, sizeof(Token));
  if (token->name) // TODO: check all values that can be copied exmaple: name ...
    new->name = strdup(token->name);
  if (token->Chars.value)
    new->Chars.value = strdup(token->Chars.value);
  add_token(new);
  return new;
}

void tokenize(char *input)
{
  int i = 0;
  int space = 0;
  bool new_line = true;
  while (input[i])
  {
    int s = i;
    if (isspace(input[i]))
    {
      if (input[i] == '\n')
      {
        new_line = true;
        space = 0;
      }
      if (input[i] == ' ')
        space++;
      /*
          + I commented this line for this case: int a b
          else if(!new_line && input[i] == ' ') space = 0;
      */
      i++;
      continue;
    }
    else if (input[i] == '#') // TODO: handle new lines inside comment
    {
      i++;
      while (input[i] && input[i] != '#')
        i++;
      if (input[i] != '#')
      {
        error("expected '#'\n");
        exit(1);
      }
      i++;
      continue;
    }
    else if (strncmp(input + i, "//", 2) == 0)
    {
      while (input[i] && input[i] != '\n')
        i++;
      continue;
    }
    new_line = false;
    bool found = false;
    for (int j = 0; specials[j].value; j++)
    {
      if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
      {
        new_token(NULL, 0, 0, space, specials[j].type);
        found = true;
        i += strlen(specials[j].value);
        break;
      }
    }
    if (found)
      continue;
    if (input[i] == '\"')
    {
      i++;
      while (input[i] && input[i] != '\"')
        i++;
      if (input[i] != '\"')
      {
        error("expected '\"'\n");
        exit(1);
      }
      i++;
      new_token(input, s, i, space, chars_);
      continue;
    }
    if (input[i] == '\'')
    {
      i++;
      if (input[i] && input[i] != '\'')
        i++;
      if (input[i] != '\'')
      {
        error("expected '\''\n");
        exit(1);
      }
      i++;
      new_token(input, s + 1, i, space, char_);
      continue;
    }
    if (isalpha(input[i]))
    {
      while (isalnum(input[i]))
        i++;
      new_token(input, s, i, space, id_);
      continue;
    }
    if (isdigit(input[i]))
    {
      Type type = int_;
      while (isdigit(input[i]))
        i++;
      if (input[i] == '.')
      {
        type = float_;
        i++;
      }
      while (isdigit(input[i]))
        i++;
      new_token(input, s, i, space, type);
      continue;
    }
    if (input[i])
    {
      error("Syntax <%c>\n", input[i]);
      exit(1);
    }
  }
  new_token(NULL, 0, 0, space, end_);
}
#endif

#if AST
Node *expr_node();
Node *assign_node();
Node *logic_node();
Node *equality_node();
Node *comparison_node();
Node *add_sub_node();
Node *mul_div_node();
Node *dot_node();
Node *sign_node();
Node *prime_node();

Node *new_node(Token *token)
{
  Node *new = calloc(1, sizeof(Node));
  new->token = token;
  debug("new node has ");
  if (token)
    ptoken(token);
  else
    debug("NULL\n");
  return new;
}

Node *copy_node(Node *node)
{
  Node *new = calloc(1, sizeof(Node));
  new->token = copy_token(node->token);
  if (node->left)
    new->left = copy_node(node->left);
  if (node->right)
    new->right = copy_node(node->right);
  return new;
}

Token *check(Type type, ...)
{
  va_list ap;
  va_start(ap, type);
  while (type)
  {
    if (type == tokens[exe_pos]->type)
      return tokens[exe_pos++];
    type = va_arg(ap, Type);
  }
  return NULL;
};

Node *expr_node()
{
  return assign_node();
}

Node *assign_node()
{
  Node *left = logic_node();
  Token *token;
  while ((token = check(assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, 0)))
  {
    Node *node = new_node(token);
    node->token->space = left->token->space;
    Node *right = logic_node();
    switch (token->type)
    {
    case add_assign_:
    case sub_assign_:
    case mul_assign_:
    case div_assign_:
    {
      Node *tmp = new_node(new_token(NULL, 0, 0, node->token->space,
                                     node->token->type == add_assign_ ? add_ : node->token->type == sub_assign_ ? sub_
                                                                           : node->token->type == mul_assign_   ? mul_
                                                                           : node->token->type == div_assign_   ? div_
                                                                                                                : 0));
      node->token->type = assign_;
      tmp->left = copy_node(left);
      tmp->left->token->declare = false;
      tmp->right = right;
      right = tmp;
      break;
    }
    default:
      break;
    }
    node->left = left;
    node->right = right;
    left = node;
  }
  return left;
}

Node *logic_node()
{
  Node *left = equality_node();
  Token *token;
  while (token = check(or_, and_, 0))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = equality_node();
    left = node;
  }
  return left;
}

Node *equality_node()
{
  Node *left = comparison_node();
  Token *token;
  while ((token = check(equal_, not_equal_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = comparison_node();
    left = node;
  }
  return left;
}

Node *comparison_node()
{
  Node *left = add_sub_node();
  Token *token;
  while ((token = check(less_, more_, less_equal_, more_equal_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = add_sub_node();
    left = node;
  }
  return left;
}

Node *add_sub_node()
{
  Node *left = mul_div_node();
  Token *token;
  while ((token = check(add_, sub_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = mul_div_node();
    left = node;
  }
  return left;
}

Node *mul_div_node()
{
  Node *left = dot_node();
  Token *token = NULL;
  while ((token = check(mul_, div_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = dot_node();
    left = node;
  }
  return left;
}

Node *dot_node()
{
  Node *left = sign_node();
  Token *token = NULL;
  if ((token = check(module_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = sign_node();
    // GLOG("", "found module\n");
    // pnode(node, NULL, 0);
    // exit(1);
    return node;
  }
  else if ((token = check(dot_, 0)))
  {
    Node *node = new_node(token);
    node->left = left;
    node->right = dot_node();
    return node;
  }
  return left;
}

Node *sign_node()
{
  Token *token = NULL;
  if ((token = check(sub_, 0)))
  {
    // TODO: check type could be int or float only
    token->type = mul_;
    Node *node = new_node(token);
    node->left = prime_node();
    node->right = new_node(new_token(NULL, 0, 0, token->space, int_));
    node->right->token->Int.value = -1;
    // GLOG("", "found module\n");
    // pnode(node, NULL, 0);
    // exit(1);
    return node;
  }
  return prime_node();
}

Specials DataTypes[] = {
    {"int", int_}, {"bool", bool_}, {"chars", chars_}, {"void", void_}, {"float", float_}, {"char", char_}, {0, 0}};

int sizeofToken(Token *token)
{
  switch (token->type)
  {
  case int_:
    return sizeof(int);
  case float_:
    return sizeof(float);
  case chars_:
    return sizeof(char *);
  case char_:
    return sizeof(char);
  case bool_:
    return sizeof(bool);
  // case struct_: return token->size;
  default:
  {
    error("%s %s:%d add this type [%s]\n", FILE, FUNC, LINE, to_string(token->type));
    exit(1);
  }
  }
  return 0;
}

// SCOOP
Scoop *global_scoop;
size_t bloc_size;
ssize_t scoop_pos = -1;
Scoop *curr_scoop;

Node *new_struct(Node *node)
{
  if (curr_scoop->structs == NULL)
  {
    curr_scoop->struct_size = 10;
    curr_scoop->structs = calloc(curr_scoop->struct_size, sizeof(Node *));
  }
  else if (curr_scoop->struct_pos + 1 == curr_scoop->struct_size)
  {
    curr_scoop->struct_size *= 2;
    Node **tmp = calloc(curr_scoop->struct_size, sizeof(Node *));
    memcpy(tmp, curr_scoop->structs, curr_scoop->struct_pos * sizeof(Node *));
    free(curr_scoop->structs);
    curr_scoop->structs = tmp;
  }
  curr_scoop->structs[curr_scoop->struct_pos++] = node;
  return node;
}

Node *prime_node()
{
  Node *node = NULL;
  Token *token;
  if ((
          token =
              check(int_, bool_, char_, float_, chars_, void_, struct_, id_, lpar_, rpar_, 0)))
  {
    for (int i = 0; token->type == id_ && DataTypes[i].value; i++)
    {
      if (strcmp(token->name, DataTypes[i].value) == 0)
      {
        Type type = DataTypes[i].type;
        int space = token->space;
        token = check(id_, 0);
        if (token)
        {
          node = new_node(token);
          node->token->type = type;
          node->token->space = space;
          node->token->declare = true;
          return node;
        }
        else
        {
          error("%s:%s\n", FUNC, LINE);
          exit(1);
        }
      }
    }
    for (int i = 0; token->type == id_ && curr_scoop->structs && curr_scoop->structs[i]; i++)
    {
      if (strcmp(curr_scoop->structs[i]->token->name, token->name) == 0)
      {
        // debug("name [%s]\n", curr_scoop->structs[i]->token->name);
        token = check(id_, 0);
        if (token)
        {
          node = copy_node(curr_scoop->structs[i]);
          node->token->space = token->space;
          node->token->declare = true;
          return node;
        }
        else
        {
          error("%s:%s\n", FUNC, LINE);
          exit(1);
        }
      }
    }
    if (token->type == lpar_)
    {
      node = expr_node();
      if (!check(rpar_, 0))
      {
        error("Expected ) but found '%s'\n", to_string(tokens[exe_pos]->type));
        exit(1);
      }
    }
#if 1
    else if (token->type == struct_)
    {
      node = new_node(token);

      if (!(token = check(id_, 0)))
      {
        // TODO: error
      }
      node->token->name = strdup(token->name);
      if (!check(dots_, 0))
      {
        // TODO: error
      }
      node->left = new_node(NULL);  // attributes
      node->right = new_node(NULL); // methods
      Node *curr = node->left;
      // Token *attrs[100];
      // size_t struct_size = 0;
      while (
          tokens[exe_pos]->type != end_ &&
          tokens[exe_pos]->space > node->token->space)
      {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr_node();
        // TODO: check all types
        if (!check_type((Type[]){int_, char_, chars_, bool_, float_, 0},
                        curr->left->token->type) ||
            !curr->left->token->declare)
        {
          error("expected data type inside struct\n");
          exit(1);
        }
        curr->left->token->size = sizeofToken(curr->left->token);
        // struct_size += sizeofType(curr->left->token->type);
        // curr->left->token->size = sizeofType(curr->left->token->type);
        // curr->left->token->ptr = struct_size;
        // attrs[pos++] = curr->left->token;
      }
      // debug(SPLIT);
      // pnode(node, NULL, 0);
      // debug(SPLIT);
      // debug("struct has the following attributes:\n");

      curr = node->left;
      size_t offset = 0;
      size_t align = 0;
      while (curr->right)
      {
        curr = curr->right;
        Token *token = curr->left->token;
        offset = (offset + token->size - 1) / token->size * token->size;
        token->offset = offset;
        offset += token->size;
        if (token->size > align)
          align = token->size;
      }
      node->token->offset = (offset + align - 1) / align * align;

      new_struct(node);

      curr = node->left;
      while (curr->right)
      {
        curr = curr->right;
        Token *token = curr->left->token;
        debug(GREEN "name [%s] type [%4s] size [%zu]\n" RESET,
              token->name, to_string(token->type), token->offset);
      }
      debug(GREEN "struct offset [%zu]\n" RESET, node->token->offset);
      // exit(1);
      return node;
    }
#endif
    else
    {
      node = new_node(token);
      if (check(lpar_, 0)) // TODO: only if is identifier
      {
        if (strcmp(node->token->name, "main") == 0)
        {
          if (!check(rpar_, 0))
          {
            // TODO: error
          }
          if (!check(dots_, 0))
          {
            // TODO: error
          }
          node->token->type = fdec_;
          Node *curr = node;
          while (
              tokens[exe_pos]->space > node->token->space &&
              tokens[exe_pos]->type != end_)
          {
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = expr_node();
          }
          curr->right = new_node(NULL);
          curr = curr->right;
          curr->left = new_node(new_token(NULL, 0, 0, node->token->space + 1, ret_));
          curr->left->left = new_node(new_token(NULL, 0, 0, node->token->space + 1, int_));
        }
        else
        {
          node->token->type = fcall_;
          // Node *func = get_function(node->token->name);
          Node *curr = node;
          while (!check(rpar_, end_, 0)) // TODO: protect it, if no ) exists
          {
            curr->left = expr_node();
            curr->left->token->isarg = true;
            if (!check(coma_, 0))
            {
              // TODO: syntax error
            }
            curr->right = new_node(NULL);
            curr = curr->right;
          }
        }
      }
    }
  }
#if 1
  else if ((token = check(fdec_, 0)))
  {
    node = new_node(token);

    node->left = new_node(NULL);
    node->left->left = prime_node();
    // TODO: hard code it
    if (!node->left->left->token || !node->left->left->token->declare)
    {
      error("expected datatype after func declaration\n");
      exit(1);
    }
    node->token->retType = node->left->left->token->type;
    node->token->name = node->left->left->token->name;
    node->left->left->token->name = NULL;
    // TODO: those errors should be checked
    if (!check(lpar_, 0))
    {
      error("expected ( after function declaration\n");
      exit(1);
    }
    debug("type: [%s]\n", to_string(tokens[exe_pos]->type));
    if (!check(rpar_, 0))
    {
      // inside_function = true;
      // arg_ptr = 8;
      Node *curr = node->left;
      while (!check_type((Type[]){rpar_, end_, 0}, tokens[exe_pos]->type))
      {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr_node();
        curr->left->token->isarg = true;
        if (tokens[exe_pos]->type != rpar_ && !check(coma_, 0))
        {
          error("expected ',' between arguments\n");
          exit(1);
        }
      }
      if (!check(rpar_, 0))
      {
        error("expected ) after func dec, but found <%s>\n",
              to_string(tokens[exe_pos]->type));
        exit(1);
      }
    }
    if (!check(dots_, 0))
    {
      error("expected : after func dec, but found <%s>\n",
            to_string(tokens[exe_pos]->type));
      exit(1);
    }

    node->token->type = fdec_;
    Node *curr = node;
    while (
        tokens[exe_pos]->space > node->token->space &&
        tokens[exe_pos]->type != end_)
    {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr_node();
    }
    curr->right = new_node(NULL);
    curr = curr->right;
    curr->left = new_node(new_token(NULL, 0, 0, node->token->space + 1, ret_));
    curr->left->left = new_node(new_token(NULL, 0, 0, node->token->space + 1,
                                          node->left->left->token->type));
    // TODO: add endblock for funstions also
    // curr->right = new_node(NULL);
    // curr = curr->right;
    // curr->left = new_node(new_token(, ));
    // Token *ret_token = new_token(NULL, 0, 0, node->token->space + 1, ret_);
    // curr->right->left = new_node(ret_token);
    return node;
  }
#endif
  // TODO: handle error parsing
  else if ((token = check(if_, 0)))
  {
    node = new_node(token);

    Node *tmp = node;
    tmp->left = new_node(NULL);
    tmp = tmp->left;

    tmp->left = expr_node(); // if condition
    if (!check(dots_, 0))
      ; // TODO: expected ':' after condition

    tmp->right = new_node(NULL);
    tmp = tmp->right;
    while (tokens[exe_pos]->space > node->token->space) // if bloc code
    {
      tmp->left = expr_node();
      tmp->right = new_node(NULL);
      tmp = tmp->right;
    }

    Node *curr = node;
    while (
        check_type((Type[]){elif_, else_, 0}, tokens[exe_pos]->type) &&
        tokens[exe_pos]->space == node->token->space)
    {
      token = tokens[exe_pos++];
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = new_node(token);
      if (token->type == elif_)
      {
        Node *tmp0 = curr->left;
        tmp0->left = expr_node();
        if (!check(dots_, 0))
        {
          error("expected dots");
          exit(1);
        }
        tmp0->right = new_node(NULL);
        tmp0 = tmp0->right;
        while (tokens[exe_pos]->space > token->space)
        {
          tmp0->left = expr_node();
          tmp0->right = new_node(NULL);
          tmp0 = tmp0->right;
        }
      }
      else if (token->type == else_)
      {
        if (!check(dots_, 0))
        {
          error("expected dots");
          exit(1);
        }
        Node *tmp0 = curr->left;

        tmp0->right = new_node(NULL);
        tmp0 = tmp0->right;
        while (tokens[exe_pos]->space > token->space)
        {
          tmp0->left = expr_node();
          tmp0->right = new_node(NULL);
          tmp0 = tmp0->right;
        }
        break;
      }
    }
    /*
    sys.output()
    sys.puts()
    */
  }
  else if ((token = check(while_, 0)))
  {
    node = new_node(token);
    node->left = expr_node();
    if (!check(dots_, 0))
    {
      error("expected :\n");
      exit(1);
    }
    Node *tmp = node;
    while (
        tokens[exe_pos]->type != end_ &&
        tokens[exe_pos]->space > token->space)
    {
      tmp->right = new_node(NULL);
      tmp = tmp->right;
      tmp->left = expr_node();
    }
  }
  else if ((token = check(ret_, 0)))
  {
    // TODO: check return type if is compatible with function
    node = new_node(token);
    node->left = expr_node();
  }
  else if ((token = check(module_, 0)))
  {
    return new_node(token);
  }
  else if (tokens[exe_pos]->type == end_)
    ;
  else
  {
    debug("Prime: Unexpected token <%s>\n", to_string(tokens[exe_pos]->type));
    exit(1);
  }
  return node;
}
#endif

// INSTRUCTIONS
Inst **first_insts;
Inst **insts = NULL;
size_t inst_size;
size_t inst_pos;
size_t reg_pos;

size_t bloc_index;
size_t str_index;
size_t float_index;
// STACK POINTER
long ptr = 0;
size_t arg_ptr;

#if AST
void enter_scoop(char *name)
{
  GLOG("ENTER SCOOP", "%s\n", name);
  if (global_scoop == NULL)
  {
    bloc_size = 10;
    global_scoop = calloc(bloc_size, sizeof(Scoop));
  }
  else if (scoop_pos + 1 == (ssize_t)bloc_size)
  {
    Scoop *tmp = calloc(bloc_size * 2, sizeof(Scoop));
    memcpy(tmp, global_scoop, scoop_pos * sizeof(Scoop));
    bloc_size *= 2;
    free(global_scoop);
    global_scoop = tmp;
  }
  scoop_pos++;
  global_scoop[scoop_pos] = (Scoop){};
  global_scoop[scoop_pos].name = name;
  curr_scoop = &global_scoop[scoop_pos];
}

void exit_scoop()
{
  GLOG(" EXIT SCOOP", "%s\n", curr_scoop->name);
  free(curr_scoop->functions);
  free(curr_scoop->variables);
  free(curr_scoop->structs);
  global_scoop[scoop_pos] = (Scoop){};
  scoop_pos--;
  // if(scoop_pos >= 0)
  curr_scoop = &global_scoop[scoop_pos];
  // else
  // curr_scoop = NULL;
}

#if IR
void add_inst(Inst *inst)
{
  if (first_insts == NULL)
  {
    inst_size = 100;
    first_insts = calloc(inst_size, sizeof(Inst *));
  }
  else if (inst_pos + 1 == inst_size)
  {
    Inst **tmp = calloc(inst_size * 2, sizeof(Inst *));
    memcpy(tmp, first_insts, inst_pos * sizeof(Inst *));
    free(first_insts);
    first_insts = tmp;
    inst_size *= 2;
  }
  first_insts[inst_pos++] = inst;
}

// NAMESPACE
#if 0
Token *get_namespace(char *name) // TODO: maybe you need to use get scoop
{
    return NULL;
}
#endif

// VARIABLES / FUNCTIONS
Token *get_variable(char *name)
{
  CLOG("get var", "%s\n", name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  for (size_t i = 0; i < curr_scoop->var_pos; i++)
  {
    Token *var = curr_scoop->variables[i];
    if (strcmp(var->name, name) == 0)
    {
      GLOG("found", "in [%s] scoop\n", curr_scoop->name);
      return curr_scoop->variables[i];
    }
  }
  error("%s:%d '%s' Not found\n", FUNC, LINE, name);
  exit(1);
  return NULL;
}

Token *new_variable(Token *token)
{
  CLOG("new var", "%s\n", token->name);
  // TODO: check here the global variables
  // Token **variables = curr_scoop->variables;
  CLOG("Scoop", "%s\n", curr_scoop->name);
  for (size_t i = 0; i < curr_scoop->var_pos; i++)
  {
    Token *var = curr_scoop->variables[i];
    if (strcmp(var->name, token->name) == 0)
    {
      debug("%sRedefinition of %s%s\n", RED, token->name, RESET);
      exit(1);
    }
  }
  if (curr_scoop->variables == NULL)
  {
    curr_scoop->var_size = 10;
    curr_scoop->variables = calloc(curr_scoop->var_size, sizeof(Token *));
  }
  else if (curr_scoop->var_pos + 1 == curr_scoop->var_size)
  {
    curr_scoop->var_size *= 2;
    Token **tmp = calloc(curr_scoop->var_size, sizeof(Token *));
    memcpy(tmp, curr_scoop->variables, curr_scoop->var_pos * sizeof(Token *));
    free(curr_scoop->variables);
    curr_scoop->variables = tmp;
  }
  // GLOG("", "in [%s] scoop\n", curr_scoop->name);
  curr_scoop->variables[curr_scoop->var_pos++] = token;
  return token;
}

Node **builtins_functions;
size_t builtins_pos;
size_t builtins_size;

void create_builtin(char *name, Type *params, Type retType)
{
  Node *func = new_node(new_token(name, 0, strlen(name), 0, fdec_));
  func->token->retType = retType;
  func->left = new_node(NULL);
  Node *curr = func->left;
  int i = 0;
  while (params[i])
  {
    curr->right = new_node(NULL);
    curr = curr->right;
    curr->left = new_node(new_token("", 0, 0, 0, params[i]));
    i++;
  }
  if (builtins_functions == NULL)
  {
    builtins_size = 2;
    builtins_functions = calloc(builtins_size, sizeof(Node *));
  }
  else if (builtins_pos + 1 == builtins_size)
  {
    Node **tmp = calloc(builtins_size * 2, sizeof(Node *));
    memcpy(tmp, builtins_functions, builtins_pos * sizeof(Node *));
    free(builtins_functions);
    builtins_size *= 2;
    builtins_functions = tmp;
  }
  builtins_functions[builtins_pos++] = func;
  debug("%screate function [%s]%s\n", GREEN, func->token->name, RESET);
}

Node *get_function(char *name)
{
  // TODO: remove output from here
  CLOG("get func", "%s\n", name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  char *builtins[] = {"output", 0};
  for (int i = 0; builtins[i]; i++)
    if (strcmp(name, builtins[i]) == 0)
      return NULL;
  for (size_t i = 0; i < builtins_pos; i++)
  {
    if (strcmp(name, builtins_functions[i]->token->name) == 0)
      return builtins_functions[i];
  }
  // if(strcmp(name, "write") == 0)
  // {
  //     Node *func = new_node(new_token("write", 0, 5, 0, fdec_));
  //     Node *curr = func;
  //     curr->left = new_node(NULL);
  //     curr = curr->left;
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, int_));
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, chars_));
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, int_));
  //     return func;
  // }
  for (ssize_t j = scoop_pos; j >= 0; j--)
  {
    Scoop *scoop = &global_scoop[j];
    for (size_t i = 0; i < scoop->func_pos; i++)
    {
      Node *func = scoop->functions[i];
      if (strcmp(func->token->name, name) == 0)
        return func;
    }
  }
  error("%s:%d '%s' Not found\n", FUNC, LINE, name);
  exit(1);
  return NULL;
}

// TODO: create a list fo built in functions
Node *new_function(Node *node)
{
  CLOG("new func", "%s\n", node->token->name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  char *builtins[] = {"output", 0};
  for (int i = 0; builtins[i]; i++)
  {
    if (strcmp(node->token->name, builtins[i]) == 0)
    {
      error("%s is a built in function\n", node->token->name);
      exit(1);
    }
  }
  for (size_t i = 0; i < curr_scoop->func_pos; i++)
  {
    Node *func = curr_scoop->functions[i];
    if (strcmp(func->token->name, node->token->name) == 0)
    {
      error("Redefinition of %s\n", node->token->name);
      exit(1);
    }
  }
  if (curr_scoop->functions == NULL)
  {
    curr_scoop->func_size = 10;
    curr_scoop->functions = calloc(curr_scoop->func_size, sizeof(Node *));
  }
  else if (curr_scoop->func_pos + 1 == curr_scoop->func_size)
  {
    curr_scoop->func_size *= 2;
    Node **tmp = calloc(curr_scoop->func_size, sizeof(Node *));
    memcpy(tmp, curr_scoop->functions, curr_scoop->func_pos * sizeof(Node *));
    free(curr_scoop->functions);
    curr_scoop->functions = tmp;
  }
  curr_scoop->functions[curr_scoop->func_pos++] = node;
  return node;
}

Inst *new_inst(Token *token)
{
  debug("new instruction has type %s\n", to_string(token->type));
  Inst *new = calloc(1, sizeof(Inst));
  new->token = token;
  if (token->name && token->declare)
  {
    new_variable(token);
    // if (token->declare)
    if (token->type == struct_)
    {
      // token->ptr = ptr;
      // ptr += token->offset;
      // token->sreg = ++reg_pos;
      token->sreg = ++reg_pos;
    }
    else
    {
      // if(token->isarg)
      //     token->ptr = (arg_ptr += 8);
      // else
      token->ptr = (ptr += sizeofToken(token));

      token->sreg = ++reg_pos;
    }
  }
  else
  {
    switch (token->type)
    {
    case add_:
    case sub_:
    case mul_:
    case div_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
    case not_equal_:
    case fcall_:
      token->sreg = ++reg_pos;
      break;
    default:
      break;
    }
  }
  add_inst(new);
  return new;
}

Token *generate_ir(Node *node)
{
  // debug("gen-ir: %s\n", to_string(node->token->type));
  Inst *inst = NULL;
  switch (node->token->type)
  {
  case id_:
  {
    Token *token = get_variable(node->token->name);
    return token;
    break;
  }
  case module_:
  {
    return node->token;
    break;
  }
  case if_:
  {
    // Node *tmp = node;
    Node *curr = node->left;

    // condition
    // Token *result =
    generate_ir(curr->left); // TODO: check if it's boolean

    node->token->type = jne_;
    node->token->name = strdup("endif");
    node->token->index = ++bloc_index;

    Token *lastInst = copy_token(node->token);
    new_inst(lastInst); // jne to endif

    curr = curr->right;
    while (curr->left) // if code bloc
    {
      generate_ir(curr->left);
      curr = curr->right;
    }

    Inst *endInst = NULL;
    if (node->right)
    {
      endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
      endInst->token->index = node->token->index;
    }

    curr = node->right;
    while (curr)
    {
      if (curr->left->token->type == elif_)
      {
        curr->left->token->index = ++bloc_index;
        curr->left->token->type = bloc_;
        curr->left->token->name = strdup("elif");

        {
          free(lastInst->name);
          lastInst->name = strdup("elif");
          lastInst->index = curr->left->token->index;
          lastInst = copy_token(lastInst);
        }

        new_inst(curr->left->token);
        Node *tmp = curr->left;
        generate_ir(tmp->left); // elif condition, TODO: check is boolean

        new_inst(lastInst);

        tmp = tmp->right;
        while (tmp->left)
        {
          generate_ir(tmp->left);
          tmp = tmp->right;
        }
      }
      else if (curr->left->token->type == else_)
      {
        curr->left->token->index = ++bloc_index;
        curr->left->token->type = bloc_;
        curr->left->token->name = strdup("else");
        new_inst(curr->left->token);

        {
          free(lastInst->name);
          lastInst->name = strdup("else");
          lastInst->index = curr->left->token->index;
          lastInst = copy_token(lastInst);
        }

        Node *tmp = curr->left;
        tmp = tmp->right;
        while (tmp->left)
        {
          generate_ir(tmp->left);
          tmp = tmp->right;
        }
        break;
      }
      if (curr->right)
      {
        endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
        endInst->token->index = node->token->index;
      }
      curr = curr->right;
    }

    Token *new = new_token("endif", 0, 5, node->token->space, bloc_);

    new->index = node->token->index;
    new_inst(new);
    // free_token(lastInst);
    return node->left->token;
    break;
  }
  case while_:
  {
    // condition
    node->token->type = bloc_;
    node->token->name = strdup("while");
    node->token->index = ++bloc_index;
    inst = new_inst(node->token);

    // Token *result =
    generate_ir(node->left); // TODO: check if it's boolean
    Token *end = copy_token(node->token);
    end->type = jne_;
    if (end->name)
      free(end->name);
    end->name = strdup("endwhile");
    new_inst(end);

    Node *curr = node->right;
    while (curr) // if code bloc
    {
      generate_ir(curr->left);
      curr = curr->right;
    }

    Token *lastInst = copy_token(node->token);
    lastInst->type = jmp_;
    free(lastInst->name);
    lastInst->name = strdup("while");
    new_inst(lastInst); // jne to endif

    lastInst = copy_token(node->token);
    lastInst->type = bloc_;
    free(lastInst->name);
    lastInst->name = strdup("endwhile");
    new_inst(lastInst); // jne to endif
    break;
  }
  case fdec_:
  {
    new_function(node);
    enter_scoop(node->token->name);
    Token *fcall = copy_token(node->token);

    size_t tmp_ptr = ptr;
    ptr = 0;

    fcall->type = fdec_;
    inst = new_inst(fcall);
    Node *curr;

    pnode(node, NULL, 0);
    // inside_function = true;
    if (node->left) // arguments
    {
      // TODO: make it compatibel with data type
      char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
      char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
      size_t i = 0;

      // size_t ptr = 8;

      Token **list = NULL;
      size_t list_pos = 0;
      size_t list_size = 0;

      curr = node->left->right;
      while (curr && curr->left)
      {
        Inst *inst = new_inst(new_token(NULL, 0, 0, node->token->space, pop_));
        inst->left = generate_ir(curr->left);
        // debug(RED"pop [%s]\n" RESET, to_string(inst->left->type));
        // exit(1);
        curr->left->token->declare = false;
        // inst->left = curr->left->token;
        if (eregs[i])
        {
          if (inst->left->type == int_)
            inst->right = new_token(eregs[i], 0, strlen(eregs[i]), node->token->space, 0);
          else
            inst->right = new_token(rregs[i], 0, strlen(rregs[i]), node->token->space, 0);
          i++;
        }
        else
        {
          inst->right = new_token(NULL, 0, 0, node->token->space, 0);
          inst->right->ptr = (ptr += 8);
          if (list == NULL)
          {
            list_size = 10;
            list = calloc(list_size, sizeof(Token *));
          }
          else if (list_pos + 1 == list_size)
          {
            Token **tmp = calloc(list_size * 2, sizeof(Token *));
            memcpy(tmp, list, list_pos * sizeof(Token *));
            free(list);
            list_size *= 2;
            list = tmp;
          }
          list[list_pos++] = inst->right;
        }
        // Token *arg = generate_ir(curr->left);
        curr = curr->right;
      }
      i = 0;
      while (i < list_pos / 2)
      {
        size_t tmp = list[i]->ptr;
        list[i]->ptr = list[list_pos - i - 1]->ptr;
        list[list_pos - i - 1]->ptr = tmp;
        i++;
      }
      free(list);
    }
    debug("%s:%d ptr is %zu\n", FUNC, LINE, ptr);
    ptr = tmp_ptr;
    // debug(SPLIT);
    // pnode(node, NULL, 0);
    // debug(SPLIT);
    // pnode(node->right, NULL, 0);
    // debug(SPLIT);
    // exit(1);
    curr = node->right;
    while (curr)
    {
      generate_ir(curr->left);
      curr = curr->right;
    }
    // exit(1);
    Token *new = new_token(NULL, 0, 0, node->token->space, end_bloc_);
    new->name = strdup(node->token->name);
    new->index = node->token->index;
    // new->index = node->token->index;
    new_inst(new);

    exit_scoop();
    break;
  }
  case ret_:
  {
    inst = new_inst(node->token);
    inst->left = generate_ir(node->left);
    break;
  }
  case fcall_:
  {
    // pnode(node, NULL, 0);
    size_t tmp_arg_ptr = arg_ptr;
    size_t tmp_ptr = ptr;
    arg_ptr = 8;
    ptr = 8; // TODO: to be checked
    if (strcmp(node->token->name, "output") == 0)
    {
      Node *curr = node;
      char *fname = NULL;
      char *regname = NULL;
#if 0
            while(curr->left)
            {
                pnode(curr->left, NULL, 0);
                curr = curr->right;
            }
#endif
      // exit(1);
      curr = node;
      while (curr->left)
      {
        // debug("loop\n");
        // Node *arg = curr->left;
        Token *left = generate_ir(curr->left);
        fname = NULL;
        switch (left->type)
        {
        // TODO: add other types / maybe you will remove it
        case chars_:
          fname = "putstr";
          regname = "rdi";
          break;
        case int_:
          fname = "putnbr";
          regname = "edi";
          break;
        default:
          error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type));
          exit(1);
        }
        if (fname)
        {
          inst = new_inst(new_token(NULL, 0, 0, node->token->space, push_));
          // TODO: it causes problem in output("hello world")
          // TODO: remove use register depends on data type
          // left = copy_token(left);
          left->declare = false;
          inst->left = left;
          inst->right = new_token(regname, 0, strlen(regname), node->token->space, 0);
          // new_inst(left);
          inst = new_inst(new_token(fname, 0, strlen(fname), node->token->space, fcall_));
          inst->token->isbuiltin = true;
          inst->left = left;
        }
        curr = curr->right;
      }
    }
    else
    {
      Node *func = get_function(node->token->name);
      node->token->retType = func->token->retType;
      Node *arg = func->left->right;
      debug("has the following arguments\n");
      while (arg)
      {
        ptoken(arg->left->token);
        arg = arg->right;
      }
      // TODO: make it compatibel with data type
      char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
      char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
      int i = 0;
      int j = 0;
      int k = 0;
      // size_t ptr = 8;

      Node *curr = node;
      arg = func->left->right;
      while (curr->left && arg)
      {
        Token *left = generate_ir(curr->left);
        Inst *inst = new_inst(new_token(NULL, 0, 0, node->token->space, push_));
        inst->left = left;
        // debug("%s => %s\n",
        // to_string(inst->left->type),
        // to_string(arg->left->token->type));
        if (
            inst->left->type != arg->left->token->type &&
            inst->left->retType != arg->left->token->type &&
            !(inst->left->type == chars_ && arg->left->token->type == ptr_))
        {
          error("%s:%d Incompatible type for function call <%s>\n", FUNC, LINE, func->token->name);
          // TODO: add line after
          exit(1);
        }
        if (eregs[i])
        {
          if (inst->left->type == int_)
            inst->right = new_token(eregs[i], 0, strlen(eregs[i]), node->token->space, 0);
          else
            inst->right = new_token(rregs[i], 0, strlen(rregs[i]), node->token->space, 0);
          i++;
        }
        else
          inst->right = new_token(NULL, 0, 0, node->token->space, 0);
        curr = curr->right;
        arg = arg->right;
        j++;
      }
      if (arg)
      {
        k = j;
        while (arg)
        {
          arg = arg->right;
          k++;
        }
        error(
            "function [%s] takes %d argument(s) but receive %d\n", func->token->name, k, j);
      }
      else if (curr->left)
      {
        k = j;
        while (curr->left)
        {
          curr = curr->right;
          k++;
        }
        error(
            "function [%s] takes %d argument(s) but receive %d\n", func->token->name, j, k);
      }
      new_inst(node->token);
    }
    arg_ptr = tmp_arg_ptr;
    ptr = tmp_ptr;
    return node->token;
    break;
  }
  case dot_:
  {
    // TODO: use namespaces here
    debug("found dot\n");
    Token *left = generate_ir(node->left);
    if (left->type == module_)
    {
      // is module
      // TODO: module must have a valid name like "sys"
      debug("left is module\n");
      // Token *right =
      generate_ir(node->right);

      // exit(1);
    }
    else
    {
      // is attribute or method
      error("handle this case in dot\n");
    }
    return left;
    // exit(1);
    break;
  }
  case bool_:
  case int_:
  case chars_:
  case char_:
  case float_:
  case void_:
  {
    inst = new_inst(node->token);
    break;
  }
  case assign_:
  case add_:
  case sub_:
  case div_:
  case mul_:
  case not_equal_:
  case equal_:
  case less_:
  case more_:
  case less_equal_:
  case more_equal_:
  {
    Token *left = generate_ir(node->left);
    Token *right = generate_ir(node->right);
    // TODO: fix this later for cases like char + int
    if (
        left->type != right->type && left->type != right->retType &&
        !(left->type == chars_ && right->retType == ptr_))
    {
      debug("%s:%d left[%s], right [%s]\n", FUNC, LINE, to_string(left->retType), to_string(right->type));
      error("%s:%d Incompatible type for [%s] and [%s]", FUNC, LINE,
            to_string(left->type), to_string(right->type));
      exit(1);
    }
    inst = new_inst(node->token);
    inst->left = left;
    inst->right = right;
    switch (node->token->type)
    {
    case assign_:
      node->token->retType = left->type;
      break;
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
      if (left->type != int_ && left->type != float_)
      {
        error("Invalid operation [%s] between [%s] and [%s]\n",
              to_string(node->token->type), to_string(left->type), to_string(right->type));
        exit(1);
      }
      node->token->retType = left->type;
      if (right->type == int_)
        node->token->creg = "eax";
      else if (right->type == float_)
        node->token->creg = "xmm0";
      break;
    }
    case not_equal_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
      node->token->retType = bool_;
      node->token->index = ++bloc_index;
      debug(RED "this is [%d]\n" RESET, node->token->sreg);
      break;
    default:
      break;
    }
    // inst->token->type = left->type; // TODO: to be checked
    break;
  }
  case and_:
  case or_:
  {
    // TODO: check if left nad right are type is bool or retType is bool
    Token *left = generate_ir(node->left);

    node->token->index = ++bloc_index;
    Token *end = copy_token(node->token);
    if (node->token->type == and_)
    {
      end->type = jne_;
      if (end->name)
        free(end->name);
      end->name = strdup(to_string(node->token->type));
      new_inst(end);
    }
    else if (node->token->type == or_)
    {
      end->type = je_;
      if (end->name)
        free(end->name);
      end->name = strdup(to_string(node->token->type));
      new_inst(end);
    }
    Token *right = generate_ir(node->right);
    end = copy_token(end);
    end->type = bloc_;
    new_inst(end);

    node->token->type = bool_;
    node->token->creg = "al";

    return node->token;
    break;
  }
  case struct_:
  {
    if (node->token->declare)
    {
      inst = new_inst(node->token);
      Node *curr = node->left->right;

      while (curr)
      {
        curr->left->token->declare = false;
        generate_ir(curr->left);
        curr->left->token->ptr = node->token->offset -
                                 (ptr + curr->left->token->offset);
        curr->left->token->declare = true;
        curr = curr->right;
      }
      new_inst(new_token(inst->token->name, 0,
                         strlen(inst->token->name), inst->token->space, end_struct_));
      debug(RED "struct offset %zu\n" RESET, node->token->offset);
      ptr += node->token->offset;
    }
    else
      return node->token;
    break;
  };
  default:
  {
    error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(node->token->type));
    exit(1);
    break;
  }
  }
  return inst->token;
}

void print_ir()
{
  debug(SPLIT);
  int j = 0;
  for (int i = 0; insts && insts[i]; i++)
  {
    Token *curr = insts[i]->token;
    Token *left = insts[i]->left;
    Token *right = insts[i]->right;
    if (curr->remove)
    {
      error("%s:%d this condtion must never exists\n", FUNC, LINE);
      exit(1);
      continue;
    }
    switch (curr->type)
    {
    case assign_:
    {
      curr->sreg = left->sreg;
      debug("r%.2d: %s [%s] ", curr->sreg, to_string(curr->type), to_string(left->type));
      debug("%s in (%d) to ", left->name, left->sreg);
      if (right->sreg)
        debug("r%.2d", right->sreg);
      else
      {
        switch (right->type)
        { // TODO: handle the other cases
        case int_:
          debug("%lld", right->Int.value);
          break;
        case bool_:
          debug("%s", right->Bool.value ? "True" : "False");
          break;
        case float_:
          debug("%f", right->Float.value);
          break;
        case char_:
          debug("%c", right->Char.value);
          break;
        case chars_:
          debug("%s", right->Chars.value);
          break;
        default:
          error("%s:%d: handle this case\n", FUNC, LINE);
          exit(1);
          break;
        }
      }
      debug("\n");
      break;
    }
    case fcall_:
    {
      debug("r%.2d: call %s\n", curr->sreg, curr->name);
      break;
    }
    case add_:
    case sub_:
    case mul_:
    case div_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
    case not_equal_:
    {
      // TODO: set invalid operation for boolean type
      debug("r%.2d: %s ", curr->sreg, to_string(curr->type));
      if (left->sreg)
        debug("r%.2d", left->sreg);
      else
        switch (left->type)
        {
        case int_:
          debug("%lld", left->Int.value);
          break;
        case char_:
          debug("%c", left->Char.value);
          break;
        case chars_:
          debug("%s", left->Chars.value);
          break;
        case float_:
          debug("%f", left->Float.value);
          break;
        default:
          break;
        }

      if (left->name)
        debug(" (%s)", left->name);
      debug(" to ");
      if (right->sreg)
        debug("r%.2d", right->sreg);
      else
        switch (right->type)
        {
        case int_:
          debug("%lld", right->Int.value);
          break;
        case char_:
          debug("%c", right->Char.value);
          break;
        case chars_:
          debug("%s", right->Chars.value);
          break;
        case float_:
          debug("%f", right->Float.value);
          break;
          ;
        default:
          break;
        }

      if (right->name)
        debug(" (%s)", right->name);
      debug("\n");
      break;
    }
    case int_:
    case char_:
    case bool_:
    case chars_:
    case float_:
    {
      debug("r%.2d: [%s] ", curr->sreg, to_string(curr->type));
      if (curr->declare)
        debug("declare %s PTR=[%zu]", curr->name, curr->ptr);
      else if (curr->name)
        debug("variable %s ", curr->name);
      else if (curr->type == int_)
        debug("value %lld ", curr->Int.value);
      else if (curr->type == char_)
        debug("value %c ", curr->Char.value);
      else if (curr->type == bool_)
        debug("value %s ", curr->Bool.value ? "True" : "False");
      else if (curr->type == float_)
      {
        curr->index = ++float_index;
        debug("value %f ", curr->Float.value);
      }
      else if (curr->type == chars_)
      {
        curr->index = ++str_index;
        debug("value %s in STR%zu ", curr->Chars.value, curr->index);
      }
      else
        error("handle this case in generate ir line %d\n", LINE);
      // if(curr->isarg)
      //     debug(" [argument]");
      debug("\n");
      break;
    }
    case void_:
    {
      debug("r%.2d: [%s]\n", curr->sreg, to_string(curr->type));
      break;
    }
    case struct_:
    {
      debug("r%.2d: [%s] ", curr->sreg, to_string(curr->type));
      if (curr->declare)
        debug("declare %s PTR=[%zu]", curr->name, curr->ptr);
      debug("\n");
      break;
    }
    case push_:
    {
      // TODO: check all cases
      debug("rxx: push ");
      // if(left->ptr)
      debug("PTR r%.2d ", left->sreg);
      if (right->name)
        debug("to %s", right->name);
      debug("\n");
      break;
    }
    case pop_:
    {
      // TODO: check all cases
      debug("rxx: pop ");
      if (left->ptr)
        debug("PTR [%zu] ", left->ptr);
      debug("from ");
      if (right->name)
        debug("%s", right->name);
      else
        debug("[%zu]", right->ptr);
      debug("\n");
      break;
    }
    /*
        TODO:
            + if function has datatype must have return
            + return value must be compatible with function
    */
    case ret_:
    {
      debug("rxx: return ");
      ptoken(left);
      break;
    }
    case jne_:
      debug("rxx: jne %s%zu\n", curr->name, curr->index);
      break;
    case je_:
      debug("rxx: je %s%zu\n", curr->name, curr->index);
      break;
    case jmp_:
      debug("rxx: jmp %s%zu\n", curr->name, curr->index);
      break;
    case bloc_:
    case end_bloc_:
    case end_struct_:
    case fdec_:
      debug("%s%zu: [%s]\n", curr->name, curr->index, to_string(curr->type));
      break;
    default:
      debug("%sPrint IR: Unkown inst [%s]%s\n", RED, to_string(curr->type), RESET);
      break;
    }
    j++;
  }
  debug("%.2d Instructions on total\n", j);
  debug(SPLIT);
}

bool optimize_ir(int op_index)
{
#if 1
  bool did_optimize = false;
  switch (op_index)
  {
  case 0:
  {
    debug("OPTIMIZATION %d (calculate operations on constant type 0)\n", op_index);
    for (int i = 0; insts[i]; i++)
    {
      Token *token = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      if (check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i]->token->type))
      {
        if (
            // TODO: check it left nad right are compatible
            check_type((Type[]){int_, float_, chars_, 0}, left->type) &&
            check_type((Type[]){int_, float_, chars_, 0}, right->type) &&
            !left->name && !right->name)
        {

          switch (left->type)
          {
          case int_:
            switch (token->type)
            {
            case add_:
              token->Int.value = left->Int.value + right->Int.value;
              break;
            case sub_:
              token->Int.value = left->Int.value - right->Int.value;
              break;
            case mul_:
              token->Int.value = left->Int.value * right->Int.value;
              break;
            case div_:
              token->Int.value = left->Int.value / right->Int.value;
              break;
            default:
              break;
            }
            break;
          case float_:
            left->index = 0;
            right->index = 0;
            token->index = ++float_index;
            switch (token->type)
            {
            case add_:
              token->Float.value = left->Float.value + right->Float.value;
              break;
            case sub_:
              token->Float.value = left->Float.value - right->Float.value;
              break;
            case mul_:
              token->Float.value = left->Float.value * right->Float.value;
              break;
            case div_:
              token->Float.value = left->Float.value / right->Float.value;
              break;
            default:
              break;
            }
          // case chars_:
          //     switch(token->type)
          //     {
          //     case add_: token->Chars.value = strjoin(left, right); break;
          //     default:
          //         error("Invalid %s op in chars\n", to_string(token->type)); break;
          //     }
          default:
            error("%s: %d handle this case\n", FUNC, LINE);
            break;
          }
          token->type = left->type;
          left->remove = true;
          right->remove = true;
          insts = copy_insts(first_insts, insts, inst_pos, inst_size);
          i = 0;
          did_optimize = true;
        }
      }
      if (check_type((Type[]){int_, float_, chars_, bool_, char_, 0}, token->type) && !token->name)
      {
        // token->c = 0;
        // free(token->creg);
        token->creg = NULL;
        // debug("found to remove in r%d\n", token->r1);
        token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 0;
        did_optimize = true;
      }
    }
    break;
  }
  case 1:
  {
    // TODO: do comparision operation on numbers etc...
    debug("OPTIMIZATION %d (calculate operations on numbers type 1)\n", op_index);
    int i = 1;
    while (insts[i])
    {
      Token *token = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      if (
          //  TODO: handle string also here X'D ma fiyach daba
          check_type((Type[]){add_, sub_, mul_, div_, 0}, token->type) &&
          insts[i - 1]->token->type == add_ &&
          left == insts[i - 1]->token &&
          !insts[i - 1]->right->name &&
          !right->name)
      {
        // debug("%sfound %s\n", RED, RESET);
        token->remove = true;
        switch (token->type)
        {
        case add_:
          insts[i - 1]->right->Int.value += right->Int.value;
          break;
        case sub_:
          insts[i - 1]->right->Int.value -= right->Int.value;
          break;
        case mul_:
          insts[i - 1]->right->Int.value *= right->Int.value;
          break;
        case div_:
          insts[i - 1]->right->Int.value /= right->Int.value;
          break;
        default:
          break;
        }
        // debug("value is %lld\n", insts[i - 1]->right->Int.value);
        if (insts[i + 1]->left == token)
          insts[i + 1]->left = insts[i - 1]->token;
        i = 1;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        did_optimize = true;
        continue;
      }
      i++;
    }
    break;
  }
  case 2:
  {
    debug("OPTIMIZATION %d (remove reassigned variables)\n", op_index);
    for (int i = 0; insts[i]; i++)
    {
      if (insts[i]->token->declare)
      {
        int j = i + 1;
        while (insts[j] && insts[j]->token->space == insts[i]->token->space)
        {
          debug("line %d: ", LINE);
          ptoken(insts[j]->token);
          if (insts[j]->token->type == assign_ && insts[j]->left == insts[i]->token)
          {
            insts[i]->token->declare = false;
            insts[i]->token->remove = true;
            did_optimize = true;
            break;
          }
          if (
              (insts[j]->left && insts[j]->left->sreg == insts[i]->token->sreg) ||
              (insts[j]->right && insts[j]->right->sreg == insts[i]->token->sreg))
            break;
          j++;
        }
      }
      else if (insts[i]->token->type == assign_)
      {
        int j = i + 1;
        while (insts[j] && insts[j]->token->space == insts[i]->token->space)
        {
          if (!insts[j]->left || !insts[j]->right || !insts[i]->token)
          {
            j++;
            continue;
          }
          if (
              insts[j]->token->type == assign_ &&
              insts[j]->left == insts[i]->left)
          {
            insts[i]->token->remove = true;
            did_optimize = true;
            break;
          }
          // if the variable is used some where
          else if (
              insts[j]->left->sreg == insts[i]->token->sreg ||
              insts[j]->right->sreg == insts[i]->token->sreg)
            break;
          j++;
        }
      }
    }
    break;
  }
  case 3:
  {
    for (int i = 1; insts[i]; i++)
    {
      if (insts[i]->token->type == ret_ && insts[i - 1]->token->type == ret_)
      {
        did_optimize = true;
        insts[i]->token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 1;
      }
    }
    break;
  }
  case 4:
  {
    debug("OPTIMIZATION %d (remove unused instructions)\n", op_index);
    for (int i = 1; insts[i]; i++)
    {
      if (
          check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i - 1]->token->type) &&
          insts[i]->left->sreg != insts[i - 1]->token->sreg &&
          insts[i]->right->sreg != insts[i - 1]->token->sreg)
      {
        did_optimize = true;
        insts[i - 1]->token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 1;
      }
    }
    break;
  }
  default:
    break;
  }
  return did_optimize;
#endif
}

#endif
#endif

void initialize()
{
  pasm(".intel_syntax noprefix\n");
  pasm(".include \"./import/header.s\"\n\n");
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
    if (curr->type == chars_ && !curr->name && !curr->ptr && curr->index)
      pasm(".STR%zu: .string %s\n", curr->index, curr->Chars.value);
    if (curr->type == float_ && !curr->name && !curr->ptr && curr->index)
      pasm(".FLT%zu: .long %zu /* %f */\n", curr->index,
           *((uint32_t *)(&curr->Float.value)), curr->Float.value);
  }
  pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
#endif
}

/*
TODOS:
    + in assign:
        + check incompatible type
        + left should be always an identifier
*/

void generate_asm()
{
  initialize();
  for (size_t i = 0; insts[i]; i++)
  {
    Token *curr = insts[i]->token;
    Token *left = insts[i]->left;
    Token *right = insts[i]->right;

    switch (curr->type)
    {
    case int_:
    case bool_:
    case chars_:
    case char_:
    case float_:
    {
      if (curr->declare)
      {
        pasm("//declare [%s]\n", curr->name);
        pasm("mov %a, 0\n", curr);
      }
      // else if(curr->isarg)
      // error("%s:%d handle this case\n", FUNC, LINE);
      break;
    }
    case assign_:
    {
      curr->ptr = left->ptr;
      pasm("//assign [%s]\n", left->name);
      if (right->ptr || right->creg)
      {
        char *inst = left->type == float_ ? "movss " : "mov ";
        if (right->ptr && !right->creg)
          pasm("%i%r, %a\n", inst, left, right);
        pasm("%i%a, %r\n", inst, left, left);
      }
      else
      {
        switch (right->type)
        {
        case int_:
        case bool_:
        case char_:
          pasm("mov %a, %v\n", left, right);
          break;
        case chars_:
          pasm("lea %r, .STR%zu[rip]\n", right, right->index);
          pasm("mov %a, %r\n", left, left);
          break;
        case float_:
          pasm("movss %r, DWORD PTR .FLT%zu[rip]\n", right, right->index);
          pasm("movss %a, %r\n", left, left);
          break;
        default:
          error("handle this case in %s:%d\n", FUNC, LINE);
          break;
        }
      }
      break;
    }
    case add_:
    case sub_:
    case mul_:
    case div_: // TODO: check all math_op operations
    {
      // TODO: use rax for long etc...
      Type type = curr->type;
      char *inst = left->type == float_ ? "movss" : "mov";

      if (left->ptr)
        pasm("%i%r, %a\n", inst, curr, left);
      else if (left->creg && strcmp(left->creg, right->creg))
        pasm("%i%r, %r\n", inst, curr, left);
      else if (!left->creg)
        pasm("%i%r, %v\n", inst, curr, left);

      switch (curr->type)
      {
      case add_:
        inst = left->type == float_ ? "addss " : "add ";
        break;
      case sub_:
        inst = left->type == float_ ? "subss " : "sub ";
        break;
      case mul_:
        inst = left->type == float_ ? "imulss " : "imul ";
        break;
      case div_:
        inst = left->type == float_ ? "divss " : "div ";
        break;
      default:
        break;
      }
      if (right->ptr)
        pasm("%i%r, %a\n", inst, curr, right);
      else if (right->creg)
        pasm("%i%r, %r\n", inst, curr, right);
      else if (!right->creg)
        pasm("%i%r, %v\n", inst, curr, left);
      curr->type = left->type;
      break;
    }
    case equal_:
    case not_equal_:
    case less_:
    case less_equal_:
    case more_:
    case more_equal_:
    {
      char *inst = left->type == float_ ? "movss" : "mov";
      if (left->ptr)
        pasm("%i%r, %a\n", inst, left, left);
      else if (left->creg /*&& strcmp(left->creg, r->creg)*/)
        pasm("%i%r, %r\n", inst, left, left);
      else if (!left->creg)
        pasm("%i%r, %v\n", inst, left, left);

      char *reg = NULL;

#define setCase(case_, var, value) \
  case case_:                      \
    var = value;                   \
    break;
      switch (left->type)
      {
        setCase(int_, reg, "ebx");
        setCase(float_, reg, "xmm1");
        setCase(char_, reg, "bl");
        setCase(bool_, reg, "ebx");
      default:
        error("%s: Unkown type [%s]\n", FUNC, to_string(left->type));
        break;
      }
      if (right->ptr)
        pasm("%i%s, %a\n", inst, reg, right);
      else if (right->creg)
        pasm("%i%s, %r\n", inst, reg, right);
      else if (!right->creg)
        pasm("%i%s, %v\n", inst, reg, right);
      inst = left->type == float_ ? "ucomiss" : "cmp";
      pasm("%i%r, %s\n", inst, left, reg);
      switch (curr->type)
      {
        setCase(equal_, inst, "sete");
        setCase(not_equal_, inst, "setne");
        setCase(less_, inst, "setl");
        setCase(less_equal_, inst, "setle");
        setCase(more_, inst, "setg");
        setCase(more_equal_, inst, "setge");
      default:
        error("%s: Unkown type [%s]\n", FUNC, to_string(left->type));
        break;
      }
      curr->retType = bool_;
      curr->creg = "al";
      pasm("%i%r\n", inst, curr);
      break;
    }
    case fdec_:
    {
      pasm("%s:\n", curr->name);
      pasm("push rbp\n");
      pasm("mov rbp, rsp\n");
      pasm("sub rsp, %zu\n", (((ptr) + 15) / 16) * 16);
      break;
    }
    case fcall_:
    {
      pasm("%i%s\n", "call", curr->name);
      break;
    }
    case push_:
    {
      if (right->name)
      {
        if (left->ptr)
          pasm("mov %s, %a\n", right->name, left);
        else if (left->creg)
          pasm("mov %s, %r\n", right->name, left);
        else
        {
          switch (left->type)
          {
          case int_:
          case bool_:
          case char_:
            pasm("mov %s, %v\n", right->name, left);
            break;
          case chars_:
            pasm("lea %r, .STR%zu[rip]\n", left, left->index);
            pasm("mov %s, %r\n", right->name, left);
            break;
          case float_:
            pasm("movss %r, DWORD PTR .FLT%zu[rip]\n", left, left->index);
            pasm("movss %s, %r\n", right->name, left);
            break;
          default:
            error("handle this case in %s:%d\n", FUNC, LINE);
            break;
          }
          // pasm("mov %s, %v\n", right->name, left);
          // error("%s:%d handle this case\n");
        }
      }
      else
      {
        if (left->ptr)
        {
          pasm("mov %r, %a\n", left, left);
          pasm("push rax\n");
        }
        else if (left->creg)
        {
          pasm("mov %r, %a\n", left, left);
          pasm("push rax\n");
        }
        else
        {
          error("%s:%d handle this case\n", FUNC, LINE);
        }
      }
      break;
    }
    case pop_:
    {
      if (right->name)
      {
        pasm("//pop [%s]\n", left->name);
        if (left->ptr)
        {
          switch (left->type)
          {
          case int_:
            pasm("mov DWORD PTR -%zu[rbp], %s\n", left->ptr, right->name);
            break;
          default:
          {
            error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type));
            exit(1);
            break;
          }
          }
        }
        else
        {
          error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type));
          exit(1);
        }
      }
      else
      {

        // error("%s:%d handle this case\n", FUNC, LINE);
      }
      break;
    }
    case ret_:
    {
      if (left->ptr)
      {
        // mov("", ""), pasm("%s, ", defaultReg(left)), paddr(left), pasm("\n");
        pasm("mov %r, %a\n", left, left);
      }
      else
      {
        switch (left->type)
        {
        case int_:
          pasm("mov %r, %ld\n", left, left->Int.value);
          break;
        case void_:
          pasm("mov eax, 0\n");
          break;
        default:
        {
          error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type));
          exit(1);
          break;
        }
        }
      }
      pasm("leave\n");
      pasm("ret\n");
      break;
    }
    case je_:
    {
      pasm("%ial, 1\n", "cmp");
      pasm("%i.%s%zu\n", "je", curr->name, curr->index);
      // curr->creg = "al";
      break;
    }
    case jne_:
    {
      pasm("%ial, 1\n", "cmp");
      pasm("%i.%s%zu\n", "jne", curr->name, curr->index);
      // curr->creg = "al";
      break;
    }
    case jmp_:
    {
      pasm("%i.%s%zu\n", "jmp", curr->name, curr->index);
      break;
    }
    case bloc_:
    {
      pasm(".%s%zu:\n", curr->name, curr->index);
      break;
    }
    case end_bloc_:
    {
      pasm(".end%s:\n", curr->name);
      break;
    }
    case struct_:
    case end_struct_:
    case void_:
      break;
    default:
      error("%s: Unkown type [%s]\n", FUNC, to_string(curr->type));
      break;
      break;
    }
  }
  finalize();
}

/*
    TODOS:
        + don't exit till you compile full file
        + function if doesn't have a return, set 0 as default
          return for in for example
*/

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    error("expected file as argument\n");
    exit(1);
  }
  char *input = open_file(argv[1]);
  char *outputFile = strdup(argv[1]);
  outputFile[strlen(outputFile) - 1] = 's';

  asm_fd = fopen(outputFile, "w+");
  if (asm_fd == NULL)
  {
    error("openning %s\n", outputFile);
    exit(1);
  }
  free(outputFile);

#if TOKENIZE
  tokenize(input);
  for (size_t i = 0; i < tk_pos; i++)
    ptoken(tokens[i]);
  debug(SPLIT);
#endif

  Node *head = NULL;
#if AST
  enter_scoop("");
  head = new_node(NULL);
  Node *curr = head;
  curr->left = expr_node();
  while (curr->left)
  {
    curr->right = new_node(NULL);
    curr = curr->right;
    curr->left = expr_node();
  }
  curr = head;
  while (curr->left)
  {
    pnode(curr->left, NULL, 0);
    curr = curr->right;
  }
  if (tokens)
  {
#if IR
#if BUILTINS
    create_builtin("write", (Type[]){int_, chars_, int_, 0}, int_);
    create_builtin("read", (Type[]){int_, chars_, int_, 0}, int_);
    create_builtin("exit", (Type[]){int_, 0}, int_);
    create_builtin("malloc", (Type[]){int_, 0}, ptr_);
    create_builtin("calloc", (Type[]){int_, int_, 0}, ptr_);
    create_builtin("strdup", (Type[]){chars_, 0}, chars_);
    create_builtin("strlen", (Type[]){chars_, 0}, int_);
    create_builtin("free", (Type[]){ptr_, 0}, void_);
    create_builtin("strcpy", (Type[]){chars_, chars_, 0}, chars_);
    create_builtin("strncpy", (Type[]){chars_, chars_, int_, 0}, chars_);
    create_builtin("puts", (Type[]){chars_, 0}, int_);
    create_builtin("putstr", (Type[]){chars_, 0}, int_);
    create_builtin("putchar", (Type[]){char_, 0}, int_);
    create_builtin("putnbr", (Type[]){int_, 0}, int_);
    create_builtin("putbool", (Type[]){bool_, 0}, int_);
    create_builtin("putfloat", (Type[]){float_, 0}, int_);
#endif
    curr = head;
    while (curr->left)
    {
      generate_ir(curr->left);
      curr = curr->right;
    }
    exit_scoop();
    if (!found_error)
    {
      insts = copy_insts(first_insts, insts, inst_pos, inst_size);
      print_ir();
#if OPTIMIZE
      int i = 0;
      bool optimized = false;
      while (i < MAX_OPTIMIZATION)
      {
        optimized = optimize_ir(i++) || optimized;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        print_ir();
        if (i == MAX_OPTIMIZATION && optimized)
        {
          optimized = false;
          i = 0;
        }
      }
#endif
#endif

#if ASM
      generate_asm();
#endif
    }
  }
#endif
  clear(head, tokens, first_insts, input);
#if ASM
  return found_error;
#else
  return 1;
#endif
}