/*
This example creates a struct type `Point` with two i32 fields: x and y.
It allocates space for the struct and stores values into the fields.
*/

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("struct_example");
    LLVMBuilderRef builder = LLVMCreateBuilder();

    LLVMTypeRef i32 = LLVMInt32Type();

    // Create struct type: { i32, i32 }
    LLVMTypeRef pointStruct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Point");
    LLVMTypeRef elements[] = { i32, i32 };
    LLVMStructSetBody(pointStruct, elements, 2, 0);

    // Define main function
    LLVMTypeRef mainType = LLVMFunctionType(i32, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // Allocate struct
    LLVMValueRef point = LLVMBuildAlloca(builder, pointStruct, "point");

    // Access and store into fields
    LLVMValueRef gepX = LLVMBuildStructGEP(builder, point, 0, "x");
    LLVMBuildStore(builder, LLVMConstInt(i32, 10, 0), gepX);

    LLVMValueRef gepY = LLVMBuildStructGEP(builder, point, 1, "y");
    LLVMBuildStore(builder, LLVMConstInt(i32, 20, 0), gepY);

    LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));

    LLVMPrintModuleToFile(mod, "struct1.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
