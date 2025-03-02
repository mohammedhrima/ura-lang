#include "header.h"

Specials *specials = (Specials[]) {
   {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
   {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
   {"==", EQUAL}, {"is", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
   {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
   {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
   {",", COMA}, {"if", IF}, {"elif", ELIF}, {"else", ELSE},
   {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
   {"and", AND}, {"&&", AND}, {"or", OR}, {"||", OR},
   {0, (Type)0}
};

Specials *dataTypes = (Specials[]) {
   {"int", INT}, {"bool", BOOL}, {"func", FDEC}, {"chars", CHARS},
   {0, (Type)0},
};

// UTILS
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
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return;
   found_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "%sError:%s:%s:%d %s", RED, filename, funcname, line, RESET);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   exit(1);
}

void ptoken(Token *token)
{
   if (!DEBUG) return;
   debug("token: space [%.2d] [%s] ", token->space, to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:
   {
      if (token->name) debug("name [%s] ", token->name);
      if (token->declare) debug("[declare] ");
      if (!token->name && !token->declare)
      {
         if (token->type == INT) debug("value [%lld] ", token->Int.value);
         else if (token->type == CHARS) debug("value [%s] ", token->Chars.value);
         else if (token->type == CHAR) debug("value [%c] ", token->Char.value);
         else if (token->type == BOOL) debug("value [%d] ", token->Bool.value);
         else if (token->type == FLOAT) debug("value [%f] ", token->Float.value);
         // else if(token->type == CHARS)
         //     debug(" value [%s]", token->Chars.value);
         // else if(token->type == BOOL)
         //     debug(" value [%d]", token->Bool.value);
      }
      break;
   }
   case FCALL: case FDEC: case ID:
      debug("name [%s] ", token->name);
      break;
   default:
   {
      // for (int i = 0; specials[i].value; i++)
      // {
      //     if (specials[i].type == token->type)
      //     {
      //         debug("[%s]", specials[i].value);
      //         break;
      //     }
      // }
      break;
   }
   }
   token->remove ? debug(" [remove]\n") : debug("\n");
}

void *allocate_func(size_t line, size_t len, size_t size)
{
   void *ptr = calloc(len, size);
   check(!ptr, "allocate did failed in line %zu\n", line);
   return ptr;
}

#define allocate(len, size) allocate_func(LINE, len, size)

char *open_file(char *filename)
{
   struct _IO_FILE *file = fopen(filename, "r");
   check(!file, "failed to open file %s\n", filename);
   fseek(file, 0, SEEK_END);
   size_t size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

// TOKENIZE
Token **tokens;
void add_token(Token *token)
{
   static size_t pos;
   static size_t len;
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

Token *new_token(char *input, size_t s, size_t e, Type type, size_t space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   switch (type)
   {
   case INT:
   {
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   case BLOC: case ID: case JMP: case JE: case JNE: case FDEC:
   {
      if (e > s)
      {
         new->name = allocate(e - s + 1, sizeof(char));
         strncpy(new->name, input + s, e - s);
         if (strcmp(new->name, "True") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = true;
         }
         else if (strcmp(new->name, "False") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = false;
         }
      }
      break;
   }
   case CHARS:
   {
      if (e > s)
      {
         new->Chars.value = allocate(e - s + 1, sizeof(char));
         strncpy(new->Chars.value, input + s, e - s);
      }
      break;
   }
   case CHAR:
   {
      if (e > s) new->Char.value = input[s];
      break;
   }
   default:
      break;
   }
   add_token(new);
   return new;
}

void setName(Token *token, char *name);
Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   // TODO: check all values that can be copied example: name ...
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   add_token(new);
   return new;
}

void tokenize(char *input)
{
   if (!TOKENIZE) return;
   debug(GREEN "=========== TOKENIZE ===========\n" RESET);
   // size_t len = 10;
   // size_t pos = 0;
   size_t i = 0;
   size_t space = 0;
   bool inc_space = true;
   while (input[i] && !found_error)
   {
      size_t s = i;
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
            if (strcmp(specials[j].value, ":") == 0) space++;
            break;
         }
      }
      for (int j = 0; !found && dataTypes[j].value; j++)
      {
         if (strncmp(dataTypes[j].value, input + i, strlen(dataTypes[j].value)) == 0)
         {
            Token *token = new_token(NULL, 0, 0, dataTypes[j].type, space);
            token->declare = true;
            setName(token, NULL);
            found = true;
            i += strlen(dataTypes[j].value);
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
   for (size_t i = 0; tokens[i]; i++) ptoken(tokens[i]);
   check(!tokens, "No token generated");
}

void free_token(Token *token)
{
   if (token->name) free(token->name);
   if (token->creg) free(token->creg);
   if (token->Chars.value) free(token->Chars.value);
   free(token);
}

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

size_t exe_pos;
Token *find(Type type, ...)
{
   va_list ap;
   va_start(ap, type);
   while (type)
   {
      if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};

void pnode(Node *node, char *side, int space)
{
   if (node && DEBUG)
   {
      int i = 0;
      while (i < space) i += printf(" ");
      if (side) debug("%snode: ", side ? side : "");
      if (node->token) ptoken(node->token);
      else debug("(NULL)\n");
      pnode(node->left, "L:", space + 2);
      pnode(node->right, "R:", space + 2);
   }
}

Node *new_node(Token *token)
{
   Node *new = allocate(1, sizeof(Node));
   new->token = token;
   // pnode(new, NULL, 0);
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

void free_node(Node *node)
{
   if (node)
   {
      free_node(node->left);
      free_node(node->right);
      free(node);
   }
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
      Node *right = logic();
      // if(token->type != ASSIGN)
      // {
      //   // check(1, "check this");
      //   // Node *tmp = copy_node(node);
      //   // tmp->token->ptr = 0;
      //   // tmp->token->creg = 0;
      //   Type math;
      //   switch(token->type)
      //   {
      //     case ADD_ASSIGN: math = ADD; break;
      //     case SUB_ASSIGN: math = SUB; break;
      //     case MUL_ASSIGN: math = MUL; break;
      //     case DIV_ASSIGN: math = DIV; break;
      //     default: check(1, "handle this case [%s]", to_string(token->type)); break;
      //   }
      //   Node *math_node = new_node(new_token(NULL, 0, 0, math, token->space));
      //   math_node->left = copy_node(left);
      //   math_node->right = right;

      //   node->token->type = ASSIGN;
      //   right = math_node;

      //   // tmp->left = copy_node(left);
      //   // tmp->left->token->declare = false;
      //   // tmp->right = right;
      //   // right = tmp;
      // }
      node->left = left;
      node->right = right;
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
      left = node;
   }
   return left;
}

Node *dot()
{
   return sign();
}

Node *sign()
{
   return prime();
}

bool within_space(int space)
{
   return tokens[exe_pos]->space > space && tokens[exe_pos]->type != END;
}
Token *new_variable(Token *token);
Token *get_variable(char *name);
void enter_scoop(char *name);
void exit_scoop();

void setReg(Token *token, char *creg)
{
   if (token->creg)
   {
      free(token->creg);
      token->creg = NULL;
   }
   if (creg) token->creg = strdup(creg);
}

void setName(Token *token, char *name)
{
   if (token->name)
   {
      free(token->name);
      token->name = NULL;
   }
   if (name) token->name = strdup(name);
}

Node *prime()
{
   // debug("prime\n");

   Token *token = NULL;
   Node *node = NULL;
   if ((token = find(ID, INT, BOOL, CHARS, CHAR, 0)))
   {
      if (token->declare) // int num
      {
         Token *tmp = find(ID, 0);
         check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
         setName(token, tmp->name);
         ptoken(token);
         new_variable(token);
      }
      else if (token->type == ID && token->name && find(LPAR, 0)) // id
      {
         node = new_node(token);
         if (strcmp(token->name, "main") == 0)
         {
            enter_scoop(token->name);
            check(!find(RPAR, 0), "expected ) after main declaration", "");
            check(!find(DOTS, 0), "expected : after main() declaration", "");
            token->type = FDEC;
            token->retType = INT;
            Node *curr = node;
            Node *last = node;
            while (within_space(token->space))
            {
               curr->right = new_node(NULL);
               curr = curr->right;
               curr->left = expr();
               last = curr->left;
            }
            if (last->token->type != RETURN)
            {
               // TODO: check that return is compatible with function
               curr->right = new_node(NULL);
               curr = curr->right;
               curr->left = new_node(new_token(NULL, 0, 0, RETURN, token->space + TAB));
               curr->left->left = new_node(new_token(NULL, 0, 0, INT, token->space + TAB));
            }
            exit_scoop();
            return node;
         }
         else
         {
            token->type = FCALL;
            Node *tmp = node;
            Token *tmptk = NULL;
            char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
            char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
            int i = 0;
            while (!(tmptk = find(RPAR, END, 0)))
            {
#if 1
               Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space));
               assign->right = expr();
               // check(!assign->right->token->declare, "Invalid argument for function call");
               assign->right->token->space = token->space;
               assign->left = new_node(new_token(NULL, 0, 0, assign->right->token->type, token->space));
               if (eregs[i])
               {
                  // assign->left->token->isarg = true;
                  if (assign->left->token->type == ID)
                  {
                     debug("found function\n");
                     Token *var = get_variable(assign->right->token->name);
                     assign->left->token->type = var->type;
                  }
                  Type type = assign->left->token->type;
                  switch (type)
                  {
                  case CHARS: setReg(assign->left->token, rregs[i]); break;
                  case ADD: case SUB: case MUL: case DIV:
                  case INT: setReg(assign->left->token, eregs[i]); break;
                  default: check(1, "handle this case [%s]", to_string(type));
                  }
                  i++;
               }
               else
               {
                  check(1, "implemnt PTR");
               }
               tmp->left = assign;
               find(COMA, 0);
#else
               tmp->left = expr();
               find(COMA, 0);
#endif
               tmp->right = new_node(NULL);
               tmp = tmp->right;
            }
            check(tmptk->type != RPAR, "expected )");
            return node;
         }
      }
      node = new_node(token);
   }
   else if ((token = find(FDEC, 0)))
   {
      node = new_node(token);

      Node *func_name = prime();
      check(!func_name->token || !func_name->token->declare, "expected data type after func declaration");

      // func_name->token->declare = false;
      // node->left = new_node(NULL);
      // node->left->left = func_name;

      node->token->retType = func_name->token->type;
      node->token->name = func_name->token->name;
      enter_scoop(token->name);
      func_name->token->name = NULL;
      free_node(func_name);
      debug("found FDEC with rettype %s\n", to_string(node->token->retType));

      check(!find(LPAR, 0), "expected ( after function declaration");
      node->left = new_node(NULL);
      Node *curr = node->left;
      Token *tmptk = NULL;
      char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
      char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
      int i = 0;

      while (!(tmptk = find(RPAR, END, 0)))
      {
#if 1
         Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space + TAB));
         assign->left = expr();
         check(!assign->left->token->declare, "Invalid argument for function declaration");
         assign->left->token->space = token->space + TAB;
         assign->right = new_node(new_token(NULL, 0, 0, assign->left->token->type, token->space + TAB));
         if (eregs[i])
         {
            switch (assign->left->token->type)
            {
            case CHARS: setReg(assign->right->token, rregs[i]); break;
            case INT: setReg(assign->right->token, eregs[i]); break;
            default: check(1, "handle this case");
            };
            i++;
         }
         else
         {
            check(1, "implemnt PTR");
         }
         curr->left = assign;
         find(COMA, 0);
#else
         curr->left = expr();
         find(COMA, 0);
#endif
         curr->right = new_node(NULL);
         curr = curr->right;
      }
      check(tmptk->type != RPAR, "expected ) after function declaration");
      check(!find(DOTS, 0), "Expected : after function declaration");

      curr = node;
      while (within_space(token->space))
      {
         curr->right = new_node(NULL);
         curr = curr->right;
         curr->left = expr();
      }
      exit_scoop();
      return node;
   }
   else if ((token = find(IF, 0)))
   {
      node = new_node(token);
      node->left = new_node(NULL);

      Node *tmp = node;
      tmp = tmp->left;

      tmp->left = expr(); // if condition
      check(!find(DOTS, 0), "Expected : after if condition\n", "");
      tmp->right = new_node(NULL);
      tmp = tmp->right;

      // if bloc code
      while (within_space(node->token->space))
      {
         tmp->left = expr();
         tmp->right = new_node(NULL);
         tmp = tmp->right;
      }

      tmp = node;
      while
      (
      includes((Type[]) {ELSE, ELIF, 0}, tokens[exe_pos]->type) &&
   node->token->space == tokens[exe_pos]->space
   )
      {
         token = tokens[exe_pos++];
         tmp->right = new_node(NULL);
         tmp = tmp->right;
         tmp->left = new_node(token);
         if (token->type == ELIF)
         {
            Node *tmp0 = tmp->left;
            tmp0->left = expr();
            check(!find(DOTS, 0), "expected : after else");
            tmp0->right = new_node(NULL);
            tmp0 = tmp0->right;
            while (within_space(token->space))
            {
               tmp0->left = expr();
               tmp0->right = new_node(NULL);
               tmp0 = tmp0->right;
            }
         }
         else if (token->type == ELSE)
         {
            check(!find(DOTS, 0), "expected dots");
            Node *tmp0 = tmp->left;
            tmp0->right = new_node(NULL);
            tmp0 = tmp0->right;
            while (within_space(token->space))
            {
               tmp0->left = expr();
               tmp0->right = new_node(NULL);
               tmp0 = tmp0->right;
            }
            break;
         }
      }
   }
   else if ((token = find(WHILE, 0)))
   {
      node = new_node(token);
      node->left = expr();
      node->left->token->isCond = true;
      check(!find(DOTS, 0), "Expected : after while condition\n", "");
      Node *tmp = node;
      while (within_space(token->space))
      {
         tmp->right = new_node(NULL);
         tmp = tmp->right;
         tmp->left = expr();
         tmp->left->token->space = node->token->space + TAB;
      }
   }
   else if ((token = find(RETURN, 0)))
   {
      // TODO: chec kif return type is compatible with function
      node = new_node(token);
      node->left = expr();
   }
   else if ((token = find(LPAR, 0)))
   {
      node = expr();
      check(!find(RPAR, 0), "Expected )\n", "");
   }
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return node;
}

Node *ast()
{
   if (!AST) return NULL;
   debug(GREEN "===========   AST    ===========\n" RESET);
   Node *head = new_node(NULL);
   Node *curr = head;
   curr->left = expr();
   while (tokens[exe_pos]->type != END)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   debug(GREEN "=========== PRINT AST ==========\n" RESET);
   curr = head;
   while (curr)
   {
      pnode(curr->left, NULL, 0);
      curr = curr->right;
   }
   return head;
}

// IR

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

Scoop *Gscoop;
Scoop *scoop;
size_t scoopSize;
size_t scoopPos;

void enter_scoop(char *name)
{
   debug(CYAN "Enter Scoop [%s]\n" RESET, name);
   if (Gscoop == NULL)
   {
      scoopSize = 10;
      Gscoop = allocate(scoopSize, sizeof(Scoop));
   }
   else if (scoopPos + 1 == scoopSize)
   {
      Scoop *tmp = allocate(scoopSize * 2, sizeof(Scoop));
      memcpy(tmp, Gscoop, scoopPos * sizeof(Scoop));
      scoopSize *= 2;
      free(Gscoop);
      Gscoop = tmp;
   }
   scoopPos++;
   Gscoop[scoopPos] = (Scoop) {};
   Gscoop[scoopPos].name = name;
   scoop = &Gscoop[scoopPos];
}

void exit_scoop()
{
   debug(CYAN "Exit Scoop [%s]\n" RESET, scoop->name);
   free(scoop->functions);
   free(scoop->vars);
   scoop[scoopPos] = (Scoop) {};
   scoop = &Gscoop[--scoopPos];
}

typedef struct
{
   Token *token;
   Token *left;
   Token *right;
} Inst;

Inst **OrgInsts;
Inst **insts;

void clone_insts()
{
   size_t pos = 0;
   size_t len = 100;
   free(insts);
   insts = allocate(len, sizeof(Inst *));

   for (size_t i = 0; OrgInsts[i]; i++)
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

void add_inst(Inst *inst)
{
   static size_t pos;
   static size_t len;
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
   // case struct_: return token->size;
   default: check(1, "add this type [%s]\n", to_string(token->type));
   }
   return 0;
}

Node **builtins_functions;
size_t builtins_pos;
size_t builtins_size;

void create_builtin(char *name, Type *params, Type retType)
{
   Node *func = new_node(new_token(name, 0, strlen(name), FDEC, 0));
   func->token->retType = retType;
   setReg(func->token, "eax");
   func->left = new_node(NULL);
   Node *curr = func->left;
   int i = 0;
   while (params[i])
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = new_node(new_token(NULL, 0, 0, params[i], func->token->space + TAB));
      i++;
   }
   if (builtins_functions == NULL)
   {
      builtins_size = 2;
      builtins_functions = allocate(builtins_size, sizeof(Node *));
   }
   else if (builtins_pos + 1 == builtins_size)
   {
      Node **tmp = allocate(builtins_size * 2, sizeof(Node *));
      memcpy(tmp, builtins_functions, builtins_pos * sizeof(Node *));
      free(builtins_functions);
      builtins_size *= 2;
      builtins_functions = tmp;
   }
   builtins_functions[builtins_pos++] = func;
   // debug("%screate builin [%s]%s\n", GREEN, func->token->name, RESET);
}

Node *new_function(Node *node)
{
   debug("new_func %s in %s scoop has return %d\n", node->token->name, scoop->name, node->token->retType);
#if 0
   char *builtins[] = {"output", 0};
   for (int i = 0; builtins[i]; i++)
   {
      if (strcmp(node->token->name, builtins[i]) == 0)
      {
         error("%s is a built in function\n", node->token->name);
         exit(1);
      }
   }
#endif
   for (size_t i = 0; i < scoop->fpos; i++)
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
   debug("get_func %s in %s scoop\n", name, scoop->name);

#if 1
   // char *builtins[] = {"output", 0};
   // for (int i = 0; builtins[i]; i++)
   //   if (strcmp(name, builtins[i]) == 0)
   //     return NULL;
   for (size_t i = 0; i < builtins_pos; i++)
   {
      debug("loop [%d]\n", i);
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
#endif
   for (size_t j = scoopPos; j > 0; j--)
   {
      Scoop *scoop = &Gscoop[j];
      for (size_t i = 0; i < scoop->fpos; i++)
      {
         Node *func = scoop->functions[i];
         if (strcmp(func->token->name, name) == 0)
            return func;
      }
   }
   check(1, "'%s' Not found\n", name);
   return NULL;
}

Token *new_variable(Token *token)
{
   debug(CYAN "variable [%s] in [%s] scoop\n" RESET, token->name, scoop->name);
   for (size_t i = 0; i < scoop->vpos; i++)
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
   debug(CYAN "get variable [%s] from [%s] scoop\n" RESET, name, scoop->name);
   for (size_t i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      if (strcmp(curr->name, name) == 0) return curr;
   }
   check(1, "%s not found\n", name);
   return NULL;
}

size_t ptr;
Inst *new_inst(Token *token)
{
   static size_t reg;

   Inst *new = allocate(1, sizeof(Inst));
   new->token = token;
   if (token->name && token->declare)
   {
      new_variable(token);
      token->ptr = (ptr += sizeofToken(token));
      token->reg = ++reg;
   }
   else if (includes((Type[]) {ADD, SUB, MUL, DIV, 0}, token->type))
   {
      token->reg = ++reg;
   }
   else if (includes((Type[]) {EQUAL, NOT_EQUAL, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0}, token->type))
   {
      token->reg = ++reg;
   }
   else if (includes((Type[]) {FCALL, 0}, token->type))
   {
      token->reg = ++reg;
   }
   debug("inst: ");
   ptoken(new->token);
   add_inst(new);
   return new;
}
// TODO: implement it
bool compatible(Token *left, Token *right)
{
   return true;
}

size_t bloc_index;
size_t str_index;
Token *generate_ir(Node *node)
{
   Inst *inst = NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *token = get_variable(node->token->name);
      return token;
      break;
   }
   case INT: case BOOL:
   {
      inst = new_inst(node->token);
      // if(inst->token->declare) ptr += sizeofToken(inst->token);
      break;
   }
   case CHARS:
   {
      inst = new_inst(node->token);
      if (!node->token->creg) node->token->index = ++str_index;
      break;
   }
   case ASSIGN: case ADD_ASSIGN:
   case ADD: case SUB: case MUL: case DIV:
   case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(!compatible(left, right), "incompatible type for %s op\n", to_string(node->token->type));
      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      switch (node->token->type)
      {
      case ADD_ASSIGN:
      case ASSIGN:
         node->token->reg = left->reg;
         node->token->retType = left->type;
         break;
      case ADD: case SUB: case MUL: case DIV:
      {
         // TODO: verify this
         // if(left->type != INT && left->type != FLOAT)
         // {
         //     char *c = to_string(node->token->type);
         //     char *l = to_string(left->type);
         //     char *r = to_string(right->type);
         //     check(1, "Invalid operation [%s] between [%s] and [%s]\n", c, l, r);
         // }
         node->token->retType = left->type;
         if (right->type == INT) setReg(node->token, "eax");
         else if (right->type == FLOAT) setReg(node->token, "xmm0");
         break;
      }
      case NOT_EQUAL: case EQUAL: case LESS:
      case MORE: case LESS_EQUAL: case MORE_EQUAL:
      {
         node->token->retType = BOOL;
         node->token->index = ++bloc_index;
         // debug(RED "this is [%d]\n" RESET, node->token->reg);
         break;
      }
      default: break;
      }
      break;
   }

   // {
   //   Token *left = generate_ir(node->left);
   //   Token *right = generate_ir(node->right);
   //   check(!compatible(left, right), "incompatible type for %s op\n", to_string(node->token->type));
   //   inst = new_inst(node->token);
   //   inst->left = left;
   //   inst->right = right;
   //   inst->token->retType = BOOL;
   //   break;
   // }
   case FDEC:
   {
      new_function(node);
      switch (node->token->retType)
      {
      case INT: setReg(node->token, "eax"); break;
      case CHARS: setReg(node->token, "rax"); break;
      default: check(1, "handle this case [%s]\n", to_string(node->token->retType)); break;
      }
      enter_scoop(node->token->name);

      size_t tmp_ptr = ptr;
      ptr = 0;

      pnode(node, NULL, 5);
      inst = new_inst(node->token);

      // arguments
      Node *arg = node->left;
      /*
        dest: left  (variable inside function)
        src : right
      */
      debug(GREEN"print arguments\n"RESET);
      while (arg && arg->left)
      {
         pnode(arg->left, "", 10);
         generate_ir(arg->left);
         arg = arg->right;
      }
      // exit(1);
      // code bloc
      Node *curr = node->right;
      while (curr)
      {
         generate_ir(curr->left);
         curr = curr->right;
      }
      Token *new = new_token(NULL, 0, 0, END_BLOC, node->token->space);
      new->name = strdup(node->token->name);
      new_inst(new);
      exit_scoop();
      node->token->ptr = ptr;
      ptr = tmp_ptr;
      break;
   }
   case FCALL:
   {
      if (strcmp(node->token->name, "output") == 0)
      {

      }
      else
      {
         Node *func = get_function(node->token->name);
         node->token->retType = func->token->retType;
         setReg(node->token, func->token->creg);
         debug("%s: has the following arguments\n", node->token->name);
         Node *arg = node;
         while (arg->left)
         {
            pnode(arg->left, "", 10);
            generate_ir(arg->left);
            arg = arg->right;
         }
      }
      new_inst(node->token);
      return node->token;
      break;
   }
   case IF:
   {
      Node *curr = node->left;

      Inst *tmp = new_inst(copy_token(node->token));
      setName(tmp->token, "if");
      tmp->token->type =  BLOC;
      tmp->token->index = ++bloc_index;

      generate_ir(curr->left); // TODO: check if it's boolean
      node->token->type = JNE;
      setName(node->token, "endif");
      node->token->index = ++bloc_index;

      Token *lastInst = copy_token(node->token);
      new_inst(lastInst); // jne to endif
      curr = curr->right;

      // if code bloc
      while (curr->left)
      {
         generate_ir(curr->left);
         curr = curr->right;
      }

      Inst *endInst = NULL;
      if (node->right)
      {
         endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space));
         endInst->token->index = node->token->index;
      }

      curr = node->right;
      while (curr)
      {
         if (curr->left->token->type == ELIF)
         {
            curr->left->token->index = ++bloc_index;
            curr->left->token->type = BLOC;
            setName(curr->left->token, "elif");

            {
               setName(lastInst, "elif");
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
         else if (curr->left->token->type == ELSE)
         {
            curr->left->token->index = ++bloc_index;
            curr->left->token->type = BLOC;
            setName(curr->left->token, "else");
            new_inst(curr->left->token);

            {
               setName(lastInst, "else");
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

         setName(lastInst, "endif");
         lastInst->index = node->token->index;

         if (curr->right) // to not add a jmp in the last statement to avoid
            // jne endifX
            // endifX:
         {
            endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space));
            endInst->token->index = node->token->index;
         }
         curr = curr->right;
      }

      Token *new = new_token("endif", 0, 5, BLOC, node->token->space);
      new->index = node->token->index;
      new_inst(new);
      // free_token(lastInst);
      return node->left->token;
      break;
   }
   case WHILE:
   {
      node->token->type = BLOC;
      setName(node->token, "while");
      node->token->index = ++bloc_index;
      inst = new_inst(node->token);

      setName(node->left->token, "endwhile");
      generate_ir(node->left); // TODO: check if it's boolean
      node->left->token->index = node->token->index;
      // Token *end = copy_token(node->token);
      // end->type = JNE;
      // setName(end, "endwhile");
      // new_inst(end);

      Node *curr = node->right;
      while (curr) // while code bloc
      {
         generate_ir(curr->left);
         curr = curr->right;
      }

      Token *lastInst = copy_token(node->token);
      lastInst->type = JMP;
      setName(lastInst, "while");
      new_inst(lastInst); // jmp back to while loop

      lastInst = copy_token(node->token);
      lastInst->type = BLOC;
      setName(lastInst, "endwhile");
      new_inst(lastInst); // end while bloc
      break;
   }
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      break;
   }
   default: break;
   }
   return inst->token;
}

void print_ir()
{
   if (!DEBUG) return;
   debug(GREEN "==========   PRINT IR  =========\n" RESET);
   int i = 0;
   clone_insts();
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
         else
         {
            // if(right->type == ADD) debug("<r%.2d>\n", right->reg);
            switch (right->type)
            {  // TODO: handle the other cases
            case INT: debug("%lld", right->Int.value); break;
            case BOOL: debug("%s", right->Bool.value ? "True" : "False"); break;
            case FLOAT: debug("%f", right->Float.value); break;
            case CHAR: debug("%c", right->Char.value); break;
            case CHARS: debug("%s", right->Chars.value); break;
            default:
            {
               check(1, "handle this case [%s]\n", to_string(right->type));
               break;
            }
            }
         }
         break;
      }
      case ADD: case SUB: case MUL: case DIV:
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
      {
         debug("[%-6s] ", to_string(curr->type));
         if (left->reg) debug("r%.2d", left->reg);
         else if (left->creg)
         {
            check(1, "handle this case");
         }
         else
         {
            switch (left->type)
            {  // TODO: handle the other cases
            case INT: debug("%lld", left->Int.value); break;
            case BOOL: debug("%s", left->Bool.value ? "True" : "False"); break;
            case FLOAT: debug("%f", left->Float.value); break;
            case CHAR: debug("%c", left->Char.value); break;
            case CHARS: debug("%s", left->Chars.value); break;
            default: check(1, "handle this case [%s]\n", to_string(left->type));
            }
         }
         if (left->name) debug(" (%s)", left->name);
         debug(" to ");
         if (right->reg) debug("r%.2d", right->reg);
         else
         {
            switch (right->type)
            {  // TODO: handle the other cases
            case INT: debug("%lld", right->Int.value); break;
            case BOOL: debug("%s", right->Bool.value ? "True" : "False"); break;
            case FLOAT: debug("%f", right->Float.value); break;
            case CHAR: debug("%c", right->Char.value); break;
            case CHARS: debug("%s", right->Chars.value); break;
            default: check(1, "handle this case [%s]\n", to_string(right->type)); break;
            }
         }
         if (right->name) debug(" (%s)", right->name);
         break;
      }
      case INT: case BOOL: case CHARS:
      {
         debug("[%-6s] ", to_string(curr->type));
         if (curr->declare) debug("declare [%s] PTR=[%zu]", curr->name, curr->ptr);
         else if (curr->name) debug("variable %s ", curr->name);
         else if (curr->type == INT) debug("value %lld ", curr->Int.value);
         else if (curr->type == BOOL) debug("value %s ", curr->Bool.value ? "True" : "False");
         // else if(curr->type == CHAR) debug("value %c ", curr->Char.value);
         // else if(curr->type == BOOL) debug("value %s ", curr->Bool.value ? "True" : "False");
         // else if(curr->type == FLOAT)
         // {
         //     curr->index = ++float_index;
         //     debug("value %f ", curr->Float.value);
         // }
         else if (curr->type == CHARS)
         {
            if (curr->index) debug("value %s in STR%zu", curr->Chars.value, curr->index);
            else debug("in %s", curr->creg);
         }
         else check(1, "handle this case in generate ir\n", "");
         break;
      }
      // case POP:
      // {
      //   /*
      //     dest: left  (variable inside function)
      //     src : right
      //   */
      //   debug("pop from ");
      //   if(right->ptr) debug("PTR %zu", right->ptr);
      //   else if(right->creg) debug("%s", right->creg);
      //   debug(" to ");
      //   if(left->ptr) debug("PTR %zu", left->ptr);
      //   else if(left->creg) debug("%s", left->creg);
      //   break;
      // }
      // case PUSH: debug("push "); break;
      case JMP: debug("jmp to [%s]", curr->name); break;
      case JNE: debug("jne to [%s]", curr->name); break;
      case FCALL: debug("call [%s]", curr->name); break;
      case BLOC: case FDEC: debug("[%s] bloc", curr->name); break;
      case END_BLOC: debug("[%s] endbloc", curr->name); break;
      case RETURN: debug("[return]"); break;
      default: debug(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
      }
      // if(curr->remove) debug(" remove");
      debug(" space (%zu)", curr->space);
      debug("\n");
   }
   debug("total instructions [%d]\n", i);
}

bool includes(Type *types, Type type)
{
   for (int i = 0; types[i]; i++)
      if (types[i] == type) return true;
   return false;
}

/*
+ TODO: check optimization when doing it in sub scoops
+ example:
    int x = 10
    if x == 2: (this will caus problem now)
        x = 3
*/
#define MAX_OPTIMIZATION 6
bool optimize_ir(int op)
{
   check(!insts, "empty file (no instruction created)");
   bool optimize = false;
   if (op == 0)
   {
      debug(CYAN "OP[%d] calculate operations on constants\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         Token *left = insts[i]->left;
         Token *right = insts[i]->right;
         if (includes((Type[]) {ADD, SUB, MUL, DIV, 0}, token->type))
         {
            Type types[] = {INT, FLOAT, CHAR, 0};
            // TODO: check if left nad right are compatible
            if (includes(types, left->type) && includes(types, right->type) && !left->name && !right->name)
            {
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
               case FLOAT:
                  // left->index = 0;
                  // right->index = 0;
                  // token->index = ++float_index;
                  // switch (token->type)
                  // {
                  // case ADD: token->Float.value = left->Float.value + right->Float.value; break;
                  // case SUB: token->Float.value = left->Float.value - right->Float.value; break;
                  // case MUL: token->Float.value = left->Float.value * right->Float.value; break;
                  // case DIV: token->Float.value = left->Float.value / right->Float.value; break;
                  // default: break;
                  // }
                  break;
               // case CHARS:
               //     switch(token->type)
               //     {
               //     case add_: token->Chars.value = strjoin(left, right); break;
               //     default:
               //         error("Invalid %s op in chars\n", to_string(token->type)); break;
               //     }
               default:
                  check(1, "handle this case\n", "");
                  break;
               }
               token->type = left->type;
               left->remove = true;
               right->remove = true;
               token->reg = 0;
               setReg(token, NULL);
               // token->creg = NULL;
               clone_insts();
               i = 0;
               optimize = true;
            }
         }
      }
   }
   else if (op == 1)
   {
      // TODO: to be checked
      debug(CYAN "OP[%d] calculate operations on constants\n" RESET, op);
      int i = 1;
      while (insts && insts[i])
      {
         Token *curr_token = insts[i]->token;
         Token *curr_left = insts[i]->left;
         Token *curr_right = insts[i]->right;

         Token *prev_token = insts[i - 1]->token;
         Token *prev_left = insts[i - 1]->left;
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
               optimize = true;
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

         i++;
      }
   }
   else if (op == 2)
   {
      debug(CYAN "OP[%d] remove reassigned variables\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         if (insts[i]->token->declare)
         {
            int j = i + 1;
            while (insts[j] && insts[j]->token->space == insts[i]->token->space)
            {
               // ptoken(insts[j]->token);
               if (insts[j]->token->type == ASSIGN && insts[j]->left == insts[i]->token)
               {
                  insts[i]->token->declare = false;
                  insts[i]->token->remove = true;
                  optimize = true;
                  break;
               }
               if (
                  (insts[j]->left && insts[j]->left->reg == insts[i]->token->reg) ||
                  (insts[j]->right && insts[j]->right->reg == insts[i]->token->reg))
                  break;
               j++;
            }
         }
         else if (insts[i]->token->type == ASSIGN)
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
                  insts[j]->token->type == ASSIGN &&
                  insts[j]->left == insts[i]->left)
               {
                  insts[i]->token->remove = true;
                  optimize = true;
                  break;
               }
               // if the variable is used some where
               else if (
                  insts[j]->left->reg == insts[i]->token->reg ||
                  insts[j]->right->reg == insts[i]->token->reg)
                  break;
               j++;
            }
         }
      }
   }
   else if (op == 3)
   {
      debug(CYAN"OP[%d] remove followed return instructions\n"RESET, op);
      for (int i = 1; insts[i]; i++)
      {
         if (insts[i]->token->type == RETURN && insts[i - 1]->token->type == RETURN)
         {
            optimize = true;
            insts[i]->token->remove = true;
            clone_insts();
            i = 1;
         }
      }
   }
   /*
     I commented it because it aused me problem in this case:
       int i = 0
       while i < 5:
         i = i + 1
   */
   // else if(op == 4)
   // {
   //   debug(CYAN "OP[%d] calculate followed operations\n" RESET, op);
   //   for(int i = 0; insts[i]; i++)
   //   {
   //     Token *curr = insts[i]->token;
   //     Token *left = insts[i]->left;   // a ptr
   //     Token *right = insts[i]->right; // value (not a ptr)
   //     // TODO: or declare
   //     if(curr->type == ASSIGN && !right->ptr)
   //     {
   //       for(int j = i + 1; insts[j]; j++)
   //       {
   //         // TODO: handle other mathematical operations
   //         if(insts[j]->token->type == ADD)
   //         {
   //           Token *math_left = insts[j]->left;
   //           Token *math_right = insts[j]->right;
   //           if(math_right->ptr == left->ptr)
   //           {
   //             insts[j]->right = copy_token(right);
   //             clone_insts();
   //             return true;
   //           }
   //           else if(math_left->ptr == left->ptr)
   //           {
   //             insts[j]->left = copy_token(right);
   //             clone_insts();
   //             return true;
   //           }
   //         }
   //       }
   //     }
   //   }
   // }
   else if (op == 4)
   {
      // TODO: be carefull this one remove anything that don't have reg
      debug(CYAN "OP[%d] (remove unused instructions)\n"RESET, op);
      for (size_t i = 0; insts[i]; i++)
      {
         Token *curr = insts[i]->token;
         if (!curr->ptr && !curr->reg && includes((Type[]) {INT, 0}, curr->type))
         {
            curr->remove = true;
            i = 0;
            clone_insts();
            return true;
         }
      }
   }
   return optimize;
}

void ir(Node *head)
{
   if (!IR) return;

   debug(GREEN "========== GENERATE IR =========\n" RESET);
   enter_scoop("");
   Node *curr = head;
   while (curr)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }
   exit_scoop();


#if OPTIMIZE
   int i = 0;
   bool optimized = false;
   clone_insts();
   while (i < MAX_OPTIMIZATION)
   {
      print_ir();
      optimized = optimize_ir(i++) || optimized;
      if (i == MAX_OPTIMIZATION && optimized)
      {
         optimized = false;
         i = 0;
      }
   }
#endif
   print_ir();
}

// ASSEMBLY
struct _IO_FILE *asm_fd;
bool did_pasm = true;
void pasm(char *fmt, ...)
{
   did_pasm = true;
   int i = 0;
   va_list args;
   va_start(args, fmt);

#if 0
#define isInstruction(inst)                        \
  do                                               \
  {                                                \
    if (strncmp(fmt + i, inst, strlen(inst)) == 0) \
    {                                              \
      i += strlen(inst);                           \
      fprintf(asm_fd, "%-8s", inst);             \
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
   isInstruction("jne ");
   isInstruction("jmp ");
#endif

// #if WITH_COMMENTS
//   isInstruction("//");
// #else
//   if (strncmp(fmt + i, "//", 2) == 0)
//     return;
// #endif

   while (fmt[i])
   {
#if !WITH_COMMENTS
      if (strncmp(fmt + i, "//", 2) == 0)
      {
         while (fmt[i] && fmt[i] != '\n') i++;
         while (fmt[i] == '\n') i++;
      }
      else
#endif
         if (fmt[i] == '%')
         {
            i++;
            if (fmt[i] == 'i')
            {
               i++;
               fprintf(asm_fd, "%-4s ", va_arg(args, char *));
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
                  case CHARS: fputs("rax", asm_fd); break;
                  case INT: fputs("eax", asm_fd); break;
                  case BOOL: case CHAR: fputs("al", asm_fd); break;
                  case FLOAT: fputs("xmm0", asm_fd); break;
                  default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
                  }
               }
            }
            else if (fmt[i] == 'a')
            {
               i++;
               Token *token = va_arg(args, Token *);
               if (token->creg)
                  fprintf(asm_fd, "%s", token->creg);
               else
                  switch (token->type)
                  {
                  case CHARS: fprintf(asm_fd, "QWORD PTR -%ld[rbp]", token->ptr); break;
                  case INT: fprintf(asm_fd, "DWORD PTR -%ld[rbp]", token->ptr); break;
                  case CHAR: fprintf(asm_fd, "BYTE PTR -%ld[rbp]", token->ptr); break;
                  case BOOL: fprintf(asm_fd, "BYTE PTR -%ld[rbp]", token->ptr); break;
                  case FLOAT: fprintf(asm_fd, "DWORD PTR -%ld[rbp]", token->ptr); break;
                  default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
                  }
            }
            else if (fmt[i] == 'v')
            {
               i++;
               Token *token = va_arg(args, Token *);
               switch (token->type)
               {
               case INT: fprintf(asm_fd, "%lld", token->Int.value); break;
               case BOOL: fprintf(asm_fd, "%d", token->Bool.value); break;
               case CHAR: fprintf(asm_fd, "%d", token->Char.value); break;
               default:
                  check(1, "Unknown type [%s]\n", to_string(token->type));
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
      if (curr->type == CHARS && !curr->name && !curr->ptr && curr->index)
         pasm(".STR%zu: .string %s\n", curr->index, curr->Chars.value ? curr->Chars.value : "\"\"");
      if (curr->type == FLOAT && !curr->name && !curr->ptr && curr->index)
         pasm(".FLT%zu: .long %zu /* %f */\n", curr->index,
              *((uint32_t *)(&curr->Float.value)), curr->Float.value);
   }
   pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
#endif
}

void skip_space(int space)
{
   if (did_pasm)
   {
      pasm("\n");
      int j = -1; while (++j < space) pasm(" ");
      did_pasm = false;
   }
}

void generate_asm()
{
   if (!ASM) return;
   debug(GREEN "======= GENERATE ASSEMBLY ======\n" RESET);
   debug(CYAN);
   // asm_fd = stdout;
   initialize();
   clone_insts();
   for (size_t i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      skip_space(curr->space);
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS:
      {
         if (curr->declare)
         {
            pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); skip_space(curr->space);
         }
         break;
      }
      case ADD_ASSIGN:
      {
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); skip_space(curr->space);}
         break;
      }
      case ASSIGN:
      {

         // if(strcmp(left->name, "i") == 0)
         // {
         //   debug(RED"found i\n");
         //   debug("has right: ");
         //   ptoken(right);
         //   if(right->ptr) debug("has ptr %zu\n", right->ptr);
         //   if(right->creg) debug("has creg %s\n", right->creg);
         //   exit(1);
         // }
         char *inst = left->type == FLOAT ? "movss " : "mov ";
         if (right->ptr)
         {
            pasm("%i%r, %a", inst, left, right);
            /*
            test this case before changing
            chars str = "fffff" int a = strlen(str)
            */
            if (!left->creg) pasm("%i%a, %r", inst, left, left);
         }
         else if (right->creg)
         {
            pasm("%i%a, %r", inst, left, right);
         }
         else
         {
            switch (right->type)
            {
            case INT: case BOOL: case CHAR:
               pasm("%i%a, %v", "mov", left, right);
               break;
            case CHARS:
               pasm("%i%r, .STR%zu[rip]", "lea", left, right->index);

               // I did this to defrenticiate
               // function parameter from
               // variable declaration
               if (left->ptr) {skip_space(curr->space); pasm("%i%a, %r", "mov", left, right); }
               break;
            // case float_:
            //     pasm("movss %r, DWORD PTR .FLT%zu[rip]\n", right, right->index);
            //     pasm("movss %a, %r\n", left, left);
            //     break;
            default:
               check(1, "handle this case (%s)\n", to_string(right->type));
               break;
            }
         }

         if (left->name) {pasm(" ;// assign [%s]", left->name); }
         else if (left->creg) {pasm(" ;// assign [%s]", left->creg);}
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
         // debug("left: "); ptoken(left);
         // debug("right: "); ptoken(right);
         // debug("left creg %s\n", left->creg);
         // debug("right creg %s\n", right->creg);
         // debug("left %s, right: %s\n", left->creg, right->creg);
         // if (left->ptr)
         //   pasm("%i%r, %a\n", inst, curr, left);
         // else if (left->creg && right->creg && strcmp(left->creg, right->creg)) // they should != eax
         //   pasm("%i%r, %r\n", inst, curr, left);
         // else if (!left->creg)
         // {
         //   // debug("helloooo\n");
         //   if(!right->creg) pasm("%i%r, %v\n", inst, curr, left);
         //   else if(strcmp(right->creg, "eax") == 0)
         //   {
         //     inst = "add";
         //     pasm("%i%r, %v\n",inst2, curr, left);
         //   }
         // }


         // if (right->ptr)
         //   pasm("%i%r, %a\n", inst2, curr, right);
         // else if (right->creg && (!left->creg || (left->creg && strcmp(left->creg, right->creg)))) // they should != eax
         //   pasm("%i%r, %r\n", inst2, curr, right);
         // else
         //   pasm("%i%r, %v\n", inst2, curr, right);
         // skip_space(curr->space);
         pasm("%i%r, ", inst, left);
         if (left->ptr) pasm("%a", left);
         else if (left->creg) pasm("%r", left) ;
         else pasm("%v", left);
         // pasm("\n [%zu]", curr->space);
         skip_space(curr->space);


         pasm("%i%r, ", inst2, right);
         if (right->ptr) pasm("%a", right);
         else if (right->creg) pasm("%r", right) ;
         else pasm("%v", right);

         // if (!left->creg && !left->ptr)
         // {
         //   if(!right->creg) pasm("%i%r, %v", inst, curr, left);
         //   else if(strcmp(right->creg, "eax") == 0)
         //   {
         //     inst = "add";
         //     pasm("%i%r, %v\n",inst2, curr, left);
         //   }
         // }
         // else
         // {
         //   if(right->ptr) pasm("%i%a, ", inst2, right);
         //   else if(right->creg) pasm("%i%r, ", inst2, right);
         //   else if (!right->creg) pasm("%i%v, ", inst2, right);
         // }
         // if(right->ptr)
         curr->type = left->type;
         break;
      }
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case MORE_EQUAL:
      {
         if (curr->isCond)
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
            skip_space(curr->space); pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%r", left);
            else if (!left->creg) pasm("%v", left);

            // skip_space(curr->space);
            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %r", right);
            else if (!right->creg) pasm(", %v", right);
            skip_space(curr->space); pasm("%i .%s%zu", inst, curr->name, curr->index);
         }
         else
         {
            char *inst = left->type == FLOAT ? "movss" : "mov";
            if (left->ptr) pasm("%i%r, %a", inst, left, left);
            else if (left->creg /*&& strcmp(left->creg, r->creg)*/)
               pasm("%i%r, %r", inst, left, left);
            else if (!left->creg) pasm("%i%r, %v", inst, left, left);

            char *reg = NULL;
            switch (left->type)
            {
            case INT: reg = "ebx"; break;
            case FLOAT: reg = "xmm1"; break;
            case CHAR: reg = "bl"; break;
            case BOOL: reg = "ebx"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            skip_space(curr->space);
            if (right->ptr) pasm("%i%s, %a", inst, reg, right);
            else if (right->creg) pasm("%i%s, %r", inst, reg, right);
            else if (!right->creg) pasm("%i%s, %v", inst, reg, right);

            inst = left->type == FLOAT ? "ucomiss" : "cmp";
            skip_space(curr->space);
            pasm("%i%r, %s", inst, left, reg);
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
            skip_space(curr->space);
            pasm("%i%r", inst, curr);
         }
         break;
      }
      case FDEC:
      {
         pasm("%s:", curr->name);
         skip_space(curr->space + TAB); pasm("%irbp", "push");
         skip_space(curr->space + TAB); pasm("%irbp, rsp", "mov");
         skip_space(curr->space + TAB); pasm("%irsp, %zu", "sub", (((curr->ptr) + 15) / 16) * 16);
         break;
      }
      case JE:
      {
         pasm("%ial, 1", "cmp");
         skip_space(curr->space); pasm("%i.%s%zu", "je", curr->name, curr->index);
         break;
      }
      case JNE:
      {
         pasm("%ial, 1", "cmp");
         skip_space(curr->space); pasm("%i.%s%zu", "jne", curr->name, curr->index);
         break;
      }
      case JMP:
      {
         pasm("%i.%s%zu", "jmp", curr->name, curr->index);
         break;
      }
      case FCALL:
      {
         pasm("%i%s", "call", curr->name);
         break;
      }
      case BLOC:
      {
         pasm(".%s%zu:", curr->name, curr->index);
         break;
      }
      case END_BLOC:
      {
         pasm(".end%s:", curr->name);
         break;
      }
      case RETURN:
      {
         if (left->ptr) pasm("%i%r, %a", "mov", left, left);
         else if (left->creg)
         {
            // TODO: check the type
            if (strcmp(left->creg, "eax")) pasm("%i%r, %a", "mov", left, left);
         }
         else
         {
            switch (left->type)
            {
            case INT: pasm("%i%r, %ld", "mov", left, left->Int.value); break;
            case VOID: pasm("%ieax, 0", "mov"); break;
            default:
            {
               check(1, "handle this case [%s]\n", to_string(left->type));
               break;
            }
            }
         }
         skip_space(curr->space); pasm("%i", "leave");
         skip_space(curr->space); pasm("%i", "ret");
         break;
      }
      // case PUSH:
      // {
      //   /*
      //     left: source
      //     right: destination
      //   */
      //   if(right->name) pasm("mov %s, ", right->name);
      //   else if(right->ptr) check(1, "handle this case");
      //   else if(right->creg) pasm("mov %r, ", right);
      //   else check(1, "handle this case");

      //   if(left->ptr) pasm("%a\n", left);
      //   else if(left->creg) pasm("%r\n", left);
      //   else pasm("%v\n", left);
      //   break;
      // }
      // case POP:
      // {
      //   /*
      //     left: source
      //     right: destination
      //   */
      //   if(right->name) pasm("mov %s, ", right->name);
      //   else if(right->ptr) check(1, "handle this case");
      //   else if(right->creg) pasm("mov %r, ", right);
      //   else check(1, "handle this case");

      //   if(left->ptr) pasm("%a\n", left);
      //   else if(left->creg) pasm("%r\n", left);
      //   else pasm("%v\n", left);
      //   break;
      // }
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
   debug(RESET);
}
// MAIN
int main(int argc, char **argv)
{
   char *outputFile = strdup(argv[1]);
   outputFile[strlen(outputFile) - 1] = 's';

   char *input = open_file(argv[1]);
   // printf("in file %s, outfile %s\n", argv[1], outputFile);

   asm_fd = fopen(outputFile, "w+");
   check(!asm_fd, "openning %s\n", outputFile);
   free(outputFile);
   // debug("===========    IR    ===========\n");
   tokenize(input);
   enter_scoop("");
   Node *head = ast();
   exit_scoop();
   create_builtin("putnbr", (Type[]) {INT, 0}, INT);
   create_builtin("write", (Type[]) {INT, CHARS, INT, 0}, INT);
   // create_builtin("read", (Type[]){int_, chars_, int_, 0}, int_);
   // create_builtin("exit", (Type[]){int_, 0}, int_);
   // create_builtin("malloc", (Type[]){int_, 0}, ptr_);
   // create_builtin("calloc", (Type[]){int_, int_, 0}, ptr_);
   // create_builtin("strdup", (Type[]){chars_, 0}, chars_);
   create_builtin("strlen", (Type[]) {CHARS, 0}, INT);
   // create_builtin("free", (Type[]){ptr_, 0}, void_);
   // create_builtin("strcpy", (Type[]){chars_, chars_, 0}, chars_);
   // create_builtin("strncpy", (Type[]){chars_, chars_, int_, 0}, chars_);
   // // create_builtin("puts", (Type[]){chars_, 0}, int_);
   create_builtin("putstr", (Type[]) {CHARS, 0}, INT);
   create_builtin("putchar", (Type[]) {CHAR, 0}, INT);
   // create_builtin("putbool", (Type[]){bool_, 0}, int_);
   // create_builtin("putfloat", (Type[]){float_, 0}, int_);
   ir(head);
   generate_asm();
   free_node(head);
   for (size_t i = 0; OrgInsts && OrgInsts[i]; i++)
      free(OrgInsts[i]);
   for (size_t i = 0; tokens && tokens[i]; i++)
      free_token(tokens[i]);
   free(tokens);
   free(input);
   debug("\n");
}
