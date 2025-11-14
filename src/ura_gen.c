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
   case CAST:
   {
      break;
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
      check(left->retType != BOOL, "expected boolean but recieved (%s)", to_string(left->retType));
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

LLVMValueRef get_llvm_ref(Token *token)
{
   if (token->name && !token->is_param && !includes(token->type, FCALL, AND, OR))
      return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, token->name);
   return token->llvm.elem;
}

// ASSEMBLY GENERATION
LLVMValueRef get_llvm_op(Token *token, Token* left, Token* right)
{
   LLVMValueRef leftRef = get_llvm_ref(left);
   LLVMValueRef rightRef = get_llvm_ref(right);
   char* op = to_string(token->type);
   switch (token->type)
   {
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, op); break;
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, op); break;
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, op); break;
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, op); break;
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, op); break;
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, op); break;
   case ADD: return LLVMBuildAdd(builder, leftRef, rightRef, op); break;
   case SUB: return LLVMBuildSub(builder, leftRef, rightRef, op); break;
   case MUL: return LLVMBuildMul(builder, leftRef, rightRef, op); break;
   case DIV: return LLVMBuildSDiv(builder, leftRef, rightRef, op); break;
   case MOD: return LLVMBuildSRem(builder, leftRef, rightRef, op); break;
   case AND: return LLVMBuildAnd(builder, leftRef, rightRef, op); break;
   case OR: return LLVMBuildOr(builder, leftRef, rightRef, op); break;
   default: todo(1, "handle this %s", op);
   }
   return NULL;
}

void handle_asm(Inst *inst)
{
   debug("Processing: %k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   LLVMValueRef ret = NULL;

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
      curr->llvm.is_set = true;
      break;
   }
   case INT: case BOOL: case LONG: case SHORT: case CHAR: case CHARS: case PTR:
   {
      if (curr->is_param)
      {
         ret = LLVMGetParam(curr->Param.func_ptr->llvm.elem, curr->Param.index);
         LLVMSetValueName(ret, curr->name);
      }
      else if (curr->is_declare)
      {
         ret = LLVMBuildAlloca(builder, get_llvm_type(curr), curr->name);
      }
      else if (curr->name)
      {
         ret = curr->llvm.elem;
      }
      else ret = get_value(curr);

      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
#if 0
   case STRUCT_DEF:
   {
      curr->llvm.is_set = true;
      // CREATE STRUCT TYPE
      // SET STRUCT BODY
      curr->llvm.structType = LLVMStructCreateNamed(LLVMGetGlobalContext(), curr->Struct.name);
      int pos = curr->Struct.pos;
      LLVMTypeRef *attrs = allocate(pos, sizeof(LLVMTypeRef));
      for (int i = 0; i < pos; i++)
      {
         Token *attr = curr->Struct.attrs[i];
         stop(!attr, "attribite is NULL\n");
         attrs[i] = get_llvm_type(attr);
      }
      // SET STRUCT BODY
      LLVMStructSetBody(curr->llvm.structType, attrs, pos, 0);
      free(attrs);
      break;
   }
   case STRUCT_CALL:
   {
      curr->llvm.elem = LLVMBuildAlloca(builder, curr->Struct.ptr->llvm.structType, curr->name);
      curr->llvm.is_set = true;
      break;
   }
   case DOT:
   {
      LLVMValueRef st_call = left->llvm.elem;
      LLVMTypeRef st_type = left->Struct.ptr->llvm.structType;
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
      if(check(!left->llvm.is_set, "assign, left is not set")) break;
      if(check(!right->llvm.is_set, "assign, right is not set")) break;

      // LLVMValueRef target = left->llvm.ptr ? left->llvm.ptr : left->llvm.elem;
      LLVMValueRef target = left->llvm.elem;
      LLVMBuildStore(builder, right->llvm.elem, target);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case MOD:
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   case AND: case OR:
   {
      if (check(!left->llvm.is_set, "left is not set")) break;
      if (check(!right->llvm.is_set, "right is not set")) break;
   
      ret = get_llvm_op(curr, left, right);
      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case NOT:
   {
      if(check(!left->llvm.is_set, "not, left is not set")) break;
      LLVMValueRef leftRef = get_llvm_ref(left);
      ret = LLVMBuildNot(builder, leftRef, to_string(curr->type));
      curr->llvm.elem = ret;
      curr->llvm.is_set = true;
      break;
   }
   case FCALL:
   {
      LLVM srcFunc = curr->Fcall.func_ptr->llvm;

      LLVMValueRef *args = NULL;
      if (curr->Fcall.pos)
      {
         args = allocate(curr->Fcall.pos, sizeof(LLVMValueRef));
         for (int i = 0; i < curr->Fcall.pos; i++)
         {
            Token *arg = curr->Fcall.args[i];
            check(!arg->llvm.is_set, "llvm is not set");
            if (arg->name && !arg->is_param && arg->type != FCALL)
               args[i] = LLVMBuildLoad2(builder, get_llvm_type(arg), arg->llvm.elem, arg->name);
            else
               args[i] = arg->llvm.elem;
         }
         curr->llvm.elem = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem, args,
                                          curr->Fcall.pos, curr->name);
         free(args);
      }
      else
         curr->llvm.elem = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem,
                                          NULL, 0, curr->name);

      curr->llvm.is_set = true;
      break;
   }
   case FDEC:
   {
      // print("FDEC: ", curr->name);
      LLVMTypeRef *args = NULL;

      if (curr->Fdec.pos)
      {
         args = allocate(curr->Fdec.pos + 1, sizeof(LLVMTypeRef));
         for (int i = 0; i < curr->Fdec.pos; i++)
            args[i] = get_llvm_type(curr->Fdec.args[i]);
         curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), args, curr->Fdec.pos, 0);
         free(args);
      }
      else curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), NULL, 0, 0);

      curr->llvm.elem = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);

      if (!curr->is_proto)
      {
         LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.elem, "entry");
         LLVMPositionBuilderAtEnd(builder, funcEntry);
         LLVMPositionBuilderAtEnd(builder, funcEntry);
      }

      enter_func(curr->llvm.elem);
      curr->llvm.is_set = true;
      break;
   }
   case END_BLOC:
   {
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
         ret = LLVMBuildRet(builder, left->llvm.elem);
         break;
      }
      case INT: case BOOL: case LONG: case SHORT: case CHAR: case FLOAT:
      {
         if (left->name
#if 0
               || left->is_attr
#endif
            )
         {
            LLVMValueRef loaded = LLVMBuildLoad2(builder, get_llvm_type(left),
                                                 left->llvm.elem, left->name);
            ret = LLVMBuildRet(builder, loaded);
         }
         else
            ret = LLVMBuildRet(builder, get_value(left));
         break;
      }
      case VOID:
      {
         ret = LLVMBuildRetVoid(builder);
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
      check(!left->name, "APPEND BLOC require a name");
      left->llvm.bloc = LLVMAppendBasicBlockInContext(context, get_current_func(), left->name);
      curr->llvm.is_set = true;
      break;
   }
   case BUILD_COND:
   {
      LLVMValueRef cond = curr->Statement.ptr->llvm.elem;
      LLVMBasicBlockRef start = left->llvm.bloc;
      LLVMBasicBlockRef end = right->llvm.bloc;
      curr->llvm.elem = LLVMBuildCondBr(builder, cond, start, end);
      curr->llvm.is_set = true;
      break;
   }
#if 0
   case ACCESS:
   {
      check(!left->llvm.is_set, "left is not set");
      check(!right->llvm.is_set, "right is not set");

      LLVMValueRef leftRef = NULL, rightRef = NULL;
      if (left->name && !left->is_param && left->type != FCALL)
         leftRef = LLVMBuildLoad2(builder, get_llvm_type(left), left->llvm.elem, left->name);
      else leftRef = left->llvm.elem;

      if (right->name && !right->is_param && right->type != FCALL)
         rightRef = LLVMBuildLoad2(builder, get_llvm_type(right), right->llvm.elem, right->name);
      else rightRef = right->llvm.elem;

      LLVMValueRef indices[] = { rightRef };
      LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, i8, leftRef, indices, 1, "access");

      curr->llvm.ptr = elem_ptr;

      curr->llvm.elem = LLVMBuildLoad2(builder, i8, elem_ptr, "access_val");
      curr->llvm.is_set = true;
      curr->type = CHAR;
      setName(curr, N)
      curr->name = NULL;
      break;
   }
#endif
   case SET_POS:
   {
      LLVMPositionBuilderAtEnd(builder, left->llvm.bloc);
      break;
   }
   case BUILD_BR:
   {
      check(!left->name, "BUILD BR require a name");
      LLVMBuildBr(builder, left->llvm.bloc);
      break;
   }
   default: todo(1, "handle this case (%s)\n", to_string(curr->type)); break;
   }
}
