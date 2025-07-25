#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// This program generates LLVM IR for the following C code:
/*
int main() {
    int a = 1;
    int b = a + 2;
    return 0;
}
*/

int main() {
    // Create a new LLVM module named "example"
    LLVMModuleRef mod = LLVMModuleCreateWithName("example");

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

    // Step 1: Declare and assign `a = 1`
    LLVMValueRef const1 = LLVMConstInt(int32Type, 1, 0);         // constant 1
    LLVMValueRef a = LLVMBuildAlloca(builder, int32Type, "a");   // allocate space for `a`
    LLVMBuildStore(builder, const1, a);                          // store 1 into `a`

    // Step 2: Load `a`, add 2 to it, and store the result in `b`
    LLVMValueRef a_val = LLVMBuildLoad2(builder, int32Type, a, "a_val"); // load `a`
    LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);                 // constant 2
    LLVMValueRef add = LLVMBuildAdd(builder, a_val, const2, "sum");     // a + 2
    LLVMValueRef b = LLVMBuildAlloca(builder, int32Type, "b");          // allocate space for `b`
    LLVMBuildStore(builder, add, b);                                    // store result into `b`

    // Step 3: Return 0
    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    // Write the generated IR to a file
    LLVMPrintModuleToFile(mod, "out.ir", NULL);

    // Clean up
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);

    return 0;
}
