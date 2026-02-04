#include "header.h"

_Value llvm_build_store(Token *token, _Value val, _Value ptr)
{
   printf("llvm_build_store: ");
   ptoken(token);
   return LLVMBuildStore(builder, val, ptr);
}

_Value llvm_build_load2(Token *token, _Type ty, _Value ptr, char *name)
{
   printf("llvm_build_load2: ");
   ptoken(token);
   return LLVMBuildLoad2(builder, ty, ptr, name);
}

_Value llvm_build_alloca(Token *token, _Type ty, char *name)
{
   printf("llvm_build_alloca: ");
   ptoken(token);
   return LLVMBuildAlloca(builder, ty, name);
}

_Value llvm_build_add(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_add: ");
   ptoken(token);
   return LLVMBuildAdd(builder, lhs, rhs, name);
}

_Value llvm_build_sub(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_sub: ");
   ptoken(token);
   return LLVMBuildSub(builder, lhs, rhs, name);
}

_Value llvm_build_mul(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_mul: ");
   ptoken(token);
   return LLVMBuildMul(builder, lhs, rhs, name);
}

_Value llvm_build_sdiv(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_sdiv: ");
   ptoken(token);
   return LLVMBuildSDiv(builder, lhs, rhs, name);
}

_Value llvm_build_srem(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_srem: ");
   ptoken(token);
   return LLVMBuildSRem(builder, lhs, rhs, name);
}

_Value llvm_build_icmp(Token *token, LLVMIntPredicate op, _Value lhs, _Value rhs,
                       char *name)
{
   printf("llvm_build_icmp: ");
   ptoken(token);
   return LLVMBuildICmp(builder, op, lhs, rhs, name);
}

_Value llvm_build_and(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_and: ");
   ptoken(token);
   return LLVMBuildAnd(builder, lhs, rhs, name);
}

_Value llvm_build_or(Token *token, _Value lhs, _Value rhs, char *name)
{
   printf("llvm_build_or: ");
   ptoken(token);
   return LLVMBuildOr(builder, lhs, rhs, name);
}

_Value llvm_build_ret(Token *token, _Value val)
{
   printf("llvm_build_ret: ");
   ptoken(token);
   return LLVMBuildRet(builder, val);
}

_Value llvm_build_ret_void(Token *token)
{
   printf("llvm_build_ret_void: ");
   ptoken(token);
   return LLVMBuildRetVoid(builder);
}

_Value llvm_build_br(Token *token, _Block dest)
{
   printf("llvm_build_br: ");
   ptoken(token);
   return LLVMBuildBr(builder, dest);
}

_Value llvm_build_cond_br(Token *token, _Value cond, _Block then_block, _Block else_block)
{
   printf("llvm_build_cond_br: ");
   ptoken(token);
   return LLVMBuildCondBr(builder, cond, then_block, else_block);
}

_Value llvm_build_call2(Token *token, _Type ty, _Value fn, _Value *args, unsigned num_args,
                        char *name)
{
   printf("llvm_build_call2: ");
   printf("name=%s num_args=%u\n", name, num_args);
   ptoken(token);
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

_Value llvm_build_global_string_ptr(Token *token, const char *str, char *name)
{
   printf("llvm_build_global_string_ptr: ");
   printf("name=%s str=\"%s\"\n", name, str);
   ptoken(token);
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

_Value llvm_build_gep2(Token *token, _Type ty, _Value ptr, _Value *indices,
                       unsigned num_indices, char *name)
{
   printf("llvm_build_gep2: ");
   printf("name=%s num_indices=%u\n", name, num_indices);
   ptoken(token);
   return LLVMBuildGEP2(builder, ty, ptr, indices, num_indices, name);
}

_Value llvm_build_bit_cast(Token *token, _Value val, _Type dest_ty, char *name)
{
   printf("llvm_build_bit_cast: ");
   ptoken(token);
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

_Value llvm_build_sext(Token *token, _Value val, _Type dest_ty, char *name)
{
   printf("llvm_build_sext: ");
   ptoken(token);
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

_Value llvm_build_trunc(Token *token, _Value val, _Type dest_ty, char *name)
{
   printf("llvm_build_trunc: ");
   ptoken(token);
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

_Value llvm_build_int_to_ptr(Token *token, _Value val, _Type dest_ty, char *name)
{
   printf("llvm_build_int_to_ptr: ");
   ptoken(token);
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

_Value llvm_build_ptr_to_int(Token *token, _Value val, _Type dest_ty, char *name)
{
   printf("llvm_build_ptr_to_int: ");
   ptoken(token);
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

_Value llvm_build_array_alloca(Token *token, _Type ty, _Value val, char *name)
{
   printf("llvm_build_array_alloca: ");
   ptoken(token);
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

_Value llvm_build_invoke2(Token *token, _Type ty, _Value fn, _Value *args,
                          unsigned num_args,
                          _Block then_block, _Block catch_block, char *name)
{
   printf("llvm_build_invoke2: ");
   printf("name=%s num_args=%u\n", name, num_args);
   ptoken(token);
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

_Value llvm_build_landing_pad(Token *token, _Type ty, _Value pers_fn, unsigned num_clauses,
                              char *name)
{
   printf("llvm_build_landing_pad: ");
   printf("name=%s num_clauses=%u\n", name, num_clauses);
   ptoken(token);
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

_Value llvm_build_extract_value(Token *token, _Value agg_val, unsigned index,  char *name)
{
   printf("llvm_build_extract_value: ");
   printf("name=%s index=%u\n", name, index);
   ptoken(token);
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

_Value llvm_build_va_arg(Token *token, _Value list, _Type ty, char *name)
{
   printf("llvm_build_va_arg: ");
   ptoken(token);
   return LLVMBuildVAArg(builder, list, ty, name);
}

_Value llvm_build_unreachable(Token *token)
{
   printf("llvm_build_unreachable: ");
   ptoken(token);
   return LLVMBuildUnreachable(builder);
}
// Type creation wrappers
_Type llvm_pointer_type(_Type element_ty, unsigned address_space)
{
   printf("llvm_pointer_type: address_space=%u\n", address_space);
   return LLVMPointerType(element_ty, address_space);
}

_Type llvm_function_type(_Type return_type, _Type *param_types, unsigned param_count,
                         int is_var_arg)
{
   printf("llvm_function_type: param_count=%u is_var_arg=%d\n", param_count, is_var_arg);
   return LLVMFunctionType(return_type, param_types, param_count, is_var_arg);
}

_Type llvm_array_type(_Type element_type, unsigned element_count)
{
   printf("llvm_array_type: element_count=%u\n", element_count);
   return LLVMArrayType(element_type, element_count);
}

// Constant creation wrappers
_Value llvm_const_int(_Type int_type, unsigned long long n, int sign_extend)
{
   printf("llvm_const_int: value=%llu sign_extend=%d\n", n, sign_extend);
   return LLVMConstInt(int_type, n, sign_extend);
}

// Function management wrappers
_Value llvm_get_named_function(char *name)
{
   printf("llvm_get_named_function: name=%s\n", name);
   return LLVMGetNamedFunction(module, name);
}

_Value llvm_add_function(char *name, _Type function_type)
{
   printf("llvm_add_function: name=%s\n", name);
   return LLVMAddFunction(module, name, function_type);
}

_Value llvm_get_param(_Value fn, unsigned index)
{
   printf("llvm_get_param: index=%u\n", index);
   return LLVMGetParam(fn, index);
}

// Block management wrappers
_Block llvm_append_basic_block_in_context(_Value func, char *name)
{
   printf("llvm_append_basic_block_in_context: name=%s\n", name);
   return LLVMAppendBasicBlockInContext(context, func, name);
}

_Block llvm_get_insert_block()
{
   printf("llvm_get_insert_block\n");
   return LLVMGetInsertBlock(builder);
}

_Value llvm_get_basic_block_parent(_Block block)
{
   printf("llvm_get_basic_block_parent\n");
   return LLVMGetBasicBlockParent(block);
}

_Block llvm_get_entry_basic_block(_Value func)
{
   printf("llvm_get_entry_basic_block\n");
   return LLVMGetEntryBasicBlock(func);
}

void llvm_position_builder_at_end(_Block block)
{
   printf("llvm_position_builder_at_end\n");
   LLVMPositionBuilderAtEnd(builder, block);
}

_Value llvm_get_basic_block_terminator(_Block block)
{
   printf("llvm_get_basic_block_terminator\n");
   return LLVMGetBasicBlockTerminator(block);
}

_Type llvm_type_of(_Value val)
{
   printf("llvm_type_of\n");
   return LLVMTypeOf(val);
}

LLVMTypeKind llvm_get_type_kind(_Type ty)
{
   printf("llvm_get_type_kind\n");
   return LLVMGetTypeKind(ty);
}

unsigned llvm_get_int_type_width(_Type int_ty)
{
   printf("llvm_get_int_type_width\n");
   return LLVMGetIntTypeWidth(int_ty);
}

// Type lookup wrappers
_Type llvm_get_type_by_name2(char *name)
{
   printf("llvm_get_type_by_name2: name=%s\n", name);
   return LLVMGetTypeByName2(context, name);
}

_Type llvm_struct_type_in_context(_Type *element_types, unsigned element_count,
                                  int packed)
{
   printf("llvm_struct_type_in_context: element_count=%u packed=%d\n", element_count, packed);
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

// Function type queries
_Type llvm_global_get_value_type(_Value global)
{
   printf("llvm_global_get_value_type\n");
   return LLVMGlobalGetValueType(global);
}

_Type llvm_get_return_type(_Type function_type)
{
   printf("llvm_get_return_type\n");
   return LLVMGetReturnType(function_type);
}

// Module and target information
const char *llvm_get_target(LLVMModuleRef m)
{
   printf("llvm_get_target\n");
   return LLVMGetTarget(m);
}

// Personality function
void llvm_set_personality_fn(_Value func, _Value pers_fn)
{
   printf("llvm_set_personality_fn\n");
   LLVMSetPersonalityFn(func, pers_fn);
}

// Landing pad clauses
void llvm_add_clause(_Value landing_pad, _Value clause_val)
{
   printf("llvm_add_clause\n");
   LLVMAddClause(landing_pad, clause_val);
}

// Data layout information
size_t llvm_abi_size_of_type(LLVMTargetDataRef td, _Type ty)
{
   printf("llvm_abi_size_of_type\n");
   return LLVMABISizeOfType(td, ty);
}

LLVMTargetDataRef llvm_get_module_data_layout(LLVMModuleRef m)
{
   printf("llvm_get_module_data_layout\n");
   return LLVMGetModuleDataLayout(m);
}

// Const null value
_Value llvm_const_null(_Type ty)
{
   printf("llvm_const_null\n");
   return LLVMConstNull(ty);
}

// Global string ptr (raw builder version)
_Value llvm_build_global_string_ptr_raw(const char *str, char *name)
{
   printf("llvm_build_global_string_ptr_raw: name=%s str=\"%s\"\n", name, str);
   return LLVMBuildGlobalStringPtr(builder, str, name);
}