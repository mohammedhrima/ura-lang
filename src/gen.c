#include "./header.h"

// ASSEMBLY GENERATION
void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   token->llvm.elem = llvm_build_alloca(token, type, token->name);
}

void _int(Token *token)
{
   TypeRef type = get_llvm_type(token); long long value;
   value = (long long)token->Int.value;
   token->llvm.elem = llvm_const_int(type, value, 0);
}

void _bool(Token *token)
{
   TypeRef type = get_llvm_type(token); long long value;
   value = (long long)token->Bool.value;
   token->llvm.elem = llvm_const_int(type, value, 0);
}

void _char(Token *token)
{
   TypeRef type = get_llvm_type(token); int value;
   value = (int)token->Char.value;
   token->llvm.elem = llvm_const_int(type, value, 0);
}

void _chars(Token *token)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);

   char *processed = calloc(strlen(token->Chars.value) * 2 + 1, 1);
   int j = 0;
   for (int i = 0; token->Chars.value[i]; i++) {
      if (token->Chars.value[i] == '\\' && token->Chars.value[i + 1]) {
         switch (token->Chars.value[i + 1]) {
         case 'n': processed[j++] = '\n'; i++; break;
         case 't': processed[j++] = '\t'; i++; break;
         case 'r': processed[j++] = '\r'; i++; break;
         case '0': processed[j++] = '\0'; i++; break;
         case '\\': processed[j++] = '\\'; i++; break;
         case '\"': processed[j++] = '\"'; i++; break;
         case '\'': processed[j++] = '\''; i++; break;
         default: processed[j++] = token->Chars.value[i]; break;
         }
      } else {
         processed[j++] = token->Chars.value[i];
      }
   }

   token->llvm.elem = llvm_build_global_string_ptr(token, processed, name);
   free(processed);
}

void _return(Token *token)
{
   Value value = token->llvm.elem;
   if (value) llvm_build_ret(token, value);
   else llvm_build_ret_void(token);
}

TypeRef get_llvm_type(Token *token)
{
   Type type = token->type;
   if (token->retType) type = token->retType;
   TypeRef res[END] = {[INT] = i32, [CHAR] = i8, [CHARS] = p8,
                       [BOOL] = i1, [VOID] = vd,
                       //[VA_LIST] = p8,
                       [ACCESS] = i8,
                       //[CATCH] = i32,
                      };

   check(!res[type], "handle this case [%s]\n", to_string(type));
   return res[type];
}

void load_if_neccessary(Node *node)
{
   Token *token = node->token;

   // this line was added to fix a = 1, b = 2, c = a + b
   // TODO: to be tested for other cases
   if (includes(token->type, MATH_TYPE, 0))
      return;

   if (token->llvm.is_loaded || includes(token->type, CHARS, /*STACK,*/ 0))
      return;

   if (token->name && token->type != FCALL)
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->llvm.is_loaded = true;
      node->token = new;
   }
   else if (includes(token->type, ACCESS, 0))
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->llvm.is_loaded = true;
      node->token = new;
   }
}

void _entry(Token *token)
{
   Block entry = llvm_append_basic_block_in_context(token->llvm.elem, "entry");
   llvm_position_builder_at_end(entry);
}

Value build_binary_op(Type op_type, Value lref, Value rref)
{
   char* op = to_string(op_type);
   switch (op_type) {
   case LESS:        return LLVMBuildICmp(builder, LLVMIntSLT, lref, rref, op);
   case MORE:        return LLVMBuildICmp(builder, LLVMIntSGT, lref, rref, op);
   case EQUAL:       return LLVMBuildICmp(builder, LLVMIntEQ,  lref, rref, op);
   case LESS_EQUAL:  return LLVMBuildICmp(builder, LLVMIntSLE, lref, rref, op);
   case MORE_EQUAL:  return LLVMBuildICmp(builder, LLVMIntSGE, lref, rref, op);
   case NOT_EQUAL:   return LLVMBuildICmp(builder, LLVMIntNE,  lref, rref, op);
   case ADD:         return LLVMBuildAdd(builder, lref, rref, op);
   case SUB:         return LLVMBuildSub(builder, lref, rref, op);
   case MUL:         return LLVMBuildMul(builder, lref, rref, op);
   case DIV:         return LLVMBuildSDiv(builder, lref, rref, op);
   case MOD:         return LLVMBuildSRem(builder, lref, rref, op);
   case AND:         return LLVMBuildAnd(builder, lref, rref, op);
   case OR:          return LLVMBuildOr(builder, lref, rref, op);
   default:          todo(1, "handle this %s", op); return NULL;
   }
}

void build_literal(Type type, Token *token)
{
   switch (type) {
   case INT:   _int(token); break;
   case BOOL:  _bool(token); break;
   case CHAR:  _char(token); break;
   case CHARS: _chars(token); break;
   default:    check(1, "handle this case [%s]\n", to_string(type)); break;
   }
}

void generate_asm(Node *node)
{
   // debug("Processing: %k\n", inst->token);
   Value ret = NULL;
   Node *left = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set\n"))
      return;
   switch (node->token->type)
   {
   case INT: case CHARS: case CHAR: case BOOL:
   {
      if (node->token->is_dec)
      {
         _alloca(node->token);
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
      {
         return;
      }
      build_literal(node->token->type, node->token);
      break;
   }
   case ASSIGN:
   {
      generate_asm(left);
      generate_asm(right);
      if (!left->token->is_ref && !right->token->is_ref)
      {
         load_if_neccessary(right);
         llvm_build_store(node->token, right->token->llvm.elem, left->token->llvm.elem);
         right->token->llvm.elem = left->token->llvm.elem;
      }
      else
         check(1, "handle this case");
      break;
   }
   case NOT:
   {
      generate_asm(left);
      load_if_neccessary(left);
      node->token->llvm.elem = llvm_build_not(left->token);
      break;
   }
   case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL:
   case MORE_EQUAL: case MOD: case ADD: case AND: case OR:
   {
      generate_asm(left);
      generate_asm(right);

      load_if_neccessary(left);
      load_if_neccessary(right);

      Value lref = left->token->llvm.elem;
      Value rref = right->token->llvm.elem;

      node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
      node->token->retType = left->token->type;
      break;
   }
   case FCALL:
   {
      LLVM srcFunc = node->token->Fcall.ptr->llvm;
      int count = node->left->cpos;
      Node **argNodes = node->left->children;

      Value *args = NULL;
      if (count)
      {
         args = allocate(count + 1, sizeof(Value));
         for (int i = 0; i < count; i++)
         {
            generate_asm(argNodes[i]);
            if (argNodes[i]->token->name) // TODO: to be tested
               load_if_neccessary(argNodes[i]);
            args[i] = argNodes[i]->token->llvm.elem;
         }
      }
      char *name = node->token->retType != VOID ? node->token->name : "";
      TypeRef funcType = srcFunc.funcType;
      Value elem = srcFunc.elem;
      node->token->llvm.elem = LLVMBuildCall2(builder, funcType, elem, args, count, name);
      free(args);
      break;
   }
   case FDEC: case PROTO:
   {
      if (scoop_pos > 1)
      {
         static int id = 0;
         char name[256];
         snprintf(name, sizeof(name), "%s.%s.%d", scoop->token->name, node->token->name, id++);
         node->token->llvm_name = strdup(name);
      }
      else
         node->token->llvm_name = strdup(node->token->name);

      enter_scoop(node);

      TypeRef retType = get_llvm_type(node->token);
      TypeRef *paramTypes = NULL;
      int param_count1 = 0;

      if (node->left->cpos)
      {
         int param_count = node->left->cpos;
         param_count1 = param_count;

         if (node->token->Fdec.is_variadic)
         {
            param_count--;
            param_count1 = param_count + 1;
         }

         paramTypes = calloc(param_count1 + 1, sizeof(TypeRef));

         for (int i = 0; i < param_count; i++)
         {
            Token *param = node->left->children[i]->token;
            if (param->is_ref) paramTypes[i] = llvm_pointer_type(get_llvm_type(param), 0);
            else paramTypes[i] = get_llvm_type(param);
         }

         // Hidden count parameter
         if (node->token->Fdec.is_variadic) paramTypes[param_count] = i32;
      }



      TypeRef funcType = llvm_function_type(retType, paramTypes, param_count1,
                                            node->token->Fdec.is_variadic);

      char *name = node->token->llvm_name ? node->token->llvm_name : node->token->name;
      Value existingFunc = llvm_get_named_function(name);
      if (existingFunc) node->token->llvm.elem = existingFunc;
      else node->token->llvm.elem = llvm_add_function(name, funcType);
      node->token->llvm.funcType = funcType;

      if (node->token->type == FDEC)
      {
         _entry(node->token);
         for (int i = 0; i < node->left->cpos; i++)
         {
            Token *param_token = node->left->children[i]->token;
            Value param = LLVMGetParam(node->token->llvm.elem, i);
            LLVMSetValueName(param, param_token->name);

            // if is not ref
            _alloca(param_token);
            param_token->is_dec = false;

            LLVMBuildStore(builder, param, param_token->llvm.elem);
         }

         for (int i = 0; i < node->cpos; i++)
         {
            // if (node->children[i]->token->type != FDEC)
            generate_asm(node->children[i]);
         }

         if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
         {
            if (node->token->retType == VOID)
               llvm_build_ret_void(node->token);
            else
            {
               fprintf(stderr, "Warning: Non-void function '%s' may not return a value\n",
                       node->token->name);
               llvm_build_ret(node->token, llvm_const_int(get_llvm_type(node->token), 0, 0));
            }
         }
      }
      exit_scoop();
      break;
   }
   case RETURN:
   {
      generate_asm(left);
      load_if_neccessary(left);

      // ExcepCTX *ctx = get_current_exception_context();
      // if (ctx && ctx->in_catch) {
      //    Value end_catch = get_end_catch();
      //    llvm_build_call2(node->token, llvm_global_get_value_type(end_catch), end_catch, NULL, 0, "");
      // }

      _return(left->token);
      break;
   }
   case END_BLOC: exit_scoop(); break;
   default:
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
}

void generate_ir(Node *node)
{
   if (found_error) return;
   Node *curr = node;
   Node *left = node->left;
   Node *right = node->right;
   switch (node->token->type)
   {
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find) node->token = find;
      break;
   }
   case INT: case BOOL: case CHAR: case STRUCT_CALL:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      node->token->ir_reg++;
      if (node->token->is_dec) new_variable(node->token);
      node->token->retType = node->token->type;
      break;
   }
   case ASSIGN:
   {
      // TODO: check compatibility
      generate_ir(left);
      generate_ir(right);
      node->token->ir_reg = left->token->ir_reg;
      node->token->retType = left->token->retType;
      break;
   }
   case NOT:
   {
      generate_ir(left);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case MORE: case LESS_EQUAL:
   case MORE_EQUAL: case MOD: case AND: case OR:
   {
      // TODO: check compatibility
      generate_ir(left);
      generate_ir(right);

      check(!left || !left->token, "error in assignment, left is NULL");
      check(!right || !right->token, "error in assignment, right is NULL");

      switch (node->token->type)
      {
      case ADD: case SUB: case MUL: case DIV: case MOD:
         node->token->retType = left->token->retType;
         break;
      case AND: case OR:
         node->token->retType = BOOL;
         break;
      case NOT_EQUAL: case EQUAL: case LESS:
      case MORE: case LESS_EQUAL: case MORE_EQUAL:
         node->token->retType = BOOL;
         break;
      default: break;
      }
      break;
   }
   case FDEC:
   {
      node->token->ir_reg++;
      new_function(node);
      enter_scoop(node);

      // parameters
      Node **params = (node->left ? node->left->children : NULL);
      Token *token = node->token;
      for (int i = 0; params && i < node->left->cpos && !found_error; i++)
         generate_ir(params[i]);

      // code bloc
      for (int i = 0; node->token->type != PROTO && i < node->cpos; i++)
      {
         Node *child = node->children[i];
         generate_ir(child);
      }

      // // if (!node->token->is_proto)
      // {
      //    Token *new = new_token(END_BLOC, node->token->space);
      //    setName(new, node->token->name);
      //    new_inst(new);
      // }
      exit_scoop();
      break;
   }
   case FCALL:
   {
      Node *func = get_function(node->token->name);
      if (!func) return;
      node->token->Fcall.ptr = func->token;
      // node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
      // node->token->Fcall.pos = node->cpos;

      func = copy_node(func);
      // node->token->retType = func->token->retType;
      // node->token->is_variadic = func->token->is_variadic;

      // Node *call_args = node;
      // Node *dec_args = func->left;

      // if (check(call_args->cpos != dec_args->cpos && !node->token->is_variadic, "Incompatible number of arguments %s", func->token->name))
      //    return NULL;

      // for (int i = 0; !found_error && i < call_args->cpos; i++)
      // {
      //    Node *carg = call_args->children[i]; // will always be ID
      //    Token *src = generate_ir(carg);


      //    if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
      //    if (i < dec_args->cpos)
      //    {
      //       Node *darg = dec_args->children[i];
      //       Token *dist = darg->token;

      //       // if (check(!compatible(src, dist), "Incompatible type arg %s", func->token->name)) break;
      //       src->is_ref = darg->token->is_ref;
      //       src->has_ref = false;
      //    }
      //    node->token->Fcall.args[i] = src;
      // }
      free_node(func);
      break;
   }
   case RETURN:
   {
      node->token->ir_reg++;
      generate_ir(node->left);
      node->token->retType = node->left->token->type;
      break;
   }
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
}