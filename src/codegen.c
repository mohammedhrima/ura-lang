#include "ura.h"

// ----------------------------------------------------------------------------
// LLVM higher-level helpers (from llvm.c)
// ----------------------------------------------------------------------------
Value allocate_stack(Value size, TypeRef elementType, char *name)
{
   Value indices[] =
   {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, 0, 0)
   };

   if (LLVMIsConstant(size))
   {
      unsigned long long constSize    = LLVMConstIntGetZExtValue(size);
      TypeRef            arrayType    = LLVMArrayType(elementType, constSize);
      Value              array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
   }

   Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef elementType, char *name)
{
   Value calloc_func = _get_named_function("calloc");
   if (!calloc_func)
   {
      TypeRef params[]  = {i64, i64};
      TypeRef func_type = _function_type(p8, params, 2, 0);
      calloc_func = _add_function("calloc", func_type);
   }
   TargetData td        = _get_module_data_layout(module);
   size_t     elem_size = _abi_size_of_type(td, elementType);

   Value      count_i64;
   unsigned   width = LLVMGetIntTypeWidth(LLVMTypeOf(count));
   if (width < 64)
      count_i64 = LLVMBuildZExt(builder, count, i64, "count");
   else if (width > 64)
      count_i64 = LLVMBuildTrunc(builder, count, i64, "count");
   else
      count_i64 = count;

   Value   size_i64    = _const_int(i64, elem_size, 0);
   Value   args[]      = {count_i64, size_i64};
   TypeRef calloc_type = _global_get_value_type(calloc_func);
   return _build_call2(calloc_type, calloc_func, args, 2, name);
}


void hoist_allocas(Node *node)
{
   if (!node) return;
   Token *tok = node->token;

   // don't recurse into nested functions
   if (tok->type == FDEC) return;

   if (includes(tok->type, INT, LONG, SHORT, CHARS, CHAR, BOOL, ARRAY_TYPE, 0) && tok->is_dec)
   {
      if (!tok->llvm.elem)
         _alloca(tok);
   }
   else if (tok->type == STRUCT_CALL && tok->is_dec && !tok->is_ref)
   {
      if (!tok->llvm.elem)
      {
         TypeRef struct_type = get_llvm_type(tok);
         tok->llvm.elem = _build_alloca(struct_type, tok->name);
      }
   }

   if (node->left)  hoist_allocas(node->left);
   if (node->right) hoist_allocas(node->right);
   for (int i = 0; i < node->cpos; i++)
      hoist_allocas(node->children[i]);
}


void load_if_necessary(Node *node)
{
   Token *token = node->token;

   if (token->is_ref)
      return;
   if (includes(token->type, MATH_TYPE, 0))
      return;
   if (includes(token->type, DATA_TYPES, 0) && !token->name)
      return;
   if (token->llvm.is_loaded)
      return;
   if (includes(token->type, STACK, HEAP, FCALL, 0))
      return;

   if (token->name || includes(token->type, ACCESS, DOT, 0))
   {
      Token *new = copy_token(token);
      new->llvm.elem      = load_value(token);
      new->llvm.is_loaded = true;
      node->token         = new;
   }
}

void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   if (token->is_ref)
      type = _pointer_type(type, 0);

   Block current = _get_insert_block();
   Value func    = _get_basic_block_parent(current);
   Block entry   = _get_entry_basic_block(func);
   // Walk past leading allocas to find the insertion point
   Value inst    = LLVMGetFirstInstruction(entry);
   while (inst && LLVMGetInstructionOpcode(inst) == LLVMAlloca)
      inst = LLVMGetNextInstruction(inst);

   // Position before the first non-alloca instruction (safe even when it's a terminator)
   if (inst)
      LLVMPositionBuilderBefore(builder, inst);
   else
      _position_at(entry);

   token->llvm.elem = _build_alloca(type, token->name);

   _position_at(current);
}

TypeRef get_llvm_type(Token *token)
{
   Type type = token->type;
   if (token->retType)
      type = token->retType;
   if (type == STRUCT_DEF)
      return token->llvm.stType;
   if (type == TUPLE)
      return token->llvm.stType;
   if (type == STRUCT_CALL)
      return get_llvm_type(token->Struct.ptr->token);
   if (includes(type, ARRAY, ARRAY_TYPE, 0))
   {
      Token   tmp  = {.type = token->Array.elem_type};
      TypeRef base = get_llvm_type(&tmp);
      for (int i = 0; i < token->Array.depth; i++)
         base = _pointer_type(base, 0);
      return base;
   }
   // if (type == FCALL)
   //    return get_llvm_type(token->Fcall.ptr->token);
   TypeRef res[END] = {
      [INT]    = i32,
      [CHAR]   = i8,
      [CHARS]  = p8,
      [BOOL]   = i1,
      [VOID]   = vd,
      [LONG]   = i64,
      [FLOAT]  = f32,
      [ACCESS] = i8,
   };
   if(check(!res[type], "handle this case [%s]", to_string(type)))
   {
      seg();
   }
   return res[type];
}

Value _get_default_value(Token *token)
{
   TypeRef type = get_llvm_type(token);

   if (token->is_ref)
      return LLVMConstNull(_pointer_type(type, 0));

   if (includes(token->type, NUMERIC_TYPES, 0))
      return LLVMConstInt(type, 0, false);
   if (token->type == FLOAT)
      return LLVMConstReal(type, 0.0);
   if (includes(token->type, CHARS, ARRAY_TYPE, 0))
      return LLVMConstNull(type);
   check(1, "handle this case %s", to_string(token->type));
   return NULL;
}

void _const_value(Token *token)
{
   TypeRef   type  = get_llvm_type(token);
   long long value = 0;

   switch (token->type)
   {
   case INT:  value = (long long)token->Int.value; break;
   case BOOL: value = (long long)token->Bool.value; break;
   case CHAR: value = (int)token->Char.value; break;
   case FLOAT:
   {
      token->llvm.elem      = LLVMConstReal(type, (double)token->Float.value);
      token->llvm.is_loaded = true;
      return;
   }
   case CHARS:
   {
      char       name[200];
      char      *processed;
      static int index = 0;
      snprintf(name, sizeof(name), "STR%d", index++);
      processed = allocate(strlen(token->Chars.value) * 2 + 1, 1);
      int j = 0;
      for (int i = 0; token->Chars.value[i]; i++)
      {
         if (token->Chars.value[i] == '\\' && token->Chars.value[i + 1])
         {
            switch (token->Chars.value[i + 1])
            {
            case 'n': processed[j++]  = '\n'; i++; break;
            case 't': processed[j++]  = '\t'; i++; break;
            case 'r': processed[j++]  = '\r'; i++; break;
            case '0': processed[j++]  = '\0'; i++; break;
            case '\\': processed[j++] = '\\'; i++; break;
            case '\"': processed[j++] = '\"'; i++; break;
            case '\'': processed[j++] = '\''; i++; break;
            default: processed[j++]   = token->Chars.value[i]; break;
            }
         }
         else
         {
            processed[j++] = token->Chars.value[i];
         }
      }
      token->llvm.elem = _const_chars(processed, name);
      free(processed);
      return;
   }
   default: check(1, "handle this case %s", to_string(token->type)); return;
   }
   token->llvm.elem = _const_int(type, value, 0);
}

Value _build_return(Token *token)
{
   if (!_get_basic_block_terminator(_get_insert_block()))
   {
      Value value = token->llvm.elem;
      if (token->type != VOID)
      {
         debug("build return %k\n", token);
         return LLVMBuildRet(builder, value);
      }
      else
         return LLVMBuildRetVoid(builder);
   }
   return NULL;
}

// ----------------------------------------------------------------------------
// ASM GENERATION
// ----------------------------------------------------------------------------
void init(char *name)
{
   context = LLVMContextCreate();
   module  = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);


   vd  = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1  = LLVMInt1TypeInContext(context);
   i8  = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8  = _pointer_type(i8, 0);
   p32 = _pointer_type(i32, 0);

   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   #if defined(__APPLE__)
   LLVMSetTarget(module, "arm64-apple-macosx16.0.0");
   #elif defined(__linux__)
   LLVMSetTarget(module, "x86_64-pc-linux-gnu");
   #else
   LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
   #endif

   // if (enable_asan)
   // {
   LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Debug Info Version",
                     18, LLVMValueAsMetadata(LLVMConstInt(i32, 3, 0)));
   LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Dwarf Version",
                     13, LLVMValueAsMetadata(LLVMConstInt(i32, 4, 0)));
   // }

   // Debug info
   di_builder = LLVMCreateDIBuilder(module);
   char *base          = strrchr(name, '/');
   char *filename_only = base ? base + 1 : name;
   char  dir[1024]     = ".";
   if (base) { size_t len = base - name; strncpy(dir, name, len); dir[len] = '\0'; }

   di_file = LLVMDIBuilderCreateFile(
      di_builder, filename_only,
      strlen(filename_only), dir, strlen(dir));

   di_compile_unit = LLVMDIBuilderCreateCompileUnit(
      di_builder, LLVMDWARFSourceLanguageC,
      di_file, "ura", 3, 0, "", 0, 0, "", 0,
      LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);

   di_current_scope = di_compile_unit;

   // if (using_refs) getRefAssignFunc();
   // if (enable_bounds_check) getNullCheckFunc();
}

void finalize(char *output)
{
   char *error = NULL;

   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();

   LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

   if (passes)
   {
      LLVMErrorRef err = LLVMRunPasses(module, passes, NULL, options);
      if (err)
      {
         char *msg = LLVMGetErrorMessage(err);
         fprintf(stderr, RED "Optimizer Error: %s\n" RESET, msg);
         LLVMDisposeErrorMessage(msg);
         found_error = true;
         return;
      }
   }

   LLVMDIBuilderFinalize(di_builder);
   LLVMDisposeDIBuilder(di_builder);
   di_builder = NULL;

   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error))
   {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
      LLVMDisposePassBuilderOptions(options);
      found_error = true;
      // return;
   }
   else
   {
      LLVMDisposePassBuilderOptions(options);

   }

   LLVMPrintModuleToFile(module, output, NULL);
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}

void emit_scope_clean(Node *scope, int from)
{
   for (int i = from; i < scope->vpos; i++)
   {
      Token *var = scope->variables[i];
      if (var->type != STRUCT_CALL || !var->llvm.elem) continue;
      if (var->is_ref) continue;  // refs are borrowed, not owned — caller cleans up
      Node *sd       = var->Struct.ptr;
      char *qname    = strjoin(sd->token->name, ".clean", NULL);
      Value clean_fn = _get_named_function(qname);
      free(qname);
      if (!clean_fn) continue;
      Value args[] = { var->llvm.elem };
      _build_call2(_global_get_value_type(clean_fn), clean_fn, args, 1, "");

   }
}

void append_output_arg(Token *tok, char *fmt, int *fpos, Value *args, int *nargs)
{
   Type type = tok->type ? tok->type : tok->retType;

   // String literal: embed directly in the format string (escape % → %%)
   if (tok->type == CHARS && !tok->name)
   {
      const char *s = tok->Chars.value;
      for (int i = 0; s[i]; i++)
      {
         if (s[i] == '%')
         {
            fmt[(*fpos)++] = '%'; // double % for printf
            fmt[(*fpos)++] = '%';
         }
         else if (s[i] == '\\' && s[i + 1])
         {
            switch (s[++i])
            {
            case 'n':  fmt[(*fpos)++] = '\n'; break;
            case 't':  fmt[(*fpos)++] = '\t'; break;
            case 'r':  fmt[(*fpos)++] = '\r'; break;
            case '\\': fmt[(*fpos)++] = '\\'; break;
            case '"':  fmt[(*fpos)++] = '"';  break;
            default:   fmt[(*fpos)++] = '\\'; fmt[(*fpos)++] = s[i]; break;
            }
         }
         else
            fmt[(*fpos)++] = s[i];
      }
      return;
   }

   // For expression nodes (DOT, FCALL, etc.), resolve to actual value type
   switch (type)
   {
   case INT: case LONG: case SHORT: case CHAR: case CHARS:
   case BOOL: case FLOAT: case STRUCT_CALL:
      break;
   case FCALL:
      type = tok->retType;
      break;
   default:
      if (tok->retType) type = tok->retType;
      break;
   }

   switch (type)
   {
   case INT: case SHORT:
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 'd';
      args[(*nargs)++] = load_value(tok);
      break;
   case BOOL:
   {
      Value bool_val  = load_value(tok);
      Value true_str  = _const_chars("True",  "true_str");
      Value false_str = _const_chars("False", "false_str");
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 's';
      args[(*nargs)++] = LLVMBuildSelect(builder, bool_val, true_str, false_str, "bool_str");
      break;
   }
   case LONG:
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 'l';
      fmt[(*fpos)++]   = 'l'; fmt[(*fpos)++] = 'd';
      args[(*nargs)++] = load_value(tok);
      break;
   case CHAR:
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 'c';
      args[(*nargs)++] = load_value(tok);
      break;
   case CHARS:
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 's';
      args[(*nargs)++] = load_value(tok);
      break;
   case FLOAT:
      fmt[(*fpos)++]   = '%'; fmt[(*fpos)++] = 'f';
      args[(*nargs)++] = LLVMBuildFPExt(builder, load_value(tok), LLVMDoubleTypeInContext(context), "f2d");
      break;
   case STRUCT_CALL:
   {
      fmt[(*fpos)++] = '{';
      fmt[(*fpos)++] = ' ';
      Node *sd = tok->Struct.ptr;
      for (int i = 0; i < sd->cpos; i++)
      {
         Token *field = sd->children[i]->token;
         int    flen  = strlen(field->name);
         memcpy(fmt + *fpos, field->name, flen); *fpos += flen;
         fmt[(*fpos)++] = ':'; fmt[(*fpos)++] = ' ';
         Token ftok = *field;
         ftok.llvm.elem = struct_field_ptr(tok, i, field->name);
         append_output_arg(&ftok, fmt, fpos, args, nargs);
         if (i < sd->cpos - 1) { fmt[(*fpos)++] = ','; fmt[(*fpos)++] = ' '; }
      }
      fmt[(*fpos)++] = ' ';
      fmt[(*fpos)++] = '}';
      break;
   }
   default:
      fmt[(*fpos)++] = '?';
      break;
   }
}

void gen_asm(Node *node)
{
   set_debug_location(node->token);
   Node *left  = node->left;
   Node *right = node->right;

   if (check(node->token->llvm.is_set, "already set")) return;
   switch (node->token->type)
   {
   case STRUCT_CALL:
   {
      if (node->token->is_dec)
      {
         if (node->token->is_global)
         {
            if (node->token->used == 0) { node->token->is_dec = false; return; }
            TypeRef type = get_llvm_type(node->token);
            node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
            LLVMSetInitializer(node->token->llvm.elem, LLVMConstNull(type));
         }
         else if (node->token->is_ref)
         {
            // TypeRef type = get_llvm_type(node->token);
            // Value   null = LLVMConstNull(LLVMPointerType(type, 0));
            // _build_store(null, node->token->llvm.elem);
         }
         else
         {
            _alloca(node->token);
            TypeRef struct_type = get_llvm_type(node->token);
            _build_store(LLVMConstNull(struct_type), node->token->llvm.elem);

            Node *src     = node->token->Struct.ptr;
            char *qname   = strjoin(src->token->name, ".init", NULL);
            Value init_fn = _get_named_function(qname);
            free(qname);
            if (init_fn)
            {
               Value args[] = { node->token->llvm.elem };
               _build_call2(_global_get_value_type(init_fn), init_fn, args, 1, "");
            }
         }
         node->token->is_dec = false;
         return;
      }
      break;
   }
   case INT: case LONG: case SHORT: case CHARS:
   case CHAR: case BOOL: case ARRAY_TYPE: case FLOAT:
   {
      if (node->token->is_dec)
      {
         if (node->token->is_global)
         {
            if (node->token->used == 0) { node->token->is_dec = false; return; }
            TypeRef type = get_llvm_type(node->token);
            node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
            LLVMSetInitializer(node->token->llvm.elem, _get_default_value(node->token));
         }
         else
         {
            _alloca(node->token);
            Value defval = _get_default_value(node->token);
            _build_store(defval, node->token->llvm.elem);
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
      if (left->token->is_global && left->token->is_dec)
      {
         gen_asm(left);
         gen_asm(right);
         if (right->token->llvm.elem && LLVMIsConstant(right->token->llvm.elem))
            LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
         return;
      }
      if (left->token->is_ref && !left->token->asm_bound && !left->token->is_param)
      {
         if (!left->token->llvm.elem)
            _alloca(left->token);
         gen_asm(right);
         _build_store(right->token->llvm.elem, left->token->llvm.elem);
         left->token->asm_bound = true;
         left->token->is_dec    = false;
         break;
      }

      gen_asm(left);
      gen_asm(right);

      TypeRef type = get_llvm_type(right->token);

      Value   val;
      if (right->token->is_ref)
         val = load_value(right->token);
      else
      {
         load_if_necessary(right);
         val = right->token->llvm.elem;
      }

      if (left->token->is_ref)
      {
         Value ptr = LLVMBuildLoad2(builder, _pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         _build_store(val, ptr);
      }
      else
         _build_store(val, left->token->llvm.elem);

      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_asm(left);
      gen_asm(right);

      TypeRef type        = get_llvm_type(left->token);
      Value   current_val = load_value(left->token);

      Value right_val;
      if (right->token->is_ref)
         right_val = load_value(right->token);
      else
      {
         load_if_necessary(right);
         right_val = right->token->llvm.elem;
      }

      Value result = build_binary_op(assign_base_op(node->token->type), current_val, right_val);

      if (left->token->is_ref)
      {
         Value ptr = LLVMBuildLoad2(builder, _pointer_type(type, 0), left->token->llvm.elem, "ref_ptr");
         _build_store(result, ptr);
      }
      else
         _build_store(result, left->token->llvm.elem);
      break;
   }
   case NOT: case BNOT:
   {
      gen_asm(left);
      load_if_necessary(left);
      if (left->token->is_ref)
      {
         left->token->llvm.elem      = load_value(left->token);
         left->token->llvm.is_loaded = true;
      }
      node->token->llvm.elem = _build_not(left->token, to_string(node->token->type));
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case AND: case OR:
   case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
   {
      gen_asm(left);
      gen_asm(right);

      load_if_necessary(left);
      load_if_necessary(right);

      Value lref = left->token->llvm.elem;
      Value rref = right->token->llvm.elem;

      if (left->token->is_ref && !left->token->llvm.is_loaded)
         lref = load_value(left->token);
      if (right->token->is_ref && !right->token->llvm.is_loaded)
         rref = load_value(right->token);

      node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
      node->token->retType   = left->token->retType ? left->token->retType : left->token->type;
      break;
   }
   case STACK:
   {
      gen_asm(node->left);
      load_if_necessary(node->left);
      Value elem_count = node->left->token->llvm.elem;

      // base type
      Token   tmp    = {.type = node->token->Array.elem_type};
      TypeRef elem_t = get_llvm_type(&tmp);

      // depth > 1 means each element is a pointer
      // [char]=i8, [[char]]=ptr, [[[char]]]=ptr (ptr to ptr semantically)
      if (node->token->Array.depth > 1)
         elem_t = p8; // pointer-sized elements

      TargetData td        = _get_module_data_layout(module);
      size_t     elem_size = _abi_size_of_type(td, elem_t);
      Value      total     = _build_mul(elem_count, _const_int(i32, (unsigned)elem_size, 0), "bytes");

      node->token->llvm.elem       = allocate_stack(total, elem_t, "stack");
      node->token->llvm.is_set     = true;
      node->token->llvm.array_size = elem_count;
      break;
   }
   case HEAP:
   {
      gen_asm(node->left);
      load_if_necessary(node->left);
      Value   elem_count = node->left->token->llvm.elem;

      Token   tmp        = {.type = node->token->Array.elem_type};
      TypeRef elem_t     = get_llvm_type(&tmp);
      if (node->token->Array.depth > 1)
         elem_t = p8;

      node->token->llvm.elem       = allocate_heap(elem_count, elem_t, "heap");
      node->token->llvm.is_set     = true;
      node->token->llvm.array_size = elem_count;
      break;
   }
   case FCALL:
   {
      LLVM   srcFunc     = node->token->Fcall.ptr->token->llvm;
      bool   is_variadic = node->token->Fcall.ptr->token->is_variadic;
      int    count       = node->left->cpos;
      Node **argNodes    = node->left->children;
      Node  *dec_args    = node->token->Fcall.ptr->left;
      Value *args        = NULL;

      if (count)
      {
         args = allocate(count + 3, sizeof(Value));

         int fixed_params = is_variadic ? node->token->Fcall.ptr->left->cpos : count;

         // handle fixed params (both variadic and non-variadic)
         for (int i = 0; i < fixed_params; i++)
         {
            gen_asm(argNodes[i]);
            bool param_is_ref = (i < dec_args->cpos) && dec_args->children[i]->token->is_ref;
            bool arg_is_ref   = argNodes[i]->token->is_ref;

            if (param_is_ref && arg_is_ref)
            {
               // ref → ref: load the pointer out of the ref's slot and pass it
               TypeRef type = get_llvm_type(argNodes[i]->token);
               args[i] = LLVMBuildLoad2(builder, _pointer_type(type, 0), argNodes[i]->token->llvm.elem, "ref_arg");
            }
            else if (param_is_ref && !arg_is_ref)
            {
               // non-ref → ref param: pass the variable's alloca directly
               args[i] = argNodes[i]->token->llvm.elem;
            }
            else if (!param_is_ref && arg_is_ref)
            {
               // ref → non-ref param: double dereference to get the value
               args[i] = load_value(argNodes[i]->token);
            }
            else
            {
               // non-ref → non-ref: normal load
               load_if_necessary(argNodes[i]);
               args[i] = argNodes[i]->token->llvm.elem;
            }
         }

         if (is_variadic)
         {
            int variadic_count = count - fixed_params;
            args[fixed_params] = _const_int(i32, variadic_count, 0);
            for (int i = fixed_params; i < count; i++)
            {
               gen_asm(argNodes[i]);
               load_if_necessary(argNodes[i]);
               args[i + 1] = argNodes[i]->token->llvm.elem;
            }
            count++;
         }
      }

      char   *name     = node->token->Fcall.ptr->token->retType != VOID ? node->token->name : "";
      TypeRef funcType = srcFunc.funcType;
      Value   elem     = srcFunc.elem;
      if (check(!srcFunc.funcType, "FCALL: funcType is NULL for '%s'", node->token->name))
         break;
      if (check(!srcFunc.elem, "FCALL: elem is NULL for '%s'", node->token->name))
         break;
      node->token->llvm.elem = LLVMBuildCall2(builder, funcType, elem, args, count, name);
      free(args);
      break;
   }
   case FDEC:
   {
      // debug("================================\n");
      // pnode(node, "");
      // debug("\n================================\n");
      enter_scope(node);
      TypeRef retType;
      if (node->token->retType == TUPLE)
      {
         int      n  = node->token->Tuple.count;
         TypeRef *ft = allocate(n, sizeof(TypeRef));
         for (int i = 0; i < n; i++)
            ft[i] = get_llvm_type(node->token->Tuple.types[i]);
         retType = LLVMStructTypeInContext(context, ft, n, 0);
         node->token->llvm.stType = retType;
         free(ft);
      }
      else if (node->token->retType == STRUCT_CALL && node->token->is_ref)
         retType = _pointer_type(get_llvm_type(node->token), 0);
      else
         retType = get_llvm_type(node->token);

      TypeRef *paramTypes  = NULL;
      int      param_count = node->left->cpos;
      int      fixed_count = param_count;
      int      _count      = param_count;

      if (node->token->is_variadic)
         _count = fixed_count + 1;

      if (param_count)
      {
         paramTypes = allocate(_count + 2, sizeof(TypeRef));

         for (int i = 0; i < fixed_count; i++)
         {
            Token *param = node->left->children[i]->token;
            if (param->is_ref) paramTypes[i] = _pointer_type(get_llvm_type(param), 0);
            else paramTypes[i] = get_llvm_type(param);
         }

         if (node->token->is_variadic)
            paramTypes[fixed_count] = i32;
      }

      char   *fname = node->token->name;
      TypeRef funcType;
      if (strcmp(fname, "main") == 0)
      {
         // Only use (i32, ptr) signature if os global is actually accessed
         bool os_used = false;
         for (int i = 0; i < global->cpos; i++)
         {
            Node *child = global->children[i];
            if (child->token->type == STRUCT_CALL && child->token->is_global
                && strcmp(child->token->name, "os") == 0 && child->token->used > 0)
            { os_used = true; break; }
         }
         if (os_used)
         {
            TypeRef argv_type  = _pointer_type(_pointer_type(i8, 0), 0); // char**
            TypeRef main_pts[] = { i32, argv_type };
            funcType = _function_type(retType, main_pts, 2, false);
         }
         else
            funcType = _function_type(retType, NULL, 0, false);
      }
      else
         funcType = _function_type(retType, paramTypes, _count, node->token->is_variadic);
      Value existingFunc = _get_named_function(fname);
      if (existingFunc) node->token->llvm.elem = existingFunc;
      else node->token->llvm.elem = _add_function(fname, funcType);
      node->token->llvm.funcType = funcType;


      if (!node->token->is_proto)
      {
         if (enable_asan)
            LLVMAddAttributeAtIndex(
               node->token->llvm.elem,
               LLVMAttributeFunctionIndex,
               LLVMCreateEnumAttribute(
                  context,
                  LLVMGetEnumAttributeKindForName(
                     "sanitize_address", 16),
                  0
                  ));

         // Debug info for this function
         LLVMMetadataRef di_func_type = LLVMDIBuilderCreateSubroutineType(
            di_builder, di_file, NULL, 0, LLVMDIFlagZero);

         LLVMMetadataRef di_func = LLVMDIBuilderCreateFunction(
            di_builder,
            di_compile_unit,
            fname, strlen(fname),
            fname, strlen(fname),
            di_file,
            node->token->line,
            di_func_type,
            0,
            1,
            node->token->line,
            LLVMDIFlagZero,
            0
            );
         LLVMSetSubprogram(node->token->llvm.elem, di_func);
         di_current_scope = di_func;
         _entry(node->token);

         // set a valid location for param allocas
         LLVMMetadataRef entry_loc = LLVMDIBuilderCreateDebugLocation(
            context, node->token->line, 0, di_func, NULL);
         LLVMSetCurrentDebugLocation2(builder, entry_loc);

         int param_idx = 0;
         for (int i = 0; i < fixed_count; i++)
         {
            Token *param_token = node->left->children[i]->token;
            Value  param       = LLVMGetParam(node->token->llvm.elem, param_idx++);
            LLVMSetValueName(param, param_token->name);
            _alloca(param_token);
            param_token->is_dec = false;
            LLVMBuildStore(builder, param, param_token->llvm.elem);
         }

         // Auto-insert nested struct field init calls before user body
         {
            int  flen       = strlen(fname);
            bool is_init_fn = flen > 5 && strcmp(fname + flen - 5, ".init") == 0;
            if (is_init_fn && fixed_count >= 1)
            {
               Token *self_tok = node->left->children[0]->token;
               if (self_tok->type == STRUCT_CALL && self_tok->is_ref)
               {
                  Node   *sd       = self_tok->Struct.ptr;
                  TypeRef st_type  = sd->token->llvm.stType;
                  TypeRef ptr_type = _pointer_type(st_type, 0);
                  Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
                  for (int i = 0; i < sd->cpos; i++)
                  {
                     Token *field = sd->children[i]->token;
                     if (field->type != STRUCT_CALL) continue;
                     Node  *fsd   = field->Struct.ptr;
                     char  *qname = strjoin(fsd->token->name, ".init", NULL);
                     Value  ffn   = _get_named_function(qname);
                     free(qname);
                     if (!ffn) continue;
                     Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
                     Value field_ptr = _build_gep2(st_type, self_ptr, indices, 2, field->name);
                     Value args[]    = { field_ptr };
                     _build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
                  }
               }
            }
         }

         // Inject global struct init calls at the start of main
         if (strcmp(fname, "main") == 0)
         {
            for (int i = 0; i < global->cpos; i++)
            {
               Node *child = global->children[i];
               if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
               if (child->token->used == 0) continue;
               char *qname  = strjoin(child->token->Struct.ptr->token->name, ".init", NULL);
               Value init_f = _get_named_function(qname);
               free(qname);
               if (!init_f) continue;
               Value args[] = { child->token->llvm.elem };
               _build_call2(_global_get_value_type(init_f), init_f, args, 1, "");
            }
            // Populate os.argc and os.argv from main params (only if os is used)
            for (int i = 0; i < global->cpos; i++)
            {
               Node *child = global->children[i];
               if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
               if (child->token->used == 0) continue;
               if (strcmp(child->token->name, "os") != 0) continue;
               Value argc_val = LLVMGetParam(node->token->llvm.elem, 0);
               Value argv_val = LLVMGetParam(node->token->llvm.elem, 1);
               Value argc_ptr = struct_field_ptr(child->token, 0, "os.argc");
               _build_store(argc_val, argc_ptr);
               Value argv_ptr = struct_field_ptr(child->token, 1, "os.argv");
               _build_store(argv_val, argv_ptr);
               break;
            }
         }

         #if USING_HOIST
         for (int i = 0; i < node->cpos; i++)
            hoist_allocas(node->children[i]);
         #endif

         for (int i = 0; i < node->cpos; i++)
         {
            gen_asm(node->children[i]);
            if (_get_basic_block_terminator(_get_insert_block()))
               break;
         }

      }
      exit_scope();
      di_current_scope = di_compile_unit;
      break;
   }
   case TUPLE_UNPACK:
   {
      // Evaluate RHS (function call returning a tuple/aggregate)
      gen_asm(node->left);
      Value tuple_val = node->left->token->llvm.elem;
      // Alloca each LHS variable and extract the corresponding field
      for (int i = 0; i < node->cpos; i++)
      {
         Node *lhs   = node->children[i];
         gen_asm(lhs); // handles is_dec → _alloca, sets is_dec = false
         Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
         _build_store(field, lhs->token->llvm.elem);
      }
      break;
   }
   case WHILE:
   {
      enter_scope(node);
      Block start = _append_block("while.start");
      Block then  = _append_block("while.then");
      Block end   = _append_block("while.end");

      // Store blocks for break/continue
      node->token->llvm.start = start;
      node->token->llvm.then  = then;
      node->token->llvm.end   = end;

      _branch(start);

      _position_at(start);
      gen_asm(node->left); // condition
      load_if_necessary(node->left);
      _condition(node->left->token->llvm.elem, then, end);

      _position_at(then);
      for (int i = 0; i < node->cpos; i++)
      {
         if (_get_basic_block_terminator(_get_insert_block()))
            break;
         gen_asm(node->children[i]);
      }
      _branch(start);

      _position_at(end);
      exit_scope();
      break;
   }
   case IF:
   {
      enter_scope(node);

      Block if_start = _append_block("if.start");
      Block end      = _append_block("if.end");

      // Store blocks for potential break/continue
      node->token->llvm.start = if_start;
      node->token->llvm.end   = end;

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
               then  = _append_block("if.then");
            }
            else
            {
               start = curr->token->llvm.bloc;
               then  = _append_block("elif.then");
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
            load_if_necessary(curr->left);
            _condition(curr->left->token->llvm.elem, then, next);

            _position_at(then);
            for (int i = 0; i < curr->cpos; i++)
            {
               gen_asm(curr->children[i]);
               if (_get_basic_block_terminator(_get_insert_block()))
                  break;
            }
            _branch(end);

            if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
               curr->right->token->llvm.bloc = next;
         }
         else if (curr->token->type == ELSE)
         {
            _position_at(curr->token->llvm.bloc);
            for (int i = 0; i < curr->cpos; i++)
            {
               gen_asm(curr->children[i]);
               if (_get_basic_block_terminator(_get_insert_block()))
                  break;
            }
            _branch(end);
         }
         curr = curr->right;
      }
      _position_at(end);
      exit_scope();
      break;
   }
   case BREAK:
   {
      for (int i = scope_pos; i > 0; i--)
      {
         if (Gscope[i]->token->type == WHILE)
         {
            _branch(Gscope[i]->token->llvm.end);
            return;
         }
      }
      check(1, "break outside loop");
      break;
   }
   case CONTINUE:
   {
      for (int i = scope_pos; i > 0; i--)
      {
         if (Gscope[i]->token->type == WHILE)
         {
            _branch(Gscope[i]->token->llvm.start);
            return;
         }
      }
      check(1, "continue outside loop");
      break;
   }
   case RETURN:
   {
      emit_scope_clean(scope, 0);

      // Auto-insert nested field clean calls for .clean methods before return
      {
         Node *fdec = NULL;
         for (int i = scope_pos; i >= 0; i--)
            if (Gscope[i] && Gscope[i]->token->type == FDEC)
            { fdec = Gscope[i]; break; }
         if (fdec)
         {
            char *fn_name    = fdec->token->name;
            int   fn_len     = strlen(fn_name);
            bool  is_clean_f = fn_len > 6 && strcmp(fn_name + fn_len - 6, ".clean") == 0;
            int   fxd_count  = fdec->left->cpos;
            if (is_clean_f && fxd_count >= 1)
            {
               Token *self_tok = fdec->left->children[0]->token;
               if (self_tok->type == STRUCT_CALL && self_tok->is_ref)
               {
                  Node   *sd       = self_tok->Struct.ptr;
                  TypeRef st_type  = sd->token->llvm.stType;
                  TypeRef ptr_type = _pointer_type(st_type, 0);
                  Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
                  for (int i = 0; i < sd->cpos; i++)
                  {
                     Token *field = sd->children[i]->token;
                     if (field->type != STRUCT_CALL) continue;
                     Node  *fsd   = field->Struct.ptr;
                     char  *qname = strjoin(fsd->token->name, ".clean", NULL);
                     Value  ffn   = _get_named_function(qname);
                     free(qname);
                     if (!ffn) continue;
                     Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
                     Value field_ptr = _build_gep2(st_type, self_ptr, indices, 2, field->name);
                     Value args[]    = { field_ptr };
                     _build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
                  }
               }
            }
         }
      }

      // Inject global struct clean calls before main returns
      {
         Node *fdec = NULL;
         for (int i = scope_pos; i >= 0; i--)
            if (Gscope[i] && Gscope[i]->token->type == FDEC)
            { fdec = Gscope[i]; break; }
         if (fdec && strcmp(fdec->token->name, "main") == 0)
         {
            for (int i = 0; i < global->cpos; i++)
            {
               Node *child = global->children[i];
               if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
               if (child->token->used == 0) continue;
               char *qname   = strjoin(child->token->Struct.ptr->token->name, ".clean", NULL);
               Value clean_f = _get_named_function(qname);
               free(qname);
               if (!clean_f) continue;
               Value args[] = { child->token->llvm.elem };
               _build_call2(_global_get_value_type(clean_f), clean_f, args, 1, "");
            }
         }
      }

      // Tuple return: pack multiple values into an anonymous struct
      {
         Node *fdec = NULL;
         for (int i = scope_pos; i >= 0; i--)
            if (Gscope[i] && Gscope[i]->token->type == FDEC) { fdec = Gscope[i]; break; }
         if (fdec && fdec->token->retType == TUPLE && node->cpos > 0)
         {
            TypeRef tuple_type = fdec->token->llvm.stType;
            Value   agg        = LLVMGetUndef(tuple_type);
            for (int i = 0; i < node->cpos; i++)
            {
               gen_asm(node->children[i]);
               load_if_necessary(node->children[i]);
               Value v = node->children[i]->token->llvm.elem;
               agg = LLVMBuildInsertValue(builder, agg, v, i, "");
            }
            LLVMBuildRet(builder, agg);
            break;
         }
      }

      if (node->left && node->left->token->type != VOID)
      {
         gen_asm(node->left);
         Token *ret_tok = node->left->token;

         // Find the enclosing FDEC to check its return type
         Node *fdec = NULL;
         for (int i = scope_pos; i >= 0; i--)
            if (Gscope[i] && Gscope[i]->token->type == FDEC)
            { fdec = Gscope[i]; break; }

         bool fdec_returns_ref    = fdec && fdec->token->is_ref;
         bool fdec_returns_struct = fdec && fdec->token->retType == STRUCT_CALL;

         if (fdec_returns_struct && !fdec_returns_ref)
         {
            // return by value: load the full struct
            Value val = load_value(ret_tok);
            LLVMBuildRet(builder, val);
            break;
         }
         else if (fdec_returns_struct && fdec_returns_ref)
         {
            // return by ref: return the pointer directly (self->llvm.elem holds User*)
            // self is stored as alloca of ptr, so load the ptr
            TypeRef st_type  = get_llvm_type(ret_tok);
            TypeRef ptr_type = _pointer_type(st_type, 0);
            Value   ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
            LLVMBuildRet(builder, ptr);
            break;
         }
         load_if_necessary(node->left);
      }
      if (node->left)
         _build_return(node->left->token);
      break;
   }
   case AS:
   {
      gen_asm(left);
      load_if_necessary(left);

      Value    source     = left->token->llvm.elem;
      TypeRef  sourceType = LLVMTypeOf(source);
      TypeRef  targetType = get_llvm_type(node->right->token);

      TypeKind sourceKind = LLVMGetTypeKind(sourceType);
      TypeKind targetKind = LLVMGetTypeKind(targetType);

      Value    result;

      // Pointer to integer
      if (sourceKind == PointerTypeKind && targetKind == IntegerTypeKind)
         result = LLVMBuildPtrToInt(builder, source, targetType, "as");
      // Integer to pointer
      else if (sourceKind == IntegerTypeKind && targetKind == PointerTypeKind)
         result = LLVMBuildIntToPtr(builder, source, targetType, "as");
      // Integer to integer
      else if (sourceKind == IntegerTypeKind && targetKind == IntegerTypeKind)
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
      else if (sourceKind == PointerTypeKind && targetKind == PointerTypeKind)
         result = LLVMBuildBitCast(builder, source, targetType, "as");

      else
      {
         check(1, "unsupported cast from %d to %d", sourceKind, targetKind);
         result = source;
      }

      node->token->llvm.elem      = result;
      node->token->llvm.is_loaded = true;
      break;
   }
   case STRUCT_DEF:
   {
      if (node->token->used == 0) return;
      // Ensure nested struct field types are also emitted
      for (int i = 0; i < node->cpos; i++)
      {
         Token *ft = node->children[i]->token;
         if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0)
         {
            ft->Struct.ptr->token->used++;
            gen_asm(ft->Struct.ptr);
         }
      }
      int      pos   = node->cpos;
      TypeRef *types = allocate((pos + 1), sizeof(TypeRef));
      int      j     = 0;
      for (int i = 0; i < pos; i++)
         types[j++] = get_llvm_type(node->children[i]->token);
      char *struct_name = strjoin("struct.", node->token->name, NULL);
      node->token->llvm.stType = _named_struct_type(struct_name, types, j, 0);
      free(struct_name);
      free(types);

      TypeRef st_type     = node->token->llvm.stType;
      TypeRef ptr_type    = _pointer_type(st_type, 0);
      TypeRef lc_params[] = { ptr_type };
      TypeRef lc_fn_type  = _function_type(vd, lc_params, 1, 0);

      if (!node->token->is_init)
      {
         char *fname = strjoin(node->token->name, ".init", NULL);
         Value fn    = _add_function(fname, lc_fn_type);
         free(fname);
         Block entry = _append_basic_block_in_context(fn, "entry");
         _position_at(entry);
         LLVMSetCurrentDebugLocation2(builder, NULL);
         Value self = LLVMGetParam(fn, 0);
         LLVMBuildStore(builder, LLVMConstNull(st_type), self);
         for (int i = 0; i < node->cpos; i++)
         {
            Token *field = node->children[i]->token;
            if (field->type != STRUCT_CALL) continue;
            Node  *fsd   = field->Struct.ptr;
            char  *qname = strjoin(fsd->token->name, ".init", NULL);
            Value  ffn   = _get_named_function(qname);
            free(qname);
            if (!ffn) continue;
            Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
            Value field_ptr = _build_gep2(st_type, self, indices, 2, field->name);
            Value args[]    = { field_ptr };
            _build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
         }
         LLVMBuildRetVoid(builder);
         node->token->is_init = true;
      }

      if (!node->token->is_clean)
      {
         char *fname = strjoin(node->token->name, ".clean", NULL);
         Value fn    = _add_function(fname, lc_fn_type);
         free(fname);
         Block entry = _append_basic_block_in_context(fn, "entry");
         _position_at(entry);
         LLVMSetCurrentDebugLocation2(builder, NULL);
         Value self = LLVMGetParam(fn, 0);
         for (int i = 0; i < node->cpos; i++)
         {
            Token *field = node->children[i]->token;
            if (field->type != STRUCT_CALL) continue;
            Node  *fsd   = field->Struct.ptr;
            char  *qname = strjoin(fsd->token->name, ".clean", NULL);
            Value  ffn   = _get_named_function(qname);
            free(qname);
            if (!ffn) continue;
            Value indices[] = { _const_int(i32, 0, 0), _const_int(i32, i, 0) };
            Value field_ptr = _build_gep2(st_type, self, indices, 2, field->name);
            Value args[]    = { field_ptr };
            _build_call2(_global_get_value_type(ffn), ffn, args, 1, "");
         }
         LLVMBuildRetVoid(builder);
         node->token->is_clean = true;
      }

      for (int i = 0; i < node->mpos; i++)
         gen_asm(node->methods[i]);
      break;
   }
   case DOT:
   {
      gen_asm(node->left);

      Token *struct_tok  = node->left->token;
      int    field_index = node->right->token->Struct.index;

      // If the struct is a ref, we need to load the pointer first
      // before indexing into it, otherwise we GEP into the alloca
      // that holds the pointer, not into the struct itself.
      if (struct_tok->is_ref)
      {
         TypeRef struct_type = get_llvm_type(struct_tok);
         Value   struct_ptr  = LLVMBuildLoad2(builder,
                                              _pointer_type(struct_type, 0),
                                              struct_tok->llvm.elem,
                                              struct_tok->name
                                              );
         Value indices[] =
         {
            _const_int(i32, 0, 0),
            _const_int(i32, field_index, 0),
         };
         node->token->llvm.elem = _build_gep2(
            struct_type, struct_ptr, indices, 2, node->right->token->name
            );
      }
      else
      {
         Value field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);
         node->token->llvm.elem = field_ptr;
      }
      break;
   }
   case ACCESS:
   {
      // bool enable_bounds_check = false;
      gen_asm(node->left);

      if (node->left->token->type == STRUCT_CALL)
      {
         Token *struct_tok  = node->left->token;
         int    field_index = node->right->token->Struct.index;

         Value  field_ptr   = struct_field_ptr(struct_tok, field_index, node->right->token->name);

         node->token->llvm.elem = field_ptr;
         /* retType already set by gen_ir; mark as NOT loaded so
            load_if_necessary() will emit the load when reading. */
         break;
      }

      gen_asm(node->right);
      Value leftValue;
      if (node->left->token->is_ref)
      {
         todo(1, "stop");
         // If left is a ref, dereference it to get the actual array
         leftValue = load_value(node->left->token);
      }
      else
      {
         // todo(1, "stop");
         load_if_necessary(node->left);
         leftValue = node->left->token->llvm.elem;
         TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
         if (kind == PointerTypeKind && node->left->token->name &&
             !node->left->token->llvm.is_loaded &&
             node->left->token->type != STACK)
         {
            leftValue = LLVMBuildLoad2(builder, p8, leftValue, "ptr_load");
         }
      }

      // Load the index (handles refs)
      load_if_necessary(node->right);
      Value   rightRef = node->right->token->llvm.elem;

      TypeRef element_type;
      Type    left_elem_type = node->left->token->retType
                      ? node->left->token->retType
                      : node->left->token->type;
      if (left_elem_type == CHARS)
      {
         element_type         = i8;
         node->token->retType = CHAR;
      }
      else if (left_elem_type == ARRAY_TYPE || left_elem_type == ARRAY)
      {
         Token tmp = {.type = node->left->token->Array.elem_type};
         element_type         = get_llvm_type(&tmp);
         node->token->retType = node->left->token->Array.elem_type;
      }
      else if (node->left->token->type == HEAP || node->left->token->type == ARRAY)
      {
         check(1, "hello");
         Token tmp = {.type = node->left->token->Array.elem_type};
         element_type         = get_llvm_type(&tmp);
         node->token->retType = node->left->token->Array.elem_type;
      }
      else
      {
         element_type         = get_llvm_type(node->left->token);
         node->token->retType = node->left->token->type;
      }

      // Add bounds checking if enabled
      if (enable_bounds_check)
      {
         // We need to track array sizes - for now we'll use a conservative approach
         // For CHARS (strings), we can use strlen at runtime
         // For STACK arrays, we need to store the size

         Value size_val       = NULL;

         Type  left_elem_type = node->left->token->retType
                      ? node->left->token->retType
                      : node->left->token->type;
         if (left_elem_type == CHARS)
         {
            // For strings, we need to get the length
            // First check if it's a string literal or a variable
            if (node->left->token->name && !left->token->llvm.array_size)
            {
               // It's a variable - we need strlen
               Value strlen_func = _get_named_function("strlen");
               if (!strlen_func)
               {
                  TypeRef strlen_type = _function_type(i64, (TypeRef[]) {p8}, 1, false);
                  strlen_func = _add_function("strlen", strlen_type);
               }
               Value strlen_result = _build_call2(_global_get_value_type(strlen_func), strlen_func,
                                                  (Value[]) {leftValue}, 1, "strlen");
               size_val = _build_trunc(strlen_result, i32, "size");
            }
            else if (node->left->token->llvm.array_size)
            {
               size_val = node->left->token->llvm.array_size;
            }
            else
            {
               // String literal - we know the size at compile time
               // This is handled in _chars function
               size_val = _const_int(i32, strlen(node->left->token->Chars.value), 0);
            }
         }
         else if (node->left->token->type == STACK)
         {
            // For STACK arrays, we should have stored the size
            // For now, use a placeholder - you'll need to enhance this
            size_val = _const_int(i32, 1000000, 0); // Conservative large value
         }

         if (size_val)
         {
            // Get bounds check function
            Value bounds_check = _get_named_function("__bounds_check");

            // Create filename string
            static Value filename_str = NULL;

            if (!filename_str)
            {
               char filename[256] = {0};

               if (getcwd(filename, sizeof(filename)) != NULL)
               {
                  size_t len = strlen(filename);
                  snprintf(filename + len, sizeof(filename) - len, "/%s", "input_file");
                  filename_str = _const_chars(filename, "filename");
               }
            }


            // Call bounds check: __bounds_check(index, size, line, filename)
            Value line_val = _const_int(i32, node->token->line, 0);
            _build_call2(_global_get_value_type(bounds_check), bounds_check,
                         (Value[]) {rightRef, size_val, line_val, filename_str}, 4, "");
         }
      }

      Value indices[] = { rightRef };
      Value gep       = _build_gep2(element_type, leftValue, indices, 1, "ACCESS");
      node->token->llvm.elem = gep;

      break;
   }
   case TYPEOF: case SIZEOF:
   {
      Token *type_tok = node->left->token;
      if (node->token->type == TYPEOF)
      {
         node->token->llvm.elem      = _const_chars(node->token->Chars.value, "typeof");
         node->token->llvm.is_loaded = true;
      }
      else
      {
         TypeRef    type = get_llvm_type(type_tok);
         TargetData td   = _get_module_data_layout(module);
         size_t     size = _abi_size_of_type(td, type);
         node->token->Int.value      = (long long)size;
         node->token->llvm.elem      = _const_int(i32, size, 0);
         node->token->llvm.is_loaded = true;
      }
      break;
   }
   case OUTPUT:
   {
      int    argc = node->left->cpos;
      Node **argv = node->left->children;

      // Compute format string capacity: literals + format specifiers + struct expansion
      int fmt_cap = 64;
      for (int i = 0; i < argc; i++)
         if (argv[i]->token->type == CHARS && !argv[i]->token->name)
            fmt_cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
         else
            fmt_cap += 128;

      char  *fmt   = allocate(fmt_cap, 1);
      Value *args  = allocate(256, sizeof(Value));
      int    fpos  = 0;
      int    nargs = 0;

      for (int i = 0; i < argc; i++)
      {
         gen_asm(argv[i]);
         append_output_arg(argv[i]->token, fmt, &fpos, args, &nargs);
      }
      fmt[fpos] = '\0';

      Value printf_fn = _get_named_function("printf");
      if (!printf_fn)
      {
         TypeRef params[]  = {p8, i32}; // fmt, nargs (language variadic convention)
         TypeRef printf_ft = _function_type(i32, params, 2, 1);
         printf_fn = _add_function("printf", printf_ft);
      }
      TypeRef printf_ft = _global_get_value_type(printf_fn);
      // Follow the language's variadic calling convention: insert arg count between
      // fixed params and variadic args — same as FCALL does for variadic functions
      Value *call_args = allocate(nargs + 3, sizeof(Value));
      call_args[0] = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
      call_args[1] = _const_int(i32, nargs, 0);
      memcpy(call_args + 2, args, nargs * sizeof(Value));
      LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 2, "");
      free(fmt); free(args); free(call_args);
      break;
   }
   default:
      pnode(node, "");
      todo(1, "handle this case %s", to_string(node->token->type));
      // seg();
      break;
   }
}
