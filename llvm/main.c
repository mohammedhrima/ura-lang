#include "./utils.c"
#include "./wrapper.c"

Node *expr_node()
{
   return assign_node();
}

Node *assign_node()
{
   Node *left = logic_node();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, 0)))
   {
      Node *node = new_node(token);
      if (token->type == ASSIGN)
      {
         node->left = left;
         node->right = logic_node();
      }
      else
      {
         node->left = left;
         Type types[END] = {
            [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB,
            [MUL_ASSIGN] = MUL, [DIV_ASSIGN] = DIV,
            [MOD_ASSIGN] = MOD
         };
         Type type = types[token->type];
         node->right = new_node(new_token(type, 0, 0, 0, 0, node->token->space));
         node->right->left = new_node(left->token);
         node->right->right = logic_node();
         node->token->type = ASSIGN;
      }
      left = node;
   }
   return left;
}

AST_NODE(logic_node, equality_node, AND, OR, 0);
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison_node, add_sub_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL, 0);
AST_NODE(add_sub_node, mul_div_node, ADD, SUB, 0);
AST_NODE(mul_div_node, cast_node, MUL, DIV, MOD, 0);

Node *cast_node()
{
   Node *left = minus_node();
   Token *token = NULL;
   while ((token = find(AS, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      Token *rtoken = find(DATA_TYPES, 0);
      check(!rtoken, "");
      node->right = new_node(rtoken);
      left = node;
   }
   return left;
}

Node *minus_node()
{
   Token *token = NULL;
   if ((token = find(SUB, 0)))
   {
      Node *node = new_node(token);
      node->token->type = MUL;
      node->left = expr_node();
      node->right = new_node(copy_token(token));
      node->right->token->type = INT;
      node->right->token->Int.value = -1;
      return node;
   }
   return prime_node();
}

Node *prime_node()
{
   Token *token;
   Node *node = NULL;
   if ((token = find(REF, 0)))
   {
      node = prime_node();
      check(!node->token->is_dec, "");
      node->token->is_ref = true;
      node->token->has_ref = false;
      return node;
   }
   else if ((token = find(DATA_TYPES, ID, 0)))
   {
      if (token->is_dec)
      {
         Token *fname = find(ID, 0);
         check(!fname, "");
         token->name = fname->name;
         fname->name = NULL;
         return new_node(token);
      }
      else if (find(LPAR, 0))
      {
         token->type = FCALL;
         Token *arg = NULL;
         int len = 0;
         int p = 0;

         do {
            arg = find(RPAR, END, 0);
            if (arg) break;

            arg = prime_node()->token;
            if (len == 0)
            {
               len = 10;
               token->Fcall.args = calloc(len, sizeof(Token*));
            }
            else if (p + 1 == len)
            {
               Token **tmp = calloc(len *= 2, sizeof(Token*));
               memcpy(tmp, token->Fcall.args, p *  sizeof(Token*));
               free(token->Fcall.args);
               token->Fcall.args = tmp;
            }

            token->Fcall.args[p++] = arg;
            arg = find(RPAR, END, 0);
            if (arg) break;
            check(!find(COMA, 0), "");
         } while (true);

         token->Fcall.len = p;
         return new_node(token);
      }
      else if (find(LBRA, 0))
      {
         node = new_node(tokens[exe_pos - 1]);
         node->token->type = ACCESS;
         node->left = new_node(token);
         node->right = expr_node();
         check(!find(RBRA, 0), "");
         return node;
      }
      else if (find(DOT, 0))
      {
         Token *dot = tokens[exe_pos - 1];
         Token *member = find(ID, 0);
         check(!member, "Expected member name after '.'");
         node = new_node(dot);

         node->left = new_node(token);
         node->right = new_node(member);
         return node;
      }
      return new_node(token);
   }
   else if ((token = find(FDEC, PROTO, 0)))
   {
      node = new_node(token);
      Token *ret = find(DATA_TYPES, 0);
      check(!ret, "");
      node->token->Fdec.retType = ret->type;
      Token *fname = find(ID, 0);
      check(!fname, "");
      node->token->name = fname->name;
      fname->name = NULL;
      check(!find(LPAR, 0), "");
      Token *arg = NULL;
      int len = 0;
      int p = 0;
      do {
         arg = find(RPAR, END, 0);
         if (arg) break;

         if (len == 0)
         {
            len = 10;
            node->token->Fdec.args = calloc(len, sizeof(Token*));
         }
         else if (p + 1 == len)
         {
            Token **tmp = calloc(len *= 2, sizeof(Token*));
            memcpy(tmp, node->token->Fdec.args, p *  sizeof(Token*));
            free(node->token->Fdec.args);
            node->token->Fdec.args = tmp;
         }

         arg = prime_node()->token;
         if (arg->type == VARIADIC)
         {
            Token *va_param = find(ID, 0);
            check(!va_param, "Expected identifier after ...");
            va_param->type = VA_LIST;

            node->token->Fdec.args[p++] = va_param;
            node->token->Fdec.is_variadic = true;

            arg = find(RPAR, END, 0);
            break;
         }

         node->token->Fdec.args[p++] = arg;
         arg = find(RPAR, END, 0);
         if (arg) break;
         check(!find(COMA, 0), "");
      } while (true);

      node->token->Fdec.len = p;
      check(!arg || arg->type != RPAR, "");

      if (node->token->type != PROTO)
      {
         check(!find(DOTS, 0), "");
         while (!(token = find(END_BLOCK, END, 0)))
            add_child_node(node, expr_node());
         check(token->type != END_BLOCK, "");
      }
      return node;
   }
   else if ((token = find(WHILE, 0)))
   {
      node = new_node(token);
      node->left = expr_node();
      check(!find(DOTS, 0), "");
      while (!(token = find(END_BLOCK, END, 0)))
         add_child_node(node, expr_node());
      check(token->type != END_BLOCK, "");
      return node;
   }
   else if ((token = find(IF, 0)))
   {
      node = new_node(token);
      node->left = expr_node();
      check(!find(DOTS, 0), "");
      while (!(token = find(END_BLOCK, END, 0)))
         add_child_node(node, expr_node());
      check(token->type != END_BLOCK, "");
      Node *curr = node;

      while ((token = find(ELIF, 0)))
      {
         curr->right = new_node(token);
         curr = curr->right;
         curr->left = expr_node();
         check(!find(DOTS, 0), "");
         while (!(token = find(END_BLOCK, END, 0)))
            add_child_node(curr, expr_node());
         check(token->type != END_BLOCK, "");
      }
      if ((token = find(ELSE, 0)))
      {
         curr->right = new_node(token);
         curr = curr->right;
         check(!find(DOTS, 0), "");
         while (!(token = find(END_BLOCK, END, 0)))
            add_child_node(curr, expr_node());
         check(token->type != END_BLOCK, "");
      }
      return node;
   }
   else if ((token = find(TRY, 0)))
   {
      node = new_node(token);
      check(!find(DOTS, 0), "Expected ':' after try");

      while (true)
      {
         if (node->token->space >= tokens[exe_pos + 1]->space)
            break;
         Node *stmt = expr_node();
         add_child_node(node, stmt);
      }

      check(!(token = find(CATCH, 0)),
            "Expected 'catch' after try block, got %s",
            to_string(tokens[exe_pos]->type));

      Token *catch_type = find(DATA_TYPES, 0);
      check(!catch_type, "Expected type after 'catch'");

      Token *catch_name = find(ID, 0);
      check(!catch_name, "Expected variable name after catch type");

      token->type = CATCH;
      token->Catch.type = catch_type->type;
      token->Catch.name = catch_name->name;

      node->right = new_node(token);

      check(!find(DOTS, 0), "Expected ':' after catch parameter");

      while (!(token = find(END_BLOCK, END, 0)))
         add_child_node(node->right, expr_node());

      check(!token || token->type != END_BLOCK, "Expected 'end' after catch block");

      return node;
   }
   else if ((token = find(THROW, 0)))
   {
      node = new_node(token);
      node->left = expr_node();
      return node;
   }
   else if ((token = find(RETURN, 0)))
   {
      node = new_node(token);
      node->left = expr_node();
      return node;
   }
   else if ((token = find(LPAR, 0)))
   {
      node = expr_node();
      check(!find(RPAR, 0), "");
      return node;
   }
   else if ((token = find(VARIADIC, 0))) return new_node(token);
   else if ((token = find(STACK)))
   {
      node = new_node(token);
      node->left = prime_node();
      return node;
   }
   check(1, "handle this case [%s]", to_string(tokens[exe_pos]->type));
   return NULL;
}

void _fdec(Token *token)
{
   TypeRef retType = get_llvm_type(token);

   int param_count = token->Fdec.len;
   int param_count1 = param_count;

   if (token->Fdec.is_variadic)
   {
      param_count--;
      param_count1 = param_count + 1;
   }

   TypeRef *paramTypes = calloc(param_count1 + 1, sizeof(TypeRef));

   for (int i = 0; i < param_count; i++)
   {
      Token *param = token->Fdec.args[i];

      if (param->is_ref)
      {
         TypeRef base_type = get_llvm_type(param);
         paramTypes[i] = llvm_pointer_type(base_type, 0);
      }
      else
         paramTypes[i] = get_llvm_type(param);
   }

   // Hidden count parameter (after regular params, before varargs)
   if (token->Fdec.is_variadic)
      paramTypes[param_count] = i32;

   TypeRef funcType = llvm_function_type(retType, paramTypes, param_count1, token->Fdec.is_variadic);

   Value existingFunc = llvm_get_named_function(token->llvm_name ? token->llvm_name : token->name);
   if (existingFunc) {
      token->llvm.elem = existingFunc;
   } else {
      token->llvm.elem = llvm_add_function(token->llvm_name ? token->llvm_name : token->name,
                                           funcType);
   }
}

void _entry(Token *token)
{
   Block entry = llvm_append_basic_block_in_context(token->llvm.elem, "entry");
   llvm_position_builder_at_end(entry);
}

void _return(Token *token)
{
   Value value = token->llvm.elem;
   if (value) llvm_build_ret(token, value);
   else llvm_build_ret_void(token);
}

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

void _load(Token *to, Token *from)
{
   TypeRef type = get_llvm_type(from);

   char *name = to->name ? to->name : to_string(to->type);
   Value source = from->llvm.elem;

   to->llvm.elem = llvm_build_load2(to, type, source, name);
}

void _op(Token *token, Token *left, Token *right)
{
   Value l = left->llvm.elem;
   Value r = right->llvm.elem;
   Value elem = NULL;
   switch (token->type)
   {
   case ADD: elem = llvm_build_add(token, l, r, "add"); break;
   case SUB: elem = llvm_build_sub(token, l, r, "sub"); break;
   case EQUAL: elem = llvm_build_icmp(token, LLVMIntEQ, l, r, "equal"); break;
   case NOT_EQUAL: elem = llvm_build_icmp(token, LLVMIntNE, l, r, "not_equal"); break;
   case LESS: elem = llvm_build_icmp(token, LLVMIntSLT, l, r, "less"); break;
   case GREAT: elem = llvm_build_icmp(token, LLVMIntSGT, l, r, "more"); break;
   case LESS_EQUAL: elem = llvm_build_icmp(token, LLVMIntSLE, l, r, "less_equal"); break;
   case GREAT_EQUAL: elem = llvm_build_icmp(token, LLVMIntSGE, l, r, "more_equal"); break;
   case MUL: elem = llvm_build_mul(token, l, r, "mul"); break;
   case DIV: elem = llvm_build_sdiv(token, l, r, "div"); break;
   case MOD: elem = llvm_build_srem(token, l, r, "mod"); break;
   case AND: elem = llvm_build_and(token, l, r, "and"); break;
   case OR: elem = llvm_build_or(token, l, r, "or"); break;
   default:
      check(stderr, "Unknown operator: %s\n", to_string(token->type));
      break;
   }
   token->llvm.elem = elem;
}

void _branch(Block bloc)
{
   if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
      llvm_build_br(NULL, bloc);
}

void _position_at(Block bloc)
{
   llvm_position_builder_at_end(bloc);
}

void _condition(Value cond, Block isTrue, Block isFalse)
{
   llvm_build_cond_br(NULL, cond, isTrue, isFalse);
}

Block _append_block(char *name)
{
   char block_name[256];
   snprintf(block_name, sizeof(block_name), "%s.%d", name, block_counter++);
   return llvm_append_basic_block_in_context(llvm_get_basic_block_parent(llvm_get_insert_block()),
          block_name);
}

void _access(Token *curr, Token *left, Token *right)
{
   Value leftValue = left->llvm.elem;
   Value rightRef = right->llvm.elem;

   TypeRef element_type;
   if (left->type == CHARS) {
      element_type = i8;
      curr->rTypeRef = CHAR;
   } else {
      element_type = get_llvm_type(left);
      curr->rTypeRef = left->type;
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
                  input_file   // actual filename
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
}

void _cast(Token *to, Token *from, TypeRef target_type)
{
   Value source = from->llvm.elem;
   TypeRef source_type = llvm_type_of(source);

   if (source_type == target_type)
   {
      to->llvm.elem = source;
      return;
   }

   LLVMTypeKind source_kind = llvm_get_type_kind(source_type);
   LLVMTypeKind target_kind = llvm_get_type_kind(target_type);

   if (source_kind == LLVMPointerTypeKind && target_kind == LLVMPointerTypeKind)
      to->llvm.elem = llvm_build_bit_cast(to, source, target_type, "cast");
   else if (source_kind == LLVMIntegerTypeKind && target_kind == LLVMIntegerTypeKind)
   {
      unsigned source_bits = llvm_get_int_type_width(source_type);
      unsigned target_bits = llvm_get_int_type_width(target_type);

      if (source_bits < target_bits)
         to->llvm.elem = llvm_build_sext(to, source, target_type, "cast"); // sign extend
      else if (source_bits > target_bits)
         to->llvm.elem = llvm_build_trunc(to, source, target_type, "cast"); // truncate
      else
         to->llvm.elem = source;
   }
   else if (source_kind == LLVMIntegerTypeKind && target_kind == LLVMPointerTypeKind)
      to->llvm.elem = llvm_build_int_to_ptr(to, source, target_type, "cast");
   else if (source_kind == LLVMPointerTypeKind && target_kind == LLVMIntegerTypeKind)
      to->llvm.elem = llvm_build_ptr_to_int(to, source, target_type, "cast");
   else
      check(1, "Unsupported cast");
}

void enter_scoop(Node *node)
{
   scoop_pos++;
   Gscoop[scoop_pos] = node;
   curr_scoop = Gscoop[scoop_pos];
}

void exit_scoop()
{
   scoop_pos--;
   if (scoop_pos >= 0)
      curr_scoop = Gscoop[scoop_pos];
}

void add_function(Node *node)
{
   printf("Adding function %s to scope %d\n", node->token->name, scoop_pos);
   if (curr_scoop->flen == 0)
   {
      curr_scoop->flen = 100;
      curr_scoop->functions = calloc(100, sizeof(Node *));
   }
   else if (curr_scoop->fpos + 1 == curr_scoop->flen)
   {
      Node **tmp = calloc((curr_scoop->flen *= 2), sizeof(Node*));
      memcpy(tmp, curr_scoop->functions, curr_scoop->fpos * sizeof(Node*));
      free(curr_scoop->functions);
      curr_scoop->functions = tmp;
   }
   curr_scoop->functions[curr_scoop->fpos++] = node;
}

Token *get_function(char *name)
{
   for (int j = scoop_pos; j >= 0; j--)
   {
      Node *curr = Gscoop[j];
      if (!curr) continue;
      for (int i = 0; i < curr->fpos; i++)
      {
         Token *token = curr->functions[i]->token;
         if (strcmp(token->name, name) == 0) return token;
      }
   }
   check(1, "function [%s] not found", name);
   seg();
   return NULL;
}

void add_variable(Token *token)
{
   if (curr_scoop->vlen == 0)
   {
      curr_scoop->vlen = 100;
      curr_scoop->variables = calloc(100, sizeof(Token));
   }
   else if (curr_scoop->vpos + 1 == curr_scoop->vlen)
   {
      Token** tmp = calloc((curr_scoop->vlen *= 2), sizeof(Token*));
      memcpy(tmp, curr_scoop->variables, curr_scoop->vpos * sizeof(Token*));
      free(curr_scoop->variables);
      curr_scoop->variables = tmp;
   }
   curr_scoop->variables[curr_scoop->vpos++] = token;
}

Token *get_variable(char *name)
{
   for (int j = scoop_pos; j >= 0; j--)
   {
      Node *curr = Gscoop[j];
      if (!curr) continue;
      for (int i = 0; i < curr->vpos; i++)
      {
         Token *token = curr->variables[i];
         if (strcmp(name, token->name) == 0) return token;
      }
   }
   check(1, "variable [%s] not found", name);
   return NULL;
}

void load_if_neccessary(Node *node)
{
   Token *token = node->token;

   if (token->is_loaded || includes(token->type, CHARS, STACK, 0))
      return;

   if (token->name && token->type != FCALL)
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->is_loaded = true;
      node->token = new;
   }
   else if (includes(token->type, ACCESS, ADD, SUB, MUL, DIV, 0))
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->is_loaded = true;
      node->token = new;
   }
}

int get_va_list_size(LLVMModuleRef module)
{
   const char *triple = llvm_get_target(module);

   if (strstr(triple, "x86_64") || strstr(triple, "amd64"))
   {
      // x86-64 System V ABI
      if (strstr(triple, "win") || strstr(triple, "windows") || strstr(triple, "msvc"))
         return 8;   // Windows x64
      else
         return 24;  // Linux/Unix x86-64
   }
   else if (strstr(triple, "aarch64") || strstr(triple, "arm64"))
      return 32;  // ARM64
   else if (strstr(triple, "i386") || strstr(triple, "i686"))
      return 4;   // 32-bit x86
   else if (strstr(triple, "arm"))
      return 4;   // 32-bit ARM

   fprintf(stderr, "Warning: Unknown target '%s', defaulting to 24 bytes for va_list\n", triple);
   return 24;
}

TypeRef get_llvm_type(Token *token)
{
   Type type = token->type;
   if (includes(type, FDEC, PROTO, 0)) type = token->Fdec.retType;
   TypeRef res[END] = {[INT] = i32, [CHAR] = i8, [CHARS] = p8,
                     [BOOL] = i1, [VOID] = vd, [VA_LIST] = p8,
                     [ACCESS] = i8, [CATCH] = i32,
                    };

   check(!res[type], "handle this case [%s]\n", to_string(type));
   return res[type];
}

void generate_ir(Node *node)
{
   printf("%s: %s\n", __func__, to_string(node->token->type));
   switch (node->token->type)
   {
   case ID:
   {
      node->token = get_variable(node->token->name);
      break;
   }
   case INT: case CHARS: case CHAR:
   {
      if (node->token->is_dec)
      {
         _alloca(node->token);
         add_variable(node->token);
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
      {
         return;
      }
      void (*funcs[END])(Token *) = {[INT] = &_int, [CHAR] = &_char, [CHARS] = &_chars};
      void (*func)(Token *) = funcs[node->token->type];
      check(!func, "handle this case [%s]\n", to_string(node->token->type));
      func(node->token);
      break;
   }
   case AND: case OR:
   case ADD: case SUB: case MUL: case DIV: case MOD: case LESS:
   case GREAT: case EQUAL: case LESS_EQUAL: case GREAT_EQUAL:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);
      generate_ir(node->right);
      load_if_neccessary(node->right);
      _op(node->token, node->left->token, node->right->token);
      node->token->type = node->left->token->type;
      break;
   }
   case ASSIGN:
   {
      generate_ir(node->left);
      generate_ir(node->right);

      Token *left = node->left->token;
      Token *right = node->right->token;

      check(right->is_ref && !right->has_ref, "Error: cannot assign from unbound reference");

      // Propagate array size information
      if (right->llvm.array_size) {
         left->llvm.array_size = right->llvm.array_size;
      }

      if (!left->is_ref && !right->is_ref)
      {
         if (left->type == CHARS && (right->type == CHARS && !right->name)) {
            llvm_build_store(node->token, right->llvm.elem, left->llvm.elem);
            node->token->llvm.elem = left->llvm.elem;
         }
         else {
            load_if_neccessary(node->right);
            llvm_build_store(node->token, right->llvm.elem, left->llvm.elem);
            right->llvm.elem = left->llvm.elem;
         }
      }
      else if (left->is_ref && !right->is_ref)
      {
         if (!left->has_ref)
         {
            left->llvm.elem = right->llvm.elem;
            left->has_ref = true;
            left->type = right->type;
            node->token->llvm.elem = left->llvm.elem;
         }
         else
         {
            load_if_neccessary(node->right);
            llvm_build_store(node->token, right->llvm.elem, left->llvm.elem);
            node->token->llvm.elem = left->llvm.elem;
         }
      }
      else if (!left->is_ref && right->is_ref)
      {
         load_if_neccessary(node->right);
         llvm_build_store(node->token, node->right->token->llvm.elem, left->llvm.elem);
         node->token->llvm.elem = left->llvm.elem;
      }
      else if (left->is_ref && right->is_ref)
      {
         if (!left->has_ref)
         {
            left->llvm.elem = right->llvm.elem;
            left->has_ref = true;
            left->type = right->type;
            node->token->llvm.elem = left->llvm.elem;
         }
         else
         {
            Token *temp = copy_token(right);
            _load(temp, right);
            llvm_build_store(node->token, temp->llvm.elem, left->llvm.elem);
            node->token->llvm.elem = left->llvm.elem;
         }
      }

      break;
   }
   case FDEC: case PROTO:
   {
      if (scoop_pos > 0)
      {
         static int nested_id = 0;
         char new_name[256];
         snprintf(new_name, sizeof(new_name), "%s.%s.%d", curr_scoop->token->name, node->token->name,
                  nested_id++);
         node->token->llvm_name = strdup(new_name);
      }
      else
      {
         node->token->llvm_name = strdup(node->token->name);
      }

      add_function(node);
      enter_scoop(node);
      _fdec(node->token);

      if (node->token->type == FDEC)
      {
         for (int i = 0; i < node->cpos; i++)
         {
            if (node->children[i]->token->type == FDEC)
               generate_ir(node->children[i]);
         }

         _entry(node->token);

         int param_idx = 0;
         for (int i = 0; i < node->token->Fdec.len; i++)
         {
            Token *param = node->token->Fdec.args[i];

            if (param->type == VA_LIST)
            {
               Value count_param = llvm_get_param(node->token->llvm.elem, param_idx);
               param_idx++;

               Value count_alloca = llvm_build_alloca(param, i32, "va_count");
               llvm_build_store(param, count_param, count_alloca);
               param->llvm.va_count = count_alloca;

               int va_list_size = get_va_list_size(module);

               TypeRef va_list_type = llvm_get_type_by_name2("struct.__va_list_tag");
               if (!va_list_type) {
                  va_list_type = llvm_array_type(i8, va_list_size);
               }

               Value va_list_alloca = llvm_build_alloca(param, va_list_type, param->name);
               param->llvm.elem = va_list_alloca;

               Value va_start = llvm_get_named_function("llvm.va_start.p0");
               if (!va_start) {
                  TypeRef params[] = { llvm_pointer_type(i8, 0) };
                  TypeRef va_start_type = llvm_function_type(vd, params, 1, false);
                  va_start = llvm_add_function("llvm.va_start.p0", va_start_type);
               }

               Value gep = llvm_build_gep2(param, llvm_array_type(i8, va_list_size),
                                            va_list_alloca,
               (Value[]) {llvm_const_int(i32, 0, 0)}, 1, "");
               Value cast = llvm_build_bit_cast(param, gep, llvm_pointer_type(i8, 0), "");
               llvm_build_call2(param, llvm_global_get_value_type(va_start), va_start, &cast, 1, "");

               add_variable(param);
            }
            else if (param->is_ref)
            {
               Value p = llvm_get_param(node->token->llvm.elem, param_idx);
               param_idx++;
               param->llvm.elem = p;
               param->has_ref = true;
               add_variable(param);
            }
            else
            {
               Value p = llvm_get_param(node->token->llvm.elem, param_idx);
               param_idx++;

               Value alloca = llvm_build_alloca(param, get_llvm_type(param), "param");
               llvm_build_store(param, p, alloca);
               param->llvm.elem = alloca;
               add_variable(param);
            }
         }
         for (int i = 0; i < node->cpos; i++)
         {
            if (node->children[i]->token->type != FDEC)
               generate_ir(node->children[i]);
         }

         if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
         {
            if (node->token->Fdec.retType == VOID)
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
   case STACK:
   {
      pnode(node, NULL, 0);
      generate_ir(node->left);

      Value size_value = node->left->token->llvm.elem;

      TypeRef size_type = llvm_type_of(size_value);
      if (llvm_get_type_kind(size_type) != LLVMIntegerTypeKind) {
         check(1, "stack() size must be an integer, got %d", llvm_get_type_kind(size_type));
      }

      Value stack_alloc = llvm_build_array_alloca(node->token, i8, size_value, "stack");

      node->token->llvm.elem = stack_alloc;
      node->token->type = STACK;

      // Store the size for bounds checking
      node->token->llvm.array_size = size_value;

      break;
   }
   case FCALL:
   {
      Token *funcToken = get_function(node->token->name);

      for (int i = 0; i < node->token->Fcall.len; i++) {
         Node *nodeArg = new_node(node->token->Fcall.args[i]);
         generate_ir(nodeArg);

         bool should_load = true;

         if (i < funcToken->Fdec.len && !funcToken->Fdec.is_variadic) {
            should_load = !funcToken->Fdec.args[i]->is_ref;
         }
         else {
            if (nodeArg->token->type == CHARS && !nodeArg->token->name) {
               should_load = false;
            }
         }

         if (should_load) {
            load_if_neccessary(nodeArg);
         }

         node->token->Fcall.args[i] = nodeArg->token;
         free(nodeArg);
      }


      Token *token = node->token;
      Value func = funcToken->llvm.elem;

      int arg_count = token->Fcall.len;
      Value *args = NULL;

      if (funcToken->Fdec.is_variadic) {
         int fixed_params = funcToken->Fdec.len - 1;
         int variadic_count = arg_count - fixed_params;
         args = calloc(arg_count + 1,  sizeof(Value));
         for (int i = 0; i < fixed_params; i++)
            args[i] = token->Fcall.args[i]->llvm.elem;
         args[fixed_params] = llvm_const_int(i32, variadic_count, 0);
         for (int i = fixed_params; i < arg_count; i++)
            args[i + 1] = token->Fcall.args[i]->llvm.elem;
         arg_count++;
      } else {
         args = calloc(arg_count,  sizeof(Value));
         for (int i = 0; i < arg_count; i++)
            args[i] = token->Fcall.args[i]->llvm.elem;
      }

      TypeRef funcType = llvm_global_get_value_type(func);
      TypeRef retType = llvm_get_return_type(funcType);
      char *callName = (llvm_get_type_kind(retType) == LLVMVoidTypeKind) ? "" : token->name;

      ExcepCTX *ctx = get_current_exception_context();
      if (ctx && ctx->lpad) {
         Block normal_dest = _append_block("invoke.cont");
         token->llvm.elem = llvm_build_invoke2(token, funcType, func, args, arg_count,
                                               normal_dest, ctx->lpad, callName);
         llvm_position_builder_at_end(normal_dest);
      } else {
         token->llvm.elem = llvm_build_call2(token, funcType, func, args, arg_count, callName);
      }
      free(args);
      break;
   }
   case WHILE:
   {
      enter_scoop(node);

      Block cond = _append_block("while.cond");
      Block body = _append_block("while.body");
      Block end = _append_block("while.end");

      _branch(cond);
      _position_at(cond);

      generate_ir(node->left);

      _condition(node->left->token->llvm.elem, body, end);

      _position_at(body);
      for (int i = 0; i < node->cpos; i++) generate_ir(node->children[i]);
      _branch(cond);

      _position_at(end);
      exit_scoop();
      break;
   }
   case IF:
   {
      enter_scoop(node);

      Block end = _append_block("if.end");
      Node* curr = node;

      while (curr && (curr->token->type == IF || curr->token->type == ELIF))
      {
         Block then_block = _append_block("if.then");
         Block else_block = _append_block("if.else");
         generate_ir(curr->left);
         _condition(curr->left->token->llvm.elem, then_block, else_block);
         _position_at(then_block);
         for (int i = 0; i < curr->cpos; i++)
            generate_ir(curr->children[i]);
         _branch(end);
         _position_at(else_block);
         if (curr->right && curr->right->token->type == ELIF)
            curr = curr->right;
         else if (curr->right && curr->right->token->type == ELSE)
         {
            for (int i = 0; i < curr->right->cpos; i++)
               generate_ir(curr->right->children[i]);
            _branch(end);
            break;
         }
         else
         {
            _branch(end);
            break;
         }
      }

      _position_at(end);
      exit_scoop();
      break;
   }
   case TRY:
   {
      enter_scoop(node);

      // Get current function from the basic block we're in
      Block current_block = llvm_get_insert_block();
      if (!current_block) {
         check(1, "try-catch must be inside a function");
         return;
      }
      Value current_func = llvm_get_basic_block_parent(current_block);
      if (!current_func) {
         check(1, "try-catch must be inside a function");
         return;
      }

      // Set personality function
      Value personality = get_personality();
      llvm_set_personality_fn(current_func, personality);

      // Create basic blocks
      Block try_block = _append_block("try");
      Block lpad_block = _append_block("lpad");
      Block catch = _append_block("catch");
      Block end = _append_block("try.end");

      // Get catch type
      Type catch_type = node->right ? node->right->token->Catch.type : INT;
      TypeRef catch_llvm_type = get_llvm_type(node->right->token);

      // Allocate storage for caught exception
      Block entry = llvm_get_entry_basic_block(current_func);
      Block saved_pos = llvm_get_insert_block();
      llvm_position_builder_at_end(entry);
      Value storage = llvm_build_alloca(node->token, catch_llvm_type, "exception.storage");
      llvm_position_builder_at_end(saved_pos);

      // Push exception context
      push_exception_context(lpad_block, catch, end, storage, catch_type);

      // Jump to try block
      _branch(try_block);
      llvm_position_builder_at_end(try_block);

      // Generate try block body - all function calls will use invoke
      for (int i = 0; i < node->cpos; i++) {
         generate_ir(node->children[i]);
      }

      // If no exception, jump to end
      _branch(end);

      // Generate landing pad
      llvm_position_builder_at_end(lpad_block);

      // Create landing pad instruction
      TypeRef lpad_type = llvm_struct_type_in_context(
      (TypeRef[]) {p8, i32}, 2, false);
      Value lpad = llvm_build_landing_pad(node->token, lpad_type,
                                           personality, 1, "lpad");

      // Add catch clause for our type
      Value type_info = get_type_info_for_type(catch_type);
      llvm_add_clause(lpad, type_info);

      // Extract exception pointer
      Value exception_ptr = llvm_build_extract_value(node->token, lpad, 0, "exc.ptr");

      // Call __cxa_begin_catch
      Value begin_catch = get_begin_catch();
      Value caught_ptr = llvm_build_call2(node->token,
                                           llvm_global_get_value_type(begin_catch), begin_catch,
                                           &exception_ptr, 1, "caught");

      // Cast and load the exception value
      Value typed_ptr = llvm_build_bit_cast(node->token, caught_ptr,
                                             llvm_pointer_type(catch_llvm_type, 0), "typed.ptr");
      Value exception_value = llvm_build_load2(node->token, catch_llvm_type,
                               typed_ptr, "exception.value");

      // Store in our storage
      llvm_build_store(node->token, exception_value, storage);

      // Jump to catch block
      llvm_build_br(node->token, catch);

      // Generate catch block
      llvm_position_builder_at_end(catch);
      ExcepCTX *ctx_for_catch = get_current_exception_context();
      if (ctx_for_catch) {
         ctx_for_catch->in_catch = true;
      }

      if (node->right) {
         // Create catch variable pointing to storage
         Token *catch_param = calloc(1, sizeof(Token));
         catch_param->type = catch_type;
         catch_param->name = node->right->token->Catch.name;
         catch_param->llvm.elem = storage;
         add_variable(catch_param);

         // Generate catch block body
         for (int i = 0; i < node->right->cpos; i++) {
            generate_ir(node->right->children[i]);
         }
      }

      // Mark that we're no longer in the catch block
      if (ctx_for_catch) {
         ctx_for_catch->in_catch = false;
      }

      // Only add end_catch and branch if there's no terminator
      // (return statements in catch block will have already added end_catch)
      if (!llvm_get_basic_block_terminator(llvm_get_insert_block())) {
         Value end_catch = get_end_catch();
         llvm_build_call2(node->token, llvm_global_get_value_type(end_catch), end_catch, NULL, 0, "");
         llvm_build_br(node->token, end);
      }

      // Position at end block
      llvm_position_builder_at_end(end);

      // Pop exception context
      pop_exception_context();
      exit_scoop();
      break;
   }
   case THROW:
   {
      // Evaluate the expression to throw
      generate_ir(node->left);
      load_if_neccessary(node->left);

      Token *throw_token = node->left->token;
      Type throw_type = throw_token->type;
      TypeRef throw_llvm_type = get_llvm_type(throw_token);

      // Allocate exception object
      Value alloc_exception = get_exception();
      size_t exception_size = llvm_abi_size_of_type(llvm_get_module_data_layout(module), throw_llvm_type);
      Value size_val = llvm_const_int(i64, exception_size, 0);
      Value exception_mem = llvm_build_call2(node->token,
                                              llvm_global_get_value_type(alloc_exception), alloc_exception,
                                              &size_val, 1, "exception");

      // Cast to appropriate pointer type and store value
      Value typed_exception = llvm_build_bit_cast(node->token, exception_mem,
                               llvm_pointer_type(throw_llvm_type, 0), "typed.exception");
      llvm_build_store(node->token, throw_token->llvm.elem, typed_exception);

      // Get type info
      Value type_info = get_type_info_for_type(throw_type);

      // Call __cxa_throw (destructor is NULL for POD types)
      Value cxa_throw = get_throw();
      Value null_dtor = llvm_const_null(p8);
      Value throw_args[] = {exception_mem, type_info, null_dtor};
      llvm_build_call2(node->token, llvm_global_get_value_type(cxa_throw), cxa_throw,
                       throw_args, 3, "");

      // Add unreachable after throw
      llvm_build_unreachable(node->token);

      // Create new block for any potential code after throw (unreachable)
      Block after_throw = _append_block("after.throw");
      llvm_position_builder_at_end(after_throw);
      break;
   }
   case DOT:
   {
      generate_ir(node->left);
      Token *object = node->left->token;
      char *member_name = node->right->token->name;

      if (object->type == VA_LIST)
      {
         if (strcmp(member_name, "elem") == 0)
         {
            check(1, "Error: args.elem requires type cast. Use: args.elem as <type>");
         }
         else if (strcmp(member_name, "len") == 0)
         {
            node->token->llvm.elem = llvm_build_load2(node->token, i32, object->llvm.va_count, "va_len");
            node->token->type = INT;
         }
         else
         {
            check(1, "Unknown va_list member: %s", member_name);
         }
      }
      else
      {
         check(1, "Member access only supported on va_list for now");
      }
      break;
   }
   case AS:
   {
      Token *target_type_token = node->right->token;
      Type target_type_enum = target_type_token->type;
      TypeRef target_type = get_llvm_type(target_type_token);

      if (node->left->token->type == DOT)
      {
         generate_ir(node->left->left);
         Token *object = node->left->left->token;
         char *member_name = node->left->right->token->name;

         if (object->type == VA_LIST && strcmp(member_name, "elem") == 0)
         {
            TypeRef extract_type = get_llvm_type(target_type_token);

            Value va_list_ptr = object->llvm.elem;
            Value bitcast = llvm_build_bit_cast(node->token, va_list_ptr, p8, "va_cast");
            Value result = llvm_build_va_arg(node->token, bitcast, extract_type, "va_arg");

            node->token->llvm.elem = result;
            node->token->type = target_type_enum;
            break;
         }
      }

      generate_ir(node->left);
      load_if_neccessary(node->left);

      Token *source = node->left->token;
      Token *result = copy_token(source);
      _cast(result, source, target_type);
      result->type = target_type_enum;

      node->token->llvm.elem = result->llvm.elem;
      node->token->type = result->type;
      node->token->llvm.array_size = source->llvm.array_size;
      break;
   }
   case RETURN:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);

      ExcepCTX *ctx = get_current_exception_context();
      if (ctx && ctx->in_catch) {
         Value end_catch = get_end_catch();
         llvm_build_call2(node->token, llvm_global_get_value_type(end_catch), end_catch, NULL, 0, "");
      }

      _return(node->left->token);
      break;
   }
   case ACCESS:
   {
      generate_ir(node->left);

      if (node->left->token->type != CHARS) {
         load_if_neccessary(node->left);
      }

      generate_ir(node->right);
      load_if_neccessary(node->right);
      _access(node->token, node->left->token, node->right->token);
      node->token->is_ref = true;
      node->token->has_ref = true;
      break;
   }
   default:
      check(1, "handle this case [%s]\n", to_string(node->token->type));
      break;
   }
}

char *input_file;
int main(int argc, char **argv)
{
   for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--check") == 0) {
         enable_bounds_check = true;
         printf("Bounds checking enabled\n");
      }
   }

   init("module");

   if (enable_bounds_check)
      create_bounds_check_function();


   input_file = NULL;
   for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--check") != 0) {
         input_file = argv[i];
         break;
      }
   }

   if (!input_file) {
      fprintf(stderr, "Error: no input file specified\n");
      fprintf(stderr, "Usage: %s [--check] <input_file>\n", argv[0]);
      return 1;
   }

   input = open_file(input_file);
   tokenize();
   Node *buff[10000] = {};
   int p = 0;
   while (tokens[exe_pos]->type != END) buff[p++] = expr_node();
   enter_scoop(new_node(new_token(END, 0, 0, 0, 0, 0)));
   printf("======================================\n");
   for (int i = 0; buff[i]; i++) generate_ir(buff[i]);
   finalize();
   free_tokens();
   return 0;
}