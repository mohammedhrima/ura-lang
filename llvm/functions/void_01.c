#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
void func1(int a)
{
    puts("abc");
    return;
}

int main()
{
    func1(1);
    return 0;
}
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("puts_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef voidType = LLVMVoidType();

   // Declare puts function: int puts(const char*)
   LLVMTypeRef charType = LLVMInt8Type();
   LLVMTypeRef charPtrType = LLVMPointerType(charType, 0);
   LLVMTypeRef putsParamTypes[] = {charPtrType};
   LLVMTypeRef putsType = LLVMFunctionType(int32Type, putsParamTypes, 1, 0);
   LLVMValueRef putsFunc = LLVMAddFunction(mod, "puts", putsType);

   // func1(int a)
   LLVMTypeRef func1ParamTypes[] = {int32Type};
   LLVMTypeRef func1Type = LLVMFunctionType(voidType, func1ParamTypes, 1, 0);
   LLVMValueRef func1 = LLVMAddFunction(mod, "func1", func1Type);

   LLVMBasicBlockRef func1Entry = LLVMAppendBasicBlock(func1, "entry");
   LLVMPositionBuilderAtEnd(builder, func1Entry);

   // Get parameter
   LLVMValueRef paramA = LLVMGetParam(func1, 0);

   // Create local variable for parameter
   LLVMValueRef aAlloca = LLVMBuildAlloca(builder, int32Type, "a");
   LLVMBuildStore(builder, paramA, aAlloca);

   // Create string constant "abc"
   LLVMValueRef strConstant = LLVMBuildGlobalStringPtr(builder, "abc", "str");

   // Call puts("abc")
   LLVMValueRef putsArgs[] = {strConstant};
   LLVMBuildCall2(builder, putsType, putsFunc, putsArgs, 1, "");

   // return;
   LLVMBuildRetVoid(builder);

   // main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // Call func1(1)
   LLVMValueRef func1Args[] = {LLVMConstInt(int32Type, 1, 0)};
   LLVMBuildCall2(builder, func1Type, func1, func1Args, 1, "");

   // return 0;
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Output the IR
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}