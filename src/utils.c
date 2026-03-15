#include "ura.h"

// FILE MANAGEMENT
char *open_file(char* filename)
{
   if (found_error) return NULL;

   File file = fopen(filename, "r");
   if (check(!file, "openning %s", filename)) return NULL;
   fseek(file, 0, SEEK_END);
   int   size  = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

char *resolve_path(char* path)
{
   if (path == NULL) return NULL;
   char  *cleaned = allocate(strlen(path) + 5, 1);
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

bool add_file(char *filename)
{
   used_files = (char**)resize_array((void**)used_files,
                                     &used_len, used_pos, sizeof(char *));
   for (int i = 0; i < used_pos; i++)
   {
      if (strcmp(filename, used_files[i]) == 0)
         return false;
   }
   used_files[used_pos++] = filename;
   return true;
}

// MEMORY
void *allocate_func(int line, int len, int size)
{
   void *res = calloc(len, size);
   check(!res, "allocate did failed in line %d", line);
   return res;
}

void **resize_array(void** array, int* len, int pos, int element_size)
{
   if (array == NULL)
   {
      *len = 10;
      return allocate(*len, element_size);
   }
   if (pos + 5 >= *len)
   {
      void **tmp = allocate(*len * 2, element_size);
      memcpy(tmp, array, (*len) * element_size);
      free(array);
      *len *= 2;
      return tmp;
   }
   return array;
}

void free_token(Token *token)
{
   free(token->name);
   free(token->Chars.value);
   free(token);
}

void free_node(Node* node)
{
   if (!node) return;
   for (int i = 0; i < node->cpos; i++) free_node(node->children[i]);
   free_node(node->left);
   free_node(node->right);
   free(node->children);
   free(node->functions);
   free(node->variables);
   free(node->structs);
   free(node->methods);
   free(node);
}

void free_memory()
{
   for (int i = 0; tokens && tokens[i]; i++)
   {
      free_token(tokens[i]);
      tokens[i] = NULL;
   }
   tk_pos  = 0;
   exe_pos = 0;
}

// PARSING HELPERS
void add_token(Token *token)
{
   tokens = (Token**)resize_array(
      (void**)tokens,
      &tk_len, tk_pos, sizeof(Token *));
   tokens[tk_pos++] = token;
}

Node *new_node(Token *token)
{
   debug("new node: %k\n", token);
   Node *new = allocate(1, sizeof(Node));
   new->token = token;
   return new;
}

Node *add_child(Node *node, Node *child)
{
   if (child)
   {
      node->children = (Node**)resize_array(
         (void**)node->children,
         &node->clen,node->cpos,sizeof(Node *));
      child->token->space          = node->token->space + TAB;
      node->children[node->cpos++] = child;
   }
   return child;
}

void enter_scope(Node *node)
{
   debug(CYAN "Enter Scope: %k index %d\n" RESET, node->token, scope_pos + 1);
   Gscope = (Node**)resize_array(
      (void**)Gscope,
      &scope_len, scope_pos, sizeof(Node*));
   scope_pos++;
   Gscope[scope_pos] = node;
   scope             = Gscope[scope_pos];
}

void exit_scope()
{
   if (check(scope_pos < 0, "No active scope to exit")) return;
   debug("%sExit Scope: %k index %d%s\n", CYAN, Gscope[scope_pos]->token, scope_pos, RESET);
   Gscope[scope_pos] = NULL;
   scope_pos--;
   scope             = Gscope[scope_pos];
}

bool includes(Type to_find, ...)
{
   if (found_error) return false;
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0)
      if (current == to_find) return true;
   return false;
}

void setName(Token *token, char *name)
{
   if (token->name) free(token->name);
   token->name = name ? strdup(name) : NULL;
}

char *to_string(Type type)
{
   char *res[END + 1] =
   {
      [ID]         = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
      [INT]        = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
      [FDEC]       = "FDEC",
      [FCALL]      = "CALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
      [IF]         = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
      [WHILE]      = "WHILE", [BREAK] = "BRK", [CONTINUE] = "CONT",
      [RETURN]     = "RET",
      [BAND]       = "BAND", [BOR] = "BOR", [BXOR] = "BXOR",
      [BNOT]       = "BNOT", [LSHIFT] = "LSHIFT", [RSHIFT] = "RSHIFT",
      [ADD]        = "ADD",
      [SUB]        = "SUB", [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
      [ADD_ASSIGN] = "ADD_ASS", [SUB_ASSIGN] = "SUB_ASS",
      [MUL_ASSIGN] = "MUL_ASS", [DIV_ASSIGN] = "DIV_ASS",
      [MOD_ASSIGN] = "MOD_ASS", [ACCESS] = "ACC",
      [MOD]        = "MOD", [COMA] = "COMA", [REF] = "REF",
      [EQUAL]      = "EQ", [NOT_EQUAL] = "NEQ", [LESS] = "LT",
      [GREAT]      = "GT", [LESS_EQUAL] = "LE", [NOT] = "NOT",
      [GREAT_EQUAL]= "GE", [AND] = "AND", [OR] = "OR",
      [DOTS]       = "DOTS",  [PROTO] = "PROT", [VARIADIC] = "VAR",
      [TYPEOF]     = "TYPEOF", [SIZEOF] = "SIZEOF", [OUTPUT] = "OUTPUT", [ARGS] = "ARGS",
      [CHILDREN]   = "CHILDREN",
      [AS]         = "AS", [STACK] = "STACK", [HEAP] = "HEAP",
      [ARRAY_TYPE] = "ARRAY_TYPE",
      //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
      [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
      [ENUM_DEF]   = "ENUM_DEF",   [ENUM_CALL]   = "ENUM_CALL",
      [TUPLE]      = "TUPLE",      [TUPLE_UNPACK] = "TUPLE_UNPACK",
      [LBRA]       = "LBRA", [RBRA] = "RBRA", [ARRAY] = "ARRAY",
      [DOT]        = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR",
   };

   if (!res[type])
   {
      printf("%s:%d handle this case %d\n", FILE, LINE, type);
      // seg();
      exit(1);
   }
   return res[type];
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

bool within(int space)
{
   Token *curr = tokens[exe_pos];
   return !found_error && curr->space > space && curr->type != END;
}

void add_struct(Node *b, Node *node)
{
   b->structs = (Node**)resize_array(
      (void**)b->structs,
      &b->slen, b->spos, sizeof(Node*));
   b->structs[b->spos++] = node;
}

Node *new_struct(Node *node)
{
   debug(CYAN "new struct [%s] in scope %k\n" RESET, node->token->name, scope->token);
   for (int i = 0; i < scope->spos; i++)
   {
      Token *curr = scope->structs[i]->token;
      bool   cond = (strcmp(curr->name, node->token->name) == 0);
      check(cond, "Redefinition of %s", node->token->name);
   }
   add_struct(scope, node);
   return node;
}

Node *get_struct(char *name)
{
   debug(CYAN "get struct [%s] from scope %k\n"RESET, name, scope->token);
   for (int j = scope_pos; j > 0; j--)
   {
      Node *node = Gscope[j];
      todo(node == NULL, RED "Error accessing NULL, %d"RESET, j);
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->token->name, name) == 0)
            return node->structs[i];
   }
   return NULL;
}

void add_method(Node *b, Node *node)
{
   b->methods = (Node**)resize_array(
      (void**)b->methods, &b->mlen,
      b->mpos, sizeof(Node*));
   b->methods[b->mpos++] = node;
}

Node *syntax_error()
{
   found_error = true;
   static Node *node;
   if (node == NULL) node = new_node(new_token(SYNTAX_ERROR, -1));
   return node;
   return node;
}

// IR / SCOPE / VARIABLE MANAGEMENT
void add_variable(Node *b, Token *token)
{
   b->variables = (Token**)resize_array(
      (void**)b->variables,
      &b->vlen, b->vpos, sizeof(Token *));
   b->variables[b->vpos++] = token;
}

Token *new_variable(Token *token)
{
   debug(CYAN "new variable [%k] in scope %k\n" RESET, token, scope->token);
   for (int i = 0; i < scope->vpos; i++)
   {
      Token *curr = scope->variables[i];
      bool   cond = (strcmp(curr->name, token->name) == 0);
      check(cond, "Redefinition of %s", token->name);
   }
   add_variable(scope, token);
   return token;
}

Token *get_variable(char *name)
{
   debug(CYAN "get variable [%s] from scope %k\n" RESET, name, scope->token);
   for (int j = scope_pos; j > 0; j--)
   {
      Node *scope = Gscope[j];
      for (int i = 0; i < scope->vpos; i++)
         if (strcmp(scope->variables[i]->name, name) == 0)
            return scope->variables[i];
   }
   check(1, "%s not found", name);
   // seg();
   return syntax_error()->token;
}

void add_function(Node *b, Node *node)
{
   b->functions = (Node**)resize_array(
      (void**)b->functions,
      &b->flen, b->fpos, sizeof(Node*));
   b->functions[b->fpos++] = node;
}

Node *new_function(Node *node)
{
   for (int i = 0; i < scope->fpos; i++)
   {
      Node *func = scope->functions[i];
      bool  cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s", node->token->name);
   }
   add_function(scope, node);
   return node;
}

Node *get_function(char *name)
{
   for (int j = scope_pos; j > 0; j--)
   {
      Node *scope = Gscope[j];
      for (int i = 0; i < scope->fpos; i++)
         if (strcmp(scope->functions[i]->token->name, name) == 0)
            return scope->functions[i];
   }
   check(1, "'%s' Not found", name);
   return syntax_error();
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   new->name = NULL;
   if (token->name) setName(new, token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   add_token(new);
   return new;
}

// DEBUGGING AND ERROR HANDLING
int vprint_(File out, char *conv, va_list args)
{
   int res = 0;

   for (int i = 0; conv[i]; i++)
   {
      if (conv[i] != '%') { res += fprintf(out, "%c", conv[i]); continue; }

      i++;
      int left_align = 0;
      if (conv[i] == '-') { left_align = 1; i++; }

      int width = 0;
      while (isdigit(conv[i])) { width = width * 10 + (conv[i] - '0'); i++; }

      int prec = -1;
      if (conv[i] == '.')
      {
         i++; prec = 0;
         while (conv[i] >= '0' && conv[i] <= '9') { prec = prec * 10 + (conv[i] - '0'); i++; }
      }

      if      (strncmp(conv + i, "zu",  2) == 0) { res += fprintf(out, "%d",   va_arg(args, int));       i++;   }
      else if (strncmp(conv + i, "lld", 3) == 0) { res += fprintf(out, "%lld", va_arg(args, long long)); i += 2; }
      else
      {
         switch (conv[i])
         {
         case 'c': res += fprintf(out, "%c", va_arg(args, int)); break;
         case 's':
         {
            char *str = va_arg(args, char *);
            if (!str) str = "(null_str)";
            if (left_align)
               res += (prec >= 0) ? fprintf(out, "%-*.*s", width, prec, str) : fprintf(out, "%-*s", width, str);
            else
               res += (prec >= 0) ? fprintf(out, "%*.*s",  width, prec, str) : fprintf(out, "%*s",  width, str);
            break;
         }
         case 'p': res += fprintf(out, "%p",   va_arg(args, void *)); break;
         case 'x': res += (prec >= 0) ? fprintf(out, "%.*x", prec, va_arg(args, unsigned int)) : fprintf(out, "%x", va_arg(args, unsigned int)); break;
         case 'X': res += (prec >= 0) ? fprintf(out, "%.*X", prec, va_arg(args, unsigned int)) : fprintf(out, "%X", va_arg(args, unsigned int)); break;
         case 'd': res += (prec >= 0) ? fprintf(out, "%.*d", prec, va_arg(args, int))          : fprintf(out, "%d", va_arg(args, int));          break;
         case 'f': res += (prec >= 0) ? fprintf(out, "%.*f", prec, va_arg(args, double))       : fprintf(out, "%f", va_arg(args, double));       break;
         case '%': res += fprintf(out, "%%"); break;
         case 't': res += fprintf(out, "%s", to_string((Type)va_arg(args, Type))); break;
         case 'k':
         {
            Token *token = va_arg(args, Token *);
            if (!token) { fprintf(out, "(null)"); break; }

            fprintf(out, "[%s] ", to_string(token->type));

            switch (token->type)
            {
            case VOID: case CHARS: case CHAR: case INT:
            case BOOL: case FLOAT: case LONG:
            {
               if (token->name) { fprintf(out, "%s ", token->name); break; }
               if (token->type == VOID) break;
               // print_value inline
               switch (token->type)
               {
               case INT:   fprintf(out, "[%lld] ", (long long)token->Int.value); break;
               case LONG:  fprintf(out, "[%lld] ", token->Long.value); break;
               case BOOL:  fprintf(out, "[%s] ", token->Bool.value ? "True" : "False"); break;
               case FLOAT: fprintf(out, "[%f] ", token->Float.value); break;
               case CHAR:
               {
                  fprintf(out, "[");
                  char c = token->Char.value;
                  switch (c)
                  {
                  case '\n': fprintf(out, "\\n");  break;
                  case '\t': fprintf(out, "\\t");  break;
                  case '\r': fprintf(out, "\\r");  break;
                  case '\\': fprintf(out, "\\\\"); break;
                  case '\"': fprintf(out, "\\\""); break;
                  default:   fprintf(out, "%c", c); break;
                  }
                  fprintf(out, "] ");
                  break;
               }
               case CHARS:
               {
                  fprintf(out, "[\"");
                  char *str = token->Chars.value;
                  if (str)
                     for (int j = 0; str[j]; j++)
                     {
                        switch (str[j])
                        {
                        case '\n': fprintf(out, "\\n");  break;
                        case '\t': fprintf(out, "\\t");  break;
                        case '\r': fprintf(out, "\\r");  break;
                        case '\\': fprintf(out, "\\\\"); break;
                        case '\"': fprintf(out, "\\\""); break;
                        default:   fprintf(out, "%c", str[j]); break;
                        }
                     }
                  fprintf(out, "\"] ");
                  break;
               }
               case ADD: case SUB: case NOT_EQUAL:
                  fprintf(out, "%s ", to_string(token->type)); break;
               default: break;
               }
               break;
            }
            case STRUCT_CALL:
               fprintf(out, "name [%s] ", token->name);
               fprintf(out, "st_name [%s] ", token->Struct.ptr->token->name);
               break;
            case STRUCT_DEF:
            case FCALL: case FDEC: case ID:
               fprintf(out, "%s ", token->name);
               break;
            default: break;
            }

            if (token->is_ref)      fprintf(out, "ref ");
            if (token->ir_bound)    fprintf(out, "bound ");
            if (token->retType)     fprintf(out, "ret [%s] ", to_string(token->retType));
            if (token->is_variadic) fprintf(out, "variadic ");
            break;
         }
         default: todo(1, "invalid format specifier [%c]", conv[i]); break;
         }
      }
   }
   return res;
}

int debug_(char *conv, ...)
{
   va_list args;
   va_start(args, conv);
   int     res = vprint_(stdout, conv, args);
   va_end(args);
   return res;
}

bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   found_error = true;
   fprintf(stderr, BOLD RED "ura_error: %s %s:%d " RESET, funcname, filename, line);
   va_list ap;
   va_start(ap, fmt);
   vprint_(stderr, fmt, ap);
   va_end(ap);
   fprintf(stderr, "\n");
   return cond;
}

void pnode(Node *node, char *indent)
{
   if (!node || !node->token || !DEBUG) return;
   Node **subs     = NULL;
   int    count    = 0;
   int    capacity = 0;

#define push(n) do { \
           subs = (Node **)resize_array( \
              (void **)subs, \
              &capacity, count, sizeof(Node *)); \
           subs[count++] = (n); \
} while(0)

   debug("%k\n", node->token);
   if (includes(node->token->type, IF, ELIF, ELSE, 0))
   {
      if (node->left) push(node->left);
      for (int i = 0; i < node->cpos; i++) push(node->children[i]);
      if (node->right) push(node->right);
   }
   else
   {
      if (node->left)  push(node->left);
      if (node->right) push(node->right);
      for (int i = 0; i < node->cpos; i++) push(node->children[i]);
      if (node->token->type == STRUCT_DEF)
      {
         for (int i = 0; i < node->mpos; i++)
            push(node->methods[i]);
      }
   }
   for (int i = 0; i < count; i++)
   {
      Node *child = subs[i];
      if (!child || !child->token || !child->token->type) continue;

      int         is_last   = (i == count - 1);
      const char *connector = is_last ? "└──" : "├──";
      const char *bar       = is_last ? "   " : "│  ";

      char        new_indent[4096];
      snprintf(new_indent, sizeof(new_indent), "%s%s", indent, bar);

      printf("%s%s", indent, connector);
      pnode(child, new_indent);
   }
   free(subs);
#undef push
}

// STRING
char *strjoin(char *str0, char *str1, char *str2)
{
   int   len0 = str0 ? strlen(str0) : 0;
   int   len1 = str1 ? strlen(str1) : 0;
   int   len2 = str2 ? strlen(str2) : 0;
   char *res  = allocate(len0 + len1 + len2 + 1, 1);
   if (str0) strcpy(res, str0);
   if (str1) strcpy(res + len0, str1);
   if (str2) strcpy(res + len0 + len1, str2);
   return res;
}

// OTHER UTILITIES
bool is_data_type(Token *token)
{
   return includes(token->type, DATA_TYPES, 0);
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
      add_struct(new, node->structs[i]);
   for (int i = 0; i < node->vpos; i++)
      add_variable(new, copy_token(node->variables[i]));
   return new;
}

bool compatible(Token *left, Token *right)
{
   Type lt = left->retType  ? left->retType  : left->type;
   Type rt = right->retType ? right->retType : right->type;

   if (lt == rt) return true;

   if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;

   if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.elem_type == CHAR)
      return true;
   if (rt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && left->Array.elem_type  == CHAR)
      return true;

   if (lt == ARRAY_TYPE && rt == ARRAY) return true;
   if (lt == ARRAY      && rt == ARRAY_TYPE) return true;

   bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
   bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
   if (lt_numeric && rt_numeric) return true;

   if (lt == VOID || rt == VOID) return false;

   if (lt == STRUCT_CALL && rt == STRUCT_CALL)
      return left->Struct.ptr == right->Struct.ptr;
   return false;
}

Type get_ret_type(Node *node)
{
   if (!node || !node->token) return 0;

   Token *token = node->token;
   Node  *left  = node->left;
   Node  *right = node->right;

   if (token->retType) return token->retType;

   // Scalar types and struct return themselves
   if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
      return token->type;
   if (token->type == FCALL)
      return token->Fcall.ptr ? token->Fcall.ptr->token->retType : 0;
   // Arithmetic, bitwise, and assign ops inherit the left operand type
   if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0))
      return get_ret_type(left);
   // Comparison and logical ops always yield bool
   if (includes(token->type, COMPARISON_OPS, AND, OR, NOT, BNOT, 0))
      return BOOL;
   if (token->type == AS)
      return right ? (right->token->retType ?
                      right->token->retType : right->token->type) : 0;
   if (token->type == RETURN) return get_ret_type(left);
   if (token->type == DOT)    return get_ret_type(right);
   if (token->type == ACCESS)
   {
      Type left_type = get_ret_type(left);
      if (left_type == CHARS || left_type == STACK) return CHAR;
      return left_type;
   }
   if (token->type == ID) return token->type != ID ? token->type : 0;
   todo(1, "handled this case [%s]", to_string(token->type));
   return 0;
}

void set_ret_type(Node *node)
{
   if (!node || !node->token) return;
   Type t = get_ret_type(node);
   if (t) node->token->retType = t;

   if (node->token->type == DOT &&
       node->token->retType == STRUCT_CALL && node->right)
      node->token->Struct.ptr = node->right->token->Struct.ptr;

   // For FCALL returning struct, copy Struct.ptr from the function declaration
   if (node->token->type == FCALL &&
       node->token->retType == STRUCT_CALL &&
       node->token->Fcall.ptr)
      node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}
