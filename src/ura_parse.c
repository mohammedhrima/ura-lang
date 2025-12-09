#include "ura_header.h"

/*
SECTIONS:
   + TOKENIZE
   + AST
   + IR
*/

// TOKENIZE
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
      new->logType = VALUE;
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   case ID:
   {
      new->logType = SYMBOL;
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      new->logName = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      strncpy(new->logName, input + s, e - s);
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
         {"long", LONG}, {"pointer", PTR}, {"array", ARRAY},
         {"short", SHORT}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->is_declare = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {{"if", IF}, {"elif", ELIF},
         {"else", ELSE}, {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
         {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF}, {"struct", STRUCT_DEF},
         {"protoFunc", PROTO_FUNC}, {"as", AS}, {0, 0},
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
      new->logType = SYMBOL;
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      new->logType = VALUE;
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
      new->logType = VALUE;
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
   // debug("new %k\n", new);
}

void tokenize(char *filename)
{
   char *input = open_file(filename);
   if (found_error) return;

   static bool calling_use = false;
   debug(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);

   struct { char *value; Type type; } specials[] = {
      {"->", ARROW}, {"...", VARIADIC},
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL}, {"!", NOT},
      {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
      {"[", LBRA}, {"]", RBRA}, {",", COMA}, {"&&", AND}, {"||", OR},
      {0, (Type)0}
   };

   int space = 0;
   int line = 0;
   bool new_line = true;

   if (!add_file(filename)) return;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      char c = input[i];

      if (isspace(c))
      {
         if (c == '\n')
         {
            line++;
            new_line = true;
            space = 0;
         }
         else if (new_line) space += (c == '\t') ? TAB : 1;
         i++;
         continue;
      }
      if (strncmp(input + i, "/*", 2) == 0)
      {
         i += 2;
         while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2))
         {
            if (input[i] == '\n') line++;
            i++;
         }
         check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'\n");
         i += 2;
         continue;
      }
      if (strncmp(input + i, "//", 2) == 0)
      {
         while (input[i] && input[i] != '\n') i++;
         continue;
      }
      new_line = false;
      if (c == '\"')
      {
         i++;
         while (input[i] && input[i] != '\"')
         {
            if (input[i] == '\\') i++;
            i++;
         }
         check(input[i] != '\"', "Expected '\"'");
         parse_token(filename, line, input, s + 1, i, CHARS, space);
         i++;
         continue;
      }
      if (c == '\'')
      {
         i++;
         if (input[i] == '\\') i++;
         if (input[i] && input[i] != '\'') i++;
         check(input[i] != '\'', "Expected '\''");
         parse_token(filename, line, input, s + 1, i, CHAR, space);
         i++;
         continue;
      }
      if (isalpha(c) || strchr("@$_", c))
      {
         while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i]))) i++;
         if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i]))
         {
            while (isspace(input[i])) i++;
            check(input[i] != '\"', "expected \" after use"); i++;
            s = i;
            while (input[i] && input[i] != '\"') i++;
            check(input[i] != '\"', "expected \"");

            char *use = strndup(input + s, i - s);
            i++;

            char *use_filename = strjoin(dirname(filename), "/", use);
            free(use);
            use = strjoin(use_filename, ".ura", NULL);
            bool old = calling_use;
            calling_use = true;
            tokenize(use);
            calling_use = old;
            free(use_filename);
         }
         else parse_token(filename, line, input, s, i, ID, space);
         continue;
      }
      if (isdigit(c))
      {
         while (isdigit(input[i])) i++;
         parse_token(filename, line, input, s, i, INT, space);
         continue;
      }
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         size_t len = strlen(specials[j].value);
         if (strncmp(specials[j].value, input + i, len) == 0)
         {
            Token *curr = parse_token(filename, line, NULL, 0, 0, specials[j].type, space);
            curr->logName = strdup(specials[j].value);
            curr->logType = SYMBOL;
            i += len;
            if (includes(specials[j].type, ARROW, DOTS, 0)) space += TAB;
            found = true;
            break;
         }
      }
      if (found) continue;
      check(1, "Syntax error <%c>\n", c);
   }
   if (!calling_use) new_token(END, -1);
   free(input);
}

// AST
Node *expr()
{
   return assign();
}

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, 0)))
   {
      Node *node = new_node(token);
      if (token->type == ASSIGN)
      {
         node->left = left;
         node->right = logic();
      }
      else
      {
         node->left = left;
         node->right = new_node(NULL);
         *(node->right) = (Node) {
            .token = new_token(0, node->token->space),
            .left = new_node(left->token),
            .right = logic(),
         };
         switch (token->type)
         {
         case ADD_ASSIGN: node->right->token->type = ADD; break;
         case SUB_ASSIGN: node->right->token->type = SUB; break;
         case MUL_ASSIGN: node->right->token->type = MUL; break;
         case DIV_ASSIGN: node->right->token->type = DIV; break;
         case MOD_ASSIGN: node->right->token->type = MOD; break;
         default: break;
         }
         node->token->type = ASSIGN;
      }
      left = node;
   }
   return left;
}

AST_NODE(logic, equality, AND, OR, 0);
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison, add_sub, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub, mul_div, ADD, SUB, 0);
AST_NODE(mul_div, dot, MUL, DIV, MOD, 0);

Node *dot()
{
   Node *left = brackets();
   Token *token;
   while ((token = find(DOT, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      token = find(ID, 0);
      todo(!token, "error, expected id after dot");
      node->right = new_node(token);
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
   Token *token;
   if ((token = find(SUB, 0)))
   {
      // TODO: left should be a number
      Node *node = new_node(token);
      token->type = MUL;
      node->left = cast_node();
      node->right = new_node(new_token(INT, node->left->token->space));
      node->right->token->Int.value = -1;
      return node;
   }
   return cast_node();
}

Node *func_dec(Node *node)
{
   // Function Declaration:
   //    + left children: arguments
   //    + children     : code block
   Token *typeName = find(DATA_TYPES, 0);
   check(!typeName, "Expected data type after func declaration");
   if (typeName->type == ID)
   {
      typeName = get_struct(typeName->name);
      todo(1, "handle function return struct properly");
   }
   Token *fname = find(ID, 0);
   if (check(!typeName || !fname, "expected data type and identifier after func declaration"))
      return node;
   node->token->retType = typeName->type;
   node->token->is_proto = (node->token->type == PROTO_FUNC);
   node->token->type = FDEC;
   setName(node->token, fname->name);
   enter_scoop(node);

   check(!find(LPAR, 0), "expected ( after function declaration");
   node->left = new_node(new_token(CHILDREN, node->token->space));
   Token *last;
   while (!found_error && !(last = find(RPAR, END, 0)))
   {
      if (find(VARIADIC, 0))
      {
         node->token->is_variadic = true;
         last = find(RPAR, 0);
         check(!last, "expected ) after function ... in variadic function");
         break;
      }
      else
      {
         bool is_ref = find(REF, 0) != NULL;
         Token* data_type = find(DATA_TYPES, ID, 0);
         if (data_type && data_type->type == ID)
         {
            data_type = get_struct(data_type->name);
            if (data_type) data_type->type = STRUCT_CALL;
         }
         // debug(">>> %k <<<<\n", tokens[exe_pos]);
         if (check(!data_type, "expected data type in function argument")) break;
         Token *name = find(ID, 0);
         if (check(!name, "expected identifier in function argument %s", fname->name)) break;
         Node *curr;
         if (data_type->type == STRUCT_CALL)
         {
            curr = new_node(data_type);
            data_type->is_ref = is_ref;
            setName(data_type, name->name);
         }
         else
         {
            curr = new_node(name);
            name->is_ref = is_ref;
            name->type = data_type->type;
         }
         curr->token->is_declare = true;
         add_child(node->left, curr);
      }
      find(COMA, 0); // TODO: check this later
   }
   check(!found_error && last->type != RPAR, "expected ) after function declaration");

   if (!node->token->is_proto)
   {
      Token *next = find(DOTS, ARROW, 0);
      check(!found_error && !next, "Expected : after function declaration");
      Node *child = NULL;

      if (next->type == DOTS) while (within_space(node->token->space)) child = add_child(node, expr());
      else
      {
         todo(1, "stop");
         Token *retToken = copy_token(next);
         retToken->type = RETURN;
         Node *retNode = new_node(retToken);
         retNode->left = expr();
         child = add_child(node, retNode);
      }
      if (next->type == DOTS)
      {
         // TODO: check if function has if/else
         if (node->token->retType != VOID)
            check(!child || child->token->type != RETURN, "expected return statment %s", node->token->name);
         else
         {
            Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
            ret->left = new_node(new_token(VOID, node->token->space + TAB));
            add_child(node, ret);
         }
      }
   }
   exit_scoop();
   return node;
}

Node *func_call(Node *node)
{
   // Function call:
   //    + children: Parameters
   node->token->type = FCALL;
   Token *arg = NULL;
   Token *token = node->token;

   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      Node *curr = expr();
      curr->token->space = token->space;
      add_child(node, curr);
      find(COMA, 0);
   }
   check(!found_error && arg->type != RPAR, "expected ) after function call");
   return node;
}

Node *func_main(Node *node)
{
   // Function main:
   //    + children: code bloc
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
      last = new_node(new_token(RETURN, node->token->space + TAB));
      last->left = new_node(new_token(INT, node->token->space + TAB));
      add_child(node, last);
   }
   exit_scoop();
   return node;
}

Node *symbol(Token *token)
{
   Node *node;
   Token *st_dec = NULL;
   if (token->is_declare)
   {
      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n",
            to_string(token->type));
      setName(token, tmp->name);
      return new_node(token);
   }
   else if (token->type == ID && find(LPAR, 0))
   {
      node = new_node(token);
      if (strcmp(token->name, "main") == 0) return func_main(node);
      return func_call(node);
   }
   else if (token->type == ID && (st_dec = get_struct(token->name)))
   {
      token = copy_token(st_dec);
      token->type = STRUCT_CALL;
      token->Struct.ptr = st_dec;
      token->is_declare = true;

      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n",
            to_string(token->type));
      setName(token, tmp->name);
      return new_node(token);
   }
   else if (token->type == ID && find(LBRA, 0))
   {
      node = new_node(copy_token(token));
      node->token->type = ACCESS;
      Node *index = prime();
      check(!index || !index->token, "expected index after left bracket\n");
      check(!find(RBRA, 0), "expected right bracket\n");
      node->left = new_node(token);
      node->right = index;
      return node;
   }
   return new_node(token);
}

Node *struct_def(Node *node)
{
   // Struct def Layout:
   //    + children: attributes
   Token *st_name;
   if (check(!(st_name = find(ID, 0)), "expected identifier after struct definition"))
      return NULL;
   if (check(!find(DOTS, 0), "expected dots after struct definition"))
      return NULL;

   setName(node->token, NULL);
   node->token->Struct.name = strdup(st_name->name);
   while (within_space(node->token->space))
   {
      Token *attr = find(DATA_TYPES, ID, 0);
      Token *id = find(ID, 0);
      if (check(!attr, "expected data type followed by id"))
      {
         ptoken(tokens[exe_pos]);
         break;
      }
      if (check(!id, "expected id after data type")) break;
      id->type = attr->type;
      add_attribute(node->token, id);
   }
   new_struct(node->token);
   return node;
}

Node *if_node(Node *node)
{
   // if Layout:
   //    + left    : condition
   //    + children: code bloc
   //    + right   : elif/else Layout
   enter_scoop(node);

   node->left = expr();
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(CHILDREN, node->token->space));

   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());
   while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within_space(node->token->space - TAB))
   {
      Token *token = find(ELSE, ELIF, 0);
      Node *curr = add_child(node->right, new_node(token));
      token->space -= TAB;
      if (token->type == ELIF)
      {
         enter_scoop(curr);
         curr->left = expr();
         Node *bloc = add_child(node->right, new_node(copy_token(token)));
         setName(bloc->token, "bloc");
         bloc->token->space -= TAB;
         check(!find(DOTS, 0), "expected : after elif condition");
         while (within_space(token->space)) add_child(curr, expr());
         exit_scoop();
      }
      else if (token->type == ELSE)
      {
         enter_scoop(curr);
         check(!find(DOTS, 0), "expected : after else");
         while (within_space(token->space)) add_child(curr, expr());
         exit_scoop();
         break;
      }
   }

   Node *end = add_child(node->right, new_node(new_token(END_IF, 0)));
   end->token->space = node->right->token->space;
   setName(end->token, "end_if");

   exit_scoop();
   return node;
}

Node *while_node(Node *node)
{
   // while Layout:
   //    + left    : condition
   //    + children: code bloc
   enter_scoop(node);

   node->left = expr();
   node->left->token->space = node->token->space;
   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());

   exit_scoop();
   return node;
}

Node *cast_node()
{
   Node *node = prime();
   Token *token;
   if ((token = find(AS, 0)))
   {
      Node *left = node;
      node = new_node(token);
      Token *to = find(DATA_TYPES);
      if (check(to == NULL || !to->is_declare, "expected data type after to"))
         return NULL;
      to->is_declare = false;
      // TODO: check that is exists
      node->right = new_node(to);
      node->left = left;
      return node;
   }
   return node;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, LONG, SHORT, 0)))
      return symbol(token);
   else if ((token = find(TYPEOF, 0)))
   {
      node = new_node(token);
      Token *tk_type = find(DATA_TYPES, 0);
      check(!tk_type, "Expected data type after TYPEOF\n");
      node->token->type = CHARS;
      node->token->retType = CHARS;
      node->token->Chars.value = strdup(to_string(tk_type->type));
      return node;
   }
   else if ((token = find(NOT, 0)))
   {
      node = new_node(token);
      node->left = expr();
      return node;
   }
   else if ((token = find(STRUCT_DEF, 0)))
      return struct_def(new_node(token));
   else if ((token = find(REF, 0)))
   {
      node = prime(); // TODO: check it
      check(!node->token->is_declare, "must be variable declaration after ref");
      node->token->is_ref = true;
      return node;
   }
   else if ((token = find(FDEC, PROTO_FUNC, 0))) return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
      // in current scoop
      node = new_node(token);
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *curr = Gscoop[i];
         if (curr->token->type == FDEC)
         {
            // TODO: check return type here
            if (curr->token->retType == VOID)
            {
               node->left = copy_node(node);
               node->left->token->type = VOID;
            }
            else node->left = expr();
            break;
         }
      }
      return node;
   }
   else if ((token = find(IF, 0))) return if_node(new_node(token));
   else if ((token = find(WHILE, 0))) return while_node(new_node(token));
   else if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr();
      check(!find(RPAR, 0), "expected right par");
      return node;
   }
   else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}