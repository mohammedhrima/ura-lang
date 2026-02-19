#include "header.h"

Value llvm_build_store(Token *token, Value val, Value ptr)
{
   return LLVMBuildStore(builder, val, ptr);
}

Value llvm_build_load2(Token *token, TypeRef ty, Value ptr, char *name)
{
   return LLVMBuildLoad2(builder, ty, ptr, name);
}

Value llvm_build_alloca(Token *token, TypeRef ty, char *name)
{
   return LLVMBuildAlloca(builder, ty, name);
}

Value llvm_build_add(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildAdd(builder, lhs, rhs, name);
}

Value llvm_build_sub(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildSub(builder, lhs, rhs, name);
}

Value llvm_build_mul(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildMul(builder, lhs, rhs, name);
}

Value llvm_build_sdiv(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildSDiv(builder, lhs, rhs, name);
}

Value llvm_build_srem(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildSRem(builder, lhs, rhs, name);
}

Value llvm_build_icmp(Token *token, LLVMIntPredicate op, Value lhs, Value rhs,
                      char *name)
{
   return LLVMBuildICmp(builder, op, lhs, rhs, name);
}

Value llvm_build_and(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildAnd(builder, lhs, rhs, name);
}

Value llvm_build_or(Token *token, Value lhs, Value rhs, char *name)
{
   return LLVMBuildOr(builder, lhs, rhs, name);
}

Value llvm_build_ret(Token *token, Value val)
{
   return LLVMBuildRet(builder, val);
}

Value llvm_build_ret_void(Token *token)
{
   return LLVMBuildRetVoid(builder);
}

Value llvm_build_br(Token *token, Block dest)
{
   return LLVMBuildBr(builder, dest);
}

Value llvm_build_cond_br(Token *token, Value cond, Block then_block, Block else_block)
{
   return LLVMBuildCondBr(builder, cond, then_block, else_block);
}

Value llvm_build_call2(Token *token, TypeRef ty, Value fn, Value *args, unsigned num_args,
                       char *name)
{
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

Value llvm_build_global_string_ptr(Token *token, const char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value llvm_build_gep2(Token *token, TypeRef ty, Value ptr, Value *indices,
                      unsigned num_indices, char *name)
{
   return LLVMBuildGEP2(builder, ty, ptr, indices, num_indices, name);
}

Value llvm_build_bit_cast(Token *token, Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

Value llvm_build_sext(Token *token, Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

Value llvm_build_trunc(Token *token, Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

Value llvm_build_int_to_ptr(Token *token, Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

Value llvm_build_ptr_to_int(Token *token, Value val, TypeRef dest_ty, char *name)
{
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

Value llvm_build_array_alloca(Token *token, TypeRef ty, Value val, char *name)
{
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

Value llvm_build_invoke2(Token *token, TypeRef ty, Value fn, Value *args,
                         unsigned num_args,
                         Block then_block, Block catch_block, char *name)
{
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

Value llvm_build_landing_pad(Token *token, TypeRef ty, Value pers_fn, unsigned num_clauses,
                             char *name)
{
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

Value llvm_build_extract_value(Token *token, Value agg_val, unsigned index,  char *name)
{
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

Value llvm_build_va_arg(Token *token, Value list, TypeRef ty, char *name)
{
   return LLVMBuildVAArg(builder, list, ty, name);
}

Value llvm_build_unreachable(Token *token)
{
   return LLVMBuildUnreachable(builder);
}
// Type creation wrappers
TypeRef llvm_pointer_type(TypeRef element_ty, unsigned address_space)
{
   return LLVMPointerType(element_ty, address_space);
}

TypeRef llvm_function_type(TypeRef return_type, TypeRef *param_types, unsigned param_count,
                           int is_var_arg)
{
   return LLVMFunctionType(return_type, param_types, param_count, is_var_arg);
}

TypeRef llvm_array_type(TypeRef element_type, unsigned element_count)
{
   return LLVMArrayType(element_type, element_count);
}

// Constant creation wrappers
Value llvm_const_int(TypeRef ref_type, unsigned long long n, int sign_extend)
{
   return LLVMConstInt(ref_type, n, sign_extend);
}

// Function management wrappers
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

// Block management wrappers
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

void llvm_position_builder_at_end(Block block)
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

LLVMTypeKind llvm_get_type_kind(TypeRef ty)
{
   return LLVMGetTypeKind(ty);
}

unsigned llvm_get_int_type_width(TypeRef ref_ty)
{
   return LLVMGetIntTypeWidth(ref_ty);
}

// Type lookup wrappers
TypeRef llvm_get_type_by_name2(char *name)
{
   return LLVMGetTypeByName2(context, name);
}

TypeRef llvm_struct_type_in_context(TypeRef *element_types, unsigned element_count,
                                    int packed)
{
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

// Function type queries
TypeRef llvm_global_get_value_type(Value global)
{
   return LLVMGlobalGetValueType(global);
}

TypeRef llvm_get_return_type(TypeRef function_type)
{
   return LLVMGetReturnType(function_type);
}

// Module and target information
const char *llvm_get_target(LLVMModuleRef m)
{
   return LLVMGetTarget(m);
}

// Personality function
void llvm_set_personality_fn(Value func, Value pers_fn)
{
   LLVMSetPersonalityFn(func, pers_fn);
}

// Landing pad clauses
void llvm_add_clause(Value landing_pad, Value clause_val)
{
   LLVMAddClause(landing_pad, clause_val);
}

// Data layout information
size_t llvm_abi_size_of_type(LLVMTargetDataRef td, TypeRef ty)
{
   return LLVMABISizeOfType(td, ty);
}

LLVMTargetDataRef llvm_get_module_data_layout(LLVMModuleRef m)
{
   return LLVMGetModuleDataLayout(m);
}

// Const null value
Value llvm_const_null(TypeRef ty)
{
   return LLVMConstNull(ty);
}

// Global string ptr (raw builder version)
Value llvm_build_global_string_ptr_raw(const char *str, char *name)
{
   return LLVMBuildGlobalStringPtr(builder, str, name);
}