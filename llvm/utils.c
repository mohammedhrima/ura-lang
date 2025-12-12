#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <stdlib.h>
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

ValueRef boundsCheckFunc = NULL;
ValueRef nullCheckFunc = NULL;

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

   voidType     = LLVMVoidTypeInContext(context);
   float32Type  = LLVMFloatTypeInContext(context);
   boolType     = LLVMInt1TypeInContext(context);
   charType     = LLVMInt8TypeInContext(context);
   int16Type    = LLVMInt16TypeInContext(context);
   int32Type    = LLVMInt32TypeInContext(context);
   int64Type    = LLVMInt64TypeInContext(context);
   charPtrType  = LLVMPointerType(charType, 0);
   int32PtrType = LLVMPointerType(int32Type, 0);
   
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

ValueRef create_int(TypeRef type, long long value)
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

ValueRef load_variable(TypeRef type, char *name, ValueRef source)
{
   return LLVMBuildLoad2(builder, type, source, name);
}

ValueRef allocate_variable(TypeRef type, char *name)
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
   if (strcmp(op, "==") == 0) return LLVMBuildICmp(builder, LLVMIntEQ, left, right, "eq");
   if (strcmp(op, "!=") == 0) return LLVMBuildICmp(builder, LLVMIntNE, left, right, "ne");
   if (strcmp(op, ">") == 0) return LLVMBuildICmp(builder, LLVMIntSGT, left, right, "gt");
   if (strcmp(op, "<") == 0) return LLVMBuildICmp(builder, LLVMIntSLT, left, right, "lt");
   if (strcmp(op, ">=") == 0) return LLVMBuildICmp(builder, LLVMIntSGE, left, right, "ge");
   if (strcmp(op, "<=") == 0) return LLVMBuildICmp(builder, LLVMIntSLE, left, right, "le");
   if (strcmp(op, "+") == 0) return LLVMBuildAdd(builder, left, right, "add");
   if (strcmp(op, "-") == 0) return LLVMBuildSub(builder, left, right, "sub");
   if (strcmp(op, "*") == 0) return LLVMBuildMul(builder, left, right, "mul");
   if (strcmp(op, "/") == 0) return LLVMBuildSDiv(builder, left, right, "div");
   if (strcmp(op, "%") == 0) return LLVMBuildSRem(builder, left, right, "mod");
   if (strcmp(op, "&&") == 0) return LLVMBuildAnd(builder, left, right, "and");
   if (strcmp(op, "||") == 0) return LLVMBuildOr(builder, left, right, "or");
   if (strcmp(op, "^") == 0) return LLVMBuildXor(builder, left, right, "xor");
   if (strcmp(op, "<<") == 0) return LLVMBuildShl(builder, left, right, "shl");
   if (strcmp(op, ">>") == 0) return LLVMBuildAShr(builder, left, right, "shr");
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
      return LLVMBuildTrunc(builder, source, ntype, name);
   } else if (sourceBits < targetBits) {
      return LLVMBuildSExt(builder, source, ntype, name);
   } else {
      return source;
   }
}

ValueRef allocate_stack(ValueRef size, TypeRef elementType, char *name)
{
   if (LLVMIsConstant(size))
   {
      unsigned long long constSize = LLVMConstIntGetZExtValue(size);
      TypeRef arrayType = LLVMArrayType(elementType, constSize);
      ValueRef array_alloca = LLVMBuildAlloca(builder, arrayType, name);
      
      ValueRef indices[] = {
         LLVMConstInt(int32Type, 0, 0),
         LLVMConstInt(int32Type, 0, 0)
      };
      return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
   }
   ValueRef array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
   return array_alloca;
}

void ret(ValueRef value)
{
   if (value) LLVMBuildRet(builder, value);
   else LLVMBuildRetVoid(builder);
}

ValueRef get_printf()
{
   ValueRef printfFunc = LLVMGetNamedFunction(module, "printf");
   if (!printfFunc) {
      TypeRef printfType = LLVMFunctionType(int32Type, (TypeRef[]){charPtrType}, 1, true);
      printfFunc = LLVMAddFunction(module, "printf", printfType);
   }
   return printfFunc;
}

ValueRef get_exit()
{
   ValueRef exitFunc = LLVMGetNamedFunction(module, "exit");
   if (!exitFunc) {
      TypeRef exitType = LLVMFunctionType(voidType, (TypeRef[]){int32Type}, 1, false);
      exitFunc = LLVMAddFunction(module, "exit", exitType);
   }
   return exitFunc;
}

ValueRef create_bounds_check_function()
{
   if (boundsCheckFunc) return boundsCheckFunc;
   
   ValueRef printfFunc = get_printf();
   ValueRef exitFunc = get_exit();
   
   TypeRef printfType = LLVMGlobalGetValueType(printfFunc);
   TypeRef exitType = LLVMGlobalGetValueType(exitFunc);
   
   TypeRef params[] = {charPtrType, int32Type, int32Type, int32Type, int32Type};
   TypeRef funcType = LLVMFunctionType(voidType, params, 5, false);
   boundsCheckFunc = LLVMAddFunction(module, "__ura_bounds_check", funcType);
   
   BasicBlockRef entry = LLVMAppendBasicBlock(boundsCheckFunc, "entry");
   BasicBlockRef error = LLVMAppendBasicBlock(boundsCheckFunc, "error");
   BasicBlockRef ok = LLVMAppendBasicBlock(boundsCheckFunc, "ok");
   
   BuilderRef oldBuilder = builder;
   builder = LLVMCreateBuilderInContext(context);
   LLVMPositionBuilderAtEnd(builder, entry);
   
   ValueRef idx = LLVMGetParam(boundsCheckFunc, 1);
   ValueRef size = LLVMGetParam(boundsCheckFunc, 2);
   ValueRef line = LLVMGetParam(boundsCheckFunc, 3);
   ValueRef col = LLVMGetParam(boundsCheckFunc, 4);
   
   ValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSGE, idx, create_int(int32Type, 0), "ge0");
   ValueRef cond2 = LLVMBuildICmp(builder, LLVMIntSLT, idx, size, "ltsize");
   ValueRef valid = LLVMBuildAnd(builder, cond1, cond2, "valid");
   
   LLVMBuildCondBr(builder, valid, ok, error);
   
   LLVMPositionBuilderAtEnd(builder, error);
   ValueRef fmt = create_string(
      "\n\033[1;31m=================================================================\033[0m\n"
      "\033[1;31mRUNTIME ERROR: Array Index Out of Bounds\033[0m\n"
      "\033[1;31m=================================================================\033[0m\n"
      "Location: \033[1;33mline %d, column %d\033[0m\n"
      "Index:    \033[1;36m%d\033[0m\n"
      "Max size: \033[1;36m%d\033[0m\n"
      "\033[1;31m=================================================================\033[0m\n\n"
   );
   LLVMBuildCall2(builder, printfType, printfFunc, 
                  (ValueRef[]){fmt, line, col, idx, size}, 5, "");
   LLVMBuildCall2(builder, exitType, exitFunc, (ValueRef[]){create_int(int32Type, 1)}, 1, "");
   LLVMBuildUnreachable(builder);
   
   LLVMPositionBuilderAtEnd(builder, ok);
   LLVMBuildRetVoid(builder);
   
   LLVMDisposeBuilder(builder);
   builder = oldBuilder;
   
   return boundsCheckFunc;
}

ValueRef safe_access(ValueRef source, ValueRef index, TypeRef elementType, 
                     int arraySize, int line, int col)
{
   ValueRef checkFunc = create_bounds_check_function();
   
   TypeRef funcType = LLVMGlobalGetValueType(checkFunc);
   LLVMBuildCall2(builder, funcType, checkFunc,
                  (ValueRef[]){
                     source,
                     index,
                     create_int(int32Type, arraySize),
                     create_int(int32Type, line),
                     create_int(int32Type, col)
                  }, 5, "");
   
   return access(source, index, elementType);
}

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