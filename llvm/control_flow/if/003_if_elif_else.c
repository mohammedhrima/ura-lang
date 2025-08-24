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
   LLVMBuildStore(builder, LLVMConstInt(int32, 1, 0), a);

   // CONDITION
   LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSLT, LLVMBuildLoad2(builder, int32, a, "a_val1"), LLVMConstInt(int32, 2, 0), "cmp1");

   // APPEND BLOC
   LLVMBasicBlockRef bloc0 = LLVMAppendBasicBlockInContext(context, main_func, "if1");
   LLVMBasicBlockRef bloc1 = LLVMAppendBasicBlockInContext(context, main_func, "elif2");
   LLVMBasicBlockRef bloc2 = LLVMAppendBasicBlockInContext(context, main_func, "elif3");
   LLVMBasicBlockRef bloc3 = LLVMAppendBasicBlockInContext(context, main_func, "elif4");
   LLVMBasicBlockRef bloc4 = LLVMAppendBasicBlockInContext(context, main_func, "elif5");
   LLVMBasicBlockRef bloc5 = LLVMAppendBasicBlockInContext(context, main_func, "else");
   LLVMBasicBlockRef end = LLVMAppendBasicBlockInContext(context, main_func, "end");

   // BUILD CONDITION
   // cond1 ? go to bloc0 : go to bloc1
   LLVMBuildCondBr(builder, cond1, bloc0, bloc1);

   // SET POSITION bloc0
   LLVMPositionBuilderAtEnd(builder, bloc0);
   //    a = 3
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   // BUILD BR to end
   LLVMBuildBr(builder, end);

   // SET POSITION bloc1
   LLVMPositionBuilderAtEnd(builder, bloc1);
   LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntSLT, LLVMBuildLoad2(builder, int32, a, "a_val2"), LLVMConstInt(int32, 3, 0), "cmp2");
   // BUILD CONDITION
   // cond2 ? go to bloc2 : go to bloc3
   LLVMBuildCondBr(builder, cond2, bloc2, bloc3);
   // SET POSITION bloc2
   LLVMPositionBuilderAtEnd(builder, bloc2);
   //    a = 4
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
   // BUILD BR to end
   LLVMBuildBr(builder, end);


   // set pos bloc3
   // cond3 = a < 4
   // cond3 ? go to bloc4 : go to bloc5
   // set pos bloc4
   //    a = 5
   // jmp to end
   LLVMPositionBuilderAtEnd(builder, bloc3);
   LLVMValueRef cond3 = LLVMBuildICmp(builder, LLVMIntSLT, LLVMBuildLoad2(builder, int32, a, "a_val3"), LLVMConstInt(int32, 4, 0), "cmp3");
   LLVMBuildCondBr(builder, cond3, bloc4, bloc5);

   LLVMPositionBuilderAtEnd(builder, bloc4);
   LLVMBuildStore(builder, LLVMConstInt(int32, 5, 0), a);
   LLVMBuildBr(builder, end);

   // set pos bloc5
   //    a = 6
   // jmp to end
   LLVMPositionBuilderAtEnd(builder, bloc5);
   LLVMBuildStore(builder, LLVMConstInt(int32, 6, 0), a);
   LLVMBuildBr(builder, end);

   // set pos end
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