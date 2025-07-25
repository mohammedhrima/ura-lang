#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int func3()
{
    return 3;
}

int main()
{
    return func3();
}
*/

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("func3_example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // func3()
    LLVMTypeRef func3Type = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef func3 = LLVMAddFunction(mod, "func3", func3Type);
    LLVMBasicBlockRef func3Entry = LLVMAppendBasicBlock(func3, "entry");
    LLVMPositionBuilderAtEnd(builder, func3Entry);
    LLVMValueRef const3 = LLVMConstInt(int32Type, 3, 0);
    LLVMBuildRet(builder, const3);

    // main()
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, mainEntry);
    LLVMValueRef result = LLVMBuildCall2(builder, func3Type, func3, NULL, 0, "");
    LLVMBuildRet(builder, result);

    LLVMPrintModuleToFile(mod, "func3.ir", NULL);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
