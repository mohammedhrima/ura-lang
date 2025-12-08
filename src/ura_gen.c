#include "./ura_header.h"

Token *func_dec_ir(Node *node)
{
   // already has retType
   new_function(node);
   enter_scoop(node);
   // int tmp_ptr = ptr;
   // ptr = 0;

   Inst* inst = NULL;
   // if (!node->token->is_proto)
   inst = new_inst(node->token);

   // parameters
   Node **params = (node->left ? node->left->children : NULL);
   Token *token = node->token;
   for (int i = 0; params && i < node->left->cpos && !found_error; i++)
   {
      Node *child = params[i];
      generate_ir(child);
      if (token->Fdec.args == NULL)
      {
         token->Fdec.size = 10;
         token->Fdec.args = allocate(token->Fdec.size, sizeof(Token*));
      }
      else if (token->Fdec.pos + 1 == token->Fdec.size)
      {
         Token **tmp = allocate(token->Fdec.size *= 2, sizeof(Token*));
         memcpy(tmp, token->Fdec.args, token->Fdec.pos * sizeof(Token*));
         free(token->Fdec.args);
         token->Fdec.args = tmp;
      }
      child->token->Param.index = i;
      child->token->Param.func_ptr = node->token;
      child->token->is_param = true;
      child->token->is_declare = false;
      token->Fdec.args[token->Fdec.pos++] = child->token;
   }

   // if (node->token->is_proto) set_remove(node);
   // code bloc
   for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
   {
      Node *child = node->children[i];
      generate_ir(child);
   }

   // if (!node->token->is_proto)
   {
      // TODO: if RETURN not found add it
      Token *new = new_token(END_BLOC, node->token->space);
      setName(new, node->token->name);
      new_inst(new);
   }
   exit_scoop();
   // if (!node->token->is_proto)
   return inst->token;
}

Token *func_call_ir(Node *node)
{
   Inst* inst = NULL;
   // static Node *printf_func = NULL;

   if (strcmp(node->token->name, "output") == 0)
   {
#if 1
      todo(1, "handle this\n");
#else
      // if (!printf_func)
      // {

      // }

      node->token->Fcall.ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID

         Token *src = generate_ir(carg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;

         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      inst = new_inst(node->token);
#endif
   }
   else if (strcmp(node->token->name, "stack") == 0)
   {
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;

      node->token->retType = PTR;
      node->token->type = STACK;

      Node *call_args = node;

      for (int i = 0; !found_error && i < call_args->cpos; i++)
      {
         Node *carg = call_args->children[i]; // will always be ID
         Token *src = generate_ir(carg);
         if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
         node->token->Fcall.args[i] = src;
         src->space = node->token->space;
      }
      inst = new_inst(node->token);
   }
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      node->token->Fcall.func_ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;

      func = copy_node(func);
      node->token->retType = func->token->retType;
      node->token->is_variadic = func->token->is_variadic;

      Node *call_args = node;
      Node *dec_args = func->left;

      if (check(call_args->cpos != dec_args->cpos && !node->token->is_variadic,
                "Incompatible number of arguments %s", func->token->name))
         return NULL;

      for (int i = 0; !found_error && i < call_args->cpos; i++)
      {
         Node *carg = call_args->children[i]; // will always be ID
         Token *src = generate_ir(carg);

         if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
         if (i < dec_args->cpos)
         {
            Node *darg = dec_args->children[i];
            Token *dist = darg->token;

            if (check(!compatible(src, dist), "Incompatible type arg %s", func->token->name)) break;
         }

         // src->is_param = false;
         node->token->Fcall.args[i] = src;
         // Token *dist = copy_token(darg->token);
         // set_func_call_regs(&r, src, dist, node);
      }
      free_node(func);
      inst = new_inst(node->token);
   }
   return inst->token;
}

Token *if_ir(Node *node)
{
   enter_scoop(node);
   Node **children = node->right->children;
   int cpos = node->right->cpos;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (check(!cond || cond->retType != BOOL, "expected condition that return bool")) return NULL;

   // APPEND BLOC
   Token *start = copy_token(node->token);
   Inst *bloc = new_inst(copy_token(start));
   bloc->token->type = APPEND_BLOC;
   bloc->left = start;

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Inst *bloc = new_inst(copy_token(children[i]->token));
      bloc->token->type = APPEND_BLOC;
      bloc->left = children[i]->token;
   }

   // BUILD CONDITION
   // cond ? go to left : go to right
   Inst *inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = start;
   inst->right = children[0]->token;

   // SET POSITION start
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = start;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = children[cpos - 1]->token;

   // SET POSITION next

   for (int i = 0; i < cpos && !found_error; i++)
   {
      Node *curr = children[i];
      inst = new_inst(copy_token(node->token));
      inst->token->type = SET_POS;
      inst->left = children[i]->token;
      switch (curr->token->type)
      {
      case ELSE:
      {
         enter_scoop(curr);
         for (int j = 0; j < curr->cpos; j++)
            generate_ir(curr->children[j]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(curr->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case ELIF:
      {
         enter_scoop(curr);
         Token *cond = generate_ir(curr->left); // TODO: check if it's boolean
         if (!cond) return NULL;
         i++;

         // BUILD CONDITION
         // cond ? go to left : go to right
         Inst *inst = new_inst(curr->token);
         inst->token->type = BUILD_COND;
         inst->token->Statement.ptr = cond;
         inst->left = children[i]->token;
         inst->right = children[i + 1]->token;


         // SET POSITION next
         inst = new_inst(copy_token(node->token));
         inst->token->type = SET_POS;
         inst->left = children[i]->token;

         for (int i = 0; i < curr->cpos && !found_error; i++)
            generate_ir(curr->children[i]);

         // BUILD BR to end (jmp to end if condition is true)
         inst = new_inst(copy_token(node->token));
         inst->token->type = BUILD_BR;
         inst->left = children[cpos - 1]->token;
         exit_scoop();
         break;
      }
      case END_IF: break;
      default: break;
      }
   }
   exit_scoop();
   return NULL;
}

Token *while_ir(Node *node)
{
   enter_scoop(node);
   // APPEND BLOC
   Token *loop_cond = copy_token(node->token);
   setName(loop_cond, "while");
   Inst *bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_cond;

   Token *loop_body = copy_token(node->token);
   setName(loop_body, "while_bloc");
   bloc = new_inst(copy_token(node->token));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_body;

   Token *loop_end = copy_token(node->token);
   setName(loop_end, "end_while");
   bloc = new_inst(copy_token(loop_end));
   bloc->token->type = APPEND_BLOC;
   bloc->left = loop_end;

   node->token->Statement.start = loop_cond;
   node->token->Statement.end = loop_end;

   // BUILD BR
   Inst *inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_cond;

   // CONDITION
   Token *cond = generate_ir(node->left); // TODO: check if it's boolean
   if (!cond) return NULL;

   // BUILD CONDITION
   // cond ? go to left : go to right
   inst = new_inst(node->token);
   inst->token->type = BUILD_COND;
   inst->token->Statement.ptr = cond;
   inst->left = loop_body;
   inst->right = loop_end;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_body;

   // children code
   for (int i = 0; i < node->cpos && !found_error; i++)
      generate_ir(node->children[i]);

   // BUILD BR to end (jmp to end if condition is true)
   inst = new_inst(copy_token(node->token));
   inst->token->type = BUILD_BR;
   inst->left = loop_cond;

   // SET POSITION
   inst = new_inst(copy_token(node->token));
   inst->token->type = SET_POS;
   inst->left = loop_end;

   exit_scoop();
   return NULL;
}

Token *op_ir(Node *node)
{
   Token *left = generate_ir(node->left);
   Token *right = generate_ir(node->right);
   if (check(!left, "error in assignment, left is NULL"))
   {
      print("%n\n", node);
      return NULL;
   }
   if (check(!right, "error in assignment, right is NULL"))
   {
      print("%n\n", node);
      return NULL;
   }

   // TODO: fix the check later
   // check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
   //       to_string(node->token->type), to_string(left->type), to_string(right->type));
   switch (node->token->type)
   {
   case ASSIGN:
   {
      if (check(!compatible(left, right), "type mismatch"))
      {
         int s = left->pos;
         int e = right->pos;
         int l = left->line;

         int ts = s;
         for (; ts > 0 && tokens[ts - 1]->line == l; ts--);
         int te = e;
         for (; tokens[te]->type != END && tokens[te]->line == l; te++);

         int space = 0;
         bool add_space = true;

         while (ts < te)
         {
            Token *token = tokens[ts];
            int to_add = 0;
            switch (token->logType)
            {
            case SYMBOL:
            {
               to_add += print("%s ", token->logName);
               break;
            }
            case VALUE:
            {
               switch (token->type)
               {
               case INT: to_add += print("%d", token->Int.value); break;
               case SHORT: to_add += print("%d", token->Short.value); break;
               case LONG: to_add += print("%lld", token->Long.value); break;
               case FLOAT: to_add += print("%f", token->Float.value); break;
               case BOOL: to_add += print("%s", token->Bool.value ? "True" : "False"); break;
               case CHARS: to_add += print_escaped(token->Chars.value); break;
               case CHAR: to_add += print("%c", token->Char.value); break;
               default:
                  todo(1, "handle this case %s", to_string(token->type));
                  break;
               }
               break;
            }
            default:
               debug(RED"%k "RESET, token);
               todo(1, "handle this case");
               break;
            }
            if (token == node->token) add_space = false;
            if (add_space) space += to_add;
            ts++;
         }
         print("\n");
         for (int i = 0; i < space; i++) print(" ");
         print("^\n");


         // print("between %k and %k\n", left, right);
      }
      node->token->ir_reg = left->ir_reg;
      node->token->retType = left->retType;
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   {
      node->token->retType = left->retType;
      break;
   }
   case AND: case OR:
   {
      node->token->retType = BOOL;
      break;
   }
   case NOT_EQUAL: case EQUAL: case LESS:
   case MORE: case LESS_EQUAL: case MORE_EQUAL:
   {
      node->token->retType = BOOL;
      // node->token->index = ++bloc_index;
      break;
   }
   default: check(1, "handle [%s]", to_string(node->token->type)); break;
   }
   Inst *inst = new_inst(node->token);
   inst->left = left;
   inst->right = right;

   return node->token;
}

Token *struct_def_ir(Node *node)
{
   new_inst(node->token);
   return node->token;
}

Token *struct_call_ir(Node *node)
{
   // ALLOCATE STRUCT
   // ADD ATTRIBUTE
#if 0
   Inst *inst = NULL;
   Token **attrs = node->token->Struct.attrs;
   int attrs_size = node->token->Struct.pos;
   // node->token->Struct.attrs = NULL;
   // node->token->Struct.pos = 0;

   // set struct body
   // Token *body = copy_token(node->token);
   // body->type = STRUCT_BODY;
   // inst = new_inst(body);
   // inst->left = node->token;

   // allocate struct
   // Token *alloca_st = copy_token(node->token);
   // alloca_st->type = STRUCT_ALLOC;
   // inst = new_inst(alloca_st);
   // inst->left = node->token;

   // TODO: handle children if they are struct type
   // ...
   for (int i = 0; i < attrs_size; i++)
   {

   }
#endif
   return node->token;
}

Token *generate_ir(Node *node)
{
   Inst *inst = NULL;
   if (found_error) return NULL;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find) return find;
      return node->token;
   }
   case AS:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);

      inst = new_inst(node->token);
      inst->token->name = NULL; // causes error in llvm generation
      inst->left = left;
      inst->right = right;
      inst->token->retType = node->right->token->retType;
      return node->token;
   }
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      inst = new_inst(node->token);
      if (node->token->type == STRUCT_CALL) struct_call_ir(node);
      if (node->token->is_declare) new_variable(node->token);
      inst->token->retType = inst->token->type;
      return node->token;
   }
   case STRUCT_DEF: return struct_def_ir(node);
   case ASSIGN: case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL:
   case MORE_EQUAL: case MOD:
   {
      return op_ir(node);
   }
   case NOT:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->left = left;
      inst->token->retType = BOOL;
      return inst->token;
   }
   case AND: case OR:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(left->retType != BOOL, "expected booleanatexit but recieved (%s)", to_string(left->retType));
      check(right->retType != BOOL, "expected boolean but recieved (%s)", to_string(right->retType));

      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      inst->token->retType = BOOL;
      return inst->token;
   }
   case IF:    return if_ir(node);
   case WHILE: return while_ir(node);
   case FCALL: return func_call_ir(node);
   case FDEC:  return func_dec_ir(node);
   case RETURN:
   {
      Token *left = generate_ir(node->left);
      inst = new_inst(node->token);
      inst->token->retType = left->type;
      inst->left = left;
      return node->token;
   }
   case BREAK: case CONTINUE:
   {
      print(RED"handle BREAK/CONTINUE\n");
      bool found = false;
      for (int i = scoopPos; i >= 0 && !found; i--)
      {
         Node *scoop = Gscoop[i];
         if (strcmp(scoop->token->name, "while") == 0)
         {
            // BUILD BR
            found = true;
            Inst *inst = new_inst(node->token);
            if (node->token->type == BREAK)
            {
               inst->left = scoop->token->Statement.end;
            }
            else inst->left = scoop->token->Statement.start;
            inst->token->type = BUILD_BR;
         }
      }
      todo(!found, "Invalid syntax\n");
      break;
   }
   case DOT:
   {
      Token *left = generate_ir(node->left); // struct name
      Token *right = node->right->token; // attribute
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(left->type != STRUCT_CALL, "%s should be a struct call", left->name)) break;
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         // print("compare %s == %s\n", attr->name, right->name);
         if (strcmp(attr->name, right->name) == 0)
         {
            attr->Struct.index = i;
            attr->Struct.ptr = left->Statement.ptr;
            Inst *inst =  new_inst(node->token);
            inst->token->retType = attr->type;

            inst->left = left;
            inst->right = attr;
            // inst->token->is_attr = true;
            return inst->token;
         }
      }

      check(1, "%s has no attribute %s", left->name, right->name);
      break;
   }
   case ACCESS:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      if (check(left->type == ID, "undeclared variable %s", left->name)) break;
      if (check(right->type != INT, "should be int")) break;

      inst = new_inst(node->token);
      inst->left = left;
      inst->right = right;
      switch (inst->left->type)
      {
      case CHARS: inst->token->retType = CHAR; break;
      default:
         check(1, "handle this case %s\n", to_string(inst->left->type)); break;
      }
      return node->token;
      break;
   }
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
   return NULL;
}

// OPTIMIZATION

bool did_opimize()
{
   bool res = false;
   Inst **all = insts;
   int i = 0;
   for (i = 0; all[i]; i++)
   {
      Inst *inst = all[i];
      bool remove = true;
      if (inst->token->type == FDEC && strcmp(inst->token->name, "main") != 0)
      {
         for (int j = i + 1; all[j]; j++)
         {
            Token *token = all[j]->token;
            if (token->type == FCALL && strcmp(token->name, inst->token->name) == 0)
            {
               remove = false;
               break;
            }
         }
         if (remove)
         {
            for (int k = 0; k < inst->token->Fdec.pos; k++)
            {
               inst->token->Fdec.args[k]->remove = true;
            }
            int k = i;
            for (; all[k]->token->type != END_BLOC; k++)
            {
               all[k]->token->remove = true;
            }
            todo(all[k]->token->type != END_BLOC, "expected end bloc");
            all[k]->token->remove = true;
            res = true;
         }
      }
   }
   Inst **tmp = allocate(i + 1, sizeof(Inst*));
   int k = 0;
   for (int j = 0; all[j]; j++)
   {
      if (!all[j]->token->remove)
      {
         tmp[k++] = all[j];
      }
      else
      {
         // debug(RED"remove %k\n"RESET, all[j]->token);
      }
   }
   free(insts);
   insts = tmp;
   return res;
}

// ASSEMBLY GENERATION
void handle_asm(Inst *inst)
{
   // debug("Processing: %k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   ValueRef ret = NULL;

   // Sanity check: ensure we don't process the same instruction twice
   if (curr->llvm.is_set)
   {
      print(RED"%k already set\n"RESET, curr);
      exit(1);
      return;
   }

   switch (curr->type)
   {
   case VOID:
   {
      // Void type has no value, just mark as processed
      curr->llvm.is_set = true;
      break;
   }
   case INT: case BOOL: case LONG: case SHORT:
   case CHAR: case CHARS: case PTR: case FLOAT:
   {
      if (curr->is_param)
      {
         // debug(" is_param");
         if (check(curr->Param.func_ptr == NULL, "error\n")) return;
         if (check(!curr->Param.func_ptr->llvm.is_set, "error\n")) return;


         ret = get_param(curr);
         curr->is_param = false;
      }
      else if (curr->is_declare)
      {
         // debug(" is_declare");
         ret = allocate_variable(get_llvm_type(curr), curr->name);
      }
      else if (curr->name)
      {
         // debug(" name");
         ret = curr->llvm.elem;
      }
      else
      {
         // debug(" value");
         ret = get_value(curr);
      }
      // debug("\n");

      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case AS:
   {
      if (check(!left->llvm.is_set, "casting, left is not set")) break;

      curr->llvm.elem = cast(left, right);
      curr->llvm.is_set = true;
      break;
   }
#if 0
   case STRUCT_DEF:
   {
      curr->llvm.is_set = true;
      // Create a named struct type
      curr->llvm.structType = LLVMStructCreateNamed(LLVMGetGlobalContext(), curr->Struct.name);
      int pos = curr->Struct.pos;
      TypeRef *attrs = allocate(pos, sizeof(TypeRef));

      // Get types for all struct attributes
      for (int i = 0; i < pos; i++)
      {
         Token *attr = curr->Struct.attrs[i];
         stop(!attr, "attribite is NULL\n");
         attrs[i] = get_llvm_type(attr);
      }

      // Set the struct's body with all attribute types
      LLVMStructSetBody(curr->llvm.structType, attrs, pos, 0);
      free(attrs);
      break;
   }
   case STRUCT_CALL:
   {
      // Allocate space for a struct instance
      curr->llvm.elem = LLVMBuildAlloca(builder, curr->Struct.ptr->llvm.structType, curr->name);
      curr->llvm.is_set = true;
      break;
   }
   case DOT:
   {
      // Access struct member using dot notation (e.g., person.age)
      ValueRef st_call = left->llvm.elem;
      TypeRef st_type = left->Struct.ptr->llvm.structType;
      int index = right->Struct.attr_index; // attribute position
      curr->llvm.elem = LLVMBuildStructGEP2(builder, st_type, st_call, index, right->name);
      curr->llvm.is_set = true;
      curr->type = right->type;
      curr->name = strdup(right->name);
      break;
   }
#endif
   case ASSIGN:
   {
      if (check(!left->llvm.is_set, "assign, left is not set")) break;
      if (check(!right->llvm.is_set, "assign, right is not set")) break;

      assign2(left, right);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   case AND: case OR:
   {
      if (check(!left->llvm.is_set, "left is not set")) break;
      if (check(!right->llvm.is_set, "right is not set")) break;

      // Perform binary operation (e.g., a + b, x < y, p && q)
      ret = operation(curr, left, right);
      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case NOT:
   {
      if (check(!left->llvm.is_set, "left is not set")) break;

      ret = NotOperation(left);
      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case STACK:
   {
      Token *arg = curr->Fcall.args[0];
      check(!arg->llvm.is_set, "llvm is not set");
      ValueRef elem = llvm_get_ref(arg);
      // TODO: compare ti with llvm_get_ref
      // if (arg->name && !arg->is_param && arg->type != FCALL)
      //    elem = load_variable(arg);
      // else
      //    elem = arg->llvm.elem;

      // Request an element pointer (i8*) directly.
      curr->llvm.elem = allocate_stack(elem, i8, "stack");
      curr->llvm.is_set = true;
      break;
   }

   case FCALL:
   {
      // TODO: move this insde call_function later
      LLVM srcFunc = curr->Fcall.func_ptr->llvm;

      ValueRef *args = NULL;
      if (curr->Fcall.pos)
      {
         args = allocate(curr->Fcall.pos, sizeof(ValueRef));
         for (int i = 0; i < curr->Fcall.pos; i++)
         {
            Token *arg = curr->Fcall.args[i];
            check(!arg->llvm.is_set, "llvm is not set");

            // Load variable values, pass literals/temporaries directly
            // TODO: compare ti with llvm_get_ref
            args[i] = llvm_get_ref(arg);
            // if (arg->name && !arg->is_param && arg->type != FCALL)
            //    args[i] = load_variable(arg);
            // else
            //    args[i] = arg->llvm.elem;
         }

      }
      call_function(curr, &srcFunc, args, curr->Fcall.pos);

      free(args);

      curr->llvm.is_set = true;
      break;
   }
   case FDEC:
   {
      create_function(curr);
      // debug(CYAN">> enter %s\n"RESET, curr->name);
      enter_func(curr->llvm.elem);
      if (!curr->is_proto) open_block(create_bloc("entry"));
      curr->llvm.is_set = true;
      break;
   }
   case END_BLOC:
   {
      // debug(CYAN">> exit %s\n"RESET, curr->name);
      exit_func();
      break;
   }
   case RETURN:
   {
      if (check(!left->llvm.is_set, "return result is not set\n")) break;

      switch (left->type)
      {
      case FCALL: case ADD: case SUB: case MUL: case DIV: case MOD:
      case LESS: case LESS_EQUAL: case MORE:
      case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
      case AND: case OR:
      {
         ret = return_(left->llvm.elem);
         break;
      }
      case INT: case BOOL: case LONG: case SHORT: case CHAR: case FLOAT:
      {
         if (left->name)
         {
            // TODO: compare ti with llvm_get_ref
            ValueRef loaded = load_variable(left);
            ret = return_(loaded);
         }
         else
            ret = return_(get_value(left));
         break;
      }
      case VOID:
      {
         ret = return_(NULL);
         break;
      }
      default:
         todo(1, "handle this case %s\n", to_string(left->type));
         break;
      }
      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case APPEND_BLOC:
   {
      if (check(!left->name, "APPEND BLOC require a name")) break;

      left->llvm.bloc = create_bloc(left->name);
      left->llvm.is_set = true;
      break;
   }
   case BUILD_COND:
   {
      check(!curr->Statement.ptr->llvm.is_set, "BUILD COND require cond to be set");
      build_condition(curr, left, right);
      curr->llvm.is_set = true;
      break;
   }
   case ACCESS:
   {
      check(!left->llvm.is_set, "left is not set");
      check(!right->llvm.is_set, "right is not set");

      curr->llvm.elem = access_(curr, left, right);
      curr->llvm.is_set = true;
      break;
   }
   case SET_POS:
   {
      check(!left->llvm.is_set, "SET POS require left to be set");
      open_block(left->llvm.bloc);
      break;
   }
   case BUILD_BR:
   {
      check(!left->llvm.is_set, "BUILD BR require left to be set");
      branch(left->llvm.bloc);
      break;
   }
   default:
      todo(1, "handle this case (%s)\n", to_string(curr->type));
      break;
   }
}
