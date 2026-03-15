#include "ura.h"

Token *new_token(Type type, int space)
{
   Token* token = allocate(1, sizeof(Token));
   token->type  = type;
   token->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(token);
   return token;
}

// ----------------------------------------------------------------------------
// Escape sequence decoder
// input[s] == '\\', input[s+1] is the escape character.
// Writes the decoded byte(s) to buf[*j] and returns the new value of s
// (pointing at the last consumed character; the caller's loop does s++ after).
// ----------------------------------------------------------------------------
static int parse_escape_seq(char *input, int s, int e, char *buf, int *j)
{
   switch (input[s + 1])
   {
   case 'n': buf[(*j)++] = '\n'; return s + 1;  // newline
   case 't': buf[(*j)++] = '\t'; return s + 1;  // tab
   case 'r': buf[(*j)++] = '\r'; return s + 1;  // carriage return
   case 'b': buf[(*j)++] = '\b'; return s + 1;  // backspace
   case 'f': buf[(*j)++] = '\f'; return s + 1;  // form feed
   case 'v': buf[(*j)++] = '\v'; return s + 1;  // vertical tab
   case 'a': buf[(*j)++] = '\a'; return s + 1;  // alert (bell)
   case '\\': buf[(*j)++] = '\\'; return s + 1; // backslash
   case '"':  buf[(*j)++] = '"';  return s + 1; // double quote
   case '\'': buf[(*j)++] = '\''; return s + 1; // single quote
   case '?':  buf[(*j)++] = '\?'; return s + 1; // question mark (trigraph)
   case '0':
   {
      // three-digit octal: \0NN
      if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
      {
         int octal = (input[s + 1] - '0') * 64 +
                     (input[s + 2] - '0') * 8 +
                     (input[s + 3] - '0');
         if (octal <= 255) { buf[(*j)++] = (char)octal; return s + 3; }
         buf[(*j)++] = '\0'; return s + 1;
      }
      // two-digit octal: \0N
      else if (s + 1 < e && isdigit(input[s + 2]))
      {
         int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
         buf[(*j)++] = (char)octal;
         return s + 2;
      }
      // plain null
      buf[(*j)++] = '\0';
      return s + 1;
   }
   case '1': case '2': case '3': case '4': case '5': case '6': case '7':
   {
      // three-digit octal: \NNN
      if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
      {
         int octal = (input[s + 1] - '0') * 64 +
                     (input[s + 2] - '0') * 8 +
                     (input[s + 3] - '0');
         if (octal <= 255) { buf[(*j)++] = (char)octal; return s + 3; }
         buf[(*j)++] = input[s]; return s + 1; // invalid, keep backslash
      }
      // two-digit octal
      else if (s + 2 < e && isdigit(input[s + 2]))
      {
         int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
         buf[(*j)++] = (char)octal;
         return s + 2;
      }
      // single-digit octal
      buf[(*j)++] = (char)(input[s + 1] - '0');
      return s + 1;
   }
   case 'x':   // Hexadecimal: \xFF
   {
      if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
      {
         int  hex = 0;
         char c1  = input[s + 2];
         char c2  = input[s + 3];
         if      (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
         else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
         else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;
         if      (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
         else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
         else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);
         buf[(*j)++] = (char)hex;
         return s + 3;
      }
      buf[(*j)++] = input[s]; // invalid hex escape, keep backslash
      return s + 1;
   }
   case 'u':   // \uXXXX — not fully implemented yet
      buf[(*j)++] = input[s];
      return s + 1;
   case 'U':   // \UXXXXXXXX — not fully implemented yet
      buf[(*j)++] = input[s];
      return s + 1;
   default:
      buf[(*j)++] = input[s]; // unknown escape, keep backslash
      return s + 1;
   }
}

Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space)
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
            break;
         }
      }
      if (keywords[i].name) break;

      struct { char *name; Type type; } keywords2[] =
      {
         {"and", AND}, {"or", OR}, {"is", EQUAL},
         {"not", NOT}, {"typeof", TYPEOF}, {"sizeof", SIZEOF}, {"output", OUTPUT},
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
            s = parse_escape_seq(input, s, e, value, &j);
         else
            value[j++] = input[s];
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

// ----------------------------------------------------------------------------
// `use` import handler — resolves and tokenizes an imported module
// ----------------------------------------------------------------------------
static bool s_calling_use = false; // file-level so handle_use_import can toggle it

static void handle_use_import(char *filename, char *input, int *i, int line)
{
   (void)line;
   while (isspace(input[*i])) (*i)++;
   check(input[*i] != '\"', "expected \" after use"); (*i)++;
   int s = *i;
   while (input[*i] && input[*i] != '\"') (*i)++;
   check(input[*i] != '\"', "expected \"");

   char *use = strndup(input + s, *i - s);
   (*i)++;

   char *use_filename = NULL;
   if (use[0] == '@')
   {
      char *lib = getenv("URA_LIB");
      if (!lib && g_argv0)
      {
         char resolved[PATH_MAX];
         if (realpath(g_argv0, resolved))
         {
            char resolved_copy[PATH_MAX];
            strncpy(resolved_copy, resolved, PATH_MAX - 1);
            resolved_copy[PATH_MAX - 1] = '\0';
            char       *bin_dir = dirname(resolved_copy);
            static char fallback[PATH_MAX];
            snprintf(fallback, sizeof(fallback), "%s/ura-lib", bin_dir);
            struct stat st;
            if (stat(fallback, &st) == 0 && S_ISDIR(st.st_mode))
               lib = fallback;
         }
      }
      check(!lib, "URA_LIB not set and no ura-lib/ found next to binary");
      char *resolved = strjoin(lib, use + 1, NULL);
      free(use);
      use          = resolved;
      use_filename = strdup(use);
   }
   else
      use_filename = strjoin(dirname(filename), "/", use);

   free(use);
   use              = strjoin(use_filename, ".ura", NULL);
   bool old         = s_calling_use;
   s_calling_use    = true;
   tokenize(use);
   s_calling_use    = old;
   free(use_filename);
}

// ----------------------------------------------------------------------------
// Tokenizer
// ----------------------------------------------------------------------------
void tokenize(char *filename)
{
   char *input = open_file(filename);
   if (found_error) return;

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
            handle_use_import(filename, input, &i, line);
         else
            parse_token(filename, line, input, s, i, ID, space);
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
   if (!s_calling_use) new_token(END, -1);
   free(input);
}
