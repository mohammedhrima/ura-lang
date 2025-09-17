#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   char *str = "abc";
   return 0;
}
*/

int main()
{
   // Create a new LLVM module named "string_example"
   LLVMModuleRef mod = LLVMModuleCreateWithName("string_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define types
   LLVMTypeRef int32Type = LLVMInt32Type();                // int
   LLVMTypeRef charType = LLVMInt8Type();                  // char (i8)
   LLVMTypeRef charPtrType = LLVMPointerType(charType, 0); // char*

   // Create the function type for main: `int main()`
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);

   // Add the `main` function to the module
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create an entry block and position the builder at its end
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // Method 1: Using LLVMBuildGlobalStringPtr (simpler approach)
   LLVMValueRef str_literal = LLVMBuildGlobalStringPtr(builder, "abc", "str_literal");

   // Declare char* str variable
   LLVMValueRef str = LLVMBuildAlloca(builder, charPtrType, "str");

   // Store the string literal address into str
   LLVMBuildStore(builder, str_literal, str);

   /* Alternative Method 2: Manual global string creation
   // Create a global string constant
   LLVMValueRef str_array = LLVMConstString("abc", 3, 0);  // "abc" without null terminator
   LLVMValueRef global_str = LLVMAddGlobal(mod, LLVMTypeOf(str_array), "global_str");
   LLVMSetInitializer(global_str, str_array);
   LLVMSetGlobalConstant(global_str, 1);  // Mark as constant
   LLVMSetLinkage(global_str, LLVMPrivateLinkage);

   // Get pointer to the global string
   LLVMValueRef indices[] = {
       LLVMConstInt(int32Type, 0, 0),
       LLVMConstInt(int32Type, 0, 0)
   };
   LLVMValueRef str_ptr = LLVMBuildGEP2(builder, LLVMTypeOf(str_array), global_str, indices, 2, "str_ptr");

   // Store the pointer
   LLVMBuildStore(builder, str_ptr, str);
   */

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "string_example.ll", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}
