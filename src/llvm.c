#include "header.h"

Value llvm_build_store(Value val, Value ptr)
{
   // printf("llvm_build_store: ");
   // ptoken(token);
   return LLVMBuildStore(builder, val, ptr);
}

Value llvm_build_load2(TypeRef ty, Value ptr, char *name)
{
   // printf("llvm_build_load2: ");
   // ptoken(token);
   return LLVMBuildLoad2(builder, ty, ptr, name);
}

Value llvm_build_alloca(TypeRef ty, char *name)
{
   // printf("llvm_build_alloca: ");
   // ptoken(token);
   return LLVMBuildAlloca(builder, ty, name);
}

Value llvm_build_add(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_add: ");
   // ptoken(token);
   return LLVMBuildAdd(builder, lhs, rhs, name);
}

Value llvm_build_sub(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_sub: ");
   // ptoken(token);
   return LLVMBuildSub(builder, lhs, rhs, name);
}

Value llvm_build_mul(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_mul: ");
   // ptoken(token);
   return LLVMBuildMul(builder, lhs, rhs, name);
}

Value llvm_build_sdiv(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_sdiv: ");
   // ptoken(token);
   return LLVMBuildSDiv(builder, lhs, rhs, name);
}

Value llvm_build_srem(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_srem: ");
   // ptoken(token);
   return LLVMBuildSRem(builder, lhs, rhs, name);
}

Value llvm_build_icmp(LLVMIntPredicate op, Value lhs, Value rhs,
                      char *name)
{
   // printf("llvm_build_icmp: ");
   // ptoken(token);
   return LLVMBuildICmp(builder, op, lhs, rhs, name);
}

Value llvm_build_and(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_and: ");
   // ptoken(token);
   return LLVMBuildAnd(builder, lhs, rhs, name);
}

Value llvm_build_or(Value lhs, Value rhs, char *name)
{
   // printf("llvm_build_or: ");
   // ptoken(token);
   return LLVMBuildOr(builder, lhs, rhs, name);
}

Value llvm_build_ret(Value val)
{
   // printf("llvm_build_ret: ");
   // ptoken(token);
   return LLVMBuildRet(builder, val);
}

Value llvm_build_ret_void()
{
   // printf("llvm_build_ret_void: ");
   // ptoken(token);
   return LLVMBuildRetVoid(builder);
}

void _branch(Block bloc)
{
   if (!llvm_get_basic_block_terminator(llvm_get_insert_block()))
      LLVMBuildBr(builder, bloc);
}

Block _append_block(char *name)
{
   return llvm_append_basic_block_in_context(llvm_get_basic_block_parent(llvm_get_insert_block()),
          name);
}

void _condition(Value cond, Block then_block, Block else_block)
{
   LLVMBuildCondBr(builder, cond, then_block, else_block);
}

Value llvm_build_call2(TypeRef ty, Value fn, Value *args, unsigned num_args,
                       char *name)
{
   // printf("llvm_build_call2: ");
   // printf("name=%s num_args=%u\n", name, num_args);
   // ptoken(token);
   return LLVMBuildCall2(builder, ty, fn, args, num_args, name);
}

Value llvm_build_global_string_ptr(const char *str, char *name)
{
   // printf("llvm_build_global_string_ptr: ");
   // printf("name=%s str=\"%s\"\n", name, str);
   // ptoken(token);
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value llvm_build_gep2(TypeRef ty, Value ptr, Value *indices, unsigned num_indices,
                      char *name)
{
   // printf("llvm_build_gep2: ");
   // printf("name=%s num_indices=%u\n", name, num_indices);
   // ptoken(token);
   return LLVMBuildGEP2(builder, ty, ptr, indices, num_indices, name);
}

Value llvm_build_bit_cast(Value val, TypeRef dest_ty, char *name)
{
   // printf("llvm_build_bit_cast: ");
   // ptoken(token);
   return LLVMBuildBitCast(builder, val, dest_ty, name);
}

Value llvm_build_sext(Value val, TypeRef dest_ty, char *name)
{
   // printf("llvm_build_sext: ");
   // ptoken(token);
   return LLVMBuildSExt(builder, val, dest_ty, name);
}

Value llvm_build_trunc(Value val, TypeRef dest_ty, char *name)
{
   // printf("llvm_build_trunc: ");
   // ptoken(token);
   return LLVMBuildTrunc(builder, val, dest_ty, name);
}

Value llvm_build_int_to_ptr(Value val, TypeRef dest_ty, char *name)
{
   // printf("llvm_build_int_to_ptr: ");
   // ptoken(token);
   return LLVMBuildIntToPtr(builder, val, dest_ty, name);
}

Value llvm_build_ptr_to_int(Value val, TypeRef dest_ty, char *name)
{
   // printf("llvm_build_ptr_to_int: ");
   // ptoken(token);
   return LLVMBuildPtrToInt(builder, val, dest_ty, name);
}

Value llvm_build_array_alloca(TypeRef ty, Value val, char *name)
{
   // printf("llvm_build_array_alloca: ");
   // ptoken(token);
   return LLVMBuildArrayAlloca(builder, ty, val, name);
}

Value llvm_build_invoke2(TypeRef ty, Value fn, Value *args, unsigned num_args,
                         Block then_block, Block catch_block, char *name)
{
   // printf("llvm_build_invoke2: ");
   // printf("name=%s num_args=%u\n", name, num_args);
   // ptoken(token);
   return LLVMBuildInvoke2(builder, ty, fn, args, num_args, then_block, catch_block, name);
}

Value llvm_build_landing_pad(TypeRef ty, Value pers_fn, unsigned num_clauses,
                             char *name)
{
   // printf("llvm_build_landing_pad: ");
   // printf("name=%s num_clauses=%u\n", name, num_clauses);
   // ptoken(token);
   return LLVMBuildLandingPad(builder, ty, pers_fn, num_clauses, name);
}

Value llvm_build_extract_value(Value agg_val, unsigned index,  char *name)
{
   // printf("llvm_build_extract_value: ");
   // printf("name=%s index=%u\n", name, index);
   // ptoken(token);
   return LLVMBuildExtractValue(builder, agg_val, index, name);
}

Value llvm_build_va_arg(Value list, TypeRef ty, char *name)
{
   // printf("llvm_build_va_arg: ");
   // ptoken(token);
   return LLVMBuildVAArg(builder, list, ty, name);
}

Value llvm_build_unreachable()
{
   // printf("llvm_build_unreachable: ");
   // ptoken(token);
   return LLVMBuildUnreachable(builder);
}
// Type creation wrappers
TypeRef llvm_pointer_type(TypeRef element_ty, unsigned address_space)
{
   // printf("llvm_pointer_type: address_space=%u\n", address_space);
   return LLVMPointerType(element_ty, address_space);
}

TypeRef llvm_function_type(TypeRef return_type, TypeRef *param_types, unsigned param_count,
                           int is_var_arg)
{
   // printf("llvm_function_type: param_count=%u is_var_arg=%d\n", param_count, is_var_arg);
   return LLVMFunctionType(return_type, param_types, param_count, is_var_arg);
}

TypeRef llvm_array_type(TypeRef element_type, unsigned element_count)
{
   // printf("llvm_array_type: element_count=%u\n", element_count);
   return LLVMArrayType(element_type, element_count);
}

// Constant creation wrappers
Value llvm_const_int(TypeRef int_type, unsigned long long n, int sign_extend)
{
   // printf("llvm_const_int: value=%llu sign_extend=%d\n", n, sign_extend);
   return LLVMConstInt(int_type, n, sign_extend);
}

// Function management wrappers
Value llvm_get_named_function(char *name)
{
   // printf("llvm_get_named_function: name=%s\n", name);
   return LLVMGetNamedFunction(module, name);
}

Value llvm_add_function(char *name, TypeRef function_type)
{
   // printf("llvm_add_function: name=%s\n", name);
   return LLVMAddFunction(module, name, function_type);
}

Value llvm_get_param(Value fn, unsigned index)
{
   // printf("llvm_get_param: index=%u\n", index);
   return LLVMGetParam(fn, index);
}

// Block management wrappers
Block llvm_append_basic_block_in_context(Value func, char *name)
{
   // printf("llvm_append_basic_block_in_context: name=%s\n", name);
   return LLVMAppendBasicBlockInContext(context, func, name);
}

Block llvm_get_insert_block()
{
   // printf("llvm_get_insert_block\n");
   return LLVMGetInsertBlock(builder);
}

Value llvm_get_basic_block_parent(Block block)
{
   // printf("llvm_get_basic_block_parent\n");
   return LLVMGetBasicBlockParent(block);
}

Block llvm_get_entry_basic_block(Value func)
{
   // printf("llvm_get_entry_basic_block\n");
   return LLVMGetEntryBasicBlock(func);
}

void _position_at(Block block)
{
   // printf("llvm_position_builder_at_end\n");
   LLVMPositionBuilderAtEnd(builder, block);
}

Value llvm_get_basic_block_terminator(Block block)
{
   // printf("llvm_get_basic_block_terminator\n");
   return LLVMGetBasicBlockTerminator(block);
}

TypeRef llvm_type_of(Value val)
{
   // printf("llvm_type_of\n");
   return LLVMTypeOf(val);
}

LLVMTypeKind llvm_get_type_kind(TypeRef ty)
{
   // printf("llvm_get_type_kind\n");
   return LLVMGetTypeKind(ty);
}

unsigned llvm_get_int_type_width(TypeRef int_ty)
{
   // printf("llvm_get_int_type_width\n");
   return LLVMGetIntTypeWidth(int_ty);
}

// Type lookup wrappers
TypeRef llvm_get_type_by_name2(char *name)
{
   // printf("llvm_get_type_by_name2: name=%s\n", name);
   return LLVMGetTypeByName2(context, name);
}

TypeRef llvm_struct_type_in_context(TypeRef *element_types, unsigned element_count, int packed)
{
   // printf("llvm_struct_type_in_context: element_count=%u packed=%d\n", element_count, packed);
   return LLVMStructTypeInContext(context, element_types, element_count, packed);
}

// Function type queries
TypeRef llvm_global_get_value_type(Value global)
{
   // printf("llvm_global_get_value_type\n");
   return LLVMGlobalGetValueType(global);
}

TypeRef llvm_get_return_type(TypeRef function_type)
{
   // printf("llvm_get_return_type\n");
   return LLVMGetReturnType(function_type);
}

// Module and target information
const char *llvm_get_target(LLVMModuleRef m)
{
   // printf("llvm_get_target\n");
   return LLVMGetTarget(m);
}

// Personality function
void llvm_set_personality_fn(Value func, Value pers_fn)
{
   // printf("llvm_set_personality_fn\n");
   LLVMSetPersonalityFn(func, pers_fn);
}

// Landing pad clauses
void llvm_add_clause(Value landing_pad, Value clause_val)
{
   // printf("llvm_add_clause\n");
   LLVMAddClause(landing_pad, clause_val);
}

// Data layout information
size_t llvm_abi_size_of_type(LLVMTargetDataRef td, TypeRef ty)
{
   // printf("llvm_abi_size_of_type\n");
   return LLVMABISizeOfType(td, ty);
}

LLVMTargetDataRef llvm_get_module_data_layout(LLVMModuleRef m)
{
   // printf("llvm_get_module_data_layout\n");
   return LLVMGetModuleDataLayout(m);
}

// Const null value
Value llvm_const_null(TypeRef ty)
{
   // printf("llvm_const_null\n");
   return LLVMConstNull(ty);
}

// Global string ptr (raw builder version)
Value llvm_build_global_string_ptr_raw(const char *str, char *name)
{
   // printf("llvm_build_global_string_ptr_raw: name=%s str=\"%s\"\n", name, str);
   return LLVMBuildGlobalStringPtr(builder, str, name);
}

Value llvm_build_not(Token *token)
{
   return LLVMBuildNot(builder, token->llvm.elem, "NOT");
}