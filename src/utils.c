#include "./include/header.h"

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
   if
   (
      type >= 1 &&
      type < sizeof(type_strings) / sizeof(type_strings[0]) &&
      type_strings[type] != NULL
   )
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

void open_file()
{
   struct _IO_FILE *file = fopen(obj.filename, "r");
   fseek(file, 0, SEEK_END);
   size_t size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   obj.input = input;
}

void free_token(Token *token)
{
   if (token->name) free(token->name);
   if (token->creg) free(token->creg);
   if (token->Chars.value) free(token->Chars.value);
   free(token);
}

void free_node(Node *node)
{
   if(!node) return;
   free_node(node->left);
   free_node(node->right);
   free(node);
}

void free_memory()
{
   free(obj.input);
   for (int i = 0; obj.tokens && obj.tokens[i]; i++) free_token(obj.tokens[i]);
   free_node(obj.head);
}

void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return;
   obj.is_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "%sError:%s:%s:%d %s", RED, filename, funcname, line, RESET);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
}

int ptoken(Token *token)
{
   int res = 0;
   if (!DEBUG) return res;

   res += debug("[%t] ", token->type);
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
   if(side) res += debug("%s", side);
   res += debug("%k\n", node->token);
   if(node->token)
   {
      switch(node->token->type)
      {
         case FDEC:
         {
   #if 1
            node = node->right;
            while(node)
            {
               res += pnode(node->left, NULL , space + TAB);
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
            case 'c': res += fprintf(stdout, "%c", va_arg(args, int)); break;
            case 's': res += fprintf(stdout, "%s", va_arg(args, char *)); break;
            case 'p': res += fprintf(stdout, "%p", (void*)(va_arg(args, void *))); break;
            case 'x': res += fprintf(stdout, "%x", va_arg(args, unsigned int)); break;
            case 'X': res += fprintf(stdout, "%X", va_arg(args, unsigned int)); break;
            case 'd': res += fprintf(stdout, "%d", (int)va_arg(args, int)); break;
            case 'f': res += fprintf(stdout, "%f", va_arg(args, double)); break;
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
               Node *node = (Node*)va_arg(args, Node*);
               res += node ? pnode(node, NULL, node->token->space) : fprintf(stdout, "(null)");
               break;
            }
            default:
               check(true, "invalid precedent [%c]", conv[i]);
               exit(1);
               break;
            }
         }
      }
      else res += fprintf(stdout, "%c", conv[i]);
      i++;
   }
   return res;
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

void setReg(Token *token, char *creg)
{
   if (token->creg)
   {
      free(token->creg);
      token->creg = NULL;
   }
   if (creg) token->creg = strdup(creg);
}

bool within_space(size_t space)
{
   return obj.tokens[obj.exe_pos]->space > space && obj.tokens[obj.exe_pos]->type != END;
}

void add_token(Token *token)
{
   static size_t pos;
   static size_t len;
   if (len == 0)
   {
      len = 10;
      obj.tokens = allocate(len, sizeof(Token *));
   }
   else if (pos + 1 == len)
   {
      Token **tmp = allocate(len * 2, sizeof(Token *));
      memcpy(tmp, obj.tokens, len * sizeof(Token *));
      free(obj.tokens);
      obj.tokens = tmp;
      len *= 2;
   }
   obj.tokens[pos++] = token;
}

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
      if (type == obj.tokens[obj.exe_pos]->type) return obj.tokens[obj.exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};
