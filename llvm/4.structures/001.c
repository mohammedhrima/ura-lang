/*
This example creates a struct type `Point` with two i32 fields: x and y.
It allocates space for the struct and stores values into the fields.
Fixed for LLVM 20+ using LLVMBuildStructGEP2 instead of deprecated LLVMBuildStructGEP.
*/

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

/*
struct Point:
    int x
    int y
*/

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("struct_example");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef i32 = LLVMInt32Type();

    // Define main function
    LLVMTypeRef mainType = LLVMFunctionType(i32, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // CREATE STRUCT
    LLVMTypeRef pointStruct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Point");
    LLVMTypeRef elements[] = { i32, i32 };

    // SET STRUCT BODY
    LLVMStructSetBody(pointStruct, elements, 2, 0);

    // ALLOCATE STRUCT
    LLVMValueRef point = LLVMBuildAlloca(builder, pointStruct, "point");

    // ACCESS STRUCT FIELD x (index 0) - Using LLVMBuildStructGEP2 for LLVM 20+
    LLVMValueRef gepX = LLVMBuildStructGEP2(builder, pointStruct, point, 0, "x");
    LLVMBuildStore(builder, LLVMConstInt(i32, 10, 0), gepX);

    // ACCESS STRUCT FIELD y (index 1) - Using LLVMBuildStructGEP2 for LLVM 20+
    LLVMValueRef gepY = LLVMBuildStructGEP2(builder, pointStruct, point, 1, "y");
    LLVMBuildStore(builder, LLVMConstInt(i32, 20, 0), gepY);

    LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));

    LLVMPrintModuleToFile(mod, "out.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}