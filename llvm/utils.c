#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef ContextRef;
typedef LLVMModuleRef ModuleRef;
typedef LLVMBuilderRef BuilderRef;
typedef LLVMBasicBlockRef BasicBlockRef;
typedef LLVMValueRef ValueRef;

ContextRef context;
ModuleRef module;
BuilderRef builder;
TypeRef voidType, float32Type, boolType, charType,
int16Type, int32Type, int64Type, charPtrType,
int32PtrType;

typedef struct {
   char *name;
   TypeRef retType;
   TypeRef *paramTypes;
   int paramCount;
   bool isVariadic;

   TypeRef funcType;
   ValueRef elem;
} Foo;

void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   // thread safety
   voidType     = LLVMVoidTypeInContext(context);
   float32Type  = LLVMFloatTypeInContext(context);
   boolType     = LLVMInt1TypeInContext(context);
   charType     = LLVMInt8TypeInContext(context);
   int16Type    = LLVMInt16TypeInContext(context);
   int32Type    = LLVMInt32TypeInContext(context);
   int64Type    = LLVMInt64TypeInContext(context);
   charPtrType  = LLVMPointerType(charType, 0);
   int32PtrType = LLVMPointerType(int32Type, 0);
   
   // Initialize targets for code generation
   LLVMInitializeAllTargetInfos();
   LLVMInitializeAllTargets();
   LLVMInitializeAllTargetMCs();
   LLVMInitializeAllAsmParsers();
   LLVMInitializeAllAsmPrinters();
}

BasicBlockRef create_bloc(char *name, Foo *parent)
{
   return LLVMAppendBasicBlockInContext(context, parent->elem, name);
}

void branch(BasicBlockRef bloc)
{
   LLVMBuildBr(builder, bloc);
}

void open_block(BasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

void create_function(Foo *func)
{
   func->funcType = LLVMFunctionType(func->retType, func->paramTypes, func->paramCount, func->isVariadic);
   func->elem = LLVMAddFunction(module, func->name, func->funcType);
}

ValueRef call_function(Foo *func, char *name, ValueRef *args, int argsCount)
{
   return LLVMBuildCall2(builder, func->funcType, func->elem, args, argsCount, name);
}

ValueRef get_param(Foo *func, int index, char *name)
{
   ValueRef param = LLVMGetParam(func->elem, index);
   if(name) LLVMSetValueName(param, name);
   return param;
}

ValueRef create_string(char *value)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);
   return LLVMBuildGlobalStringPtr(builder, value, name);
}

ValueRef create_int(LLVMTypeRef type, int value)
{
   return LLVMConstInt(type, value, 0);
}

ValueRef create_float(float value)
{
   return LLVMConstReal(float32Type, value);
}

void create_condition(ValueRef cond, BasicBlockRef isTrue, BasicBlockRef isFalse)
{
   LLVMBuildCondBr(builder, cond, isTrue, isFalse);
}

ValueRef load_variable(LLVMTypeRef type, char *name, ValueRef source)
{
   return LLVMBuildLoad2(builder, type, source, name);
}

ValueRef allocate_variable(LLVMTypeRef type, char *name)
{
   return LLVMBuildAlloca(builder, type, name);
}

ValueRef assign(ValueRef variable, ValueRef value)
{
   LLVMBuildStore(builder, value, variable);
   return variable;
}

ValueRef operation(ValueRef left, char *op, ValueRef right)
{
   // ---- Comparison (int) ----
   if (strcmp(op, "==") == 0) return LLVMBuildICmp(builder, LLVMIntEQ, left, right, "eq");
   if (strcmp(op, "!=") == 0) return LLVMBuildICmp(builder, LLVMIntNE, left, right, "ne");
   if (strcmp(op, ">") == 0) return LLVMBuildICmp(builder, LLVMIntSGT, left, right, "gt");
   if (strcmp(op, "<") == 0) return LLVMBuildICmp(builder, LLVMIntSLT, left, right, "lt");
   if (strcmp(op, ">=") == 0) return LLVMBuildICmp(builder, LLVMIntSGE, left, right, "ge");
   if (strcmp(op, "<=") == 0) return LLVMBuildICmp(builder, LLVMIntSLE, left, right, "le");

   // ---- Arithmetic ----
   if (strcmp(op, "+") == 0) return LLVMBuildAdd(builder, left, right, "add");
   if (strcmp(op, "-") == 0) return LLVMBuildSub(builder, left, right, "sub");
   if (strcmp(op, "*") == 0) return LLVMBuildMul(builder, left, right, "mul");
   if (strcmp(op, "/") == 0) return LLVMBuildSDiv(builder, left, right, "div");
   if (strcmp(op, "%") == 0) return LLVMBuildSRem(builder, left, right, "mod");

   // ---- Bitwise ----
   if (strcmp(op, "&&") == 0) return LLVMBuildAnd(builder, left, right, "and");
   if (strcmp(op, "||") == 0) return LLVMBuildOr(builder, left, right, "or");
   if (strcmp(op, "^") == 0) return LLVMBuildXor(builder, left, right, "xor");
   if (strcmp(op, "<<") == 0) return LLVMBuildShl(builder, left, right, "shl");
   if (strcmp(op, ">>") == 0) return LLVMBuildAShr(builder, left, right, "shr"); // arithmetic shift

   // ---- Logical (boolean) ----
   if (strcmp(op, "and") == 0) return LLVMBuildAnd(builder, left, right, "and");
   if (strcmp(op, "or") == 0) return LLVMBuildOr(builder, left, right, "or");

   fprintf(stderr, "Unknown operator: %s\n", op);
   return NULL;
}

ValueRef access(ValueRef source, ValueRef index, TypeRef elementType)
{
   ValueRef indices[] = {index};
   return LLVMBuildGEP2(builder, elementType, source, indices, 1, "access");
}

ValueRef cast_to(ValueRef source, TypeRef ntype, char *name)
{
   TypeRef sourceType = LLVMTypeOf(source);
   unsigned sourceBits = LLVMGetIntTypeWidth(sourceType);
   unsigned targetBits = LLVMGetIntTypeWidth(ntype);
   
   if (sourceBits > targetBits) {
      // Truncate: large → small
      return LLVMBuildTrunc(builder, source, ntype, name);
   } else if (sourceBits < targetBits) {
      // Extend: small → large
      return LLVMBuildSExt(builder, source, ntype, name);  // or ZExt for unsigned
   } else {
      // Same size, return as-is
      return source;
   }
}

void ret(ValueRef value)
{
   if (value) LLVMBuildRet(builder, value);
   else LLVMBuildRetVoid(builder);
}

// Verify and print module
int verify_module()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
      return 0;
   }
   return 1;
}

void finalize()
{
   if (verify_module()) {
      LLVMPrintModuleToFile(module, "out.ll", NULL);
      printf("LLVM IR written to out.ll\n");
   }
   
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}
