#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
Generates LLVM IR for:
func void putnbr(int n):
    int digit
    while n > 10:
        digit = n % 10
        n = n / 10

main():
    putnbr(123)
    return 0
*/

int main()
{
   // Create a new LLVM module
   LLVMModuleRef mod = LLVMModuleCreateWithName("putnbr_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define types
   LLVMTypeRef voidType = LLVMVoidType();
   LLVMTypeRef int32Type = LLVMInt32Type();

   // ========== Create putnbr function ==========
   // Function signature: void putnbr(int n)
   LLVMTypeRef putnbrParamTypes[] = {int32Type};
   LLVMTypeRef putnbrType = LLVMFunctionType(voidType, putnbrParamTypes, 1, 0);
   LLVMValueRef putnbrFunc = LLVMAddFunction(mod, "putnbr", putnbrType);

   // Get the parameter 'n'
   LLVMValueRef n_param = LLVMGetParam(putnbrFunc, 0);
   LLVMSetValueName(n_param, "n");

   // Create basic blocks for putnbr
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(putnbrFunc, "entry");
   LLVMBasicBlockRef whileCond = LLVMAppendBasicBlock(putnbrFunc, "while");
   LLVMBasicBlockRef whileBody = LLVMAppendBasicBlock(putnbrFunc, "while_bloc");
   LLVMBasicBlockRef whileEnd = LLVMAppendBasicBlock(putnbrFunc, "end_while");

   // === Entry block ===
   LLVMPositionBuilderAtEnd(builder, entry);

   // Allocate space for 'n' (to make it mutable)
   LLVMValueRef n = LLVMBuildAlloca(builder, int32Type, "n");
   LLVMBuildStore(builder, n_param, n);

   // Allocate space for 'digit'
   LLVMValueRef digit = LLVMBuildAlloca(builder, int32Type, "digit");

   // Branch to while condition
   LLVMBuildBr(builder, whileCond);

   // === While condition block ===
   LLVMPositionBuilderAtEnd(builder, whileCond);

   // Load n
   LLVMValueRef n_val = LLVMBuildLoad2(builder, int32Type, n, "n_val");

   // Compare n > 10
   LLVMValueRef cmp = LLVMBuildICmp(builder, LLVMIntSGT, n_val,
                                    LLVMConstInt(int32Type, 10, 0), "MORE");

   // Conditional branch: if true goto while_bloc, else goto end_while
   LLVMBuildCondBr(builder, cmp, whileBody, whileEnd);

   // === While body block ===
   LLVMPositionBuilderAtEnd(builder, whileBody);

   // Load n
   LLVMValueRef n_val2 = LLVMBuildLoad2(builder, int32Type, n, "n_val2");

   // digit = n % 10
   LLVMValueRef mod_result = LLVMBuildSRem(builder, n_val2,
                                           LLVMConstInt(int32Type, 10, 0), "MOD");
   LLVMBuildStore(builder, mod_result, digit);

   // Load n again for division
   LLVMValueRef n_val3 = LLVMBuildLoad2(builder, int32Type, n, "n_val3");

   // n = n / 10
   LLVMValueRef div_result = LLVMBuildSDiv(builder, n_val3,
                                           LLVMConstInt(int32Type, 10, 0), "DIV");
   LLVMBuildStore(builder, div_result, n);

   // Branch back to condition
   LLVMBuildBr(builder, whileCond);

   // === While end block ===
   LLVMPositionBuilderAtEnd(builder, whileEnd);

   // Return void
   LLVMBuildRetVoid(builder);

   // ========== Create main function ==========
   // Function signature: int main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create entry block for main
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // Call putnbr(123)
   LLVMValueRef args[] = {LLVMConstInt(int32Type, 123, 0)};
   LLVMBuildCall2(builder, putnbrType, putnbrFunc, args, 1, "");

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   printf("LLVM IR written to out.ll\n");

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}