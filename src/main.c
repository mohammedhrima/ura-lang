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
      {"and", AND}, {"&&", AND}, {"or", OR}, {"||", OR},
      {0, (Type)0}
   };

   size_t i = 0;
   size_t space = 0;
   bool inc_space = true;
   while (input[i])
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
            if (strcmp(specials[j].value, ":") == 0) space++;
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

Node *func_dec()
{
   return NULL;
}

Node *func_call(Node *node)
{
   check(1, "handle this case");
   return NULL;
}

Node *func_main(Node *node)
{
   check(!find(RPAR, 0), "expected ) after main declaration", "");
   check(!find(DOTS, 0), "expected : after main() declaration", "");
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
   if (last->token->type != RETURN)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = new_node(new_token(NULL, 0, 0, RETURN, node->token->space + TAB));
      curr->left->left = new_node(new_token(NULL, 0, 0, INT, node->token->space + TAB));
   }
   return node;
}

Node *prime()
{
   Node *node = NULL;
   Token *token;
   if ((token = find(ID, INT, BOOL, CHARS, CHAR, 0)))
   {
      if (token->declare) // int variable_name
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
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return NULL;
}

void generate_ast()
{
   if (found_error) return;
   debug(BLUE BOLD"AST:\n" RESET);
   head = new_node(NULL);
   Node *curr = head;
   curr->left = expr();
   while (tokens[exe_pos]->type != END)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   debug(BLUE BOLD"PRINT AST:\n" RESET);
   curr = head;
   while (curr)
   {
      debug("%n\n", curr->left);
      curr = curr->right;
   }
}

// INTERMEDIATE REPRESENTATION
bool optimize_ir()
{
   return false;
}

Token* generate_ir(Node *node)
{
   static size_t str_index;
   static size_t bloc_index;

   if (found_error) return NULL;
   Inst *inst = NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *token = get_variable(node->token->name);
      return token;
   }
   case INT: case BOOL:
   {
      inst = new_inst(node->token);
      break;
   }
   case CHARS:
   {
      inst = new_inst(node->token);
      if (!node->token->creg) node->token->index = ++str_index;
      break;
   }
   case ASSIGN: case ADD_ASSIGN:
   case ADD: case SUB: case MUL: case DIV:
   case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(!compatible(left, right), "incompatible type for %s op\n", to_string(node->token->type));
      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      switch (node->token->type)
      {
      case ADD_ASSIGN:
      case ASSIGN:
         node->token->reg = left->reg;
         node->token->retType = left->type;
         break;
      case ADD: case SUB: case MUL: case DIV:
      {
         // TODO: verify this
         // if(left->type != INT && left->type != FLOAT)
         // {
         //     char *c = to_string(node->token->type);
         //     char *l = to_string(left->type);
         //     char *r = to_string(right->type);
         //     check(1, "Invalid operation [%s] between [%s] and [%s]\n", c, l, r);
         // }
         node->token->retType = left->type;
         if (right->type == INT) setReg(node->token, "eax");
         else if (right->type == FLOAT) setReg(node->token, "xmm0");
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
      default: break;
      }
      break;
   }
   case FDEC:
   {
      new_function(node);
      switch (node->token->retType)
      {
      case INT: setReg(node->token, "eax"); break;
      case CHARS: setReg(node->token, "rax"); break;
      default: check(1, "handle this case [%s]\n", to_string(node->token->retType)); break;
      }
      enter_scoop(node->token->name);

      debug("%n", node);
      size_t tmp_ptr = ptr;
      ptr = 0;
      inst = new_inst(node->token);

      // arguments
      Node *arg = node->left;
      // dest: left  (variable inside function)
      // src : right
      // debug(GREEN"print arguments\n"RESET);
      while (arg && arg->left)
      {
         debug("%n", arg->left);
         generate_ir(arg->left);
         arg = arg->right;
      }

      // code bloc
      Node *curr = node->right;
      while (curr)
      {
         generate_ir(curr->left);
         curr = curr->right;
      }
      Token *new = new_token(NULL, 0, 0, END_BLOC, node->token->space);
      new->name = strdup(node->token->name);
      new_inst(new);
      exit_scoop();
      node->token->ptr = ptr;
      ptr = tmp_ptr;
      break;
   }
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      break;
   }
   default:
      check(1, "handle this case %s", to_string(node->token->type));
      return NULL;
      break;
   }
   return inst->token;
}

// ASSMBLY GENERATION
void generate_asm()
{
   if (found_error) return;
}

void generate()
{
   if (found_error) return;
   debug(BLUE BOLD"GENERATE IR:\n" RESET);
   enter_scoop("");
   Node *curr = head;
   while (curr && !found_error)
   {
      generate_ir(curr->left);
      curr = curr->right;
   }
   debug(BLUE BOLD"PRINT IR:\n" RESET);
   print_ir();
   debug(BLUE BOLD"OPTIMIZE IR:\n" RESET);
   while (optimize_ir());
   debug(BLUE BOLD"GENERATE ASM:\n" RESET);
   generate_asm();
}

int main(int argc, char **argv)
{
   check(argc < 2, "Invalid arguments");
   open_file(argv[1]);
   tokenize();
   generate_ast();
   generate();
   free_memory();
}