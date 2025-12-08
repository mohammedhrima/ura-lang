#include "ura_header.h"

TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;
void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   // thread safety
   vd = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1 = LLVMInt1TypeInContext(context);
   i8 = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8 = LLVMPointerType(i8, 0);
   p32 = LLVMPointerType(i32, 0);

   // Initialize targets for code generation
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
   switch (token->retType)
   {
   case VOID: return vd;
   case INT: return i32;
   case FLOAT: return f32;
   case LONG: return i64;
   case SHORT: return i16;
   case BOOL: return i1;
   case CHAR: return i8;
   case CHARS: return p8;
   case PTR: return p32;
   default:
   {
      todo(1, "handle this case %s", to_string(token->retType));
      seg();
      break;
   }
   }
   return NULL;
}

LLVMValueRef get_value(Token *token)
{
   LLVMTypeRef llvmType = get_llvm_type(token);
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
   return (LLVMValueRef) {};
}

ValueRef llvm_get_ref(Token *token)
{
   debug(RED"get %k, ", token);
   if (token->name && !token->is_param && !includes(token->type, FCALL, AND, OR, AS, STACK))
   {
      debug("load elem\n");
      return load_variable(token);
   }
   debug("current elem\n");
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
         args[i] = get_llvm_type(token->Fdec.args[i]);
   }
   token->llvm.funcType = LLVMFunctionType(get_llvm_type(token), args, pos, isVariadic);
   token->llvm.elem = LLVMAddFunction(module, token->name, token->llvm.funcType);
   free(args);
}

void call_function(Token *token, LLVM *source, ValueRef *args, int argsCount)
{
   char *name = token->retType != VOID ? token->name : "";
   token->llvm.elem = LLVMBuildCall2(builder, source->funcType, source->elem, args, argsCount, name);
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
   debug("load %k\n", token);
   return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, token->name);
}

ValueRef assign2(Token *variable, Token* value)
{
   ValueRef rightRef = llvm_get_ref(value);

   // Check if we need implicit type conversion
   TypeRef varType = get_llvm_type(variable);
   TypeRef valType = LLVMTypeOf(rightRef);

   // Only convert if both are integer types (not pointers)
   if (LLVMGetTypeKind(valType) != LLVMPointerTypeKind &&
         LLVMGetTypeKind(varType) != LLVMPointerTypeKind)
   {
      unsigned varBits = LLVMGetIntTypeWidth(varType);
      unsigned valBits = LLVMGetIntTypeWidth(valType);

      if (varBits > valBits) {
         // Extend: i8 → i32
         rightRef = LLVMBuildSExt(builder, rightRef, varType, "implicit_cast");
      } else if (varBits < valBits) {
         // Truncate: i32 → i8
         rightRef = LLVMBuildTrunc(builder, rightRef, varType, "implicit_cast");
      }
   }

   LLVMBuildStore(builder, rightRef, variable->llvm.elem);
   return variable->llvm.elem;
}

ValueRef operation(Token *token, Token* left, Token* right)
{
   ValueRef leftRef = llvm_get_ref(left);
   ValueRef rightRef = llvm_get_ref(right);
   char* op = to_string(token->type);
   switch (token->type)
   {
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, op); break;
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, op); break;
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, op); break;
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, op); break;
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, op); break;
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, op); break;
   case ADD: return LLVMBuildAdd(builder, leftRef, rightRef, op); break;
   case SUB: return LLVMBuildSub(builder, leftRef, rightRef, op); break;
   case MUL: return LLVMBuildMul(builder, leftRef, rightRef, op); break;
   case DIV: return LLVMBuildSDiv(builder, leftRef, rightRef, op); break;
   case MOD: return LLVMBuildSRem(builder, leftRef, rightRef, op); break;
   case AND: return LLVMBuildAnd(builder, leftRef, rightRef, op); break;
   case OR: return LLVMBuildOr(builder, leftRef, rightRef, op); break;
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

ValueRef allocate_variable(LLVMTypeRef type, char *name)
{
   return LLVMBuildAlloca(builder, type, name);
}

ValueRef get_param(Token *token)
{
   ValueRef param = LLVMGetParam(token->Param.func_ptr->llvm.elem, token->Param.index);
   LLVMSetValueName(param, token->name);
   // TODO: check if the param got modified
   // else: don't need to allocate stack for it
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
   BasicBlocRef start = left->llvm.bloc;
   BasicBlocRef end = right->llvm.bloc;
   curr->llvm.elem = LLVMBuildCondBr(builder, cond, start, end);
}

ValueRef access_(Token *curr, Token *left, Token *right)
{
   ValueRef leftRef = NULL, rightRef = NULL;
   leftRef = llvm_get_ref(left);
   rightRef = llvm_get_ref(right);

   ValueRef indices[] = { rightRef };
   return LLVMBuildGEP2(builder, get_llvm_type(curr), leftRef, indices, 1, to_string(curr->type));
}

ValueRef cast(Token *from, Token *to)
{
   ValueRef source = llvm_get_ref(from); //.elem;
   TypeRef sourceType = LLVMTypeOf(source);
   TypeRef ntype = get_llvm_type(to);

   // Check if source is a pointer - if so, LOAD it first
   // if (LLVMGetTypeKind(sourceType) == LLVMPointerTypeKind) {
   //    source = load_variable(from);
   //    sourceType = get_llvm_type(from);
   // }

   unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
   unsigned targetBits = LLVMGetIntTypeWidth(ntype);

   if (sourceBits > targetBits) {
      return LLVMBuildTrunc(builder, source, ntype, "cast");
   } else if (sourceBits < targetBits) {
      return LLVMBuildSExt(builder, source, ntype, "cast");
   } else {
      // print(RED"the same\n"RESET);
      return source;
   }
}

ValueRef allocate_stack(ValueRef size, TypeRef elementType, char *name)
{
   // Indices for GEP to get the pointer to the first element
   ValueRef indices[] = {
      LLVMConstInt(i32, 0, 0),
      LLVMConstInt(i32, 0, 0)
   };

   // Constant size: create an [N x elem] alloca and GEP to &array[0]
   if (LLVMIsConstant(size))
   {
      unsigned long long constSize = LLVMConstIntGetZExtValue(size);
      TypeRef arrayType = LLVMArrayType(elementType, constSize);
      ValueRef array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      // Return pointer to first element (elementType*)
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);;
   }

   // Dynamic size: use LLVMBuildArrayAlloca then GEP to &array[0]
   ValueRef array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}
