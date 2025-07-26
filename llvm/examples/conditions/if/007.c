#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

// Type definitions and core LLVM structs
typedef enum Type Type;
typedef struct LLVM LLVM;
typedef struct LLVMFunction LLVMFunction;
typedef struct LLVMEntity LLVMEntity;

LLVMTypeRef int32;

enum Type
{
   INT = 1
};

struct LLVM
{
   LLVMModuleRef module;
   LLVMBuilderRef builder;
   LLVMContextRef context;
};

struct LLVMFunction
{
   LLVMTypeRef retType;
   LLVMTypeRef setup;
   LLVMValueRef ref;
   LLVMBasicBlockRef bloc;
};

struct LLVMEntity
{
   Type type;
   char *name;
   LLVMValueRef content;
};

LLVM init(char *name)
{
   LLVM res = {};
   res.module = LLVMModuleCreateWithName(name);
   res.builder = LLVMCreateBuilder();
   res.context = LLVMGetGlobalContext();
   int32 = LLVMInt32Type();
   return res;
}

LLVMFunction new_function(LLVM *llvm, char *name, Type ret)
{
   LLVMFunction res = {};
   if (ret == INT)
      res.setup = LLVMFunctionType(int32, NULL, 0, 0);
   res.ref = LLVMAddFunction(llvm->module, name, res.setup);
   res.bloc = LLVMAppendBasicBlock(res.ref, "entry");
   LLVMPositionBuilderAtEnd(llvm->builder, res.bloc);
   return res;
}

LLVMEntity new_variable(LLVM *llvm, char *name, Type type)
{
   LLVMEntity res = {.type = type, .name = name};
   if (type == INT)
      res.content = LLVMBuildAlloca(llvm->builder, int32, name);
   return res;
}

LLVMEntity new_constant(int value)
{
   LLVMEntity res = {.type = INT, .content = LLVMConstInt(int32, value, 0)};
   return res;
}

void LLVMassign(LLVM *llvm, LLVMEntity *left, LLVMEntity *right)
{
   LLVMValueRef value = right->content;
   if (right->name)
      value = LLVMBuildLoad2(llvm->builder, int32, right->content, right->name);
   LLVMBuildStore(llvm->builder, value, left->content);
}

void LLVMreturn(LLVM *llvm, LLVMEntity *entity)
{
   LLVMValueRef value = entity->content;
   if (entity->name)
      value = LLVMBuildLoad2(llvm->builder, int32, entity->content, "ret");
   LLVMBuildRet(llvm->builder, value);
}

void LLVMsaveToFile(LLVM *llvm, char *outfile)
{
   LLVMVerifyModule(llvm->module, LLVMAbortProcessAction, NULL);
   LLVMPrintModuleToFile(llvm->module, outfile, NULL);
   LLVMDisposeBuilder(llvm->builder);
   LLVMDisposeModule(llvm->module);
}

// Control flow
void LLVMif_elif_else(LLVM *llvm, LLVMEntity cond1, LLVMEntity cond2, LLVMEntity cond3, LLVMEntity *target)
{
   LLVMValueRef c1 = LLVMBuildICmp(llvm->builder, LLVMIntSGT, LLVMBuildLoad2(llvm->builder, int32, cond1.content, "c1"), LLVMConstInt(int32, 5, 0), "cmp1");
   LLVMValueRef c2 = LLVMBuildICmp(llvm->builder, LLVMIntEQ, LLVMBuildLoad2(llvm->builder, int32, cond1.content, "c2"), LLVMConstInt(int32, 5, 0), "cmp2");

   LLVMBasicBlockRef then1 = LLVMAppendBasicBlockInContext(llvm->context, llvm->builder->InsertBlock->Parent, "then1");
   LLVMBasicBlockRef then2 = LLVMAppendBasicBlockInContext(llvm->context, llvm->builder->InsertBlock->Parent, "then2");
   LLVMBasicBlockRef elseB = LLVMAppendBasicBlockInContext(llvm->context, llvm->builder->InsertBlock->Parent, "else");
   LLVMBasicBlockRef end = LLVMAppendBasicBlockInContext(llvm->context, llvm->builder->InsertBlock->Parent, "ifend");

   LLVMBuildCondBr(llvm->builder, c1, then1, then2);

   LLVMPositionBuilderAtEnd(llvm->builder, then1);
   LLVMassign(llvm, target, &cond1);
   LLVMBuildBr(llvm->builder, end);

   LLVMPositionBuilderAtEnd(llvm->builder, then2);
   LLVMBuildCondBr(llvm->builder, c2, elseB, end);

   LLVMPositionBuilderAtEnd(llvm->builder, elseB);
   LLVMassign(llvm, target, &cond2);
   LLVMBuildBr(llvm->builder, end);

   LLVMPositionBuilderAtEnd(llvm->builder, end);
}

int main()
{
   LLVM llvm = init("module");
   LLVMFunction mainFn = new_function(&llvm, "main", INT);

   LLVMEntity a = new_variable(&llvm, "a", INT);
   LLVMEntity c1 = new_constant(1);
   LLVMEntity c2 = new_constant(2);
   LLVMEntity c3 = new_constant(3);

   LLVMassign(&llvm, &a, &c3);

   LLVMif_elif_else(&llvm, a, c1, c2, &a);

   LLVMreturn(&llvm, &a);
   LLVMsaveToFile(&llvm, "ifelifelse.ll");
   return 0;
}
