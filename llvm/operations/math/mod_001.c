#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// This program generates LLVM IR for the following C code:
/*
int main() {
    int a = 10;
    int b = a % 2;
    return 0;
}
*/

int main()
{
   // Create a new LLVM module named "modulo_example"
   LLVMModuleRef mod = LLVMModuleCreateWithName("modulo_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define the int32 type (equivalent to `int` in C)
   LLVMTypeRef int32Type = LLVMInt32Type();

   // Create the function type for main: `int main()`
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);

   // Add the `main` function to the module
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create an entry block and position the builder at its end
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // Step 1: Declare and assign `a = 10`
   LLVMValueRef const10 = LLVMConstInt(int32Type, 10, 0);     // constant 10
   LLVMValueRef a = LLVMBuildAlloca(builder, int32Type, "a"); // allocate space for `a`
   LLVMBuildStore(builder, const10, a);                       // store 10 into `a`

   // Step 2: Load `a`, compute a % 2, and store the result in `b`
   LLVMValueRef a_val = LLVMBuildLoad2(builder, int32Type, a, "a_val");    // load `a`
   LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);                    // constant 2
   LLVMValueRef mod_result = LLVMBuildSRem(builder, a_val, const2, "mod"); // a % 2 (signed remainder)
   LLVMValueRef b = LLVMBuildAlloca(builder, int32Type, "b");              // allocate space for `b`
   LLVMBuildStore(builder, mod_result, b);                                 // store result into `b`

   // Step 3: Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   printf("LLVM IR written to out.ll\n");

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}