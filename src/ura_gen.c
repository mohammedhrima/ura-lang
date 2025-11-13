#include "./ura_header.h"

Token *func_dec_ir(Node *node)
{
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
      child->token->is_param = true;
      child->token->Param.index = i;
      child->token->Param.func_ptr = node->token;
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

   if (!node->token->is_proto)
   {
      // TODO: if RETURN not found add it
      Token *new = new_token(END_BLOC, node->token->space);
      new->name = strdup(node->token->name);
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
   else
   {
      Node *func = get_function(node->token->name);
      if (!func) return NULL;
      node->token->Fcall.func_ptr = func->token;
      node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      node->token->Fcall.pos = node->cpos;

      func = copy_node(func);
      node->token->retType = func->token->retType;

      // setReg(node->token, func->token->creg);
      Node *fcall = node;
      Node *fdec = func->left;

      if (check(fcall->cpos != fdec->cpos,
                "Incompatible number of arguments %s", func->token->name))
         return NULL;

      for (int i = 0; !found_error && i < fcall->cpos; i++)
      {
         Node *carg = fcall->children[i]; // will always be ID
         Node *darg = fdec->children[i];

         Token *src = generate_ir(carg);
         Token *dist = generate_ir(darg);

         if (check(src->type == ID, "Indeclared variable %s",
                   carg->token->name)) break;
         if (check(!compatible(src, dist), "Incompatible type arg %s",
                   func->token->name)) break;
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
   if (!cond) return NULL;

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
   if (!right || !right) return NULL;

   // TODO: fix the check later
   // check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
   //       to_string(node->token->type), to_string(left->type), to_string(right->type));
   switch (node->token->type)
   {
   case ASSIGN:
   {
      node->token->ir_reg = left->ir_reg;
      node->token->retType = getRetType(node);
#if 0
      // if (left->is_ref) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
      //         else node->token->assign_type = REF_HOLD_REF;
      //     }
      //     else if (right->ptr || right->creg) // ptr
      //     {
      //         if (left->has_ref) node->token->assign_type = REF_ID;
      //         else node->token->assign_type = REF_HOLD_ID;
      //     }
      //     else // ir_reg, value
      //     {
      //         print("line %d: %n\n", LINE, node);
      //         // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
      //         node->token->assign_type = REF_VAL;
      //     }
      //     left->has_ref = true;
      // }
      // else if (left->ptr || left->creg) // ir_reg, ptr
      // {
      //     if (right->is_ref) // ir_reg, ptr
      //     {
      //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
      //         node->token->assign_type = ID_REF;
      //     }
      //     else if (right->ptr) // ptr
      //         node->token->assign_type = ID_ID;
      //     else // ir_reg, value
      //         node->token->assign_type = ID_VAL;
      // }
      // else if (left->type == STRUCT_CALL)
      // {
      //     stop(1, "check this");
      //     print(">> %k\n", left);
      //     print(">> %k\n", right);
      //     // TODO: check compatibility
      //     for (int i = 0; i < left->Struct.pos; i++)
      //     {
      //         Node *tmp = new_node(new_token(ASSIGN, node->token->space));
      //         tmp->left = new_node(left->Struct.attrs[i]);
      //         tmp->right = new_node(right->Struct.attrs[i]);
      //         op_ir(tmp);
      //         free_node(tmp);
      //     }
      //     return NULL;
      //     // exit(1);
      // }
      // else
      // {
      //     pnode(node, NULL, 0);
      //     print("<%k>\n", left);
      //     print("<%k>\n", right);
      //     // todo(1, "Invalid assignment");
      // }
#endif
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   {
      // TODO: to be checked
      // node->token->retType = INT;
      // if (node->token->retType  == INT) setReg(node->token, "eax");
      // else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
      // else
      // check(1, "handle this case");
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
   case CAST:
   {

      break;
   }
   // case VOID: return node->token;
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      inst = new_inst(node->token);
      if (node->token->type == STRUCT_CALL) struct_call_ir(node);
      if (node->token->is_declare) new_variable(node->token);
      return node->token;
   }
   case STRUCT_DEF: return struct_def_ir(node);
   case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
   case DIV_ASSIGN:
   case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL: case MOD:
   {
      // TODO: check if right is DEFAULT, then initlize it, and return left
      return op_ir(node);
   }
   case AND: case OR:
   {
      Token *left = generate_ir(node->left);
      Token *right = generate_ir(node->right);
      check(!(left->type == BOOL || left->retType == BOOL),
            "left should be boolean but instead recieved (%s) (%s)", to_string(left->type),
            left->retType ? to_string(left->retType) : "");
      check(!(right->type == BOOL || right->retType == BOOL),
            "right should be boolean but instead recieved (%s) (%s)", to_string(right->type),
            right->retType ? to_string(right->retType) : "");

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
               // print(RED "break to %k\n" RESET, scoop->token->Statement.end);
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
      if (check(left->type != STRUCT_CALL,
                "%s should be a struct call", left->name)) break;
      for (int i = 0; i < left->Struct.pos; i++)
      {
         Token *attr = left->Struct.attrs[i];
         // print("compare %s == %s\n", attr->name, right->name);
         if (strcmp(attr->name, right->name) == 0)
         {
            attr->Struct.index = i;
            attr->Struct.ptr = left->Statement.ptr;
            Inst *inst =  new_inst(node->token);

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
      default: check(1, "handle this case %s\n", to_string(inst->left->type)); break;
         break;
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
