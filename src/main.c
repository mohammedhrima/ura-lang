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

char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
// Tokenization - Convert source text to token stream
// Syntax Analysis - Build abstract syntax tree from tokens
void tokenize()
{
   if (found_error) return;
   debug(GREEN BOLD"TOKENIZE:\n" RESET);
   struct { char *value; Type type; } specials[] = {
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"is", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
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
      // TODO: handle new lines inside comment
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
      if (isalpha(input[i]))
      {
         while (isalnum(input[i])) i++;
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

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = logic();
      // node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *logic()
{
   Node *left = equality();
   Token *token;
   while ((token = find(AND, OR, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = equality();
      left = node;
   }
   return left;
}

// TODO: handle NOT operator
Node *equality()
{
   Node *left = comparison();
   Token *token;
   while ((token = find(EQUAL, NOT_EQUAL, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = comparison();
      left = node;
   }
   return left;
}

Node *comparison()
{
   Node *left = add_sub();
   Token *token;
   while ((token = find(LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = add_sub();
      left = node;
   }
   return left;
}

Node *add_sub()
{
   Node *left = mul_div();
   Token *token;
   while ((token = find(ADD, SUB, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = mul_div();
      // node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *mul_div()
{
   Node *left = dot();
   Token *token;
   while ((token = find(MUL, DIV, 0))) // TODO: handle modulo
   {
      Node *node = new_node(token);
      node->left = left;
      node->right = dot();
      // node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *dot()
{
   Node *left = brackets();
   Token *token;
   while ((token = find(DOT, 0)))
   {
      /*
      TODO: expect left to be an struct call
      and iterate over it here
      handle the dot here
      */
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

Node *sign()
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
   if (typeName->type == ID) typeName = get_struct(typeName->name);
   Token *fname = find(ID, 0);
   Token *arg = NULL;
   if (check(!typeName || !fname, "expected data type and identifier after func declaration"))
      return node;

   node->token->retType = typeName->type;
   setName(node->token, fname->name);
   enter_scoop(node);

   switch (node->token->retType)
   {
   case INT:   setReg(node->token, "eax"); break;
   case CHAR:  setReg(node->token, "al"); break;
   case CHARS: setReg(node->token, "rax"); break;
   case VOID:  setReg(node->token, "rax"); break;
   default: todo(1, "this case %s", to_string(node->token->retType));
   }

   check(!find(LPAR, 0), "expected ( after function declaration");
   node->left = new_node(new_token(0, 0, 0, CHILDREN, node->token->space));
   Node *args = node->left;
   Token *last;
   int i = 0;
   while (!found_error && !(last = find(RPAR, END, 0)))
   {
      bool is_ref = find(REF, 0) != NULL;
      arg = find(INT, CHARS, CHAR, FLOAT, BOOL, 0);
      if (check(!arg, "expected data type in function argument")) break;
      Token *name = find(ID, 0);
      if (check(!name, "expected identifier in function argument")) break;
      name->is_ref = is_ref;
      name->type = arg->type;

      if (i < (int)(sizeof(eregs) / sizeof(eregs[0])))
      {
         if (name->is_ref) setReg(name, rregs[i]);
         else
         {
            // TODO: add other data type and math operations
            switch (name->type)
            {
            case CHARS: setReg(name, rregs[i]); break;
            case INT:   setReg(name, eregs[i]); break;
            case CHAR:  setReg(name, eregs[i]); break;
            case FLOAT: setReg(name, rregs[i]); break; // TODO: to be checked
            case BOOL:  setReg(name, eregs[i]); break;
            default: todo(1, "set reg for %s", to_string(name->type));
            };
            i++;
         }
      }
      else
      {
         // TODO:
         todo(1, "implement assigning function argument using PTR");
      }
      add_child(args, new_node(name));
      find(COMA, 0); // TODO: check this later
   }
   check((!found_error && last->type != RPAR), "expected ) after function declaration");
   check((!found_error && !find(DOTS, 0)), "Expected : after function declaration");

   for (int i = 0; i < args->cpos; i++)
   {
      // I used assign here because I need to declare variables
      // inside function declaration
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space + TAB));
      Node *left = args->children[i];
      Node *right = copy_node(args->children[i]);
      left->token->declare = true;

      // if (right->token->is_ref)
      // {
      //    right->token->has_ref = true;
      //    right->token->ptr = 0;
      // }
      setReg(left->token, NULL);
      setName(right->token, NULL);

      assign->left = left;
      assign->right = right;
      args->children[i] = assign;
   }
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

/*
struct User:
   chars name
   int id

main():
   User user
   user.name = "mohammed"
   int a = strlen(user.name)
   putnbr(a)
*/

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
      // I used assign here because I need to assign register
      // before function call
      // TODO: this approach need to be modified
      // doesn't work well if function takes references
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space));
      assign->right = expr();
      assign->right->token->space = token->space;
      assign->left = new_node(new_token(NULL, 0, 0, TMP, token->space));
      add_child(node, assign);
      assign->token->space = node->token->space;
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
      Token *attr = find(INT, CHARS, CHAR, FLOAT, BOOL, ID, 0);
      Token *id = find(ID, 0);
      if (check(!attr, "expected data type followed by id")) break;
      if (check(!id, "expected id after data type")) break;

      if (attr->type == ID) // attribute is a struct
      {
         attr = get_struct(attr->name);
         if (check(!attr, "")) exit(1);
         attr = copy_token(attr);
         char *name = id->name;
         id = attr;
         setName(id, name);
         id->type = STRUCT_CALL;
      }
      else
         id->type = attr->type;
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
      //struct_token = copy_token(struct_token);
      node = new_node(struct_token);
      if (!node->token->Struct.id)
      {
         check(1, "expected struct id\n");
         exit(1);
      }
      token = find(ID, 0);
      (check(!token, "Expected variable name after struct declaration\n"));
      setName(node->token, token->name);
      node->token->type = STRUCT_CALL;
      node->token->declare = true;
      return node;
   }
   else if (token->declare)
   {
      Token *tmp = find(ID, 0);
      check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
      setName(token, tmp->name);
      // new_variable(token);
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
   while (within_space(node->token->space))
   {
      add_child(node, expr());
   }
   while (!found_error && includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within_space(node->token->space - TAB))
   {
      Token *token = find(ELSE, ELIF, 0);
      Node *curr = add_child(node->right, new_node(token));
      token->space -= TAB;
      if (token->type == ELIF)
      {
         curr->left = expr();
         curr->left->token->is_cond = true;
         check(!find(DOTS, 0), "expected : after elif condition");
         while (within_space(token->space))
         {
            add_child(curr, expr());
         }
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
   node->left = expr(); // TODO: expect to be boolean
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
      check(!node->token->declare, "must be variable declaration after ref");
      node->token->is_ref = true;
      return node;
   }
   else if ((token = find(FDEC, 0))) return func_dec(new_node(token));
   else if ((token = find(RETURN, 0)))
   {
      // TODO: check if return type is compatible with function
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

   debug(GREEN BOLD"AST:\n" RESET);
   head = new_node(new_token(NULL, 0, 0, TMP, 0));
   head->token->space = -TAB;
#if BUILTINS
   add_builtins();
#endif
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

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
         to_string(node->token->type), to_string(left->type), to_string(right->type));

   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   switch (node->token->type)
   {
   case ASSIGN:
   {
      node->token->reg = left->reg;
      node->token->retType = getRetType(node);
      if (left->is_ref) // reg, ptr
      {
         if (right->is_ref) // reg, ptr
         {
            if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
            if (left->has_ref) node->token->assign_type = REF_REF;
            else node->token->assign_type = REF_HOLD_REF;
         }
         else if (right->ptr) // ptr
         {
            if (left->has_ref) node->token->assign_type = REF_ID;
            else node->token->assign_type = REF_HOLD_ID;
         }
         else // reg, value
         {
            if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
            node->token->assign_type = REF_VAL;
         }
         left->has_ref = true;
      }
      else if (left->ptr || left->creg) // reg, ptr
      {
         if (right->is_ref) // reg, ptr
         {
            if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
            node->token->assign_type = ID_REF;
         }
         else if (right->ptr) // ptr
         {
            node->token->assign_type = ID_ID;
         }
         else // reg, value
         {
            node->token->assign_type = ID_VAL;
         }
      }
      else
      {
         todo(1, "Invalid assignment");
      }
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case ADD_ASSIGN:
   {
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
   return inst->token;
}

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
   // free_token(lastInst);
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
   new_inst(lastInst); // jmp back to while loop

   lastInst = copy_token(node->token);
   lastInst->type = BLOC;
   setName(lastInst, "endwhile");
   new_inst(lastInst); // end while bloc
   exit_scoop();
   return inst->token;
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
   for (int i = 0; curr && i < curr->cpos && !found_error; i++)
   {
      Node *child = curr->children[i];
      // if (child->left->token->is_ref)
      // {
      //    Node *tmp = copy_node(child);
      //    tmp->left->token->has_ref = true;
      //    generate_ir(tmp);
      //    free_node(tmp);
      // }
      // else
      generate_ir(child);
   }
   //pnode(node, NULL, 0);

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

Token *func_call_ir(Node *node)
{
   if (strcmp(node->token->name, "output") == 0)
   {
      todo(1, "handle this case"); // TODO
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      func = copy_node(func);
      node->token->retType = func->token->retType;
      setReg(node->token, func->token->creg);
      Node *dec = func->left;
      Node *call = node;
#if 0
      pnode(func, "dec", 0);
      pnode(call, "call", 0);
      // todo("hey", "");
#endif
      for (int i = 0; !found_error && i < call->cpos && i < dec->cpos; i++)
      {
         Node *carg = call->children[i];
         Node *darg = dec->children[i];

         generate_ir(carg->right); // get right element of assign

         debug(RED SPLIT RESET);
         pnode(carg, NULL, 0);
         debug(RED SPLIT RESET);

         if (check(carg->right->token->type == ID, "Indeclared variable %s", carg->right->token->name)) break;
         carg->left->token->type = carg->right->token->type;
         //TODO: check if they arr compatible carg->left->token->type != darg->token->type;
         if (darg->token->is_ref)
         {
            todo(1, "check dot here", "");
            generate_ir(carg);
            // carg->left->token->is_ref = true;
            // carg->left->token->has_ref = 0;
            // carg->left->token->ptr = 0;
            // carg->token->ptr = 0;
         }
         else
         {
            if (eregs[i] && !found_error)
            {
               Type type = carg->left->token->type;
               Type retType = carg->left->token->retType;
               switch (type)
               {
               case CHARS: setReg(carg->left->token, rregs[i]); break;
               case ADD: case SUB: case MUL: case DIV:
               case INT: setReg(carg->left->token, eregs[i]); break;
               default: todo(1, "handle this case [%s] [%s]", to_string(type), to_string(retType));
               }
            }
            else if (!found_error)
            {
               todo(1, "implemnt PTR");
            }
            generate_ir(carg);
         }

      }
      free_node(func);
   }
   Inst* inst = new_inst(node->token);
   return inst->token;
}

Token* generate_ir(Node *node)
{
   if (found_error) return NULL;
   Inst *inst = NULL;
   switch (node->token->type)
   {
   case ID:
   {
      // if (node->token->isattr) return node->token;
      Token *find = get_variable(node->token->name);
      if (find)
      {
         node->token = copy_token(find);
         node->token->declare = false;
      }
      // debug(GREEN "after getting variable %k \n", token);
      return node->token;
   }
   case INT: case BOOL: case CHAR: case FLOAT: case LONG: case CHARS:
   {
      if (node->token->declare) new_variable(node->token);
      inst = new_inst(node->token);
      break;
   }
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
      return op_ir(node);
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FDEC:  return func_dec_ir(node);
   case FCALL: return func_call_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
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
            inst = new_inst(token);
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
            inst = new_inst(token);
            break;
         }
      }
      // TODO: invalid syntax, break should be inside whie loop
      break;
   }
   case STRUCT_CALL:
   {
      new_variable(node->token);
      new_inst(node->token);
      return node->token;
      break;
   }
   case DOT:
   {
      debug(CYAN"handle dot: %n\n"RESET, node);
      if (check(!node->left, "Error", "")) exit(1);
      if (check(!node->right, "Error", "")) exit(1);

      Token *left =  generate_ir(node->left);
      Token *right =  node->right->token;
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         // debug("%k\n", attr);
         char *to_find = strjoin(left->name, ".", right->name);
         if (strcmp(to_find, attr->name) == 0)
         {
            free(to_find);
            return attr;
         }
         free(to_find);
      }

      check(1, "%s has no attribute %s", left->name, right->name);
      return right;
      break;
   }
   case LBRA:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      debug("access %k in %k\n", right, left);
      inst = new_inst(node->token);
      break;
   }
   case STRUCT_DEF: return node->token;
   default: check(1, "handle this case %s", to_string(node->token->type)); return NULL;
   }
   return inst->token;
}

// Assembly - Generate machine code
bool did_pasm;
void asm_space(int space)
{
   if (did_pasm)
   {
      space = (space / TAB) * 4;
      pasm("\n");
      for (int i = 0; i < space; i++) pasm(" ");
      did_pasm = false;
   }
}

void generate_asm(char *name)
{
   if (found_error) return;
   char *outfile = strdup(name);
   outfile[strlen(outfile) - 1] = 's';
   asm_fd = fopen(outfile, "w+");
   check(!asm_fd, "openning %s\n", outfile);
   if (found_error) return;
   free(outfile);
   initialize();
   copy_insts();
   for (int i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      asm_space(curr->space);
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT:
      {
         if (curr->declare)
         {
            pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         }
         break;
      }
      case ADD_ASSIGN:
      {
         // TODO: check this
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); asm_space(curr->space);}
         break;
      }
      case ASSIGN:
      {
         // char *inst = left->type == FLOAT ? "movss " : "mov ";
         debug("LEFT : %k\n", left);
         debug("RIGHT: %k\n", right);
         switch (curr->assign_type)
         {
         case ID_VAL:
         {
            // todo(1, "handle this case");
            // left ptr, creg
            // right value, creg
            if (left->ptr)
            {
               if (right->creg) pasm("%i%a, %r", "mov", left, right); // left ptr, right creg
               else // left ptr, right value
               {
                  if (right->type == CHARS)
                  {
                     pasm("%i%ra, .STR%d[rip]", "lea", left, right->index); asm_space(curr->space);
                     pasm("%i%a, %ra", "mov", left, right);
                  }
                  else
                  {
                     pasm("%i%a, %v", "mov", left, right, left->name);
                  }
               }
            }
            else if (left->creg)
            {
               todo(1, "handle this case", "");
               if (right->creg) // left creg, right creg
               {
                  todo(1, "handle this case", "");
               }
               else // left creg, right value
               {
                  if (right->type == CHARS)
                  {
                     todo(1, "handle this case", "");
                  }
                  else
                  {
                     todo(1, "handle this case", "");
                  }
               }
            }
            else
            {
               todo(1, "handle this case", "");
            }
            break;
         }
         case ID_ID:
         {
            // left ptr, creg
            // right ptr
            if (left->ptr && right->ptr)
            {
               pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            // functions parameters
            else if (left->creg && right->ptr)
            {
               todo(1, "check this case", "");
               pasm("%i%a, %ra", "mov", left, right);
            }
            else
            {
               todo(1, "handle this case", "");
            }
            break;
         }
         case REF_HOLD_ID:
         {
            // left ptr, creg
            // right ptr
            if (left->ptr && right->ptr)
            {
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
            }
            else if (left->ptr && right->creg)
            {
               todo(1, "handle this case");
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               pasm("%irdi, rax", "mov");
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case ID_REF:
         {
            // left ptr, creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");

               }
               else if (right->creg)
               {
                  todo(1, "handle this case");

               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else if (left->creg)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");
               }
               else if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_ID:
         {
            // left ptr (has_refrence), creg
            // right ptr
            todo(1, "handle this case");
            if (left->ptr && right->ptr)
            {
               // pasm("%irax, %a", "mov", left); asm_space(curr->space);
               // pasm("%i%rd, %a", "mov", left, right); asm_space(curr->space);
               // pasm("%i%ma, %rd", "mov", left, right);
            }
            else if (left->creg && right->ptr)
            {

            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_VAL:
         {
            // left ptr (has_refrence) , creg
            // right creg, value
            if(left->ptr)
            {
               if(right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
                  pasm("%i%ma, %v", "mov", left,  right);
               }
            }
            else if(left->creg)
            {
               if(right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  todo(1, "handle this case");
               }
            }
            else
            {
               todo(1, "handle this case");
            }
            break;
         }
         case REF_REF:
         {
            // left ptr (has_refrence), creg
            // right ptr, creg
            todo(1, "handle this case");
            break;
         }
         case REF_HOLD_REF:
         {
            // left ptr, creg
            // right ptr, creg
            todo(1, "handle this case");
            break;
         }
         default:
         {
            todo(1, "handle this case");
            break;
         }
         }
         if (left->name) pasm(" ;// assign [%s]", left->name);
         else if (left->creg) pasm(" ;// assign [%s]", left->creg);
         if (left->is_ref) pasm(" is_ref");
         break;

#if 1
         if (left->ptr && left->is_ref && !left->has_ref)
         {
            left->has_ref = true;
            if (right->ptr)
            {
               //check(1, "found"); exit(1);
               // int a = 1 ref int b = a
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
               else pasm("%irdi, rax", "mov"); // is function argument
            }
            else if (right->creg) // I added for function arguments that are references
            {
               /*
                  func int m(ref int a):
                     a = 1
                     return 1
                  main():
                     int x = 2
                     m(x)
                     putnbr(x)
               */
               if (left->ptr) pasm("%iQWORD PTR -%d[rbp], %ra", "mov", left->ptr, right);
               else check(1, "handle this case");
            }
            else
            {
               check(1, "handle this case");
            }
         }
         else if (left->ptr && left->is_ref && left->has_ref)
         {
            if (right->ptr)
            {
               //check(1, "found"); exit(1);
               // int a = 1 int c = 2 ref int b = a b = c putnbr(a)
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%rd, %a", "mov", left, right); asm_space(curr->space);
               pasm("%i%ma, %rd", "mov", left, right);
            }
            else if (right->creg)
            {
               //check(1, "handle this case");
               // int a = 1 + 2 ref int b = a b = 2 + a putnbr(a)
               pasm("%i%rb, %ra", "mov", left, right); asm_space(curr->space);
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%ma, %rb", "mov", left, left);
            }
            // else if(right->type == CHARS)
            // {
            //    check(1, "found"); exit(1);
            // }
            else // right is value
            {
               // int a = 1 ref int b = a b = 3
               pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
               pasm("%i%ma, %v", "mov", left,  right);
               check(1, "found"); exit(1);
            }
         }
         else if (left->ptr && !left->is_ref)
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 + 2 ref int b = a int c = b putnbr(c)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 int b = a
               pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index); asm_space(curr->space);
               pasm("%i%a, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               // int a = 1
               pasm("%i%a, %v", "mov", left, right, left->name);
            }
         }
         else if (left->creg) // function parameter
         {
            if (right->is_ref)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a putnbr(a)
               pasm("%irax, %a", "mov", right); asm_space(curr->space);
               pasm("%i%ra, %ma", "mov", left, right);
            }
            else if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a b = 3 putnbr(a)
               pasm("%i%ra, %a", "mov", left, right); //asm_space(curr->space);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "found"); exit(1);
               // putnbr(1 + 2)
               pasm("%i%ra, %ra", "mov", left, right);
            }
            else if (right->type == CHARS)
            {
               // check(1, "found"); exit(1);
               // putstr("cond 1\n")
               pasm("%i%ra, .STR%d[rip]", "lea", left, right->index);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, %v", "mov", left, right, left->name);
            }
         }
         else
         {
            check(1, "handle this case");
            debug("left:%k, right:%k\n", left, right);
            // check(1, "handle this case left:%k, right:%k", left, right);
         }
         break;
#endif
      }
      case ADD: case SUB: case MUL: case DIV: // TODO: check all math_op operations
      {
         // TODO: use rax for long etc...
         // TODO: something wrong here, fix it
         // Type type = curr->type;
         char *inst = left->type == FLOAT ? "movss" : "mov";
         char *inst2 = NULL;
         switch (curr->type)
         {
         case ADD: inst2 = left->type == FLOAT ? "addss " : "add "; break;
         case SUB: inst2 = left->type == FLOAT ? "subss " : "sub "; break;
         case MUL: inst2 = left->type == FLOAT ? "imulss " : "imul "; break;
         case DIV: inst2 = left->type == FLOAT ? "divss " : "div "; break;
         default: break;
         }
         pasm("%i%ra, ", inst, left);
         if (left->ptr) pasm("%a", left);
         else if (left->creg) pasm("%ra", left) ;
         else pasm("%v", left);
         asm_space(curr->space);
         pasm("%i%ra, ", inst2, right);
         if (right->ptr) pasm("%a", right);
         else if (right->creg) pasm("%ra", right) ;
         else pasm("%v", right);
         // curr->type = getTypeleft->type;
         break;
      }
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case MORE_EQUAL:
      {
         if (curr->is_cond)
         {
            char *inst = NULL;
            switch (curr->type)
            {
            case EQUAL: inst = "jne"; break;
            case NOT_EQUAL: inst = "je"; break;
            case LESS: inst = "jge"; break;
            case LESS_EQUAL: inst = "jg"; break;
            case MORE: inst = "jle"; break;
            case MORE_EQUAL: inst = "jl"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space); pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%ra", left);
            else if (!left->creg) pasm("%v", left);

            // asm_space(curr->space);
            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %ra", right);
            else if (!right->creg) pasm(", %v", right);
            asm_space(curr->space); pasm("%i .%s%d", inst, curr->name ? curr->name : "(null)", curr->index);
         }
         else
         {
            char *inst = left->type == FLOAT ? "movss" : "mov";
            if (left->ptr) pasm("%i%ra, %a", inst, left, left);
            else if (left->creg /*&& strcmp(left->creg, r->creg)*/) pasm("%i%ra, %ra", inst, left, left);
            else if (!left->creg) pasm("%i%ra, %v", inst, left, left);

            char *reg = NULL;
            switch (left->type)
            {
            case INT: reg = "ebx"; break;
            case FLOAT: reg = "xmm1"; break;
            case CHAR: reg = "bl"; break;
            case BOOL: reg = "ebx"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            asm_space(curr->space);
            if (right->ptr) pasm("%i%s, %a", inst, reg, right);
            else if (right->creg) pasm("%i%s, %ra", inst, reg, right);
            else if (!right->creg) pasm("%i%s, %v", inst, reg, right);

            inst = left->type == FLOAT ? "ucomiss" : "cmp";
            asm_space(curr->space);
            pasm("%i%ra, %s", inst, left, reg);
            switch (curr->type)
            {
            case EQUAL: inst = "sete"; break;
            case NOT_EQUAL: inst = "setne"; break;
            case LESS: inst = "setl"; break;
            case LESS_EQUAL: inst = "setle"; break;
            case MORE: inst = "setg"; break;
            case MORE_EQUAL: inst = "setge"; break;
            default: check(1, "Unkown type [%s]\n", to_string(left->type)); break;
            }
            curr->retType = BOOL;
            setReg(curr, "al");
            asm_space(curr->space); pasm("%i%ra", inst, curr);
         }
         break;
      }
      case FDEC:
      {
         pasm("%s:", curr->name);
         asm_space(curr->space + TAB); pasm("%irbp", "push");
         asm_space(curr->space + TAB); pasm("%irbp, rsp", "mov");
         asm_space(curr->space + TAB); pasm("%irsp, %d", "sub", (((curr->ptr) + 15) / 16) * 16);
         break;
      }
      case RETURN:
      {
         /*
         TODO: handle reference return
         func int m(ref int a):
            a = 1
            return a
         */
         if (left->ptr) pasm("%i%ra, %a", "mov", left, left);
         else if (left->creg)
         {
            // TODO: check the type
            if (strcmp(left->creg, "eax")) pasm("%i%ra, %a", "mov", left, left);
         }
         else
         {
            switch (left->type)
            {
            case INT: pasm("%i%ra, %ld", "mov", left, left->Int.value); break;
            case VOID: pasm("%ieax, 0", "mov"); break;
            default: check(1, "handle this case [%s]\n", to_string(left->type)); break;
            }
         }
         asm_space(curr->space); pasm("%i", "leave");
         asm_space(curr->space); pasm("%i", "ret");
         break;
      }
      case JE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "je", curr->name, curr->index); break;
      case JNE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%d", "jne", curr->name, curr->index); break;
      case JMP: pasm("%i.%s%d", "jmp", curr->name, curr->index); break;
      case FCALL: pasm("%i%s", "call", curr->name); break;
      case BLOC: pasm(".%s%d:", (curr->name ? curr->name : "(null)"), curr->index); break;
      case END_BLOC: pasm(".end%s:", curr->name); break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
}

void generate(char *name)
{
   if (found_error) return;
#if IR
   debug(GREEN BOLD"GENERATE IR:\n" RESET);
   for (int i = 0; !found_error && i < head->cpos; i++) generate_ir(head->children[i]);
   if (found_error) return;
   print_ir();
#endif
#if OPTIMIZE
   debug(GREEN BOLD"OPTIMIZE IR:\n" RESET);
   copy_insts();
   while (OPTIMIZE && !found_error && optimize_ir()) copy_insts();
#endif
#if ASM
   debug(GREEN BOLD"GENERATE ASM:\n" RESET);
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