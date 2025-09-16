#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
bool isalpha(char c)
{
   return c >= 'a' && c <= 'z';
}
int main()
{
   return 0;
}
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("isalpha_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef charType = LLVMInt8Type(); // char is i8
   LLVMTypeRef boolType = LLVMInt1Type(); // bool is i1

   // -----------------------------------
   // Define: bool isalpha(char c)
   // -----------------------------------
   LLVMTypeRef isalpha_params[] = {charType};
   LLVMTypeRef isalphaFuncType = LLVMFunctionType(boolType, isalpha_params, 1, 0);
   LLVMValueRef isalphaFunc = LLVMAddFunction(mod, "isalpha", isalphaFuncType);

   // Get function parameter
   LLVMValueRef param_c = LLVMGetParam(isalphaFunc, 0);
   LLVMSetValueName(param_c, "c");

   // Create entry block for isalpha function
   LLVMBasicBlockRef isalphaEntry = LLVMAppendBasicBlock(isalphaFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, isalphaEntry);

   // c >= 'a' (ASCII value 97)
   LLVMValueRef char_a = LLVMConstInt(charType, 97, 0); // 'a' = 97
   LLVMValueRef c_ge_a = LLVMBuildICmp(builder, LLVMIntUGE, param_c, char_a, "c_ge_a");

   // c <= 'z' (ASCII value 122)
   LLVMValueRef char_z = LLVMConstInt(charType, 122, 0); // 'z' = 122
   LLVMValueRef c_le_z = LLVMBuildICmp(builder, LLVMIntULE, param_c, char_z, "c_le_z");

   // c >= 'a' && c <= 'z'
   LLVMValueRef result = LLVMBuildAnd(builder, c_ge_a, c_le_z, "result");

   // Return the boolean result
   LLVMBuildRet(builder, result);

   // -----------------------------------
   // Define: int main()
   // -----------------------------------
   LLVMTypeRef mainFuncType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainFuncType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // return 0;
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // -----------------------------------
   // Emit IR
   // -----------------------------------
   LLVMPrintModuleToFile(mod, "isalpha.ll", NULL);
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}