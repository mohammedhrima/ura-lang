#include "./include/header.h"

// TOKENIZE
Specials *dataTypes = (Specials[]) {{"int", INT}, {"bool", BOOL}, {"chars", CHARS}, {0, (Type)0}};

Token* new_token(char *input, size_t s, size_t e, Type type, size_t space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   switch (type)
   {
   case INT:
   {
      while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
      break;
   }
   // case BLOC: case ID: case JMP: case JE: case JNE: case FDEC:
   case ID:
   {
      if (e > s)
      {
         new->name = allocate(e - s + 1, sizeof(char));
         strncpy(new->name, input + s, e - s);
         if (strcmp(new->name, "True") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = true;
         }
         else if (strcmp(new->name, "False") == 0)
         {
            free(new->name);
            new->name = NULL;
            new->type = BOOL;
            new->Bool.value = false;
         }
         else
         {
            for (int j = 0; dataTypes[j].value; j++)
            {
               if (strncmp(dataTypes[j].value, new->name, strlen(dataTypes[j].value)) == 0)
               {
                  new->type = dataTypes[j].type;
                  new->declare = true;
                  free(new->name);
                  new->name = NULL;
                  break;
               }
            }
         }
      }
      break;
   }
   case CHARS:
   {
      if (e > s)
      {
         new->Chars.value = allocate(e - s + 1, sizeof(char));
         strncpy(new->Chars.value, input + s, e - s);
      }
      break;
   }
   case CHAR:
   {
      if (e > s) new->Char.value = input[s];
      break;
   }
   default:
      break;
   }
   add_token(new);
   debug("new %k\n", new);
   return new;
}

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

void tokenize()
{
   if (obj.is_error) return;
   debug(GREEN "=========== TOKENIZE ===========\n" RESET);

   size_t i = 0;
   size_t space = 0;
   bool inc_space = true;
   char *input = obj.input;
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

// AST
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
      else if(token->type == ID && token->name && find(LPAR, 0))
      {
         node = new_node(token);
         if (strcmp(token->name, "main") == 0) return func_main(node);
         return func_call(node);
      }
      return new_node(token);
   }
   else check(1, "Unexpected token has type %s\n", to_string(obj.tokens[obj.exe_pos]->type));
   return NULL;
}

void generate_ast()
{
   if (obj.is_error) return;
   debug(GREEN "===========   AST    ===========\n" RESET);
   Node *head = new_node(NULL);
   Node *curr = head;
   curr->left = expr();
   while (obj.tokens[obj.exe_pos]->type != END)
   {
      curr->right = new_node(NULL);
      curr = curr->right;
      curr->left = expr();
   }
   debug(GREEN "=========== PRINT AST ==========\n" RESET);
   curr = head;
   while (curr)
   {
      debug("%n\n", curr->left);
      curr = curr->right;
   }
   obj.head = head;
}

void parse()
{
   if (obj.is_error) return;
   tokenize();
   generate_ast();
}