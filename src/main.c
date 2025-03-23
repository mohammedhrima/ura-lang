#include "./include/header.h"

// GLOBALS
bool found_error = false;
Inst **OrgInsts;
Inst **insts;
Token **tokens;
Node *head;
size_t exe_pos;
char *input;

// TOKENIZE
void tokenize()
{
   if (found_error) return;
   debug(GREEN BOLD"TOKENIZE:\n" RESET);
   Specials *specials = (Specials[]) {
      {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
      {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
      {"==", EQUAL}, {"is", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
      {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
      {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
      {",", COMA}, {"if", IF}, {"elif", ELIF}, {"else", ELSE},
      {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
      {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF},
      {"and", AND}, {"&&", AND}, {"or", OR}, {"||", OR}, {"struct", STRUCT_DEF},
      {0, (Type)0}
   };

   size_t space = 0;
   bool inc_space = true;
   for (size_t i = 0; input[i] && !found_error; )
   {
      size_t s = i;
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

// ABSTRACT SYNTAX TREE
Type getRetType(Node *node)
{
   if (!node || !node->token) return 0;
   if (includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0}, node->token->type)) return node->token->type;
   if (includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0}, node->token->retType)) return node->token->retType;
   Type left = 0, right = 0;
   if (node->left) left = getRetType(node->left);
   if (node->right) right = getRetType(node->right);
   if (left) return left;
   if (right) return right;
   return 0;
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
      node->token->retType = getRetType(node);
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
      node->token->retType = getRetType(node);
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
      node->token->retType = getRetType(node);
      left = node;
   }
   return left;
}

Node *dot()
{
   return sign();
}

Node *sign()
{
   return prime();
}

Node *func_dec(Node *node)
{
   Token *typeName = find(INT, CHARS, CHAR, FLOAT, BOOL, 0);
   Token *fname = find(ID, 0);
   check(!typeName || !fname, "expected data type and identifier after func declaration");

   node->token->retType = typeName->type;
   node->token->name = fname->name;
   fname->name = NULL;

   enter_scoop(node->token);
   debug("found FDEC with retType %s\n", to_string(node->token->retType));
   char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
   char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
   int i = 0;
   Token *arg = NULL;
   Token *token = node->token;

   node->left = new_node(NULL);
   Node *curr = node->left;

   check(!find(LPAR, 0), "expected ( after function declaration");
#if 0
   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space + 2 * TAB));
      assign->left = expr();
      check(!assign->left->token->declare, "Invalid argument for function declaration");
      if (assign->left->token->isref) assign->left->token->hasref = true;
      assign->left->token->space = token->space + 2 * TAB;
      assign->right = new_node(new_token(NULL, 0, 0, assign->left->token->type, token->space + 2 * TAB));
      if (eregs[i])
      {
         // TODO: add other data type and math operations
         switch (assign->left->token->type)
         {
         case CHARS: setReg(assign->right->token, rregs[i]); break;
         case INT:   setReg(assign->right->token, eregs[i]); break;
         default: check(1, "set reg for %s", to_string(assign->left->token->type));
         };
         i++;
      }
      else
      {
         // TODO:
         check(1, "implement assigning function argument using PTR");
      }
      curr->left = assign;
      find(COMA, 0);
      curr->right = new_node(NULL);
      curr = curr->right;
   }
#else
   while (!found_error && !includes((Type[]) {RPAR, END, 0}, tokens[exe_pos]->type))
   {
      Token *ref = find(REF, 0);
      arg = find(INT, CHARS, CHAR, FLOAT, BOOL, 0);
      if (check(!arg, "expected data type in function argument")) break;
      Token *name = find(ID, 0);
      if (check(!name, "expected identifier in function argument")) break;
      if (ref)
      {
         name->isref = true;
         // name->hasref = true;
      }
      name->type = arg->type;
      // name->declare = true;
      if (eregs[i])
      {
         if (name->isref)
         {
            setReg(name, rregs[i]);
         }
         else
         {
            // TODO: add other data type and math operations
            switch (name->type)
            {
            case CHARS: setReg(name, rregs[i]); break;
            case INT:   setReg(name, eregs[i]); break;
            default: check(1, "set reg for %s", to_string(name->type));
            };
         }
         i++;
      }
      else
      {
         // TODO:
         check(1, "implement assigning function argument using PTR");
      }
      curr->left = new_node(name);
      find(COMA, 0);
      curr->right = new_node(NULL);
      curr = curr->right;
   }
#endif
   check(!found_error && !find(RPAR, 0), "expected ) after function declaration");
   check(!found_error && !find(DOTS, 0), "Expected : after function declaration");

   curr = node;
   while (within_space(node->token->space))
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   exit_scoop();
   return node;
}

Node *func_call(Node *node)
{
   node->token->type = FCALL;
   Node *curr = node;
   Token *arg = NULL;
   Token *token = node->token;

   char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
   char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
   int i = 0;
   while (!found_error && !(arg = find(RPAR, END, 0)))
   {
      // TODO: this approach need to be modified
      // doesn't work well if function takes references
      Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, token->space));
      assign->right = expr();
      assign->right->token->space = token->space;
      assign->left = new_node(NULL);
      if (assign->right->token->type == ID)
      {
         Token *var = get_variable(assign->right->token->name);
         if (var)
         {
            assign->left->token = copy_token(var);
            setName(assign->left->token, NULL);
            setReg(assign->left->token, NULL);
            assign->left->token->space = token->space;
            assign->left->token->declare = false;
         }
      }
      else
      {
         assign->left->token = copy_token(assign->right->token);
         assign->left->token->space = token->space;
         assign->left->token->type = getRetType(assign->right);
      }
      check(assign->left->token->type == 0, "found another stupid error in function call");
      if (eregs[i])
      {
         if (!found_error)
         {
            Type type = assign->left->token->type;
            switch (type)
            {
            case CHARS: setReg(assign->left->token, rregs[i]); break;
            case ADD: case SUB: case MUL: case DIV:
            case INT: setReg(assign->left->token, eregs[i]); break;
            default: check(1, "handle this case [%s]", to_string(type));
            }
         }
         i++;
      }
      else
      {
         check(1, "implemnt PTR");
      }
      curr->left = assign;
      find(COMA, 0);
      curr->right = new_node(NULL);
      curr = curr->right;
   }
   check(!found_error && arg->type != RPAR, "expected ) after function call");
   return node;
}

Node *func_main(Node *node)
{
   check(!find(RPAR, 0), "expected ) after main declaration");
   check(!find(DOTS, 0), "expected : after main() declaration");

   enter_scoop(node->token);
   node->token->type = FDEC;
   node->token->retType = INT;
   Node *curr = node;
   Node *last = node;
   while (within_space(node->token->space))
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
      last = curr->left;
   }
   if (last && last->token->type != RETURN)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = new_node(new_token(NULL, 0, 0, RETURN, node->token->space + TAB));
      curr->left->left = new_node(new_token(NULL, 0, 0, INT, node->token->space + TAB));
   }
   exit_scoop();
   return node;
}

Node *if_node(Node *node)
{
   enter_scoop(node->token);
   debug(GREEN "Enter if" RESET);
   node->left = new_node(NULL);
   Node *curr = node->left; // if bloc

   curr->left = expr(); // condition
   curr->left->token->space = node->token->space;

   check(!find(DOTS, 0), "Expected : after if condition\n", "");

   curr->right = new_node(NULL);
   curr = curr->right;
   while (within_space(node->token->space))
   {
      curr->left = expr();
      // curr->left->token->space = node->token->space + TAB;
      curr->right = new_node(NULL);
      curr = curr->right;
   }
   curr = node;
   while (
   !found_error && includes((Type[]) {ELSE, ELIF, 0}, tokens[exe_pos]->type) &&
node->token->space == tokens[exe_pos]->space)
   {
      Token *token = find(ELSE, ELIF, 0);

      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = new_node(token);
      if (token->type == ELIF)
      {
         Node *elif_node = curr->left;
         elif_node->left = expr();
         elif_node->left->token->space = node->token->space;
         check(!find(DOTS, 0), "expected : after elif condition");
         elif_node->right = new_node(NULL);
         elif_node = elif_node->right;
         while (within_space(token->space))
         {
            elif_node->left = expr();
            elif_node->left->token->space = node->token->space + TAB;
            elif_node->right = new_node(NULL);
            elif_node = elif_node->right;
         }
      }
      else if (token->type == ELSE)
      {
         check(!find(DOTS, 0), "expected : after else");
         Node *else_node = curr->left;
         else_node->right = new_node(NULL);
         else_node = else_node->right;
         while (within_space(token->space))
         {
            else_node->left = expr();
            else_node->left->token->space = node->token->space + TAB;
            else_node->right = new_node(NULL);
            else_node = else_node->right;
         }
         break;
      }
   }
   exit_scoop();
   return node;
}

Node *while_node(Node *node)
{
   enter_scoop(node->token);
   node->left = expr();
   node->left->token->isCond = true;
   node->left->token->space = node->token->space;

   check(!find(DOTS, 0), "Expected : after while condition\n", "");
   Node *curr = node;
   while (within_space(node->token->space))
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      Token *token = find(CONTINUE, BREAK, 0);
      if (token) curr->left = new_node(token);
      else curr->left = expr();
      curr->left->token->space = node->token->space + TAB;
   }
   exit_scoop();
   return node;
}

Node *is_struct(Token *token)
{
   Node *struct_ = get_struct(token);
   if (struct_) return copy_node(struct_);
   return NULL;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(STRUCT_DEF)))
   {
      if (check(!(token = find(ID, 0)), "expected identifier after struct definition")) return NULL;
      if (check(!find(DOTS, 0), "expected dots after struct definition")) return NULL;
      node = new_node(token);
      node->token->type = STRUCT_DEF;
      node->right = new_node(NULL);
      Node *curr = node->right;
      while (within_space(token->space))
      {
         Token *attr = find(INT, CHARS, CHAR, FLOAT, BOOL, 0);
         Token *id = find(ID, 0);
         if (check(!attr || !id, "expected data type followed by id")) break;
         id->type = attr->type;
         // char *name = strjoin(token->name, ".", id->name);
         // setName(id, name);
         // free(name);
         curr->left = new_node(id);
         curr->right = new_node(NULL);
         curr = curr->right;
      }
      // define struct size
      new_struct(node);
      return node;
   }
   else if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, 0)))
   {
      Node *struct_node = NULL;
      if (token->type == ID && token->name && (struct_node =  is_struct(token)))
      {
         // token = tmp_node->token;
         token = find(ID, 0);
         check(!token, "Expected variable name after struct declaration\n");
         setName(struct_node->token, token->name);
         struct_node->token->declare = true;
         struct_node->token->type = STRUCT_CALL;
         Node *curr = struct_node->right;
         while(curr->left)
         {
            char *name = strjoin(token->name, ".", curr->left->token->name);
            setName(curr->left->token, name);
            free(name);
            curr = curr->right;
         }
         new_variable(struct_node->token);
         return struct_node;
      }
      else 
      if (token->declare)
      {
         Token *tmp = find(ID, 0);
         check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
         setName(token, tmp->name);
         new_variable(token);
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
   else if ((token = find(REF, 0)))
   {
      node = prime(); // TODO: check it
      check(!node->token->declare, "must be variable declaration after ref");
      node->token->isref = true;
      return node;
   }
   else if ((token = find(FDEC, 0)))
      return func_dec(new_node(token));
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
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
}

void generate_ast()
{
   if (found_error) return;
   enter_scoop(new_token(".global", 0, strlen(".global"), ID, 0));

   debug(BLUE BOLD"AST:\n" RESET);
   head = new_node(NULL);
   Node *curr = head;
   curr->left = expr();
   while (tokens[exe_pos]->type != END && !found_error)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   if (found_error) return;
   debug(BLUE BOLD"PRINT AST:\n" RESET);
   curr = head;
   while (curr && !found_error)
   {
      debug("%n\n", curr->left);
      curr = curr->right;
   }
   exit_scoop();
}

// INTERMEDIATE REPRESENTATION
bool optimize_ir()
{
   static int op = 0;
   static bool did_optimize = false;
   bool did_something = false;
   switch (op)
   {
   case 0:
   {
      debug(CYAN "OP[%d] calculate operations on values\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         Token *left = insts[i]->left;
         Token *right = insts[i]->right;

         Type types[] = {INT, FLOAT, 0};
         Type ops[] = {ADD, SUB, MUL, DIV, 0};
         // TODO: check if left and right are compatible
         // test if left is function, and right is number ...
         if (
            includes(ops, token->type) && includes(types, left->type) &&
            compatible(left, right) && !left->name && !right->name)
         {
            did_something = true;
            did_optimize = true;
            switch (left->type)
            {
            case INT:
               switch (token->type)
               {
               case ADD: token->Int.value = left->Int.value + right->Int.value; break;
               case SUB: token->Int.value = left->Int.value - right->Int.value; break;
               case MUL: token->Int.value = left->Int.value * right->Int.value; break;
               case DIV: token->Int.value = left->Int.value / right->Int.value; break;
               default: break;
               }
               break;
            default:
               check(1, "handle this case\n", "");
               break;
            }
            token->type = left->type;
            token->retType = 0;
            insts[i]->left = NULL;
            insts[i]->right = NULL;
            left->remove = true;
            right->remove = true;
            token->reg = 0;
            setReg(token, NULL);
            if (i > 0) i -= 2;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 1:
   {
      debug(CYAN "OP[%d] calculate operations on constants\n" RESET, op);
      for (int i = 1; insts && insts[i]; i++)
      {
         Token *curr_token = insts[i]->token;
         Token *curr_left = insts[i]->left;
         Token *curr_right = insts[i]->right;

         Token *prev_token = insts[i - 1]->token;
         // Token *prev_left = insts[i - 1]->left;
         Token *prev_right = insts[i - 1]->right;

         //  TODO: handle string also here X'D ma fiyach daba
         if (curr_token->type == ADD && prev_token->type == ADD)
         {
            if (curr_left == prev_token && !prev_right->name && !curr_right->name)
            {
               // prev_right->type = INT;
               curr_token->remove = true;
               prev_right->Int.value += curr_right->Int.value;
               i = 1;
               clone_insts();
               did_something = true;
               did_optimize = true;
               continue;
            }
            // else
            // if(curr_right == prev_token && !prev_left->name && !curr_left->name)
            // {
            //   // prev_r->type = INT;
            //   // curr_token->remove = true;
            //   prev_left->Int.value += curr_left->Int.value;
            //   i = 1;
            //   clone_insts();
            //   optimize = true;
            //   continue;
            // }
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 2:
   {
      debug(CYAN "OP[%d] remove reassigned variables\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         if (token->declare)
         {
            for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++) {
               if (insts[j]->token->type == ASSIGN && insts[j]->left->ptr == token->ptr /*&& !token->isref*/) {
                  token->declare = false;
                  token->remove = true;
                  did_optimize = true;
                  did_something = true;
                  insts[j]->left->isref = token->isref;
                  break;
               }
               if ((insts[j]->left && insts[j]->left->reg == token->reg) ||
                     (insts[j]->right && insts[j]->right->reg == token->reg)) {
                  break;
               }
            }
         }
         else if (token->type == ASSIGN)
         {
            for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++) {
               if (!insts[j]->left || !insts[j]->right)
                  continue;
               if (insts[j]->token->type == ASSIGN && insts[j]->left == insts[i]->left && !insts[i]->left->isref) {
                  token->remove = true;
                  did_optimize = true;
                  did_something = true;
                  break;
               }
               if (insts[j]->left->reg == token->reg || insts[j]->right->reg == token->reg)
                  break;
            }
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 3:
   {
      debug(CYAN"OP[%d] remove followed return instructions\n"RESET, op);
      for (int i = 1; insts[i]; i++)
      {
         if (insts[i]->token->type == RETURN && insts[i - 1]->token->type == RETURN)
         {
            did_optimize = true;
            did_something = true;
            insts[i]->token->remove = true;
            clone_insts();
            i = 1;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 4:
   {
      // TODO: be carefull this one remove anything that don't have reg
      debug(CYAN "OP[%d] (remove unused instructions)\n"RESET, op);
      for (size_t i = 0; insts[i]; i++)
      {
         Token *curr = insts[i]->token;
         if (!curr->ptr && !curr->reg && includes((Type[]) {INT, CHARS, CHAR, FLOAT, BOOL, 0}, curr->type))
         {
            curr->remove = true;
            did_something = true;
            did_optimize = true;
         }
      }
      if (did_something) print_ir();
      break;
   }
   case 5:
   {

      break;
   }
   default:
   {
      op = 0;
      if (!did_optimize) return false;
      did_optimize = false;
      return true;
      break;
   }
   }
   op++;
   return true;
}

size_t str_index;
size_t bloc_index;
Token *if_ir(Node *node)
{
   enter_scoop(node->token);
   Node *curr = node->left;

   Inst *tmp = new_inst(copy_token(node->token));
   setName(tmp->token, "if");
   tmp->token->type =  BLOC;
   tmp->token->index = ++bloc_index;

   generate_ir(curr->left); // TODO: check if it's boolean
   node->token->type = JNE;
   setName(node->token, "endif");
   node->token->index = ++bloc_index;

   Token *lastInst = copy_token(node->token);
   lastInst->space += TAB;
   new_inst(lastInst); // jne to endif
   curr = curr->right;

   // if code bloc
   while (curr->left)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }

   Inst *endInst = NULL;
   if (node->right)
   {
      endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space));
      endInst->token->index = node->token->index;
   }

   curr = node->right;
   while (curr)
   {
      if (curr->left->token->type == ELIF)
      {
         curr->left->token->index = ++bloc_index;
         curr->left->token->type = BLOC;
         setName(curr->left->token, "elif");
         {
            setName(lastInst, "elif");
            lastInst->index = curr->left->token->index;
            lastInst = copy_token(lastInst);
         }
         new_inst(curr->left->token);
         Node *tmp = curr->left;
         generate_ir(tmp->left); // elif condition, TODO: check is boolean
         new_inst(lastInst);
         tmp = tmp->right;
         while (tmp->left)
         {
            generate_ir(tmp->left);
            tmp = tmp->right;
         }
      }
      else if (curr->left->token->type == ELSE)
      {
         curr->left->token->index = ++bloc_index;
         curr->left->token->type = BLOC;
         setName(curr->left->token, "else");
         new_inst(curr->left->token);
         {
            setName(lastInst, "else");
            lastInst->index = curr->left->token->index;
            lastInst = copy_token(lastInst);
         }
         Node *tmp = curr->left;
         tmp = tmp->right;
         while (tmp->left)
         {
            generate_ir(tmp->left);
            tmp = tmp->right;
         }
         break;
      }

      setName(lastInst, "endif");
      lastInst->index = node->token->index;
      if (curr->right) // to not add a jmp in the last statement
      {
         endInst = new_inst(new_token("endif", 0, 5, JMP, node->token->space));
         endInst->token->index = node->token->index;
      }
      curr = curr->right;
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
   enter_scoop(node->token);
   node->token->type = BLOC;
   setName(node->token, "while");
   node->token->index = ++bloc_index;
   Inst *inst = new_inst(node->token);

   setName(node->left->token, "endwhile");
   generate_ir(node->left); // TODO: check if it's boolean
   node->left->token->index = node->token->index;

   Node *curr = node->right;
   while (curr) // while code bloc
   {
      Token *lastInst;
      switch (curr->left->token->type)
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
         generate_ir(curr->left); break;
      }
      curr = curr->right;
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
   enter_scoop(node->token);
   size_t tmp_ptr = ptr;
   ptr = 0;
   Inst* inst = new_inst(node->token);

   Node *curr = node->left;
   while (curr && curr->left)
   {
      // if (check(!curr->left, "is NULL 0")) exit(1);
      // if (check(!curr->left->token, "is NULL 1")) exit(1);
      // generate_ir(curr->left);
      new_variable(curr->left->token);
      if (curr->left->token->isref) curr->left->token->hasref = true;
      curr = curr->right;
   }
   pnode(node, NULL, 0);
   curr = node->right;
   while (curr)
   {
      if (check(!curr->left, "is NULL 0")) exit(1);
      if (check(!curr->left->token, "is NULL 1")) exit(1);
      generate_ir(curr->left);
      curr = curr->right;
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
      check(1, "handle this case"); // TODO
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;

      node->token->retType = func->token->retType;
      setReg(node->token, func->token->creg);
      Node *arg = node;
      Node *farg = func->left;
      while (arg->left && !found_error)
      {
         if (farg->left->token->isref)
         {
            generate_ir(arg->left);
            arg->left->left->token->isref = true;
            arg->left->left->token->hasref = 0;
            arg->left->left->token->ptr = 0;
            arg->left->token->ptr = 0;
         }
         else
         {
            generate_ir(arg->left);
         }
         arg = arg->right;
         farg = farg->right;
      }
   }
   Inst* inst = new_inst(node->token);
   return inst->token;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   check(!compatible(left, right), "invalid [%s] op between %s and %s \n",
         to_string(node->token->type), to_string(left->type), to_string(right->type));

   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;
   switch (node->token->type)
   {
   case ADD_ASSIGN: case ASSIGN:
      node->token->reg = left->reg;
      node->token->retType = getRetType(node);
      break;
   case ADD: case SUB: case MUL: case DIV:
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
      // debug(RED "this is [%d]\n" RESET, node->token->reg);
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   return inst->token;
}

Token* generate_ir(Node *node)
{
   if (found_error) return NULL;
   Inst *inst = NULL;
   switch (node->token->type)
   {
   case ID: return get_variable(node->token->name);
   case INT: case BOOL: case CHAR: case FLOAT:
   {
      inst = new_inst(node->token);
      if (inst->token->name && inst->token->declare) new_variable(inst->token);
      break;
   }
   case CHARS:
   {
      inst = new_inst(node->token);
      if (inst->token->name && inst->token->declare) new_variable(inst->token);
      else if (!node->token->creg) node->token->index = ++str_index;
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
         Scoop *scoop = &Gscoop[i];
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
         Scoop *scoop = &Gscoop[i];
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
   case STRUCT_DEF: return NULL;
   case STRUCT_CALL:
   {
      pnode(node, NULL, 0);
      // if(node->token) new_variable(node->token);
      node = node->right;
      while(node->left)
      {
         node->left->token->declare = true;
         generate_ir(node->left);
         node = node->right;
      }
      // exit(1);
      return NULL;
      break;
   }
   default: check(1, "handle this case %s", to_string(node->token->type)); return NULL;
   }
   return inst->token;
}

// ASSMBLY GENERATION
struct _IO_FILE *asm_fd;

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
   clone_insts();
   for (size_t i = 0; insts[i]; i++)
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
         char *inst = left->type == FLOAT ? "movss " : "mov ";
         check(right->isref && !right->hasref, "can't assign from reference that don't point to anything");
         check(left->isref && !left->hasref && !right->ptr, "first assignment for ref must have have ptr");
         check(left->isref && right->isref, "assignment between two references is forbidden");

         if (left->isref) debug("left is ref\n");
         if (left->hasref) debug("left has ref\n");
         // if(left->isarg) debug("left is arg\n");

         if (right->isref) debug("right is ref\n");
         if (right->hasref) debug("right has ref\n");
         // if(right->isarg) debug("right is arg\n");

         if (left->isref && !left->hasref)
         {
            // check(1, "found"); exit(1);
            // int a = 1 ref int b = a
            left->hasref = true;
            pasm("%irax, -%zu[rbp]", "lea", right->ptr); asm_space(curr->space);
            if (left->ptr) pasm("%iQWORD PTR -%zu[rbp], rax", "mov", left->ptr);
            else pasm("%irdi, rax", "mov"); // is function argument
         }
         else if (left->isref && left->hasref)
         {
            if (right->ptr)
            {
               // check(1, "found"); exit(1);
               // int a = 1 int c = 2 ref int b = a b = c putnbr(a)
               pasm("%irax, %a", "mov", left); asm_space(curr->space);
               pasm("%i%rd, %a", "mov", left, right); asm_space(curr->space);
               pasm("%i%ma, %rd", "mov", left, right);
            }
            else if (right->creg)
            {
               // check(1, "handle this case");
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
               // check(1, "found"); exit(1);
               // int a = 1 ref int b = a b = 3
               pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
               pasm("%i%ma, %v", "mov", left,  right);
            }
         }
         else if (left->ptr && !left->isref)
         {
            if (right->isref)
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
               pasm("%i%ra, .STR%zu[rip]", "lea", left, right->index); asm_space(curr->space);
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
            if (right->isref)
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
               pasm("%i%ra, .STR%zu[rip]", "lea", left, right->index);
               // pasm("%i%ra, %ra", "mov", left, right);
            }
            else // right is value
            {
               // check(1, "found"); exit(1);
               pasm("%i%ra, %v", "mov", left, right, left->name);
            }
         }
         else check(1, "handle this case");
         if (left->name) {pasm(" ;// assign [%s]", left->name); }
         else if (left->creg) {pasm(" ;// assign [%s]", left->creg);}
         if (left->isref) {pasm(" isref"); }
         asm_space(curr->space);
         break;
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
         if (curr->isCond)
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
            asm_space(curr->space); pasm("%i .%s%zu", inst, curr->name ? curr->name : "(null)", curr->index);
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
         asm_space(curr->space + TAB); pasm("%irsp, %zu", "sub", (((curr->ptr) + 15) / 16) * 16);
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
      case JE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%zu", "je", curr->name, curr->index); break;
      case JNE: pasm("%ial, 1", "cmp"); asm_space(curr->space); pasm("%i.%s%zu", "jne", curr->name, curr->index); break;
      case JMP: pasm("%i.%s%zu", "jmp", curr->name, curr->index); break;
      case FCALL: pasm("%i%s", "call", curr->name); break;
      case BLOC: pasm(".%s%zu:", (curr->name ? curr->name : "(null)"), curr->index); break;
      case END_BLOC: pasm(".end%s:", curr->name); break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
}

void add_builtins()
{
   if (found_error) return;
   create_builtin("putnbr", (Type[]) {INT, 0}, INT);
   create_builtin("write", (Type[]) {INT, CHARS, INT, 0}, INT);
   create_builtin("read", (Type[]) {INT, CHARS, INT, 0}, INT);
   create_builtin("exit", (Type[]) {INT, 0}, INT);
   create_builtin("malloc", (Type[]) {INT, 0}, VOID);
   create_builtin("calloc", (Type[]) {INT, INT, 0}, VOID);
   create_builtin("strdup", (Type[]) {CHARS, 0}, CHARS);
   create_builtin("strlen", (Type[]) {CHARS, 0}, INT);
   create_builtin("free", (Type[]) {VOID, 0}, VOID);
   create_builtin("strcpy", (Type[]) {CHARS, CHARS, 0}, CHARS);
   create_builtin("strncpy", (Type[]) {CHARS, CHARS, INT, 0}, CHARS);
   create_builtin("puts", (Type[]) {CHARS, 0}, INT);
   create_builtin("putstr", (Type[]) {CHARS, 0}, INT);
   create_builtin("putchar", (Type[]) {CHAR, 0}, INT);
   create_builtin("putbool", (Type[]) {BOOL, 0}, INT);
   create_builtin("putfloat", (Type[]) {FLOAT, 0}, INT);
}

void generate(char *name)
{
   if (found_error) return;
   debug(BLUE BOLD"GENERATE IR:\n" RESET);
   enter_scoop(new_token(".global", 0, strlen(".global"), ID, 0));
   Node *curr = head;
   add_builtins();
   while (curr && !found_error)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }
   if (found_error) return;
   debug(BLUE BOLD"PRINT IR:\n" RESET);
   print_ir();
   debug(BLUE BOLD"OPTIMIZE IR:\n" RESET);
   clone_insts();
   while (!found_error && optimize_ir()) clone_insts();
#if 1
   debug(BLUE BOLD"GENERATE ASM:\n" RESET);
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
