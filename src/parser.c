#include "ura.h"

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
      if (check(to == NULL || !to->is_dec, "expected data type after to"))
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
      if (token->type == DOT)
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

// ----------------------------------------------------------------------------
// prime_node sub-parsers (token already consumed by caller)
// ----------------------------------------------------------------------------

// [INT/CHARS/…] varname               variable declaration
// [INT] [42]                           literal value
// [STRUCT_CALL] varname → StructType  struct variable declaration
// [TUPLE_UNPACK]                       a int, b chars = fn()
// ├──[INT] a
// ├──[CHARS] b
// └──left: [FCALL] fn ret [TUPLE]
// [FCALL] name ret [TYPE]             function call
// └──[ARGS]
//    ├──arg1
//    └──arg2
static Node *parse_var_dec(Token *token)
{
   Node *node   = NULL;
   Node *st_dec = NULL;

   // Literal value: "hello", 1, 'c'
   if (token->type != ID && !token->is_dec && !token->name)
      return new_node(token);
   // int, char, chars, etc. declaration keyword
   if (token->is_dec) return new_node(token);

   // variable declaration: name type [ref] [= ...]
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
      // Tuple unpack: a int, b chars = foo()
      if (!is_ref
          && tokens[exe_pos]->type == COMA
          && exe_pos + 2 < tk_pos
          && tokens[exe_pos + 1]->type == ID
          && is_data_type(tokens[exe_pos + 2]))
      {
         Token *tu_tok = new_token(TUPLE_UNPACK, token->space);
         Node  *tu     = new_node(tu_tok);
         add_child(tu, new_node(tmp)); // first decl
         while (tokens[exe_pos]->type == COMA)
         {
            exe_pos++;                // consume ,
            Token *nname = find(ID, 0);
            Token *ntype = find(DATA_TYPES, 0);
            if (ntype->type == ARRAY_TYPE)
            {
               expect_token(LBRA, "expected [ after array");
               int    depth     = 1;
               while (find(LBRA, 0)) depth++;
               Token *elem_type = find(DATA_TYPES, ID, 0);
               check(!elem_type, "expected element type in array type");
               for (int i = 0; i < depth; i++)
                  expect_token(RBRA, "expected ] in array type");
               ntype->Array.elem_type = elem_type->type;
               ntype->Array.depth     = depth;
               ntype->retType         = ARRAY_TYPE;
            }
            setName(ntype, nname->name);
            ntype->is_dec = true;
            add_child(tu, new_node(ntype));
         }
         expect_token(ASSIGN, "expected = in tuple unpack");
         tu->left = expr_node();
         return tu;
      }
      return new_node(tmp);
   }

   // variable declaration (struct type): name StructName [ref] [= ...]
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

   // function call or main declaration
   if (token->type == ID && find(LPAR, 0))
   {
      node = new_node(token);
      if (strcmp(token->name, "main") == 0)
      {
         // Function main:
         //    + children: code bloc
         expect_token(RPAR, "expected ) after main declaration");
         expect_token(DOTS, "expected : after main() declaration");

         enter_scope(node);
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
         exit_scope();
      }
      else
      {
         // Function call:
         //    + left:
         //       + children: parameters
         node->token->type = FCALL;
         Token *arg        = NULL;
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

// [STRUCT_DEF] Name
// ├──[FIELD_TYPE] field1
// ├──[FIELD_TYPE] field2
// └──methods[]: [FDEC] Name.method …
static Node *parse_struct_def(Token *token)
{
   // Struct def layout:
   //    + children: attributes
   //    + methods[]: method FDEC nodes
   Node  *node = new_node(token);
   Token *st_name;
   if (check(!(st_name = find(ID, 0)), "expected identifier after struct definition"))
      return NULL;
   expect_token(DOTS, "expected dots after struct definition\n");

   setName(node->token, st_name->name);
   node->token->type       = STRUCT_DEF;
   node->token->Struct.ptr = node;
   new_struct(node);
   enter_scope(node);

   Node **methods      = NULL;
   int    method_count = 0;
   int    method_cap   = 0;

   while (within(node->token->space))
   {
      Node *child = prime_node();
      if (!child) return syntax_error();
      if (child->token->type == FDEC)
      {
         methods = (Node**)resize_array((void**)methods, &method_cap, method_count, sizeof(Node*));
         methods[method_count++] = child;
      }
      else
      {
         if (check(!child->token->is_dec, "invalid attribute"))
         {
            exit_scope();
            free(methods);
            return syntax_error();
         }
         child->token->is_dec = false;
         add_child(node, child);
      }
   }
   exit_scope();

   // Hoist methods AFTER struct is registered so gen_asm sees STRUCT_DEF first
   for (int i = 0; i < method_count; i++)
   {
      Token *mt = methods[i]->token;
      if (strcmp(mt->name, "init") == 0)
      {
         if (check(mt->retType != VOID, "struct '%s': init() must return void", node->token->name))
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
      else if (strcmp(mt->name, "clean") == 0)
      {
         if (check(mt->retType != VOID, "struct '%s': clean() must return void", node->token->name))
         {
            free(methods);
            return syntax_error();
         }
         if (check(methods[i]->left->cpos != 1, "struct '%s': clean() must take no parameters", node->token->name))
         {
            free(methods);
            return syntax_error();
         }
         node->token->is_clean = true;
      }
      char *qualified = strjoin(node->token->name, ".", mt->name);
      setName(mt, qualified);
      free(qualified);
      add_method(node, methods[i]);
   }
   free(methods);
   return node;
}

// [IF]
// ├──left:     condition
// ├──children: body stmts
// └──right: [ELIF]
//           ├──left:     condition
//           ├──children: body stmts
//           └──right: [ELSE]
//                     └──children: body stmts
static Node *parse_if(Token *token)
{
   // if/elif/else layout:
   //    + left    : condition
   //    + children: code bloc
   //    + right   : elif/else node (same layout)
   Node *node = new_node(token);
   enter_scope(node);

   node->left = expr_node();
   expect_token(DOTS, "expected : after if condition");

   while (within(node->token->space)) add_child(node, expr_node());

   Node *curr = node;
   while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within(node->token->space - TAB))
   {
      Token *tok = find(ELSE, ELIF, 0);

      curr->right = new_node(tok);
      curr        = curr->right;

      if (tok->type == ELIF)
      {
         enter_scope(curr);
         curr->left = expr_node();
         expect_token(DOTS, "expected : after elif condition");
         while (within(tok->space)) add_child(curr, expr_node());
         exit_scope();
      }
      else if (tok->type == ELSE)
      {
         enter_scope(curr);
         expect_token(DOTS, "expected : after else");
         while (within(tok->space)) add_child(curr, expr_node());
         exit_scope();
         break;
      }
   }
   exit_scope();
   return node;
}

// [FDEC] name ret [TYPE]
// ├──[ARGS]
// │  ├──[TYPE] param1
// │  └──[STRUCT_CALL] param2    (struct param)
// └──children: body stmts
//
// [FDEC] name ret [TUPLE]       (tuple return)
// ├──[ARGS]
// │  └──…
// └──children: body stmts
static Node *parse_fdec(Token *token)
{
   // Function declaration layout:
   //    + left:
   //       + children: arguments
   //    + children: code block
   Node  *node = new_node(token);
   Token *fname = find(ID, 0);
   if (check(!fname, "expected identifier after fn declaration"))
      return syntax_error();

   // Check if we're inside a struct scope (method)
   Node *struct_owner = NULL;
   Node *parent       = Gscope[scope_pos];
   if (parent && parent->token->type == STRUCT_DEF)
      struct_owner = parent;

   enter_scope(node);
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
            data_type->is_ref = is_ref;
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

   if (tokens[exe_pos]->type == LPAR)
   {
      // Tuple return type: fn foo(…) (int, chars):
      exe_pos++; // consume (
      node->token->retType = TUPLE;
      int tc = 0;
      while (tokens[exe_pos]->type != RPAR && tc < 8)
      {
         if (tokens[exe_pos]->type == ARRAY_TYPE)
         {
            Token *t  = tokens[exe_pos++];
            expect_token(LBRA, "expected [ in tuple array type");
            Token *et = find(DATA_TYPES, ID, 0);
            if (check(!et, "expected element type in tuple array")) return syntax_error();
            expect_token(RBRA, "expected ] in tuple array type");
            t->Array.elem_type             = et->type;
            t->retType                     = ARRAY_TYPE;
            node->token->Tuple.types[tc++] = t;
         }
         else if (tokens[exe_pos]->type == ID)
         {
            Token *t  = tokens[exe_pos++];
            Node  *st = get_struct(t->name);
            if (check(!st, "unknown type '%s' in tuple", t->name)) return syntax_error();
            t->type       = STRUCT_CALL;
            t->Struct.ptr = st;
            node->token->Tuple.types[tc++] = t;
         }
         else if (is_data_type(tokens[exe_pos]))
         {
            node->token->Tuple.types[tc++] = tokens[exe_pos++];
         }
         else
         {
            check(1, "unexpected token in tuple return type");
            return syntax_error();
         }
         if (tokens[exe_pos]->type == COMA) exe_pos++;
      }
      expect_token(RPAR, "expected ) after tuple return types");
      node->token->Tuple.count = tc;
   }
   else
   {
      if (check(!is_data_type(tokens[exe_pos]) && tokens[exe_pos]->type != ID,
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
   }
   node->token->is_ref = find(REF, 0) != NULL;
   setName(node->token, fname->name);

   if (!node->token->is_proto)
   {
      Token *next = find(DOTS, 0);
      if (check(!found_error && !next, "expected : after function declaration"))
         return syntax_error();
      Node *child = NULL;

      if (next->type == DOTS)
         while (within(node->token->space)) child = add_child(node, expr_node());
      else
      {
         Token *retToken = copy_token(next);
         retToken->type  = RETURN;
         Node  *retNode  = new_node(retToken);
         retNode->left   = expr_node();
         child           = add_child(node, retNode);
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
   exit_scope();
   return node;
}

// [RETURN] ret [TYPE]        (single value)
// └──left: expr
//
// [RETURN] ret [TUPLE]       (multiple values)
// ├──expr1
// └──expr2
//
// [RETURN] ret [VOID]        (void function — left = [VOID] copy)
// └──left: [VOID]
static Node *parse_return(Token *token)
{
   // TODO: check if return type is compatible with function in current scope (done in gen_ir)
   Node *node = new_node(token);
   for (int i = scope_pos; i >= 0; i--)
   {
      Node *curr = Gscope[i];
      if (curr->token->type == FDEC)
      {
         if (curr->token->retType == VOID)
         {
            node->left              = copy_node(node);
            node->left->token->type = VOID;
         }
         else if (curr->token->retType == TUPLE)
         {
            add_child(node, expr_node()); // first value
            while (tokens[exe_pos]->type == COMA)
            {
               exe_pos++;
               add_child(node, expr_node());
            }
         }
         else node->left = expr_node();
         break;
      }
   }
   return node;
}

// ----------------------------------------------------------------------------
// Primary expression parser
// ----------------------------------------------------------------------------
Node *prime_node()
{
   Node  *node = NULL;
   Token *token;

   if ((token = find(ID, DATA_TYPES, 0))) return parse_var_dec(token);
   if ((token = find(STRUCT_DEF, 0)))     return parse_struct_def(token);
   if ((token = find(IF, 0)))             return parse_if(token);

   if ((token = find(WHILE, 0)))
   {
      // while layout:
      //    + left    : condition
      //    + children: code bloc
      node = new_node(token);
      enter_scope(node);
      node->left = expr_node();
      expect_token(DOTS, "expected : after if condition");
      while (within(node->token->space)) add_child(node, expr_node());
      exit_scope();
      return node;
   }
   if ((token = find(BREAK, CONTINUE, 0)))
      return new_node(token);

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
   if ((token = find(OUTPUT, 0)))
   {
      node       = new_node(token);
      expect_token(LPAR, "output: expected (");
      node->left = new_node(new_token(ARGS, token->space));
      Token *end_tok = NULL;
      while (!found_error && !(end_tok = find(RPAR, END, 0)))
      {
         Node *curr = expr_node();
         add_child(node->left, curr);
         find(COMA, 0);
      }
      if (check(!found_error && end_tok->type != RPAR, "output: expected )"))
         return syntax_error();
      node->token->retType = VOID;
      return node;
   }
   if ((token = find(PROTO, 0)))
   {
      if (includes(tokens[exe_pos]->type, FDEC, STRUCT_DEF, 0))
         tokens[exe_pos]->is_proto = true;
      else check(1, "expected <fn> or <struct> after proto");
      return expr_node();
   }

   if ((token = find(FDEC, 0)))   return parse_fdec(token);
   if ((token = find(RETURN, 0))) return parse_return(token);

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
      // TODO: add disable default if next is assignment
#if 0
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
