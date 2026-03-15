#pragma once

// ----------------------------------------------------------------------------
// LLVM Wrappers (static inline)
// ----------------------------------------------------------------------------
static inline Value _build_store(Value val, Value ptr) { return LLVMBuildStore(builder, val, ptr); }

static inline Value _build_alloca(TypeRef ty, char *name) { return LLVMBuildAlloca(builder, ty, name); }

static inline Value _build_add(Value lhs, Value rhs, char *name) { return LLVMBuildAdd(builder, lhs, rhs, name); }

static inline Value _build_sub(Value lhs, Value rhs, char *name) { return LLVMBuildSub(builder, lhs, rhs, name); }

static inline Value _build_mul(Value lhs, Value rhs, char *name) { return LLVMBuildMul(builder, lhs, rhs, name); }

static inline Value _build_sdiv(Value lhs, Value rhs, char *name) { return LLVMBuildSDiv(builder, lhs, rhs, name); }

static inline Value _build_srem(Value lhs, Value rhs, char *name) { return LLVMBuildSRem(builder, lhs, rhs, name); }

static inline Value _build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name) { return LLVMBuildICmp(builder, op, lhs, rhs, name); }

static inline Value _build_and(Value lhs, Value rhs, char *name) { return LLVMBuildAnd(builder, lhs, rhs, name); }

static inline Value _build_or(Value lhs, Value rhs, char *name) { return LLVMBuildOr(builder, lhs, rhs, name); }

static inline Value _build_ret(Value val) { return LLVMBuildRet(builder, val); }

static inline LLVMIntPredicate icmp_predicate(Type op)
{
   switch (op)
   {
   case LESS:        return LLVMIntSLT;
   case GREAT:       return LLVMIntSGT;
   case EQUAL:       return LLVMIntEQ;
   case LESS_EQUAL:  return LLVMIntSLE;
   case GREAT_EQUAL: return LLVMIntSGE;
   default:          return LLVMIntNE; // NOT_EQUAL
   }
}

static inline LLVMRealPredicate fcmp_predicate(Type op)
{
   switch (op)
   {
   case LESS:        return LLVMRealOLT;
   case GREAT:       return LLVMRealOGT;
   case EQUAL:       return LLVMRealOEQ;
   case LESS_EQUAL:  return LLVMRealOLE;
   case GREAT_EQUAL: return LLVMRealOGE;
   default:          return LLVMRealONE; // NOT_EQUAL
   }
}

static inline int is_float_value(Value v)
{
   LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
   return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

static inline Type assign_base_op(Type assign_op)
{
   switch (assign_op)
   {
   case ADD_ASSIGN: return ADD;
   case SUB_ASSIGN: return SUB;
   case MUL_ASSIGN: return MUL;
   case DIV_ASSIGN: return DIV;
   default:         return MOD; // MOD_ASSIGN
   }
}

static inline Value build_binary_op(Type op, Value l, Value r)
{
   char *name = to_string(op);
   if (is_float_value(l))
   {
      if (includes(op, COMPARISON_OPS, 0))
         return LLVMBuildFCmp(builder, fcmp_predicate(op), l, r, name);
      switch (op)
      {
      case ADD: return LLVMBuildFAdd(builder, l, r, name);
      case SUB: return LLVMBuildFSub(builder, l, r, name);
      case MUL: return LLVMBuildFMul(builder, l, r, name);
      case DIV: return LLVMBuildFDiv(builder, l, r, name);
      default:  todo(1, "build_binary_op: unhandled float op %s", name); return NULL;
      }
   }
   if (includes(op, COMPARISON_OPS, 0))
      return _build_icmp(icmp_predicate(op), l, r, name);
   switch (op)
   {
   case ADD:    return _build_add(l, r, name);
   case SUB:    return _build_sub(l, r, name);
   case MUL:    return _build_mul(l, r, name);
   case DIV:    return _build_sdiv(l, r, name);
   case MOD:    return _build_srem(l, r, name);
   case AND:    return _build_and(l, r, name);
   case OR:     return _build_or(l, r, name);
   case BAND:   return _build_and(l, r, name);
   case BOR:    return _build_or(l, r, name);
   case BXOR:   return LLVMBuildXor(builder, l, r, name);
   case LSHIFT: return LLVMBuildShl(builder, l, r, name);
   case RSHIFT: return LLVMBuildAShr(builder, l, r, name);
   default:     todo(1, "build_binary_op: unhandled op %s", name); return NULL;
   }
}

static inline Value _build_ret_void() { return LLVMBuildRetVoid(builder); }

static inline Block _append_basic_block_in_context(Value func, char *name)
{
   return LLVMAppendBasicBlockInContext(context, func, name);
}

static inline Block _get_insert_block()
{
   return LLVMGetInsertBlock(builder);
}

static inline Value _get_basic_block_parent(Block block)
{
   return LLVMGetBasicBlockParent(block);
}

static inline Value _get_basic_block_terminator(Block block)
{
   return LLVMGetBasicBlockTerminator(block);
}

static inline void _branch(Block bloc)
{
   if (!_get_basic_block_terminator(_get_insert_block()))
      LLVMBuildBr(builder, bloc);
}

static inline Block _append_block(char *name)
{
   Value parent = _get_basic_block_parent(_get_insert_block());
   return _append_basic_block_in_context(parent, name);
}

static inline void _condition(Value cond, Block then_block, Block else_block)
{
   LLVMBuildCondBr(builder, cond, then_block, else_block);
}

static inline Value _build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name)
{
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

static inline Value _const_chars(char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

static inline Value _build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned indece, char *name)
{
   return LLVMBuildGEP2(builder, ty, ptr, indices, indece, name);
}

static inline Value _build_bit_cast(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

static inline Value _build_sext(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

static inline Value _build_trunc(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

static inline Value _build_int_to_ptr(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

static inline Value _build_ptr_to_int(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

static inline Value _build_array_alloca(TypeRef ty, Value val, char *name)
{
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

static inline Value _build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args, Block then_block, Block catch_block, char *name)
{
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

static inline Value _build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses, char *name)
{
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

static inline Value _build_extract_value(Value agg_val, unsigned index, char *name)
{
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

static inline Value _build_va_arg(Value list, TypeRef ty, char *name)
{
   return LLVMBuildVAArg(builder, list, ty, name);
}

static inline Value _build_unreachable()
{
   return LLVMBuildUnreachable(builder);
}

static inline Value _build_not(Token *token, char *name)
{
   return LLVMBuildNot(builder, token->llvm.elem, name);
}

static inline Value _build_memcpy(Value dest, Value src, Value size)
{
   return LLVMBuildMemCpy(builder, dest, 0, src, 0, size);
}

static inline Value _const_int(TypeRef ref_type, unsigned long long n, int sign_extend)
{
   return LLVMConstInt(ref_type, n, sign_extend);
}

static inline Value _const_null(TypeRef ty)
{
   return LLVMConstNull(ty);
}

static inline TypeRef _pointer_type(TypeRef element_ty, unsigned address_space)
{
   return LLVMPointerType(element_ty, address_space);
}

static inline TypeRef _function_type(TypeRef retType, TypeRef *types, int count, int is_variadic)
{
   return LLVMFunctionType(retType, types, count, is_variadic);
}

static inline TypeRef _array_type(TypeRef element_type, unsigned element_count)
{
   return LLVMArrayType(element_type, element_count);
}

static inline TypeRef _struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed)
{
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

static inline TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed)
{
   TypeRef type = LLVMStructCreateNamed(context, name);
   LLVMStructSetBody(type, element_types, element_count, packed);
   return type;
}

static inline Value _get_named_function(char *name)
{
   return LLVMGetNamedFunction(module, name);
}

static inline Value _add_function(char *name, TypeRef function_type)
{
   Value f = LLVMGetNamedFunction(module, name);
   if (f) return f;
   return LLVMAddFunction(module, name, function_type);
}

static inline void set_debug_location(Token *token)
{
   if (!token || !di_builder || !di_current_scope) return;
   if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
   LLVMMetadataRef loc = LLVMDIBuilderCreateDebugLocation(
      context, token->line, 0,
      di_current_scope, NULL
      );
   LLVMSetCurrentDebugLocation2(builder, loc);
}

static inline Value _get_param(Value fn, unsigned index)
{
   return LLVMGetParam(fn, index);
}

static inline Block _get_entry_basic_block(Value func)
{
   return LLVMGetEntryBasicBlock(func);
}

static inline void _position_at(Block block)
{
   LLVMPositionBuilderAtEnd(builder, block);
}

static inline TypeRef _global_get_value_type(Value global)
{
   return LLVMGlobalGetValueType(global);
}

static inline TypeRef _type_of(Value val)
{
   return LLVMTypeOf(val);
}

static inline TypeKind _get_type_kind(TypeRef ty)
{
   return LLVMGetTypeKind(ty);
}

static inline unsigned _get_int_type_width(TypeRef ref_ty)
{
   return LLVMGetIntTypeWidth(ref_ty);
}

static inline TypeRef _get_type_by_name2(char *name)
{
   return LLVMGetTypeByName2(context, name);
}

static inline TypeRef _get_return_type(TypeRef function_type)
{
   return LLVMGetReturnType(function_type);
}

static inline const char *_get_target(Module m)
{
   return LLVMGetTarget(m);
}

static inline void _set_personality_fn(Value func, Value pers_fn)
{
   LLVMSetPersonalityFn(func, pers_fn);
}

static inline void _add_clause(Value landing_pad, Value clause_val)
{
   LLVMAddClause(landing_pad, clause_val);
}

static inline size_t _abi_size_of_type(TargetData td, TypeRef ty)
{
   return LLVMABISizeOfType(td, ty);
}

static inline TargetData _get_module_data_layout(Module m)
{
   return LLVMGetModuleDataLayout(m);
}

// ----------------------------------------------------------------------------
// Higher-level helpers
// ----------------------------------------------------------------------------

static inline Value get_store_ptr(Token *token)
{
   if (!token->is_ref) return token->llvm.elem;
   TypeRef type     = get_llvm_type(token);
   TypeRef ptr_type = LLVMPointerType(type, 0);
   return LLVMBuildLoad2(builder, ptr_type, token->llvm.elem, "store_ptr");
}

static inline Value struct_field_ptr(Token *struct_tok, int field_index, char *name)
{
   TypeRef struct_type = get_llvm_type(struct_tok);
   Value   indices[]   =
   {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, field_index, 0),
   };
   return _build_gep2(struct_type, struct_tok->llvm.elem, indices, 2, name);
}

static inline Value load_value(Token *token)
{
   if (token->llvm.is_loaded) return token->llvm.elem;
   // Already-computed values: function returns, literals, allocated buffers
   if (includes(token->type, MATH_TYPE, FCALL, STACK, HEAP, 0))
      return token->llvm.elem;
   if (!token->name && !includes(token->type, DOT, ACCESS, 0))
      return token->llvm.elem;

   // Scalar ref: double-deref (alloca-of-ptr → ptr → value)
   if (token->is_ref && token->type != STRUCT_CALL)
   {
      TypeRef type = get_llvm_type(token);
      Value   ptr  = LLVMBuildLoad2(builder, _pointer_type(type, 0), token->llvm.elem, "ref_ptr");
      return LLVMBuildLoad2(builder, type, ptr, "ref_val");
   }

   char *name = token->name;
   if (token->type == DOT)    name = to_string(DOT);
   if (token->type == ACCESS) name = to_string(ACCESS);
   return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, name ? name : "");
}

static inline void _entry(Token *token)
{
   Block entry = _append_basic_block_in_context(token->llvm.elem, "entry");
   _position_at(entry);
}

static inline Value _get_param_with_name(Token *fn, int index, char *name)
{
   Value param = LLVMGetParam(fn->llvm.elem, index);
   LLVMSetValueName(param, name);
   return param;
}
