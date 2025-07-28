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

   // int a = 10;
   LLVMValueRef a = LLVMBuildAlloca(builder, int32, "a");
   LLVMBuildStore(builder, LLVMConstInt(int32, 1, 0), a);

   // Create basic blocks for if, else and after
   LLVMBasicBlockRef if_beg = LLVMAppendBasicBlockInContext(context, main_func, "if_body");
   LLVMBasicBlockRef else_beg = LLVMAppendBasicBlockInContext(context, main_func, "else_body");
   LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "after");

   // if (a < 10)
   LLVMValueRef condition = LLVMBuildICmp(builder, LLVMIntSLT,
                                          LLVMBuildLoad2(builder, int32, a, "a_val"),
                                          LLVMConstInt(int32, 10, 0), "cmp");
   LLVMBuildCondBr(builder, condition, if_beg, else_beg); // if condition false, jump to

   // startif
   //    a = 3
   // endif
   LLVMPositionBuilderAtEnd(builder, if_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   LLVMBuildBr(builder, if_end); // if condition true, jump to

   // startelse
   //    a = 4
   // endelse
   LLVMPositionBuilderAtEnd(builder, else_beg);
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
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