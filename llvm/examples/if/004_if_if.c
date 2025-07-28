#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("independent_ifs", context);
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

   // Create basic blocks for each if statement
   LLVMBasicBlockRef if1_beg = LLVMAppendBasicBlockInContext(context, main_func, "if1_beg");
   LLVMBasicBlockRef if2_beg = LLVMAppendBasicBlockInContext(context, main_func, "if2_beg");
   LLVMBasicBlockRef if3_beg = LLVMAppendBasicBlockInContext(context, main_func, "if3_beg");
   LLVMBasicBlockRef if4_beg = LLVMAppendBasicBlockInContext(context, main_func, "if4_beg");
   LLVMBasicBlockRef if5_beg = LLVMAppendBasicBlockInContext(context, main_func, "if5_beg");
   LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "end");

   // if (a < 2)
   LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSLT,
                                      LLVMBuildLoad2(builder, int32, a, "a_val1"),
                                      LLVMConstInt(int32, 2, 0), "cmp1");
   LLVMBuildCondBr(builder, cond1, if1_beg, if2_beg); // if condition false, jump to

   // startif
   //    a = 3
   // endif
   LLVMPositionBuilderAtEnd(builder, if1_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   LLVMBuildBr(builder, if2_beg); // if condition true, jump to

   // if (a < 3)
   LLVMPositionBuilderAtEnd(builder, if2_beg);
   LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntSLT,
                                      LLVMBuildLoad2(builder, int32, a, "a_val2"),
                                      LLVMConstInt(int32, 3, 0), "cmp2");
   LLVMBuildCondBr(builder, cond2, if3_beg, if4_beg);

   // startif
   //    a = 4
   // endif
   LLVMPositionBuilderAtEnd(builder, if3_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
   LLVMBuildBr(builder, if4_beg); // next instrution

   // if (a < 4)
   LLVMPositionBuilderAtEnd(builder, if4_beg);
   LLVMValueRef cond3 = LLVMBuildICmp(builder, LLVMIntSLT,
                                      LLVMBuildLoad2(builder, int32, a, "a_val3"),
                                      LLVMConstInt(int32, 4, 0), "cmp3");
   LLVMBuildCondBr(builder, cond3, if5_beg, if_end);

   // startif
   //    a = 5
   // endif
   LLVMPositionBuilderAtEnd(builder, if5_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 5, 0), a);
   LLVMBuildBr(builder, if_end); // next instrution

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