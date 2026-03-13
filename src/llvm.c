#include "header.h"

// ----------------------------------------------------------------------------
// Utility / High-level helpers
// ----------------------------------------------------------------------------
Value _load2(Token *token)
{
   if (token->llvm.is_loaded) return token->llvm.elem;
   if (!token->name && !includes(token->type, DOT, ACCESS, 0))
      return token->llvm.elem;

   char *name = token->name;
   if (token->type == DOT)    name = to_string(DOT);
   if (token->type == ACCESS) name = to_string(ACCESS);
   return _build_load2(get_llvm_type(token), token->llvm.elem, name);
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
      new->llvm.elem      = _load2(token);
      new->llvm.is_loaded = true;
      node->token         = new;
   }
}

void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   if (token->is_ref)
      type = _pointer_type(type, 0);

   Block current     = _get_insert_block();
   Value func        = _get_basic_block_parent(current);
   Block entry       = _get_entry_basic_block(func);
   Value last_alloca = NULL;
   Value inst        = LLVMGetFirstInstruction(entry);
   while (inst && LLVMGetInstructionOpcode(inst) == LLVMAlloca)
   {
      last_alloca = inst;
      inst        = LLVMGetNextInstruction(inst);
   }

   if (last_alloca)
   {
      Value next = LLVMGetNextInstruction(last_alloca);
      if (next) LLVMPositionBuilderBefore(builder, next);
      else _position_at(entry);
   }
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
   if (token->is_ref)
   {
      TypeRef type = get_llvm_type(token);
      Value   null = LLVMConstNull(_pointer_type(type, 0));
      return null;
   }
   check(1, "handle this case %s", to_string(token->type));
   return NULL;
}

void _const_value(Token *token)
{
   TypeRef   type      = get_llvm_type(token);
   long long value     = 0;
   char     *processed = NULL;
   char      name[200];

   switch (token->type)
   {
   case INT:  value = (long long)token->Int.value; break;
   case BOOL: value = (long long)token->Bool.value; break;
   case CHAR: value = (int)token->Char.value; break;
   case CHARS:
   {
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
      break;
   }
   default: check(1, "handle this case %s", to_string(token->type)); return;
   }
   if (token->type == CHARS)
      token->llvm.elem = _const_chars(processed, name);
   else
      token->llvm.elem = _const_int(type, value, 0);
   free(processed);
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

void _entry(Token *token)
{
   Block entry = _append_basic_block_in_context(token->llvm.elem, "entry");
   _position_at(entry);
}

Value _get_param_with_name(Token *fn, int index, char *name)
{
   Value param = LLVMGetParam(fn->llvm.elem, index);
   LLVMSetValueName(param, name);
   return param;
}


// ----------------------------------------------------------------------------
// LLVM Builder wrappers
// ----------------------------------------------------------------------------

Value _build_store(Value val, Value ptr) { return LLVMBuildStore(builder, val, ptr); }

Value _build_load2(TypeRef ty, Value ptr, char *name) { return LLVMBuildLoad2(builder, ty, ptr, name); }

Value _build_alloca(TypeRef ty, char *name) { return LLVMBuildAlloca(builder, ty, name); }

Value _build_add(Value lhs, Value rhs, char *name) { return LLVMBuildAdd(builder, lhs, rhs, name); }

Value _build_sub(Value lhs, Value rhs, char *name) { return LLVMBuildSub(builder, lhs, rhs, name); }

Value _build_mul(Value lhs, Value rhs, char *name) { return LLVMBuildMul(builder, lhs, rhs, name); }

Value _build_sdiv(Value lhs, Value rhs, char *name) { return LLVMBuildSDiv(builder, lhs, rhs, name); }

Value _build_srem(Value lhs, Value rhs, char *name) { return LLVMBuildSRem(builder, lhs, rhs, name); }

Value _build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name) { return LLVMBuildICmp(builder, op, lhs, rhs, name); }

Value _build_and(Value lhs, Value rhs, char *name) { return LLVMBuildAnd(builder, lhs, rhs, name); }

Value _build_or(Value lhs, Value rhs, char *name) { return LLVMBuildOr(builder, lhs, rhs, name); }

Value _build_ret(Value val) { return LLVMBuildRet(builder, val); }

Value _build_ret_void() { return LLVMBuildRetVoid(builder); }

void _branch(Block bloc)
{
   if (!_get_basic_block_terminator(_get_insert_block()))
      LLVMBuildBr(builder, bloc);
}

Block _append_block(char *name)
{
   Value parent = _get_basic_block_parent(_get_insert_block());
   return _append_basic_block_in_context(parent, name);
}

void _condition(Value cond, Block then_block, Block else_block)
{
   LLVMBuildCondBr(builder, cond, then_block, else_block);
}

Value _build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name)
{
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

Value _const_chars(char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value _build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned indece, char *name)
{
   return LLVMBuildGEP2(builder, ty, ptr, indices, indece, name);
}

Value _build_bit_cast(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

Value _build_sext(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

Value _build_trunc(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

Value _build_int_to_ptr(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

Value _build_ptr_to_int(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

Value _build_array_alloca(TypeRef ty, Value val, char *name)
{
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

Value _build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args, Block then_block, Block catch_block, char *name)
{
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

Value _build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses, char *name)
{
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

Value _build_extract_value(Value agg_val, unsigned index, char *name)
{
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

Value _build_va_arg(Value list, TypeRef ty, char *name)
{
   return LLVMBuildVAArg(builder, list, ty, name);
}

Value _build_unreachable()
{
   return LLVMBuildUnreachable(builder);
}

Value _build_not(Token *token, char *name)
{
   return LLVMBuildNot(builder, token->llvm.elem, name);
}

Value _build_memcpy(Value dest, Value src, Value size)
{
   return LLVMBuildMemCpy(builder, dest, 0, src, 0, size);
}

// ----------------------------------------------------------------------------
// LLVM Type / Constant wrappers
// ----------------------------------------------------------------------------

TypeRef _pointer_type(TypeRef element_ty, unsigned address_space)
{
   return LLVMPointerType(element_ty, address_space);
}

TypeRef _function_type(TypeRef retType, TypeRef *types, int count, int is_variadic)
{
   return LLVMFunctionType(retType, types, count, is_variadic);
}

TypeRef _array_type(TypeRef element_type, unsigned element_count)
{
   return LLVMArrayType(element_type, element_count);
}

TypeRef _struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed)
{
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

Value _const_int(TypeRef ref_type, unsigned long long n, int sign_extend)
{
   return LLVMConstInt(ref_type, n, sign_extend);
}

Value _const_null(TypeRef ty)
{
   return LLVMConstNull(ty);
}

// ----------------------------------------------------------------------------
// LLVM Module / Function / Block wrappers
// ----------------------------------------------------------------------------

Value _get_named_function(char *name)
{
   return LLVMGetNamedFunction(module, name);
}

Value _add_function(char *name, TypeRef function_type)
{
   Value f = LLVMGetNamedFunction(module, name);
   if (f) return f;
   return LLVMAddFunction(module, name, function_type);
}

void set_debug_location(Token *token)
{
   if (!token || !di_builder || !di_current_scope) return;
   if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
   LLVMMetadataRef loc = LLVMDIBuilderCreateDebugLocation(
      context, token->line,0,
      di_current_scope, NULL
      );
   LLVMSetCurrentDebugLocation2(builder, loc);
}

Value _get_param(Value fn, unsigned index)
{
   return LLVMGetParam(fn, index);
}

Block _append_basic_block_in_context(Value func, char *name)
{
   return LLVMAppendBasicBlockInContext(context, func, name);
}

Block _get_insert_block()
{
   return LLVMGetInsertBlock(builder);
}

Value _get_basic_block_parent(Block block)
{
   return LLVMGetBasicBlockParent(block);
}

Block _get_entry_basic_block(Value func)
{
   return LLVMGetEntryBasicBlock(func);
}

void _position_at(Block block)
{
   LLVMPositionBuilderAtEnd(builder, block);
}

Value _get_basic_block_terminator(Block block)
{
   return LLVMGetBasicBlockTerminator(block);
}

TypeRef _global_get_value_type(Value global)
{
   return LLVMGlobalGetValueType(global);
}

// ----------------------------------------------------------------------------
// LLVM Introspection wrappers
// ----------------------------------------------------------------------------

TypeRef _type_of(Value val)
{
   return LLVMTypeOf(val);
}

TypeKind _get_type_kind(TypeRef ty)
{
   return LLVMGetTypeKind(ty);
}

unsigned _get_int_type_width(TypeRef ref_ty)
{
   return LLVMGetIntTypeWidth(ref_ty);
}

TypeRef _get_type_by_name2(char *name)
{
   return LLVMGetTypeByName2(context, name);
}

TypeRef _get_return_type(TypeRef function_type)
{
   return LLVMGetReturnType(function_type);
}

const char *_get_target(Module m)
{
   return LLVMGetTarget(m);
}

void _set_personality_fn(Value func, Value pers_fn)
{
   LLVMSetPersonalityFn(func, pers_fn);
}

void _add_clause(Value landing_pad, Value clause_val)
{
   LLVMAddClause(landing_pad, clause_val);
}

size_t _abi_size_of_type(TargetData td, TypeRef ty)
{
   return LLVMABISizeOfType(td, ty);
}

TargetData _get_module_data_layout(Module m)
{
   return LLVMGetModuleDataLayout(m);
}