#include "header.h"

bool    found_error;

Token **tokens;
int     tk_pos;
int     tk_len;

Node   *global;
int     exe_pos;

Node  **Gscoop;
Node   *scoop;
int     scoop_len;
int     scoop_pos;

char  **used_files;
int     used_len;
int     used_pos;

Context context;
Module  module;
Builder builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
File    asm_fd;


bool enable_bounds_check = false;
// bool             using_refs;
// bool             is_method_call;
char            *passes;
bool             enable_asan;

LLVMDIBuilderRef di_builder;
LLVMMetadataRef  di_compile_unit;
LLVMMetadataRef  di_file;
LLVMMetadataRef  di_current_scope;

// PARSING
void tokenize(char *filename)
{
   char *input = open_file(filename);
   if (found_error) return;

   static bool calling_use = false;
   debug(GREEN BOLD "TOKENIZE: [%s]\n" RESET, filename);

   struct { char *value; Type type; } specials[] =
   {
      {"...", VARIADIC}, {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN},
      {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN},
      {"!=", NOT_EQUAL}, {"!", NOT},{"==", EQUAL},
      {"<<", LSHIFT}, {">>", RSHIFT}, {"<=", LESS_EQUAL},
      {">=", GREAT_EQUAL}, {"<", LESS}, {">", GREAT}, {"=", ASSIGN},
      {"+", ADD}, {"-", SUB},{"*", MUL}, {"/", DIV}, {"%", MOD},
      {"(", LPAR}, {")", RPAR}, {"[", LBRA}, {"]", RBRA}, {",", COMA},
      {"&&", AND}, {"||", OR}, {"&", BAND}, {"|", BOR}, {"^", BXOR},
      {"~", BNOT},
      {0, (Type)0}
   };

   int  space    = 0;
   int  line     = 1;
   bool new_line = true;

   if (!add_file(filename)) return;
   for (int i = 0; input[i] && !found_error; )
   {
      int  s = i;
      char c = input[i];

      if (isspace(c))
      {
         if (c == '\n')
         {
            line++;
            new_line = true;
            space    = 0;
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
         check(input[i] != '\"', "expected '\"'");
         parse_token(filename, line, input, s + 1, i, CHARS, space);
         i++;
         continue;
      }
      if (c == '\'')
      {
         i++;
         if (input[i] == '\\') i++;
         if (input[i] && input[i] != '\'') i++;
         check(input[i] != '\'', "expected '\''");
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
               char *lib      = getenv("URA_LIB");
               check(!lib, "URA_LIB environment variable not set");
               char *resolved = strjoin(lib, use + 1, NULL);
               free(use);
               use          = resolved;
               use_filename = strdup(use);
            }
            else
               use_filename = strjoin(dirname(filename), "/", use);

            free(use);
            use         = strjoin(use_filename, ".ura", NULL);
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
            i    += len;
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

AST_NODE(assign_node, logic_and_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN)
AST_NODE(logic_and_node, logic_or_node, AND)
AST_NODE(logic_or_node, bitor_node, OR)
AST_NODE(bitor_node, bitxor_node, BOR)
AST_NODE(bitxor_node, bitnot_node, BXOR)

Node *bitnot_node() // ~
{
   Token *token;
   if ((token = find(BNOT, 0)))
   {
      Node *node = new_node(token);
      node->left = bitnot_node();
      return node;
   }
   return bitand_node();
}

AST_NODE(bitand_node, equality_node, BAND)
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL)
AST_NODE(comparison_node, shift_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL)
AST_NODE(shift_node, add_sub_node, LSHIFT, RSHIFT)
AST_NODE(add_sub_node, mul_div_node, ADD, SUB)
AST_NODE(mul_div_node, as_node, MUL, DIV, MOD)

Node *as_node() // as
{
   Node  *left = unary_node();
   Token *token;
   while ((token = find(AS, 0)))
   {
      Node *node = new_node(token);
      node->left  = left;
      node->right = unary_node();
      Token *to = node->right->token;
      if(check(to == NULL || !to->is_dec, "expected data type after to"))
         return syntax_error();
      to->is_dec = false;
      left       = node;
   }
   return left;
}

Node *unary_node() // - + (unary)
{
   Token *token;
   if ((token = find(ADD, SUB, 0)))
   {
      Node *node = new_node(token);
      node->left  = unary_node();
      node->right = new_node(new_token(INT, token->space));
      node->right->token->filename  = token->filename;
      node->right->token->line      = token->line;
      node->right->token->Int.value = token->type == SUB ? -1 : 1;
      node->token->type             = MUL;
      return node;
   }
   return access_node();
}

Node *access_node() // . []
{
   Node  *left = prime_node();
   Token *token;
   while ((token = find(DOT, LBRA, 0)))
   {
      Node *node = new_node(token);
      if(token->type == DOT)
      {
         node->left  = left;
         node->right = prime_node();

         check(!node->right, "expected (attribute/method call) after .");
         if (node->right->token->type == FCALL)
         {
            Node *func = node->right;
            func->token->is_method_call = true;
            add_child(func->left, left);
            node->right = NULL;
            node->left  = NULL;
            node        = func;
         }
      }
      else // LBRA
      {
         node->token->type = ACCESS;
         node->left        = left;
         node->right       = expr_node();
         check(!node->right, "expected something between []");
         expect_token(RBRA, "expected right bracket");
      }
      left = node;
   }
   return left;
}

Node *prime_node() // primary
{
   Node  *node = NULL;
   Token *token;

   if ((token = find(ID, DATA_TYPES, 0)))
   {
      Node *node;
      Node *st_dec = NULL;
      // value example: "hello", 1, 'c'
      if (token->type != ID && !token->is_dec && !token->name)
         return new_node(token);
      // int, char, chars, etc...
      if (token->is_dec) return new_node(token);
      // variable declaration
      if (token->type == ID && is_data_type(tokens[exe_pos]))
      {
         Token *tmp    = find(DATA_TYPES, 0); // skip data type
         bool   is_ref = find(REF, 0) != NULL;
         if (tmp->type == ARRAY_TYPE)
         {
            expect_token(LBRA, "expected [ after array");
            int    depth     = 1;
            while (find(LBRA, 0)) depth++;
            Token *elem_type = find(DATA_TYPES, ID, 0);
            check(!elem_type, "expected element type in array type");
            for (int i = 0; i < depth; i++)
               expect_token(RBRA, "expected ] in array type");
            tmp->Array.elem_type = elem_type->type;
            tmp->Array.depth     = depth;
            tmp->retType         = ARRAY_TYPE;
         }
         setName(tmp, token->name);
         tmp->is_dec = true;
         tmp->is_ref = is_ref;
         if (is_ref)
         {
            expect_token(ASSIGN, "'%s': ref must be initialized at "
                         "declaration, e.g: x int ref = a", token->name);
            exe_pos--;
         }
         return new_node(tmp);
      }
      // variable declaration (struct variable)
      bool cond = token->type == ID && tokens[exe_pos]->type == ID;
      if (cond && (st_dec = get_struct(tokens[exe_pos]->name)))
      {
         find(ID, 0); // skip struct data type
         bool is_ref = find(REF, 0) != NULL;
         if (is_ref)
         {
            expect_token(ASSIGN, "'%s': ref must be initialized at "
                         "declaration, e.g: x int ref = a", token->name);
            exe_pos--;
         }
         token->type       = STRUCT_CALL;
         token->is_dec     = true;
         token->is_ref     = is_ref;
         token->Struct.ptr = st_dec;
         return new_node(token);
      }
      if (token->type == ID && find(LPAR, 0))
      {
         node = new_node(token);
         if (strcmp(token->name, "main") == 0)
         {
            // Function main:
            //    + children: code bloc
            expect_token(RPAR, "expected ) after main declaration");
            expect_token(DOTS, "expected : after main() declaration");

            enter_scoop(node);
            node->token->type    = FDEC;
            node->token->retType = INT;
            node->left           = new_node(new_token(ARGS, node->token->space));

            Node *last = NULL;
            while (within(node->token->space))
            {
               last = expr_node();
               add_child(node, last);
            }
            if (!last || last->token->type != RETURN)
            {
               last       = new_node(new_token(RETURN, node->token->space + TAB));
               last->left = new_node(new_token(INT, node->token->space + TAB));
               add_child(node, last);
            }
            exit_scoop();
         }
         else
         {
            // Function call:
            //    + left:
            //       + children: Parameters
            node->token->type = FCALL;
            Token *arg = NULL;
            node->left        = new_node(new_token(ARGS, node->token->space));

            while (!found_error && !(arg = find(RPAR, END, 0)))
            {
               Node *curr = expr_node();
               add_child(node->left, curr);
               find(COMA, 0);
            }
            check(!found_error && arg->type != RPAR, "expected ) after function call");
         }
         return node;
      }
      return new_node(token);
   }
   if ((token = find(STRUCT_DEF, 0)))
   {
      // Struct def Layout:
      //    + children: attributes
      node = new_node(token);
      Token *st_name;
      if (check(!(st_name = find(ID, 0)), "expected identifier after struct definition"))
         return NULL;
      expect_token(DOTS, "expected dots after struct definition\n");

      setName(node->token, st_name->name);
      node->token->type       = STRUCT_DEF; // keep it, it's changed at the bottom
      node->token->Struct.ptr = node;
      new_struct(node);
      enter_scoop(node);

      // Collect methods to hoist AFTER the struct is registered
      Node **methods      = NULL;
      int    method_count = 0;
      int    method_cap   = 0;

      while (within(node->token->space))
      {
         // TODO: free memory in all place we exit on error
         Node *child = prime_node();
         if(!child) return syntax_error();
         if (child->token->type == FDEC)
         {
            methods = (Node**)resize_array((void**)methods, &method_cap, method_count, sizeof(Node*));
            methods[method_count++] = child;
         }
         else
         {
            if (check(!child->token->is_dec, "invalid attribute"))
            {
               exit_scoop();
               free(methods);
               return syntax_error();
            }
            child->token->is_dec = false;
            add_child(node, child);
         }
      }

      exit_scoop();

      // Hoist methods AFTER struct is registered so gen_asm sees STRUCT_DEF first
      for (int i = 0; i < method_count; i++)
      {
         Token *mt = methods[i]->token;
         if(strcmp(mt->name, "init") == 0)
         {
            if (check(mt->retType != VOID,"struct '%s': init() must return void", node->token->name))
            {
               free(methods);
               return syntax_error();
            }
            if (check(methods[i]->left->cpos != 1, "struct '%s': init() must take no parameters", node->token->name))
            {
               free(methods);
               return syntax_error();
            }
            node->token->is_init = true;
         }
         char *qualified = strjoin(node->token->name, ".", mt->name);
         setName(mt, qualified);
         free(qualified);
         add_method(node, methods[i]);
      }
      free(methods);

      return node;
   }
   if ((token = find(IF, 0)))
   {
      // if Layout:
      //    + left    : condition
      //    + children: code bloc
      //    + right   : elif/else Layout
      node = new_node(token);
      enter_scoop(node);

      node->left = expr_node();

      expect_token(DOTS, "expected : after if condition");

      // code bloc
      while (within(node->token->space)) add_child(node, expr_node());

      Node *curr = node;
      while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within(node->token->space - TAB))
      {
         Token *token = find(ELSE, ELIF, 0);

         curr->right = new_node(token);
         curr        = curr->right;

         if (token->type == ELIF)
         {
            enter_scoop(curr);
            curr->left = expr_node();
            expect_token(DOTS, "expected : after elif condition");
            while (within(token->space)) add_child(curr, expr_node());
            exit_scoop();
         }
         else if (token->type == ELSE)
         {
            enter_scoop(curr);
            expect_token(DOTS, "expected : after else");
            while (within(token->space)) add_child(curr, expr_node());
            exit_scoop();
            break;
         }
      }
      exit_scoop();
      return node;
   }
   if ((token = find(WHILE, 0)))
   {
      node = new_node(token);
      // while Layout:
      //    + left    : condition
      //    + children: code bloc
      enter_scoop(node);

      node->left = expr_node();
      // node->left->token->space = node->token->space;
      expect_token(DOTS, "expected : after if condition");

      // code bloc
      while (within(node->token->space)) add_child(node, expr_node());

      exit_scoop();
      return node;
   }
   if ((token = find(BREAK, CONTINUE, 0)))
   {
      return new_node(token);
   }
   if ((token = find(STACK, HEAP, 0)))
   {
      node = new_node(token);
      expect_token(LBRA, "expected [ after stack");

      int depth = 1;
      while (find(LBRA, 0)) depth++;

      Token *elem_type = find(DATA_TYPES, ID, 0);
      check(!elem_type, "expected element type in stack");

      for (int i = 0; i < depth; i++)
         expect_token(RBRA, "expected ] in stack type");

      expect_token(LPAR, "expected ( after stack[type]");

      node->token->retType         = ARRAY;
      node->token->Array.elem_type = elem_type->type;
      node->token->Array.depth     = depth;

      // stack[[char]](10)
      node->left = expr_node();
      expect_token(RPAR, "expected ) after stack size");
      return node;
   }
   if ((token = find(TYPEOF, SIZEOF, 0)))
   {
      char *msg  = token->type == TYPEOF ? "typeof" : "sizeof";
      Type  type = token->type == TYPEOF ? CHARS : INT;
      node                 = new_node(token);
      expect_token(LPAR, "%s: expected (", msg);
      node->left           = prime_node();
      expect_token(RPAR, "%s: expected )", msg);
      node->token->retType = type;
      return node;
   }
   if ((token = find(PROTO, 0)))
   {
      if (includes(tokens[exe_pos]->type, FDEC, STRUCT_DEF, 0))
         tokens[exe_pos]->is_proto = true;
      else check(1, "expected <fn> or <struct> after proto");
      return expr_node();
   }
   if ((token = find(FDEC, 0)))
   {
      // Function Declaration:
      //    + left:
      //       + children: arguments
      //    + children     : code block
      node = new_node(token);
      Token *fname = find(ID, 0);
      if (check(!fname, "expected identifier after fn declaration"))
         return syntax_error();

      // Check if we're inside a struct scope
      Node *struct_owner = NULL;
      Node *parent       = Gscoop[scoop_pos];
      if (parent && parent->token->type == STRUCT_DEF)
         struct_owner = parent;

      enter_scoop(node);
      expect_token(LPAR, "expected ( after function declaration");

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
               return syntax_error();

            Token *data_type = find(DATA_TYPES, ID, 0);
            bool   is_ref    = find(REF, 0) != NULL;
            if (check(!data_type, "expected data type in function argument")) break;
            if (data_type->type == ID)
            {
               Node *to_find = get_struct(data_type->name);
               if (to_find) data_type->type = STRUCT_CALL;
            }
            Node *curr;
            if (data_type->type == STRUCT_CALL)
            {
               Node *st = get_struct(data_type->name);
               if (check(!st, "Unknown struct type '%s'", data_type->name)) break;
               data_type->Struct.ptr = st;
               curr = new_node(data_type);
               data_type->is_ref     = is_ref;
               setName(data_type, name->name);
            }
            else
            {
               curr         = new_node(name);
               name->is_ref = is_ref;
               name->type   = data_type->type;
            }
            curr->token->is_dec   = true;
            curr->token->is_param = true;
            add_child(node->left, curr);
         }
         if (tokens[exe_pos]->type != RPAR) expect_token(COMA, "expected coma");
      }
      check(!found_error && last->type != RPAR, "expected ) after function declaration");

      // If inside a struct, prepend implicit 'self' parameter
      if (struct_owner)
      {
         Token *self_tok = new_token(STRUCT_CALL, node->token->space);
         setName(self_tok, "self");
         self_tok->Struct.ptr = struct_owner;
         self_tok->is_dec     = true;
         self_tok->is_ref     = true;
         add_child(node->left, new_node(self_tok));
      }

      if(
         check(
            !is_data_type(tokens[exe_pos]) &&
            tokens[exe_pos]->type != ID,
            "expected data type after fun declaration"))
         return syntax_error();
      Token *ret_tok = tokens[exe_pos++];
      if (ret_tok->type == ID)
      {
         Node *st = get_struct(ret_tok->name);
         if (check(!st, "unknown return type '%s'", ret_tok->name))
            return syntax_error();
         node->token->retType    = STRUCT_CALL;
         node->token->Struct.ptr = st;
      }
      else
         node->token->retType = ret_tok->type;
      node->token->is_ref = find(REF, 0) != NULL;

      setName(node->token, fname->name);

      if (!node->token->is_proto)
      {
         Token *next = find(DOTS, 0);
         if(check(!found_error && !next, "expected : after function declaration"))
            return syntax_error();
         Node *child = NULL;

         if (next->type == DOTS)
            while (within(node->token->space)) child = add_child(node, expr_node());
         else
         {
            Token *retToken = copy_token(next);
            retToken->type = RETURN;
            Node  *retNode  = new_node(retToken);
            retNode->left  = expr_node();
            child          = add_child(node, retNode);
         }
         if (next->type == DOTS)
         {
            if (node->token->retType != VOID)
               check(!child || child->token->type != RETURN,
                     "expected return statement %s", node->token->name);
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
   if ((token = find(RETURN, 0)))
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
   if ((token = find(NOT, 0)))
   {
      node       = new_node(token);
      node->left = expr_node();
      return node;
   }
   if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr_node();
      expect_token(RPAR, "expected right )");
      return node;
   }

   check(1, "Unexpected token has type %s line %d", to_string(tokens[exe_pos]->type), tokens[exe_pos]->line);
   return syntax_error();
}

void unuse(Node *node)
{
   if (!node || !node->token) return;

   if (node->left  && node->left->token)
   {
      if (node->left->token->used  > 0) node->left->token->used--;
   }
   if (node->right && node->right->token)
   {
      if (node->right->token->used > 0) node->right->token->used--;
   }
   for (int i = 0; i < node->cpos; i++)
      if (node->children[i] && node->children[i]->token)
         if (node->children[i]->token->used > 0) node->children[i]->token->used--;
}

Node *optimize_ir(Node *node, bool *changed)
{
   if (!node || found_error) return NULL;

   node->left  = optimize_ir(node->left, changed);
   node->right = optimize_ir(node->right, changed);

   if (node->cpos)
   {
      int    j            = 0;
      Node **new_children = allocate(node->clen, sizeof(Node*));
      for (int i = 0; i < node->cpos; i++)
      {
         Node *child = optimize_ir(node->children[i], changed);
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
         *changed = true;
         return NULL;
      }
      break;
   }
      // TODO: add disbale default if next is assignement
#if 0
   // Variable declaration never read after being set
   case INT: case BOOL: case CHAR: case FLOAT:
   case LONG: case CHARS: case PTR: case STRUCT_CALL:
   {
      if (node->token->is_dec && node->token->used == 0)
      {
         debug(RED "remove unused var %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         *changed = true;
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
         *changed = true;
         return NULL;
      }
      break;
   }
   // Assignment to a variable nobody reads
   case ASSIGN:
   {
      bool cond = node->left == NULL || (node->left
                                         && node->left->token
                                         && node->left->token->used == 0
                                         && is_data_type(node->left->token));
      if (cond)
      {
         debug(RED "remove unused assign %k\n" RESET, node->token);
         unuse(node);
         free_node(node);
         *changed = true;
         return NULL;
      }
      break;
   }
#endif
   default: break;
   }

   return node;
}

// returns the .ll path (caller must free)
char *compile(char *filename)
{
   debug("%s===========================================\n", RED);
   debug("START COMPILATION\n");
   debug("===========================================\n%s",RESET);
   // reset global state for each file
   found_error = false;
   tk_pos      = 0; exe_pos = 0;
   scoop_pos   = 0; scoop = NULL;
   global      = NULL;

   tokenize(filename);
   if (found_error) return NULL;

#if AST
   if (found_error) return NULL;
   global = new_node(new_token(ID, -TAB));
   setName(global->token, "ura-scoop");
   enter_scoop(global);
   while (!find(END, 0) && !found_error)
      add_child(global, expr_node());
   debug("%s===========================================\n", GREEN);
   debug("AFTER PARSING\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");
   if (found_error) return NULL;
#endif

#if IR
   if (found_error) return NULL;
   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_ir(global->children[i]);
   if (found_error) return NULL;
   debug("%s===========================================\n", GREEN);
   debug("AFTER IR\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

#if OPTIMIZE
   bool changed = true;
   int  op      = 0;
   while(changed)
   {
      changed = false;
      optimize_ir(global, &changed);
      if(changed) op++;
   }
   debug("%s===========================================\n", GREEN);
   debug("AFTER OPTMIZING\n");
   debug("===========================================\n%s",RESET);
   for (int i = 0; op && !found_error && i < global->cpos; i++)
      pnode(global->children[i], "");

#endif
#endif


#if ASM
   if (found_error) return NULL;

   // build/  directory next to the source file
   char *dir  = dirname(strdup(filename));
   char *base = basename(strdup(filename));
   // strip .ura extension
   char *dot  = strrchr(base, '.');
   if (dot) *dot = '\0';

   // mkdir build/
   char *build_dir = strjoin(dir, "/build", NULL);
   mkdir(build_dir, 0755);

   char *ll_path    = strjoin(build_dir, "/", strjoin(base, ".ll", NULL));

   char *moduleName = resolve_path(filename);
   init(moduleName);
   free(moduleName);

   for (int i = 0; !found_error && i < global->cpos; i++)
      gen_asm(global->children[i]);

   finalize(ll_path);
   free(build_dir);
   return found_error ? NULL : ll_path;
#endif
   return NULL;
}

int main(int argc, char **argv)
{
   if(check(argc < 2, "usage: ura <file.ura> [file2.ura ...]"
            "[-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
      return 1;

   debug("recieved %d files compiling\n", argc - 1);
   for(int i = 1; i < argc; i++)
   {
      debug("%s\n", argv[i]);
   };
   char  *output       = "exe.out";
   char **src_files    = NULL;
   int    src_count    = 0;
   bool   testing_mode = false;
   bool   no_exec      = false;

   for (int i = 1; i < argc; i++)
   {
      if      (strcmp(argv[i], "-O0")     == 0) passes = PASSES_O0;
      else if (strcmp(argv[i], "-O1")     == 0) passes = PASSES_O1;
      else if (strcmp(argv[i], "-O2")     == 0) passes = PASSES_O2;
      else if (strcmp(argv[i], "-O3")     == 0) passes = PASSES_O3;
      else if (strcmp(argv[i], "-Os")     == 0) passes = PASSES_Os;
      else if (strcmp(argv[i], "-Oz")     == 0) passes = PASSES_Oz;
      else if (strcmp(argv[i], "-san")    == 0) enable_asan = true;
      else if (strcmp(argv[i], "-testing") == 0) testing_mode = true;
      else if (strcmp(argv[i], "-no-exec") == 0) no_exec = true;
      else if (strcmp(argv[i], "-o")      == 0)
      {
         check(i + 1 >= argc, "-o requires an argument");
         output = argv[++i];
      }
      else if (argv[i][0] != '-')
      {
         src_files = realloc(src_files, (src_count + 1) * sizeof(char *));
         src_files[src_count++] = argv[i];
      }
      else
         fprintf(stderr, "unknown flag: %s\n", argv[i]);
   }

   bool link_ok = true;
   char final_cmd[8192];
   int  pos     = 0;

   pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
   if (enable_asan)
      pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos,
                      " -fsanitize=address,undefined -fno-omit-frame-pointer -g");

   for (int i = 0; i < src_count && link_ok; i++)
   {
      char *ll = compile(src_files[i]);
      #if ASM
      if (!ll)
      {
         link_ok = false;
         break;
      }
      if (testing_mode)
      {
         free(ll);
         free_memory();
         continue;
      }
      if (enable_asan)
         pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
      else
      {
         char  s[4096];
         snprintf(s, sizeof(s), "%s", ll);
         char *dot = strrchr(s, '.');
         if (dot) strcpy(dot, ".s");

         char llc_cmd[4096];
         snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
         if (system(llc_cmd) != 0)
         {
            fprintf(stderr, RED "llc failed for %s\n" RESET, ll);
            link_ok = false;
            free(ll);
            break;
         }
         pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", s);
      }
      free(ll);
      #endif

      free_memory();
   }

   if (!testing_mode && link_ok && !no_exec)
   {
      pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
      if (system(final_cmd) != 0)
         fprintf(stderr, RED "linking failed\n" RESET);
      else
      {
         char full[4096];
         realpath(output, full);

         if (enable_asan)
         {
            char *asan_file = getenv("ASAN_FILE");
            if (asan_file)
               setenv("ASAN_OPTIONS", "detect_leaks=1", 1),
               setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
            else
               setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
         }

         fprintf(stderr, GREEN "running %s...\n" RESET, full);

         char run[8192];
         snprintf(run, sizeof(run), "\"%s\"", full);

         int status    = system(run);
         int exit_code = WEXITSTATUS(status);

         fprintf(stderr, exit_code == 0
                 ? GREEN "exit code: %d\n" RESET
                 : RED   "exit code: %d\n" RESET, exit_code);
      }
   }

   free(src_files);
   return found_error || !link_ok;
}

// GENERATE
// ASSEMBLY GENERATION
Value allocate_stack(Value size, TypeRef elementType, char *name)
{
   Value indices[] =
   {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, 0, 0)
   };

   if (LLVMIsConstant(size))
   {
      unsigned long long constSize    = LLVMConstIntGetZExtValue(size);
      TypeRef            arrayType    = LLVMArrayType(elementType, constSize);
      Value              array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
   }

   Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef elementType, char *name)
{
   Value calloc_func = _get_named_function("calloc");
   if (!calloc_func)
   {
      TypeRef params[]  = {i64, i64};
      TypeRef func_type = _function_type(p8, params, 2, 0);
      calloc_func = _add_function("calloc", func_type);
   }
   TargetData td        = _get_module_data_layout(module);
   size_t     elem_size = _abi_size_of_type(td, elementType);

   Value      count_i64;
   unsigned   width = LLVMGetIntTypeWidth(LLVMTypeOf(count));
   if (width < 64)
      count_i64 = LLVMBuildZExt(builder, count, i64, "count");
   else if (width > 64)
      count_i64 = LLVMBuildTrunc(builder, count, i64, "count");
   else
      count_i64 = count;

   Value   size_i64      = _const_int(i64, elem_size, 0);
   Value   args[]        = {count_i64, size_i64};
   TypeRef calloc_type = _global_get_value_type(calloc_func);
   return _build_call2(calloc_type, calloc_func, args, 2, name);
}

Value get_store_ptr(Token *token)
{
   // Regular variable - return its alloca'd address
   if (!token->is_ref) return token->llvm.elem;
   // Reference - need to load the pointer it points to
   TypeRef type     = get_llvm_type(token);
   TypeRef ptr_type = LLVMPointerType(type, 0);
   return _build_load2(ptr_type, token->llvm.elem, "store_ptr");
}

Value struct_field_ptr(Token *struct_tok, int field_index, char *name)
{
   TypeRef struct_type = get_llvm_type(struct_tok);   // the struct's LLVM type
   Value   indices[]   =
   {
      LLVMConstInt(i32, 0, 0),            // deref the alloca pointer
      LLVMConstInt(i32, field_index, 0),  // pick the field
   };
   return _build_gep2(struct_type, struct_tok->llvm.elem, indices, 2, name);
}

void hoist_allocas(Node *node)
{
   if (!node) return;
   Token *tok = node->token;

   // don't recurse into nested functions
   if (tok->type == FDEC) return;

   if (includes(tok->type, INT, LONG, SHORT, CHARS, CHAR, BOOL, ARRAY_TYPE, 0) && tok->is_dec)
   {
      if (!tok->llvm.elem)
         _alloca(tok);
   }
   else if (tok->type == STRUCT_CALL && tok->is_dec && !tok->is_ref)
   {
      if (!tok->llvm.elem)
      {
         TypeRef struct_type = get_llvm_type(tok);
         tok->llvm.elem = _build_alloca(struct_type, tok->name);
      }
   }

   if (node->left)  hoist_allocas(node->left);
   if (node->right) hoist_allocas(node->right);
   for (int i = 0; i < node->cpos; i++)
      hoist_allocas(node->children[i]);
}

void gen_asm(Node *node)
{
   set_debug_location(node->token);
   Node *left  = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set")) return;
   switch (node->token->type)
   {
#if 0
   case STRUCT_CALL:
   {
      if (node->token->is_dec)
      {
         if (node->token->is_ref)
         {
            // TypeRef type = get_llvm_type(node->token);
            // Value   null = LLVMConstNull(LLVMPointerType(type, 0));
            // _build_store(null, node->token->llvm.elem);
         }
         else
         {
            TypeRef struct_type = get_llvm_type(node->token);
            _build_store(LLVMConstNull(struct_type), node->token->llvm.elem);

            Node *src = node->token->Struct.ptr;
            if (src->token->is_init)
            {
               char *qname   = strjoin(src->token->name, ".init", NULL);
               Value init_fn = _get_named_function(qname);
               free(qname);
               if (init_fn)
               {
                  Value args[] = { node->token->llvm.elem };
                  _build_call2(_global_get_value_type(init_fn), init_fn, args, 1, "");
               }
            }
         }
         node->token->is_dec = false;
         return;
      }
      break;
   }
#endif
   case INT: case LONG: case SHORT: case CHARS:
   case CHAR: case BOOL: case ARRAY_TYPE:
   {
      if (node->token->is_dec)
      {
         _alloca(node->token);
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
         return;
      _const_value(node->token);
      break;
   }
   case ASSIGN:
   {
      if (left->token->is_ref && !left->token->asm_bound && !left->token->is_param)
      {
         if (!left->token->llvm.elem)
            _alloca(left->token);
         gen_asm(right);
         _build_store(right->token->llvm.elem, left->token->llvm.elem);
         left->token->asm_bound = true;
         left->token->is_dec    = false;
         break;
      }

      gen_asm(left);
      gen_asm(right);

      TypeRef type = get_llvm_type(right->token);

      Value   val;
      if (right->token->is_ref)
      {
         Value ptr = _build_load2(_pointer_type(type, 0), right->token->llvm.elem, "ref_ptr");
         val = _build_load2(type, ptr, "ref_val");
      }
      else
      {
         load_if_necessary(right);
         val = right->token->llvm.elem;
      }

      if (left->token->is_ref)
      {
         Value ptr = _build_load2(_pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         _build_store(val, ptr);
      }
      else
         _build_store(val, left->token->llvm.elem);

      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_asm(left);
      gen_asm(right);

      TypeRef type = get_llvm_type(left->token);
      Value   current_val;

      if (left->token->is_ref)
      {
         // TODD: check it later
         Value ptr = _build_load2(_pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         current_val = _build_load2(type, ptr, "current");
      }
      else
         current_val = _build_load2(type, left->token->llvm.elem, "current");

      Value right_val;
      if (right->token->is_ref)
      {
         Value ptr = _build_load2(_pointer_type(type, 0), right->token->llvm.elem, "ref_ptr");
         right_val = _build_load2(type, ptr, "rval");
      }
      else
      {
         load_if_necessary(right);
         right_val = right->token->llvm.elem;
      }

      // Perform operation
      Value result;
      switch (node->token->type)
      {
      case ADD_ASSIGN: result = _build_add(current_val, right_val, to_string(ADD)); break;
      case SUB_ASSIGN: result = _build_sub(current_val, right_val, to_string(SUB)); break;
      case MUL_ASSIGN: result = _build_mul(current_val, right_val, to_string(MUL)); break;
      case DIV_ASSIGN: result = _build_sdiv(current_val, right_val, to_string(DIV)); break;
      case MOD_ASSIGN: result = _build_srem(current_val, right_val, to_string(MOD)); break;
      default: break;
      }

      if (left->token->is_ref)
      {
         Value ptr = _build_load2(_pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         _build_store(result, ptr);
      }
      else
         _build_store(result, left->token->llvm.elem);
      break;
   }
   case NOT: case BNOT:
   {
      gen_asm(left);
      load_if_necessary(left);
      if (left->token->is_ref)
      {
         TypeRef type = get_llvm_type(left->token);
         Value   ptr  = _build_load2(_pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         left->token->llvm.elem      = _build_load2(type, ptr, "ref_val");
         left->token->llvm.is_loaded = true;
      }
      node->token->llvm.elem = _build_not(left->token, to_string(node->token->type));
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case AND: case OR:
   case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
   {
      gen_asm(left);
      gen_asm(right);

      load_if_necessary(left);
      load_if_necessary(right);

      Value lref = left->token->llvm.elem;
      Value rref = right->token->llvm.elem;

      if (left->token->is_ref && !left->token->llvm.is_loaded)
      {
         TypeRef type = get_llvm_type(left->token);
         Value   ptr  = _build_load2(_pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         lref = _build_load2(type, ptr, "ref_val");
      }
      if (right->token->is_ref && !right->token->llvm.is_loaded)
      {
         TypeRef type = get_llvm_type(right->token);
         Value   ptr  = _build_load2(_pointer_type(type, 0), right->token->llvm.elem, "ref_ptr");
         rref = _build_load2(type, ptr, "ref_val");
      }

      Value elem = NULL;
      char *op   = to_string(node->token->type);
      switch (node->token->type)
      {
      case LESS:        elem = LLVMBuildICmp(builder, LLVMIntSLT, lref, rref, op); break;
      case GREAT:       elem = LLVMBuildICmp(builder, LLVMIntSGT, lref, rref, op); break;
      case EQUAL:       elem = LLVMBuildICmp(builder, LLVMIntEQ,  lref, rref, op); break;
      case LESS_EQUAL:  elem = LLVMBuildICmp(builder, LLVMIntSLE, lref, rref, op); break;
      case GREAT_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntSGE, lref, rref, op); break;
      case NOT_EQUAL:   elem = LLVMBuildICmp(builder, LLVMIntNE,  lref, rref, op); break;
      case ADD:         elem = LLVMBuildAdd(builder,  lref, rref, op); break;
      case SUB:         elem = LLVMBuildSub(builder,  lref, rref, op); break;
      case MUL:         elem = LLVMBuildMul(builder,  lref, rref, op); break;
      case DIV:         elem = LLVMBuildSDiv(builder, lref, rref, op); break;
      case MOD:         elem = LLVMBuildSRem(builder, lref, rref, op); break;
      case AND:         elem = LLVMBuildAnd(builder,  lref, rref, op); break;
      case OR:          elem = LLVMBuildOr(builder,   lref, rref, op); break;
      case BAND:        elem = LLVMBuildAnd(builder,  lref, rref, op); break;
      case BOR:         elem = LLVMBuildOr(builder,   lref, rref, op); break;
      case BXOR:        elem = LLVMBuildXor(builder,  lref, rref, op); break;
      case LSHIFT:      elem = LLVMBuildShl(builder,  lref, rref, op); break;
      case RSHIFT:      elem = LLVMBuildAShr(builder, lref, rref, op); break;
      default:          todo(1, "handle this %s", op); break;
      }

      node->token->llvm.elem = elem;
      node->token->retType   = left->token->retType ? left->token->retType : left->token->type;
      break;
   }
   case STACK:
   {
      gen_asm(node->left);
      load_if_necessary(node->left);
      Value elem_count = node->left->token->llvm.elem;

      // base type
      Token   tmp    = {.type = node->token->Array.elem_type};
      TypeRef elem_t = get_llvm_type(&tmp);

      // depth > 1 means each element is a pointer
      // [char]=i8, [[char]]=ptr, [[[char]]]=ptr (ptr to ptr semantically)
      if (node->token->Array.depth > 1)
         elem_t = p8; // pointer-sized elements

      TargetData td        = _get_module_data_layout(module);
      size_t     elem_size = _abi_size_of_type(td, elem_t);
      Value      total     = _build_mul(elem_count, _const_int(i32, (unsigned)elem_size, 0), "bytes");

      node->token->llvm.elem       = allocate_stack(total, elem_t, "stack");
      node->token->llvm.is_set     = true;
      node->token->llvm.array_size = elem_count;
      break;
   }
   case HEAP:
   {
      gen_asm(node->left);
      load_if_necessary(node->left);
      Value   elem_count = node->left->token->llvm.elem;

      Token   tmp        = {.type = node->token->Array.elem_type};
      TypeRef elem_t     = get_llvm_type(&tmp);
      if (node->token->Array.depth > 1)
         elem_t = p8;

      node->token->llvm.elem       = allocate_heap(elem_count, elem_t, "heap");
      node->token->llvm.is_set     = true;
      node->token->llvm.array_size = elem_count;
      break;
   }
   case FCALL:
   {
      LLVM   srcFunc     = node->token->Fcall.ptr->token->llvm;
      bool   is_variadic = node->token->Fcall.ptr->token->is_variadic;
      int    count       = node->left->cpos;
      Node **argNodes    = node->left->children;
      Node  *dec_args    = node->token->Fcall.ptr->left;
      Value *args        = NULL;

      if (count)
      {
         args = allocate(count + 3, sizeof(Value));

         int fixed_params = is_variadic ? node->token->Fcall.ptr->left->cpos : count;

         // handle fixed params (both variadic and non-variadic)
         for (int i = 0; i < fixed_params; i++)
         {
            gen_asm(argNodes[i]);
            bool param_is_ref = (i < dec_args->cpos) && dec_args->children[i]->token->is_ref;
            bool arg_is_ref   = argNodes[i]->token->is_ref;

            if (param_is_ref && arg_is_ref)
            {
               // ref → ref: load the pointer out of the ref's slot and pass it
               TypeRef type = get_llvm_type(argNodes[i]->token);
               args[i] = _build_load2(_pointer_type(type, 0), argNodes[i]->token->llvm.elem, "ref_arg");
            }
            else if (param_is_ref && !arg_is_ref)
            {
               // non-ref → ref param: pass the variable's alloca directly
               args[i] = argNodes[i]->token->llvm.elem;
            }
            else if (!param_is_ref && arg_is_ref)
            {
               // ref → non-ref param: double dereference to get the value
               TypeRef type = get_llvm_type(argNodes[i]->token);
               Value   ptr  = _build_load2(_pointer_type(type, 0), argNodes[i]->token->llvm.elem, "ref_ptr");
               args[i] = _build_load2(type, ptr, "ref_val");
            }
            else
            {
               // non-ref → non-ref: normal load
               load_if_necessary(argNodes[i]);
               args[i] = argNodes[i]->token->llvm.elem;
            }
         }

         if (is_variadic)
         {
            int variadic_count = count - fixed_params;
            args[fixed_params] = _const_int(i32, variadic_count, 0);
            for (int i = fixed_params; i < count; i++)
            {
               gen_asm(argNodes[i]);
               load_if_necessary(argNodes[i]);
               args[i + 1] = argNodes[i]->token->llvm.elem;
            }
            count++;
         }
      }

      char   *name     = node->token->Fcall.ptr->token->retType != VOID ? node->token->name : "";
      TypeRef funcType = srcFunc.funcType;
      Value   elem     = srcFunc.elem;
      if (check(!srcFunc.funcType, "FCALL: funcType is NULL for '%s'", node->token->name))
         break;
      if (check(!srcFunc.elem, "FCALL: elem is NULL for '%s'", node->token->name))
         break;
      node->token->llvm.elem = LLVMBuildCall2(builder, funcType, elem, args, count, name);
      free(args);
      break;
   }
   case FDEC:
   {
      // debug("================================\n");
      // pnode(node, "");
      // debug("\n================================\n");
      enter_scoop(node);
      TypeRef retType;
      if (node->token->retType == STRUCT_CALL && node->token->is_ref)
         retType = _pointer_type(get_llvm_type(node->token), 0);
      else
         retType = get_llvm_type(node->token);

      TypeRef *paramTypes  = NULL;
      int      param_count = node->left->cpos;
      int      fixed_count = param_count;
      int      _count      = param_count;

      if (node->token->is_variadic)
         _count = fixed_count + 1;

      if (param_count)
      {
         paramTypes = allocate(_count + 2, sizeof(TypeRef));

         for (int i = 0; i < fixed_count; i++)
         {
            Token *param = node->left->children[i]->token;
            if (param->is_ref) paramTypes[i] = _pointer_type(get_llvm_type(param), 0);
            else paramTypes[i] = get_llvm_type(param);
         }

         if (node->token->is_variadic)
            paramTypes[fixed_count] = i32;
      }

      TypeRef funcType     = _function_type(retType, paramTypes, _count, node->token->is_variadic);
      char   *fname        = node->token->name;
      Value   existingFunc = _get_named_function(fname);
      if (existingFunc) node->token->llvm.elem = existingFunc;
      else node->token->llvm.elem = _add_function(fname, funcType);
      node->token->llvm.funcType = funcType;


      if (!node->token->is_proto)
      {
         if (enable_asan)
            LLVMAddAttributeAtIndex(
               node->token->llvm.elem,
               LLVMAttributeFunctionIndex,
               LLVMCreateEnumAttribute(
                  context,
                  LLVMGetEnumAttributeKindForName(
                     "sanitize_address", 16),
                  0
                  ));

         // Debug info for this function
         LLVMMetadataRef di_func_type = LLVMDIBuilderCreateSubroutineType(
            di_builder, di_file, NULL, 0, LLVMDIFlagZero);

         LLVMMetadataRef di_func = LLVMDIBuilderCreateFunction(
            di_builder,
            di_compile_unit,
            fname, strlen(fname),
            fname, strlen(fname),
            di_file,
            node->token->line,
            di_func_type,
            0,
            1,
            node->token->line,
            LLVMDIFlagZero,
            0
            );
         LLVMSetSubprogram(node->token->llvm.elem, di_func);
         di_current_scope = di_func;
         _entry(node->token);

         // set a valid location for param allocas
         LLVMMetadataRef entry_loc = LLVMDIBuilderCreateDebugLocation(
            context, node->token->line, 0, di_func, NULL);
         LLVMSetCurrentDebugLocation2(builder, entry_loc);

         int param_idx = 0;
         for (int i = 0; i < fixed_count; i++)
         {
            Token *param_token = node->left->children[i]->token;
            Value  param       = LLVMGetParam(node->token->llvm.elem, param_idx++);
            LLVMSetValueName(param, param_token->name);
            _alloca(param_token);
            param_token->is_dec = false;
            LLVMBuildStore(builder, param, param_token->llvm.elem);
         }

         #if USING_HOIST
         for (int i = 0; i < node->cpos; i++)
            hoist_allocas(node->children[i]);
         #endif

         for (int i = 0; i < node->cpos; i++)
         {
            gen_asm(node->children[i]);
            if (_get_basic_block_terminator(_get_insert_block()))
               break;
         }
      }
      exit_scoop();
      di_current_scope = di_compile_unit;
      break;
   }
   case WHILE:
   {
      enter_scoop(node);
      Block start = _append_block("while.start");
      Block then  = _append_block("while.then");
      Block end   = _append_block("while.end");

      // Store blocks for break/continue
      node->token->llvm.start = start;
      node->token->llvm.then  = then;
      node->token->llvm.end   = end;

      _branch(start);

      _position_at(start);
      gen_asm(node->left); // condition
      load_if_necessary(node->left);
      _condition(node->left->token->llvm.elem, then, end);

      _position_at(then);
      for (int i = 0; i < node->cpos; i++)
      {
         if (_get_basic_block_terminator(_get_insert_block()))
            break;
         gen_asm(node->children[i]);
      }
      _branch(start);

      _position_at(end);
      exit_scoop();
      break;
   }
   case IF:
   {
      enter_scoop(node);

      Block if_start = _append_block("if.start");
      Block end      = _append_block("if.end");

      // Store blocks for potential break/continue
      node->token->llvm.start = if_start;
      node->token->llvm.end   = end;

      _branch(if_start);

      Node *curr = node;

      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
      {
         if (includes(curr->token->type, IF, ELIF, 0))
         {
            Block start;
            Block then;
            Block next;

            if (curr->token->type == IF)
            {
               start = if_start;
               then  = _append_block("if.then");
            }
            else
            {
               start = curr->token->llvm.bloc;
               then  = _append_block("elif.then");
            }

            // Store then block
            curr->token->llvm.then = then;

            if (curr->right)
            {
               if (curr->right->token->type == ELSE) next = _append_block("if.else");
               else next = _append_block("elif.start");
            }
            else next = end;

            _position_at(start);
            gen_asm(curr->left); // condition
            load_if_necessary(curr->left);
            _condition(curr->left->token->llvm.elem, then, next);

            _position_at(then);
            for (int i = 0; i < curr->cpos; i++)
            {
               gen_asm(curr->children[i]);
               if (_get_basic_block_terminator(_get_insert_block()))
                  break;
            }
            _branch(end);

            if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
               curr->right->token->llvm.bloc = next;
         }
         else if (curr->token->type == ELSE)
         {
            _position_at(curr->token->llvm.bloc);
            for (int i = 0; i < curr->cpos; i++)
            {
               gen_asm(curr->children[i]);
               if (_get_basic_block_terminator(_get_insert_block()))
                  break;
            }
            _branch(end);
         }
         curr = curr->right;
      }
      _position_at(end);
      exit_scoop();
      break;
   }
   case BREAK:
   {
      for (int i = scoop_pos; i > 0; i--)
      {
         if (Gscoop[i]->token->type == WHILE)
         {
            _branch(Gscoop[i]->token->llvm.end);
            return;
         }
      }
      check(1, "break outside loop");
      break;
   }
   case CONTINUE:
   {
      for (int i = scoop_pos; i > 0; i--)
      {
         if (Gscoop[i]->token->type == WHILE)
         {
            _branch(Gscoop[i]->token->llvm.start);
            return;
         }
      }
      check(1, "continue outside loop");
      break;
   }
   case RETURN:
   {
      if (node->left->token->type != VOID)
      {
         gen_asm(node->left);
         Token *ret_tok = node->left->token;

         // Find the enclosing FDEC to check its return type
         Node *fdec = NULL;
         for (int i = scoop_pos; i >= 0; i--)
            if (Gscoop[i] && Gscoop[i]->token->type == FDEC)
            { fdec = Gscoop[i]; break; }

         bool fdec_returns_ref    = fdec && fdec->token->is_ref;
         bool fdec_returns_struct = fdec && fdec->token->retType == STRUCT_CALL;

         if (fdec_returns_struct && !fdec_returns_ref)
         {
            // return by value: load the full struct
            TypeRef st_type = ret_tok->Struct.ptr->token->llvm.stType;
            Value   val     = _build_load2(st_type, ret_tok->llvm.elem, "ret_struct");
            LLVMBuildRet(builder, val);
            break;
         }
         else if (fdec_returns_struct && fdec_returns_ref)
         {
            // return by ref: return the pointer directly (self->llvm.elem holds User*)
            // self is stored as alloca of ptr, so load the ptr
            TypeRef st_type  = get_llvm_type(ret_tok);
            TypeRef ptr_type = _pointer_type(st_type, 0);
            Value   ptr      = _build_load2(ptr_type, ret_tok->llvm.elem, "ret_ptr");
            LLVMBuildRet(builder, ptr);
            break;
         }
         load_if_necessary(node->left);
      }
      _build_return(node->left->token);
      break;
   }
   case AS:
   {
      gen_asm(left);
      load_if_necessary(left);

      Value    source     = left->token->llvm.elem;
      TypeRef  sourceType = LLVMTypeOf(source);
      TypeRef  targetType = get_llvm_type(node->right->token);

      TypeKind sourceKind = LLVMGetTypeKind(sourceType);
      TypeKind targetKind = LLVMGetTypeKind(targetType);

      Value    result;

      // Pointer to integer
      if (sourceKind == PointerTypeKind && targetKind == IntegerTypeKind)
         result = LLVMBuildPtrToInt(builder, source, targetType, "as");
      // Integer to pointer
      else if (sourceKind == IntegerTypeKind && targetKind == PointerTypeKind)
         result = LLVMBuildIntToPtr(builder, source, targetType, "as");
      // Integer to integer
      else if (sourceKind == IntegerTypeKind && targetKind == IntegerTypeKind)
      {
         unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
         unsigned targetBits = LLVMGetIntTypeWidth(targetType);

         if (sourceBits > targetBits)
            result = LLVMBuildTrunc(builder, source, targetType, "as");
         else if (sourceBits < targetBits)
            result = LLVMBuildSExt(builder, source, targetType, "as");
         else
            result = source;
      }
      // Pointer to pointer (bitcast)
      else if (sourceKind == PointerTypeKind && targetKind == PointerTypeKind)
         result = LLVMBuildBitCast(builder, source, targetType, "as");

      else
      {
         check(1, "unsupported cast from %d to %d", sourceKind, targetKind);
         result = source;
      }

      node->token->llvm.elem      = result;
      node->token->llvm.is_loaded = true;
      break;
   }
   case STRUCT_DEF:
   {
      int      pos   = node->cpos;
      TypeRef *types = allocate((pos + 1), sizeof(TypeRef));
      int      j     = 0;
      for (int i = 0; i < pos; i++)
         types[j++] = get_llvm_type(node->children[i]->token);
      node->token->llvm.stType = _struct_type_in_context(types, j, 0);
      free(types);

      for (int i = 0; i < node->mpos; i++)
         gen_asm(node->methods[i]);
      break;
   }
   case DOT:
   {
      gen_asm(node->left);

      Token *struct_tok  = node->left->token;
      int    field_index = node->right->token->Struct.index;

      // If the struct is a ref, we need to load the pointer first
      // before indexing into it, otherwise we GEP into the alloca
      // that holds the pointer, not into the struct itself.
      if (struct_tok->is_ref)
      {
         TypeRef struct_type = get_llvm_type(struct_tok);
         Value   struct_ptr  = _build_load2(
            _pointer_type(struct_type, 0),
            struct_tok->llvm.elem,
            struct_tok->name
            );
         Value indices[] =
         {
            _const_int(i32, 0, 0),
            _const_int(i32, field_index, 0),
         };
         node->token->llvm.elem = _build_gep2(
            struct_type, struct_ptr, indices, 2, node->right->token->name
            );
      }
      else
      {
         Value field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);
         node->token->llvm.elem = field_ptr;
      }
      break;
   }
   case ACCESS:
   {
      // bool enable_bounds_check = false;
      gen_asm(node->left);

      if (node->left->token->type == STRUCT_CALL)
      {
         Token *struct_tok  = node->left->token;
         int    field_index = node->right->token->Struct.index;

         Value  field_ptr   = struct_field_ptr(struct_tok, field_index, node->right->token->name);

         node->token->llvm.elem = field_ptr;
         /* retType already set by gen_ir; mark as NOT loaded so
            load_if_necessary() will emit the load when reading. */
         break;
      }

      gen_asm(node->right);
      Value leftValue;
      if (node->left->token->is_ref)
      {
         todo(1, "stop");
         // If left is a ref, dereference it to get the actual array
         leftValue = _load2(node->left->token);
      }
      else
      {
         // todo(1, "stop");
         load_if_necessary(node->left);
         leftValue = node->left->token->llvm.elem;
         TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
         if (kind == PointerTypeKind && node->left->token->name &&
             !node->left->token->llvm.is_loaded &&
             node->left->token->type != STACK)
         {
            leftValue = _build_load2(p8, leftValue, "ptr_load");
         }
      }

      // Load the index (handles refs)
      load_if_necessary(node->right);
      Value   rightRef = node->right->token->llvm.elem;

      TypeRef element_type;
      Type    left_elem_type = node->left->token->retType
                      ? node->left->token->retType
                      : node->left->token->type;
      if (left_elem_type == CHARS)
      {
         element_type         = i8;
         node->token->retType = CHAR;
      }
      else
      {
         element_type         = get_llvm_type(left->token);
         node->token->retType = left->token->type;
      }

      // Add bounds checking if enabled
      if (enable_bounds_check)
      {
         // We need to track array sizes - for now we'll use a conservative approach
         // For CHARS (strings), we can use strlen at runtime
         // For STACK arrays, we need to store the size

         Value size_val       = NULL;

         Type  left_elem_type = node->left->token->retType
                      ? node->left->token->retType
                      : node->left->token->type;
         if (left_elem_type == CHARS)
         {
            // For strings, we need to get the length
            // First check if it's a string literal or a variable
            if (node->left->token->name && !left->token->llvm.array_size)
            {
               // It's a variable - we need strlen
               Value strlen_func = _get_named_function("strlen");
               if (!strlen_func)
               {
                  TypeRef strlen_type = _function_type(i64, (TypeRef[]) {p8}, 1, false);
                  strlen_func = _add_function("strlen", strlen_type);
               }
               Value strlen_result = _build_call2(_global_get_value_type(strlen_func), strlen_func,
                                                  (Value[]) {leftValue}, 1, "strlen");
               size_val = _build_trunc(strlen_result, i32, "size");
            }
            else if (node->left->token->llvm.array_size)
            {
               size_val = node->left->token->llvm.array_size;
            }
            else
            {
               // String literal - we know the size at compile time
               // This is handled in _chars function
               size_val = _const_int(i32, strlen(node->left->token->Chars.value), 0);
            }
         }
         else if (node->left->token->type == STACK)
         {
            // For STACK arrays, we should have stored the size
            // For now, use a placeholder - you'll need to enhance this
            size_val = _const_int(i32, 1000000, 0); // Conservative large value
         }

         if (size_val)
         {
            // Get bounds check function
            Value bounds_check = _get_named_function("__bounds_check");

            // Create filename string
            static Value filename_str = NULL;

            if (!filename_str)
            {
               char filename[256] = {0};

               if (getcwd(filename, sizeof(filename)) != NULL)
               {
                  size_t len = strlen(filename);
                  snprintf(filename + len, sizeof(filename) - len, "/%s", "input_file");
                  filename_str = _const_chars(filename, "filename");
               }
            }


            // Call bounds check: __bounds_check(index, size, line, filename)
            Value line_val = _const_int(i32, node->token->line, 0);
            _build_call2(_global_get_value_type(bounds_check), bounds_check,
                         (Value[]) {rightRef, size_val, line_val, filename_str}, 4, "");
         }
      }

      Value indices[] = { rightRef };
      Value gep       = _build_gep2(element_type, leftValue, indices, 1, "ACCESS");
      node->token->llvm.elem = gep;

      break;
   }
   case TYPEOF: case SIZEOF:
   {
      Token *type_tok = node->left->token;
      if (node->token->type == TYPEOF)
      {
         node->token->llvm.elem      = _const_chars(node->token->Chars.value, "typeof");
         node->token->llvm.is_loaded = true;
      }
      else
      {
         TypeRef    type = get_llvm_type(type_tok);
         TargetData td   = _get_module_data_layout(module);
         size_t     size = _abi_size_of_type(td, type);
         node->token->Int.value      = (long long)size;
         node->token->llvm.elem      = _const_int(i32, size, 0);
         node->token->llvm.is_loaded = true;
      }
      break;
   }
   default:
      pnode(node, "");
      todo(1, "handle this case %s", to_string(node->token->type));
      // seg();
      break;
   }
}

void gen_ir(Node * node)
{
   if (found_error) return;
   Node *left  = node->left;
   Node *right = node->right;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find)
      {
         node->token = find;
      }
      break;
   }
   case STRUCT_CALL:
   {
      Node *src = get_struct(node->token->Struct.ptr->token->name);
      node->token->Struct.ptr = src;
      if (node->token->is_dec)
      {
         new_variable(node->token);
         if (src->token->is_init)
         {
            // TODO: to be checked later
            char *qname   = strjoin(src->token->name, ".init", NULL);
            Node *init_fn = get_function(qname);
            free(qname);
            if (init_fn) init_fn->token->used++;
         }
      }
      break;
   }
   case INT: case BOOL: case CHAR: case ARRAY_TYPE:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      if (node->token->is_dec) new_variable(node->token);
      else node->token->used++;
      break;
   }
   case ASSIGN:
   {
      // TODO: check compatibility
      // TODO: add all ir_bound checks in &ll places

      gen_ir(left);
      gen_ir(right);
      if(!compatible(left->token, right->token))
      {
         check(1, "Invalid assignement %k= %k", left->token, right->token);
         break;
      }
      bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound) &&
                      (!right->token->name || right->token->type == FCALL);
      if (check(error_op,"'%s': ref must be bound to a named variable", left->token->name))
         break;

      node->token->retType = left->token->retType;
      if(left->token->is_ref)
         left->token->ir_bound = true;
      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_ir(left);
      bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound);
      if (check(error_op,"'%s': ref must be bound to a named variable", left->token->name))
         break;

      gen_ir(right);
      // check(!validate_ref_assignment(left->token, right->token), "");
      node->token->retType = left->token->retType;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      break;
   }
   case NOT: case BNOT:
   {
      // TODO: left must be boolean
      gen_ir(left);
      node->token->used++;
      node->left->token->used++;
      node->token->retType = BOOL;

      if(node->token->type == NOT)
         node->token->retType = BOOL;
      else
         node->token->retType = left->token->retType ?
                                left->token->retType : left->token->type;
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case AND: case OR:
   case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
   {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);

      check(!left || !left->token, "error in assignment, left is NULL");
      check(!right || !right->token, "error in assignment, right is NULL");

      switch (node->token->type)
      {
      case ADD: case SUB: case MUL: case DIV: case MOD:
         node->token->retType = left->token->retType; break;
      case AND: case OR: case NOT_EQUAL: case EQUAL: case LESS:
      case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
         node->token->retType = BOOL; break;
      default: break;
      }
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      set_ret_type(node);
      break;
   }
   case WHILE:
   {
      enter_scoop(node);
      gen_ir(node->left); // condition
      node->left->token->used++;
      // code bloc
      for (int i = 0; i < node->cpos; i++) gen_ir(node->children[i]);
      exit_scoop();
      node->token->used++;
      break;
   }
   case IF:
   {
      enter_scoop(node);
      Node *curr = node;
      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
      {
         if (includes(curr->token->type, IF, ELIF, 0))
         {
            check(curr->left == NULL, "error");
            gen_ir(curr->left); // condition
            curr->left->token->used++;
         }
         // code bloc
         for (int i = 0; i < curr->cpos; i++)
            gen_ir(curr->children[i]);
         curr = curr->right;
      }

      exit_scoop();
      node->token->used++;
      break;
   }
   case FDEC:
   {
      new_function(node);
      enter_scoop(node);

      // parameters
      Node **params = (node->left ? node->left->children : NULL);
      for (int i = 0; params && i < node->left->cpos && !found_error; i++)
      {
         gen_ir(params[i]);
         // if(node->token->is_proto)
         params[i]->token->used++;
      }

      // code bloc
      for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
      {
         Node *child = node->children[i];
         gen_ir(child);
      }
      exit_scoop();
      break;
   }
   case FCALL:
   {
      if (node->token->is_method_call && node->left)
      {
         // resolve object type
         Node  *obj_node = node->left->children[node->left->cpos - 1];
         gen_ir(obj_node);
         Token *obj      = obj_node->token;

         // build qualified name: "User.greed"
         char *struct_name = obj->Struct.ptr->token->name;
         char *qname       = strjoin(struct_name, ".", node->token->name);
         // pnode(obj_node, "");
         // exit(1);
         setName(node->token, qname);
         free(qname);
         node->token->is_method_call = false;

         Node *func = get_function(node->token->name);
         if (!func) break;
         func->token->used++;
         node->token->Fcall.ptr = func;
         func = copy_node(func);

         // first arg already gen_ir'd (obj), do rest
         obj_node->token->used++;
         for (int i = 0; !found_error && i < node->left->cpos - 1; i++)
         {
            Node *carg = node->left->children[i];
            gen_ir(carg);
            if (check(carg->token->type == ID, "Indeclared variable %s", carg->token->name))
               break;

            carg->token->used++;
         }
         free_node(func);
         set_ret_type(node);
         break;
      }

      Node *func = get_function(node->token->name);
      if (!func) return;
      func->token->used++;
      node->token->Fcall.ptr = func;
      func = copy_node(func);

      Node *call_args = node->left;
      Node *dec_args  = func->left;
      // TODO: check number of arguments
      for (int i = 0; !found_error && call_args && i < call_args->cpos; i++)
      {
         Node *carg = call_args->children[i];
         gen_ir(carg);
         carg->token->used++;
         Token *src = carg->token;
         if (check(src->type == ID, "Undeclared variable %s", carg->token->name)) break;

         if (i < dec_args->cpos)
         {
            bool param_is_ref = dec_args->children[i]->token->is_ref;
            if (param_is_ref)
               if (check(!src->name || src->type == FCALL,
                         "'%s': ref parameter requires a named variable",
                         dec_args->children[i]->token->name))
                  break;
         }
      }
      free_node(func);
      set_ret_type(node);
      break;
   }
   case AS:
   {
      gen_ir(node->left);
      node->token->retType = node->right->token->type;
      break;
   }
   case DOT:
   {
      gen_ir(node->left);
      if (found_error) break;
      node->left->token->used++;
      Type  retType = 0;

      Node *src     = NULL;
      if (node->left->token->type == STRUCT_CALL)
         src = get_struct(node->left->token->Struct.ptr->token->name);
      else if (node->left->token->retType == STRUCT_CALL && node->left->token->Struct.ptr)
         src = get_struct(node->left->token->Struct.ptr->token->name);
      else
      {
         check(1, "handle this case %s", to_string(node->left->token->type));
         break;
      }

      bool found = false;
      for (int i = 0; src && i < src->cpos; i++)
      {
         Node *src_child = src->children[i];
         if (strcmp(src_child->token->name, node->right->token->name) == 0)
         {
            found   = true;
            retType = src_child->token->type;
            node->right->token->Struct.index = i;
            node->right->token->type         = src_child->token->type;
            node->right->token->retType      = src_child->token->retType;
            if (src_child->token->type == STRUCT_CALL)
               node->right->token->Struct.ptr = src_child->token->Struct.ptr;
            else if (src_child->token->type == ARRAY_TYPE)
            {
               node->right->token->Array.elem_type = src_child->token->Array.elem_type;
               node->right->token->Array.depth     = src_child->token->Array.depth;
            }
            break;
         }
      }
      if(check(!found, ""))
      {
         debug("%s%k doesn't have attribute %k%s\n", RED,
               node->left->token, node->right->token, RESET);
         return;
      }

      node->token->retType = retType;
      if (retType == STRUCT_CALL)
         node->token->Struct.ptr = node->right->token->Struct.ptr;
      else if (retType == ARRAY_TYPE)
      {
         node->token->Array.elem_type = node->right->token->Array.elem_type;
         node->token->Array.depth     = node->right->token->Array.depth;
      }
      break;
   }
   case ACCESS:
   {
      gen_ir(node->left);
      gen_ir(node->right);
      if (found_error) break;
      // node->left->token->used++;
      node->right->token->used++;

      Type retType = 0;
      Type type    = node->left->token->retType ? node->left->token->retType : node->left->token->type;
      switch (type)
      {
      case CHARS: retType = CHAR; break;
      // TODO: handle multiple [[[]]]
      case STACK: case HEAP: case ARRAY_TYPE:
      {
         retType = node->left->token->Array.elem_type;
         break;
      }
      default:
         check(1, "handle this case %s", to_string(node->left->token->type));
         break;
      }
      node->token->retType = retType;
      break;
   }
   case RETURN:
   {
      gen_ir(node->left);
      node->token->retType = node->left->token->type;
      if (node->left->token->type == STRUCT_CALL)
         node->token->Struct.ptr = node->left->token->Struct.ptr;
      node->left->token->used++;
      break;
   }
   case TYPEOF: case SIZEOF:
   {
      gen_ir(node->left);
      Token *type_tok = node->left->token;
      Type   type     = type_tok->type ? type_tok->type : type_tok->retType;

      if (node->token->type == TYPEOF)
      {
         node->token->retType     = CHARS;
         node->token->Chars.value = strdup(to_string(type));
      }
      else
      {
         node->token->retType   = INT;
         node->token->Int.value = 0; // placeholder
      }
      node->token->used++;
      break;
   }
   case STACK: case HEAP:
   {
      // TODO; left must be an integer
      gen_ir(node->left);
      node->token->used++;
      node->left->token->used++;
      break;
   }
   case STRUCT_DEF:
   {
      for (int i = 0; node && i < node->mpos; i++)
         gen_ir(node->methods[i]);
      break;
   }
   case BREAK: case CONTINUE: break;
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
}

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

// PARSING
void add_token(Token *token)
{
   tokens = (Token**)resize_array(
      (void**)tokens,
      &tk_len, tk_pos, sizeof(Token *));
   tokens[tk_pos++] = token;
}

Token *new_token(Type type, int space)
{
   Token* token = allocate(1, sizeof(Token));
   token->type  = type;
   token->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(token);
   return token;
}

Token *parse_token(char *filename, int line, char *input, int s, int e,  Type type, int space)
{
   Token *new = new_token(type, space);
   new->line     = line;
   new->filename = filename;
   new->pos      = tk_pos;

   switch (type)
   {
   case INT:
   {
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   case ID:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      int i = 0;
      struct { char *name; bool value; } bools[] =
      {
         {"True", true}, {"False", false}, {0, 0},
      };
      for (i = 0; bools[i].name; i++)
      {
         if (strcmp(new->name, bools[i].name) == 0)
         {
            setName(new, NULL);
            new->type       = BOOL;
            new->Bool.value = bools[i].value;
            break;
         }
      }
      if (new->name == NULL) break;
      // TODO: PTR's approach need to be fixed later
      struct { char *name; Type type; } dataTypes [] =
      {
         {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
         {"char", CHAR}, {"float", FLOAT}, {"void", VOID},
         {"long", LONG}, {"pointer", CHARS}, {"short", SHORT},
         {0, 0},
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type   = dataTypes[i].type;
            new->is_dec = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] =
      {
         {"if", IF},  {"elif", ELIF}, {"else", ELSE},
         {"while", WHILE},  {"fn", FDEC}, {"return", RETURN},
         {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF},
         {"struct", STRUCT_DEF}, {"proto", PROTO}, {"as", AS},
         {0, 0},
      };
      for (i = 0; keywords[i].name; i++)
      {
         if (strcmp(keywords[i].name, new->name) == 0)
         {
            new->type = keywords[i].type;
            // if (new->type == REF) using_refs = true;
            break;
         }
      }
      if (keywords[i].name) break;

      struct { char *name; Type type; } keywords2[] =
      {
         {"and", AND}, {"or", OR}, {"is", EQUAL},
         {"not", NOT}, {"typeof", TYPEOF}, {"sizeof", SIZEOF},
         {"stack", STACK}, {"heap", HEAP}, {"array", ARRAY_TYPE},
         {0, 0},
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
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      if (e <= s)
      {
         new->Chars.value = allocate(1, sizeof(char));
         break;
      }
      int   len   = e - s;
      new->Chars.value = allocate(len + 1, sizeof(char));
      char *value = new->Chars.value;
      int   j     = 0;

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
            case '0':
            {
               // check for octal: \0NN
               if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
               {
                  int octal = (input[s + 1] - '0') * 64 +
                              (input[s + 2] - '0') * 8 +
                              (input[s + 3] - '0');
                  if (octal <= 255) { value[j++] = (char)octal; s += 3; }
                  else value[j++] = '\0';
               }
               else if (s + 1 < e && isdigit(input[s + 2]))
               {
                  // \0N — two digit octal starting with 0
                  int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
                  value[j++] = (char)octal;
                  s         += 2;
               }
               else {
                  value[j++] = '\0';
                  s++;
               }
               break; // plain null
            }
            case '\\': value[j++] = '\\'; s++; break; // backslash
            case '"': value[j++]  = '"'; s++; break;  // double quote
            case '\'': value[j++] = '\''; s++; break; // single quote
            case '?': value[j++]  = '\?'; s++; break; // question mark (for trigraphs)
            // three digit octal
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            {
               if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
               {
                  int octal = (input[s + 1] - '0') * 64 +
                              (input[s + 2] - '0') * 8 +
                              (input[s + 3] - '0');
                  if (octal <= 255) { value[j++] = (char)octal; s += 3; }
                  // invalid octal, keep backslash
                  else value[j++] = input[s];
               }
               // two digit octal
               else if (s + 2 < e && isdigit(input[s + 2]))
               {
                  int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
                  value[j++] = (char)octal;
                  s         += 2;
               }
               // single digit octal
               else
               {
                  value[j++] = (char)(input[s + 1] - '0');
                  s++;
               }
               break;
            }
            case 'x':   // Hexadecimal escape sequences (\x00 to \xFF)
            {
               if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
               {
                  int  hex = 0;
                  char c1  = input[s + 2];
                  char c2  = input[s + 3];
                  // convert first hex digit
                  if (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
                  else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
                  else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;
                  // convert second hex digit
                  if (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
                  else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
                  else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);
                  value[j++] = (char)hex;
                  s         += 3;
               }
               else value[j++] = input[s]; // invalid hex escape, keep backslash
               break;
            }
            case 'u':   // \uXXXX - 4 hex digits for Unicode, basic implementation
            {
               if (s + 5 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;
            }
            case 'U':   // \UXXXXXXXX - 8 hex digits for Unicode, basic implementation
            {
               if (s + 9 < e) value[j++] = input[s]; // for now
               else value[j++] = input[s];
               break;
            }
            // unknown escape sequence, keep the backslash
            default: value[j++] = input[s]; break;
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
      char value = 0;
      if (e == s + 1) value = input[s];
      else   // special character
      {
         char escape_map[256] =
         {
            ['n'] = '\n', ['t'] = '\t', ['r'] = '\r',
            ['b'] = '\b', ['f'] = '\f', ['v'] = '\v',
            ['a'] = '\a', ['0'] = '\0', ['\\'] = '\\',
            ['"'] = '\"', ['\''] = '\'', ['?'] = '\?',
         };
         char c = escape_map[(unsigned char)input[s + 1]];
         check(!c && input[s + 1] != '0', "unknown escape character");
         value = c;
      }
      new->Char.value = value;
      break;
   }
   default:
      break;
   }
   return new;
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

void enter_scoop(Node *node)
{
   debug(CYAN "Enter Scoop: %k index %d\n" RESET, node->token, scoop_pos + 1);
   Gscoop = (Node**)resize_array(
      (void**)Gscoop,
      &scoop_len, scoop_pos, sizeof(Node*));
   scoop_pos++;
   Gscoop[scoop_pos] = node;
   scoop             = Gscoop[scoop_pos];
}

void exit_scoop()
{
   if (check(scoop_pos < 0, "No active scoop to exit")) return;
   debug("%sExit Scoop: %k index %d%s\n", CYAN, Gscoop[scoop_pos]->token, scoop_pos, RESET);
   Gscoop[scoop_pos] = NULL;
   scoop_pos--;
   scoop             = Gscoop[scoop_pos];
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
      [TYPEOF]     = "TYPEOF", [SIZEOF] = "SIZEOF", [ARGS] = "ARGS",
      [CHILDREN]   = "CHILDREN",
      [AS]         = "AS", [STACK] = "STACK", [HEAP] = "HEAP",
      [ARRAY_TYPE] = "ARRAY_TYPE",
      //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
      [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
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
   debug(CYAN "new struct [%s] in scoop %k\n" RESET, node->token->name, scoop->token);
   for (int i = 0; i < scoop->spos; i++)
   {
      Token *curr = scoop->structs[i]->token;
      bool   cond = (strcmp(curr->name, node->token->name) == 0);
      check(cond, "Redefinition of %s", node->token->name);
   }
   add_struct(scoop, node);
   return node;
}

Node *get_struct(char *name)
{
   debug(CYAN "get struct [%s] from scoop %k\n"RESET, name, scoop->token);
   for (int j = scoop_pos; j > 0; j--)
   {
      Node *node = Gscoop[j];
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

// IR GENERATION
void add_variable(Node *b, Token *token)
{
   b->variables = (Token**)resize_array(
      (void**)b->variables,
      &b->vlen, b->vpos, sizeof(Token *));
   b->variables[b->vpos++] = token;
}

Token *new_variable(Token *token)
{
   debug(CYAN "new variable [%k] in scoop %k\n" RESET, token, scoop->token);
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->variables[i];
      bool   cond = (strcmp(curr->name, token->name) == 0);
      check(cond, "Redefinition of %s", token->name);
   }
   add_variable(scoop, token);
   return token;
}

Token *get_variable(char *name)
{
   debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
   for (int j = scoop_pos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->variables[i]->name, name) == 0)
            return scoop->variables[i];
   }
   check(1, "%s not found", name);
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
   for (int i = 0; i < scoop->fpos; i++)
   {
      Node *func = scoop->functions[i];
      bool  cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s", node->token->name);
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
   check(1, "'%s' Not found", name);
   return syntax_error();
}

// ASM GENERATION
void init(char *name)
{
   context = LLVMContextCreate();
   module  = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);


   vd  = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1  = LLVMInt1TypeInContext(context);
   i8  = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8  = _pointer_type(i8, 0);
   p32 = _pointer_type(i32, 0);

   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   #if defined(__APPLE__)
   LLVMSetTarget(module, "arm64-apple-macosx16.0.0");
   #elif defined(__linux__)
   LLVMSetTarget(module, "x86_64-pc-linux-gnu");
   #else
   LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
   #endif

   // if (enable_asan)
   // {
   LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Debug Info Version",
                     18, LLVMValueAsMetadata(LLVMConstInt(i32, 3, 0)));
   LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Dwarf Version",
                     13, LLVMValueAsMetadata(LLVMConstInt(i32, 4, 0)));
   // }

   // Debug info
   di_builder = LLVMCreateDIBuilder(module);
   char *base          = strrchr(name, '/');
   char *filename_only = base ? base + 1 : name;
   char  dir[1024]     = ".";
   if (base) { size_t len = base - name; strncpy(dir, name, len); dir[len] = '\0'; }

   di_file = LLVMDIBuilderCreateFile(
      di_builder, filename_only,
      strlen(filename_only), dir, strlen(dir));

   di_compile_unit = LLVMDIBuilderCreateCompileUnit(
      di_builder, LLVMDWARFSourceLanguageC,
      di_file, "ura", 3, 0, "", 0, 0, "", 0,
      LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);

   di_current_scope = di_compile_unit;

   // if (using_refs) getRefAssignFunc();
   // if (enable_bounds_check) getNullCheckFunc();
}

void finalize(char *output)
{
   char *error = NULL;

   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();

   LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

   // if (enable_asan)
   // {
   //    // LLVMErrorRef err = LLVMRunPasses(module, "asan-module,asan", NULL, options);
   //    LLVMErrorRef err = LLVMRunPasses(module, "asan,asan-stack", NULL, options);
   //    if (err)
   //    {
   //       char *msg = LLVMGetErrorMessage(err);
   //       fprintf(stderr, RED "ASan Error: %s\n" RESET, msg);
   //       LLVMDisposeErrorMessage(msg);
   //    }
   // }

   if (passes)
   {
      LLVMErrorRef err = LLVMRunPasses(module, passes, NULL, options);
      if (err)
      {
         char *msg = LLVMGetErrorMessage(err);
         fprintf(stderr, RED "Optimizer Error: %s\n" RESET, msg);
         LLVMDisposeErrorMessage(msg);
         found_error = true;
         return;
      }
   }

   LLVMDIBuilderFinalize(di_builder);
   LLVMDisposeDIBuilder(di_builder);
   di_builder = NULL;

   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error))
   {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
      LLVMDisposePassBuilderOptions(options);
      found_error = true;
      // return;
   }
   else
   {
      LLVMDisposePassBuilderOptions(options);

   }

   LLVMPrintModuleToFile(module, output, NULL);
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
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

// DEBUGING AND ERROR HANDLING
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

// TO BE USED LATER

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

   bool lt_numeric = includes(lt, INT, LONG, SHORT, BOOL, CHAR, 0);
   bool rt_numeric = includes(rt, INT, LONG, SHORT, BOOL, CHAR, 0);
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

   switch (token->type)
   {
   case INT: case BOOL: case CHAR: case FLOAT: case LONG: case VOID: case PTR:
      return token->type;
   case FCALL: return token->Fcall.ptr ? token->Fcall.ptr->token->retType : 0;
   case CHARS:  case STRUCT_CALL: return token->type;
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN:
   case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
      return get_ret_type(left);
   case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case AND: case OR: case NOT: case BNOT:
      return BOOL;
   case AS:
      return right ? (right->token->retType ?
                      right->token->retType : right->token->type) : 0;
   case RETURN: return get_ret_type(left);
   case DOT: return get_ret_type(right);
   case ACCESS:
   {
      Type left_type = get_ret_type(left);
      if (left_type == CHARS || left_type == STACK) return CHAR;
      return left_type;
   }
   case ID: return token->type != ID ? token->type : 0;
   default:
      todo(1, "handled this case [%s]", to_string(token->type));
      return 0;
   }
}

void set_ret_type(Node *node)
{
   if (!node || !node->token) return;
   Type t = get_ret_type(node);
   if (t) node->token->retType = t;

   if (node->token->type == DOT &&
       node->token->retType == STRUCT_CALL && node->right)
      node->token->Struct.ptr = node->right->token->Struct.ptr;
}