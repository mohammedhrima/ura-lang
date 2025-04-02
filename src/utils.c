#include "./include/header.h"

// TOKENIZE
Specials *dataTypes = (Specials[]) { {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
   {"char", CHAR}, {"float", FLOAT}, {"void", VOID}, {0, (Type)0}
};

Token* new_token_(char *filename, int line, char *input, int s, int e, Type type, int space)
{
   // debug("new token in %s:%d\n", filename, line);
   filename = NULL, line = 0;
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