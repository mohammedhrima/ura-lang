#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int func1(int a)
{
    a = 2;
    return 1;
}

int func2()
{
    return 2;
}

int main()
{
    func1(1);
    func2();
}
*/

int main() {
    // Setup LLVM Module and Builder
    LLVMModuleRef mod = LLVMModuleCreateWithName("example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // ======================================================
    // Define: int func1(int a)
    // ======================================================
    LLVMTypeRef func1ParamTypes[] = { int32Type };
    LLVMTypeRef func1Type = LLVMFunctionType(int32Type, func1ParamTypes, 1, 0);
    LLVMValueRef func1 = LLVMAddFunction(mod, "func1", func1Type);

    // func1 entry block
    LLVMBasicBlockRef func1Entry = LLVMAppendBasicBlock(func1, "entry");
    LLVMPositionBuilderAtEnd(builder, func1Entry);

    // Allocate local variable 'a' and set to 2
    LLVMValueRef func1_a = LLVMGetParam(func1, 0);
    LLVMValueRef func1_a_alloca = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMBuildStore(builder, func1_a, func1_a_alloca);
    LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);
    LLVMBuildStore(builder, const2, func1_a_alloca);

    // return 1;
    LLVMValueRef const1 = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildRet(builder, const1);

    // ======================================================
    // Define: int func2()
    // ======================================================
    LLVMTypeRef func2Type = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef func2 = LLVMAddFunction(mod, "func2", func2Type);

    // func2 entry block
    LLVMBasicBlockRef func2Entry = LLVMAppendBasicBlock(func2, "entry");
    LLVMPositionBuilderAtEnd(builder, func2Entry);

    // return 2;
    LLVMBuildRet(builder, const2);  // already defined const2 = 2

    // ======================================================
    // Define: int main()
    // ======================================================
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, mainEntry);

    // func1(1);
    LLVMValueRef arg1 = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildCall2(builder, func1Type, func1, &arg1, 1, "");

    // func2();
    LLVMBuildCall2(builder, func2Type, func2, NULL, 0, "");

    // return 0;
    LLVMValueRef const0 = LLVMConstInt(int32Type, 0, 0);
    LLVMBuildRet(builder, const0);

    // ======================================================
    // Write IR to file and clean up
    // ======================================================
    LLVMPrintModuleToFile(mod, "out.ir", NULL);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
