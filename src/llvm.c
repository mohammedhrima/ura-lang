#include "header.h"

Value llvm_build_store(Value val, Value ptr)
{
   return LLVMBuildStore(builder, val, ptr);
}

Value llvm_build_load2(TypeRef ty, Value ptr, char *name)
{
   return LLVMBuildLoad2(builder, ty, ptr, name);
}

Value llvm_build_alloca(TypeRef ty, char *name)
{
   return LLVMBuildAlloca(builder, ty, name);
}

Value llvm_build_add(Value lhs, Value rhs, char *name)
{
   return LLVMBuildAdd(builder, lhs, rhs, name);
}

Value llvm_build_sub(Value lhs, Value rhs, char *name)
{
   return LLVMBuildSub(builder, lhs, rhs, name);
}

Value llvm_build_mul(Value lhs, Value rhs, char *name)
{
   return LLVMBuildMul(builder, lhs, rhs, name);
}

Value llvm_build_sdiv(Value lhs, Value rhs, char *name)
{
   return LLVMBuildSDiv(builder, lhs, rhs, name);
}

Value llvm_build_srem(Value lhs, Value rhs, char *name)
{
   return LLVMBuildSRem(builder, lhs, rhs, name);
}

Value llvm_build_icmp(LLVMIntPredicate op, Value lhs, Value rhs, char *name)
{
   return LLVMBuildICmp(builder, op, lhs, rhs, name);
}

Value llvm_build_and(Value lhs, Value rhs, char *name)
{
   return LLVMBuildAnd(builder, lhs, rhs, name);
}

Value llvm_build_or(Value lhs, Value rhs, char *name)
{
   return LLVMBuildOr(builder, lhs, rhs, name);
}

Value llvm_build_ret(Value val)
{
   return LLVMBuildRet(builder, val);
}

Value llvm_build_ret_void()
{
   return LLVMBuildRetVoid(builder);
}

void _branch(Block bloc)
{
   if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
      LLVMBuildBr(builder, bloc);
}

Block _append_block(char *name)
{
   Value parent = llvm_get_basic_block_parent(llvm_get_insert_block());
   return llvm_append_basic_block_in_context(parent, name);
}

void _condition(Value cond, Block then_block, Block else_block)
{
   LLVMBuildCondBr(builder, cond, then_block, else_block);
}

Value llvm_build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args, char *name)
{
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

Value llvm_build_global_string_ptr(char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value llvm_build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned indece, char *name)
{
   return LLVMBuildGEP2(builder, ty, ptr, indices, indece, name);
}

Value llvm_build_bit_cast(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

Value llvm_build_sext(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

Value llvm_build_trunc(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

Value llvm_build_int_to_ptr(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

Value llvm_build_ptr_to_int(Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

Value llvm_build_array_alloca(TypeRef ty, Value val, char *name)
{
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

Value llvm_build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args,  Block then_block, Block catch_block, char *name)
{
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

Value llvm_build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses, char *name)
{
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

Value llvm_build_extract_value(Value agg_val, unsigned index,  char *name)
{
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

Value llvm_build_va_arg(Value list, TypeRef ty, char *name)
{
   return LLVMBuildVAArg(builder, list, ty, name);
}

Value llvm_build_unreachable()
{
   return LLVMBuildUnreachable(builder);
}
TypeRef llvm_pointer_type(TypeRef element_ty, unsigned address_space)
{
   return LLVMPointerType(element_ty, address_space);
}

TypeRef llvm_function_type(TypeRef retType, TypeRef *types, int count, int is_variadic)
{
   return LLVMFunctionType(retType, types, count, is_variadic);
}

TypeRef llvm_array_type(TypeRef element_type, unsigned element_count)
{
   return LLVMArrayType(element_type, element_count);
}

Value llvm_const_int(TypeRef ref_type, unsigned long long n, int sign_extend)
{
   return LLVMConstInt(ref_type, n, sign_extend);
}

Value llvm_get_named_function(char *name)
{
   return LLVMGetNamedFunction(module, name);
}

Value llvm_add_function(char *name, TypeRef function_type)
{
   return LLVMAddFunction(module, name, function_type);
}

Value llvm_get_param(Value fn, unsigned index)
{
   return LLVMGetParam(fn, index);
}

Block llvm_append_basic_block_in_context(Value func, char *name)
{
   return LLVMAppendBasicBlockInContext(context, func, name);
}

Block llvm_get_insert_block()
{
   return LLVMGetInsertBlock(builder);
}

Value llvm_get_basic_block_parent(Block block)
{
   return LLVMGetBasicBlockParent(block);
}

Block llvm_get_entry_basic_block(Value func)
{
   return LLVMGetEntryBasicBlock(func);
}

void _position_at(Block block)
{
   LLVMPositionBuilderAtEnd(builder, block);
}

Value llvm_get_basic_block_terminator(Block block)
{
   return LLVMGetBasicBlockTerminator(block);
}

TypeRef llvm_type_of(Value val)
{
   return LLVMTypeOf(val);
}

TypeKind llvm_get_type_kind(TypeRef ty)
{
   return LLVMGetTypeKind(ty);
}

unsigned llvm_get_int_type_width(TypeRef ref_ty)
{
   return LLVMGetIntTypeWidth(ref_ty);
}

TypeRef llvm_get_type_by_name2(char *name)
{
   return LLVMGetTypeByName2(context, name);
}

TypeRef llvm_struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed)
{
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

TypeRef llvm_global_get_value_type(Value global)
{
   return LLVMGlobalGetValueType(global);
}

TypeRef llvm_get_return_type(TypeRef function_type)
{
   return LLVMGetReturnType(function_type);
}

const char *llvm_get_target(Module m)
{
   return LLVMGetTarget(m);
}

void llvm_set_personality_fn(Value func, Value pers_fn)
{
   LLVMSetPersonalityFn(func, pers_fn);
}

void llvm_add_clause(Value landing_pad, Value clause_val)
{
   LLVMAddClause(landing_pad, clause_val);
}

size_t llvm_abi_size_of_type(TargetData td, TypeRef ty)
{
   return LLVMABISizeOfType(td, ty);
}

TargetData llvm_get_module_data_layout(Module m)
{
   return LLVMGetModuleDataLayout(m);
}

Value llvm_const_null(TypeRef ty)
{
   return LLVMConstNull(ty);
}

Value llvm_build_global_string_ptr_raw(char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value llvm_build_not(Token *token)
{
   return LLVMBuildNot(builder, token->llvm.elem, to_string(NOT));
}
