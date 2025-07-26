#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

// int a = 1;
// if (a > 2) a = a + 3;
// else if (a == 4) a = a + 5;
// else a = a + 6;

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("if_else_chain");
    LLVMContextRef ctx = LLVMGetGlobalContext();
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(ctx);

    // Define integer types
    LLVMTypeRef int32Ty = LLVMInt32TypeInContext(ctx);
    LLVMTypeRef funcType = LLVMFunctionType(int32Ty, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", funcType);

    // Entry block
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx, mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // === Constants ===
    LLVMValueRef const0 = LLVMConstInt(int32Ty, 0, 0);
    LLVMValueRef const1 = LLVMConstInt(int32Ty, 1, 0);
    LLVMValueRef const2 = LLVMConstInt(int32Ty, 2, 0);
    LLVMValueRef const3 = LLVMConstInt(int32Ty, 3, 0);
    LLVMValueRef const4 = LLVMConstInt(int32Ty, 4, 0);
    LLVMValueRef const5 = LLVMConstInt(int32Ty, 5, 0);
    LLVMValueRef const6 = LLVMConstInt(int32Ty, 6, 0);

    // int a = 1;
    LLVMValueRef a = LLVMBuildAlloca(builder, int32Ty, "a");
    LLVMBuildStore(builder, const1, a);

    // === First condition: if (a > 2) ===
    LLVMValueRef a_val1 = LLVMBuildLoad2(builder, int32Ty, a, "a_val1");
    LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntSGT, a_val1, const2, "cond1");

    LLVMBasicBlockRef then1 = LLVMAppendBasicBlockInContext(ctx, mainFunc, "then1");
    LLVMBasicBlockRef cond2 = LLVMAppendBasicBlockInContext(ctx, mainFunc, "cond2");
    LLVMBuildCondBr(builder, cond1, then1, cond2);

    // === then1: a = a + 3 ===
    LLVMPositionBuilderAtEnd(builder, then1);
    LLVMValueRef a_val_then1 = LLVMBuildLoad2(builder, int32Ty, a, "a_then1");
    LLVMValueRef result_then1 = LLVMBuildAdd(builder, a_val_then1, const3, "result_then1");
    LLVMBuildStore(builder, result_then1, a);
    LLVMBasicBlockRef mergeBB = LLVMAppendBasicBlockInContext(ctx, mainFunc, "merge");
    LLVMBuildBr(builder, mergeBB);

    // === cond2: else if (a == 4) ===
    LLVMPositionBuilderAtEnd(builder, cond2);
    LLVMValueRef a_val2 = LLVMBuildLoad2(builder, int32Ty, a, "a_val2");
    LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntEQ, a_val2, const4, "cond2");

    LLVMBasicBlockRef then2BB = LLVMAppendBasicBlockInContext(ctx, mainFunc, "then2");
    LLVMBasicBlockRef elseBB = LLVMAppendBasicBlockInContext(ctx, mainFunc, "else");
    LLVMBuildCondBr(builder, cond2, then2BB, elseBB);

    // === then2: a = a + 5 ===
    LLVMPositionBuilderAtEnd(builder, then2BB);
    LLVMValueRef a_val_then2 = LLVMBuildLoad2(builder, int32Ty, a, "a_then2");
    LLVMValueRef result_then2 = LLVMBuildAdd(builder, a_val_then2, const5, "result_then2");
    LLVMBuildStore(builder, result_then2, a);
    LLVMBuildBr(builder, mergeBB);

    // === else: a = a + 6 ===
    LLVMPositionBuilderAtEnd(builder, elseBB);
    LLVMValueRef a_val_else = LLVMBuildLoad2(builder, int32Ty, a, "a_else");
    LLVMValueRef result_else = LLVMBuildAdd(builder, a_val_else, const6, "result_else");
    LLVMBuildStore(builder, result_else, a);
    LLVMBuildBr(builder, mergeBB);

    // === merge ===
    LLVMPositionBuilderAtEnd(builder, mergeBB);
    LLVMBuildRet(builder, const0);

    // Output the IR
    LLVMDumpModule(mod);

    // Clean up
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);

    return 0;
}
