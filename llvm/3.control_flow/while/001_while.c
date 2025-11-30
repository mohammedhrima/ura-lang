#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("while_loop", context);
   LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
   LLVMTypeRef int32 = LLVMInt32TypeInContext(context);

   // Create main function
   LLVMTypeRef main_type = LLVMFunctionType(int32, NULL, 0, 0);
   LLVMValueRef main_func = LLVMAddFunction(module, "main", main_type);
   LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, main_func, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // int i = 0;
   LLVMValueRef i = LLVMBuildAlloca(builder, int32, "i");
   LLVMBuildStore(builder, LLVMConstInt(int32, 0, 0), i);

   // APPEND BLOC
   LLVMBasicBlockRef loop_cond = LLVMAppendBasicBlockInContext(context, main_func, "loop_cond");
   LLVMBasicBlockRef loop_body = LLVMAppendBasicBlockInContext(context, main_func, "loop_body");
   LLVMBasicBlockRef end = LLVMAppendBasicBlockInContext(context, main_func, "end");

   // BUILD BR to loop_cond
   LLVMBuildBr(builder, loop_cond);

   // SET POSITION loop_cond
   LLVMPositionBuilderAtEnd(builder, loop_cond);
   // CONDITION: i < 5
   LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSLT, LLVMBuildLoad2(builder, int32, i, "i_val"), LLVMConstInt(int32, 5, 0), "cmp");
   
   // BUILD CONDITION
   // cond ? go to loop_body : go to end
   LLVMBuildCondBr(builder, cond, loop_body, end);

   // SET POSITION loop_body
   LLVMPositionBuilderAtEnd(builder, loop_body);
   // i = i + 1
   LLVMValueRef i_loaded = LLVMBuildLoad2(builder, int32, i, "i_loaded");
   LLVMValueRef i_inc = LLVMBuildAdd(builder, i_loaded, LLVMConstInt(int32, 1, 0), "i_inc");
   LLVMBuildStore(builder, i_inc, i);
   // BUILD BR to loop_cond
   LLVMBuildBr(builder, loop_cond);

   // SET POSITION end
   LLVMPositionBuilderAtEnd(builder, end);

   /* ==================================================================== */
   // return i
   LLVMBuildRet(builder, LLVMBuildLoad2(builder, int32, i, "ret"));
   // Verify and output
   LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
   LLVMPrintModuleToFile(module, "out.ll", NULL);
   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);

   return 0;
}