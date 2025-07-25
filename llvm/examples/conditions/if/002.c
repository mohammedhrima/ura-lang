#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudo code:
// int a = 10;
// if (a > 3)
//     a = a + 1;
// else if (a > 2)
//     a = a + 2;
// else
//     a = a + 3;

int main() {
    LLVMModuleRef module = LLVMModuleCreateWithName("if_elif_else");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    // Function: int main()
    LLVMTypeRef funcType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(module, "main", funcType);
    LLVMBasicBlockRef entryBB = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBB);

    // int a = 10;
    LLVMValueRef aVar = LLVMBuildAlloca(builder, int32Type, "a");
    LLVMBuildStore(builder, LLVMConstInt(int32Type, 10, 0), aVar);

    // Load a for first condition: if (a > 3)
    LLVMValueRef aVal1 = LLVMBuildLoad2(builder, int32Type, aVar, "a_val1");
    LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSGT, aVal1, LLVMConstInt(int32Type, 3, 0), "cond1");

    // Blocks
    LLVMBasicBlockRef ifBB = LLVMAppendBasicBlock(mainFunc, "if");
    LLVMBasicBlockRef elifCondBB = LLVMAppendBasicBlock(mainFunc, "elif_cond");
    LLVMBasicBlockRef elifBB = LLVMAppendBasicBlock(mainFunc, "elif");
    LLVMBasicBlockRef elseBB = LLVMAppendBasicBlock(mainFunc, "else");
    LLVMBasicBlockRef mergeBB = LLVMAppendBasicBlock(mainFunc, "merge");

    LLVMBuildCondBr(builder, cond1, ifBB, elifCondBB);

    // if (a > 3): a = a + 1;
    LLVMPositionBuilderAtEnd(builder, ifBB);
    LLVMValueRef aIf = LLVMBuildLoad2(builder, int32Type, aVar, "a_if");
    LLVMValueRef inc1 = LLVMBuildAdd(builder, aIf, LLVMConstInt(int32Type, 1, 0), "a_inc1");
    LLVMBuildStore(builder, inc1, aVar);
    LLVMBuildBr(builder, mergeBB);

    // elif condition: (a > 2)
    LLVMPositionBuilderAtEnd(builder, elifCondBB);
    LLVMValueRef aVal2 = LLVMBuildLoad2(builder, int32Type, aVar, "a_val2");
    LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntSGT, aVal2, LLVMConstInt(int32Type, 2, 0), "cond2");
    LLVMBuildCondBr(builder, cond2, elifBB, elseBB);

    // elif: a = a + 2;
    LLVMPositionBuilderAtEnd(builder, elifBB);
    LLVMValueRef aElif = LLVMBuildLoad2(builder, int32Type, aVar, "a_elif");
    LLVMValueRef inc2 = LLVMBuildAdd(builder, aElif, LLVMConstInt(int32Type, 2, 0), "a_inc2");
    LLVMBuildStore(builder, inc2, aVar);
    LLVMBuildBr(builder, mergeBB);

    // else: a = a + 3;
    LLVMPositionBuilderAtEnd(builder, elseBB);
    LLVMValueRef aElse = LLVMBuildLoad2(builder, int32Type, aVar, "a_else");
    LLVMValueRef inc3 = LLVMBuildAdd(builder, aElse, LLVMConstInt(int32Type, 3, 0), "a_inc3");
    LLVMBuildStore(builder, inc3, aVar);
    LLVMBuildBr(builder, mergeBB);

    // merge block
    LLVMPositionBuilderAtEnd(builder, mergeBB);
    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    // Output IR
    char *irString = LLVMPrintModuleToString(module);
    puts(irString);
    LLVMDisposeMessage(irString);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);

    return 0;
}
