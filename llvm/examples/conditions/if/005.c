// int a = 10;
// if a > 5: a = a + 1

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   LLVMModuleRef module = LLVMModuleCreateWithName("simple_if");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMContextRef context = LLVMGetGlobalContext();

   // Define function: int main()
   LLVMTypeRef retType = LLVMInt32Type();
   LLVMTypeRef funcType = LLVMFunctionType(retType, NULL, 0, 0);
   LLVMValueRef func = LLVMAddFunction(module, "main", funcType);
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // int a = 10
   LLVMValueRef a = LLVMBuildAlloca(builder, LLVMInt32Type(), "a");
   LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), 10, 0), a);

   // if (a > 5)
   LLVMValueRef a_val = LLVMBuildLoad2(builder, LLVMInt32Type(), a, "a_val");
   LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSGT, a_val,
                                     LLVMConstInt(LLVMInt32Type(), 5, 0), "cond");

   LLVMBasicBlockRef thenBB = LLVMAppendBasicBlock(func, "then");
   LLVMBasicBlockRef mergeBB = LLVMAppendBasicBlock(func, "merge");

   LLVMBuildCondBr(builder, cond, thenBB, mergeBB);

   // then block: a = a + 1
   LLVMPositionBuilderAtEnd(builder, thenBB);
   LLVMValueRef a_then = LLVMBuildLoad2(builder, LLVMInt32Type(), a, "a_then");
   LLVMValueRef a_inc = LLVMBuildAdd(builder, a_then,
                                     LLVMConstInt(LLVMInt32Type(), 1, 0), "a_inc");
   LLVMBuildStore(builder, a_inc, a);
   LLVMBuildBr(builder, mergeBB);

   // merge block
   LLVMPositionBuilderAtEnd(builder, mergeBB);
   LLVMValueRef ret_val = LLVMBuildLoad2(builder, LLVMInt32Type(), a, "ret_val");
   LLVMBuildRet(builder, ret_val);

   // Validate and dump IR
   if (LLVMVerifyModule(module, LLVMAbortProcessAction, NULL))
   {
      fprintf(stderr, "Error verifying module\n");
   }
   LLVMPrintModuleToFile(module, "out.ir", NULL);

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   return 0;
}
