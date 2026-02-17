#include "./header.h"

// ASSEMBLY GENERATION
void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   if (token->is_ref) {
      type = LLVMPointerType(type, 0);
   }
   token->llvm.elem = llvm_build_alloca(type, token->name);
}

void _const_value(Token *token)
{
   TypeRef type = get_llvm_type(token);
   long long value = 0;
   char *processed = NULL;
   char name[20];

   switch (token->type) {
   case INT:  value = (long long)token->Int.value; break;
   case BOOL: value = (long long)token->Bool.value; break;
   case CHAR: value = (int)token->Char.value; break;
   case CHARS:
   {
      static int index = 0;
      snprintf(name, sizeof(name), "STR%d", index++);

      processed = calloc(strlen(token->Chars.value) * 2 + 1, 1);
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
      break;
   }
   default: check(1, "Invalid constant type"); return;
   }
   if (token->type == CHARS) token->llvm.elem = llvm_build_global_string_ptr(processed, name);
   else token->llvm.elem = llvm_const_int(type, value, 0);
   free(processed);
}

void _return(Token *token)
{
   Value value = token->llvm.elem;
   if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
   {
      if (value) llvm_build_ret(value);
      else llvm_build_ret_void();
   }
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

   // added fo the case of value chars, to be tested with all data types
   if (includes(token->type, MATH_TYPE, 0) || (includes(token->type, DATA_TYPES, 0) && !token->name))
      return;
   // I commented CHARS
   // because when sending it as param
   // the thing that CHARS point to should be loaded
   if (token->llvm.is_loaded || includes(token->type, STACK, /*CHARS,*/ 0))
      return;

   if (token->name && token->type != FCALL)
   {
      Token *new = copy_token(token);

      // Use the deref_or_load helper which handles refs
      if (token->is_ref)
         new->llvm.elem = deref_or_load(token);
      else
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
   case GREAT:        return LLVMBuildICmp(builder, LLVMIntSGT, lref, rref, op);
   case EQUAL:       return LLVMBuildICmp(builder, LLVMIntEQ,  lref, rref, op);
   case LESS_EQUAL:  return LLVMBuildICmp(builder, LLVMIntSLE, lref, rref, op);
   case GREAT_EQUAL:  return LLVMBuildICmp(builder, LLVMIntSGE, lref, rref, op);
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

Value deref_or_load(Token *token)
{
   // It's a constant value, already loaded
   if (!token->name && !token->is_ref) return token->llvm.elem;
   // Already loaded
   if (token->llvm.is_loaded) return token->llvm.elem;
   TypeRef type = get_llvm_type(token);
   if (token->is_ref)
   {
      // It's a reference - need to:
      // 1. Load the pointer from the ref variable
      // 2. Check if null (with check_null)
      // 3. Load the value from that pointer
      Value ptr = check_null(token);
      return llvm_build_load2(type, ptr, token->name ? token->name : "deref");
   }
   // Regular variable - just load it
   return llvm_build_load2(type, token->llvm.elem, token->name);

}

Value get_store_ptr(Token *token)
{
   // Regular variable - return its alloca'd address
   if (!token->is_ref) return token->llvm.elem;
   // Reference - need to load the pointer it points to
   TypeRef type = get_llvm_type(token);
   TypeRef ptr_type = LLVMPointerType(type, 0);
   return llvm_build_load2(ptr_type, token->llvm.elem, "store_ptr");
}

void gen_asm(Node *node)
{
   // debug("Processing: %k\n", inst->token);
   Node *left = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set")) return;
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
            llvm_build_store(null, node->token->llvm.elem);
         }
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
         return;
      _const_value(node->token);
      break;
   }
   case ASSIGN:
   {
      gen_asm(left);
      gen_asm(right);

      // Case 1: Neither is ref - simple assignment
      if (!left->token->is_ref && !right->token->is_ref)
      {
         load_if_neccessary(right);
         llvm_build_store(right->token->llvm.elem, left->token->llvm.elem);
      }
      // Case 2: Left is ref, right is not ref - use ref_assign builtin
      // Case 2: Left is ref, right is not ref
      else if (left->token->is_ref && !right->token->is_ref)
      {
         TypeRef type = get_llvm_type(right->token);

         // Check if right is a simple variable (for direct binding)
         bool is_variable = (right->token->name != NULL &&
                             !right->token->llvm.is_loaded &&
                             right->token->type != CHARS);

         if (is_variable)
         {
            // INITIALIZATION: Direct binding - store address of variable
            llvm_build_store(right->token->llvm.elem, left->token->llvm.elem);
         }
         else
         {
            Value refAssignFunc = getRefAssignFunc();
            // REASSIGNMENT: Use ref_assign for constants/expressions
            Value temp = llvm_build_alloca(type, "ref_temp");
            load_if_neccessary(right);
            llvm_build_store(right->token->llvm.elem, temp);

            TargetData target_data = llvm_get_module_data_layout(module);
            size_t type_size = llvm_abi_size_of_type(target_data, type);

            Value ref_cast = llvm_build_bit_cast(left->token->llvm.elem, p8, "ref");
            Value temp_cast = llvm_build_bit_cast(temp, p8, "val");
            Value args[] = {ref_cast, temp_cast, llvm_const_int(i32, type_size, 0)};
            llvm_build_call2(llvm_global_get_value_type(refAssignFunc), refAssignFunc, args, 3, "");
         }
      }
      // Case 3: Both are refs - copy reference pointer
      // Case 3: Both are refs - copy VALUE, not pointer
      else if (left->token->is_ref && right->token->is_ref)
      {
         Value refAssignFunc = getRefAssignFunc();
         TypeRef type = get_llvm_type(right->token);

         // Load the value from right ref
         Value right_ptr = check_null(right->token);
         Value value = llvm_build_load2(type, right_ptr, "ref_val");

         // Store through left ref using ref_assign
         Value temp = llvm_build_alloca(type, "ref_temp");
         llvm_build_store(value, temp);

         TargetData target_data = llvm_get_module_data_layout(module);
         size_t type_size = llvm_abi_size_of_type(target_data, type);

         Value ref_cast = llvm_build_bit_cast(left->token->llvm.elem, p8, "ref");
         Value temp_cast = llvm_build_bit_cast(temp, p8, "val");
         Value args[] = {ref_cast, temp_cast, llvm_const_int(i32, type_size, 0)};
         llvm_build_call2(llvm_global_get_value_type(refAssignFunc), refAssignFunc, args, 3, "");
      }
      // Case 4: Left is not ref, right is ref - dereference right
      else
      {
         TypeRef type = get_llvm_type(right->token);
         Value ptr = check_null(right->token);
         Value val = llvm_build_load2(type, ptr, "val");
         llvm_build_store(val, left->token->llvm.elem);
      }
      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_asm(left);
      gen_asm(right);

      TypeRef type = get_llvm_type(left->token);
      Value current_val;

      // Load current value from left
      if (left->token->is_ref)
      {
         Value ptr = check_null(left->token);
         current_val = llvm_build_load2(type, ptr, "current");
      }
      else
         current_val = llvm_build_load2(type, left->token->llvm.elem, "current");

      // Load right value
      Value right_val;
      if (right->token->is_ref)
      {
         Value ptr = check_null(right->token);
         right_val = llvm_build_load2(type, ptr, "rval");
      }
      else
      {
         load_if_neccessary(right);
         right_val = right->token->llvm.elem;
      }

      // Perform operation
      Value result;
      switch (node->token->type)
      {
      case ADD_ASSIGN: result = llvm_build_add(current_val, right_val, to_string(ADD)); break;
      case SUB_ASSIGN: result = llvm_build_sub(current_val, right_val, to_string(SUB)); break;
      case MUL_ASSIGN: result = llvm_build_mul(current_val, right_val, to_string(MUL)); break;
      case DIV_ASSIGN: result = llvm_build_sdiv(current_val, right_val, to_string(DIV)); break;
      case MOD_ASSIGN: result = llvm_build_srem(current_val, right_val, to_string(MOD)); break;
      default: break;
      }

      // Store result back
      if (left->token->is_ref)
      {
         Value refAssignFunc = getRefAssignFunc();
         // Use ref_assign with temp
         Value temp = llvm_build_alloca(type, "op_temp");
         llvm_build_store(result, temp);

         TargetData target_data = llvm_get_module_data_layout(module);
         size_t type_size = llvm_abi_size_of_type(target_data, type);

         Value ref_cast = llvm_build_bit_cast(left->token->llvm.elem, p8, "ref");
         Value temp_cast = llvm_build_bit_cast(temp, p8, "val");
         Value args[] = {ref_cast, temp_cast, llvm_const_int(i32, type_size, 0)};
         llvm_build_call2(llvm_global_get_value_type(refAssignFunc), refAssignFunc, args, 3, "");
      }
      else
         llvm_build_store(result, left->token->llvm.elem);
      break;
   }
   case NOT:
   {
      gen_asm(left);
      load_if_neccessary(left);
      node->token->llvm.elem = llvm_build_not(left->token);
      break;
   }
   case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case ADD: case AND: case OR:
   {
      gen_asm(left);
      gen_asm(right);

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
      gen_asm(node->left->children[0]);
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
      bool is_variadic = node->token->Fcall.ptr->is_variadic;
      int count = node->left->cpos;
      Node **argNodes = node->left->children;

      Value *args = NULL;
      if (count)
      {
         int i = 0;
         args = allocate(count + 2, sizeof(Value));
         if (is_variadic)
         {
            Token *args_len = new_token(INT, node->left->token->space + TAB);
            args_len->Int.value = count;
            _const_value(args_len);
            args[i++] = args_len->llvm.elem;
         }
         for (int j = 0; j < count; j++)
         {
            gen_asm(argNodes[j]);
            load_if_neccessary(argNodes[j]);
            args[j + i] = argNodes[j]->token->llvm.elem;
         }
      }
      char *name = node->token->Fcall.ptr->retType != VOID ? node->token->name : "";
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
      else node->token->llvm_name = strdup(node->token->name);

      enter_scoop(node);

      TypeRef retType = get_llvm_type(node->token);
      TypeRef *paramTypes = NULL;
      int param_count = node->left->cpos;

      if (node->left->cpos)
      {
         paramTypes = calloc(param_count + 2, sizeof(TypeRef));
         for (int i = 0; i < param_count; i++)
         {
            Token *param = node->left->children[i]->token;
            if (param->is_ref) paramTypes[i] = llvm_pointer_type(get_llvm_type(param), 0);
            else paramTypes[i] = get_llvm_type(param);
         }
         if (node->token->is_variadic) paramTypes[param_count - 1] = i32;
      }

      TypeRef funcType = llvm_function_type(retType, paramTypes, param_count, node->token->is_variadic);
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
            gen_asm(node->children[i]);
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
      gen_asm(node->left); // condition
      load_if_neccessary(node->left);
      _condition(node->left->token->llvm.elem, then, end);

      _position_at(then);
      for (int i = 0; i < node->cpos; i++)
         gen_asm(node->children[i]);
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
            gen_asm(curr->left); // condition
            load_if_neccessary(curr->left);
            _condition(curr->left->token->llvm.elem, then, next);

            _position_at(then);
            for (int i = 0; i < curr->cpos; i++)
               gen_asm(curr->children[i]);
            _branch(end);

            if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
               curr->right->token->llvm.bloc = next;
         }
         else if (curr->token->type == ELSE)
         {
            _position_at(curr->token->llvm.bloc);
            for (int i = 0; i < curr->cpos; i++)
               gen_asm(curr->children[i]);
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
      if (node->left->token->type != VOID)
      {
         gen_asm(node->left);
         load_if_neccessary(node->left);
      }
      _return(node->left->token);
      break;
   }
   case AS:
   {
      gen_asm(left);
      load_if_neccessary(left);

      Value source = left->token->llvm.elem;
      TypeRef sourceType = LLVMTypeOf(source);
      TypeRef targetType = get_llvm_type(node->right->token);

      TypeKind sourceKind = LLVMGetTypeKind(sourceType);
      TypeKind targetKind = LLVMGetTypeKind(targetType);

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
      gen_asm(node->left);
      gen_asm(node->right);

      Value leftValue;
      if (node->left->token->is_ref)
      {
         todo(1, "stop");
         // If left is a ref, dereference it to get the actual array
         leftValue = deref_or_load(node->left->token);
      }
      else
      {
         // todo(1, "stop");
         load_if_neccessary(node->left);
         leftValue = node->left->token->llvm.elem;
         TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
         if (kind == LLVMPointerTypeKind && node->left->token->name &&
               !node->left->token->llvm.is_loaded &&
               node->left->token->type != STACK)
         {
            leftValue = llvm_build_load2(p8, leftValue, "ptr_load");
         }
      }

      // Load the index (handles refs)
      load_if_neccessary(node->right);
      Value rightRef = node->right->token->llvm.elem;

      TypeRef element_type;
      if (node->left->token->type == CHARS) {
         element_type = i8;
         node->token->retType = CHAR;
      } else {
         element_type = get_llvm_type(left->token);
         node->token->retType = left->token->type;
      }

      // Add bounds checking if enabled
      if (enable_bounds_check) {
         // We need to track array sizes - for now we'll use a conservative approach
         // For CHARS (strings), we can use strlen at runtime
         // For STACK arrays, we need to store the size

         Value size_val = NULL;

         if (node->left->token->type == CHARS) {
            // For strings, we need to get the length
            // First check if it's a string literal or a variable
            if (node->left->token->name && !left->token->llvm.array_size) {
               // It's a variable - we need strlen
               Value strlen_func = llvm_get_named_function("strlen");
               if (!strlen_func) {
                  TypeRef strlen_type = llvm_function_type(i64, (TypeRef[]) {p8}, 1, false);
                  strlen_func = llvm_add_function("strlen", strlen_type);
               }
               Value strlen_result = llvm_build_call2(llvm_global_get_value_type(strlen_func), strlen_func,
               (Value[]) {leftValue}, 1, "strlen");
               size_val = llvm_build_trunc(strlen_result, i32, "size");
            }
            else if (node->left->token->llvm.array_size)
            {
               size_val = node->left->token->llvm.array_size;
            }
            else
            {
               // String literal - we know the size at compile time
               // This is handled in _chars function
               size_val = llvm_const_int(i32, strlen(node->left->token->Chars.value), 0);
            }
         } else if (node->left->token->type == STACK) {
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
                  snprintf(filename + len, sizeof(filename) - len, "/%s", "input_file");
                  filename_str = llvm_build_global_string_ptr_raw(filename, "filename");
               }
            }


            // Call bounds check: __bounds_check(index, size, line, filename)
            Value line_val = llvm_const_int(i32, node->token->line, 0);
            llvm_build_call2(llvm_global_get_value_type(bounds_check), bounds_check,
            (Value[]) {rightRef, size_val, line_val, filename_str}, 4, "");
         }
      }

      Value indices[] = { rightRef };
      Value gep = llvm_build_gep2(element_type, leftValue, indices, 1, "ACCESS");
      node->token->llvm.elem = gep;

      break;
   }
   // TODO: check if it has to be removed
   case END_BLOC: exit_scoop(); break;
   default:
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
}

void gen_ir(Node *node)
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
      break;
   }
   case ASSIGN:
   {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      node->token->retType = left->token->retType;
      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_ir(left);
      gen_ir(right);
      // check(!validate_ref_assignment(left->token, right->token), "");
      node->token->retType = left->token->retType;
      break;
   }
   case NOT:
   {
      gen_ir(left);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case AND: case OR:
   {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);

      check(!left || !left->token, "error in assignment, left is NULL");
      check(!right || !right->token, "error in assignment, right is NULL");

      switch (node->token->type)
      {
      case ADD: case SUB: case MUL: case DIV: case MOD:
         node->token->retType = left->token->retType; break;
      case AND: case OR: case NOT_EQUAL: case EQUAL: case LESS:
      case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
         node->token->retType = BOOL; break;
      default: break;
      }
      break;
   }
   case WHILE:
   {
      enter_scoop(node);
      gen_ir(node->left); // condition
      // code bloc
      for (int i = 0; i < node->cpos; i++) gen_ir(node->children[i]);
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
            gen_ir(curr->left); // condition
         }
         // code bloc
         for (int i = 0; i < curr->cpos; i++)
            gen_ir(curr->children[i]);
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
         gen_ir(params[i]);

      // code bloc
      for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
      {
         Node *child = node->children[i];
         gen_ir(child);
      }
      exit_scoop();
      break;
   }
   case FCALL:
   {
      if (strcmp(node->token->name, "stack") == 0)
      {
         node->token->retType = CHARS;
         node->token->type = STACK;

         Node *call_args = node->left;
         Node *carg = call_args->children[0];
         gen_ir(carg);
         Token *src = carg->token;
         if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
      }
      else
      {
         Node *func = get_function(node->token->name);
         if (!func) return;
         node->token->Fcall.ptr = func->token;
         func = copy_node(func);

         Node *call_args = node->left;
         Node *dec_args = func->left;

         for (int i = 0; !found_error && call_args && i < call_args->cpos; i++)
         {
            Node *carg = call_args->children[i];
            gen_ir(carg);
            Token *src = carg->token;
            if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
            if (i < dec_args->cpos)
            {
               Node *darg = dec_args->children[i];
               src->is_ref = darg->token->is_ref;
            }
         }
         free_node(func);
      }

      break;
   }
   case AS:
   {
      gen_ir(node->left);
      node->token->retType = node->right->token->type;
      break;
   }
   case ACCESS:
   {
      gen_ir(node->left);
      gen_ir(node->right);
      if (found_error) break;
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
      gen_ir(node->left);
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