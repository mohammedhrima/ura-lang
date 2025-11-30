#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("if_else", context);
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

   // CONDITION
   LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSLT, LLVMBuildLoad2(builder, int32, a, "load_a"), LLVMConstInt(int32, 10, 0), "cmp");
   
   // APPEND BLOC
   LLVMBasicBlockRef bloc0 = LLVMAppendBasicBlockInContext(context, main_func, "if_body");
   LLVMBasicBlockRef bloc1 = LLVMAppendBasicBlockInContext(context, main_func, "else_body");
   LLVMBasicBlockRef end = LLVMAppendBasicBlockInContext(context, main_func, "after");

   // BUILD CONDITION
   // cond ? go to bloc0 : go to bloc1
   LLVMBuildCondBr(builder, cond, bloc0, bloc1);
   
   // SET POSITION bloc0
   LLVMPositionBuilderAtEnd(builder, bloc0);
   //    a = 3
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   // BUILD BR to end
   LLVMBuildBr(builder, end);

   // SET POSITION bloc1
   LLVMPositionBuilderAtEnd(builder, bloc1);
   //    a = 4
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
   // BUILD BR to end
   LLVMBuildBr(builder, end);

   // SET POSITION end
   LLVMPositionBuilderAtEnd(builder, end);

   /* ==================================================================== */
   // return a
   LLVMBuildRet(builder, LLVMBuildLoad2(builder, int32, a, "ret"));
   // Verify and output
   LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
   LLVMPrintModuleToFile(module, "out.ll", NULL);
   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
   return 0;
}