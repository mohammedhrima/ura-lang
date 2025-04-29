#include "./include/header.h"

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
#if DEBUG_NEW_TOKEN
Token* new_token_(char *filename, int line, char *input, int s, int e, Type type, int space)
#else
Token* new_token(char *input, int s, int e, Type type, int space)
#endif
{
#if DEBUG_NEW_TOKEN
   debug("%s:%d: ", filename, line);
#endif
   Token *new = allocate(1, sizeof(Token));
   new->type = type;
   new->space = ((space + TAB / 2) / TAB) * TAB;
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

      struct { char *name; Type type; } dataTypes [] = { {"int", INT}, {"bool", BOOL}, {"chars", CHARS},
         {"char", CHAR}, {"float", FLOAT}, {"void", VOID}, {"long", LONG}, {"_ptr_", PTR}, {0, 0}
      };
      for (i = 0; dataTypes[i].name; i++)
      {
         if (strcmp(dataTypes[i].name, new->name) == 0)
         {
            setName(new, NULL);
            new->type = dataTypes[i].type;
            new->is_data_type = true;
            break;
         }
      }
      if (dataTypes[i].name) break;

      struct { char *name; Type type; } keywords[] = {{"if", IF}, {"elif", ELIF}, {"else", ELSE},
         {"while", WHILE}, {"func", FDEC}, {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
         {"ref", REF}, {"and", AND}, {"or", OR}, {"struct", STRUCT_DEF}, {"is", EQUAL}, {0, 0}
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
#if DEBUG
   // debug("token: %k\n", new);
#endif
   add_token(new);
   return new;
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
   static int structs_ids;
   token->Struct.id = (++structs_ids);
#if DEBUG
   debug(CYAN "new struct [%s] id [%d] in scoop %k\n" RESET, token->name, token->Struct.id, scoop->token);
#endif
   for (int i = 0; i < scoop->spos; i++)
   {
      Token *curr = scoop->structs[i];
      if (strcmp(curr->name, token->name) == 0) check(1, "Redefinition of %s\n", token->name);
   }
   add_struct(scoop, token);
   return token;
}

Token *get_struct_by_id(int id)
{
#if DEBUG
   debug("get struct with id [%d] from scoop %k\n", id, scoop->token);
#endif
   for (int j = scoopPos; j >= 0; j--)
   {
      Node *node = Gscoop[j];
#if DEBUG
      debug("[%d] scoop [%s] has %d structs\n", j, scoop->token->name, node->spos);
#endif
      for (int i = 0; i < node->spos; i++)
      {
         // debug(GREEN"struct has [%d]\n"RESET, node->structs[i]->Struct.id);
         if (node->structs[i]->Struct.id == id)
            return copy_token(node->structs[i]);
      }
   }
   // check(1, "%s not found\n", name);
   return NULL;
}


Token *get_struct(char *name)
{
#if DEBUG
   debug(CYAN "get struct [%s] from scoop %k\n"RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j >= 0; j--)
   {
      Node *node = Gscoop[j];
#if DEBUG
      debug("[%d] scoop [%s] has %d structs\n", j, scoop->token->name, node->spos);
#endif
      for (int i = 0; i < node->spos; i++)
         if (strcmp(node->structs[i]->name, name) == 0)
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
      child->isattr = true;
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

         attr = get_struct_by_id(attr->Struct.id);
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
   new_inst(token);
   add_variable(scoop, token);
   return token;
}

Token *get_variable(char *name)
{
#if DEBUG
   debug(CYAN "get variable [%s] from scoop %k\n" RESET, name, scoop->token);
#endif
   for (int j = scoopPos; j >= 0; j--)
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
   add_function(scoop, node);
   return node;
}

Node *get_function(char *name)
{
#if DEBUG
   debug("get_func %s in scoop %k\n", name, scoop->token);
#endif
   for (int j = scoopPos; j >= 0; j--)
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

   for (int i = 0; OrgInsts[i]; i++)
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

   if (token->type == STRUCT_CALL)
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
   }
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
   switch (type)
   {
   case CHARS:
   {
      token->Chars.value = strdup("\"\"");
      break;
   }
   case CHAR:
   {
      token->Char.value = 0;
      break;
   }
   case LONG:
   {
      token->Long.value = 0;
      break;
   }
   case INT: break;
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
         if (token->type == ASSIGN && !insts[i]->left->is_ref)
         {
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
               default: check(1, "Unknown type [%s]\n", to_string(token->type)); break;
               }
         }
         else if (strncmp(fmt + i, "ma", 2) == 0)
         {
            i += 2;
            Token *token = va_arg(args, Token *);
            // if (token->is_ref) fprintf(asm_fd, "QWORD PTR -%d[rbp]", token->ptr);
            // else
            switch (token->type)
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

void open_file(char *filename)
{
   if (found_error) return;
   struct _IO_FILE *file = fopen(filename, "r");
   if (check(!file, "openning %s", filename)) return;
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   input = allocate((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
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
      [FDEC] = "F_DEC", [FCALL] = "F_CALL", [INT] = "INT", [VOID] = "VOID", [CHARS] = "CHARS",
      [CHAR] = "CHAR", [BOOL] = "BOOL", [FLOAT] = "FLOAT", [STRUCT_CALL] = "ST_CALL",
      [STRUCT_DEF] = "ST_DEF", [ID] = "ID", [END_BLOC] = "END_BLOC", [BLOC] = "BLOC",
      [JNE] = "JNE", [JE] = "JE", [JMP] = "JMP", [LBRA] = "L_BRA", [RBRA] = "R_BRA",
      [END] = "END", [CHILDREN] = "CHILDREN", [TMP] = "TMP", [LONG] = "LONG", [PTR] = "PTR",
      [REF_ID] = "REF_ID", [REF_HOLD_ID] = "REF_HOLD_ID", [REF_VAL] = "REF_VAL",
      [REF_HOLD_REF] = "REF_HOLD_REF", [REF_REF] = "REF_REF", [ID_ID] = "ID_ID",
      [ID_REF] = "ID_REF", [ID_VAL] = "ID_VAL", [DEFAULT] = "DEFAULT",
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

void add_token(Token *token)
{
   static int pos;
   static int len;
   if (len == 0)
   {
      len = 10;
      tokens = allocate(len, sizeof(Token *));
   }
   else if (pos + 1 == len)
   {
      Token **tmp = allocate(len * 2, sizeof(Token *));
      memcpy(tmp, tokens, len * sizeof(Token *));
      free(tokens);
      tokens = tmp;
      len *= 2;
   }
   tokens[pos++] = token;
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
   case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:
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
   case STRUCT_CALL:
   case STRUCT_DEF:
   {
      res += debug("name [%s] ", token->name);
      res += debug("struct_id [%d] ", token->Struct.id);
      res += debug("space [%d] ", token->space);
      res += debug("attributes:\n");
      for (int i = 0; i < token->Struct.pos; i++)
      {
         Token *attr = token->Struct.attrs[i];
#if 1

         for (int j = 0; !TESTING && j < attr->space; ) j += debug(" ");
         res += ptoken(attr) + debug(", offset [%d] PTR [%d]\n", attr->offset, attr->ptr);
#else
         res += debug("%s %t [%d], ", attr->name, attr->type, attr->ptr);
#endif
      }
      return res;
      break;
   }
   case FCALL:
   case FDEC: case ID: res += debug("name [%s] ", token->name); break;
   default: break;
   }
   if (token->ptr) res += debug("PTR [%d] ", token->ptr);
   if (token->ir_reg) res += debug("ir_reg [%d] ", token->ir_reg);
   if (token->is_ref) debug("ref ");
   if (token->has_ref) debug("has-ref ");
   if (token->remove) res += debug("[remove] ");
   if (token->retType) res += debug("ret [%t] ", token->retType);
   res += debug("space [%d] ", token->space);
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
      case ADD_ASSIGN:
      case ASSIGN:
      {
         debug("[%-6s] [%s] ", to_string(curr->type), (curr->assign_type ? to_string(curr->assign_type) : ""));
         if (left->creg) debug("r%.2d (%s) = ", left->ir_reg, left->creg);
         else debug("r%.2d (%s) = ", left->ir_reg, left->name);

         if (right->ir_reg) debug("r%.2d (%s) ", right->ir_reg, right->name ? right->name : "");
         else if (right->creg) debug("[%s] ", right->creg);
         else print_value(right);
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
      case INT: case BOOL: case CHARS: case CHAR:
      {
         debug("[%-6s] ", to_string(curr->type));
         if (curr->is_data_type)
         {
            stop(1, "I removed is_data_type in intialize variable, this coniditon should never be true");
            // debug("is_data_type [%s] PTR=[%d] ", curr->name, curr->ptr);
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

void print_ast()
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
   free(input);
   for (int i = 0; tokens && tokens[i]; i++) free_token(tokens[i]);
   free_node(head);
}

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
         debug(RED"found struct %k\n"RESET, data_type);
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
      Token *attr = find(LONG, INT, CHARS, CHAR, FLOAT, BOOL, ID, 0);
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
      id->isattr = true;
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

Token* inialize_variable(Node *node, Token *src)
{
   node->token->is_data_type = false;
   if (node->token->type == STRUCT_CALL)
   {
      // debug(RED "initialize : offset[%d] %k\n"RESET, node->token->offset,  node->token);
      // pnode(node, NULL, 0);
      if (!node->token->isattr) new_variable(node->token);
      for (int i = 0; i < node->token->Struct.pos; i++)
      {
         Token *attr = node->token->Struct.attrs[i];
         if (attr->type == STRUCT_CALL)
         {
            Node *tmp = new_node(attr);
            attr->offset += node->token->offset;
            inialize_variable(tmp, NULL);
            free_node(tmp);
         }
         else
         {
            attr->ptr = ptr + node->token->offset - attr->offset;
            debug(CYAN"%k\n"RESET, attr);

            Node *tmp = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
            tmp->token->ir_reg = attr->ir_reg;
            tmp->left = new_node(attr);
            tmp->right = new_node(new_token(NULL, 0, 0, DEFAULT, attr->space));
            to_default(tmp->right->token, tmp->left->token->type);
            generate_ir(tmp);
            free_node(tmp);
         }
      }
      if (!node->token->isattr) inc_ptr(node->token->offset);
   }
   else
   {
      new_variable(node->token);
      Node *tmp = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
      tmp->token->ir_reg = node->token->ir_reg;
      tmp->left = copy_node(node);
      tmp->right = new_node(src);
      to_default(src, tmp->left->token->type);
      generate_ir(tmp);
      free_node(tmp);
   }
   return node->token;
}

void set_func_dec_regs(Token *child, int *ptr)
{
   debug("set %k\n", child);
   Token *src = new_token(NULL, 0, 0, DEFAULT, child->space);
   int i = *ptr;
   if (i < (int)(sizeof(eregs) / sizeof(eregs[0])))
   {
      if (child->is_ref)
      {
         if (child->type == STRUCT_DEF)
         {
            ptoken(child);
            //Token *tmp = get_struct_by_id()
            todo(1, "found");
         }
         setReg(src, rregs[i]);
      }
      else
      {
         // TODO: add other data type and math operations
         switch (child->type)
         {
         case CHARS: setReg(src, rregs[i]); break;
         case INT:   setReg(src, eregs[i]); break;
         case CHAR:  setReg(src, eregs[i]); break;
         case FLOAT: setReg(src, rregs[i]); break; // TODO: to be checked
         case BOOL:  setReg(src, eregs[i]); break;
         case STRUCT_CALL:
         {
            for (int j = 0; j < child->Struct.pos; j++)
            {
               i += j;
               set_func_dec_regs(child->Struct.attrs[j],  &i);
               debug(RED"%k\n"RESET, child->Struct.attrs[j]);
            }
            break;
         }
         default: todo(1, "set ir_reg for %s", to_string(child->type));
         };
      }
   }
   else
   {
      // TODO:
      todo(1, "implement assigning function argument using PTR");
   }
   // if(!child->isattr)
   {
      if (child->is_ref /*&& !child->token->has_ref*/)
      {
         src->is_ref = true;
         src->has_ref = true;
      }
      child->has_ref = true;
      Node *child_node = new_node(child);
      inialize_variable(child_node, src);
      free_node(child_node);
   }
   *ptr = i;
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
      set_func_dec_regs(child->token, &i);
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

      for (int i = 0; !found_error && i < fcall->cpos && i < fdec->cpos; i++)
      {
         Node *darg = fdec->children[i];
         Node *carg = fcall->children[i]; // will always be ID
#if DEBUG
         // debug(RED SPLIT);
         // debug("dec : %n", darg);
         // debug("call: %n", carg);
         // // pnode(scoop, "scoop", 0);
         // debug(SPLIT RESET);
#endif
         Token *var = generate_ir(carg);

         if (check(var->type == ID, "Indeclared variable %s", carg->token->name)) break;
         Token *src = copy_token(darg->token);

         if (i < (int)(sizeof(eregs) / sizeof(eregs[0])))
         {
            // added because unfction declaration params do have ptrs
            src->ptr = 0;
            if (src->is_ref) setReg(src, rregs[i]);
            else
            {
               // TODO: add other data type and math operations
               switch (src->type)
               {
               case CHARS: setReg(src, rregs[i]); break;
               case INT:   setReg(src, eregs[i]); break;
               case CHAR:  setReg(src, eregs[i]); break;
               case FLOAT: setReg(src, rregs[i]); break; // TODO: to be checked
               case BOOL:  setReg(src, eregs[i]); break;
               default: todo(1, "set ir_reg for %s", to_string(src->type));
               };
            }
         }
         else
         {
            // TODO:
            todo(1, "implement assigning function argument using PTR");
         }
         if (src->is_ref)
         {
            src->is_ref = true;
            src->has_ref = false;
         }
         new_inst(src);
         Node *assign = new_node(new_token(NULL, 0, 0, ASSIGN, node->token->space));
         assign->left = new_node(src);
         assign->right = new_node(var);
         assign->token->ir_reg = src->ir_reg;

         debug(RED);
         pnode(assign, NULL, 0);
         debug(RESET);

         generate_ir(assign);
         free_node(assign);
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
      node->token->ir_reg = left->ir_reg;
      if (!node->token->ir_reg || !left->ir_reg)
      {
         pnode(node, NULL, 0);
         debug(">> %k\n", left);
         debug(">> %d\n", left->ir_reg);
         // todo(1, "tmp condition");
      }
      node->token->retType = getRetType(node);
      if (left->is_ref) // ir_reg, ptr
      {
         if (right->is_ref) // ir_reg, ptr
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
         if (node->token->type == STRUCT_CALL) return inialize_variable(node, NULL);
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
   config();
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