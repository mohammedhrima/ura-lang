#include "./header.h"

bool found_error;

Token **tokens;
int tk_pos;
int tk_len;

char *input;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

int ptr;
#if defined(__APPLE__)
struct __sFILE *asm_fd;
#elif defined(__linux__)
struct _IO_FILE *asm_fd;
#endif


void tokenize(char *filename)
{
   if (found_error) return;
#if DEBUG
   debug(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);
#endif
   char *input = open_file(filename);
   if (!input) return;

   struct { char *value; Type type; } specials[] = {
      {"->", ARROW},
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
      {"[", LBRA}, {"]", RBRA}, {",", COMA}, {"&&", AND}, {"||", OR},
      {0, (Type)0}
   };

   int space = 0;
   int line = 0;
   bool new_line = true;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            line++;
            new_line = true;
            space = 0;
         }
         else if (new_line)
         {
            if (input[i] == '\t') space += TAB;
            else space++;
         }
         i++;
         continue;
      }
      else if (strncmp(input + i, "/*", 2) == 0)
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
      else if (strncmp(input + i, "//", 2) == 0)
      {
         while (input[i] && input[i] != '\n') i++;
         continue;
      }
      new_line = false;
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
         {
            parse_token(NULL, 0, 0, specials[j].type, space, filename, line);
            found = true;
            i += strlen(specials[j].value);
            if (includes(specials[j].type, ARROW, DOTS, 0)) space += TAB;
            break;
         }
      }
      if (found) continue;
      if (input[i] && input[i] == '\"')
      {
         i++;
         while (input[i] && input[i] != '\"') i++;
         check(input[i] != '\"', "Expected '\"'");
         i++;
         parse_token(input, s + 1, i - 1, CHARS, space, filename, line);
         continue;
      }
      if (input[i] && input[i] == '\'')
      {
         i++;
         if (input[i] == '\\') i++;
         if (input[i] && input[i] != '\'')
            i++;
         check(input[i] != '\'', "Expected '\''");
         parse_token(input, s + 1, i, CHAR, space, filename, line);
         i++;
         continue;
      }
      if (isalpha(input[i]) || strchr("@$-_", input[i]))
      {
         while (input[i] && (isalnum(input[i]) || strchr("@$-_", input[i]))) i++;
         if (strncmp(input + s, "use ", 4) == 0)
         {
            while (isspace(input[i])) i++;
            check(input[i] != '\"', "expected \" after use"); i++;
            s = i;
            while (input[i] && !isspace(input[i]) && input[i] != '\"') i++;
            check(input[i] != '\"', "expected \""); i++;

            char *use = allocate(i - s, sizeof(char));
            strncpy(use, input + s, i - s - 1);

            char *use_filename = strjoin(dirname(filename), "/", use);
            free(use);
            use = strjoin(use_filename, ".ura", NULL);
            tokenize(use);
            free(use_filename);
         }
         else parse_token(input, s, i, ID, space, filename, line);
         continue;
      }
      if (isdigit(input[i]))
      {
         while (isdigit(input[i])) i++;
         parse_token(input, s, i, INT, space, filename, line);
         continue;
      }
      check(input[i], "Syntax error <%c>\n", input[i]);
   }
   free(input);
}

Node *expr() {
   return assign();
}

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0)))
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
         node->right = new_node(copy_token(token));
         node->right->left = new_node(left->token);
         node->right->right = logic();
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
// AST_NODE(assign, logic, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0);
AST_NODE(logic, equality, AND, OR, 0);
// TODO: handle ! operator
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison, add_sub, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub, mul_div, ADD, SUB, 0);
AST_NODE(mul_div, dot, MUL, DIV, 0); // TODO: handle modulo %

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

Node *sign() // TODO: implement it
{
   Token *token;
   if ((token = find(SUB, 0)))
   {
      // TODO: left should be a number
      Node *node = new_node(token);
      token->type = MUL;
      node->left = prime();

      node->right = new_node(new_token(INT, node->left->token->space));
      node->right->token->Int.value = -1;
      return node;
   }
   return prime();
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
   if (check(!typeName || !fname,
             "expected data type and identifier after func declaration"))
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
      bool is_ref = find(REF, 0) != NULL;
      Token* data_type = find(DATA_TYPES, ID, 0);
      if (data_type && data_type->type == ID)
      {
         data_type = get_struct(data_type->name);
         if (data_type) data_type->type = STRUCT_CALL;
      }
      if (check(!data_type, "expected data type in function argument")) break;
      Token *name = find(ID, 0);
      if (check(!name, "expected identifier in function argument")) break;
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
         if (node->token->retType != VOID)
            check(!child || child->token->type != RETURN, "expected return statment");
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
   // Token *struct_token = NULL;
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
   if (check(!(st_name = find(ID, 0)),
             "expected identifier after struct definition"))
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

      // if (attr->type == ID) // attribute is a struct
      // {
      //    Token *st = get_struct(attr->name);
      //    if (check(!st, "Unkown data type [%s]\n", attr->name)) break;
      //    attr = st;
      //    char *name = id->name;
      //    id = attr;
      //    setName(id, name);
      //    id->type = STRUCT_CALL;

      // }
      // else
      id->type = attr->type;
      id->is_attr = true; // TODO: check if is-attr is useful
      add_attribute(node->token, id);
   }
   // set_struct_size(node->token);
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

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(CHILDREN, node->token->space));

   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());

   while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) &&
          within_space(node->token->space - TAB))
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

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->space = node->token->space;
   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   // code bloc
   while (within_space(node->token->space)) add_child(node, expr());

   exit_scoop();
   return node;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, LONG, SHORT, 0)))
      return symbol(token);
   else if ((token = find(STRUCT_DEF, 0)))
      return struct_def(new_node(token));
   // else if ((token = find(REF, 0)))
   // {
   //     node = prime(); // TODO: check it
   //     check(!node->token->is_declare, "must be variable declaration after ref");
   //     node->token->is_ref = true;
   //     return node;
   // }
   else if ((token = find(FDEC, PROTO_FUNC, 0))) return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
      // in current scoop
      node = new_node(token);
      node->left = expr();
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
   else check(1, "Unexpected token has type %s\n",
                 to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}

Token *func_dec_ir(Node *node)
{
   new_function(node);
   enter_scoop(node);
   // int tmp_ptr = ptr;
   // ptr = 0;

   Inst* inst = NULL;
   // if (!node->token->is_proto)
   inst = new_inst(node->token);

   // parameters
   Node **params = (node->left ? node->left->children : NULL);
   Token *token = node->token;
   for (int i = 0; params && i < node->left->cpos && !found_error; i++)
   {
      Node *child = params[i];
      generate_ir(child);
      if (token->Fdec.args == NULL)
      {
         token->Fdec.size = 10;
         token->Fdec.args = allocate(token->Fdec.size, sizeof(Token*));
      }
      else if (token->Fdec.pos + 1 == token->Fdec.size)
      {
         Token **tmp = allocate(token->Fdec.size *= 2, sizeof(Token*));
         memcpy(tmp, token->Fdec.args, token->Fdec.pos * sizeof(Token*));
         free(token->Fdec.args);
         token->Fdec.args = tmp;
      }
      child->token->is_param = true;
      child->token->Param.index = i;
      child->token->Param.func_ptr = node->token;
      child->token->is_declare = false;
      token->Fdec.args[token->Fdec.pos++] = child->token;
   }

   // if (node->token->is_proto) set_remove(node);
   // code bloc
   for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
   {
      Node *child = node->children[i];
      generate_ir(child);
   }

   if (!node->token->is_proto)
   {
      // TODO: if RETURN not found add it
      Token *new = new_token(END_BLOC, node->token->space);
      new->name = strdup(node->token->name);
      new_inst(new);
   }
   exit_scoop();
   // if (!node->token->is_proto)
   return inst->token;
}

Token *func_call_ir(Node *node)
{
   Inst* inst = NULL;
   // static Node *printf_func = NULL;

   if (strcmp(node->token->name, "output") == 0)
   {
#if 1
      todo(1, "handle this\n");
#else
      // if (!printf_func)
      // {

      // }

      node->token->Fcall.ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID

         Token *src = generate_ir(carg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;

         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      inst = new_inst(node->token);
#endif
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      node->token->Fcall.ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;

      func = copy_node(func);
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      if (check(fcall->cpos != fdec->cpos,
                "Incompatible number of arguments %s", func->token->name))
         return NULL;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID
         Node *darg = fdec->children[i];

         Token *src = generate_ir(carg);
         Token *dist = generate_ir(darg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;
         if (check(!compatible(src, dist), "Incompatible type arg %s",
                   func->token->name)) break;
         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      free_node(func);
      inst = new_inst(node->token);
   }
   return inst->token;
}

Token *if_ir(Node *node)
{
   enter_scoop(node);
   Node **children = node->right->children;
   int cpos = node->right->cpos;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (!cond) return NULL;

   // APPEND BLOC
   Token *start = copy_token(node->token);
   Inst *bloc = new_inst(copy_token(start));
   bloc->token->type = APPEND_BLOC;
   bloc->left = start;

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Inst *bloc = new_inst(copy_token(children[i]->token));
      bloc->token->type = APPEND_BLOC;
      bloc->left = children[i]->token;
   }

   // BUILD CONDITION
   // cond ? go to left : go to right
   Inst *inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = start;
   inst->right = children[0]->token;

   // SET POSITION start
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = start;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = children[cpos - 1]->token;

   // SET POSITION next

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Node *curr = children[i];
      inst = new_inst(copy_token(node->token));
      inst->token->type = SET_POS;
      inst->left = children[i]->token;
      switch (curr->token->type)
      {
      case ELSE:
      {
         enter_scoop(curr);
         for (int j = 0; j < curr->cpos; j++)
            generate_ir(curr->children[j]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(curr->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case ELIF:
      {
         enter_scoop(curr);
         Token *cond = generate_ir(curr->left); // TODO: check if it's boolean
         if (!cond) return NULL;
         i++;

         // BUILD CONDITION
         // cond ? go to left : go to right
         Inst *inst = new_inst(curr->token);
         inst->token->type = BUILD_COND;
         inst->token->Statement.ptr = cond;
         inst->left = children[i]->token;
         inst->right = children[i + 1]->token;


         // SET POSITION next
         inst = new_inst(copy_token(node->token));
         inst->token->type = SET_POS;
         inst->left = children[i]->token;

         for (int i = 0; i < curr->cpos && !found_error; i++)
            generate_ir(curr->children[i]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(node->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case END_IF: break;
      default: break;
      }
   }
   exit_scoop();
   return NULL;
}

Token *while_ir(Node *node)
{
   enter_scoop(node);
   // APPEND BLOC
   Token *loop_cond = copy_token(node->token);
   setName(loop_cond, "while");
   Inst *bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_cond;

   Token *loop_body = copy_token(node->token);
   setName(loop_body, "while_bloc");
   bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_body;

   Token *loop_end = copy_token(node->token);
   setName(loop_end, "end_while");
   bloc = new_inst(copy_token(loop_end));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_end;

   node->token->Statement.start = loop_cond;
   node->token->Statement.end = loop_end;

   // BUILD BR
   Inst *inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_cond;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (!cond) return NULL;

   // BUILD CONDITION
   // cond ? go to left : go to right
   inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = loop_body;
   inst->right = loop_end;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_body;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_end;

   exit_scoop();
   return NULL;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   if (!right || !right) return NULL;

   check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
         to_string(node->token->type), to_string(left->type), to_string(right->type));
   switch (node->token->type)
   {
   case ASSIGN:
   {
      node->token->ir_reg = left->ir_reg;
      node->token->retType = getRetType(node);
#if 0
      // if (left->is_ref) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
      //         else node->token->assign_type = REF_HOLD_REF;
      //     }
      //     else if (right->ptr || right->creg) // ptr
      //     {
      //         if (left->has_ref) node->token->assign_type = REF_ID;
      //         else node->token->assign_type = REF_HOLD_ID;
      //     }
      //     else // ir_reg, value
      //     {
      //         debug("line %d: %n\n", LINE, node);
      //         // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
      //         node->token->assign_type = REF_VAL;
      //     }
      //     left->has_ref = true;
      // }
      // else if (left->ptr || left->creg) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         node->token->assign_type = ID_REF;
      //     }
      //     else if (right->ptr) // ptr
      //         node->token->assign_type = ID_ID;
      //     else // ir_reg, value
      //         node->token->assign_type = ID_VAL;
      // }
      // else if (left->type == STRUCT_CALL)
      // {
      //     stop(1, "check this");
      //     debug(">> %k\n", left);
      //     debug(">> %k\n", right);
      //     // TODO: check compatibility
      //     for (int i = 0; i < left->Struct.pos; i++)
      //     {
      //         Node *tmp = new_node(new_token(ASSIGN, node->token->space));
      //         tmp->left = new_node(left->Struct.attrs[i]);
      //         tmp->right = new_node(right->Struct.attrs[i]);
      //         op_ir(tmp);
      //         free_node(tmp);
      //     }
      //     return NULL;
      //     // exit(1);
      // }
      // else
      // {
      //     pnode(node, NULL, 0);
      //     debug("<%k>\n", left);
      //     debug("<%k>\n", right);
      //     // todo(1, "Invalid assignment");
      // }
#endif
      break;
   }
   case ADD: case SUB: case MUL: case DIV:
   {
      // TODO: to be checked
      // node->token->retType = INT;
      // if (node->token->retType  == INT) setReg(node->token, "eax");
      // else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
      // else
      // check(1, "handle this case");
      break;
   }
   case AND: case OR:
   {
      node->token->retType = BOOL;
      break;
   }
   case NOT_EQUAL: case EQUAL: case LESS:
   case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      node->token->retType = BOOL;
      // node->token->index = ++bloc_index;
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   return node->token;
}

Token *struct_def_ir(Node *node)
{
   new_inst(node->token);
   return node->token;
}

Token *struct_call_ir(Node *node)
{
   // ALLOCATE STRUCT
   // ADD ATTRIBUTE
#if 0
   Inst *inst = NULL;
   Token **attrs = node->token->Struct.attrs;
   int attrs_size = node->token->Struct.pos;
   // node->token->Struct.attrs = NULL;
   // node->token->Struct.pos = 0;

   // set struct body
   // Token *body = copy_token(node->token);
   // body->type = STRUCT_BODY;
   // inst = new_inst(body);
   // inst->left = node->token;

   // allocate struct
   // Token *alloca_st = copy_token(node->token);
   // alloca_st->type = STRUCT_ALLOC;
   // inst = new_inst(alloca_st);
   // inst->left = node->token;

   // TODO: handle children if they are struct type
   // ...
   for (int i = 0; i < attrs_size; i++)
   {

   }
#endif
   return node->token;
}

Token *generate_ir(Node *node)
{
   Inst *inst = NULL;
   if (found_error) return NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find) return find;
      return node->token;
   }
   // case VOID: return node->token;
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      inst = new_inst(node->token);
      if (node->token->type == STRUCT_CALL) struct_call_ir(node);
      if (node->token->is_declare) new_variable(node->token);
      return node->token;
   }
   case STRUCT_DEF: return struct_def_ir(node);
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
   case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      // TODO: check if right is DEFAULT, then initlize it, and return left
      return op_ir(node);
   }
   case AND: case OR:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(!(left->type == BOOL || left->retType == BOOL), "left should be boolean");
      check(!(right->type == BOOL || right->retType == BOOL), "right should be boolean");
      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      return inst->token;
   }
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FCALL: return func_call_ir(node);
   case FDEC:  return func_dec_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      return node->token;
   }
   case BREAK: case CONTINUE:
   {
      debug(RED"handle BREAK/CONTINUE\n");
      bool found = false;
      for (int i = scoopPos; i >= 0 && !found; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            // BUILD BR
            found = true;
            Inst *inst = new_inst(node->token);
            if (node->token->type == BREAK)
            {
               inst->left = scoop->token->Statement.end;
               // debug(RED "break to %k\n" RESET, scoop->token->Statement.end);
            }
            else inst->left = scoop->token->Statement.start;
            inst->token->type = BUILD_BR;
         }
      }
      todo(!found, "Invalid syntax\n");
      break;
   }
   case DOT:
   {
      Token *left = generate_ir(node->left); // struct name
      Token *right = node->right->token; // attribute
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(left->type != STRUCT_CALL,
                "%s should be a struct call", left->name)) break;
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         // debug("compare %s == %s\n", attr->name, right->name);
         if (strcmp(attr->name, right->name) == 0)
         {
            attr->Struct.attr_index = i;
            attr->Struct.ptr = left->Statement.ptr;
            Inst *inst =  new_inst(node->token);

            inst->left = left;
            inst->right = attr;
            inst->token->is_attr = true;
            return inst->token;
         }
      }

      check(1, "%s has no attribute %s", left->name, right->name);
      break;
   }
   case ACCESS:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(right->type != INT, "should be int")) break;

      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      switch (inst->left->type)
      {
      case CHARS: inst->token->retType = CHAR; break;
      default: check(1, "handle this case %s\n", to_string(inst->left->type)); break;
         break;
      }
      return node->token;
      break;
   }
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
   return NULL;
}

void compile(char *filename)
{
   tokenize(filename);
   new_token(END, -1);
   if (found_error) return;

   Node *global = new_node(new_token(ID, -TAB - 1));
   setName(global->token, ".global");
   enter_scoop(global);

   if (DEBUG) debug(GREEN BOLD"AST:\n" RESET);
   while (tokens[exe_pos]->type != END && !found_error)
      add_child(global, expr());
   print_ast(global);
   if (found_error) return;

#if IR
   if (DEBUG) debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++)
      generate_ir(global->children[i]);
   if (found_error) return;
   if (DEBUG) print_ir();
#endif

#if OPTIMIZE
   debug(GREEN BOLD"OPTIMIZE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   copy_insts();
   while (OPTIMIZE && !found_error && optimize_ir()) copy_insts();
#if !DEUBG
   print_ir();
#endif
#endif

#if ASM
   if (DEBUG) debug(GREEN BOLD"GENERATE ASSEMBLY CODE:\n" RESET);
   copy_insts();
   generate_asm(filename);
#endif

   free_node(global);
   if (DEBUG) debug(BLUE BOLD"FINISH COMPILATION:\n" RESET);
}

int main(int argc, char **argv)
{
   check(argc < 2, "require argument, usage ura <file.ura>");
   compile(argv[1]);
   free_memory();
   if (DEBUG) debug(BLUE BOLD"EXIT CODE:\n" RESET);
#if !IR
   debug(RED"NO IR GENERATION\n"RESET);
#endif
#if !ASM
   debug(RED"NO ASSEMBLY GENERATION\n"RESET);
#endif
   if (found_error || !ASM || !IR) exit(1);
}