#include "./utils.c"

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
         Type type = (Type)0;
         switch (token->type)
         {
         case ADD_ASSIGN: type = ADD; break;
         case SUB_ASSIGN: type = SUB; break;
         case MUL_ASSIGN: type = MUL; break;
         case DIV_ASSIGN: type = DIV; break;
         case MOD_ASSIGN: type = MOD; break;
         default: break;
         }
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
AST_NODE(comparison_node, add_sub_node, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub_node, mul_div_node, ADD, SUB, 0);
AST_NODE(mul_div_node, cast_node, MUL, DIV, MOD, 0);

Node *cast_node()
{
   Node *left = prime_node();
   Token *token = NULL;
   while ((token = find(AS, 0)))
   {
      Node *node = new_node(token);
      node->left = left;
      Token * rtoken = find(DATA_TYPES, 0);
      check(!rtoken, "");
      node->right = new_node(rtoken);
      left = node;
   }
   return left;
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

         token->Fcall.args_len = p;
         return new_node(token);
      }
      else if (find(LBRA, 0))
      {
         node = new_node(tokens[exe_pos - 1]);
         node->token->type = ACCESS;
         node->left = new_node(token);
         node->right = prime_node();
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

         // if (find(AS, 0))
         // {
         //    Token *cast_type = find(DATA_TYPES, 0);
         //    check(!cast_type, "Expected type after 'as'");
         //    node->token->cast_type = cast_type->type;
         // }

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

      node->token->Fdec.args_len = p;
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

      // Parse try block body - we need to be very careful here
      while (true)
      {
         // Check if we've hit catch/end before trying to parse
         if (node->token->space >= tokens[exe_pos + 1]->space)
            break;
         printf("===================\n");
         Node *stmt = expr_node();
         add_child_node(node, stmt);
      }

      // Now we should be at CATCH
      if (tokens[exe_pos]->type != CATCH)
      {
         ptoken(tokens[exe_pos]);
         ptoken(tokens[exe_pos + 1]);
         check(1, "Expected 'catch' after try block, got %s", to_string(tokens[exe_pos]->type));
      }

      // Consume CATCH
      exe_pos++;

      // Parse catch parameter: "int error" or "chars msg" etc
      Token *error_type = find(DATA_TYPES, 0);
      check(!error_type, "Expected type after 'catch'");

      Token *error_name = find(ID, 0);
      check(!error_name, "Expected variable name after catch type");

      // Create catch token
      Token *catch_token = new_token(CATCH, 0, 0, 0, 0, 0);
      catch_token->Catch.error_type = error_type->type;
      catch_token->Catch.error_name = error_name->name;
      error_name->name = NULL;

      node->right = new_node(catch_token);

      // Expect colon after catch parameter
      check(!find(DOTS, 0), "Expected ':' after catch parameter");

      // Parse catch block body
      while (!(token = find(END_BLOCK, END, 0)))
         add_child_node(node->right, expr_node());

      check(!token || token->type != END_BLOCK, "Expected 'end' after catch block");

      return node;
   }
   else if ((token = find(THROW, 0)))
   {
      node = new_node(token);
      node->left = expr_node(); // The value to throw
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

void _fcall(Token *token)
{
   Token *funcToken = get_function(token->name);
   LLVMValueRef func = funcToken->llvm.elem;

   int arg_count = token->Fcall.args_len;
   LLVMValueRef *args = NULL;

   if (funcToken->Fdec.is_variadic)
   {
      int fixed_params = funcToken->Fdec.args_len - 1;
      int variadic_count = arg_count - fixed_params;

      args = calloc(arg_count + 1, sizeof(LLVMValueRef));

      // copy fixed params
      for (int i = 0; i < fixed_params; i++)
         args[i] = token->Fcall.args[i]->llvm.elem;
      // insert variadic args count
      args[fixed_params] = LLVMConstInt(i32, variadic_count, 0);
      // copy variadic args
      for (int i = fixed_params; i < arg_count; i++)
         args[i + 1] = token->Fcall.args[i]->llvm.elem;
      arg_count++; // add the variadic_count, to be considered
   }
   else
   {
      args = calloc(arg_count, sizeof(LLVMValueRef));
      for (int i = 0; i < arg_count; i++)
         args[i] = token->Fcall.args[i]->llvm.elem;
   }

   LLVMTypeRef funcType = LLVMGlobalGetValueType(func);
   LLVMTypeRef retType = LLVMGetReturnType(funcType);
   char *callName = (LLVMGetTypeKind(retType) == LLVMVoidTypeKind) ? "" : token->name;
   token->llvm.elem = LLVMBuildCall2(builder, funcType, func, args, arg_count, callName);
   free(args);
}


void _fdec(Token *token)
{
   TypeRef retType = get_llvm_type(token);

   int param_count = token->Fdec.args_len;
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
         paramTypes[i] = LLVMPointerType(base_type, 0);
      }
      else
         paramTypes[i] = get_llvm_type(param);
   }

   // Hidden count parameter (after regular params, before varargs)
   if (token->Fdec.is_variadic)
      paramTypes[param_count] = i32;

   TypeRef funcType = LLVMFunctionType(retType, paramTypes, param_count1, token->Fdec.is_variadic);

   ValueRef existingFunc = LLVMGetNamedFunction(module,
                           token->llvm_name ? token->llvm_name : token->name);
   if (existingFunc) {
      token->llvm.elem = existingFunc;
   } else {
      token->llvm.elem = LLVMAddFunction(module, token->llvm_name ? token->llvm_name : token->name,
                                         funcType);
   }
}

void _entry(Token *token)
{
   BasicBlockRef entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);
}

void _return(Token *token)
{
   ValueRef value = token->llvm.elem;
   if (value) LLVMBuildRet(builder, value);
   else LLVMBuildRetVoid(builder);
}

void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   token->llvm.elem = LLVMBuildAlloca(builder, type, token->name);
}

void _int(Token *token)
{
   TypeRef type = get_llvm_type(token); long long value;
   value = (long long)token->Int.value;
   token->llvm.elem = LLVMConstInt(type, value, 0);
}

void _char(Token *token)
{
   TypeRef type = get_llvm_type(token); int value;
   value = (int)token->Char.value;
   token->llvm.elem = LLVMConstInt(type, value, 0);
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

   token->llvm.elem = LLVMBuildGlobalStringPtr(builder, processed, name);
   free(processed);
}

void _load(Token *to, Token *from)
{
   TypeRef type = get_llvm_type(from);
   char *name = to->name;
   ValueRef source = from->llvm.elem;
   to->llvm.elem = LLVMBuildLoad2(builder, type, source, name);
}

void _op(Token *token, Token *left, Token *right)
{
   ValueRef l = left->llvm.elem;
   ValueRef r = right->llvm.elem;
   ValueRef elem = NULL;
   switch (token->type)
   {
   case ADD: elem = LLVMBuildAdd(builder, l, r, "add"); break;
   case SUB: elem = LLVMBuildSub(builder, l, r, "sub"); break;
   case EQUAL: elem = LLVMBuildICmp(builder, LLVMIntEQ, l, r, "equal"); break;
   case NOT_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntNE, l, r, "not_equal"); break;
   case LESS: elem = LLVMBuildICmp(builder, LLVMIntSLT, l, r, "less"); break;
   case MORE: elem = LLVMBuildICmp(builder, LLVMIntSGT, l, r, "more"); break;
   case LESS_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntSLE, l, r, "less_equal"); break;
   case MORE_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntSGE, l, r, "more_equal"); break;
   case MUL: elem = LLVMBuildMul(builder, l, r, "mul"); break;
   case DIV: elem = LLVMBuildSDiv(builder, l, r, "div"); break;
   case MOD: elem = LLVMBuildSRem(builder, l, r, "mod"); break;
   case AND: elem = LLVMBuildAnd(builder, l, r, "and"); break;
   case OR: elem = LLVMBuildOr(builder, l, r, "or"); break;
   default:
      check(stderr, "Unknown operator: %s\n", to_string(token->type));
      break;
   }
   token->llvm.elem = elem;
}

void _branch(BasicBlockRef bloc)
{
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder)))
      LLVMBuildBr(builder, bloc);
}

void _position_at(BasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

void _condition(ValueRef cond, BasicBlockRef isTrue, BasicBlockRef isFalse)
{
   LLVMBuildCondBr(builder, cond, isTrue, isFalse);
}

BasicBlockRef _append_block(char *name)
{
   char block_name[256];
   snprintf(block_name, sizeof(block_name), "%s.%d", name, block_counter++);
   return LLVMAppendBasicBlockInContext(context,
                                        LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)),
                                        block_name);
}

void _access(Token *curr, Token *left, Token *right)
{
   ValueRef leftValue = left->llvm.elem;
   ValueRef rightRef = right->llvm.elem;

   ValueRef indices[] = { rightRef };
   ValueRef gep = LLVMBuildGEP2(builder, get_llvm_type(curr), leftValue, indices, 1, "ACCESS");
   curr->llvm.elem = LLVMBuildLoad2(builder, get_llvm_type(curr), gep, "");
}

void _cast(Token *to, Token *from, TypeRef target_type)
{
   LLVMValueRef source = from->llvm.elem;
   TypeRef source_type = LLVMTypeOf(source);

   if (source_type == target_type)
   {
      to->llvm.elem = source;
      return;
   }

   LLVMTypeKind source_kind = LLVMGetTypeKind(source_type);
   LLVMTypeKind target_kind = LLVMGetTypeKind(target_type);

   // Pointer to pointer
   if (source_kind == LLVMPointerTypeKind && target_kind == LLVMPointerTypeKind)
   {
      to->llvm.elem = LLVMBuildBitCast(builder, source, target_type, "cast");
   }
   // Int to int (different sizes)
   else if (source_kind == LLVMIntegerTypeKind && target_kind == LLVMIntegerTypeKind)
   {
      unsigned source_bits = LLVMGetIntTypeWidth(source_type);
      unsigned target_bits = LLVMGetIntTypeWidth(target_type);

      if (source_bits < target_bits)
         to->llvm.elem = LLVMBuildSExt(builder, source, target_type, "cast"); // sign extend
      else if (source_bits > target_bits)
         to->llvm.elem = LLVMBuildTrunc(builder, source, target_type, "cast"); // truncate
      else
         to->llvm.elem = source;
   }
   // Int to pointer
   else if (source_kind == LLVMIntegerTypeKind && target_kind == LLVMPointerTypeKind)
      to->llvm.elem = LLVMBuildIntToPtr(builder, source, target_type, "cast");
   // Pointer to int
   else if (source_kind == LLVMPointerTypeKind && target_kind == LLVMIntegerTypeKind)
      to->llvm.elem = LLVMBuildPtrToInt(builder, source, target_type, "cast");
   else
      check(1, "Unsupported cast");
}


Node *scoop[100];
int scoop_pos = -1;
Node *curr_scoop;

void enter_scoop(Node *node)
{
   scoop_pos++;
   scoop[scoop_pos] = node;
   curr_scoop = scoop[scoop_pos];
}

void exit_scoop()
{
   scoop_pos--;
   if (scoop_pos >= 0)
      curr_scoop = scoop[scoop_pos];
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
      Node *curr = scoop[j];
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

void add_variable(Node *node)
{
   if (curr_scoop->vlen == 0)
   {
      curr_scoop->vlen = 100;
      curr_scoop->variables = calloc(100, sizeof(Node *));
   }
   else if (curr_scoop->vpos + 1 == curr_scoop->vlen)
   {
      Node **tmp = calloc((curr_scoop->vlen *= 2), sizeof(Node*));
      memcpy(tmp, curr_scoop->variables, curr_scoop->vpos * sizeof(Node*));
      free(curr_scoop->variables);
      curr_scoop->variables = tmp;
   }
   curr_scoop->variables[curr_scoop->vpos++] = node;
}

Token *get_variable(char *name)
{
   for (int j = scoop_pos; j >= 0; j--)
   {
      Node *curr = scoop[j];
      if (!curr) continue;
      for (int i = 0; i < curr->vpos; i++)
      {
         Token *token = curr->variables[i]->token;
         if (strcmp(name, token->name) == 0) return token;
      }
   }
   check(1, "variable [%s] not found", name);
   return NULL;
}

void load_if_neccessary(Node *node)
{
   Token *token = node->token;
   if ((token->name && token->type != FCALL) || includes(token->type, MATH_OP, 0))
   {
      Token *new = copy_token(token);
      _load(new, token);
      token = new;
   }
   node->token = token;
}

int get_va_list_size(LLVMModuleRef module)
{
   const char *triple = LLVMGetTarget(module);

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
         add_variable(node);
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
   case ADD: case SUB: case MUL: case DIV: case LESS:
   case MORE: case EQUAL: case LESS_EQUAL: case MORE_EQUAL:
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

      check(right->is_ref && !right->has_ref, "Error: cannot assign_node from unbound reference");

      if (!left->is_ref && !right->is_ref)
      {
         load_if_neccessary(node->right);

         LLVMBuildStore(builder, right->llvm.elem, left->llvm.elem);
         right->llvm.elem = left->llvm.elem;
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
            LLVMBuildStore(builder, right->llvm.elem, left->llvm.elem);
            node->token->llvm.elem = left->llvm.elem;
         }
      }
      else if (!left->is_ref && right->is_ref)
      {
         Token *temp = copy_token(right);
         _load(temp, right);

         LLVMBuildStore(builder, temp->llvm.elem, left->llvm.elem);
         temp->llvm.elem = left->llvm.elem;
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
            LLVMBuildStore(builder, temp->llvm.elem, left->llvm.elem);
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
         for (int i = 0; i < node->token->Fdec.args_len; i++)
         {
            Token *param = node->token->Fdec.args[i];

            if (param->type == VA_LIST)
            {
               LLVMValueRef count_param = LLVMGetParam(node->token->llvm.elem, param_idx);
               param_idx++;

               LLVMValueRef count_alloca = LLVMBuildAlloca(builder, i32, "va_count");
               LLVMBuildStore(builder, count_param, count_alloca);
               param->llvm.va_count = count_alloca;

               int va_list_size = get_va_list_size(module);

               LLVMTypeRef va_list_type = LLVMGetTypeByName2(context, "struct.__va_list_tag");
               if (!va_list_type) {
                  va_list_type = LLVMArrayType(i8, va_list_size);
               }

               LLVMValueRef va_list_alloca = LLVMBuildAlloca(builder, va_list_type, param->name);
               param->llvm.elem = va_list_alloca;

               LLVMValueRef va_start = LLVMGetNamedFunction(module, "llvm.va_start.p0");
               if (!va_start) {
                  TypeRef params[] = { LLVMPointerType(i8, 0) };
                  TypeRef va_start_type = LLVMFunctionType(vd, params, 1, false);
                  va_start = LLVMAddFunction(module, "llvm.va_start.p0", va_start_type);
               }

               LLVMValueRef gep = LLVMBuildGEP2(builder, LLVMArrayType(i8, va_list_size),
                                                va_list_alloca,
               (LLVMValueRef[]) {LLVMConstInt(i32, 0, 0)}, 1, "");
               LLVMValueRef cast = LLVMBuildBitCast(builder, gep, LLVMPointerType(i8, 0), "");
               LLVMBuildCall2(builder, LLVMGlobalGetValueType(va_start), va_start, &cast, 1, "");

               add_variable(new_node(param));
            }
            else if (param->is_ref)
            {
               LLVMValueRef p = LLVMGetParam(node->token->llvm.elem, param_idx);
               param_idx++;
               param->llvm.elem = p;
               param->has_ref = true;
               add_variable(new_node(param));
            }
            else
            {
               LLVMValueRef p = LLVMGetParam(node->token->llvm.elem, param_idx);
               param_idx++;

               LLVMValueRef alloca = LLVMBuildAlloca(builder, get_llvm_type(param), "param");
               LLVMBuildStore(builder, p, alloca);
               param->llvm.elem = alloca;
               add_variable(new_node(param));
            }
         }
         for (int i = 0; i < node->cpos; i++)
         {
            if (node->children[i]->token->type != FDEC)
               generate_ir(node->children[i]);
         }

         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder)))
         {
            if (node->token->Fdec.retType == VOID)
               LLVMBuildRetVoid(builder);
            else
            {
               fprintf(stderr, "Warning: Non-void function '%s' may not return a value\n",
                       node->token->name);
               LLVMBuildRet(builder, LLVMConstInt(get_llvm_type(node->token), 0, 0));
            }
         }
      }
      exit_scoop();
      break;
   }
   case STACK:
   {
      pnode(node, NULL, 0);
      // Generate the size expression
      generate_ir(node->left);

      Token *size_token = node->left->token;
      LLVMValueRef size_value = size_token->llvm.elem;

      // Ensure size_value is actually an integer type
      LLVMTypeRef size_type = LLVMTypeOf(size_value);
      if (LLVMGetTypeKind(size_type) != LLVMIntegerTypeKind) {
         check(1, "stack() size must be an integer, got %d", LLVMGetTypeKind(size_type));
      }

      // Allocate array of i8 (bytes) on the stack
      LLVMValueRef stack_alloc = LLVMBuildArrayAlloca(builder, i8, size_value, "stack");

      node->token->llvm.elem = stack_alloc;
      node->token->type = STACK;
      break;
   }
   case FCALL:
   {
      Token *funcToken = get_function(node->token->name);

      for (int i = 0; i < node->token->Fcall.args_len; i++) {
         Node *nodeArg = new_node(node->token->Fcall.args[i]);
         generate_ir(nodeArg);

         bool param_is_ref = false;
         if (i < funcToken->Fdec.args_len)
            param_is_ref = funcToken->Fdec.args[i]->is_ref;

         if (!param_is_ref)
            load_if_neccessary(nodeArg);

         node->token->Fcall.args[i] = nodeArg->token;
         free(nodeArg);
      }
      _fcall_invoke(node->token);
      break;
   }
   case WHILE:
   {
      enter_scoop(node);

      BasicBlockRef cond = _append_block("while.cond");
      BasicBlockRef body = _append_block("while.body");
      BasicBlockRef end = _append_block("while.end");

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

      BasicBlockRef end_block = _append_block("if.end");
      Node* curr = node;

      while (curr && (curr->token->type == IF || curr->token->type == ELIF))
      {
         BasicBlockRef then_block = _append_block("if.then");
         BasicBlockRef else_block = _append_block("if.else");
         generate_ir(curr->left);
         _condition(curr->left->token->llvm.elem, then_block, else_block);
         _position_at(then_block);
         for (int i = 0; i < curr->cpos; i++)
            generate_ir(curr->children[i]);
         _branch(end_block);
         _position_at(else_block);
         if (curr->right && curr->right->token->type == ELIF)
            curr = curr->right;
         else if (curr->right && curr->right->token->type == ELSE)
         {
            for (int i = 0; i < curr->right->cpos; i++)
               generate_ir(curr->right->children[i]);
            _branch(end_block);
            break;
         }
         else
         {
            _branch(end_block);
            break;
         }
      }

      _position_at(end_block);
      exit_scoop();
      break;
   }
   case TRY:
   {
      generate_ir_try_catch(node);
      break;
   }
   case THROW:
   {
      generate_ir_throw(node);
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
            node->token->llvm.elem = LLVMBuildLoad2(builder, i32, object->llvm.va_count, "va_len");
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

            LLVMValueRef va_list_ptr = object->llvm.elem;
            LLVMValueRef bitcast = LLVMBuildBitCast(builder, va_list_ptr, p8, "va_cast");
            LLVMValueRef result = LLVMBuildVAArg(builder, bitcast, extract_type, "va_arg");

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
      break;
   }
   case RETURN:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);

      ExceptionContext *ctx = get_current_exception_context();
      if (ctx && ctx->in_catch_block) {
         LLVMValueRef end_catch = get_or_declare_cxa_end_catch();
         LLVMBuildCall2(builder, LLVMGlobalGetValueType(end_catch), end_catch, NULL, 0, "");
      }

      _return(node->left->token);
      break;
   }
   case ACCESS:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);
      generate_ir(node->right);
      load_if_neccessary(node->right);
      _access(node->token, node->left->token, node->right->token);
      break;
   }
   default:
      check(1, "handle this case [%s]\n", to_string(node->token->type));
      break;
   }
}

int main(int argc, char **argv)
{
   (void)argc;
   init("module");

   input = open_file(argv[1]);
   tokenize();
   Node *buff[10000] = {};
   int p = 0;
   while (tokens[exe_pos]->type != END) buff[p++] = expr_node();
   enter_scoop(new_node(new_token(END, 0, 0, 0, 0, 0)));
   for (int i = 0; buff[i]; i++) generate_ir(buff[i]);
   finalize();
   free_tokens();
   return 0;
}

// UTILS
void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   vd = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1 = LLVMInt1TypeInContext(context);
   i8 = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8 = LLVMPointerType(i8, 0);
   p32 = LLVMPointerType(i32, 0);

   LLVMInitializeAllTargetInfos();
   LLVMInitializeAllTargets();
   LLVMInitializeAllTargetMCs();
   LLVMInitializeAllAsmParsers();
   LLVMInitializeAllAsmPrinters();
   LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
}

char *to_string(Type type)
{
   char* res[END + 1] = {
      [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
      [INT] = "INT", [BOOL] = "BOOL", [NEWLINE] = "NEWLINE", [FDEC] = "FDEC",
      [FCALL] = "FCALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
      [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE", [WHILE] = "WHILE",
      [RETURN] = "RETURN", [END_BLOCK] = "END_BLOCK", [ADD] = "ADD",
      [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
      [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
      [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
      [MOD_ASSIGN] = "MOD_ASSIGN", [ACCESS] = "ACCESS",
      [MOD] = "MOD", [COMA] = "COMA", [REF] = "REF",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQUAL", [LESS] = "LESS",
      [MORE] = "MORE", [LESS_EQUAL] = "LESS_EQUAL",
      [MORE_EQUAL] = "MORE_EQUAL", [AND] = "AND", [OR] = "OR",
      [DOTS] = "DOTS", [COLON] = "COLON", [COMMA] = "COMMA",
      [LBRACKET] = "LBRACKET", [RBRACKET] = "RBRACKET",
      [PROTO] = "PROTO", [VARIADIC] = "VARIADIC",
      [VA_LIST] = "VA_LIST", [AS] = "AS", [STACK] = "STACK",
      [TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW",
      [USE] = "USE", [LBRA] = "LBRA", [RBRA] = "RBRA",
      [DOT] = "DOT",
   };
   if (!res[type])
   {
      printf("%s:%d handle this case %d\n", __FILE__, __LINE__, type);
      exit(1);
   }
   return res[type];
}

void ptoken(Token *token)
{
   printf("token %s ", to_string(token->type));
   if (token->name) printf("[%s] ", token->name);

   if (token->is_dec) printf("is dec");
   else
      switch (token->type)
      {
      case INT: printf("%ld", token->Int.value); break;
      case CHAR: printf("[%c]", token->Char.value); break;
      case CHARS: printf("[%s]", token->Chars.value); break;
      default: break;
      }
   printf(" space [%d]\n", token->space);
}

void pnode(Node *node, char *side, int space)
{
   if (!node) return;
   int i = 0;
   while (i < space) i += printf(" ");
   if (side) printf("%s ", side);
   printf("node "); ptoken(node->token);
   pnode(node->left, "L:", space + TAB);
   pnode(node->right, "R:", space + TAB);
   switch (node->token->type)
   {
   case FDEC:
   {
      for (int i = 0; i < node->cpos; i++)
         pnode(node->children[i], NULL, space + TAB);
      break;
   }
   default:
      break;
   }
}

char *strjoin(char *str, ...)
{
   if (!str) return NULL;

   va_list ap;
   size_t len = 0;

   va_start(ap, str);
   char *ptr = str;
   while (ptr)
   {
      len += strlen(ptr);
      ptr = va_arg(ap, char *);
   }
   va_end(ap);

   char *res = calloc(len + 1, 1);
   if (!res) return NULL;

   char *dst = res;
   va_start(ap, str);
   ptr = str;
   while (ptr)
   {
      size_t l = strlen(ptr);
      memcpy(dst, ptr, l);
      dst += l;
      ptr = va_arg(ap, char *);
   }
   va_end(ap);

   return res;
}

char* open_file(char *filename)
{
   char dir[100000] = {};
   getwd(dir);
   filename = strjoin(dir, "/", filename, NULL);
   File file = fopen(filename, "r");
   if (!file)
   {
      printf("Error: openning file [%s]", filename);
      free(filename);
      exit(1);
   }
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = calloc((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   free(filename);
   return input;
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

void finalize()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, RED"Module verification failed:\n%s\n"RESET, error);
      LLVMDisposeMessage(error);
   }
   LLVMPrintModuleToFile(module, "build/out.ll", NULL);

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}

bool includes(Type to_find, ...)
{
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0)
      if (current == to_find) return true;
   return false;
}

char *substr(char *input, int s, int e)
{
   char *res = calloc(e - s + 1, sizeof(char));
   strncpy(res, input + s, e - s);
   return res;
}

void free_tokens()
{
   for (int i = 0; tokens[i]; i++)
   {
      if (tokens[i]->Chars.value) free(tokens[i]->Chars.value);
      if (tokens[i]->name) free(tokens[i]->name);
      free(tokens[i]);
   }
}

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = calloc(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   // TODO: can't use setName here to investigate later why ?
   if (token->name)
      new->name = strdup(token->name);

   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   tokens[tk_pos++] = new;
   return new;
}