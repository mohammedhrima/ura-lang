#include "ura_header.h"

TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

// Global variable to track if bounds check function was created
static ValueRef boundsCheckFunc = NULL;

// Global map to track array sizes
typedef struct {
   ValueRef array_ptr;
   ValueRef size;
} ArraySizeInfo;

static ArraySizeInfo array_sizes[1000];
static int array_size_count = 0;

// Store array size
void store_array_size(ValueRef array_ptr, ValueRef size) {
   array_sizes[array_size_count].array_ptr = array_ptr;
   array_sizes[array_size_count].size = size;
   array_size_count++;
}

// Get array size
ValueRef get_array_size(ValueRef array_ptr) {
   for (int i = 0; i < array_size_count; i++) {
      if (array_sizes[i].array_ptr == array_ptr) {
         return array_sizes[i].size;
      }
   }
   return NULL;
}

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
}

void finalize(char *moduleName)
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
   }

   LLVMPrintModuleToFile(module, moduleName, NULL);
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}

ValueRef create_string(char *value)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);
   return LLVMBuildGlobalStringPtr(builder, value, name);
}

TypeRef get_llvm_type(Token *token)
{
   if (!token) {
      check(1, "get_llvm_type: token is NULL");
      return NULL;
   }

   TypeRef base_type;
   switch (token->retType)
   {
   case VOID: base_type = vd; break;
   case INT: base_type = i32; break;
   case FLOAT: base_type = f32; break;
   case LONG: base_type = i64; break;
   case SHORT: base_type = i16; break;
   case BOOL: base_type = i1; break;
   case CHAR: base_type = i8; break;
   case CHARS: base_type = p8; break;
   case PTR: base_type = p32; break;
   default:
   {
      todo(1, "handle this case %s", to_string(token->retType));
      seg();
      return NULL;
   }
   }

   if (!base_type) {
      check(1, "get_llvm_type: base_type is NULL for retType %d", token->retType);
      return NULL;
   }

   if (token->is_ref)
      return LLVMPointerType(base_type, 0);
   return base_type;
}

ValueRef get_value(Token *token)
{
   TypeRef llvmType = get_llvm_type(token);
   switch (token->type)
   {
   case INT: return LLVMConstInt(llvmType, token->Int.value, 0);
   case FLOAT: return LLVMConstReal(llvmType, token->Float.value);
   case BOOL: return LLVMConstInt(llvmType, token->Bool.value ? 1 : 0, 0);
   case LONG: return LLVMConstInt(llvmType, token->Long.value, 0);
   case SHORT: return LLVMConstInt(llvmType, token->Short.value, 0);
   case CHAR: return LLVMConstInt(llvmType, token->Char.value, 0);
   case CHARS: return create_string(token->Chars.value);
   default: todo(1, "handle this literal case %s", to_string(token->type));
   }
   return (ValueRef) {};
}

TypeRef get_base_type(Token *token)
{
   bool was_ref = token->is_ref;
   token->is_ref = false;
   TypeRef baseType = get_llvm_type(token);
   token->is_ref = was_ref;
   return baseType;
}

ValueRef dereference_if_ref(Token *token)
{
   if (!token->is_ref || !token->has_ref)
      return token->llvm.elem;

   TypeRef baseType = get_base_type(token);
   ValueRef addr = LLVMBuildLoad2(builder,
                                  LLVMPointerType(baseType, 0),
                                  token->llvm.elem,
                                  "ref_addr");

   return LLVMBuildLoad2(builder, baseType, addr, "deref");
}

ValueRef llvm_get_ref(Token *token)
{
   if (token->is_ref && token->has_ref)
      return dereference_if_ref(token);

   if (token->name && !token->is_param && !includes(token->type, FCALL, AND, OR, AS, STACK))
      return load_variable(token);

   return token->llvm.elem;
}

void create_function(Token *token)
{
   TypeRef *args = NULL;
   int pos = token->Fdec.pos;
   bool isVariadic = token->is_variadic;
   if (pos)
   {
      args = allocate(pos + 1, sizeof(TypeRef));
      for (int i = 0; i < pos; i++)
      {
         Token *param = token->Fdec.args[i];
         if (!param) {
            check(1, "create_function: parameter %d is NULL", i);
            continue;
         }

         TypeRef argType = get_base_type(param);
         if (param->is_ref)
            argType = LLVMPointerType(argType, 0);

         args[i] = argType;
      }
   }

   TypeRef retType = get_base_type(token);

   token->llvm.funcType = LLVMFunctionType(retType, args, pos, isVariadic);
   token->llvm.elem = LLVMAddFunction(module, token->name, token->llvm.funcType);
   free(args);
}

void call_function(Token *curr)
{
   LLVM srcFunc = curr->Fcall.func_ptr->llvm;
   int argsCount = curr->Fcall.pos;

   ValueRef *args = NULL;
   if (curr->Fcall.pos)
   {
      args = allocate(curr->Fcall.pos, sizeof(ValueRef));
      for (int i = 0; i < curr->Fcall.pos; i++)
      {
         Token *arg = curr->Fcall.args[i];
         Token *param = curr->Fcall.func_ptr->Fdec.args[i];
         check(!arg->llvm.is_set, "llvm is not set");

         if (param->is_ref)
         {
            if (arg->name && arg->llvm.is_set)
            {
               args[i] = arg->llvm.elem;
            }
            else
            {
               ValueRef val = llvm_get_ref(arg);
               TypeRef valType = LLVMTypeOf(val);
               ValueRef storage = LLVMBuildAlloca(builder, valType, "temp_ref");
               LLVMBuildStore(builder, val, storage);
               args[i] = storage;
            }
         }
         else
         {
            args[i] = llvm_get_ref(arg);
         }
      }
   }
   char *name = curr->retType != VOID ? curr->name : "";
   curr->llvm.elem = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem, args, argsCount, name);
   free(args);
}

BasicBlockRef create_bloc(char *name)
{
   return LLVMAppendBasicBlockInContext(context, get_current_func(), name);
}

void branch(BasicBlockRef bloc)
{
   LLVMBuildBr(builder, bloc);
}

void open_block(BasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

ValueRef load_variable(Token *token)
{
   TypeRef baseType = token->is_ref ? get_base_type(token) : get_llvm_type(token);
   return LLVMBuildLoad2(builder, baseType, token->llvm.elem, token->name);
}

ValueRef convert_type_if_needed(ValueRef value, TypeRef targetType)
{
   TypeRef valType = LLVMTypeOf(value);

   if (valType == targetType)
      return value;

   if (LLVMGetTypeKind(valType) == LLVMIntegerTypeKind &&
         LLVMGetTypeKind(targetType) == LLVMIntegerTypeKind)
   {
      unsigned targetBits = LLVMGetIntTypeWidth(targetType);
      unsigned valBits = LLVMGetIntTypeWidth(valType);

      if (targetBits > valBits)
         return LLVMBuildSExt(builder, value, targetType, "cast");
      else if (targetBits < valBits)
         return LLVMBuildTrunc(builder, value, targetType, "cast");
   }

   return value;
}

ValueRef assign2(Token *variable, Token *value)
{
   if (variable->is_ref)
   {
      if (!variable->has_ref)
      {
         if (value->name && value->llvm.is_set)
         {
            ValueRef addr = value->llvm.elem;
            LLVMBuildStore(builder, addr, variable->llvm.elem);
            variable->has_ref = true;
            return variable->llvm.elem;
         }
         else
         {
            ValueRef rightVal = llvm_get_ref(value);
            TypeRef valueType = LLVMTypeOf(rightVal);
            ValueRef allocatedSpace = LLVMBuildAlloca(builder, valueType, "ref_storage");
            LLVMBuildStore(builder, rightVal, allocatedSpace);
            LLVMBuildStore(builder, allocatedSpace, variable->llvm.elem);
            variable->has_ref = true;
            return variable->llvm.elem;
         }
      }
      else
      {
         TypeRef baseType = get_base_type(variable);
         ValueRef targetAddr = LLVMBuildLoad2(builder,
                                              LLVMPointerType(baseType, 0),
                                              variable->llvm.elem,
                                              "ref_addr");

         ValueRef rightVal;
         if (value->is_ref)
         {
            if (!value->has_ref)
            {
               check(1, "Cannot assign from uninitialized reference");
               return NULL;
            }
            rightVal = dereference_if_ref(value);
         }
         else
         {
            rightVal = llvm_get_ref(value);
         }

         rightVal = convert_type_if_needed(rightVal, baseType);
         LLVMBuildStore(builder, rightVal, targetAddr);
         return targetAddr;
      }
   }
   else
   {
      ValueRef rightRef = llvm_get_ref(value);
      rightRef = convert_type_if_needed(rightRef, get_llvm_type(variable));
      LLVMBuildStore(builder, rightRef, variable->llvm.elem);
      return variable->llvm.elem;
   }
}

ValueRef operation(Token *token, Token* left, Token* right)
{
   ValueRef leftRef = llvm_get_ref(left);
   ValueRef rightRef = llvm_get_ref(right);
   char* op = to_string(token->type);

   switch (token->type)
   {
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, op);
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, op);
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, op);
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, op);
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, op);
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, op);
   case ADD: return LLVMBuildAdd(builder, leftRef, rightRef, op);
   case SUB: return LLVMBuildSub(builder, leftRef, rightRef, op);
   case MUL: return LLVMBuildMul(builder, leftRef, rightRef, op);
   case DIV: return LLVMBuildSDiv(builder, leftRef, rightRef, op);
   case MOD: return LLVMBuildSRem(builder, leftRef, rightRef, op);
   case AND: return LLVMBuildAnd(builder, leftRef, rightRef, op);
   case OR: return LLVMBuildOr(builder, leftRef, rightRef, op);
   default: todo(1, "handle this %s", op);
   }
   return NULL;
}

ValueRef NotOperation(Token *token)
{
   ValueRef leftRef = llvm_get_ref(token);
   return LLVMBuildNot(builder, leftRef, "NOT");
}

ValueRef return_(ValueRef value)
{
   if (value) return LLVMBuildRet(builder, value);
   return LLVMBuildRetVoid(builder);
}

ValueRef allocate_variable(TypeRef type, char *name)
{
   return LLVMBuildAlloca(builder, type, name);
}

ValueRef get_param(Token *token)
{
   ValueRef param = LLVMGetParam(token->Param.func_ptr->llvm.elem, token->Param.index);
   LLVMSetValueName(param, token->name);

   if (token->is_ref)
   {
      TypeRef baseType = get_base_type(token);
      ValueRef ref_storage = LLVMBuildAlloca(builder, LLVMPointerType(baseType, 0), token->name);
      LLVMBuildStore(builder, param, ref_storage);
      token->has_ref = true;
      return ref_storage;
   }

   if (!token->Param.func_ptr->is_proto)
   {
      ValueRef ret = allocate_variable(get_llvm_type(token), token->name);
      LLVMBuildStore(builder, param, ret);
      return ret;
   }
   return param;
}

void build_condition(Token* curr, Token *left, Token* right)
{
   ValueRef cond = curr->Statement.ptr->llvm.elem;
   BasicBlockRef start = left->llvm.bloc;
   BasicBlockRef end = right->llvm.bloc;
   curr->llvm.elem = LLVMBuildCondBr(builder, cond, start, end);
}

ValueRef access_(Token *curr, Token *left, Token *right)
{
   ValueRef leftRef = llvm_get_ref(left);
   ValueRef rightRef;

   // For index, we always need the actual value, not a pointer
   if (right->name && right->llvm.is_set)
   {
      // Load the index value
      TypeRef rightType = right->is_ref ? get_base_type(right) : get_llvm_type(right);
      rightRef = LLVMBuildLoad2(builder, rightType, right->llvm.elem, "idx");
   }
   else
   {
      rightRef = llvm_get_ref(right);
   }

   ValueRef indices[] = { rightRef };
   TypeRef indexType = curr->is_ref ? get_base_type(curr) : get_llvm_type(curr);

   return LLVMBuildGEP2(builder, indexType, leftRef, indices, 1, to_string(curr->type));
}

ValueRef cast(Token *from, Token *to)
{
   ValueRef source = llvm_get_ref(from);
   TypeRef sourceType = LLVMTypeOf(source);
   TypeRef ntype = get_base_type(to);

   unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
   unsigned targetBits = LLVMGetIntTypeWidth(ntype);

   if (sourceBits > targetBits)
      return LLVMBuildTrunc(builder, source, ntype, "cast");
   else if (sourceBits < targetBits)
      return LLVMBuildSExt(builder, source, ntype, "cast");

   return source;
}

ValueRef allocate_stack(ValueRef size, TypeRef elementType, char *name)
{
   ValueRef indices[] = {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, 0, 0)
   };

   if (LLVMIsConstant(size))
   {
      unsigned long long constSize = LLVMConstIntGetZExtValue(size);
      TypeRef arrayType = LLVMArrayType(elementType, constSize);
      ValueRef array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
   }

   ValueRef array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

// Create the bounds check function once
ValueRef create_bounds_check_function()
{
   if (boundsCheckFunc) return boundsCheckFunc;

   // Get printf and exit functions
   ValueRef printfFunc = LLVMGetNamedFunction(module, "printf");
   if (!printfFunc) {
      TypeRef printfType = LLVMFunctionType(i32, (TypeRef[]) {p8}, 1, true);
      printfFunc = LLVMAddFunction(module, "printf", printfType);
   }

   ValueRef exitFunc = LLVMGetNamedFunction(module, "exit");
   if (!exitFunc) {
      TypeRef exitType = LLVMFunctionType(vd, (TypeRef[]) {i32}, 1, false);
      exitFunc = LLVMAddFunction(module, "exit", exitType);
   }

   TypeRef printfType = LLVMGlobalGetValueType(printfFunc);
   TypeRef exitType = LLVMGlobalGetValueType(exitFunc);

   // Function signature: void __bounds_check(i32 index, i32 size, i32 line, i8* filename)
   TypeRef params[] = {i32, i32, i32, p8};
   TypeRef funcType = LLVMFunctionType(vd, params, 4, false);
   boundsCheckFunc = LLVMAddFunction(module, "__bounds_check", funcType);

   BasicBlockRef entry = LLVMAppendBasicBlock(boundsCheckFunc, "entry");
   BasicBlockRef error = LLVMAppendBasicBlock(boundsCheckFunc, "error");
   BasicBlockRef ok = LLVMAppendBasicBlock(boundsCheckFunc, "ok");

   // Save current builder and create temporary one
   BuilderRef oldBuilder = builder;
   builder = LLVMCreateBuilderInContext(context);
   LLVMPositionBuilderAtEnd(builder, entry);

   // Get parameters in correct order
   ValueRef idx = LLVMGetParam(boundsCheckFunc, 0);
   ValueRef size = LLVMGetParam(boundsCheckFunc, 1);
   ValueRef line = LLVMGetParam(boundsCheckFunc, 2);
   ValueRef filename = LLVMGetParam(boundsCheckFunc, 3);

   // Check: index >= 0 && index < size
   ValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSGE, idx, LLVMConstInt(i32, 0, 0), "ge0");
   ValueRef cond2 = LLVMBuildICmp(builder, LLVMIntSLT, idx, size, "ltsize");
   ValueRef valid = LLVMBuildAnd(builder, cond1, cond2, "valid");

   LLVMBuildCondBr(builder, valid, ok, error);

   // Error block - simple message
   LLVMPositionBuilderAtEnd(builder, error);
   ValueRef fmt = LLVMBuildGlobalStringPtr(builder,
                                           "Error: index %d out of bounds (size: %d) at %s:%d\n", "err_fmt");
   LLVMBuildCall2(builder, printfType, printfFunc,
   (ValueRef[]) {fmt, idx, size, filename, line}, 5, "");
   LLVMBuildCall2(builder, exitType, exitFunc,
   (ValueRef[]) {LLVMConstInt(i32, 1, 0)}, 1, "");
   LLVMBuildUnreachable(builder);

   // OK block
   LLVMPositionBuilderAtEnd(builder, ok);
   LLVMBuildRetVoid(builder);

   // Restore builder
   LLVMDisposeBuilder(builder);
   builder = oldBuilder;

   return boundsCheckFunc;
}

// Safe access with bounds checking
ValueRef safe_access_(Token *curr, Token *left, Token *right)
{
   ValueRef leftRef = llvm_get_ref(left);
   ValueRef rightRef;

   if (right->name && right->llvm.is_set) {
      TypeRef rightType = right->is_ref ? get_base_type(right) : get_llvm_type(right);
      rightRef = LLVMBuildLoad2(builder, rightType, right->llvm.elem, "idx");
   } else {
      rightRef = llvm_get_ref(right);
   }

   // Get size from the token itself
   ValueRef size = left->Array.size;

   if (size) {
      ValueRef checkFunc = create_bounds_check_function();
      TypeRef funcType = LLVMGlobalGetValueType(checkFunc);

      ValueRef filename_str = LLVMBuildGlobalStringPtr(builder,
                              curr->filename ? curr->filename : "unknown", "filename");

      LLVMBuildCall2(builder, funcType, checkFunc,
      (ValueRef[]) {
         rightRef,
         size,
         LLVMConstInt(i32, curr->line, 0),
         filename_str
      }, 4, "");
   }

   ValueRef indices[] = { rightRef };
   TypeRef indexType = curr->is_ref ? get_base_type(curr) : get_llvm_type(curr);

   return LLVMBuildGEP2(builder, indexType, leftRef, indices, 1, "access");
}