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

   // Create all basic blocks
   LLVMBasicBlockRef if1_beg = LLVMAppendBasicBlockInContext(context, main_func, "if1_beg");
   LLVMBasicBlockRef elif2_beg = LLVMAppendBasicBlockInContext(context, main_func, "elif2_beg");
   LLVMBasicBlockRef elif3_beg = LLVMAppendBasicBlockInContext(context, main_func, "elif3_beg");
   LLVMBasicBlockRef elif4_beg = LLVMAppendBasicBlockInContext(context, main_func, "elif4_beg");
   LLVMBasicBlockRef elif5_beg = LLVMAppendBasicBlockInContext(context, main_func, "elif5_beg");
   LLVMBasicBlockRef else_beg = LLVMAppendBasicBlockInContext(context, main_func, "else_beg");
   LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "end");

   // if (a < 2)
   LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSLT,
                                           LLVMBuildLoad2(builder, int32, a, "a_val1"),
                                           LLVMConstInt(int32, 2, 0), "cmp1");
   LLVMBuildCondBr(builder, cond1, if1_beg, elif2_beg); // if condition false, jump to

   // startif
   //    a = 3
   // endif
   LLVMPositionBuilderAtEnd(builder, if1_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   LLVMBuildBr(builder, if_end); // if condition true, jump to

   // else if (a < 3)
   LLVMPositionBuilderAtEnd(builder, elif2_beg);
   LLVMValueRef condition2 = LLVMBuildICmp(builder, LLVMIntSLT,
                                           LLVMBuildLoad2(builder, int32, a, "a_val2"),
                                           LLVMConstInt(int32, 3, 0), "cmp2");
   LLVMBuildCondBr(builder, condition2, elif3_beg, elif4_beg);

   // startif
   //    a = 4
   // endif
   LLVMPositionBuilderAtEnd(builder, elif3_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
   LLVMBuildBr(builder, if_end); // if condition true, jump to

   // else if (a < 4)
   LLVMPositionBuilderAtEnd(builder, elif4_beg);
   LLVMValueRef condition3 = LLVMBuildICmp(builder, LLVMIntSLT,
                                           LLVMBuildLoad2(builder, int32, a, "a_val3"),
                                           LLVMConstInt(int32, 4, 0), "cmp3");
   LLVMBuildCondBr(builder, condition3, elif5_beg, else_beg);

   // startelif
   //    a = 5
   // endelif
   LLVMPositionBuilderAtEnd(builder, elif5_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 5, 0), a);
   LLVMBuildBr(builder, if_end); // if condition true, jump to

   // startelse
   //    a = 6
   // endelse
   LLVMPositionBuilderAtEnd(builder, else_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 6, 0), a);
   LLVMBuildBr(builder, if_end); 

   // set position
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