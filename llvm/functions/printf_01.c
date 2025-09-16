#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

// Pseudocode:
/*
int main() {
   int result = 10;
   printf("func1 returned: %d\n", result);
   return 0;
}
*/

int main()
{
   // Initialize LLVM module and builder
   LLVMModuleRef mod = LLVMModuleCreateWithName("simple_printf");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef int8Type = LLVMInt8Type();
   LLVMTypeRef int8PtrType = LLVMPointerType(int8Type, 0);

   // ===== DECLARE PRINTF FUNCTION =====
   // printf signature: int printf(const char* format, ...)
   LLVMTypeRef printfParamTypes[] = {int8PtrType};                               // char* format string
   LLVMTypeRef printfType = LLVMFunctionType(int32Type, printfParamTypes, 1, 1); // variadic = 1
   LLVMValueRef printfFunc = LLVMAddFunction(mod, "printf", printfType);

   // ===== main() =====
   // Define function signature: int main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create entry basic block for main
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // int result = 10;
   LLVMValueRef result_alloca = LLVMBuildAlloca(builder, int32Type, "result");
   LLVMValueRef const10 = LLVMConstInt(int32Type, 10, 0);
   LLVMBuildStore(builder, const10, result_alloca);

   // Load result for printf
   LLVMValueRef result_val = LLVMBuildLoad2(builder, int32Type, result_alloca, "result_val");

   // Create format string: "func1 returned: %d\n"
   LLVMValueRef format_str = LLVMBuildGlobalStringPtr(builder, "func1 returned: %d\\n", "format");

   // printf("func1 returned: %d\n", result);
   LLVMValueRef printfArgs[] = {format_str, result_val};
   LLVMBuildCall2(builder, printfType, printfFunc, printfArgs, 2, "");

   // return 0;
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // ===== Output and Cleanup =====
   // Generate LLVM IR file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   // Clean up resources
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}