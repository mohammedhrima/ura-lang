#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int add_multiply(int a, int b, int c)
{
   int sum = a + b;
   int result = sum * c;
   return result;
}

int main()
{
   int x = add_multiply(5, 3, 2);
   return x;
}
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("multi_args_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef int32Type = LLVMInt32Type();

   // add_multiply(int a, int b, int c)
   LLVMTypeRef addMultiplyParamTypes[] = {int32Type, int32Type, int32Type};
   LLVMTypeRef addMultiplyType = LLVMFunctionType(int32Type, addMultiplyParamTypes, 3, 0);
   LLVMValueRef addMultiplyFunc = LLVMAddFunction(mod, "add_multiply", addMultiplyType);

   LLVMBasicBlockRef addMultiplyEntry = LLVMAppendBasicBlock(addMultiplyFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, addMultiplyEntry);

   // Get parameters
   LLVMValueRef paramA = LLVMGetParam(addMultiplyFunc, 0);
   LLVMValueRef paramB = LLVMGetParam(addMultiplyFunc, 1);
   LLVMValueRef paramC = LLVMGetParam(addMultiplyFunc, 2);

   // Create local variables
   LLVMValueRef aAlloca = LLVMBuildAlloca(builder, int32Type, "a");
   LLVMValueRef bAlloca = LLVMBuildAlloca(builder, int32Type, "b");
   LLVMValueRef cAlloca = LLVMBuildAlloca(builder, int32Type, "c");
   LLVMValueRef sumAlloca = LLVMBuildAlloca(builder, int32Type, "sum");
   LLVMValueRef resultAlloca = LLVMBuildAlloca(builder, int32Type, "result");

   // Store parameters to local variables
   LLVMBuildStore(builder, paramA, aAlloca);
   LLVMBuildStore(builder, paramB, bAlloca);
   LLVMBuildStore(builder, paramC, cAlloca);

   // Load values and perform operations
   LLVMValueRef aVal = LLVMBuildLoad2(builder, int32Type, aAlloca, "a_val");
   LLVMValueRef bVal = LLVMBuildLoad2(builder, int32Type, bAlloca, "b_val");
   LLVMValueRef cVal = LLVMBuildLoad2(builder, int32Type, cAlloca, "c_val");

   // int sum = a + b;
   LLVMValueRef sum = LLVMBuildAdd(builder, aVal, bVal, "sum_temp");
   LLVMBuildStore(builder, sum, sumAlloca);

   // int result = sum * c;
   LLVMValueRef sumVal = LLVMBuildLoad2(builder, int32Type, sumAlloca, "sum_val");
   LLVMValueRef result = LLVMBuildMul(builder, sumVal, cVal, "result_temp");
   LLVMBuildStore(builder, result, resultAlloca);

   // return result;
   LLVMValueRef returnVal = LLVMBuildLoad2(builder, int32Type, resultAlloca, "return_val");
   LLVMBuildRet(builder, returnVal);

   // main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // Create local variable x
   LLVMValueRef xAlloca = LLVMBuildAlloca(builder, int32Type, "x");

   // Call add_multiply(5, 3, 2)
   LLVMValueRef args[] = {
       LLVMConstInt(int32Type, 5, 0),
       LLVMConstInt(int32Type, 3, 0),
       LLVMConstInt(int32Type, 2, 0),
   };
   LLVMValueRef callResult = LLVMBuildCall2(builder, addMultiplyType, 
      addMultiplyFunc, args, 3, "call_result");

   // int x = add_multiply(5, 3, 2);
   LLVMBuildStore(builder, callResult, xAlloca);

   // return x;
   LLVMValueRef xVal = LLVMBuildLoad2(builder, int32Type, xAlloca, "x_val");
   LLVMBuildRet(builder, xVal);

   // Output the IR
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}