#include "./include/header.h"

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
bool found_error;
bool did_pasm;
char *input;
Token **tokens;
Node *head;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos = -1;

int ptr;
struct _IO_FILE *asm_fd;
int str_index;
int bloc_index;

char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9", NULL};
char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9", NULL};

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
// Tokenization - Convert source text to token stream
// Syntax Analysis - Build abstract syntax tree from tokens
void tokenize()
{
   if (found_error) return;
#if DEBUG
   debug(GREEN BOLD"TOKENIZE:\n" RESET);
#endif
   struct { char *value; Type type; } specials[] = {
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR}, {"[", LBRA}, {"]", RBRA},
      {",", COMA}, {"&&", AND}, {"||", OR}, {0, (Type)0}
   };

   int space = 0;
   bool inc_space = true;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            inc_space = true;
            space = 0;
         }
         else if (inc_space)
         {
            if (input[i] == '\t') space += TAB;
            else space++;
         }
         i++;
         continue;
      }
      // TODO: handle new lines inside commebnt
      else if (strncmp(input + i, "/*", 2) == 0)
      {
         i += 2;
         while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2)) i++;
         check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'\n");
         i += 2;
         continue;
      }
      else if (strncmp(input + i, "//", 2) == 0)
      {
         while (input[i] && input[i] != '\n') i++;
         continue;
      }
      inc_space = false;
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
         {
            new_token(NULL, 0, 0, specials[j].type, space);
            found = true;
            i += strlen(specials[j].value);
            if (strcmp(specials[j].value, ":") == 0) space += TAB;
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
         new_token(input, s, i, CHARS, space);
         continue;
      }
      if (input[i] && input[i] == '\'')
      {
         i++;
         if (input[i] && input[i] != '\'') i++;
         check(input[i] != '\'', "Expected '\''");
         i++;
         new_token(input, s + 1, i, CHAR, space);
         continue;
      }
      if (isalpha(input[i]) || strchr("@$-_", input[i]))
      {
         while (input[i] && (isalnum(input[i]) || strchr("@$-_", input[i]))) i++;
         new_token(input, s, i, ID, space);
         continue;
      }
      if (isdigit(input[i]))
      {
         while (isdigit(input[i])) i++;
         new_token(input, s, i, INT, space);
         continue;
      }
      check(input[i], "Syntax error <%c>\n", input[i]);
   }
   new_token(input, 0, 0, END, -1);
}

Node *expr()
{
   return assign();
}

AST_NODE(assign, logic, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0);
AST_NODE(logic, equality, AND, OR, 0);
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0); // TODO: handle ! operator
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
      if (check(!token, "error, expected id after dot")) exit(1);
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
   return prime();
}

/******************************************
 *  Function Declaration Layout:          *
 *  ┌───────────────┐    ┌───────────┐    *
 *  │ left children │ -> │ Arguments │    *
 *  └───────────────┘    └───────────┘    *
 *  ┌───────────────┐    ┌────────────┐   *
 *  │ children      │ -> │ Code block │   *
 *  └───────────────┘    └────────────┘   *
 ******************************************/
Node *func_dec(Node *node)
{
   Token *typeName = find(INT, CHARS, CHAR, FLOAT, BOOL, VOID, ID, 0);
   if (typeName->type == ID)
   {
      typeName = get_struct(typeName->name);
      todo(1, "handle function return struct properly");
   }
   Token *fname = find(ID, 0);
   if (check(!typeName || !fname, "expected data type and identifier after func declaration"))
      return node;
   node->token->retType = typeName->type;
   setName(node->token, fname->name);
   enter_scoop(node);

   check(!find(LPAR, 0), "expected ( after function declaration");
   node->left = new_node(new_token(0, 0, 0, CHILDREN, node->token->space));
   Token *last;
   while (!found_error && !(last = find(RPAR, END, 0)))
   {
      bool is_ref = find(REF, 0) != NULL;
      Token* data_type = find(INT, CHARS, CHAR, FLOAT, BOOL, ID, 0);
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
      add_child(node->left, curr);
      find(COMA, 0); // TODO: check this later
   }
   check((!found_error && last->type != RPAR), "expected ) after function declaration");
   check((!found_error && !find(DOTS, 0)), "Expected : after function declaration");

   Node *child = NULL;
   while (within_space(node->token->space)) child = add_child(node, expr());
   if (node->token->retType != VOID)
      check(!child || child->token->type != RETURN, "expected return statment");
   else
   {
      Node *ret = new_node(new_token(0, 0, 0, RETURN, node->token->space + TAB));
      ret->left = new_node(new_token(0, 0, 0, INT, node->token->space + TAB));
      add_child(node, ret);
   }
   exit_scoop();
   return node;
}

/******************************************
 *  Function call Layout:                 *
 *  ┌───────────────┐    ┌────────────┐   *
 *  │ children      │ -> │ Parameters │   *
 *  └───────────────┘    └────────────┘   *
 ******************************************/
Node *func_call(Node *node)
{
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

/******************************************
 *  Function main Layout:                 *
 *  ┌───────────────┐    ┌────────────┐   *
 *  │ children      │ -> │ Code bloc  │   *
 *  └───────────────┘    └────────────┘   *
 ******************************************/
Node *func_main(Node *node)
{
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
      last = new_node(new_token(NULL, 0, 0, RETURN, node->token->space + TAB));
      last->left = new_node(new_token(NULL, 0, 0, INT, node->token->space + TAB));
      add_child(node, last);
   }
   exit_scoop();
   return node;
}

/******************************************
 *  Struct def Layout:                    *
 *  ┌───────────────┐    ┌────────────┐   *
 *  │ children      │ -> │ attributes │   *
 *  └───────────────┘    └────────────┘   *
 ******************************************/
Node *struct_def(Node *node)
{
   Token *name;
   if (check(!(name = find(ID, 0)), "expected identifier after struct definition")) return NULL;
   if (check(!find(DOTS, 0), "expected dots after struct definition")) return NULL;
   setName(node->token, name->name);
   while (within_space(node->token->space))
   {
      Token *attr = find(LONG, INT, CHARS, CHAR, FLOAT, BOOL, SHORT, ID, 0);
      Token *id = find(ID, 0);
      if (check(!attr, "expected data type followed by id"))
      {
         ptoken(tokens[exe_pos]);
         break;
      }
      if (check(!id, "expected id after data type")) break;

      if (attr->type == ID) // attribute is a struct
      {
         Token *st = get_struct(attr->name);
         if (check(!st, "Unkown data type [%s]\n", attr->name)) exit(1);
         attr = st;
         char *name = id->name;
         id = attr;
         setName(id, name);
         id->type = STRUCT_CALL;
      }
      else id->type = attr->type;
      id->is_attr = true;
      add_attribute(node->token, id);
   }
   set_struct_size(node->token);
   new_struct(node->token);
   return node;
}

Node *symbol(Token *token)
{
   Token *struct_token = NULL;
   Node *node;
   if (token->type == ID && token->name && (struct_token =  is_struct(token)))
   {
      node = new_node(struct_token);
      token = find(ID, 0);
      if (check(!token, "Expected variable name after struct declaration\n"))
         debug(RED"found instead %k\n"RESET, tokens[exe_pos]);

      setName(node->token, token->name);
      node->token->type = STRUCT_CALL;
      node->token->is_data_type = true;
      return node;
   }
   else if (token->is_data_type)
   {
      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
      setName(token, tmp->name);
      return new_node(token);
   }
   else if (token->type == ID && token->name && find(LPAR, 0))
   {
      node = new_node(token);
      if (strcmp(token->name, "main") == 0) return func_main(node);
      return func_call(node);
   }
   return new_node(token);
}

/*************************************
 *  if Layout:                       *
 *  ┌───────────┐    ┌───────────┐   *
 *  │ left      │ -> │ condition │   *
 *  └───────────┘    └───────────┘   *
 *  ┌───────────┐    ┌───────────┐   *
 *  │ children  │ -> │ code bloc │   *
 *  └───────────┘    └───────────┘   *
 *  ┌───────────┐    ┌───────────┐   *
 *  │ right     │ -> │ elif/else │   *
 *  └───────────┘    └───────────┘   *
 *************************************/
Node *if_node(Node *node)
{
   enter_scoop(node);

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->is_cond = true;
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(NULL, 0, 0, CHILDREN, node->token->space));

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
         curr->left = expr();
         curr->left->token->is_cond = true;
         check(!find(DOTS, 0), "expected : after elif condition");
         while (within_space(token->space)) add_child(curr, expr());
      }
      else if (token->type == ELSE)
      {
         check(!find(DOTS, 0), "expected : after else");
         while (within_space(token->space)) add_child(curr, expr());
         break;
      }
   }
   exit_scoop();
   return node;
}

/*************************************
 *  while Layout:                    *
 *  ┌───────────┐    ┌───────────┐   *
 *  │ left      │ -> │ condition │   *
 *  └───────────┘    └───────────┘   *
 *  ┌───────────┐    ┌───────────┐   *
 *  │ children  │ -> │ code bloc │   *
 *  └───────────┘    └───────────┘   *
 *************************************/
Node *while_node(Node *node)
{
   enter_scoop(node);
   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->is_cond = true;
   node->left->token->space = node->token->space;

   check(!find(DOTS, 0), "Expected : after while condition\n", "");
   while (within_space(node->token->space))
   {
      Token *token = find(CONTINUE, BREAK, 0);
      Node *child = NULL;
      if (token) child = new_node(token);
      else child = expr();
      add_child(node, child);
   }
   exit_scoop();
   return node;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(STRUCT_DEF))) return struct_def(new_node(token));
   else if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, 0))) return symbol(token);
   else if ((token = find(REF, 0)))
   {
      node = prime(); // TODO: check it
      check(!node->token->is_data_type, "must be variable declaration after ref");
      node->token->is_ref = true;
      return node;
   }
   else if ((token = find(FDEC, 0))) return func_dec(new_node(token));
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
   else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
   else if ((token = find(LPAR, 0)))
   {
      if (tokens[exe_pos]->type != RPAR) node = expr();
      check(!find(RPAR, 0), "expected right par");
      return node;
   }
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}

void generate_ast()
{
   if (found_error) return;
   global = new_node(new_token(".global", 0, strlen(".global"), ID, 0));
   enter_scoop(global);
#if DEBUG
   debug(GREEN BOLD"AST:\n" RESET);
#endif
   head = new_node(new_token(NULL, 0, 0, TMP, 0));
   head->token->space = -TAB;
#if BUILTINS
   // add_builtins();
#endif
   config();
   while (tokens[exe_pos]->type != END && !found_error)
   {
      /*Node *child = */add_child(head, expr());
   }
   print_ast();
}

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
// IR Generation - Produce intermediate representation
// Optimization - Apply optimization passes

Token *if_ir(Node *node)
{
   enter_scoop(node);

   Inst *inst = new_inst(node->token);
   setName(inst->token, "if");
   inst->token->type =  BLOC;
   inst->token->index = ++bloc_index;

   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   setName(cond, "endif");
   cond->index = inst->token->index;
   --bloc_index;

   Token *lastInst = cond;
   // code bloc
   for (int i = 0; i < node->cpos; i++) generate_ir(node->children[i]);

   Inst *endInst = NULL;
   if (node->right->cpos)
   {
      endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space + TAB));
      endInst->token->index = node->token->index;
   }
   Node *subs = node->right;
   for (int i = 0; i < subs->cpos; i++)
   {
      Node *curr = subs->children[i];
      if (curr->token->type == ELIF)
      {
         curr->token->index = ++bloc_index;
         curr->token->type = BLOC;
         setName(curr->token, "elif");
         char *name = strdup(lastInst->name);
         int index = lastInst->index;
         {
            setName(lastInst, "elif");
            lastInst->index = curr->token->index;
            lastInst = copy_token(lastInst);
         }
         new_inst(curr->token); // elif bloc
         setName(curr->left->token, name);
         free(name);
         generate_ir(curr->left); // elif condition, TODO: check is boolean
         --bloc_index;
         curr->left->token->index = index;
         lastInst = curr->left->token;
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space + TAB));
         endInst->token->index = node->token->index;
      }
      else if (curr->token->type == ELSE)
      {
         curr->token->index = ++bloc_index;
         curr->token->type = BLOC;
         setName(curr->token, "else");
         new_inst(curr->token);
         {
            setName(lastInst, "else");
            lastInst->index = curr->token->index;
            lastInst = copy_token(lastInst);
         }
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         break;
      }

   }
   Token *new = new_token("endif", 0, 5, BLOC, node->token->space);
   new->index = node->token->index;
   new_inst(new);
   exit_scoop();
   return node->left->token;
}

Token *while_ir(Node *node)
{
   enter_scoop(node);
   node->token->type = BLOC;
   setName(node->token, "while");
   node->token->index = ++bloc_index;
   Inst *inst = new_inst(node->token);

   setName(node->left->token, "endwhile");
   generate_ir(node->left); // TODO: check if it's boolean
   node->left->token->index = node->token->index;

   for (int i = 0; i < node->cpos; i++) // while code bloc
   {
      Node *curr = node->children[i];
      Token *lastInst;
      switch (curr->token->type)
      {
      case BREAK:
         lastInst = copy_token(node->left->token);
         lastInst->type = JMP;
         setName(lastInst, "endwhile");
         new_inst(lastInst); // jmp back to while loop
         break;
      case CONTINUE:
         lastInst = copy_token(node->token);
         lastInst->type = JMP;
         setName(lastInst, "while");
         new_inst(lastInst); // jmp back to while loop
         break;
      default:
         generate_ir(curr); break;
      }
   }

   Token *lastInst = copy_token(node->token);
   lastInst->type = JMP;
   setName(lastInst, "while");
   lastInst->space += TAB;
   new_inst(lastInst); // jmp back to while loop

   lastInst = copy_token(node->token);
   lastInst->type = BLOC;
   setName(lastInst, "endwhile");
   new_inst(lastInst); // end while bloc
   exit_scoop();
   return inst->token;
}

Token *inialize_struct(Node *node)
{
   if (!node->token->is_attr) new_variable(node->token);
   for (int i = 0; i < node->token->Struct.pos; i++)
   {
      Token *attr = node->token->Struct.attrs[i];
      if (attr->type == STRUCT_CALL)
      {
         Node *tmp = new_node(attr);
         // attr->is_attr = true;
         attr->offset += node->token->offset;
         inialize_struct(tmp);
         free_node(tmp);
      }
      else
      {
         attr->ptr = ptr + node->token->offset - attr->offset;
         Node *tmp = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
         tmp->token->ir_reg = attr->ir_reg;
         tmp->left = new_node(attr);
         tmp->right = new_node(new_token(NULL, 0, 0, DEFAULT, attr->space));
         to_default(tmp->right->token, tmp->left->token->type);
         generate_ir(tmp);
         free_node(tmp);
      }
   }
   if (!node->token->is_attr) inc_ptr(node->token->offset);
   return node->token;
}

Token* inialize_variable(Node *node, Token *src)
{
   node->token->is_data_type = false;
   new_variable(node->token);
   Node *tmp = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
   tmp->token->ir_reg = node->token->ir_reg;
   tmp->left = copy_node(node);
   tmp->right = new_node(src);
   to_default(src, tmp->left->token->type);
   generate_ir(tmp);
   free_node(tmp);
   return node->token;
}

void set_func_dec_regs(Token *child, int *ptr)
{
   Token *src = new_token(NULL, 0, 0, DEFAULT, child->space);
   int r = *ptr;
   if (r < (int)(sizeof(eregs) / sizeof(eregs[0])))
   {
      if (child->is_ref)
      {
         setReg(src, rregs[r]);
      }
      else
      {
         // TODO: add other data type and math operations
         switch (child->type)
         {
         case CHARS: setReg(src, rregs[r]); break;
         case INT:   setReg(src, eregs[r]); break;
         case CHAR:  setReg(src, eregs[r]); break;
         case FLOAT: setReg(src, rregs[r]); break; // TODO: to be checked
         case BOOL:  setReg(src, eregs[r]); break;
         case STRUCT_CALL:
         {
            if (!child->is_attr) new_variable(child);
            for (int j = 0; j < child->Struct.pos; j++)
            {
               set_func_dec_regs(child->Struct.attrs[j],  &r);
               debug(RED"%k\n"RESET, child->Struct.attrs[j]);
            }
            break;
         }
         default: todo(1, "set ir_reg for %s", to_string(child->type));
         };
      }
      r++;
   }
   else
   {
      // TODO:
      todo(1, "implement assigning function argument using PTR");
   }
   if (child->is_ref /*&& !child->token->has_ref*/)
   {
      src->is_ref = true;
      src->has_ref = true;
   }
   if (child->type != STRUCT_CALL || child->is_ref)
   {
      if (src->is_ref) child->has_ref = true;
      Node *child_node = new_node(child);
      inialize_variable(child_node, src);
      free_node(child_node);
   }
   *ptr = r;
}

Token *func_dec_ir(Node *node)
{
   new_function(node);
   enter_scoop(node);
   int tmp_ptr = ptr;
   ptr = 0;
   Inst* inst = new_inst(node->token);

   // parameters
   Node *curr = node->left;
   for (int i = 0, r = 0; curr && i < curr->cpos && !found_error; i++)
   {
      Node *child = curr->children[i];
      set_func_dec_regs(child->token, &r);
   }

   // code bloc
   for (int i = 0; i < node->cpos; i++)
   {
      Node *child = node->children[i];
      generate_ir(child);
   }

   // TODO: if RETURN not found add it
   Token *new = new_token(NULL, 0, 0, END_BLOC, node->token->space);
   new->name = strdup(node->token->name);
   new_inst(new);
   node->token->ptr = ptr;
   ptr = tmp_ptr;
   exit_scoop();
   return inst->token;
}

void set_func_call_regs(int *ptr, Token *src, Token *dist, Node *node)
{
   int r = *ptr;
   if (r < (int)(sizeof(eregs) / sizeof(eregs[0])))
   {
      // added because unfction declaration params do have ptrs
      dist->ptr = 0;
      if (dist->is_ref) setReg(dist, rregs[r]);
      else
      {
         // TODO: add other data type and math operations
         switch (dist->type)
         {
         case CHARS: setReg(dist, rregs[r]); break;
         case INT:   setReg(dist, eregs[r]); break;
         case CHAR:  setReg(dist, eregs[r]); break;
         case FLOAT: setReg(dist, rregs[r]); break; // TODO: to be checked
         case BOOL:  setReg(dist, eregs[r]); break;
         case STRUCT_CALL:
         {
            for (int j = 0; j < dist->Struct.pos; j++)
            {
               set_func_call_regs(&r, src->Struct.attrs[j], dist->Struct.attrs[j], node);
               // debug(RED"%k\n"RESET, child->Struct.attrs[j]);
            }
            break;
         }
         default: todo(1, "set ir_reg for %s", to_string(dist->type));
         };
      }
      r++;
   }
   else
   {
      todo(1, "implement assigning function argument using PTR");
   }
   if (dist->is_ref)
   {
      dist->is_ref = true;
      dist->has_ref = false;
   }
   if (dist->type != STRUCT_CALL)
   {
      new_inst(dist);
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
      assign->left = new_node(dist);
      assign->right = new_node(src);
      assign->token->ir_reg = dist->ir_reg;

      debug(RED);
      pnode(assign, NULL, 0);
      debug(RESET);

      generate_ir(assign);
      free_node(assign);
      *ptr = r;
   }
}

Token *func_call_ir(Node *node)
{
   if (strcmp(node->token->name, "output") == 0)
   {
      setReg(node->token, "eax");
      setName(node->token, "printf");
      Node *fcall = node;

      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
      Token *_register = new_token(0, 0, 0, CHARS, fcall->token->space + TAB);
      _register->creg = strdup("rdi");
      Token *varg = new_token("\"", 0, 1, CHARS, fcall->token->space + TAB);

      new_inst(_register);
      assign->left = new_node(_register);
      assign->right = new_node(varg);
      assign->token->ir_reg = _register->ir_reg;

      generate_ir(assign);
      free_node(assign);

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i];
         Token *var = generate_ir(carg);
         if (check(var->type == ID, "Indeclared variable %s", carg->token->name)) break;
         Token *src = new_token(NULL, 0, 0, INT, var->space);

         int j = i + 1;
         if (j < (int)(sizeof(eregs) / sizeof(eregs[0])))
         {
            // added because unfction declaration params do have ptrs
            // TODO: add other data type and math operations
            src->ptr = 0;
            switch (var->type)
            {
            case CHARS:
            {
               setReg(src, rregs[j]);
               char *tmp = strjoin(varg->Chars.value, "%s", NULL);
               free(varg->Chars.value);
               varg->Chars.value = tmp;
               break;
            }
            case INT:
            {
               setReg(src, eregs[j]);
               char *tmp = strjoin(varg->Chars.value, "%d", NULL);
               free(varg->Chars.value);
               varg->Chars.value = tmp;
               break;
            }
            // case CHAR:  setReg(src, eregs[j]); break;
            // case FLOAT: setReg(src, rregs[j]); break; // TODO: to be checked
            // case BOOL:  setReg(src, eregs[j]); break;
            default: todo(1, "set ir_reg for %s", to_string(src->type));
            };
         }
         else
         {
            todo(1, "implement assigning function argument using PTR");
         }
         new_inst(src);
         assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
         assign->left = new_node(src);
         assign->right = new_node(var);
         assign->token->ir_reg = src->ir_reg;

         debug(RED);
         pnode(assign, NULL, 0);
         debug(RESET);

         generate_ir(assign);
         free_node(assign);
      }
      char *tmp = strjoin(varg->Chars.value, "\"", NULL);
      free(varg->Chars.value);
      varg->Chars.value = tmp;
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      func = copy_node(func);
      node->token->retType = func->token->retType;
      setReg(node->token, func->token->creg);
      Node *fdec = func->left;
      Node *fcall = node;

      for (int i = 0, r = 0; !found_error && i < fcall->cpos && i < fdec->cpos; i++)
      {
         Node *darg = fdec->children[i];
         Node *carg = fcall->children[i]; // will always be ID

         Token *src = generate_ir(carg);

         if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
         Token *dist = copy_token(darg->token);
         set_func_call_regs(&r, src, dist, node);
      }
      free_node(func);
   }
   Inst* inst = new_inst(node->token);
   return inst->token;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   if (!right || !right) return NULL;

   check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
         to_string(node->token->type), to_string(left->type), to_string(right->type));

   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   switch (node->token->type)
   {
   case ASSIGN:
   {
      if (left->type == STRUCT_CALL)
      {
         debug(">> %k\n", left);
         debug(">> %k\n", right);
         //    stop(1, "found");
      }
      node->token->ir_reg = left->ir_reg;
      if (!node->token->ir_reg || !left->ir_reg)
      {
         pnode(node, NULL, 0);
         debug(">> %k\n", left);
         debug(">> %k\n", right);
         //todo(1, "tmp condition");
      }
      node->token->retType = getRetType(node);
      if (left->is_ref) // ir_reg, ptr
      {
         if (right->is_ref) // ir_reg, ptr
         {
            if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
            if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
            else node->token->assign_type = REF_HOLD_REF;
         }
         else if (right->ptr) // ptr
         {
            if (left->has_ref) node->token->assign_type = REF_ID;
            else node->token->assign_type = REF_HOLD_ID;
         }
         else // ir_reg, value
         {
            if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
            node->token->assign_type = REF_VAL;
         }
         left->has_ref = true;
      }
      else if (left->ptr || left->creg) // ir_reg, ptr
      {
         if (right->is_ref) // ir_reg, ptr
         {
            if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
            node->token->assign_type = ID_REF;
         }
         else if (right->ptr) // ptr
            node->token->assign_type = ID_ID;
         else // ir_reg, value
            node->token->assign_type = ID_VAL;
      }
      else
      {
         pnode(node, NULL, 0);
         debug("<%k>\n", left);
         debug("<%k>\n", right);
         todo(1, "Invalid assignment");
      }
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case ADD_ASSIGN:
   {
      // TODO: to be checked
      node->token->retType = getRetType(node);
      if (node->token->retType  == INT) setReg(node->token, "eax");
      else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
      else check(1, "handle this case");
      break;
   }
   case NOT_EQUAL: case EQUAL: case LESS:
   case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      node->token->retType = BOOL;
      node->token->index = ++bloc_index;
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   return node->token;
}
Token *generate_ir(Node *node)
{
   if (found_error) return NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find) return find;
      return node->token;
      break;
   }
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS:
   {
      if (node->token->is_data_type)
      {
         if (node->token->type == STRUCT_CALL) return inialize_struct(node);
         return inialize_variable(node, new_token(NULL, 0, 0, DEFAULT, node->token->space));
      }
      return node->token;
      break;
   }
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      // check if right is DEFAULT, then initlize it, and return left
      return op_ir(node);
      break;
   }
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FDEC:  return func_dec_ir(node);
   case FCALL: return func_call_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      new_inst(node->token)->left = left;
      return node->token;
      break;
   }
   case BREAK:
   {
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            Token *token = copy_token(node->token);
            token->type = JMP;
            token->index = scoop->token->index;
            setName(token, "endwhile");
            return new_inst(token)->token;
            break;
         }
      }
      // TODO: invalid syntax, break should be inside whie loop
      break;
   }
   case CONTINUE:
   {
      for (int i = scoopPos; i >= 0; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            Token *token = copy_token(node->token);
            token->type = JMP;
            token->index = scoop->token->index;
            setName(token, "while");
            return new_inst(token)->token;
            break;
         }
      }
      // TODO: invalid syntax, break should be inside whie loop
      break;
   }
   case DOT:
   {
      Token *left = generate_ir(node->left);
      Token *right = node->right->token;
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;

      debug(RED SPLIT RESET);
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         debug("%k\n", attr);
         char *to_find = strjoin(left->name, ".", right->name);
         if (strcmp(to_find, attr->name) == 0)
         {
            free(to_find);
            return attr;
         }
         free(to_find);
      }
      debug(RED SPLIT RESET);

      todo(1, "%s has no attribute %s", left->name, right->name);
      return right;
      break;
   }
   case STRUCT_DEF:
   {
      return node->token;
   }
   case DEFAULT:
   {
      todo(1, "who is looking for default ?");
      return node->token;
   }
   default:
   {
      check(1, "handle this case %s", to_string(node->token->type));
      return node->token;
   }
   }
   return NULL;
}

// Assembly - Generate machine code
void generate(char *name)
{
   if (found_error) return;
#if IR
   debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   for (int i = 0; !found_error && i < head->cpos; i++) generate_ir(head->children[i]);
   if (found_error) return;
   print_ir();
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
   copy_insts();
   debug(GREEN BOLD"GENERATE ASSEMBLY CODE:\n" RESET);
   generate_asm(name);
#endif
}

int main(int argc, char **argv)
{
   check(argc < 2, "Invalid arguments");
   open_file(argv[1]);
   tokenize();

   generate_ast();
   generate(argv[1]);
   free_memory();
}