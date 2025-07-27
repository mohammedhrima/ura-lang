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
   LLVMValueRef const1 = LLVMConstInt(int32, 1, 0);
   LLVMValueRef const10 = LLVMConstInt(int32, 10, 0);
   LLVMBuildStore(builder, const1, a);

   // if (a < 10)
   LLVMValueRef a_val = LLVMBuildLoad2(builder, int32, a, "a_val");
   LLVMValueRef condition = LLVMBuildICmp(builder, LLVMIntSLT, a_val, const10, "cmp");

   // Create basic blocks for if and after
   LLVMBasicBlockRef if_start = LLVMAppendBasicBlockInContext(context, main_func, "if_body");
   LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "after");

   // set start and end
   LLVMBuildCondBr(builder, condition, if_start, if_end);

   // put postion at the beginning of if
   LLVMPositionBuilderAtEnd(builder, if_start);
   
   // if body: a = 3
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   
   // end if
   LLVMBuildBr(builder, if_end);

   // put postion at the ending of if
   LLVMPositionBuilderAtEnd(builder, if_end);

   // after block: return a
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