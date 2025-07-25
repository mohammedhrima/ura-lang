#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// pseudo code
// int a = 10;
// if (a > 5) {
//    a = a + 1;
// }

int main() {
    LLVMModuleRef module = LLVMModuleCreateWithName("if_stmt");
    LLVMContextRef context = LLVMGetGlobalContext();
    LLVMBuilderRef builder = LLVMCreateBuilder();

    LLVMTypeRef funcType = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(module, "main", funcType);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // int a = 10;
    LLVMValueRef aVar = LLVMBuildAlloca(builder, LLVMInt32Type(), "a");
    LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), 10, 0), aVar);

    // if (a > 5)
    LLVMValueRef aVal = LLVMBuildLoad2(builder, LLVMInt32Type(), aVar, "a.load");
    LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSGT, aVal, LLVMConstInt(LLVMInt32Type(), 5, 0), "cond");

    LLVMBasicBlockRef thenBB = LLVMAppendBasicBlock(mainFunc, "then");
    LLVMBasicBlockRef mergeBB = LLVMAppendBasicBlock(mainFunc, "merge");
    LLVMBuildCondBr(builder, cond, thenBB, mergeBB);

    // then: a = a + 1;
    LLVMPositionBuilderAtEnd(builder, thenBB);
    LLVMValueRef aValThen = LLVMBuildLoad2(builder, LLVMInt32Type(), aVar, "a.load");
    LLVMValueRef inc = LLVMBuildAdd(builder, aValThen, LLVMConstInt(LLVMInt32Type(), 1, 0), "a.inc");
    LLVMBuildStore(builder, inc, aVar);
    LLVMBuildBr(builder, mergeBB);

    // merge
    LLVMPositionBuilderAtEnd(builder, mergeBB);
    LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), 0, 0));

    // Print the IR to stdout
    char *irString = LLVMPrintModuleToString(module);
    puts(irString);
    LLVMDisposeMessage(irString);

    // Clean up
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);

    return 0;
}
