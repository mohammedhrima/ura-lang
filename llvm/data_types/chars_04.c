#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   char* str = "abcd";
   char c = str[0];
   putchar(c);
}
*/

int main()
{
   // Create a new LLVM module
   LLVMModuleRef mod = LLVMModuleCreateWithName("putchar_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define types
   LLVMTypeRef int32Type = LLVMInt32Type();                // int
   LLVMTypeRef charType = LLVMInt8Type();                  // char (i8)
   LLVMTypeRef charPtrType = LLVMPointerType(charType, 0); // char*

   // Declare external function: int putchar(int c)
   LLVMTypeRef putcharParamTypes[] = { int32Type };
   LLVMTypeRef putcharType = LLVMFunctionType(int32Type, putcharParamTypes, 1, 0);
   LLVMValueRef putcharFunc = LLVMAddFunction(mod, "putchar", putcharType);

   // Create the function type for main: `int main()`
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);

   // Add the `main` function to the module
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create entry basic block
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");

   // === Entry block ===
   LLVMPositionBuilderAtEnd(builder, entry);

   // Create string literal "abcd"
   LLVMValueRef str_literal = LLVMBuildGlobalStringPtr(builder, "abcd", "str");

   // Allocate local variable for str pointer
   LLVMValueRef str = LLVMBuildAlloca(builder, charPtrType, "str");
   LLVMBuildStore(builder, str_literal, str);

   // Load str
   LLVMValueRef str_val = LLVMBuildLoad2(builder, charPtrType, str, "str_val");

   // Calculate str[0] address using GEP
   LLVMValueRef indices[] = { LLVMConstInt(int32Type, 0, 0) };
   LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, charType, str_val, 
                                          indices, 1, "elem_ptr");

   // Load char c = str[0]
   LLVMValueRef c_val = LLVMBuildLoad2(builder, charType, elem_ptr, "c");

   // Allocate local variable for c
   LLVMValueRef c = LLVMBuildAlloca(builder, charType, "c");
   LLVMBuildStore(builder, c_val, c);

   // Load c for putchar call
   LLVMValueRef c_for_call = LLVMBuildLoad2(builder, charType, c, "c_val");

   // Extend char to int32 for putchar (putchar takes int)
   LLVMValueRef c_extended = LLVMBuildSExt(builder, c_for_call, int32Type, "c_ext");

   // Call putchar(c)
   LLVMValueRef putcharArgs[] = { c_extended };
   LLVMBuildCall2(builder, putcharType, putcharFunc, putcharArgs, 1, "");

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}