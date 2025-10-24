#include "./header.h"

/*
SECTIONS:
   + LOGGING:
   + MEMMORY
   + PARSING
   + IR
   + UTILS
*/

// LOGGING
int debug(char *conv, ...)
{
   if (TESTING) return 0;
   int res = 0;
   va_list args;
   va_start(args, conv);

   for (int i = 0; conv[i]; i++)
   {
      if (conv[i] == '%')
      {
         i++;
         int left_align = 0;
         if (conv[i] == '-') { left_align = 1; i++; }
         int width = 0;
         while (isdigit(conv[i])) { width = width * 10 + (conv[i] - '0'); i++; }
         int prec = -1;
         if (conv[i] == '.')
         {
            i++;
            prec = 0;
            while (conv[i] >= '0' && conv[i] <= '9')
            {
               prec = prec * 10 + (conv[i] - '0');
               i++;
            }
         }
         if (strncmp(conv + i, "zu", 2) == 0) {res += fprintf(stdout, "%d", va_arg(args, int)); i++;}
         else if (strncmp(conv + i, "lld", 3) == 0)
         {res += fprintf(stdout, "%lld", va_arg(args, long long)); i += 2;}
         else
         {
            switch (conv[i])
            {
            case 'c': res += fprintf(stdout, "%c", va_arg(args, int)); break;
            case 's':
            {
               char *str = va_arg(args, char *);
               if (!str) str = "(null_str)";
               if (left_align)
               {
                  if (prec >= 0) res += fprintf(stdout, "%-*.*s", width, prec, str);
                  else res += fprintf(stdout, "%-*s", width, str);
               }
               else
               {
                  if (prec >= 0) res += fprintf(stdout, "%*.*s", width, prec, str);
                  else res += fprintf(stdout, "%*s", width, str);
               }
               break;
            }
            case 'p':
               res += fprintf(stdout, "%p", (void *)(va_arg(args, void *))); break;
            case 'x':
               if (prec >= 0)
                  res += fprintf(stdout, "%.*x", prec, va_arg(args, unsigned int));
               else
                  res += fprintf(stdout, "%x", va_arg(args, unsigned int));
               break;
            case 'X':
               if (prec >= 0) res += fprintf(stdout, "%.*X", prec, va_arg(args, unsigned int));
               else res += fprintf(stdout, "%X", va_arg(args, unsigned int));
               break;
            case 'd':
               if (prec >= 0) res += fprintf(stdout, "%.*d", prec, va_arg(args, int));
               else res += fprintf(stdout, "%d", va_arg(args, int));
               break;
            case 'f':
               if (prec >= 0) res += fprintf(stdout, "%.*f", prec, va_arg(args, double));
               else res += fprintf(stdout, "%f", va_arg(args, double));
               break;
            case '%': res += fprintf(stdout, "%%"); break;
            case 't':
               res += fprintf(stdout, "%s", to_string((Type)va_arg(args, Type))); break;
            case 'k':
            {
               Token *token = va_arg(args, Token *);
               res += token ? ptoken(token) : fprintf(stdout, "(null)");
               break;
            }
            case 'n':
            {
               Node *node = (Node *)va_arg(args, Node *);
               res += debug("node: ") + (node ?
                                         pnode(node, NULL, node->token->space) :
                                         fprintf(stdout, "(null)"));
               break;
            }
            default: todo(1, "invalid format specifier [%c]\n", conv[i]);
               break;
            }
         }
      }
      else res += fprintf(stdout, "%c", conv[i]);
   }
   va_end(args);
   return res;
}

int ptoken(Token *token)
{
   int res = 0;
   if (!token) return debug("null token");
   res += debug("[%-8s] ", to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT:
   case BOOL: case FLOAT: case LONG:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      // if (token->is_declare) res += debug("[declare] ");
      // if (!token->name)
      else
      {
         // if (token->creg) res += debug("creg [%s] ", token->creg);
         // else
         if (token->type != VOID) print_value(token);
      }
      break;
   }
   case STRUCT_CALL: case STRUCT_DEF:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      res += debug("st_name [%s] ", token->Struct.name);
      // res += debug("space [%d] ", token->space);
      res += debug("attributes:\n");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
#if 1
         for (int j = 0; !TESTING && j < token->space + TAB; )
            j += debug(" ");
         res += ptoken(attr);
         debug("\n");
         // + debug(", offset [%d] PTR [%d]\n", attr->offset, attr->ptr);
#else
         res += debug("%s %t [%d], ", attr->name,
                      attr->type, attr->ptr);
#endif
      }
      break;
   }
   case FCALL:
   case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   // if (token->ptr) res += debug("PTR [%d] ", token->ptr);
   if (token->ir_reg) res += debug("ir_reg [%d] ", token->ir_reg);
   if (token->is_ref) debug("ref ");
   if (token->has_ref) debug("has_ref ");
   if (token->remove) res += debug("[remove] ");
   if (token->retType) res += debug("ret [%t] ", token->retType);
   // if (!includes(token->type, STRUCT_CALL, STRUCT_DEF, 0))
   //    res += debug("space [%d] ", token->space);
   return res;
}

int pnode(Node *node, char *side, int space)
{
   if (!node) return 0;
   int res = 0;
   for (int i = 0; i < space; i++) res += debug(" ");
   if (side) res += debug("%s", side);
   res += debug("%k\n", node->token);
   res += pnode(node->left, "L: ", space + TAB);
   res += pnode(node->right, "R: ", space + TAB);
   if (node->children)
   {
      for (int i = 0; i < space; i++) res += debug(" ");
      res += debug("children: \n");
      for (int i = 0; i < node->cpos; i++)
         pnode(node->children[i], NULL, space + TAB);
   }
   return res;
}

int print_value(Token *token)
{
   // TODO: handle the other cases
   switch (token->type)
   {
   case INT: return debug("value [%lld] ", token->Int.value);
   case LONG: return debug("value [%lld] ", token->Long.value);
   case BOOL: return debug("value [%s] ", token->Bool.value ? "True" : "False");
   case FLOAT: return debug("value [%f] ", token->Float.value);
   case CHAR: return debug("value [%c] ", token->Char.value);
   case CHARS: return debug("value [%s] index [%d] ", token->Chars.value, token->index);
   case STRUCT_CALL: return debug("has [%d] attrs ", token->Struct.pos);
   case DEFAULT: return debug("default value ");
   case ADD: return debug("%t ", token->type); break;
   default: check(1, "handle this case [%s]\n", to_string(token->type));
   }
   return 0;
}

void print_ast(Node *head)
{
   if (!DEBUG) return;
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT AST:\n" RESET);
   for (int i = 0; !found_error && i < head->cpos; i++)
      debug("%n\n", head->children[i]);
   debug(GREEN BOLD SPLIT RESET);
}

void print_inst(Inst *inst)
{
   if (!DEBUG) return;
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   curr->ir_reg ? debug("r%.2d:", curr->ir_reg) : debug("rxx:");
   int k = 0;
   while (!TESTING && k < curr->space) k += debug(" ");
   debug("[%-6s] ", to_string(curr->type));

   switch (curr->type)
   {
   case ADD_ASSIGN: case ASSIGN:
   {
      debug("[%s] ", curr->assign_type ? to_string(curr->assign_type) : "");

      if (left->name) debug("r%.2d (%s) = ", left->ir_reg, left->name);
      else debug("r%.2d = ", left->ir_reg);

      if (right->ir_reg) debug("r%.2d (%s) ", right->ir_reg, right->name ? right->name : "");
      else print_value(right);
      break;
   }
   case ADD: case SUB: case MUL: case DIV:
   case EQUAL: case NOT_EQUAL: case LESS: case MORE:
   case LESS_EQUAL: case MORE_EQUAL:
   case AND: case OR:
   {
      if (left->ir_reg) debug("r%.2d", left->ir_reg);
      else print_value(left);
      if (left->name) debug("(%s)", left->name);

      debug(" to ");
      if (right->ir_reg) debug("r%.2d", right->ir_reg);
      else print_value(right);
      if (right->name) debug("(%s)", right->name);
      break;
   }
   case INT: case BOOL: case CHARS: case CHAR: case LONG:
   {
      if (curr->name) debug("name %s ", curr->name);
      if (curr->type == CHARS && !curr->name)
         debug("value \"%s\" in STR%d ", curr->Chars.value, curr->index);
      else if (!curr->name) print_value(curr);
      break;
   }
   case DOT: debug("get attribute [%s] in %k", right->name, left); break;
   case JMP: debug("jmp to [%s] ", curr->name); break;
   case JNE: debug("jne to [%s] ", curr->name); break;
   case FCALL: debug("call [%s] ", curr->name); break;
   case BLOC: case FDEC: debug("[%s] bloc ", curr->name); break;
   case END_BLOC: debug("[%s] endbloc ", curr->name); break;
   case STRUCT_CALL: debug("%s ", curr->name); break;
   case STRUCT_DEF: debug("%s ", curr->Struct.name); break;
   case BUILD_COND: debug("%s ", curr->name); break;
   case SET_POS: case APPEND_BLOC: case BUILD_BR:
      debug("%s ", left->name); break;
   case ACCESS: 
   {
      if (left->ir_reg) debug("r%.2d ", left->ir_reg);
      if (left->name) debug("(%s) ", left->name);
      debug("in ");
      if (right->ir_reg) debug("r%.2d ", right->ir_reg);
      else print_value(right);
      if (right->name) debug("(%s) ", right->name);
   }
   case IF: case ELIF: case END_IF: case ELSE: case END_COND:
   case STRUCT_ALLOC: case STRUCT_BODY:
   case RETURN: case CONTINUE: case BREAK: break;
   default: debug(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
   }
   debug("\n");
}

void print_ir()
{
   if (!DEBUG) return;
   copy_insts();
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT IR:\n" RESET);
   int i;
   for (i = 0; insts && insts[i]; i++) print_inst(insts[i]);
   debug("total instructions [%d]\n", i);
   debug(GREEN BOLD SPLIT RESET);
}

// MEMMORY
void *allocate_func(int line, int len, int size)
{
   void *res = calloc(len, size);
   check(!res, "allocate did failed in line %d\n", line);
   return res;
}

void free_token(Token *token)
{
   free(token->name);
   // free(token->creg);
   free(token->Chars.value);
   free(token->Struct.attrs);
   free(token->Struct.name);
   free(token);
}

void free_node(Node *node)
{
   if (!node) return;
   for (int i = 0; i < node->cpos; i++) free_node(node->children[i]);
   free_node(node->left);
   free_node(node->right);
   free(node->children);
   free(node->functions);
   free(node->vars);
   free(node->structs);
   free(node);
}

void free_memory()
{
   for (int i = 0; tokens && tokens[i]; i++)
   {
      free_token(tokens[i]);
      tokens[i] = NULL;
   }
   tk_pos = 0;
   exe_pos = 0;
}

// PARSING
Token* new_token(Type type, int space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(new);
   return new;
}

void parse_token(char *input, int s, int e,
                 Type type, int space, char *filename, int line)
{
   Token *new = new_token(type, space);
   new->line = line;
   new->filename = filename;
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

      struct { char *name; bool value; } bools[] = {{"True", true}, {"False", false}, {0, 0}};
      for (i = 0; bools[i].name; i++)
      {
         if (strcmp(new->name, bools[i].name) == 0)
         {
            setName(new, NULL);
            new->type = BOOL;
            new->Bool.value = bools[i].value;
            break;
         }
      }
      if (new->name == NULL) break;

      struct { char *name; Type type; } dataTypes [] =
      {
         {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
         {"char", CHAR}, {"float", FLOAT}, {"void", VOID},
         {"long", LONG}, {"pointer", PTR}, {"array", ARRAY},
         {"short", SHORT}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->is_declare = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {{"if", IF}, {"elif", ELIF},
         {"else", ELSE}, {"while", WHILE}, {"func", FDEC}, {"return", RETURN},
         {"break", BREAK}, {"continue", CONTINUE}, {"ref", REF}, {"and", AND},
         {"or", OR}, {"struct", STRUCT_DEF}, {"is", EQUAL}, {"protoFunc", PROTO_FUNC},
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
      break;
   }
   case BLOC: case JMP: case JE: case JNE: case FDEC:
   case END_BLOC:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      if (e <= s) break;
      int len = e - s;
      new->Chars.value = allocate(len + 1, sizeof(char));
      char *value = new->Chars.value;
      int j = 0;

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
            case '0': value[j++] = '\0'; s++; break;  // null character
            case '\\': value[j++] = '\\'; s++; break; // backslash
            case '"': value[j++] = '"'; s++; break;   // double quote
            case '\'': value[j++] = '\''; s++; break; // single quote
            case '?': value[j++] = '\?'; s++; break;  // question mark (for trigraphs)
            // Three digit octal
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
               if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3]))
               {
                  int octal = (input[s + 1] - '0') * 64 +
                              (input[s + 2] - '0') * 8 +
                              (input[s + 3] - '0');
                  if (octal <= 255) { value[j++] = (char)octal; s += 3; }
                  // Invalid octal, keep backslash
                  else value[j++] = input[s];
               }
               // Two digit octal
               else if (s + 2 < e && isdigit(input[s + 2]))
               {
                  int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
                  value[j++] = (char)octal;
                  s += 2;
               }
               // Single digit octal
               else { value[j++] = (char)(input[s + 1] - '0'); s++; }
               break;
            case 'x':
               // Hexadecimal escape sequences (\x00 to \xFF)
               if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3]))
               {
                  int hex = 0;
                  char c1 = input[s + 2];
                  char c2 = input[s + 3];

                  // Convert first hex digit
                  if (c1 >= '0' && c1 <= '9') hex += (c1 - '0') * 16;
                  else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
                  else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;

                  // Convert second hex digit
                  if (c2 >= '0' && c2 <= '9') hex += (c2 - '0');
                  else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
                  else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);

                  value[j++] = (char)hex;
                  s += 3;
               }
               else value[j++] = input[s]; // Invalid hex escape, keep backslash
               break;
            case 'u':
               // Unicode escape sequences (basic support)
               // \uXXXX - 4 hex digits for Unicode, basic implementation
               if (s + 5 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;
            case 'U':
               // \UXXXXXXXX - 8 hex digits for Unicode, basic implementation
               if (s + 9 < e) value[j++] = input[s]; // For now, keep as-is
               else value[j++] = input[s];
               break;

            default:
               // Unknown escape sequence, keep the backslash
               value[j++] = input[s];
               break;
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
      if (e == s + 1) new->Char.value = input[s];
      else // is special character
      {
         switch (input[s + 1])
         {
         case 'n':  new->Char.value = '\n';  break;  
         case 't':  new->Char.value = '\t';  break;  
         case 'r':  new->Char.value = '\r';  break;  
         case 'b':  new->Char.value = '\b';  break;  
         case 'f':  new->Char.value = '\f';  break;  
         case 'v':  new->Char.value = '\v';  break;  
         case 'a':  new->Char.value = '\a';  break;  
         case '0':  new->Char.value = '\0';  break;  
         case '\\': new->Char.value = '\\';  break;  
         case '"':  new->Char.value = '"'; break;    
         case '\'': new->Char.value = '\'';  break; 
         case '?':  new->Char.value = '\?';  break;  
         default: break;
         }
      }
      break;
   }
   default:
      check(e > s, "implement adding name for this one %s", to_string(type));
      break;
   }
#if DEBUG
   debug("new %k\n", new);
#endif
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   if (token->Struct.attrs)
   {
      new->Struct.attrs = allocate(token->Struct.size, sizeof(Token*));
      for (int i = 0; i < new->Struct.pos; i++)
         new->Struct.attrs[i] = copy_token(token->Struct.attrs[i]);
   }
   if (token->Struct.name) new->Struct.name = strdup(token->Struct.name);
   add_token(new);
   return new;
}

// IR
void copy_insts()
{
#if IR
   int pos = 0;
   int len = 100;
   free(insts);
   insts = allocate(len, sizeof(Inst *));

   for (int i = 0; OrgInsts && OrgInsts[i]; i++)
   {
      if (OrgInsts[i]->token->remove) continue;
      insts[pos++] = OrgInsts[i];
      if (pos + 1 == len)
      {
         Inst **tmp = allocate((len *= 2), sizeof(Inst *));
         memcpy(tmp, insts, pos * sizeof(Inst *));
         free(insts);
         insts = tmp;
      }
   }
#endif
}

void add_inst(Inst *inst)
{
   static int pos;
   static int len;
   if (len == 0)
   {
      len = 10;
      OrgInsts = allocate(len, sizeof(Inst *));
   }
   else if (pos + 2 == len)
   {
      Inst **tmp = allocate(len * 2, sizeof(Inst *));
      memcpy(tmp, OrgInsts, len * sizeof(Inst *));
      free(OrgInsts);
      OrgInsts = tmp;
      len *= 2;
   }
   OrgInsts[pos++] = inst;
}

Inst *new_inst(Token *token)
{
   static int ir_reg;

   Inst *new = allocate(1, sizeof(Inst));
   new->token = token;

   if (includes(token->type, DATA_TYPES) && token->name) token->ir_reg = ++ir_reg;
   switch (token->type)
   {
   case STRUCT_CALL:
   {
      for (int i = 0; i < token->Struct.pos; i++) {
         Token *attr = token->Struct.attrs[i];
         attr->ir_reg = ++ir_reg;
      }
      break;
   }
   case RETURN: token->ir_reg = ++ir_reg; break;
   case FCALL: case ADD: case SUB: case MUL: case DIV: case AND: case OR:
   case LESS: case LESS_EQUAL: case MORE: case MORE_EQUAL: case EQUAL:
   case ACCESS:
      token->ir_reg = ++ir_reg; break;
   default: break;
   }
#if DEBUG
   debug("new inst: %k%c", new->token, token->type != STRUCT_CALL ? '\n' : '\0');
#endif
   add_inst(new);
   return new;
}

Token *new_variable(Token *token)
{
#if DEBUG
   debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET,
         token->name, to_string(token->type), scoop->token);
#endif
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      check(strcmp(curr->name, token->name) == 0,
            "Redefinition of %s\n", token->name);
   }
   // if (token->ptr)
   // {
   //     debug(RED"%k already has ptr"RESET, token);
   //     todo(1, "");
   // }
   // if (token->type == STRUCT_CALL)
   // {
   //     token->ptr = ptr;
   //     if (token->is_arg) token->ptr += sizeofToken(token->Struct.attrs[0]);
   //     setAttrName(NULL, token);
   //     inc_ptr(sizeofToken(token)); // TODO: each struct must have attributes
   // }
   // else
   // {
   //     inc_ptr(sizeofToken(token));
   //     token->ptr = ptr;
   // }
   // new_inst(token);
   add_variable(scoop, token);
   return token;
}

// UTILS
char* open_file(char *filename)
{
   if (found_error) return NULL;
   // for (int i = 0; filename[i]; i++)
   //    if (filename[i] == ':') filename[i] = '/';

#if defined(__APPLE__)
   struct __sFILE *file;
#elif defined(__linux__)
   struct _IO_FILE *file;
#endif
   file = fopen(filename, "r");
   if (check(!file, "openning %s", filename)) return NULL;
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

// Token names table (static, global to this translation unit)
static const char *type_names[] = {
   [TMP] = "TMP", [CHILDREN] = "CHILD", [DEFAULT] = "DEF",
   [COMMENT] = "COMMENT", [END] = "END",

   [ID] = "ID", [REF] = "REF",
   [REF_ID] = "REF_ID", [REF_HOLD_ID] = "REF_HID",
   [REF_VAL] = "REF_VAL", [REF_HOLD_REF] = "REF_HRF",
   [REF_REF] = "REF_REF", [ID_ID] = "ID_ID",
   [ID_REF] = "ID_REF", [ID_VAL] = "ID_VAL",

   [VOID] = "VOID", [INT] = "INT",
   [FLOAT] = "FLOAT", [LONG] = "LONG",
   [SHORT] = "SHORT", [BOOL] = "BOOL",
   [CHAR] = "CHAR", [CHARS] = "CHARS",
   [PTR] = "PTR",
   [STRUCT_DEF] = "ST_DEF", [STRUCT_BODY] = "ST_BODY",
   [STRUCT_ALLOC] = "ST_ALLOC", [STRUCT_CALL] = "ST_CALL",
   [STRUCT_ATTR] = "ST_ATTR",
   [ARRAY] = "ARRAY",


   [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD_ASGN",
   [SUB_ASSIGN] = "SUB_ASGN", [MUL_ASSIGN] = "MUL_ASGN",
   [DIV_ASSIGN] = "DIV_ASGN", [MOD_ASSIGN] = "MOD_ASGN",
   [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQ",
   [LESS_EQUAL] = "LE_EQ", [MORE_EQUAL] = "MO_EQ",
   [LESS] = "LESS", [MORE] = "MORE", [ADD] = "ADD",
   [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV",
   [MOD] = "MOD", [AND] = "AND", [OR] = "OR", [NOT] = "NOT",

   [LPAR] = "LPAR", [RPAR] = "RPAR", [LBRA] = "LBRA",
   [RBRA] = "RBRA", [COMA] = "COMA", [DOT] = "DOT",
   [DOTS] = "DOTS", [ACCESS] = "ACCESS",

   [RETURN] = "RETURN", [ARROW] = "ARROW",
   [IF] = "IF", [ELIF] = "ELIF",
   [ELSE] = "ELSE", [END_IF] = "END_IF",
   [BUILD_COND] = "BLD_COND", [WHILE] = "WHILE",
   [CONTINUE] = "CONT", [BREAK] = "BREAK",
   [END_WHILE] = "END_WHILE", [BLOC] = "BLOC",
   [END_BLOC] = "ENDBLOC", [END_COND] = "ENDCOND",
   [APPEND_BLOC] = "APP_BLC", [SET_POS] = "SET_POS",
   [BUILD_BR] = "BLD_BR",

   [FDEC] = "FDEC", [FCALL] = "FCALL", [PROTO_FUNC] = "PROTO_FUNC",
   [JMP] = "JMP", [JE] = "JE", [JNE] = "JNE",

   [PUSH] = "PUSH", [POP] = "POP"
};


char *to_string_(char *filename, int line, Type type)
{
   int size = (int)(sizeof(type_names) / sizeof(type_names[0]));
   if (type > 0 && (int)type < size && type_names[type])
      return (char *)type_names[type];
   check(1, "Unknown type [%d] in %s:%d\n", type, filename, line);
   seg();
   return NULL;
}

bool check_error(const char *filename, const char *funcname,
                 int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   found_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, BOLD RED"ura_error:%s:%s:%d "RESET,
           filename, funcname, line);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   debug("\n");
   va_end(ap);
   return cond;
}

void add_token(Token *token)
{
   if (tk_len == 0)
   {
      tk_len = 10;
      tokens = allocate(tk_len, sizeof(Token *));
   }
   else if (tk_pos + 5 >= tk_len)
   {
      Token **tmp = allocate(tk_len * 2, sizeof(Token *));
      memcpy(tmp, tokens, tk_len * sizeof(Token *));
      free(tokens);
      tokens = tmp;
      tk_len *= 2;
   }
   tokens[tk_pos++] = token;
}

bool includes(Type to_find, ...)
{
   if (found_error) return false;
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0)
      if (current == to_find) return true;
   return false;
}

Token *find(Type type, ...)
{
   if (found_error) return NULL;
   va_list ap; va_start(ap, type);
   while (type)
   {
      // debug("search for %t, current %t\n", type, tokens[exe_pos]->type);
      if (type == tokens[exe_pos]->type)
         return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};

bool within_space(int space)
{
   return tokens[exe_pos]->space > space &&
          tokens[exe_pos]->type != END && !found_error;
}

char *strjoin(char *str0, char *str1, char *str2)
{
   int len0 = str0 ? strlen(str0) : 0;
   int len1 = str1 ? strlen(str1) : 0;
   int len2 = str2 ? strlen(str2) : 0;
   char *res = allocate(len0 + len1 + len2 + 1, 1);
   if (str0) strcpy(res, str0);
   if (str1) strcpy(res + len0, str1);
   if (str2) strcpy(res + len0 + len1, str2);
   return res;
}

void setName(Token *token, char *name)
{
   if (token->name) free(token->name);
   token->name = name ? strdup(name) : NULL;
}

void enter_scoop(Node *node)
{
#if DEBUG
   debug(CYAN "Enter Scoop: %k index %d\n" RESET,
         node->token, scoopPos + 1);
#endif
   if (Gscoop == NULL)
   {
      scoopSize = 10;
      Gscoop = allocate(scoopSize, sizeof(Node*));
   }
   else if (scoopPos + 1 >= scoopSize)
   {
      Node **tmp = allocate(scoopSize * 2, sizeof(Node*));
      memcpy(tmp, Gscoop, scoopPos * sizeof(Node*));
      scoopSize *= 2;
      free(Gscoop);
      Gscoop = tmp;
   }
   scoopPos++;
   Gscoop[scoopPos] = node;
   scoop = Gscoop[scoopPos];
}

void exit_scoop()
{
   if (check(scoopPos < 0, "No active scoop to exit\n")) return;
#if DEBUG
   debug(CYAN "Exit Scoop: %k index %d\n" RESET,
         Gscoop[scoopPos]->token, scoopPos);
#endif
   Gscoop[scoopPos] = NULL;
   scoopPos--;
   if (scoopPos >= 0) scoop = Gscoop[scoopPos];
}

void add_struct(Node *bloc, Token *token)
{
   if (bloc->structs == NULL)
   {
      bloc->ssize = 10;
      bloc->structs = allocate(bloc->ssize, sizeof(Token *));
   }
   else if (bloc->spos + 1 == bloc->ssize)
   {
      Token **tmp = allocate(bloc->ssize *= 2, sizeof(Token *));
      memcpy(tmp, bloc->structs, bloc->spos * sizeof(Token *));
      free(bloc->structs);
      bloc->structs = tmp;
   }
   bloc->structs[bloc->spos++] = token;
}

Token *new_struct(Token *token)
{
#if DEBUG
   debug(CYAN "in scoop %k, new struct [%k]\n" RESET,
         scoop->token, token);
#endif
   for (int i = 0; i < scoop->spos; i++)
   {
      debug(GREEN"loop [%d]\n"RESET, i);
      Token *curr = scoop->structs[i];
      check(strcmp(curr->Struct.name, token->Struct.name) == 0,
            "Redefinition of %s\n", token->Struct.name);
   }
   add_struct(scoop, token);
   return token;
}

Token *get_struct(char *name)
{
#if DEBUG
   debug(CYAN "get struct [%s] from scoop %k\n"RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *node = Gscoop[j];
      todo(node == NULL, RED"Error accessing NULL, %d\n"RESET, j);
#if DEBUG
      debug("[%d] scoop [%s] has %d structs\n", j,
            node->token->name, node->spos);
#endif
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->Struct.name, name) == 0)
            return node->structs[i];
   }
   // check(1, "%s not found\n", name);
   return NULL;
}

Token *is_struct(Token *token)
{
   Token *res = get_struct(token->name);
   if (res) return res;
   return NULL;
}

Node *new_node(Token *token)
{
   Node *new = allocate(1, sizeof(Node));
   new->token = token;
   return new;
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
      add_struct(new, copy_token(node->structs[i]));
   for (int i = 0; i < node->vpos; i++)
      add_variable(new, copy_token(node->vars[i]));
   return new;
}

Node* add_child(Node *node, Node *child)
{
   if (node->csize == 0)
   {
      node->csize = 10;
      node->children = allocate(node->csize, sizeof(Node *));
   }
   else if (node->cpos + 1 == node->csize)
   {
      Node **tmp = allocate(node->csize * 2, sizeof(Node *));
      memcpy(tmp, node->children, node->csize * sizeof(Node *));
      free(node->children);
      node->children = tmp;
      node->csize *= 2;
   }
   child->token->space = node->token->space + TAB;
   node->children[node->cpos++] = child;
   return child;
}

void add_attribute(Token *obj, Token *attr)
{
   if (obj->Struct.attrs == NULL)
   {
      obj->Struct.size = 10;
      obj->Struct.attrs = allocate(obj->Struct.size, sizeof(Token *));
   }
   else if (obj->Struct.pos + 1 == obj->Struct.size)
   {
      Token **tmp = allocate((obj->Struct.size *= 2), sizeof(Token *));
      memcpy(tmp, obj->Struct.attrs, obj->Struct.pos * sizeof(Token *));
      free(obj->Struct.attrs);
      obj->Struct.attrs = tmp;
   }
   attr->space = obj->space + TAB;
   obj->Struct.attrs[obj->Struct.pos++] = attr;
}

void add_variable(Node *bloc, Token *token)
{
   if (bloc->vars == NULL)
   {
      bloc->vsize = 10;
      bloc->vars = allocate(bloc->vsize, sizeof(Token *));
   }
   else if (bloc->vpos + 1 == bloc->vsize)
   {
      Token **tmp = allocate(bloc->vsize *= 2, sizeof(Token *));
      memcpy(tmp, bloc->vars, bloc->vpos * sizeof(Token *));
      free(bloc->vars);
      bloc->vars = tmp;
   }
   bloc->vars[bloc->vpos++] = token;
}

Token *get_variable(char *name)
{
#if DEBUG
   debug(CYAN "get variable [%s] from scoop %k\n"
         RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->vars[i]->name, name) == 0)
            return scoop->vars[i];
   }
   check(1, "%s not found\n", name);
   return NULL;
}

void add_function(Node *bloc, Node *node)
{
   if (bloc->functions == NULL)
   {
      bloc->fsize = 10;
      bloc->functions = allocate(bloc->fsize, sizeof(Node *));
   }
   else if (bloc->fpos + 1 == bloc->fsize)
   {
      bloc->fsize *= 2;
      Node **tmp = allocate(bloc->fsize, sizeof(Node *));
      memcpy(tmp, bloc->functions, bloc->fpos * sizeof(Node *));
      free(bloc->functions);
      bloc->functions = tmp;
   }
   bloc->functions[bloc->fpos++] = node;
}

Node *new_function(Node *node)
{
#if DEBUG
   // debug("new_func %s in scoop %kthat return %t\n", node->token->name, scoop->token, node->token->retType);
#endif
   for (int i = 0; i < scoop->fpos; i++)
   {
      Node *func = scoop->functions[i];
      bool cond = strcmp(func->token->name, node->token->name) == 0;
      check(cond, "Redefinition of %s\n", node->token->name);
   }
   // switch (node->token->retType)
   // {
   // case CHAR: case INT: case VOID:
   //     setReg(node->token, "eax"); break;
   // case LONG: case PTR: case CHARS:
   //     setReg(node->token, "rax"); break;
   // default:
   //     todo(1, "handle this case %s\n", to_string(node->token->retType));
   //     break;
   // }
   add_function(scoop, node);
   return node;
}

Node *get_function(char *name)
{
#if DEBUG
   debug("get_func %s in scoop %k\n", name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->fpos; i++)
         if (strcmp(scoop->functions[i]->token->name, name) == 0)
            return scoop->functions[i];
   }
   check(1, "'%s' Not found\n", name);
   return NULL;
}

bool compatible(Token *left, Token *right)
{
   Type left_type = left->type;
   Type right_type = right->type;
   Type left_ret_type = left->retType;
   Type right_ret_type = right->retType;
   if (left_type == CHARS && (right_type == PTR || right_ret_type == PTR))
      return true;
   return (left_type == right_type || left_type == right_ret_type ||
           left_ret_type == right_type || left_ret_type == right_ret_type);
}

Type getRetType(Node *node)
{
   if (!node || !node->token) return 0;
   if (includes(node->token->type, INT, CHARS, CHAR, FLOAT, BOOL, 0))
      return node->token->type;
   if (includes(node->token->retType, INT, CHARS, CHAR, FLOAT, BOOL, 0))
      return node->token->retType;

   Type left = 0, right = 0;
   if (node->left) left = getRetType(node->left);
   if (node->right) right = getRetType(node->right);
   if (left) return left;
   if (right) return right;
   return 0;
}

char* resolve_path(char* path)
{
   if (path == NULL) return NULL;

   char* cleaned = allocate(strlen(path) + 5, 1);
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