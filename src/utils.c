#include "header.h"

// DEBUG
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
   debug("\n");
   va_end(ap);
   return cond;
}

int debug_(char *conv, ...)
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
               res += debug("node: ") + (node ?
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

int pspace(int space)
{
   int res = 0;
   for (int i = 0; i < space; i++) res += debug(" ");
   return res;
}

int pnode(Node *node, char *side, int space)
{
   if (!node) return 0;
   int res = 0;
   pspace(space);
   if (side) res += debug("%s", side);

   res += debug("%k\n", node->token);
   if (includes(node->token->type, FDEC, FCALL, STACK, 0))
   {
      if (node->left)
      {
         pspace(space);
         res += debug("params:\n");
         for (int i = 0; i < node->left->cpos; i++)
            res += pnode(node->left->children[i], NULL, space + TAB);
      }
      if (node->cpos)
      {
         pspace(space);
         res += debug("children: \n");
         for (int i = 0; i < node->cpos; i++)
            res += pnode(node->children[i], NULL, space + TAB);
      }
      return res;
   }
   else if (includes(node->token->type, IF, ELIF, ELSE, 0))
   {
      if (node->token->type != ELSE)
      {
         pspace(space);
         res += debug("condition:\n");
         res += pnode(node->left, NULL, space + TAB);
      }
      pspace(space);
      res += debug("children: \n");
      for (int i = 0; i < node->cpos; i++)
         res += pnode(node->children[i], NULL, space + TAB);
      if (node->right) pnode(node->right, NULL, node->right->token->space);
      return res;
   }
   else
   {
      res += pnode(node->left, "L: ", space + TAB);
      res += pnode(node->right, "R: ", space + TAB);
   }
   if (node->children)
   {
      for (int i = 0; i < space; i++) res += debug(" ");
      res += debug("children: \n");
      for (int i = 0; i < node->cpos; i++)
         res += pnode(node->children[i], NULL, space + TAB);
   }
   return res;
}

int ptoken(Token *token)
{
   int res = 0;
   if (!token) return debug("null token");
   res += debug("[%-8s] ", to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT:
   case BOOL: case FLOAT: case LONG:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      else if (token->type != VOID) print_value(token);
      break;
   }
   case STRUCT_CALL: case STRUCT_DEF:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      res += debug("st_name [%s] ", token->Struct.name);
      res += debug("attributes:\n");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
         for (int j = 0; j < token->space + TAB; ) j += debug(" ");
         res += ptoken(attr);
         debug("\n");
      }
      break;
   }
   case FCALL: case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   if (token->ir_reg) res += debug("ir_reg [%d] ", token->ir_reg);
   if (token->is_ref) debug("ref ");
   if (token->has_ref) debug("has_ref ");
   if (token->retType) res += debug("ret [%t] ", token->retType);
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
      default: r += putchar(str[i]); break;
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
   case INT: return debug("[%lld] ", token->Int.value);
   case LONG: return debug("[%lld] ", token->Long.value);
   case BOOL: return debug("[%s] ", token->Bool.value ? "True" : "False");
   case FLOAT: return debug("[%f] ", token->Float.value);
   case CHAR: return debug("[%c] ", token->Char.value);
   case CHARS:
   {
      int r = 0;
      r += debug("[");
      r += print_escaped(token->Chars.value);
      r += debug("]");
      return r;
   }
   case STRUCT_CALL: return debug("has [%d] attrs ", token->Struct.pos);
   case ADD: case SUB: case NOT_EQUAL: return debug("%t ", token->type); break;
   case VOID: break;
   default: check(1, "handle this case [%s]\n", to_string(token->type));
   }
   return 0;
}

// TOKENIZE
char *to_string(Type type)
{
   char* res[END + 1] = {
      [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
      [INT] = "INT", [BOOL] = "BOOL", [FDEC] = "FDEC",
      [FCALL] = "FCALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
      [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE", [END_IF] = "END_IF",
      [WHILE] = "WHILE", [BREAK] = "BREAK", [CONTINUE] = "CONTINUE",
      [RETURN] = "RETURN", [ADD] = "ADD",
      [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
      [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
      [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
      [MOD_ASSIGN] = "MOD_ASSIGN", [ACCESS] = "ACCESS",
      [MOD] = "MOD", [COMA] = "COMA", //[REF] = "REF",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQUAL", [LESS] = "LESS",
      [MORE] = "MORE", [LESS_EQUAL] = "LESS_EQUAL", [NOT] = "NOT",
      [MORE_EQUAL] = "MORE_EQUAL", [AND] = "AND", [OR] = "OR",
      [DOTS] = "DOTS", //[COLON] = "COLON", [COMMA] = "COMMA",
      [PROTO] = "PROTO", [VARIADIC] = "VARIADIC",
      //[VA_LIST] = "VA_LIST",
      [AS] = "AS", [END_BLOC] = "END_BLOC",
      [STACK] = "STACK",
      //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW",
      //[USE] = "USE",
      [LBRA] = "LBRA", [RBRA] = "RBRA",
      [DOT] = "DOT",
   };
   if (!res[type])
   {
      printf("%s:%d handle this case %d\n", __FILE__, __LINE__, type);
      seg();
      exit(1);
   }
   return res[type];
}

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

Token* new_token(Type type, int space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(new);
   return new;
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

Token *parse_token(char *filename, int line, char *input, int s, int e,  Type type, int space)
{
   Token *new = new_token(type, space);
   new->line = line;
   new->filename = filename;
   new->pos = tk_pos;

   switch (type)
   {
   case INT:
   {
      // new->logType = VALUE;
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   case ID:
   {
      // new->logType = SYMBOL;
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      // new->logName = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      // strncpy(new->logName, input + s, e - s);
      int i = 0;

      struct { char *name; bool value; } bools[] = {{"True", true}, {"False", false}, {0, 0}};
      for (i = 0; bools[i].name; i++)
      {
         if (strcmp(new->name, bools[i].name) == 0)
         {
            setName(new, NULL);
            new->type = BOOL;
            new->Bool.value = bools[i].value;
            break;
         }
      }
      if (new->name == NULL) break;

      struct { char *name; Type type; } dataTypes [] =
      {
         {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
         {"char", CHAR}, {"float", FLOAT}, {"void", VOID},
         {"long", LONG}, {"pointer", PTR},
         {"short", SHORT}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->is_dec = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {
         {"if", IF},  {"elif", ELIF}, {"else", ELSE},
         {"while", WHILE},  {"fn", FDEC}, {"return", RETURN},
         {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF},
         {"struct", STRUCT_DEF}, {"proto", PROTO}, {"as", AS}, {0, 0},
      };
      for (i = 0; keywords[i].name; i++)
      {
         if (strcmp(keywords[i].name, new->name) == 0)
         {
            new->type = keywords[i].type;
            break;
         }
      }
      if (keywords[i].name) break;

      struct { char *name; Type type; } keywords2[] = {{"and", AND},
         {"or", OR}, {"is", EQUAL}, {"not", NOT}, {"typeof", TYPEOF}, {0, 0},
      };
      for (i = 0; keywords2[i].name; i++)
      {
         if (strcmp(keywords2[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = keywords2[i].type;
            break;
         }
      }
      break;
   }
   case FDEC:
   {
      // new->logType = SYMBOL;
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      // new->logType = VALUE;
      if (e <= s) break;
      int len = e - s;
      new->Chars.value = allocate(len + 1, sizeof(char));
      char *value = new->Chars.value;
      int j = 0;

      for (int i = 0; i < len && s < e; i++, s++)
      {
         if (s + 1 < e && input[s] == '\\')
         {
            switch (input[s + 1])
            {
            case 'n': value[j++] = '\n'; s++; break;  // newline
            case 't': value[j++] = '\t'; s++; break;  // tab
            case 'r': value[j++] = '\r'; s++; break;  // carriage return
            case 'b': value[j++] = '\b'; s++; break;  // backspace
            case 'f': value[j++] = '\f'; s++; break;  // form feed
            case 'v': value[j++] = '\v'; s++; break;  // vertical tab
            case 'a': value[j++] = '\a'; s++; break;  // alert (bell)
            case '0': value[j++] = '\0'; s++; break;  // null character
            case '\\': value[j++] = '\\'; s++; break; // backslash
            case '"': value[j++] = '"'; s++; break;   // double quote
            case '\'': value[j++] = '\''; s++; break; // single quote
            case '?': value[j++] = '\?'; s++; break;  // question mark (for trigraphs)
            // Three digit octal
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
               if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
               {
                  int octal = (input[s + 1] - '0') * 64 +
                              (input[s + 2] - '0') * 8 +
                              (input[s + 3] - '0');
                  if (octal <= 255) { value[j++] = (char)octal; s += 3; }
                  // Invalid octal, keep backslash
                  else value[j++] = input[s];
               }
               // Two digit octal
               else if (s + 2 < e && isdigit(input[s + 2]))
               {
                  int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
                  value[j++] = (char)octal;
                  s += 2;
               }
               // Single digit octal
               else { value[j++] = (char)(input[s + 1] - '0'); s++; }
               break;
            case 'x':
               // Hexadecimal escape sequences (\x00 to \xFF)
               if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
               {
                  int hex = 0;
                  char c1 = input[s + 2];
                  char c2 = input[s + 3];

                  // Convert first hex digit
                  if (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
                  else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
                  else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;

                  // Convert second hex digit
                  if (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
                  else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
                  else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);

                  value[j++] = (char)hex;
                  s += 3;
               }
               else value[j++] = input[s]; // Invalid hex escape, keep backslash
               break;
            case 'u':
               // Unicode escape sequences (basic support)
               // \uXXXX - 4 hex digits for Unicode, basic implementation
               if (s + 5 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;
            case 'U':
               // \UXXXXXXXX - 8 hex digits for Unicode, basic implementation
               if (s + 9 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;

            default:
               // Unknown escape sequence, keep the backslash
               value[j++] = input[s];
               break;
            }
         }
         else value[j++] = input[s];
      }
      if (j < len)
      {
         char *resized = allocate(j + 1, sizeof(char));
         strcpy(resized, value);
         new->Chars.value = resized;
      }
      break;
   }
   case CHAR:
   {
      // new->logType = VALUE;
      if (e == s + 1) new->Char.value = input[s];
      else // is special character
      {
         switch (input[s + 1])
         {
         case 'n':  new->Char.value = '\n';  break;
         case 't':  new->Char.value = '\t';  break;
         case 'r':  new->Char.value = '\r';  break;
         case 'b':  new->Char.value = '\b';  break;
         case 'f':  new->Char.value = '\f';  break;
         case 'v':  new->Char.value = '\v';  break;
         case 'a':  new->Char.value = '\a';  break;
         case '0':  new->Char.value = '\0';  break;
         case '\\': new->Char.value = '\\';  break;
         case '"':  new->Char.value = '"'; break;
         case '\'': new->Char.value = '\'';  break;
         case '?':  new->Char.value = '\?';  break;
         default: break;
         }
      }
      break;
   }
   default:
      // todo(1 , "implement adding name for this one %s\n", to_string(type));
      break;
   }
   return new;
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

// AST
Node *new_node(Token *token)
{
   // debug("new node: %k\n", token);
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
      add_variable(new, copy_token(node->variables[i]));
   return new;
}

Node* add_child(Node *node, Node *child)
{
   if (node->clen == 0)
   {
      node->clen = 10;
      node->children = allocate(node->clen, sizeof(Node *));
   }
   else if (node->cpos + 1 == node->clen)
   {
      Node **tmp = allocate(node->clen * 2, sizeof(Node *));
      memcpy(tmp, node->children, node->clen * sizeof(Node *));
      free(node->children);
      node->children = tmp;
      node->clen *= 2;
   }
   child->token->space = node->token->space + TAB;
   node->children[node->cpos++] = child;
   return child;
}

void enter_scoop(Node *node)
{
   // debug(CYAN "Enter Scoop: %k index %d\n" RESET, node->token, scoop_pos + 1);
   if (Gscoop == NULL)
   {
      scoopSize = 10;
      Gscoop = allocate(scoopSize, sizeof(Node*));
   }
   else if (scoop_pos + 1 >= scoopSize)
   {
      Node **tmp = allocate(scoopSize * 2, sizeof(Node*));
      memcpy(tmp, Gscoop, scoop_pos * sizeof(Node*));
      scoopSize *= 2;
      free(Gscoop);
      Gscoop = tmp;
   }
   scoop_pos++;
   Gscoop[scoop_pos] = node;
   scoop = Gscoop[scoop_pos];
}

void exit_scoop()
{
   if (check(scoop_pos < 0, "No active scoop to exit\n")) return;
   Gscoop[scoop_pos] = NULL;
   scoop_pos--;
   if (scoop_pos >= 0) scoop = Gscoop[scoop_pos];
}

void add_struct(Node *bloc, Token *token)
{
   if (bloc->structs == NULL)
   {
      bloc->slen = 10;
      bloc->structs = allocate(bloc->slen, sizeof(Token *));
   }
   else if (bloc->spos + 1 == bloc->slen)
   {
      Token **tmp = allocate(bloc->slen *= 2, sizeof(Token *));
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
      // debug(GREEN"loop [%d]\n"RESET, i);
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
   for (int j = scoop_pos; j > 0; j--)
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
   if (bloc->variables == NULL)
   {
      bloc->vlen = 10;
      bloc->variables = allocate(bloc->vlen, sizeof(Token *));
   }
   else if (bloc->vpos + 1 == bloc->vlen)
   {
      Token **tmp = allocate(bloc->vlen *= 2, sizeof(Token *));
      memcpy(tmp, bloc->variables, bloc->vpos * sizeof(Token *));
      free(bloc->variables);
      bloc->variables = tmp;
   }
   bloc->variables[bloc->vpos++] = token;
}

Token *new_variable(Token *token)
{
   debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET,
         token->name, to_string(token->type), scoop->token);
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->variables[i];
      check(strcmp(curr->name, token->name) == 0, "Redefinition of %s\n", token->name);
   }
   add_variable(scoop, token);
   return token;
}

Token *get_variable(char *name)
{
   // debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
   for (int j = scoop_pos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->variables[i]->name, name) == 0)
            return scoop->variables[i];
   }
   check(1, "%s not found\n", name);
   return NULL;
}

void add_function(Node *bloc, Node *node)
{
   if (bloc->functions == NULL)
   {
      bloc->flen = 10;
      bloc->functions = allocate(bloc->flen, sizeof(Node *));
   }
   else if (bloc->fpos + 1 == bloc->flen)
   {
      bloc->flen *= 2;
      Node **tmp = allocate(bloc->flen, sizeof(Node *));
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
   for (int j = scoop_pos; j > 0; j--)
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

// CODE GEN
void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   vd = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1 = LLVMInt1TypeInContext(context);
   i8 = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8 = LLVMPointerType(i8, 0);
   p32 = LLVMPointerType(i32, 0);

   LLVMInitializeAllTargetInfos();
   LLVMInitializeAllTargets();
   LLVMInitializeAllTargetMCs();
   LLVMInitializeAllAsmParsers();
   LLVMInitializeAllAsmPrinters();
   LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
}

void finalize(char *output)
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, RED"Module verification failed:\n%s\n"RESET, error);
      LLVMDisposeMessage(error);
   }
   LLVMPrintModuleToFile(module, output, NULL);

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}

void _load(Token *to, Token *from)
{
   TypeRef type = get_llvm_type(from);

   char *name = to->name ? to->name : to_string(to->type);
   Value source = from->llvm.elem;

   to->llvm.elem = llvm_build_load2(to, type, source, name);
}

// UTILS
void setName(Token *token, char *name)
{
   if (token->name) free(token->name);
   token->name = name ? strdup(name) : NULL;
}

bool within(int space)
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

char* open_file(char *filename)
{
   if (found_error) return NULL;

   File file = fopen(filename, "r");
   if (check(!file, "openning %s", filename)) return NULL;
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

// MEMORY
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
   free(node->variables);
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