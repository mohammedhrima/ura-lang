#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int func()
{
    return 1;
}

int main()
{
    int a = 1 + 2;
    int b = 3;
    int c = a + b;

    int d = a * c;
    int e = d % 3;
    int g = func();
}
*/

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // -----------------------------------
    // Define: int func()
    // -----------------------------------
    LLVMTypeRef funcType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef func = LLVMAddFunction(mod, "func", funcType);
    LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(func, "entry");
    LLVMPositionBuilderAtEnd(builder, funcEntry);
    LLVMValueRef const1 = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildRet(builder, const1);

    // -----------------------------------
    // Define: int main()
    // -----------------------------------
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", funcType);
    LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, mainEntry);

    // int a = 1 + 2;
    LLVMValueRef a = LLVMBuildAdd(builder,
        LLVMConstInt(int32Type, 1, 0),
        LLVMConstInt(int32Type, 2, 0),
        "a");

    // int b = 3;
    LLVMValueRef b = LLVMConstInt(int32Type, 3, 0);

    // int c = a + b;
    LLVMValueRef c = LLVMBuildAdd(builder, a, b, "c");

    // int d = a * c;
    LLVMValueRef d = LLVMBuildMul(builder, a, c, "d");

    // int e = d % 3;
    LLVMValueRef e = LLVMBuildURem(builder, d, LLVMConstInt(int32Type, 3, 0), "e");

    // int g = func();
    LLVMValueRef g = LLVMBuildCall2(builder, funcType, func, NULL, 0, "g");

    // return 0;
    LLVMValueRef const0 = LLVMConstInt(int32Type, 0, 0);
    LLVMBuildRet(builder, const0);

    // -----------------------------------
    // Emit IR
    // -----------------------------------
    LLVMPrintModuleToFile(mod, "out.ir", NULL);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
