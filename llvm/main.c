#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("example");
    LLVMBuilderRef builder = LLVMCreateBuilder();

    LLVMTypeRef int32Type = LLVMInt32Type();
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef const1 = LLVMConstInt(int32Type, 1, 0);  // a = 1
    LLVMValueRef a = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMBuildStore(builder, const1, a);

    LLVMValueRef a_val = LLVMBuildLoad2(builder, int32Type, a, "a_val");
    LLVMValueRef const2 = LLVMConstInt(int32Type, 2, 0);  // +2
    LLVMValueRef add = LLVMBuildAdd(builder, a_val, const2, "sum");

    LLVMValueRef b = LLVMBuildAlloca(builder, int32Type, "b");
    LLVMBuildStore(builder, add, b);

    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    LLVMPrintModuleToFile(mod, "out.ir", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
