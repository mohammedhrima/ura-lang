#include "./header.h"

// ASSEMBLY GENERATION
void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   if (token->is_ref) {
      type = LLVMPointerType(type, 0);
   }
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
   // if (token->is_ref) return i64;
   TypeRef res[END] = {[INT] = i32, [CHAR] = i8, [CHARS] = p8,
                       [BOOL] = i1, [VOID] = vd,
                       //[VA_LIST] = p8,
                       [ACCESS] = i8,
                       //[CATCH] = i32,
                      };

   check(!res[type], "handle this case [%s]", to_string(type));
   return res[type];
}

void load_if_neccessary(Node *node)
{
   Token *token = node->token;

   // this line was added to fix a = 1, b = 2, c = a + b
   // TODO: to be tested for other cases
   if (includes(token->type, MATH_TYPE, 0))
      return;

   if (token->llvm.is_loaded || includes(token->type, CHARS, STACK, 0))
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
   _position_at(entry);
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
   default:    check(1, "handle this case [%s]", to_string(type)); break;
   }
}

Value allocate_stack(Value size, TypeRef elementType, char *name)
{
   Value indices[] = {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, 0, 0)
   };

   if (LLVMIsConstant(size))
   {
      unsigned long long constSize = LLVMConstIntGetZExtValue(size);
      TypeRef arrayType = LLVMArrayType(elementType, constSize);
      Value array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
   }

   Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

void generate_asm(Node *node)
{
   // debug("Processing: %k\n", inst->token);
   Node *left = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set"))
      return;
   switch (node->token->type)
   {
   case INT: case CHARS: case CHAR: case BOOL:
   {
      if (node->token->is_dec)
      {
         _alloca(node->token);
         if (node->token->is_ref)
         {
            TypeRef type = get_llvm_type(node->token);
            Value null = LLVMConstNull(LLVMPointerType(type, 0));
            llvm_build_store(node->token, null, node->token->llvm.elem);
         }
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
         return;
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
      }
      else if (left->token->is_ref && !right->token->is_ref)
      {
         llvm_build_store(node->token, right->token->llvm.elem, left->token->llvm.elem);
      }
      else if (left->token->is_ref && right->token->is_ref)
      {
         Value val = enable_bounds_check ? check_null(right) : right->token->llvm.elem;
         llvm_build_store(node->token, val, left->token->llvm.elem);
      }
      else
      {
         TypeRef type = get_llvm_type(right->token);
         Value val = enable_bounds_check ? check_null(right) : right->token->llvm.elem;
         val = llvm_build_load2(right->token, type, val, "val");
         llvm_build_store(node->token, val, left->token->llvm.elem);
      }
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
   case STACK:
   {
      generate_asm(node->left->children[0]);
      load_if_neccessary(node->left->children[0]);
      Value elem = node->left->children[0]->token->llvm.elem;

      node->token->llvm.elem = allocate_stack(elem, i8, "stack");
      node->token->llvm.is_set = true;

      // Store size in the token itself
      node->token->llvm.array_size = elem;
      // if (LLVMIsConstant(elem)) {
      //    curr->Array.const_size = LLVMConstIntGetZExtValue(elem);
      // }

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
   case FDEC:
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

      if (!node->token->is_proto)
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
   case WHILE:
   {
      enter_scoop(node);
      Block start = _append_block("while.start");
      Block then = _append_block("while.then");
      Block end = _append_block("while.end");

      // Store blocks for break/continue
      node->token->llvm.start = start;
      node->token->llvm.then = then;
      node->token->llvm.end = end;

      _branch(start);

      _position_at(start);
      generate_asm(node->left); // condition
      load_if_neccessary(node->left);
      _condition(node->left->token->llvm.elem, then, end);

      _position_at(then);
      for (int i = 0; i < node->cpos; i++)
         generate_asm(node->children[i]);
      _branch(start);

      _position_at(end);
      exit_scoop();
      break;
   }
   case IF:
   {
      enter_scoop(node);

      Block if_start = _append_block("if.start");
      Block end = _append_block("if.end");

      // Store blocks for potential break/continue
      node->token->llvm.start = if_start;
      node->token->llvm.end = end;

      _branch(if_start);

      Node *curr = node;

      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
      {
         if (includes(curr->token->type, IF, ELIF, 0))
         {
            Block start;
            Block then;
            Block next;

            if (curr->token->type == IF)
            {
               start = if_start;
               then = _append_block("if.then");
            }
            else
            {
               start = curr->token->llvm.bloc;
               then = _append_block("elif.then");
            }

            // Store then block
            curr->token->llvm.then = then;

            if (curr->right)
            {
               if (curr->right->token->type == ELSE) next = _append_block("if.else");
               else next = _append_block("elif.start");
            }
            else next = end;

            _position_at(start);
            generate_asm(curr->left); // condition
            load_if_neccessary(curr->left);
            _condition(curr->left->token->llvm.elem, then, next);

            _position_at(then);
            for (int i = 0; i < curr->cpos; i++)
               generate_asm(curr->children[i]);
            _branch(end);

            if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
               curr->right->token->llvm.bloc = next;
         }
         else if (curr->token->type == ELSE)
         {
            _position_at(curr->token->llvm.bloc);
            for (int i = 0; i < curr->cpos; i++)
               generate_asm(curr->children[i]);
            _branch(end);
         }
         curr = curr->right;
      }
      _position_at(end);
      exit_scoop();
      break;
   }
   case BREAK:
   {
      for (int i = scoop_pos; i >= 0; i--)
      {
         if (Gscoop[i]->token->type == WHILE)
         {
            _branch(Gscoop[i]->token->llvm.end);
            return;
         }
      }
      check(1, "break outside loop");
      break;
   }
   case CONTINUE:
   {
      for (int i = scoop_pos; i >= 0; i--)
      {
         if (Gscoop[i]->token->type == WHILE)
         {
            _branch(Gscoop[i]->token->llvm.start);
            return;
         }
      }
      check(1, "continue outside loop");
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
   case AS:
   {
      generate_asm(left);
      load_if_neccessary(left);

      Value source = left->token->llvm.elem;
      TypeRef sourceType = LLVMTypeOf(source);
      TypeRef targetType = get_llvm_type(node->right->token);

      LLVMTypeKind sourceKind = LLVMGetTypeKind(sourceType);
      LLVMTypeKind targetKind = LLVMGetTypeKind(targetType);

      Value result;

      // Pointer to integer
      if (sourceKind == LLVMPointerTypeKind && targetKind == LLVMIntegerTypeKind)
         result = LLVMBuildPtrToInt(builder, source, targetType, "as");
      // Integer to pointer
      else if (sourceKind == LLVMIntegerTypeKind && targetKind == LLVMPointerTypeKind)
         result = LLVMBuildIntToPtr(builder, source, targetType, "as");
      // Integer to integer
      else if (sourceKind == LLVMIntegerTypeKind && targetKind == LLVMIntegerTypeKind)
      {
         unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
         unsigned targetBits = LLVMGetIntTypeWidth(targetType);

         if (sourceBits > targetBits)
            result = LLVMBuildTrunc(builder, source, targetType, "as");
         else if (sourceBits < targetBits)
            result = LLVMBuildSExt(builder, source, targetType, "as");
         else
            result = source;
      }
      // Pointer to pointer (bitcast)
      else if (sourceKind == LLVMPointerTypeKind && targetKind == LLVMPointerTypeKind)
         result = LLVMBuildBitCast(builder, source, targetType, "as");

      else
      {
         check(1, "unsupported cast from %d to %d", sourceKind, targetKind);
         result = source;
      }

      node->token->llvm.elem = result;
      node->token->llvm.is_loaded = true;
      break;
   }
   case ACCESS:
   {

      // bool enable_bounds_check = false;
      generate_asm(node->left);
      generate_asm(node->right);

      load_if_neccessary(node->left);
      load_if_neccessary(node->right);

      Token *left = node->left->token;
      Token *right = node->right->token;
      Token *curr = node->token;

      Value leftValue = left->llvm.elem;
      Value rightRef = right->llvm.elem;

      TypeRef element_type;
      if (left->type == CHARS) {
         element_type = i8;
         curr->retType = CHAR;
      } else {
         element_type = get_llvm_type(left);
         curr->retType = left->type;
      }

      // Add bounds checking if enabled
      if (enable_bounds_check) {
         // We need to track array sizes - for now we'll use a conservative approach
         // For CHARS (strings), we can use strlen at runtime
         // For STACK arrays, we need to store the size

         Value size_val = NULL;

         if (left->type == CHARS) {
            // For strings, we need to get the length
            // First check if it's a string literal or a variable
            if (left->name && !left->llvm.array_size) {
               // It's a variable - we need strlen
               Value strlen_func = llvm_get_named_function("strlen");
               if (!strlen_func) {
                  TypeRef strlen_type = llvm_function_type(i64, (TypeRef[]) {p8}, 1, false);
                  strlen_func = llvm_add_function("strlen", strlen_type);
               }
               Value strlen_result = llvm_build_call2(NULL,
                                                      llvm_global_get_value_type(strlen_func), strlen_func,
               (Value[]) {leftValue}, 1, "strlen");
               size_val = llvm_build_trunc(NULL, strlen_result, i32, "size");
            }
            else if (left->llvm.array_size)
            {
               size_val = left->llvm.array_size;
            }
            else
            {
               // String literal - we know the size at compile time
               // This is handled in _chars function
               size_val = llvm_const_int(i32, strlen(left->Chars.value), 0);
            }
         } else if (left->type == STACK) {
            // For STACK arrays, we should have stored the size
            // For now, use a placeholder - you'll need to enhance this
            size_val = llvm_const_int(i32, 1000000, 0); // Conservative large value
         }

         if (size_val) {
            // Get bounds check function
            Value bounds_check = llvm_get_named_function("__bounds_check");

            // Create filename string
            static Value filename_str = NULL;

            if (!filename_str) {
               char filename[256] = {0};

               if (getcwd(filename, sizeof(filename)) != NULL) {
                  size_t len = strlen(filename);

                  snprintf(
                     filename + len,
                     sizeof(filename) - len,
                     "/%s",
                     "input_file"   // actual filename
                  );

                  filename_str = llvm_build_global_string_ptr_raw(
                                    filename,
                                    "filename"
                                 );
               }
            }


            // Call bounds check: __bounds_check(index, size, line, filename)
            Value line_val = llvm_const_int(i32, curr->line, 0);
            llvm_build_call2(NULL, llvm_global_get_value_type(bounds_check), bounds_check,
            (Value[]) {rightRef, size_val, line_val, filename_str}, 4, "");
         }
      }

      Value indices[] = { rightRef };
      Value gep = llvm_build_gep2(curr, element_type, leftValue, indices, 1, "ACCESS");
      curr->llvm.elem = gep;

      break;
   }
   // TODO: check if it has to be removed
   case END_BLOC: exit_scoop(); break;
   default:
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
}

bool validate_ref_assignment(Token *left, Token *right)
{
   if (!left->is_ref && !right->is_ref) return true;
   if (left->is_ref && !right->is_ref) return true;

   if (left->is_ref && right->is_ref)
   {
      if (!right->has_ref)
      {
         fprintf(stderr, RED "Error:" RESET " Cannot assign uninitialized reference '%s' to '%s'\n",
                 right->name, left->name);
         fprintf(stderr, "  at %s:%d\n", right->filename, right->line);
         return false;
      }
      return true;
   }

   if (!left->is_ref && right->is_ref)
   {
      if (!right->has_ref)
      {
         fprintf(stderr, RED "Error:" RESET " Cannot use uninitialized reference '%s'\n",
                 right->name);
         fprintf(stderr, "  at %s:%d\n", right->filename, right->line);
         return false;
      }
      return true;
   }

   return false;
}

void generate_ir(Node *node)
{
   if (found_error) return;
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
      if (node->token->is_dec) new_variable(node->token);
      // node->token->retType = node->token->type;
      break;
   }
   case ASSIGN:
   {
      // TODO: check compatibility
      generate_ir(left);
      generate_ir(right);
      check(!validate_ref_assignment(left->token, right->token), "");

      if (left->token->is_ref)
      {
         if (right->token->is_ref) left->token->has_ref = right->token->has_ref;
         else left->token->has_ref = true;
      }

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
   case WHILE:
   {
      enter_scoop(node);
      generate_ir(node->left); // condition
      // code bloc
      for (int i = 0; i < node->cpos; i++)
         generate_ir(node->children[i]);
      exit_scoop();
      break;
   }
   case IF:
   {
      enter_scoop(node);
      Node *curr = node;
      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
      {
         if (includes(curr->token->type, IF, ELIF, 0))
         {
            check(curr->left == NULL, "error");
            generate_ir(curr->left); // condition
         }
         // code bloc
         for (int i = 0; i < curr->cpos; i++)
            generate_ir(curr->children[i]);
         curr = curr->right;
      }

      exit_scoop();
      break;
   }
   case FDEC:
   {
      new_function(node);
      enter_scoop(node);

      // parameters
      Node **params = (node->left ? node->left->children : NULL);
      for (int i = 0; params && i < node->left->cpos && !found_error; i++)
         generate_ir(params[i]);

      // code bloc
      for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
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
      if (strcmp(node->token->name, "stack") == 0)
      {
         // node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
         // node->token->Fcall.pos = node->cpos;

         node->token->retType = CHARS;
         node->token->type = STACK;

         Node *call_args = node->left;

         // if (check(call_args->cpos != dec_args->cpos && !node->token->is_variadic, "Incompatible number of arguments %s", func->token->name))
         //    return NULL;

         Node *carg = call_args->children[0];
         generate_ir(carg);
         Token *src = carg->token;
         if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;

         // Node *call_args = node;

         // for (int i = 0; !found_error && i < call_args->cpos; i++)
         // {
         //    Node *carg = call_args->children[i]; // will always be ID
         //    Token *src = generate_ir(carg);
         //    if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
         //    node->token->Fcall.args[i] = src;
         //    src->space = node->token->space;
         // }
         // inst = new_inst(node->token);
      }
      else
      {
         Node *func = get_function(node->token->name);
         if (!func) return;
         node->token->Fcall.ptr = func->token;
         // TODO: I guess declaring params mut be here
         // node->token->Fcall.args = allocate(node->cpos, sizeof(Token*));
         // node->token->Fcall.pos = node->cpos;

         func = copy_node(func);
         // node->token->retType = func->token->retType;
         // node->token->is_variadic = func->token->is_variadic;

         Node *call_args = node->left;
         Node *dec_args = func->left;

         // if (check(call_args->cpos != dec_args->cpos && !node->token->is_variadic, "Incompatible number of arguments %s", func->token->name))
         //    return NULL;

         for (int i = 0; !found_error && call_args && i < call_args->cpos; i++)
         {
            Node *carg = call_args->children[i];
            generate_ir(carg);
            Token *src = carg->token;
            if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
            if (i < dec_args->cpos)
            {
               Node *darg = dec_args->children[i];

               // if (check(!compatible(src, dist), "Incompatible type arg %s", func->token->name)) break;
               src->is_ref = darg->token->is_ref;
               src->has_ref = false;
            }
            // node->token->Fcall.args[i] = src;
         }
         free_node(func);
      }

      break;
   }
   case AS:
   {
      generate_ir(node->left);
      node->token->retType = node->right->token->type;
      break;
   }
   case ACCESS:
   {
      generate_ir(node->left);
      generate_ir(node->right);
      Type retType = 0;
      switch (node->left->token->type)
      {
      case CHARS: retType = CHAR; break;
      default:
         check(1, "handle this case %s", to_string(node->left->token->type));
         break;
      }
      node->token->retType = retType;
      break;
   }
   case RETURN:
   {
      generate_ir(node->left);
      node->token->retType = node->left->token->type;
      break;
   }
   case BREAK: case CONTINUE: break;
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
}