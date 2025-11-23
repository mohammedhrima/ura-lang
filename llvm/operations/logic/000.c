#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   int a = 1;
   bool b = !(a > 1);
   return 0;
}
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("not_op");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef boolType = LLVMInt1Type(); // i1 for boolean

   // Define main function
   LLVMTypeRef mainFuncType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainFuncType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // int a = 1;
   LLVMValueRef a = LLVMBuildAlloca(builder, int32Type, "a");
   LLVMBuildStore(builder, LLVMConstInt(int32Type, 1, 0), a);

   // Load value for comparison
   LLVMValueRef a_val = LLVMBuildLoad2(builder, int32Type, a, "a_val");

   // Comparison: a > 1
   LLVMValueRef a_gt_1 = LLVMBuildICmp(builder, LLVMIntSGT, a_val,
                                       LLVMConstInt(int32Type, 1, 0), "a_gt_1");

   // Logical NOT: !(a > 1)
   LLVMValueRef not_result = LLVMBuildNot(builder, a_gt_1, "not_result");

   // bool b = !(a > 1);
   LLVMValueRef b = LLVMBuildAlloca(builder, boolType, "b");
   LLVMBuildStore(builder, not_result, b);

   // return 0;
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // -----------------------------------
   // Emit IR
   // -----------------------------------
   LLVMPrintModuleToFile(mod, "out.ll", NULL);
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}