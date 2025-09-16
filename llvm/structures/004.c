/*
This example allocates two `Point` structs.
It sets fields in one and performs a memory copy to assign it to another.
Fixed for LLVM 20+ using LLVMBuildStructGEP2 and updated intrinsic declarations.
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

    // src.x = 5 - Using LLVMBuildStructGEP2
    LLVMValueRef x = LLVMBuildStructGEP2(builder, point, src, 0, "src_x");
    LLVMBuildStore(builder, LLVMConstInt(i32, 5, 0), x);

    // src.y = 7 - Using LLVMBuildStructGEP2
    LLVMValueRef y = LLVMBuildStructGEP2(builder, point, src, 1, "src_y");
    LLVMBuildStore(builder, LLVMConstInt(i32, 7, 0), y);

    // memcpy(dst, src, sizeof(Point)) (via llvm.memcpy)
    // In LLVM 20+, we use opaque pointers, so the intrinsic name is simplified
    LLVMTypeRef voidTy = LLVMVoidType();
    LLVMTypeRef ptrTy = LLVMPointerType(LLVMInt8Type(), 0);
    LLVMTypeRef memcpyParamTypes[] = {
        ptrTy,           // dst pointer
        ptrTy,           // src pointer
        LLVMInt64Type(), // size
        LLVMInt1Type()   // is_volatile
    };
    LLVMTypeRef memcpyType = LLVMFunctionType(voidTy, memcpyParamTypes, 4, 0);
    
    // For LLVM 20+, the intrinsic name might be simplified due to opaque pointers
    LLVMValueRef memcpyFn = LLVMAddFunction(mod, "llvm.memcpy.p0.p0.i64", memcpyType);

    // Cast pointers to i8* for memcpy
    LLVMValueRef dstCast = LLVMBuildBitCast(builder, dst, ptrTy, "dst8");
    LLVMValueRef srcCast = LLVMBuildBitCast(builder, src, ptrTy, "src8");
    
    // Calculate size of Point struct - Create target data from module's data layout
    const char* dataLayoutStr = LLVMGetDataLayout(mod);
    LLVMTargetDataRef targetData = LLVMCreateTargetData(dataLayoutStr);
    unsigned long long pointSize = LLVMABISizeOfType(targetData, point);
    LLVMValueRef size = LLVMConstInt(LLVMInt64Type(), pointSize, 0);
    LLVMDisposeTargetData(targetData);

    // Call memcpy with is_volatile = false
    LLVMValueRef args[] = { 
        dstCast, 
        srcCast, 
        size, 
        LLVMConstInt(LLVMInt1Type(), 0, 0) // is_volatile = false
    };
    LLVMBuildCall2(builder, memcpyType, memcpyFn, args, 4, "");

    LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));
    LLVMPrintModuleToFile(mod, "out.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    return 0;
}