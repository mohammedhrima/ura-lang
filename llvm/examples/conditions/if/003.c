#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
// int a = 10;
// if (a > 1)  // a = a + 1
// if (a < 1)  // a = a + 1
// if (a <= 1) // a = a + 1
// if (a >= 1) // a = a + 1
// if (a == 1) // a = a + 1
// if (a != 1) // a = a + 1

int main() {
    LLVMModuleRef module = LLVMModuleCreateWithName("multiple_if_conditions");
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

    // Helper for building each if condition
    #define BUILD_IF(CONDITION_NAME, PREDICATE, VALUE)                          \
        LLVMValueRef aVal_##CONDITION_NAME = LLVMBuildLoad2(builder, int32Type, aVar, "a_load_" #CONDITION_NAME); \
        LLVMValueRef cond_##CONDITION_NAME = LLVMBuildICmp(builder, PREDICATE, aVal_##CONDITION_NAME, LLVMConstInt(int32Type, VALUE, 0), "cond_" #CONDITION_NAME); \
        LLVMBasicBlockRef block_##CONDITION_NAME = LLVMAppendBasicBlock(mainFunc, "if_" #CONDITION_NAME); \
        LLVMBasicBlockRef next_##CONDITION_NAME = LLVMAppendBasicBlock(mainFunc, "after_" #CONDITION_NAME); \
        LLVMBuildCondBr(builder, cond_##CONDITION_NAME, block_##CONDITION_NAME, next_##CONDITION_NAME); \
        LLVMPositionBuilderAtEnd(builder, block_##CONDITION_NAME); \
        LLVMValueRef aThen_##CONDITION_NAME = LLVMBuildLoad2(builder, int32Type, aVar, "a_" #CONDITION_NAME); \
        LLVMValueRef inc_##CONDITION_NAME = LLVMBuildAdd(builder, aThen_##CONDITION_NAME, LLVMConstInt(int32Type, 1, 0), "a_inc_" #CONDITION_NAME); \
        LLVMBuildStore(builder, inc_##CONDITION_NAME, aVar); \
        LLVMBuildBr(builder, next_##CONDITION_NAME); \
        LLVMPositionBuilderAtEnd(builder, next_##CONDITION_NAME);

    // Build all conditions
    BUILD_IF(gt, LLVMIntSGT, 1)   // if a > 1
    BUILD_IF(lt, LLVMIntSLT, 1)   // if a < 1
    BUILD_IF(le, LLVMIntSLE, 1)   // if a <= 1
    BUILD_IF(ge, LLVMIntSGE, 1)   // if a >= 1
    BUILD_IF(eq, LLVMIntEQ, 1)    // if a == 1
    BUILD_IF(ne, LLVMIntNE, 1)    // if a != 1

    // Final return block
    LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

    // Output IR
    char *irString = LLVMPrintModuleToString(module);
    puts(irString);
    LLVMDisposeMessage(irString);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    return 0;
}
