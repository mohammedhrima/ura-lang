/*
This example defines two structs: `Point` and `Line`.
`Line` contains two `Point` fields. We allocate a `Line`, then set coordinates for both points.
Fixed for LLVM 20+ using LLVMBuildStructGEP2.
*/

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("nested_struct");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef i32 = LLVMInt32Type();

    // Create Point struct: { i32, i32 }
    LLVMTypeRef point = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Point");
    LLVMTypeRef pointFields[] = { i32, i32 };
    LLVMStructSetBody(point, pointFields, 2, 0);

    // Create Line struct: { Point, Point }
    LLVMTypeRef line = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Line");
    LLVMTypeRef lineFields[] = { point, point };
    LLVMStructSetBody(line, lineFields, 2, 0);

    LLVMTypeRef mainType = LLVMFunctionType(i32, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef lineVar = LLVMBuildAlloca(builder, line, "line");

    // Access line.a.x (field 0 -> x = 0) - Using LLVMBuildStructGEP2
    LLVMValueRef a = LLVMBuildStructGEP2(builder, line, lineVar, 0, "a");
    LLVMValueRef a_x = LLVMBuildStructGEP2(builder, point, a, 0, "a_x");
    LLVMBuildStore(builder, LLVMConstInt(i32, 1, 0), a_x);

    // Access line.b.y (field 1 -> y = 1) - Using LLVMBuildStructGEP2
    LLVMValueRef b = LLVMBuildStructGEP2(builder, line, lineVar, 1, "b");
    LLVMValueRef b_y = LLVMBuildStructGEP2(builder, point, b, 1, "b_y");
    LLVMBuildStore(builder, LLVMConstInt(i32, 9, 0), b_y);

    LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));
    LLVMPrintModuleToFile(mod, "out.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}