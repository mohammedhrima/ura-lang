/*
This example allocates two `Point` structs.
It sets fields in one and performs a memory copy to assign it to another.
*/

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/ExecutionEngine.h>
#include <stdio.h>

int main() {
    LLVMModuleRef mod = LLVMModuleCreateWithName("struct_assign");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef i32 = LLVMInt32Type();

    // Define Point struct
    LLVMTypeRef point = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Point");
    LLVMTypeRef fields[] = { i32, i32 };
    LLVMStructSetBody(point, fields, 2, 0);

    // Create function main
    LLVMTypeRef mainType = LLVMFunctionType(i32, NULL, 0, 0);
    LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef src = LLVMBuildAlloca(builder, point, "src");
    LLVMValueRef dst = LLVMBuildAlloca(builder, point, "dst");

    // src.x = 5
    LLVMValueRef x = LLVMBuildStructGEP(builder, src, 0, "src_x");
    LLVMBuildStore(builder, LLVMConstInt(i32, 5, 0), x);

    // src.y = 7
    LLVMValueRef y = LLVMBuildStructGEP(builder, src, 1, "src_y");
    LLVMBuildStore(builder, LLVMConstInt(i32, 7, 0), y);

    // memcpy(dst, src, sizeof(Point)) (via llvm.memcpy)
    LLVMTypeRef voidTy = LLVMVoidType();
    LLVMTypeRef memcpyParamTypes[] = {
        LLVMPointerType(LLVMInt8Type(), 0),
        LLVMPointerType(LLVMInt8Type(), 0),
        LLVMInt64Type(), LLVMInt1Type()
    };
    LLVMTypeRef memcpyType = LLVMFunctionType(voidTy, memcpyParamTypes, 4, 0);
    LLVMValueRef memcpyFn = LLVMAddFunction(mod, "llvm.memcpy.p0i8.p0i8.i64", memcpyType);

    LLVMValueRef dstCast = LLVMBuildBitCast(builder, dst, LLVMPointerType(LLVMInt8Type(), 0), "dst8");
    LLVMValueRef srcCast = LLVMBuildBitCast(builder, src, LLVMPointerType(LLVMInt8Type(), 0), "src8");
    LLVMValueRef size = LLVMConstInt(LLVMInt64Type(), LLVMABISizeOfType(LLVMGetDataLayout(mod), point), 0);

    LLVMValueRef args[] = { dstCast, srcCast, size, LLVMConstInt(LLVMInt1Type(), 0, 0) };
    LLVMBuildCall2(builder, memcpyType, memcpyFn, args, 4, "");

    LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));
    LLVMPrintModuleToFile(mod, "struct3.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}
