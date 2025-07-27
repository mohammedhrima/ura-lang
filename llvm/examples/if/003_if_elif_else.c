#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("if_elif_else", context);
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
   LLVMValueRef const2 = LLVMConstInt(int32, 2, 0);
   LLVMValueRef const3 = LLVMConstInt(int32, 3, 0);
   LLVMValueRef const4 = LLVMConstInt(int32, 4, 0);
   LLVMBuildStore(builder, const1, a);

   // Create all basic blocks
   LLVMBasicBlockRef if1_body = LLVMAppendBasicBlockInContext(context, main_func, "if1_body");
   LLVMBasicBlockRef elif1_check = LLVMAppendBasicBlockInContext(context, main_func, "elif1_check");
   LLVMBasicBlockRef elif1_body = LLVMAppendBasicBlockInContext(context, main_func, "elif1_body");
   LLVMBasicBlockRef elif2_check = LLVMAppendBasicBlockInContext(context, main_func, "elif2_check");
   LLVMBasicBlockRef elif2_body = LLVMAppendBasicBlockInContext(context, main_func, "elif2_body");
   LLVMBasicBlockRef else_body = LLVMAppendBasicBlockInContext(context, main_func, "else_body");
   LLVMBasicBlockRef end_block = LLVMAppendBasicBlockInContext(context, main_func, "end");

   // if (a < 2)
   LLVMValueRef a_val1 = LLVMBuildLoad2(builder, int32, a, "a_val1");
   LLVMValueRef condition1 = LLVMBuildICmp(builder, LLVMIntSLT, a_val1, const2, "cmp1");
   LLVMBuildCondBr(builder, condition1, if1_body, elif1_check);

   // if body: a = 3
   LLVMPositionBuilderAtEnd(builder, if1_body);
   LLVMBuildStore(builder, const3, a);
   LLVMBuildBr(builder, end_block);

   // else if (a < 3)
   LLVMPositionBuilderAtEnd(builder, elif1_check);
   LLVMValueRef a_val2 = LLVMBuildLoad2(builder, int32, a, "a_val2");
   LLVMValueRef condition2 = LLVMBuildICmp(builder, LLVMIntSLT, a_val2, const3, "cmp2");
   LLVMBuildCondBr(builder, condition2, elif1_body, elif2_check);

   // elif1 body: a = 4
   LLVMPositionBuilderAtEnd(builder, elif1_body);
   LLVMBuildStore(builder, const4, a);
   LLVMBuildBr(builder, end_block);

   // else if (a < 4)
   LLVMPositionBuilderAtEnd(builder, elif2_check);
   LLVMValueRef a_val3 = LLVMBuildLoad2(builder, int32, a, "a_val3");
   LLVMValueRef condition3 = LLVMBuildICmp(builder, LLVMIntSLT, a_val3, const4, "cmp3");
   LLVMBuildCondBr(builder, condition3, elif2_body, else_body);

   // elif2 body: a = 5
   LLVMPositionBuilderAtEnd(builder, elif2_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 5, 0), a);
   LLVMBuildBr(builder, end_block);

   // else body: a = 6
   LLVMPositionBuilderAtEnd(builder, else_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 6, 0), a);
   LLVMBuildBr(builder, end_block);

   // end block: return a
   LLVMPositionBuilderAtEnd(builder, end_block);
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