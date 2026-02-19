#include "header.h"

bool found_error;

Token **tokens;
int tk_pos;
int tk_len;

Node *global;
int exe_pos;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoop_pos;

char **used_files;
int used_size;
int used_pos;

Context context;
Module module;
Builder builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
File asm_fd;

Value boundsCheckFunc;
Value nullCheckFunc;
Value vaStartFunc;
Value vaEndFunc;
Value refAssignFunc;

bool enable_bounds_check = false;
bool using_refs = false;

// PARSING
void tokenize(char *filename)
{
   char *input = open_file(filename);
   if (found_error) return;

   static bool calling_use = false;
   debug(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);

   struct { char *value; Type type; } specials[] = {
      {"...", VARIADIC}, {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN},
      {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL}, {"!", NOT},
      {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", GREAT_EQUAL},
      {"<", LESS}, {">", GREAT}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
      {"[", LBRA}, {"]", RBRA}, {",", COMA}, {"&&", AND}, {"||", OR},
      {0, (Type)0}
   };

   int space = 0;
   int line = 1;
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
         check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'");
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

            char *use_filename = NULL;
            if (use[0] == '@')
            {
               char *lib = getenv("URA_LIB");
               check(!lib, "URA_LIB environment variable not set");
               char *resolved = strjoin(lib, use + 1, NULL);
               free(use);
               use = resolved;
               use_filename = strdup(use);
            }
            else
               use_filename = strjoin(dirname(filename), "/", use);

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
            parse_token(filename, line, NULL, 0, 0, specials[j].type, space);
            i += len;
            if (includes(specials[j].type, DOTS, 0)) space += TAB;
            found = true;
            break;
         }
      }
      if (found) continue;
      check(1, "Syntax error <%c>", c);
   }
   if (!calling_use) new_token(END, -1);
   free(input);
}

Node *expr_node() { return assign_node(); }

AST_NODE(assign_node, logic_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, 0);
AST_NODE(logic_node, equality_node, AND, OR, 0);
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison_node, add_sub_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL, 0);
AST_NODE(add_sub_node, mul_div_node, ADD, SUB, 0);
AST_NODE(mul_div_node, dot_node, MUL, DIV, MOD, 0);

Node *dot_node()
{
   Node *left = brackets_node();
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

Node *brackets_node()
{
   Node *left = sign_node();
   Token *token;
   if ((token = find(LBRA, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = brackets_node();

      check(!find(RBRA, 0), "expected right bracket");
      return node;
   }
   return left;
}

Node *sign_node()
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

Node *func_call(Node *node)
{
   // Function call:
   //    + left:
   //       + children: Parameters
   node->token->type = FCALL;
   Token *arg = NULL;
   node->left = new_node(new_token(ARGS, node->token->space));

   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      Node *curr = expr_node();
      add_child(node->left, curr);
      find(COMA, 0);
   }
   check(!found_error && arg->type != RPAR, "expected ) after function call");
   return node;
}

Node *func_dec(Node *node)
{
   // Function Declaration:
   //    + left:
   //       + children: arguments
   //    + children     : code block
   Token *fname = find(ID, 0);
   if (check(!fname, "expected identifier after fn declaration"))
      return syntax_error_node();

   enter_scoop(node);
   check(!find(LPAR, 0), "expected ( after function declaration");
   // arguments
   node->left = new_node(new_token(ARGS, node->token->space));
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
         Token *name = find(ID, 0);
         if (check(!name, "expected identifier in function argument %s", fname->name))
            return syntax_error_node();

         bool is_ref = find(REF, 0) != NULL;
         Token* data_type = find(DATA_TYPES, ID, 0);
         if (check(!data_type, "expected data type in function argument")) break;
         if (data_type->type == ID)
         {
            Node *to_find = get_struct(data_type->name);
            if (to_find) data_type->type = STRUCT_CALL;
         }
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
         curr->token->is_dec = true;
         add_child(node->left, curr);
      }
      if (tokens[exe_pos]->type != RPAR)
         check(!find(COMA, 0), "xpected coma");
   }
   check(!found_error && last->type != RPAR, "expected ) after function declaration");

   Token *typeName = find(DATA_TYPES, 0);
   check(!typeName, "Expected data type after fun declaration");
   if (typeName->type == ID)
   {
      // typeName = get_struct(typeName->name);
      todo(1, "handle function return struct properly");
   }
   node->token->retType = typeName->type;
   setName(node->token, fname->name);

   if (!node->token->is_proto)
   {
      Token *next = find(DOTS, 0);
      check(!found_error && !next, "Expected : after function declaration");
      Node *child = NULL;

      if (next->type == DOTS)
         while (within(node->token->space)) child = add_child(node, expr_node());
      else
      {
         todo(1, "stop");
         Token *retToken = copy_token(next);
         retToken->type = RETURN;
         Node *retNode = new_node(retToken);
         retNode->left = expr_node();
         child = add_child(node, retNode);
      }
      if (next->type == DOTS)
      {
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

Node *func_main(Node *node)
{
   // Function main:
   //    + children: code bloc
   check(!find(RPAR, 0), "expected ) after main declaration");
   check(!find(DOTS, 0), "expected : after main() declaration");

   enter_scoop(node);
   node->token->type = FDEC;
   node->token->retType = INT;
   node->left = new_node(new_token(0, node->token->space));

   Node *last = NULL;
   while (within(node->token->space))
   {
      last = expr_node();
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
   Node *st_dec = NULL;
   // value example: "hello", 1, 'c'
   if (token->type != ID && !token->is_dec) return new_node(token);
   // int, char, chars, etc...
   else if (token->is_dec)
   {
      check(1, "unxpected token %s", to_string(token->type));
      return syntax_error_node();
   }
   // variable declaration
   else if (token->type == ID && includes(tokens[exe_pos]->type, DATA_TYPES, 0))
   {
      Token *tmp = find(DATA_TYPES, 0); // skip data type
      bool is_ref = find(REF, 0) != NULL;
      setName(tmp, token->name);
      tmp->is_dec = true;
      tmp->is_ref = is_ref;
      return new_node(tmp);
   }
   // variable declaration (struct variable)
   else if (
      token->type == ID &&
      includes(tokens[exe_pos]->type, ID, 0) &&
      (st_dec = get_struct(tokens[exe_pos]->name)))
   {
      find(ID, 0); // skip struct data type
      bool is_ref = find(REF, 0) != NULL;
      token->type = STRUCT_CALL;
      token->is_dec = true;
      token->is_ref = is_ref;
      token->Struct.ptr = st_dec->token;
      return new_node(token);
   }
   else if (token->type == ID && find(LPAR, 0))
   {
      node = new_node(token);
      if (strcmp(token->name, "main") == 0) return func_main(node);
      return func_call(node);
   }
   else if (token->type == ID && find(LBRA, 0))
   {
      node = new_node(copy_token(token));
      node->token->type = ACCESS;
      Node *index = prime_node();
      check(!index || !index->token, "expected index after left bracket");
      check(!find(RBRA, 0), "expected right bracket");
      node->left = new_node(token);
      node->right = index;
      return node;
   }
   else if (token->type == ID && find(DOT, 0))
   {
      node = new_node(copy_token(token));
      node->token->type = DOT;
      Node *index = prime_node();
      check(!index || !index->token, "expected index after dot");
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

   setName(node->token, st_name->name);
   while (within(node->token->space))
   {
      Token *id = find(ID, 0);
      if (check(!id, "expected id followed by data type"))
         return syntax_error_node();
      Token *attr = find(DATA_TYPES, ID, 0);
      if (check(!attr, "expected data type after id"))
         return syntax_error_node();
      id->type = attr->type;
      add_child(node, new_node(id));
   }
   new_struct(node);
   return node;
}

Node *if_node(Node *node)
{
   // if Layout:
   //    + left    : condition
   //    + children: code bloc
   //    + right   : elif/else Layout
   enter_scoop(node);

   node->left = expr_node();

   check(!find(DOTS, 0), "Expected : after if condition");

   // code bloc
   while (within(node->token->space)) add_child(node, expr_node());

   Node *curr = node;
   while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within(node->token->space - TAB))
   {
      Token *token = find(ELSE, ELIF, 0);

      curr->right = new_node(token);
      curr = curr->right;

      if (token->type == ELIF)
      {
         enter_scoop(curr);
         curr->left = expr_node();
         check(!find(DOTS, 0), "expected : after elif condition");
         while (within(token->space)) add_child(curr, expr_node());
         exit_scoop();
      }
      else if (token->type == ELSE)
      {
         enter_scoop(curr);
         check(!find(DOTS, 0), "expected : after else");
         while (within(token->space)) add_child(curr, expr_node());
         exit_scoop();
         break;
      }
   }
   exit_scoop();
   return node;
}

Node *while_node(Node *node)
{
   // while Layout:
   //    + left    : condition
   //    + children: code bloc
   enter_scoop(node);

   node->left = expr_node();
   // node->left->token->space = node->token->space;
   check(!find(DOTS, 0), "Expected : after if condition");

   // code bloc
   while (within(node->token->space)) add_child(node, expr_node());

   exit_scoop();
   return node;
}

Node *cast_node()
{
   Node *node = prime_node();
   Token *token;
   if ((token = find(AS, 0)))
   {
      Node *left = node;
      node = new_node(token);
      Token *to = find(DATA_TYPES, 0);
      if (check(to == NULL || !to->is_dec, "expected data type after to"))
         return syntax_error_node();
      to->is_dec = false;
      node->right = new_node(to);
      node->left = left;
      return node;
   }
   return node;
}

Node *prime_node()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(ID, DATA_TYPES, 0)))
      return symbol(token);
   else if ((token = find(TYPEOF, 0)))
   {
      node = new_node(token);
      Token *tk_type = find(DATA_TYPES, 0);
      check(!tk_type, "Expected data type after TYPEOF");
      node->token->type = CHARS;
      node->token->retType = CHARS;
      node->token->Chars.value = strdup(to_string(tk_type->type));
      return node;
   }
   else if ((token = find(NOT, 0)))
   {
      node = new_node(token);
      node->left = expr_node();
      return node;
   }
   else if ((token = find(STRUCT_DEF, 0)))
      return struct_def(new_node(token));
   else if ((token = find(PROTO, 0)))
   {
      if (includes(tokens[exe_pos]->type, FDEC, STRUCT_DEF, 0))
         tokens[exe_pos]->is_proto = true;
      else check(1, "expected fn or struct after proto");
      return expr_node();
   }
   else if ((token = find(FDEC, 0))) return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
      // in current scoop, must be done inside gen_ir
      node = new_node(token);
      for (int i = scoop_pos; i >= 0; i--)
      {
         Node *curr = Gscoop[i];
         if (curr->token->type == FDEC)
         {
            if (curr->token->retType == VOID)
            {
               node->left = copy_node(node);
               node->left->token->type = VOID;
            }
            else node->left = expr_node();
            break;
         }
      }
      return node;
   }
   else if ((token = find(IF, 0))) return if_node(new_node(token));
   else if ((token = find(WHILE, 0))) return while_node(new_node(token));
   else if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr_node();
      check(!find(RPAR, 0), "expected right )");
      return node;
   }
   else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
   check(1, "Unexpected token has type %s", to_string(tokens[exe_pos]->type));
   return syntax_error_node();
}


void parse()
{
   if (found_error) return;
#if AST
   global = new_node(new_token(ID, -TAB));
   setName(global->token, "ura-scoop");
   enter_scoop(global);
   while (!find(END, 0) && !found_error)
      add_child(global, expr_node());
#endif
}

void build_ir()
{
#if IR
   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_ir(global->children[i]);
#endif
}

static void unuse(Node *node)
{
   if (!node || !node->token) return;

   if (node->left  && node->left->token)  { if (node->left->token->used  > 0) node->left->token->used--;  }
   if (node->right && node->right->token) { if (node->right->token->used > 0) node->right->token->used--; }
   for (int i = 0; i < node->cpos; i++)
      if (node->children[i] && node->children[i]->token)
         if (node->children[i]->token->used > 0) node->children[i]->token->used--;
}

Node *optimize_ir(Node *node)
{
   if (!node || found_error) return NULL;

   node->left  = optimize_ir(node->left);
   node->right = optimize_ir(node->right);

   if (node->cpos)
   {
      int j = 0;
      Node **new_children = allocate(node->clen, sizeof(Node*));
      for (int i = 0; i < node->cpos; i++)
      {
         Node *child = optimize_ir(node->children[i]);
         if (child) new_children[j++] = child;
      }
      node->cpos     = j;
      node->children = new_children;
   }

   if (!node->token) return node;

   switch (node->token->type)
   {
   case FDEC: case PROTO:
   {
      if (node->token->used == 0 && strcmp(node->token->name, "main") != 0)
      {
         debug(RED "remove %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         return NULL;
      }
      break;
   }

   // Variable declaration never read after being set
   case INT: case BOOL: case CHAR: case FLOAT:
   case LONG: case CHARS: case PTR: case STRUCT_CALL:
   {
      if (node->token->is_dec && node->token->used == 0)
      {
         debug(RED "remove unused var %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         return NULL;
      }
      break;
   }

   // Pure math/logic with unused result
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case AND: case OR: case NOT:
   case EQUAL: case NOT_EQUAL:
   case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
   {
      if (node->token->used == 0)
      {
         debug(RED "remove unused op %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         return NULL;
      }
      break;
   }

   // Assignment to a variable nobody reads
   case ASSIGN:
   {
      if (node->left && node->left->token && node->left->token->used == 0 && is_data_type(node->left->token))
      {
         debug(RED "remove unused assign %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         return NULL;
      }
      break;
   }

   default: break;
   }

   return node;
}


void code_gen(char *filename)
{
#if ASM
   char *moduleName = resolve_path(filename);
   init(moduleName);

   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_asm(global->children[i]);

   int len = strlen(moduleName);
   strcpy(moduleName + len - 3, "ll");
   moduleName[len - 1] = '\0';
   finalize(moduleName);
   free(moduleName);
#endif
}

void compile(char *filename)
{
   tokenize(filename);
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT TOKENS:\n" RESET);
   for (int i = 0; tokens[i]; i++) debug("%k\n", tokens[i]);

   parse();
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT AST:\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

   build_ir();
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT IR (BEFORE OPTIMIZE):\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

   #if OPTIMIZE
   bool removed_something = false;
   do {
      removed_something = false;
      global = optimize_ir(global);
   } while (removed_something);
   #endif

   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT IR (AFTER OPTIMIZE):\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

   code_gen(filename);
}

int main(int argc, char **argv)
{
   check(argc < 2, "require argument, usage ura <file.ura>");
   compile(argv[1]);
   free_memory();
#if !IR
   debug(RED"NO IR GENERATION\n"RESET);
#endif
#if !ASM
   debug(RED"NO ASSEMBLY GENERATION\n"RESET);
#endif
   return (found_error || !ASM || !IR);
}