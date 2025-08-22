#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int main()
{
    int a = 1;
    return a;
}
*/

int main() {
    LLVMModuleRef module = LLVMModuleCreateWithName("main_module");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMContextRef context = LLVMGetGlobalContext();

    LLVMTypeRef int32Type = LLVMInt32Type();

    // Define the function type: int () -> int32
    LLVMTypeRef funcType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(module, "main", funcType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // int a = 1;
    LLVMValueRef aAlloca = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMValueRef one = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildStore(builder, one, aAlloca);

    // return a;
    LLVMValueRef aValue = LLVMBuildLoad2(builder, int32Type, aAlloca, "a_val");
    LLVMBuildRet(builder, aValue);

    // Output LLVM IR
    LLVMPrintModuleToFile(module, "output.ll", NULL);

    // Clean up
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);

    return 0;
}
