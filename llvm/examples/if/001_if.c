#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("simple_if", context);
   LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
   LLVMTypeRef int32 = LLVMInt32TypeInContext(context);

   // Create main function
   LLVMTypeRef main_type = LLVMFunctionType(int32, NULL, 0, 0);
   LLVMValueRef main_func = LLVMAddFunction(module, "main", main_type);
   LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, main_func, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // int a = 1;
   LLVMValueRef a = LLVMBuildAlloca(builder, int32, "a");
   LLVMBuildStore(builder, LLVMConstInt(int32, 1, 0), a);

   // if (a < 10)
   LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSLT,
                                     LLVMBuildLoad2(builder, int32, a, "load_a"),
                                     LLVMConstInt(int32, 10, 0), "cmp");

   // Create basic blocks for if and after
   LLVMBasicBlockRef if_beg = LLVMAppendBasicBlockInContext(context, main_func, "start_if");
   LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "end_if");

   // set start and end
   LLVMBuildCondBr(builder, cond, if_beg, if_end);

   // startif
   // if body: a = 3
   // endif
   LLVMPositionBuilderAtEnd(builder, if_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   LLVMBuildBr(builder, if_end);

   // set postion
   LLVMPositionBuilderAtEnd(builder, if_end);

   /* ==================================================================== */
   // return a
   LLVMBuildRet(builder, LLVMBuildLoad2(builder, int32, a, "ret"));
   // Verify and output
   LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
   LLVMPrintModuleToFile(module, "out.ir", NULL);
   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
   return 0;
}