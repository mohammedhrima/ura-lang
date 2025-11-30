#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   int a = 1;
   int b = 2;
   bool c = a > 1 && b > 1;  // logical AND
   bool d = a > 1 || b > 1;  // logical OR
   return 0;
}
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("logical_ops");
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

   // int b = 2;
   LLVMValueRef b = LLVMBuildAlloca(builder, int32Type, "b");
   LLVMBuildStore(builder, LLVMConstInt(int32Type, 2, 0), b);

   // Load values for comparisons
   LLVMValueRef a_val = LLVMBuildLoad2(builder, int32Type, a, "a_val");
   LLVMValueRef b_val = LLVMBuildLoad2(builder, int32Type, b, "b_val");

   // bool c = a > 1 && b > 1;
   // First comparison: a > 1
   LLVMValueRef a_gt_1 = LLVMBuildICmp(builder, LLVMIntSGT, a_val,
                                       LLVMConstInt(int32Type, 1, 0), "a_gt_1");

   // Second comparison: b > 1
   LLVMValueRef b_gt_1 = LLVMBuildICmp(builder, LLVMIntSGT, b_val,
                                       LLVMConstInt(int32Type, 1, 0), "b_gt_1");

   // Logical AND: a_gt_1 && b_gt_1
   LLVMValueRef logical_and = LLVMBuildAnd(builder, a_gt_1, b_gt_1, "logical_and");

   // Store result in variable c
   LLVMValueRef c = LLVMBuildAlloca(builder, boolType, "c");
   LLVMBuildStore(builder, logical_and, c);

   // bool d = a > 1 || b > 1;
   // We already have a_gt_1 and b_gt_1 from above

   // Logical OR: a_gt_1 || b_gt_1
   LLVMValueRef logical_or = LLVMBuildOr(builder, a_gt_1, b_gt_1, "logical_or");

   // Store result in variable d
   LLVMValueRef d = LLVMBuildAlloca(builder, boolType, "d");
   LLVMBuildStore(builder, logical_or, d);

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