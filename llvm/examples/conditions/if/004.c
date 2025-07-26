#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
// int a = 1;
// if (a > 2) {
//   a = a + 3;
// } else if (a == 4) {
//   a = a + 5;
// } else {
//   a = a + 6;
// }

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("if_elif_else_chain");
   LLVMContextRef ctx = LLVMGetGlobalContext();
   LLVMBuilderRef builder = LLVMCreateBuilderInContext(ctx);

   LLVMTypeRef int32Ty = LLVMInt32TypeInContext(ctx);
   LLVMTypeRef funcType = LLVMFunctionType(int32Ty, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", funcType);

   LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx, mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // Constants
   LLVMValueRef const1 = LLVMConstInt(int32Ty, 1, 0);
   LLVMValueRef const2 = LLVMConstInt(int32Ty, 2, 0);
   LLVMValueRef const3 = LLVMConstInt(int32Ty, 3, 0);
   LLVMValueRef const4 = LLVMConstInt(int32Ty, 4, 0);
   LLVMValueRef const5 = LLVMConstInt(int32Ty, 5, 0);
   LLVMValueRef const6 = LLVMConstInt(int32Ty, 6, 0);
   LLVMValueRef const0 = LLVMConstInt(int32Ty, 0, 0);

   // int a = 1;
   LLVMValueRef a = LLVMBuildAlloca(builder, int32Ty, "a");
   LLVMBuildStore(builder, const1, a);

   // Blocks
   LLVMBasicBlockRef then1 = LLVMAppendBasicBlockInContext(ctx, mainFunc, "then1");
   LLVMBasicBlockRef elifCond = LLVMAppendBasicBlockInContext(ctx, mainFunc, "elif_cond");
   LLVMBasicBlockRef then2 = LLVMAppendBasicBlockInContext(ctx, mainFunc, "then2");
   LLVMBasicBlockRef elseBlock = LLVMAppendBasicBlockInContext(ctx, mainFunc, "else");
   LLVMBasicBlockRef merge = LLVMAppendBasicBlockInContext(ctx, mainFunc, "merge");

// Macro to build a conditional branch: if(cond) then goto thenBB else goto elseBB
#define BUILD_COND_BR(cond, thenBB, elseBB) LLVMBuildCondBr(builder, cond, thenBB, elseBB)

// Macro to build add and store 'a = a + val;' and branch to merge
#define BUILD_ADD_AND_STORE(valToAdd, name)                                      \
   do                                                                            \
   {                                                                             \
      LLVMValueRef loaded = LLVMBuildLoad2(builder, int32Ty, a, "load_" name);   \
      LLVMValueRef added = LLVMBuildAdd(builder, loaded, valToAdd, "add_" name); \
      LLVMBuildStore(builder, added, a);                                         \
      LLVMBuildBr(builder, merge);                                               \
   } while (0)

   // Entry: check first condition: if (a > 2)
   LLVMValueRef aVal = LLVMBuildLoad2(builder, int32Ty, a, "a_val");
   LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSGT, aVal, const2, "cond1");
   BUILD_COND_BR(cond1, then1, elifCond);

   // then1: a = a + 3;
   LLVMPositionBuilderAtEnd(builder, then1);
   BUILD_ADD_AND_STORE(const3, "then1");

   // elif condition: if (a == 4)
   LLVMPositionBuilderAtEnd(builder, elifCond);
   LLVMValueRef aVal2 = LLVMBuildLoad2(builder, int32Ty, a, "a_val2");
   LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntEQ, aVal2, const4, "cond2");
   BUILD_COND_BR(cond2, then2, elseBlock);

   // then2: a = a + 5;
   LLVMPositionBuilderAtEnd(builder, then2);
   BUILD_ADD_AND_STORE(const5, "then2");

   // else: a = a + 6;
   LLVMPositionBuilderAtEnd(builder, elseBlock);
   BUILD_ADD_AND_STORE(const6, "else");

   // merge block: return 0
   LLVMPositionBuilderAtEnd(builder, merge);
   LLVMBuildRet(builder, const0);

   // Output IR
   char *irString = LLVMPrintModuleToString(mod);
   puts(irString);
   LLVMDisposeMessage(irString);

   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}
