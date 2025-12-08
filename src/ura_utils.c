#include "./ura_header.h"

/*
SECTIONS:
   + DEBUGING AND ERROR HANDLING
   + MEMMORY
   + PARSING
   + IR
   + LLVM UTILS
   + UTILS
*/

// DEBUGING AND ERROR HANDLING
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
         if (strncmp(conv + i, "zu", 2) == 0)
         {res += fprintf(stdout, "%d", va_arg(args, int)); i++;}
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
   // if (token->remove) res += print("[remove] ");
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

int print_escaped(char *str)
{
   if (!str) return 0;
   int r = 0;
#if DEBUG
   r += debug("\"");
   for (int i = 0; str[i]; i++) {
      switch (str[i]) {
      case '\n': r += debug("\\n"); break;
      case '\t': r += debug("\\t"); break;
      case '\r': r += debug("\\r"); break;
      case '\\': r += debug("\\\\"); break;
      case '\"': r += debug("\\\""); break;
      default:
         // print normal character
         r += putchar(str[i]);
         break;
      }
   }
   r += debug("\"");
#endif
   return r;
}

int print_value(Token *token)
{
   switch (token->type)
   {
   case INT: return print("[%lld] ", token->Int.value);
   case LONG: return print("[%lld] ", token->Long.value);
   case BOOL: return print("[%s] ", token->Bool.value ? "True" : "False");
   case FLOAT: return print("[%f] ", token->Float.value);
   case CHAR: return print("[%c] ", token->Char.value);
   case CHARS:
   {
      int r = 0;
      r += print("[");
      r += print_escaped(token->Chars.value);
      r += print("]");
      return r;
   }
   case STRUCT_CALL: return print("has [%d] attrs ", token->Struct.pos);
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
   curr->ir_reg ? print("r%.4d:", curr->ir_reg) : print("rxxxx:");
   int k = 0;
   while (k < curr->space) k += print(" ");
   print("[%-6s] ", to_string(curr->type));

   switch (curr->type)
   {
   case ASSIGN:
   {
      if (curr->assign_type) print("[%s] ", to_string(curr->assign_type));

      print("r%.2d = r%.2d ", left->ir_reg, right->ir_reg);

      // if (left->name) print("(%s) = ", left->name);
      // if (right->name) print("(%s)", right->name);
      // else print_value(right);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case EQUAL: case NOT_EQUAL: case LESS: case MORE:
   case LESS_EQUAL: case MORE_EQUAL:
   case AND: case OR:
   {
      print("r%.2d to r%.2d ", left->ir_reg, right->ir_reg);

      // if (left->name) print("(%s)", left->name);
      // else print_value(left);
      // print(" to ");
      // if (right->name) print("(%s)", right->name);
      // else print_value(right);

      break;
   }
   case NOT:
   {
      print("r%.2d ", left->ir_reg);
      break;
   }
   case INT: case BOOL: case CHARS: case CHAR:
   case FLOAT: case LONG: case VOID: case PTR:
   {
      if (curr->name) print("name %s ", curr->name);
      else print_value(curr);
      break;
   }
   case DOT: print("get attribute [%s] in %k ", right->name, left); break;
   case FCALL: print("%s ", curr->name); break;
   case FDEC: print("%s ", curr->name); break;
   case END_BLOC: print("%s ", curr->name); break;
   case STRUCT_CALL: print("%s ", curr->name); break;
   case STRUCT_DEF: print("%s ", curr->Struct.name); break;
   case BUILD_COND: print("%s ", curr->name); break;
   case RETURN: print("r%.2d ", left->ir_reg); break;
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
      break;
   }
   case AS:
   {
      print("from ");
      if (left->ir_reg) print("r%.2d ", left->ir_reg);
      if (left->name) print("(%s) ", left->name);
      print("to %s", to_string(right->type));
      break;
   }
   case STACK:
   {
      print("allocate");
      break;
   }
   case IF: case ELIF: case END_IF: case ELSE:
   case STRUCT_ALLOC: case STRUCT_BODY:
   case CONTINUE: case BREAK: break;
   default: print(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
   }
   print("\n");
}

void print_ir()
{
   if (!DEBUG || found_error) return;
   print(GREEN BOLD SPLIT RESET);
   print(GREEN BOLD"PRINT IR:\n" RESET);
   int i;
   for (i = 0; insts && insts[i]; i++) print_inst(insts[i]);
   print("total instructions [%d]\n", i);
   print(GREEN BOLD SPLIT RESET);
}

bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...)
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
   // free(token->logName);
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
bool add_file(char *filename)
{
   if (used_files == NULL)
   {
      used_size = 10;
      used_files = allocate(used_size, sizeof(char*));
   }
   else if (used_pos + 1 == used_size)
   {
      char**tmp = allocate((used_size *= 2), sizeof(char*));
      memcpy(tmp, used_files, used_pos * sizeof(char*));
      free(used_files);
      used_files = tmp;
   }
   for (int i = 0; i < used_pos; i++)
   {
      if (strcmp(filename, used_files[i]) == 0)
         return false;
   }
   used_files[used_pos++] = filename;
   return true;
}

Token* new_token(Type type, int space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(new);
   return new;
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

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   // TODO: can't use setName here to investigate later why ?
   if (token->name)
   {
      new->name = strdup(token->name);
      // if (strcmp(new->name, "n") == 0)
      // {
      //    debug(RED"found [%p]\n"RESET, new);
      // }
   }

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

Node *new_node(Token *token)
{
   // print("new node: %k\n", token);
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
   // debug(CYAN "in scoop %k, new struct [%k]\n" RESET, scoop->token, token);
   for (int i = 0; i < scoop->spos; i++)
   {
      // print(GREEN"loop [%d]\n"RESET, i);
      Token *curr = scoop->structs[i];
      check(strcmp(curr->Struct.name, token->Struct.name) == 0,
            "Redefinition of %s\n", token->Struct.name);
   }
   add_struct(scoop, token);
   return token;
}

Token *get_struct(char *name)
{
   // debug(CYAN "get struct [%s] from scoop %k\n"RESET, name, scoop->token);
   for (int j = scoopPos; j > 0; j--)
   {
      Node *node = Gscoop[j];
      todo(node == NULL, RED"Error accessing NULL, %d\n"RESET, j);
      // debug("[%d] scoop [%s] has %d structs\n", j, node->token->name, node->spos);
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->Struct.name, name) == 0)
            return node->structs[i];
   }
   return NULL;
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

Token *new_variable(Token *token)
{
   // debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET, token->name, to_string(token->type), scoop->token);
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      check(strcmp(curr->name, token->name) == 0, "Redefinition of %s\n", token->name);
   }
   add_variable(scoop, token);
   return token;
}

Token *get_variable(char *name)
{
   // debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
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
   for (int i = 0; i < scoop->fpos; i++)
   {
      Node *func = scoop->functions[i];
      bool cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s\n", node->token->name);
   }
   add_function(scoop, node);
   return node;
}

Node *get_function(char *name)
{
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

// IR
void copy_insts()
{
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
   // debug("new inst: %k%c", new->token, token->type != STRUCT_CALL ? '\n' : '\0');
   add_inst(new);
   return new;
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
   if (!node || !node->token)
   {
      check(1, "recieved NULL\n");
      return 0;
   }
   if (includes(node->token->retType, DATA_TYPES, 0))
      return node->token->type;

   Type left = 0, right = 0;
   if (node->left) left = getRetType(node->left);
   if (node->right) right = getRetType(node->right);
   if (left) return left;
   if (right) return right;
   return 0;
}

// LLVM UTILS
ValueRef *farr;
int fpos;
int flen;
ValueRef get_current_func()
{
   return farr[fpos - 1];
}

void enter_func(ValueRef func)
{
   if (fpos == 0)
   {
      flen = 100;
      farr = allocate(flen, sizeof(ValueRef));
   }
   else if (fpos + 1 == flen)
   {
      ValueRef *tmp = allocate(flen *= 2, sizeof(ValueRef));
      memcpy(tmp, farr, fpos * sizeof(ValueRef));
      free(farr);
      farr = tmp;
   }
   // debug("access %d\n", fpos);
   farr[fpos] = func;
   fpos++;
}

void exit_func()
{
   fpos--;
}

// UTILS
char *to_string_(char *filename, int line, Type type)
{
   char *type_names[] = {
      [CHILDREN] = "CHILDREN",

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
      [ARRAY] = "ARRAY", [AS] = "AS", [VARIADIC] = "VARIADIC",
      [STACK] = "STACK", [TYPEOF] = "TYPEOF",


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
      [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
      [END_IF] = "END_IF",
      [BUILD_COND] = "BLD_COND", [WHILE] = "WHILE",
      [CONTINUE] = "CONT", [BREAK] = "BREAK",
      [END_WHILE] = "END_WHILE",
      [APPEND_BLOC] = "APP_BLC", [SET_POS] = "SET_POS",
      [BUILD_BR] = "BLD_BR", [END_BLOC] = "END_BLOC",

      [FDEC] = "FDEC", [FCALL] = "FCALL", [PROTO_FUNC] = "PROTO_FUNC",
   };
   int size = (int)(sizeof(type_names) / sizeof(type_names[0]));
   if (type > 0 && (int)type < size && type_names[type])
      return (char *)type_names[type];
   check(1, "Unknown type [%d] in %s:%d\n", type, filename, line);
   return "";
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
      if (type == tokens[exe_pos]->type)
         return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};

bool within_space(int space)
{
   Token *curr = tokens[exe_pos];
   return !found_error && curr->space > space && curr->type != END;
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

void setName(Token *token, char *name)
{
   if (token->name) free(token->name);
   token->name = name ? strdup(name) : NULL;
}

void enter_scoop(Node *node)
{
   // debug(CYAN "Enter Scoop: %k index %d\n" RESET, node->token, scoopPos + 1);
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
   // debug(CYAN "Exit Scoop: %k index %d\n" RESET, Gscoop[scoopPos]->token, scoopPos);
   Gscoop[scoopPos] = NULL;
   scoopPos--;
   if (scoopPos >= 0) scoop = Gscoop[scoopPos];
}