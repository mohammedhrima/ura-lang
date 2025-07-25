#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main() {
    int a = 1;
    int *b = &a;
    *b = 2;
    return 0;
}
*/

int main() {
    // === Setup LLVM context ===
    LLVMModuleRef mod = LLVMModuleCreateWithName("pointer_example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // === Define main function ===
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // === Step 1: int a = 1; ===
    LLVMValueRef a_alloca = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMValueRef one = LLVMConstInt(int32Type, 1, 0);
    LLVMBuildStore(builder, one, a_alloca);

    // === Step 2: int *b = &a; ===
    LLVMValueRef b_alloca = LLVMBuildAlloca(builder, LLVMPointerType(int32Type, 0), "b");
    LLVMBuildStore(builder, a_alloca, b_alloca);  // store pointer to `a` in `b`

    // === Step 3: *b = 2; ===
    LLVMValueRef two = LLVMConstInt(int32Type, 2, 0);
    LLVMValueRef b_loaded = LLVMBuildLoad2(builder, LLVMPointerType(int32Type, 0), b_alloca, "b_val");
    LLVMBuildStore(builder, two, b_loaded);

    // === Return 0 ===
    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    // === Output IR ===
    LLVMPrintModuleToFile(mod, "pointer_example.ir", NULL);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);

    return 0;
}
