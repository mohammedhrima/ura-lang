#include "./include/header.h"

bool found_error;
bool did_pasm;
char *input;
Token **tokens;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

int ptr;
struct _IO_FILE *asm_fd;
int str_index;
int bloc_index;

char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9", NULL};
char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9", NULL};

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------

Token* new_token(Type type, int space)
{
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
   add_token(new);
   return new;
}

void parse_token(char *input, int s, int e, Type type, int space, char *filename, int line)
{
   Token *new = new_token(type, space);
   new->line = line;
   new->filename = filename;
   switch (type)
   {
   case INT: while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0'; break;
   case ID:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      int i = 0;

      struct { char *name; bool value; } bools[] = {{"True", true},  {"False", false}, {0, 0}};
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
      if (bools[i].name) break;

      struct { char *name; Type type; } dataTypes [] = { {"int", INT}, {"bool", BOOL},
         {"chars", CHARS}, {"char", CHAR}, {"float", FLOAT}, {"void", VOID}, {"long", LONG},
         {"pointer", PTR}, {"array", ARRAY}, {"short", SHORT}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->declare = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {{"if", IF}, {"elif", ELIF}, {"else", ELSE},
         {"while", WHILE}, {"func", FDEC}, {"return", RETURN}, {"break", BREAK},
         {"continue", CONTINUE}, {"ref", REF}, {"and", AND}, {"or", OR}, {"struct", STRUCT_DEF},
         {"is", EQUAL}, {"proto", PROTO}, {0, 0}
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
   case BLOC: case JMP: case JE: case JNE: case FDEC: case END_BLOC:
   {
      if (e <= s) break;
      new->name = allocate(e - s + 1, sizeof(char));
      strncpy(new->name, input + s, e - s);
      break;
   }
   case CHARS:
   {
      if (e <= s) break;
      new->Chars.value = allocate(e - s + 1, sizeof(char));
      strncpy(new->Chars.value, input + s, e - s);
      new->index = ++str_index;
      break;
   }
   case CHAR: if (e > s) new->Char.value = input[s]; break;
   default: check(e > s, "implement adding name for this one %s", to_string(type)); break;
   }
   debug("new %k\n", new);
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = allocate(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   if (token->creg) new->creg = strdup(token->creg);
   if (token->Struct.attrs)
   {
      new->Struct.attrs = allocate(token->Struct.len, sizeof(Token*));
      for (int i = 0; i < new->Struct.pos; i++) new->Struct.attrs[i] = copy_token(token->Struct.attrs[i]);
   }
   if (token->Struct.name) new->Struct.name = strdup(token->Struct.name);
   new->llvm = token->llvm;
   add_token(new);
   return new;
}

bool includes(Type to_find, ...)
{
   if (found_error) return false;
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0) if (current == to_find) return true;
   return false;
}

Token *find(Type type, ...)
{
   if (found_error) return NULL;
   va_list ap; va_start(ap, type);
   while (type)
   {
      if (type == tokens[exe_pos]->type) return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
};

bool within_space(int space)
{
   return tokens[exe_pos]->space > space && tokens[exe_pos]->type != END && !found_error;
}

int calculate_padding(int offset, int alignment)
{
   if (check(!alignment, "invalid alignment")) return 0;
   return (alignment - (offset % alignment)) % alignment;
}

int alignofToken(Token *token)
{
   return sizeofToken(token);
}

void enter_scoop(Node *node)
{
#if DEBUG
   debug(CYAN "Enter Scoop: %k index %d\n" RESET, node->token, scoopPos + 1);
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
   debug(CYAN "Exit Scoop: %k index %d\n" RESET, Gscoop[scoopPos]->token, scoopPos);
#endif
   Gscoop[scoopPos] = NULL;
   scoopPos--;
   if (scoopPos >= 0) scoop = Gscoop[scoopPos];
}

void set_struct_size(Token *token)
{
   int offset = 0;
   int max_align = 0;
   for (int i = 0; i < token->Struct.pos; i++)
   {
      Token *attr = token->Struct.attrs[i];
      int align = alignofToken(attr);
      int size = sizeofToken(attr);

      int padding = (align - (offset % align)) % align;
      offset += padding;
      attr->offset = offset;
      offset += size;

      if (align > max_align) max_align = align;
   }
   int final_padding = (max_align - (offset % max_align)) % max_align;
   token->offset = offset + final_padding;
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
   debug(CYAN "in scoop %k, new struct [%k]\n" RESET, scoop->token, token);
#endif
   for (int i = 0; i < scoop->spos; i++)
   {
      debug(GREEN"loop [%d]\n"RESET, i);
      Token *curr = scoop->structs[i];
      // if(curr->Struct.name == NULL){ todo(1, "found");}
      // if(token == NULL){ todo(1, "found");}
      // if(token->name == NULL){ todo(1, "found");}
      if (strcmp(curr->Struct.name, token->Struct.name) == 0) check(1, "Redefinition of %s\n", token->Struct.name);
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
      if (node == NULL)
      {
         debug(RED"Error accessing NULL, %d\n"RESET, j);
         exit(1);
      }
#if DEBUG
      debug("[%d] scoop [%s] has %d structs\n", j, node->token->name, node->spos);
#endif
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->Struct.name, name) == 0)
            return copy_token(node->structs[i]);
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
   for (int i = 0; i < node->cpos; i++) add_child(new, copy_node(node->children[i]));
   for (int i = 0; i < node->spos; i++) add_struct(new, copy_token(node->structs[i]));
   for (int i = 0; i < node->vpos; i++) add_variable(new, copy_token(node->vars[i]));
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
      obj->Struct.len = 10;
      obj->Struct.attrs = allocate(obj->Struct.len, sizeof(Token *));
   }
   else if (obj->Struct.pos + 1 == obj->Struct.len)
   {
      Token **tmp = allocate((obj->Struct.len *= 2), sizeof(Token *));
      memcpy(tmp, obj->Struct.attrs, obj->Struct.pos * sizeof(Token *));
      free(obj->Struct.attrs);
      obj->Struct.attrs = tmp;
   }
   attr->space = obj->space + TAB;
   obj->Struct.attrs[obj->Struct.pos++] = attr;
}

int sizeofToken(Token *token)
{
   if (token->is_ref) return sizeof(void*);
   switch (token->type)
   {
   case INT: return sizeof(int);
   case FLOAT: return sizeof(float);
   case CHARS: return sizeof(char *);
   case CHAR: return sizeof(char);
   case BOOL: return sizeof(bool);
   case LONG: return sizeof(long);
   case SHORT: return sizeof(short);
   case PTR: return sizeof(void*);
   case STRUCT_DEF: return token->offset;
   case STRUCT_CALL: return token->offset;
   default: todo(1, "add this type [%s]\n", to_string(token->type));
   }
   return 0;
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

void setAttrName(Token *parent, Token *child)
{
   if (parent)
   {
      child->is_attr = true;
      char *name = strjoin(parent->name, ".", child->name);
      setName(child, name);
      free(name);
   }
   for (int i = 0; i < child->Struct.pos; i++)
   {
      Token *attr = child->Struct.attrs[i];
      if (attr->type == STRUCT_CALL)
      {
         char *name = attr->name;
         int space = attr->space;
         int offset = attr->offset;

         attr = get_struct(attr->Struct.name);
         setName(attr, name);
         attr->offset = offset;
         attr->space = space;
         attr->type = STRUCT_CALL;
         setAttrName(child, attr);
         child->Struct.attrs[i] = attr;
      }
      else
      {
         setAttrName(child, attr);
      }
   }
}

Token *new_variable(Token *token)
{
#if DEBUG
   debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET, token->name, to_string(token->type), scoop->token);
#endif
   for (int i = 0; i < scoop->vpos; i++)
   {
      Token *curr = scoop->vars[i];
      if (strcmp(curr->name, token->name) == 0) check(1, "Redefinition of %s\n", token->name);
   }
   if (token->type == STRUCT_CALL)
   {
      setAttrName(NULL, token);
      if (token->is_ref && !token->ptr)
      {
         inc_ptr(sizeofToken(token));
         token->ptr = ptr;
      }
      else
      {
         // token->ptr = token->Struct.attrs[0]->ptr;
      }
   }
   else
   {
      // is attribute
      if (!token->ptr)
      {
         inc_ptr(sizeofToken(token));
         token->ptr = ptr;
      }
   }
   add_variable(scoop, token);
   return token;
}

Token *get_variable(char *name)
{
#if DEBUG
   debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j > 0; j--)
   {
      Node *scoop = Gscoop[j];
      for (int i = 0; i < scoop->vpos; i++)
         if (strcmp(scoop->vars[i]->name, name) == 0) return scoop->vars[i];
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
   switch (node->token->retType)
   {
   case CHAR: case INT: case VOID:
      setReg(node->token, "eax"); break;
   case LONG: case PTR: case CHARS:
      setReg(node->token, "rax"); break;
   default:
      todo(1, "handle this case %s\n", to_string(node->token->retType));
      break;
   }
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
         if (strcmp(scoop->functions[i]->token->name, name) == 0) return scoop->functions[i];
   }
   check(1, "'%s' Not found\n", name);
   return NULL;
}

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
void copy_insts()
{
   int pos = 0;
   int len = 100;
   free(insts);
   insts = allocate(len, sizeof(Inst *));

   for (int i = 0; OrgInsts && OrgInsts[i]; i++)
   {
      if (!OrgInsts[i]->token->remove)
      {
         insts[pos++] = OrgInsts[i];
         if (pos + 1 == len)
         {
            Inst **tmp = allocate((len *= 2), sizeof(Inst *));
            memcpy(tmp, insts, pos * sizeof(Inst *));
            free(insts);
            insts = tmp;
         }
      }
   }
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

   switch (token->type)
   {
   case CHARS:
   {
      if (token->ptr || token->creg) token->ir_reg = ++ir_reg;

      if (token->Chars.value)
      {
         debug("%k\n", new->token);
         stop(1, "found");
      }
      break;
   }
   case INT:
   {
      if (token->ptr || token->creg) token->ir_reg = ++ir_reg;
      break;
   }
   case STRUCT_CALL:
   {
      // debug("handle [%k], offset [%d]\n", token, token->offset);
      for (int i = 0; i < token->Struct.pos; i++) {
         Token *attr = token->Struct.attrs[i];
         // todo(1, "hello");
         if (attr->type == STRUCT_CALL) // struct ptr should be ptr for the first element
         {
            attr->ir_reg = ++ir_reg;
         }
         else
         {
            attr->ir_reg = ++ir_reg;
         }
      }
      break;
   }
   case RETURN: token->ir_reg = ++ir_reg; break;
   case ASSIGN:
   {
      break;
   }
   default: break;
   }
#if DEBUG
   debug("new inst: %k%c", new->token, token->type != STRUCT_CALL ? '\n' : '\0');
#endif
   add_inst(new);
   return new;
}

void to_default(Token *token, Type type)
{
   token->type = type;
   if (token->is_ref) return;
   switch (type)
   {
   case CHARS:
   {
      token->Chars.value = strdup("\"\"");
      break;
   }
   case CHAR: case LONG:
   case PTR: case INT: break;
   default: check(1, "handle this case [%s]", to_string(type)); break;
   }
}

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

         // TODO: check if left and right are compatible
         // test if left is function, and right is number ...
         if (
            includes(token->type, ADD, SUB, MUL, DIV, 0) && includes(left->type, INT, FLOAT, CHAR, 0)
            && compatible(left, right) && !left->name && !right->name)
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
            token->ir_reg = 0;
            setReg(token, NULL);
#if DEBUG
            debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
            if (i > 0) i -= 2;
         }
      }
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
               copy_insts();
               did_something = true;
               did_optimize = true;
#if DEBUG
               debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
               continue;
            }
            // else
            // if(curr_right == prev_token && !prev_left->name && !curr_left->name)
            // {
            //   // prev_r->type = INT;
            //   // curr_token->remove = true;
            //   prev_left->Int.value += curr_left->Int.value;
            //   i = 1;
            //   copy_insts();
            //   optimize = true;
            //   continue;
            // }
         }
      }
      break;
   }
   case 2:
   {
      debug(CYAN "OP[%d] remove reassigned variables\n" RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         // if (token->declare)
         // {
         //    for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
         //    {
         //       if (insts[j]->token->type == ASSIGN && insts[j]->left->ir_reg == token->ir_reg )
         //       {
         //          // debug(RED"1. remove r%d %k\n"RESET, token->ir_reg, token);
         //          token->declare = false;
         //          token->remove = true;
         //          did_optimize = true;
         //          did_something = true;
         //          insts[j]->left->is_ref = token->is_ref;
         //          break;
         //       }
         //       else if ((insts[j]->left && insts[j]->left->ir_reg == token->ir_reg) ||
         //                (insts[j]->right && insts[j]->right->ir_reg == token->ir_reg)) {
         //          break;
         //       }
         //    }
         // }
         // else
         if (token->type == ASSIGN)
         {
            if (includes(token->assign_type, REF_HOLD_ID, REF_REF, 0)) continue;
            for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
            {
               if (!insts[j]->left || !insts[j]->right || !token->ir_reg) continue;
               // TODO: to be checked
               // I replaced insts[j]->left == insts[i]->left with insts[j]->left->ir_reg == insts[i]->left->ir_reg
               if (insts[j]->token->type == ASSIGN && insts[j]->left->ir_reg == token->ir_reg)
               {
                  // debug(RED"2. remove r%d %k\n"RESET, token->ir_reg, token);
                  token->remove = true;
#if DEBUG
                  debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                  did_optimize = true;
                  did_something = true;
                  break;
               }
               else if (insts[j]->left->ir_reg == token->ir_reg || insts[j]->right->ir_reg == token->ir_reg)
                  break;
            }
         }
      }
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
#if DEBUG
            debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
            copy_insts();
            i = 1;
         }
      }
      break;
   }
   case 4:
   {
      // TODO: be carefull this one remove anything that don't have ir_reg
      debug(CYAN "OP[%d] remove unused instructions\n"RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *curr = insts[i]->token;
         if (!curr->ptr && !curr->ir_reg && !curr->creg && includes(curr->type, INT, CHARS, CHAR, FLOAT, BOOL, 0))
         {
            curr->remove = true;
            did_something = true;
            did_optimize = true;
#if DEBUG
            debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
         }
      }
      break;
   }
   case 5:
   {
      debug(CYAN "OP[%d] remove structs declarations\n"RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *curr = insts[i]->token;
         Token *left = insts[i]->left;
         // Token *right = insts[i]->right;
         if (curr->type == ASSIGN && left->type == STRUCT_CALL)
         {
            curr->remove = true;
#if DEBUG
            debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
            did_something = true;
            did_optimize = true;
         }
      }
      break;
   }
   case 6:
   {
      debug(CYAN "OP[%d] replace DEFAULT\n"RESET, op);
      for (int i = 0; insts[i]; i++)
      {
         Token *token = insts[i]->token;
         Token *right = insts[i]->right;
         Token *left = insts[i]->left;
         if (token->type == ASSIGN && right->type == DEFAULT)
         {
            todo(1, "there should not be any default remaining here")
            to_default(right, left->type);
         }
      }
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
   if (did_something)
   {
#if DEBUG
      print_ir();
#endif
   }
   op++;
   return true;
}

void asm_space(int space)
{
   // if (did_pasm)
   {
      space = (space / TAB) * 4;
      pasm("\n");
      for (int i = 0; i < space; i++) pasm(" ");
      did_pasm = false;
   }
}

void pasm(char *fmt, ...)
{
   did_pasm = true;
   int i = 0;
   va_list args;
   va_start(args, fmt);

   while (fmt[i])
   {
      if (fmt[i] == '%')
      {
         i++;
         if (fmt[i] == 'i')
         {
            i++;
            fprintf(asm_fd, "%-4s ", va_arg(args, char *));
         }
         else if (strncmp(fmt + i, "ra", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg && token->creg[1] != 'a') fprintf(asm_fd, "{{%s}}", token->creg); // TODO: those lines are bad
            else
            {
               if (token->is_ref)
               {
                  fputs("rax", asm_fd);
               }
               else
               {
                  Type type = token->retType ? token->retType : token->type;
                  switch (type)
                  {
                  case CHARS: fputs("rax", asm_fd); break;
                  case INT: fputs("eax", asm_fd); break;
                  case BOOL: case CHAR: fputs("al", asm_fd); break;
                  case FLOAT: fputs("xmm0", asm_fd); break;
                  default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
                  }
               }
            }
         }
         else if (strncmp(fmt + i, "rb", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg && token->creg[1] != 'b') fprintf(asm_fd, "{{%s}}", token->creg);
            else
            {
               if (token->is_ref)
               {
                  fputs("rbx", asm_fd);
               }
               else
               {
                  Type type = token->retType ? token->retType : token->type;
                  switch (type)
                  {
                  case CHARS: fputs("rbx", asm_fd); break;
                  case INT: fputs("ebx", asm_fd); break;
                  case BOOL: case CHAR: fputs("bl", asm_fd); break;
                  case FLOAT: fputs("xmm1", asm_fd); break;
                  default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
                  }
               }
            }
         }
         else if (strncmp(fmt + i, "rd", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            if (token->creg && strcmp(token->creg + 1, "dx")) fprintf(asm_fd, "{{%s}}", token->creg);
            else
            {
               Type type = token->retType ? token->retType : token->type;
               switch (type)
               {
               case CHARS: fputs("edx", asm_fd); break;
               case INT: fputs("edx", asm_fd); break;
               case BOOL: case CHAR: fputs("dl", asm_fd); break;
               case FLOAT: fputs("xmm2", asm_fd); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
            }
         }
         else if (fmt[i] == 'r')
         {
            i++;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "%s", token->creg);
            else
            {
               check(1, "fix this one");
               fputs("rxx", asm_fd);
            }
         }
         else if (fmt[i] == 'a')
         {
            i++;
            Token *token = va_arg(args, Token *);
            if (token->creg) fprintf(asm_fd, "{{%s}}", token->creg);
            else if (token->is_ref) fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr);
            else
               switch (token->type)
               {
               case CHARS: fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr); break;
               case LONG: fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr); break;
               case INT: fprintf(asm_fd, "DWORD PTR -%d[rbp]", token->ptr); break;
               case CHAR: fprintf(asm_fd, "BYTE PTR -%d[rbp]", token->ptr); break;
               case BOOL: fprintf(asm_fd, "BYTE PTR -%d[rbp]", token->ptr); break;
               case FLOAT: fprintf(asm_fd, "DWORD PTR -%d[rbp]", token->ptr); break;
               case PTR: fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr); break;
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
         }
         else if (strncmp(fmt + i, "ma", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            // if (token->is_ref) fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr);
            // else
            Type type = token->retType ? token->retType : token->type;
            switch (type)
            {
            case CHARS: fprintf(asm_fd, "QWORD PTR [rax]"); break;
            case INT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
            case CHAR: fprintf(asm_fd, "BYTE PTR [rax]"); break;
            case BOOL: fprintf(asm_fd, "BYTE PTR [rax]"); break;
            case FLOAT: fprintf(asm_fd, "DWORD PTR [rax]"); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         else if (strncmp(fmt + i, "mb", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            // if (token->is_ref) fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr);
            // else
            switch (token->type)
            {
            case CHARS: fprintf(asm_fd, "QWORD PTR [rbx]"); break;
            case INT: fprintf(asm_fd, "DWORD PTR [rbx]"); break;
            case CHAR: fprintf(asm_fd, "BYTE PTR [rbx]"); break;
            case BOOL: fprintf(asm_fd, "BYTE PTR [rbx]"); break;
            case FLOAT: fprintf(asm_fd, "DWORD PTR [rbx]"); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         else if (fmt[i] == 'v')
         {
            i++;
            Token *token = va_arg(args, Token *);
            switch (token->type)
            {
            case INT: fprintf(asm_fd, "%ld", token->Int.value); break;
            case BOOL: fprintf(asm_fd, "%d", token->Bool.value); break;
            case CHAR: fprintf(asm_fd, "%d", token->Char.value); break;
            case LONG: fprintf(asm_fd, "%lld", token->Long.value); break;
            default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
            }
         }
         else
         {
            int handled = 0;
#define check_format(string, type)                     \
               do                                                   \
               {                                                    \
                  if (strncmp(fmt + i, string, strlen(string)) == 0) \
                  {                                                  \
                     handled = 1;                                     \
                     i += strlen(string);                             \
                     fprintf(asm_fd, "%" string, va_arg(args, type)); \
                  }                                                  \
               } while (0)
            check_format("d", int);
            check_format("ld", long);
            check_format("s", char *);
            check_format("zu", unsigned long);
            check_format("f", double);
            check(!handled, "handle this case [%s]\n", fmt + i);
         }
      }
      else
      {
         fputc(fmt[i], asm_fd);
         i++;
      }
   }
   va_end(args);
}

void initialize()
{
   pasm(".intel_syntax noprefix\n");
   pasm(".include \"/import/header.s\"\n");
   pasm(".text\n");
   pasm(".globl	main\n");
}

void finalize()
{
#if TOKENIZE
   for (int i = 0; tokens[i]; i++)
   {
      Token *curr = tokens[i];
      // test char variable before making any modification
      if (curr->type == CHARS && curr->Chars.value && !curr->creg)
         pasm(".STR%d: .string %s\n", curr->index, curr->Chars.value);
      if (curr->type == FLOAT && !curr->name && !curr->ptr && !curr->creg)
         pasm(".FLT%d: .long %d /* %f */\n", curr->index, *((uint32_t *)(&curr->Float.value)), curr->Float.value);
   }
   pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
#endif
}

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------

char* open_file(char *filename)
{
   if (found_error) return NULL;
   //filename = strjoin("/", filename, NULL);

   for (int i = 0; filename[i]; i++) if (filename[i] == ':') filename[i] = '/';

   struct _IO_FILE *file = fopen(filename, "r");

   if (check(!file, "openning %s", filename))
   {
      // free(filename);
      return NULL;
   }
   //free(filename);
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   return input;
}

void *allocate_func(int line, int len, int size)
{
   void *res = calloc(len, size);
   check(!res, "allocate did failed in line %d\n", line);
   return res;
}

const char *to_string_(const char *filename, const int line, Type type) {
   const char *arr[] = {
      [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD ASSIGN", [SUB_ASSIGN] = "SUB ASSIGN",
      [MUL_ASSIGN] = "MUL ASSIGN", [DIV_ASSIGN] = "DIV ASSIGN", [MOD_ASSIGN] = "MOD_ASSIGN",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT EQUAL", [LESS_EQUAL] = "LESS THAN OR EQUAL",
      [MORE_EQUAL] = "MORE THAN OR EQUAL", [LESS] = "LESS THAN", [MORE] = "MORE THAN",
      [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [MOD] = "MOD",
      [AND] = "AND", [OR]  = "OR", [RPAR] = "R_PAR", [LPAR] = "L_PAR", [COMA] = "COMMA",
      [DOTS] = "DOTS", [DOT] = "DOT", [RETURN] = "RETURN", [IF] = "IF", [ELIF] = "ELIF",
      [ELSE] = "ELSE", [WHILE] = "HILE", [CONTINUE] = "continue", [BREAK] = "break", [REF] = "REF",
      [FDEC] = "F_DEC", [FCALL] = "F_CALL", [PROTO] = "PROTO", [INT] = "INT", [VOID] = "VOID", [CHARS] = "CHARS",
      [CHAR] = "CHAR", [BOOL] = "BOOL", [FLOAT] = "FLOAT", [SHORT] = "SHORT", [STRUCT_CALL] = "ST_CALL",
      [STRUCT_DEF] = "ST_DEF", [ID] = "ID", [END_BLOC] = "END_BLOC", [BLOC] = "BLOC",
      [JNE] = "JNE", [JE] = "JE", [JMP] = "JMP", [LBRA] = "L_BRA", [RBRA] = "R_BRA",
      [END] = "END", [CHILDREN] = "CHILDREN", [TMP] = "TMP", [LONG] = "LONG", [PTR] = "PTR",
      [REF_ID] = "REF_ID", [REF_HOLD_ID] = "REF_HOLD_ID", [REF_VAL] = "REF_VAL",
      [REF_HOLD_REF] = "REF_HOLD_REF", [REF_REF] = "REF_REF", [ID_ID] = "ID_ID", [ARRAY] = "ARRAY",
      [ID_REF] = "ID_REF", [ID_VAL] = "ID_VAL", [DEFAULT] = "DEFAULT", [ACCESS] = "ACCESS"
   };
   if (type > 0 && type < sizeof(arr) / sizeof(arr[0]) && arr[type] != NULL) return arr[type];
   check(1, "Unknown type [%d] in %s:%d\n", type, filename, line);
   return NULL;
}

bool check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   found_error = true;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, BOLD RED"Error:%s:%s:%d "RESET, filename, funcname, line);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   return cond;
}

int tk_pos;
int tk_len;

void add_token(Token *token)
{
   if (tk_len == 0)
   {
      tk_len = 10;
      tokens = allocate(tk_len, sizeof(Token *));
   }
   else if (tk_pos + 1 == tk_len)
   {
      Token **tmp = allocate(tk_len * 2, sizeof(Token *));
      memcpy(tmp, tokens, tk_len * sizeof(Token *));
      free(tokens);
      tokens = tmp;
      tk_len *= 2;
   }
   tokens[tk_pos++] = token;
}

void setName(Token *token, char *name)
{
   if (token->name)
   {
      free(token->name);
      token->name = NULL;
   }
   if (name) token->name = strdup(name);
}

void setReg(Token *token, char *creg)
{
   if (token->creg)
   {
      free(token->creg);
      token->creg = NULL;
   }
   if (creg) token->creg = strdup(creg);
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

bool compatible(Token *left, Token *right)
{
   Type ltype = left->type;
   Type lrtype = left->retType;
   Type rtype = right->type;
   Type rrtype = right->retType;
   if (ltype == CHARS && (rtype == PTR || rrtype == PTR)) return true;
   return (ltype == rtype || ltype == rrtype || lrtype == rtype || lrtype == rrtype);
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
#if DEBUG_INC_PTR
void inc_ptr_(char *filename, int line, int size)
#else
void inc_ptr(int size)
#endif
{
#if DEBUG_INC_PTR
   debug(RED"%s:%d add [%d] to ptr [%d]\n"RESET, filename, line, size, ptr);
#endif
   ptr += size;
}

// ----------------------------------------------------------------------------
// Logs
// ----------------------------------------------------------------------------
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
         int precision = -1;
         if (conv[i] == '.')
         {
            i++;
            precision = 0;
            while (conv[i] >= '0' && conv[i] <= '9')
            {
               precision = precision * 10 + (conv[i] - '0');
               i++;
            }
         }
         if (strncmp(conv + i, "zu", 2) == 0) { res += fprintf(stdout, "%d", va_arg(args, int)); i++; }
         else if (strncmp(conv + i, "lld", 3) == 0) { res += fprintf(stdout, "%lld", va_arg(args, long long)); i += 2; }
         else
         {
            switch (conv[i])
            {
            case 'c': res += fprintf(stdout, "%c", va_arg(args, int)); break;
            case 's':
            {
               char *str = va_arg(args, char *);
               if (left_align)
               {
                  if (precision >= 0) res += fprintf(stdout, "%-*.*s", width, precision, str);
                  else res += fprintf(stdout, "%-*s", width, str);
               }
               else
               {
                  if (precision >= 0) res += fprintf(stdout, "%*.*s", width, precision, str);
                  else res += fprintf(stdout, "%*s", width, str);
               }
               break;
            }
            case 'p': res += fprintf(stdout, "%p", (void *)(va_arg(args, void *))); break;
            case 'x':
               if (precision >= 0) res += fprintf(stdout, "%.*x", precision, va_arg(args, unsigned int));
               else res += fprintf(stdout, "%x", va_arg(args, unsigned int));
               break;
            case 'X':
               if (precision >= 0) res += fprintf(stdout, "%.*X", precision, va_arg(args, unsigned int));
               else res += fprintf(stdout, "%X", va_arg(args, unsigned int));
               break;
            case 'd':
               if (precision >= 0) res += fprintf(stdout, "%.*d", precision, va_arg(args, int));
               else res += fprintf(stdout, "%d", va_arg(args, int));
               break;
            case 'f':
               if (precision >= 0) res += fprintf(stdout, "%.*f", precision, va_arg(args, double));
               else res += fprintf(stdout, "%f", va_arg(args, double));
               break;
            case '%': res += fprintf(stdout, "%%"); break;
            case 't': res += fprintf(stdout, "%s", to_string((Type)va_arg(args, Type))); break;
            case 'k':
            {
               Token *token = va_arg(args, Token *);
               res += token ? ptoken(token) : fprintf(stdout, "(null)");
               break;
            }
            case 'n':
            {
               Node *node = (Node *)va_arg(args, Node *);
               res += debug("node: ") + (node ? pnode(node, NULL, node->token->space) : fprintf(stdout, "(null)"));
               break;
            }
            default: check(1, "invalid format specifier [%c]\n", conv[i]); exit(1); break;
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
   res += debug("[%-7s] ", to_string(token->type));
   switch (token->type)
   {
   case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT: case LONG:
   {
      if (token->name) res += debug("name [%s] ", token->name);
      // if (token->declare) res += debug("[declare] ");
      // if (!token->name)
      else
      {
         if (token->creg) res += debug("creg [%s] ", token->creg);
         else if (token->type != VOID) print_value(token);
      }
      break;
   }
   case STRUCT_CALL: case STRUCT_DEF:
   {
      res += debug("name [%s] ", token->Struct.name);
      res += debug("space [%d] ", token->space);
      res += debug("attributes:\n");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
#if 1
         for (int j = 0; !TESTING && j < token->space + TAB; ) j += debug(" ");
         res += ptoken(attr)  + debug(", offset [%d] PTR [%d]\n", attr->offset, attr->ptr);
#else
         res += debug("%s %t [%d], ", attr->name, attr->type, attr->ptr);
#endif
      }
      break;
   }
   case FCALL:
   case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   if (token->ptr) res += debug("PTR [%d] ", token->ptr);
   if (token->ir_reg) res += debug("ir_reg [%d] ", token->ir_reg);
   if (token->is_ref) debug("ref ");
   if (token->has_ref) debug("has_ref ");
   if (token->remove) res += debug("[remove] ");
   if (token->retType) res += debug("ret [%t] ", token->retType);
   if (!includes(token->type, STRUCT_CALL, STRUCT_DEF, 0)) res += debug("space [%d] ", token->space);
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
      for (int i = 0; i < node->cpos; i++) pnode(node->children[i], NULL, space + TAB);
   }
   return res;
}

int print_value(Token *token)
{
   switch (token->type)
   {  // TODO: handle the other cases
   case INT: return debug("value [%lld] ", token->Int.value);
   case LONG: return debug("value [%lld] ", token->Long.value);
   case BOOL: return debug("value [%s] ", token->Bool.value ? "True" : "False");
   case FLOAT: return debug("value [%f] ", token->Float.value);
   case CHAR: return debug("value [%c] ", token->Char.value);
   case CHARS: return debug("value [%s] index [%d] ", token->Chars.value, token->index);
   case STRUCT_CALL: return debug("has [%d] attrs ", token->Struct.pos);
   case DEFAULT: return debug("default value ");
   default: check(1, "handle this case [%s]\n", to_string(token->type));
   }
   return 0;
}

void print_ir()
{
   //if (!DEBUG) return;
   copy_insts();
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT IR:\n" RESET);
   int i = 0;
   for (i = 0; insts[i]; i++)
   {
      Token *curr = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      curr->ir_reg ? debug("r%.2d:", curr->ir_reg) : debug("rxx:");
      int k = 0;
      while (!TESTING && k < curr->space) k += debug(" ");
      switch (curr->type)
      {
      case ADD_ASSIGN: case ASSIGN:
      {
         debug("[%-6s] [%s] ", to_string(curr->type), (curr->assign_type ? to_string(curr->assign_type) : ""));
         if (left->creg) debug("r%.2d (%s) = ", left->ir_reg, left->creg);
         else debug("r%.2d (%s) = ", left->ir_reg, left->name);

         if (right->ir_reg) debug("r%.2d (%s) ", right->ir_reg, right->name ? right->name : "");
         else if (right->creg) debug("[%s] ", right->creg);
         else print_value(right);
         break;
      }
      case ACCESS:
      {
         debug("[%-6s] ", to_string(curr->type));

         if (right->ir_reg) debug("r%.2d ", right->ir_reg);
         else print_value(right);
         if (right->name) debug("(%s) ", right->name);

         debug("in ");

         if (left->ir_reg) debug("r%.2d ", left->ir_reg);
         else if (left->creg) debug("creg %s ", left->creg);

         if (left->name) debug("(%s) ", left->name);

         break;
      }
      case ADD: case SUB: case MUL: case DIV:
      case EQUAL: case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
      {
         debug("[%-6s] ", to_string(curr->type));
         if (left->ir_reg) debug("r%.2d", left->ir_reg);
         else if (left->creg) debug("creg %s ", left->creg);
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
         debug("[%-6s] ", to_string(curr->type));
         if (curr->declare)
         {
            stop(1, "I removed declare in intialize variable, this coniditon should never be true");
            // debug("declare [%s] PTR=[%d] ", curr->name, curr->ptr);
         }
         if (curr->name) debug("var %s PTR=[%d] ", curr->name, curr->ptr);
         if (curr->creg) debug("creg %s ", curr->creg);
         // else if(curr->type == FLOAT)
         // {
         //     curr->index = ++float_index;
         //     debug("value %f ", curr->Float.value);
         // }
         if (curr->type == CHARS && !curr->name)
            debug("value %s in STR%d ", curr->Chars.value, curr->index);
         else if (!curr->name && !curr->creg)
         {
            debug("value: "); print_value(curr);
         }
         //else check(1, "handle this case in generate ir\n", "");
         break;
      }
      case DOT:
      {
         debug("[%-6s] ", to_string(curr->type));
         debug("access [%s] in %k", right->name, left);
         break;
      }
      case JMP: debug("jmp to [%s] ", curr->name); break;
      case JNE: debug("jne to [%s] ", curr->name); break;
      case FCALL: debug("call [%s] ", curr->name); break;
      case BLOC: case FDEC: debug("[%s] bloc ", curr->name); break;
      case END_BLOC:  debug("[%s] endbloc ", curr->name); break;
      case STRUCT_CALL: debug("[%-6s] %s ", to_string(curr->type), curr->name); break;
      case RETURN: case CONTINUE: case BREAK: debug("[%s] ", to_string(curr->type)); break;
      default: debug(RED "print_ir:handle [%s]"RESET, to_string(curr->type)); break;
      }

      debug("space (%d)", curr->space);
      debug("\n");
   }
   debug("total instructions [%d]\n", i);
   debug(GREEN BOLD SPLIT RESET);
}

void print_ast(Node *head)
{
   debug(GREEN BOLD SPLIT RESET);
   debug(GREEN BOLD"PRINT AST:\n" RESET);
   for (int i = 0; !found_error && i < head->cpos; i++) debug("%n\n", head->children[i]);
   debug(GREEN BOLD SPLIT RESET);
}

// CLEAR MEMORY
void free_token(Token *token)
{
   free(token->name);
   free(token->creg);
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

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
// Tokenization - Convert source text to token stream
// Syntax Analysis - Build abstract syntax tree from tokens
void tokenize(char *filename)
{
   if (found_error) return;
#if DEBUG
   debug(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);
#endif
   char *input = open_file(filename);
   if (!input) return;

   struct { char *value; Type type; } specials[] = {
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
   bool inc_space = true;
   for (int i = 0; input[i] && !found_error; )
   {
      int s = i;
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            line++;
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
      inc_space = false;
      bool found = false;
      for (int j = 0; specials[j].value; j++)
      {
         if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
         {
            parse_token(NULL, 0, 0, specials[j].type, space, filename, line);
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
         parse_token(input, s, i, CHARS, space, filename, line);
         continue;
      }
      if (input[i] && input[i] == '\'')
      {
         i++;
         if (input[i] && input[i] != '\'') i++;
         check(input[i] != '\'', "Expected '\''");
         i++;
         parse_token(input, s + 1, i, CHAR, space, filename, line);
         continue;
      }
      if (isalpha(input[i]) || strchr("@$-_", input[i]))
      {
         while (input[i] && (isalnum(input[i]) || strchr("@$-_", input[i]))) i++;
         if (strncmp(input + s, "use ", 4) == 0)
         {
            while (isspace(input[i])) i++;
            s = i;
            while (input[i] && !isspace(input[i])) i++;
            char *use = allocate(i - s + 1, sizeof(char));
            strncpy(use, input + s, i - s);
            char *tmp = strjoin(use, ".w", NULL);
            free(use);
            use = tmp;
            tokenize(use);
            free(use);
         }
         else
            parse_token(input, s, i, ID, space, filename, line);
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

Node *expr() {return assign();}

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

// Function Declaration:
//    + left children: arguments
//    + children     : code block
Node *func_dec(Node *node)
{
   bool is_proto = find(PROTO, 0) != NULL;

   Token *typeName = find(DATA_TYPES, 0);
   if (typeName->type == ID)
   {
      typeName = get_struct(typeName->name);
      todo(1, "handle function return struct properly");
   }
   Token *fname = find(ID, 0);
   if (check(!typeName || !fname, "expected data type and identifier after func declaration"))
      return node;
   node->token->retType = typeName->type;
   node->token->is_proto = is_proto;
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
      add_child(node->left, curr);
      find(COMA, 0); // TODO: check this later
   }
   check((!found_error && last->type != RPAR), "expected ) after function declaration");
   check((!found_error && !find(DOTS, 0)), "Expected : after function declaration");

   Node *child = NULL;
   while (within_space(node->token->space)) child = add_child(node, expr());
   if (!is_proto)
   {
      if (node->token->retType != VOID)
         check(!child || child->token->type != RETURN, "expected return statment");
      else
      {
         Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
         ret->left = new_node(new_token(INT, node->token->space + TAB));
         add_child(node, ret);
      }
   }
   exit_scoop();
   return node;
}

// Function call:
//    + children: Parameters
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

// Function main:
//    + children: code bloc
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
      last = new_node(new_token(RETURN, node->token->space + TAB));
      last->left = new_node(new_token(INT, node->token->space + TAB));
      add_child(node, last);
   }
   exit_scoop();
   return node;
}

// Struct def Layout:
//    + children: attributes
Node *struct_def(Node *node)
{
   Token *name;
   if (check(!(name = find(ID, 0)), "expected identifier after struct definition")) return NULL;
   if (check(!find(DOTS, 0), "expected dots after struct definition")) return NULL;
   setName(node->token, NULL);
   node->token->Struct.name = strdup(name->name);
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

      if (attr->type == ID) // attribute is a struct
      {
         Token *st = get_struct(attr->name);
         if (check(!st, "Unkown data type [%s]\n", attr->name)) break;
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
      node->token->declare = true;
      return node;
   }
   else if (token->declare)
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
   else if (token->type == ID && token->name && find(LBRA, 0))
   {
      node = new_node(tokens[exe_pos - 1]); // LBRA
      node->token->type = ACCESS;


      node->left = new_node(token);
      node->right = expr(); // TODO: should be integer or ID;

      check(!find(RBRA, 0), "expected right bra");
      // debug("%n\n", node);
      // exit(1);
      return node;
   }
   return new_node(token);
}

// if Layout:
//    + left    : condition
//    + children: code bloc
//    + right   : elif/else
Node *if_node(Node *node)
{
   enter_scoop(node);

   node->left = expr();  // condition, TODO: check if it's boolean
   node->left->token->is_cond = true;
   node->left->token->space = node->token->space;
   node->right = new_node(new_token(CHILDREN, node->token->space));

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

// while Layout:
//    left     : condition
//    children : code bloc
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
      check(!node->token->declare, "must be variable declaration after ref");
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
   // else if((token = find(LBRA, 0)))
   // {
   //    if (tokens[exe_pos]->type != RPAR) node = expr();
   //    check(!find(RBRA, 0), "expected right bra");
   //    return node;
   // }
   else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
   return new_node(tokens[exe_pos]);
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
   if (!cond) return NULL;
   setName(cond, "endif");
   cond->index = inst->token->index;
   --bloc_index;

   Token *next = cond;
   // code bloc
   for (int i = 0; i < node->cpos && !found_error; i++) generate_ir(node->children[i]);

   Inst *end = NULL;
   if (node->right->cpos)
   {
      end = new_inst(new_token(JMP, node->token->space + TAB));
      setName(end->token, "endif");
      end->token->index = node->token->index;
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
         char *name = strdup(next->name);
         int index = next->index;
         {
            setName(next, "elif");
            next->index = curr->token->index;
            next = copy_token(next);
         }
         new_inst(curr->token); // elif bloc
         setName(curr->left->token, name);
         free(name);
         generate_ir(curr->left); // elif condition, TODO: check is boolean
         --bloc_index;
         curr->left->token->index = index;
         next = curr->left->token;
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         end = new_inst(new_token(JMP, node->token->space + TAB));
         setName(end->token, "endif");
         end->token->index = node->token->index;
      }
      else if (curr->token->type == ELSE)
      {
         curr->token->index = ++bloc_index;
         curr->token->type = BLOC;
         setName(curr->token, "else");
         new_inst(curr->token);
         {
            setName(next, "else");
            next->index = curr->token->index;
            next = copy_token(next);
         }
         for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
         break;
      }

   }
   Token *new = new_token(BLOC, node->token->space);
   setName(new, "endif");
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
      Token *next;
      switch (curr->token->type)
      {
      case BREAK:
         next = copy_token(node->left->token);
         next->type = JMP;
         setName(next, "endwhile");
         new_inst(next); // jmp back to while loop
         break;
      case CONTINUE:
         next = copy_token(node->token);
         next->type = JMP;
         setName(next, "while");
         new_inst(next); // jmp back to while loop
         break;
      default:
         generate_ir(curr); break;
      }
   }

   Token *next = copy_token(node->token);
   next->type = JMP;
   setName(next, "while");
   next->space += TAB;
   new_inst(next); // jmp back to while loop

   next = copy_token(node->token);
   next->type = BLOC;
   setName(next, "endwhile");
   new_inst(next); // end while bloc
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
         Node *tmp = new_node(new_token(ASSIGN, node->token->space));
         tmp->token->ir_reg = attr->ir_reg;
         tmp->left = new_node(attr);
         tmp->right = new_node(new_token(DEFAULT, attr->space));
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
   node->token->declare = false;
   new_variable(node->token);
   Node *tmp = new_node(new_token(ASSIGN, node->token->space));
   tmp->token->ir_reg = node->token->ir_reg;
   tmp->left = copy_node(node);
   tmp->right = new_node(src);
   to_default(src, tmp->left->token->type);
   generate_ir(tmp);
   free_node(tmp);
   return node->token;
}

void set_func_dec_regs(Token *child, int *ptr, bool is_proto)
{
   Token *src = new_token(DEFAULT, child->space);
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
         case LONG: case CHARS: case PTR: setReg(src, rregs[r]); break;
         case FLOAT: setReg(src, rregs[r]); break; // TODO: to be checked
         case INT: case CHAR: case BOOL: setReg(src, eregs[r]); break;
         case STRUCT_CALL:
         {
            if (!child->is_attr) new_variable(child);
            for (int j = 0; j < child->Struct.pos; j++)
            {
               set_func_dec_regs(child->Struct.attrs[j],  &r, is_proto);
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
   if (is_proto);
   else if (child->type != STRUCT_CALL || child->is_ref)
   {
      if (src->is_ref) child->has_ref = true;
      Node *child_node = new_node(child);
      inialize_variable(child_node, src);
      free_node(child_node);
   }
   *ptr = r;
}

void set_remove(Node *node)
{
   for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
   {
      Node *child = node->children[i];
      child->token->remove = true;
      set_remove(child);
   }
}

Token *func_dec_ir(Node *node)
{
   new_function(node);
   enter_scoop(node);
   int tmp_ptr = ptr;
   ptr = 0;

   Inst* inst = NULL;
   if (!node->token->is_proto) inst = new_inst(node->token);

   // parameters
   Node *curr = node->left;
   for (int i = 0, r = 0; curr && i < curr->cpos && !found_error; i++)
   {
      Node *child = curr->children[i];
      set_func_dec_regs(child->token, &r, node->token->is_proto);
   }

   if (node->token->is_proto) set_remove(node);
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
      node->token->ptr = ptr;
      ptr = tmp_ptr;
   }
   exit_scoop();
   if (!node->token->is_proto) return inst->token;
   return NULL;
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
         case LONG: case CHARS: setReg(dist, rregs[r]); break;
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
      Node *assign = new_node(new_token(ASSIGN, node->token->space));
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

      Node *assign = new_node(new_token(ASSIGN, node->token->space));
      Token *_register = new_token(CHARS, fcall->token->space + TAB);
      _register->creg = strdup("rdi");
      Token *varg = new_token(CHARS, fcall->token->space + TAB);
      varg->index = ++str_index;
      varg->Chars.value = strdup("\"");

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
         Token *src = new_token(INT, var->space);

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
         assign = new_node(new_token(ASSIGN, node->token->space));
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


   switch (node->token->type)
   {
   case ASSIGN:
   {
      // if (left->type == STRUCT_CALL)
      // {
      //    debug(">> %k\n", left);
      //    debug(">> %k\n", right);
      //    //    stop(1, "found");
      // }
      node->token->ir_reg = left->ir_reg;
      node->token->retType = getRetType(node);
      if (left->is_ref) // ir_reg, ptr
      {
         if (right->is_ref) // ir_reg, ptr
         {
            if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
            if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
            else node->token->assign_type = REF_HOLD_REF;
         }
         else if (right->ptr || right->creg) // ptr
         {
            if (left->has_ref) node->token->assign_type = REF_ID;
            else node->token->assign_type = REF_HOLD_ID;
         }
         else // ir_reg, value
         {
            debug("%n\n", node);
            // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
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
      else if (left->type == STRUCT_CALL)
      {
         debug(">> %k\n", left);
         debug(">> %k\n", right);
         // TODO: check compatibility
         for (int i = 0; i < left->Struct.pos; i++)
         {
            Node *tmp = new_node(new_token(ASSIGN, node->token->space));
            tmp->left = new_node(left->Struct.attrs[i]);
            tmp->right = new_node(right->Struct.attrs[i]);
            op_ir(tmp);
            free_node(tmp);
         }
         return NULL;
         // exit(1);
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
   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

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
   case FLOAT: case LONG: case CHARS: case PTR:
   {
      if (node->token->declare)
      {
         if (node->token->type == STRUCT_CALL) return inialize_struct(node);
         return inialize_variable(node, new_token(DEFAULT, node->token->space));
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
   case ACCESS:
   {
      debug("%n\n", node);
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      // TODO: check if right is a number
      // TODO: check if left is aan iterable data type

      Inst *inst = new_inst(node->token);
      inst->token->creg = strdup("rax");
      inst->token->is_ref = true;
      inst->token->has_ref = true;
      switch (left->type)
      {
      case CHARS:
      {
         inst->token->offset = sizeof(char);
         inst->token->retType = CHAR;
         break;
      }
      case INT:
      {
         inst->token->offset = sizeof(int);
         inst->token->retType = INT;
         break;
      }
      default: todo(1, "handle this case");
      }
      inst->left = left;
      inst->right = right;
      return node->token;
   }
   case STRUCT_DEF: return node->token;
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
      switch (curr->type)
      {
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT: case LONG: case PTR:
      {
         // if (curr->declare)
         // {
         //    pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         // }
         break;
      }
      case ADD_ASSIGN:
      {
         asm_space(curr->space);
         // TODO: check this
         char *inst = "add";
         pasm("%i%a, %v", inst, left, right);
         if (left->name) {pasm(" ;// add_assign [%s]", left->name); }
         break;
      }
      case ASSIGN:
      {
         // char *inst = left->type == FLOAT ? "movss " : "mov ";
         // debug("LEFT : %k\n", left);
         // debug("RIGHT: %k\n", right);
         // if (right->type == DEFAULT)
         // {
         //    pasm("%i%a, 0 ;// declare [%s]", "mov", curr, curr->name); asm_space(curr->space);
         //    break;
         // }
         asm_space(curr->space);
         switch (curr->assign_type)
         {
         case ID_VAL:
         {
            // todo(1, "handle this case");
            // left ptr, creg
            // right value, creg
            if (left->ptr)
            {
               if (right->creg)
               {
                  pasm("%i%a, %r", "mov", left, right); // left ptr, right creg
               }
               else // left ptr, right value
               {
                  if (right->type == CHARS)
                  {
                     pasm("%i%ra, .STR%d[rip]", "lea", left, right->index);
                     asm_space(curr->space);
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
               if (right->creg) // left creg, right creg
               {
                  todo(1, "handle this case", "");
               }
               else // left creg, right value
               {
                  if (right->type == CHARS)
                  {
                     //todo(1, "handle this case", "");
                     // I used %r for this:
                     // chars planet = "Mars"
                     // strcmp(planet, "Earth")
                     pasm("%i%r, .STR%d[rip]", "lea", left, right->index);
                  }
                  else
                  {
                     // todo(1, "handle this case", "");
                     pasm("%i%r, %v", "mov", left, right, left->name);
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
               pasm("%i%r, %a", "mov", left, right);
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
            if (left->creg)
            {
               // todo(1, "handle this case");
               pasm("%i%r, -%d[rbp]", "lea", left,  right->ptr);
            }
            else if (left->ptr)
            {
               // debug("left : %k\n", left);
               // debug("right: %k\n", right);
               // todo(1, "handle this case");
               pasm("%irax, -%d[rbp]", "lea", right->ptr); asm_space(curr->space);
               pasm("%iQWORD PTR -%d[rbp], rax", "mov", left->ptr);
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
                  pasm("%i%ra, %a", "mov", right, right); asm_space(curr->space);
                  pasm("%i%ra, %ma", "mov", left, left);  asm_space(curr->space);
                  pasm("%i%a, %ra", "mov", left, left);
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
                  // todo(1, "handle this case");
                  pasm("%irax, %a", "mov", right); asm_space(curr->space);
                  pasm("%i%r, %ma", "mov", left, left);
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
            if (left->ptr && right->ptr)
            {
               pasm("%irax, %a", "mov", left, right); asm_space(curr->space);
               left->is_ref = false;
               pasm("%i%rb, %a", "mov", left, right); asm_space(curr->space);
               left->is_ref = true;
               pasm("%i%ma, %rb", "mov", left, right);
            }
            else if (left->creg && right->ptr)
            {
               todo(1, "handle this case");
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
            if (left->ptr)
            {
               if (right->creg)
               {
                  pasm("%i%a, %r", "mov", left, right);
               }
               else
               {
                  pasm("%irax, %a", "mov", left, left); asm_space(curr->space);
                  pasm("%i%ma, %v", "mov", left,  right);
               }
            }
            else if (left->creg)
            {
               if (right->creg)
               {
                  todo(1, "handle this case");
               }
               else
               {
                  pasm("%i%ma, %v", "mov", left,  right);
                  // todo(1, "handle this case");
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
//            todo(1, "handle this case");

            // left has_ref, right has_ref
            // left ptr (has_refrence), creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  // todo(1, "handle this case");
                  pasm("%i%ra, %a", "mov", left, right); asm_space(curr->space);
                  left->is_ref = false;
                  pasm("%i%ra, %ma", "mov", left, right); asm_space(curr->space);
                  left->is_ref = true;
                  pasm("%i%rb, %a", "mov", left, left); asm_space(curr->space);
                  right->is_ref = false;
                  pasm("%i%mb, %ra", "mov", left, right);
                  right->is_ref = true;
               }
               else if (right->creg)
               {
                  // todo(1, "handle this case");
                  // pasm("%i%ra, %r", "mov", left, right); asm_space(curr->space);
                  pasm("%i%a, %r", "mov", left, right);
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
         case REF_HOLD_REF:
         {
            // left ptr (don't have refrence), creg
            // right ptr, creg
            if (left->ptr)
            {
               if (right->ptr)
               {
                  todo(1, "handle this case");
               }
               else if (right->creg)
               {
                  pasm("%iQWORD PTR -%d[rbp], %ri", "mov", left->ptr, right);
                  // todo(1, "handle this case");
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
         default:
         {
            todo(1, "handle this case [%d]", curr->assign_type);
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
         asm_space(curr->space);
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
            asm_space(curr->space);
            pasm("%i", "cmp");
            if (left->ptr) pasm("%a", left);
            else if (left->creg) pasm("%ra", left);
            else if (!left->creg) pasm("%v", left);

            if (right->ptr) pasm(", %a", right);
            else if (right->creg) pasm(", %ra", right);
            else if (!right->creg) pasm(", %v", right);
            asm_space(curr->space);
            pasm("%i .%s%d", inst, curr->name ? curr->name : "(null)", curr->index);
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
            asm_space(curr->space);
            pasm("%i%ra", inst, curr);
         }
         break;
      }
      case ACCESS:
      {
         asm_space(curr->space); pasm("%irax, %a", "mov", left);
         asm_space(curr->space); pasm("%irax, %d", "add", curr->offset * right->Int.value);
         break;
      }
      case FDEC:
      {
         asm_space(curr->space); pasm("%s:", curr->name);
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
         asm_space(curr->space);
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
      case JE:
      {
         asm_space(curr->space);
         pasm("%ial, 1", "cmp");
         asm_space(curr->space);
         pasm("%i.%s%d", "je", curr->name, curr->index);
         break;
      }
      case JNE:
      {
         asm_space(curr->space);
         pasm("%ial, 1", "cmp");
         asm_space(curr->space);
         pasm("%i.%s%d", "jne", curr->name, curr->index);
         break;
      }
      case JMP:
      {
         asm_space(curr->space);
         pasm("%i.%s%d", "jmp", curr->name, curr->index);
         break;
      }
      case FCALL:
      {
         asm_space(curr->space);
         pasm("%i%s", "call", curr->name);
         break;
      }
      case BLOC:
      {
         asm_space(curr->space);
         pasm(".%s%d:", (curr->name ? curr->name : "(null)"), curr->index);
         break;
      }
      case END_BLOC:
      {
         asm_space(curr->space);
         pasm(".end%s:", curr->name);
         asm_space(curr->space);
         break;
      }
      case STRUCT_CALL: break;
      default: check(1, "handle this case (%s)\n", to_string(curr->type)); break;
      }
   }
   finalize();
}

void compile(char *filename)
{
   tokenize(filename);
   new_token(END, -1);
   if (found_error) return;
   Node *global = new_node(new_token(ID, -TAB - 1));
   setName(global->token, ".global");
   enter_scoop(global);

#if DEBUG
   debug(GREEN BOLD"AST:\n" RESET);
#endif
   while (tokens[exe_pos]->type != END && !found_error) add_child(global, expr());
   print_ast(global);

   if (found_error) return;
#if IR
   debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
   for (int i = 0; !found_error && i < global->cpos; i++) generate_ir(global->children[i]);
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
   generate_asm(filename);
#endif

   free_node(global);
}

int main(int argc, char **argv)
{
   check(argc < 2, "Invalid arguments");

   for (int i = 1; i < argc; i++)
   {
      compile(argv[i]);
      free_memory();
   }
}