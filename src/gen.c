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
   debug("==> %k\n", token);
   if (includes(type, FDEC, PROTO, 0)) type = token->retType;
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

   if (token->llvm.is_loaded || includes(token->type, CHARS, /*STACK,*/ 0))
      return;

   if (token->name && token->type != FCALL)
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->llvm.is_loaded = true;
      node->token = new;
   }
   else if (includes(token->type, ACCESS, ADD, SUB, MUL, DIV, 0))
   {
      Token *new = copy_token(token);
      _load(new, token);
      new->llvm.is_loaded = true;
      node->token = new;
   }
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

void handle_asm(Node *node)
{
   // debug("Processing: %k\n", inst->token);
   Value ret = NULL;
   Node *left = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set\n"))
      return;
   switch (node->token->type)
   {
   case INT: case CHARS: case CHAR:
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
      void (*funcs[END])(Token *) = {[INT] = &_int, [CHAR] = &_char, [CHARS] = &_chars};
      void (*func)(Token *) = funcs[node->token->type];
      check(!func, "handle this case [%s]\n", to_string(node->token->type));
      func(node->token);
      break;
   }
   case ASSIGN:
   {
      handle_asm(left);
      handle_asm(right);
      if (!left->token->is_ref && !right->token->is_ref)
      {
         if (left->token->type == CHARS &&
               (right->token->type == CHARS && !right->token->name))
         {
            llvm_build_store(node->token, right->token->llvm.elem, left->token->llvm.elem);
            node->token->llvm.elem = left->token->llvm.elem;
         }
         else {
            load_if_neccessary(node->right);
            llvm_build_store(node->token, right->token->llvm.elem, left->token->llvm.elem);
            right->token->llvm.elem = left->token->llvm.elem;
         }
      }
      else
         check(1, "handle this case");
      break;
   }
   case FDEC: case PROTO:
   {
      if (scoop_pos > 1)
      {
         static int nested_id = 0;
         char new_name[256];
         snprintf(new_name, sizeof(new_name), "%s.%s.%d", scoop->token->name, node->token->name,
                  nested_id++);
         node->token->llvm_name = strdup(new_name);
      }
      else
         node->token->llvm_name = strdup(node->token->name);

      enter_scoop(node);
      _fdec(node->token);

      if (node->token->type == FDEC)
      {
         _entry(node->token);

         for (int i = 0; i < node->cpos; i++)
         {
            // if (node->children[i]->token->type != FDEC)
            handle_asm(node->children[i]);
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
      handle_asm(node->left);
      load_if_neccessary(node->left);

      // ExcepCTX *ctx = get_current_exception_context();
      // if (ctx && ctx->in_catch) {
      //    Value end_catch = get_end_catch();
      //    llvm_build_call2(node->token, llvm_global_get_value_type(end_catch), end_catch, NULL, 0, "");
      // }

      _return(node->left->token);
      break;
   }
   case END_BLOC: exit_scoop(); break;
   default:
      check(1, "handle this case %s", to_string(node->token->type));
      break;
   }

}

void generate_ir(Node *node)
{
   if (found_error) return;
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
      generate_ir(node->left);
      generate_ir(node->right);
      node->token->ir_reg = node->left->token->ir_reg;
      node->token->retType = node->left->token->retType;
      break;
   }
   case FDEC:
   {
      node->token->ir_reg++;
      new_function(node);
      enter_scoop(node);

#if 0
      // parameters
      Node **params = (node->left ? node->left->children : NULL);
      Token *token = node->token;
      for (int i = 0; params && i < node->left->cpos && !found_error; i++)
      {
         Node *child = params[i];
         generate_ir(child);
         if (token->Fdec.args == NULL)
         {
            token->Fdec.len = 10;
            token->Fdec.args = allocate(token->Fdec.len, sizeof(Token*));
         }
         else if (token->Fdec.pos + 1 == token->Fdec.len)
         {
            Token **tmp = allocate(token->Fdec.len *= 2, sizeof(Token*));
            memcpy(tmp, token->Fdec.args, token->Fdec.pos * sizeof(Token*));
            free(token->Fdec.args);
            token->Fdec.args = tmp;
         }
         if (child->token->is_ref)
            child->token->has_ref = true;
         child->token->Param.index = i;
         child->token->Param.func_ptr = node->token;
         child->token->is_param = true;
         child->token->is_dec = false;
         token->Fdec.args[token->Fdec.pos++] = child->token;
      }
#endif

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