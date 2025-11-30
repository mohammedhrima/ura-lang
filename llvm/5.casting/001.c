#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int main()
{
    char* str = "abc";
    char c = str[0];
    int a = c + 2;
    char b = a;
}
*/

int main()
{
   // Initialize LLVM module and builder
   LLVMModuleRef mod = LLVMModuleCreateWithName("string_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();

   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef int8Type = LLVMInt8Type();
   LLVMTypeRef int8PtrType = LLVMPointerType(int8Type, 0);

   // Create global string constant "abc"
   LLVMValueRef stringConstant = LLVMConstString("abc", 3, 0);
   LLVMValueRef globalStr = LLVMAddGlobal(mod, LLVMTypeOf(stringConstant), "str_literal");
   LLVMSetInitializer(globalStr, stringConstant);
   LLVMSetGlobalConstant(globalStr, 1);
   LLVMSetLinkage(globalStr, LLVMPrivateLinkage);

   // ===== main() =====
   // Define function signature: int main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create entry basic block for main
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // char* str = "abc";
   LLVMValueRef str_alloca = LLVMBuildAlloca(builder, int8PtrType, "str");
   LLVMBuildStore(builder, globalStr, str_alloca);

   // char c = str[0];
   LLVMValueRef c_alloca = LLVMBuildAlloca(builder, int32Type, "c");

   // Load the pointer
   LLVMValueRef str_loaded = LLVMBuildLoad2(builder, int8PtrType, str_alloca, "str1");

   // Get pointer to str[0]
   LLVMValueRef indices[] = {LLVMConstInt(int32Type, 0, 0)};
   LLVMValueRef access_ptr = LLVMBuildGEP2(builder, int8Type, str_loaded, indices, 1, "access");

   // Load the i8 value
   LLVMValueRef access_val = LLVMBuildLoad2(builder, int8Type, access_ptr, "access_val");

   // Extend i8 to i32 (since char size is similar to int)
   LLVMValueRef c_extended = LLVMBuildZExt(builder, access_val, int32Type, "extended");

   // Store to c
   LLVMBuildStore(builder, c_extended, c_alloca);

   // int a = c + 2;
   LLVMValueRef a_alloca = LLVMBuildAlloca(builder, int32Type, "a");

   // Load c
   LLVMValueRef c_loaded = LLVMBuildLoad2(builder, int32Type, c_alloca, "c1");

   // Add 2
   LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);
   LLVMValueRef add_result = LLVMBuildAdd(builder, c_loaded, const2, "ADD");

   // Store to a
   LLVMBuildStore(builder, add_result, a_alloca);

   // char b = a;
   LLVMValueRef b_alloca = LLVMBuildAlloca(builder, int32Type, "b");

   // Load a
   LLVMValueRef a_loaded = LLVMBuildLoad2(builder, int32Type, a_alloca, "a1");

   // Store to b (no truncation needed since both are i32)
   LLVMBuildStore(builder, a_loaded, b_alloca);

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