#include "./include/header.h"

void add_token(Token *token)
{
   static size_t pos;
   static size_t len;
   if (len == 0)
   {
      len = 10;
      obj.tokens = allocate(len, sizeof(Token *));
   }
   else if (pos + 1 == len)
   {
      Token **tmp = allocate(len * 2, sizeof(Token *));
      memcpy(tmp, obj.tokens, len * sizeof(Token *));
      free(obj.tokens);
      obj.tokens = tmp;
      len *= 2;
   }
   obj.tokens[pos++] = token;
}

Specials *dataTypes = (Specials[]) {
   {"int", INT}, {"bool", BOOL}, {"chars", CHARS}, {0, (Type)0},};

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

void generate_ast()
{
   if (obj.is_error) return;
   
}