#include "./include/header.h"

// TOKENIZE
Specials *dataTypes = (Specials[]) { {"int", INT}, {"bool", BOOL}, {"chars", CHARS}, {0, (Type)0}};

Token* new_token(char *input, size_t s, size_t e, Type type, size_t space)
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
   // case BLOC: case ID: case JMP: case JE: case JNE: case FDEC:
   case ID:
   {
      if (e <= s)
         break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
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
      if (e <= s)
         break;
      new->Chars.value = allocate(e - s + 1, sizeof(char));
      strncpy(new->Chars.value, input + s, e - s);
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
   debug("new %k\n", new);
   return new;
}

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

// ABSTRACT SYNTAX TREE
Node *new_node(Token *token)
{
   Node *new = allocate(1, sizeof(Node));
   new->token = token;
   return new;
}

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


bool within_space(size_t space)
{
   return tokens[exe_pos]->space > space && tokens[exe_pos]->type != END;
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
}

void setReg(Token *token, char *creg)
{
   if (token->creg)
   {
      free(token->creg);
      token->creg = NULL;
   }
   if (creg) token->creg = strdup(creg);
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

bool includes(Type *types, Type type)
{
   for (int i = 0; types[i]; i++)
      if (types[i] == type) return true;
   return false;
}

size_t ptr;
Inst *new_inst(Token *token)
{
   static size_t reg;

   Inst *new = allocate(1, sizeof(Inst));
   new->token = token;
   if (token->name && token->declare)
   {
      // new_variable(token);
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
   debug("inst: %k\n", new->token);
   add_inst(new);
   return new;
}

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
#if 0
   for (size_t i = 0; i < builtins_pos; i++)
   {
      debug("loop [%d]\n", i);
      if (strcmp(name, builtins_functions[i]->token->name) == 0)
         return builtins_functions[i];
   }
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
// TODO: implement it
bool compatible(Token *left, Token *right)
{
   return true;
}


// ASSMBLY GENERATION

// UTILS
const char *to_string(Type type) {
   const char *type_strings[] = {
      [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD ASSIGN", [SUB_ASSIGN] = "SUB ASSIGN",
      [MUL_ASSIGN] = "MUL ASSIGN", [DIV_ASSIGN] = "DIV ASSIGN", [EQUAL] = "EQUAL",
      [NOT_EQUAL] = "NOT EQUAL", [LESS_EQUAL] = "LESS THAN OR EQUAL",
      [MORE_EQUAL] = "MORE THAN OR EQUAL", [LESS] = "LESS THAN", [MORE] = "MORE THAN",
      [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [MOD] = "MOD",
      [AND] = "AND", [OR]  = "OR", [RPAR] = "R_PAR", [LPAR] = "L_PAR", [COMA] = "COMMA",
      [DOTS] = "DOTS", [DOT] = "DOT", [RETURN] = "RETURN", [IF] = "IF", [ELIF] = "ELIF",
      [ELSE] = "ELSE", [WHILE] = "HILE", [FDEC] = "F_DEC", [FCALL] = "F_CALL", [INT] = "INT",
      [VOID] = "VOID", [CHARS] = "CHARS", [CHAR] = "CHAR", [BOOL] = "BOOL", [FLOAT] = "FLOAT",
      [STRUCT] = "STRUCT", [ID] = "ID", [END_BLOC] = "END_BLOC", [BLOC] = "BLOC", [JNE] = "JNE",
      [JE] = "JE", [JMP] = "JMP", [END] = "END"
   };
   if (type >= 1 && type < sizeof(type_strings) / sizeof(type_strings[0]) && type_strings[type] != NULL)
      return type_strings[type];
   check(1, "Unknown type [%d]\n", type);
   return NULL;
}

void *allocate_func(size_t line, size_t len, size_t size)
{
   void *ptr = calloc(len, size);
   check(!ptr, "allocate did failed in line %zu\n", line);
   return ptr;
}

void open_file(char *filename)
{
   if (found_error) return;
   struct _IO_FILE *file = fopen(filename, "r");
   fseek(file, 0, SEEK_END);
   size_t size = ftell(file);
   fseek(file, 0, SEEK_SET);
   input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
}

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
}

// ASSEMBLY
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

bool did_pasm;
void skip_space(int space)
{
   if (did_pasm)
   {
      pasm("\n");
      int j = -1; while (++j < space) pasm(" ");
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
   default:
   {
      check(1, "handle this case [%s]\n", to_string(token->type));
      break;
   }
   }
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
      size_t k = 0;
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

int ptoken(Token *token)
{
   int res = 0;
   if (!DEBUG) return res;

   res += debug("[%-6s] ", to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      if (token->declare) res += debug("[declare] ");
      if (!token->name && !token->declare)
      {
         switch (token->type)
         {
         case INT: res += debug("value [%lld]", token->Int.value); break;
         case CHARS: res += debug("value [%s]", token->Chars.value); break;
         case CHAR: res += debug("value [%c]", token->Char.value); break;
         case BOOL: res += debug("value [%d]", token->Bool.value); break;
         case FLOAT: res += debug("value [%f]", token->Float.value); break;
         default: break;
         }
      }
      break;
   }
   case FCALL: case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   if (token->remove) res += debug("[remove] ");
   res += debug("space [%zu] ", token->space);
   return res;
}

int pnode(Node *node, char *side, size_t space)
{
   if (!node) return 0;

   int res = 0;
   for (size_t i = 0; i < space; i++)
      res += debug(" ");
   if (side) res += debug("%s", side);
   res += debug("%k\n", node->token);
   if (node->token)
   {
      switch (node->token->type)
      {
      case FDEC:
      {
#if 0
         node = node->right;
         while (node)
         {
            res += pnode(node->left, NULL, space + TAB);
            node = node->right;
         }
#else
         res += pnode(node->left, "L: ", space + TAB);
         res += pnode(node->right, "R: ", space + TAB);
#endif
         break;
      }
      default:
      {
         res += pnode(node->left, "L: ", space + TAB);
         res += pnode(node->right, "R: ", space + TAB);
         break;
      }
      }
   }
   else
   {
      res += pnode(node->left, "L: ", space + TAB);
      res += pnode(node->right, "R: ", space + TAB);
   }
   return res;
}

int debug(char *conv, ...)
{
   size_t i = 0;
   int res = 0;

   va_list args;
   va_start(args, conv);

   while (conv[i])
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
         if (strncmp(conv + i, "zu", 2) == 0)
         {
            res += fprintf(stdout, "%zu", va_arg(args, size_t));
            i++;
         }
         else if (strncmp(conv + i, "lld", 3) == 0)
         {
            res += fprintf(stdout, "%lld", va_arg(args, long long));
            i += 2;
         }
         else
         {
            switch (conv[i])
            {
            case 'c':
               res += fprintf(stdout, "%c", va_arg(args, int));
               break;
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
            default:
               check(1, "invalid format specifier [%c]\n", conv[i]);
               exit(1);
               break;
            }
         }
      }
      else res += fprintf(stdout, "%c", conv[i]);
      i++;
   }

   va_end(args);
   return res;
}
// CLEAR MEMORY
void free_token(Token *token)
{
   if (token->name) free(token->name);
   if (token->creg) free(token->creg);
   if (token->Chars.value) free(token->Chars.value);
   free(token);
}

void free_node(Node *node)
{
   if (!node) return;
   free_node(node->left);
   free_node(node->right);
   free(node);
}

void free_memory()
{
   free(input);
   for (int i = 0; tokens && tokens[i]; i++) free_token(tokens[i]);
   free_node(head);
}