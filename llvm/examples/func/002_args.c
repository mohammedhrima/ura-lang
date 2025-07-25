#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int func1(int a)
{
    a = 2;
    return 1;
}

int main()
{
    func1(1);
    return 0;
}
*/

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("func1_example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // func1(int a)
    LLVMTypeRef func1ParamTypes[] = { int32Type };
    LLVMTypeRef func1Type = LLVMFunctionType(int32Type, func1ParamTypes, 1, 0);
    LLVMValueRef func1 = LLVMAddFunction(mod, "func1", func1Type);
    LLVMBasicBlockRef func1Entry = LLVMAppendBasicBlock(func1, "entry");
    LLVMPositionBuilderAtEnd(builder, func1Entry);
    LLVMValueRef param = LLVMGetParam(func1, 0);
    LLVMValueRef param_alloca = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMBuildStore(builder, param, param_alloca);
    LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);
    LLVMBuildStore(builder, const2, param_alloca);
    LLVMValueRef const1 = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildRet(builder, const1);

    // main()
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, mainEntry);
    LLVMValueRef arg1 = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildCall2(builder, func1Type, func1, &arg1, 1, "");
    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    LLVMPrintModuleToFile(mod, "func1.ir", NULL);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
