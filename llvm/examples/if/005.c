#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

int main()
{
   // Initialize LLVM
   LLVMContextRef context = LLVMContextCreate();
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("sequential_pattern", context);
   LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
   LLVMTypeRef int32 = LLVMInt32TypeInContext(context);

   // Create main function
   LLVMTypeRef main_type = LLVMFunctionType(int32, NULL, 0, 0);
   LLVMValueRef main_func = LLVMAddFunction(module, "main", main_type);
   LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, main_func, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // Variables
   LLVMValueRef a = LLVMBuildAlloca(builder, int32, "a");
   LLVMBuildStore(builder, LLVMConstInt(int32, 1, 0), a);

   // ===== SEQUENTIAL PATTERN =====
   LLVMBasicBlockRef endif = LLVMAppendBasicBlockInContext(context, main_func, "endif");

   // CREATE FIRST CONDITION
   LLVMValueRef load_a = LLVMBuildLoad2(builder, int32, a, "load_a");
   LLVMValueRef if1_cond = LLVMBuildICmp(builder, LLVMIntSLT, load_a, LLVMConstInt(int32, 2, 0), "cmp1");

   // CREATE IF BODY BLOCK
   LLVMBasicBlockRef if1_body = LLVMAppendBasicBlockInContext(context, main_func, "if1_body");
   // CREATE NEXT BLOCK (elif check)
   LLVMBasicBlockRef elif1_check = LLVMAppendBasicBlockInContext(context, main_func, "elif1_check");

   // BRANCH: if true go to if_body, if false go to next
   LLVMBuildCondBr(builder, if1_cond, if1_body, elif1_check);

   // FILL IF BODY OF FIRST CONDITION
   LLVMPositionBuilderAtEnd(builder, if1_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 3, 0), a);
   LLVMBuildBr(builder, endif); // if body jumps to endif

   // MOVE TO ELIF CHECK
   LLVMPositionBuilderAtEnd(builder, elif1_check);

   // CREATE SECOND CONDITION
   LLVMValueRef a_val2 = LLVMBuildLoad2(builder, int32, a, "a_val2");
   LLVMValueRef if2_cond = LLVMBuildICmp(builder, LLVMIntSLT, a_val2, LLVMConstInt(int32, 3, 0), "cmp2");

   // CREATE ELIF BODY
   LLVMBasicBlockRef elif1_body = LLVMAppendBasicBlockInContext(context, main_func, "elif1_body");

   // CREATE NEXT ELIF CHECK
   LLVMBasicBlockRef elif2_check = LLVMAppendBasicBlockInContext(context, main_func, "elif2_check");

   // BRANCH ELIF
   LLVMBuildCondBr(builder, if2_cond, elif1_body, elif2_check);

   // FILL ELIF BODY
   LLVMPositionBuilderAtEnd(builder, elif1_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 4, 0), a);
   LLVMBuildBr(builder, endif); // elif body also jumps to endif

   // MOVE TO SECOND ELIF CHECK
   LLVMPositionBuilderAtEnd(builder, elif2_check);
   LLVMValueRef a_val3 = LLVMBuildLoad2(builder, int32, a, "a_val3");
   LLVMValueRef condition3 = LLVMBuildICmp(builder, LLVMIntSLT, a_val3, LLVMConstInt(int32, 4, 0), "cmp3");

   // CREATE SECOND ELIF BODY
   LLVMBasicBlockRef elif2_body = LLVMAppendBasicBlockInContext(context, main_func, "elif2_body");

   // CREATE ELSE BLOCK
   LLVMBasicBlockRef else_body = LLVMAppendBasicBlockInContext(context, main_func, "else_body");

   // BRANCH SECOND ELIF
   LLVMBuildCondBr(builder, condition3, elif2_body, else_body);

   // FILL SECOND ELIF BODY
   LLVMPositionBuilderAtEnd(builder, elif2_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 5, 0), a);
   LLVMBuildBr(builder, endif);

   // FILL ELSE BODY
   LLVMPositionBuilderAtEnd(builder, else_body);
   LLVMBuildStore(builder, LLVMConstInt(int32, 6, 0), a);
   LLVMBuildBr(builder, endif);

   // MERGE POINT
   LLVMPositionBuilderAtEnd(builder, endif);

   // RETURN
   LLVMBuildRet(builder, LLVMBuildLoad2(builder, int32, a, "ret"));

   // Verify and output
   LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
   LLVMPrintModuleToFile(module, "out.ir", NULL);

   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);

   printf("Sequential pattern completed!\n");
   return 0;
}