#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode:
/*
int sum(int count, ...)
{
   va_list args;
   va_start(args, count);

   int total = 0;
   int i = 0;
   while (i < count)
   {
      total += va_arg(args, int);
      i++;
   }
   va_end(args);
   return total;
}

int main()
{
   int result = sum(3, 10, 20, 30);
   return result;
}
*/

int main()
{
   LLVMModuleRef module = LLVMModuleCreateWithName("variadic_module");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMContextRef context = LLVMGetGlobalContext();

   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef int8Type = LLVMInt8Type();
   LLVMTypeRef int8PtrType = LLVMPointerType(int8Type, 0);
   LLVMTypeRef voidType = LLVMVoidType();

   // Declare va_start and va_end intrinsics
   LLVMTypeRef vaStartParams[] = {int8PtrType};
   LLVMTypeRef vaStartType = LLVMFunctionType(voidType, vaStartParams, 1, 0);
   LLVMValueRef vaStartFunc = LLVMAddFunction(module, "llvm.va_start.p0", vaStartType);

   LLVMTypeRef vaEndParams[] = {int8PtrType};
   LLVMTypeRef vaEndType = LLVMFunctionType(voidType, vaEndParams, 1, 0);
   LLVMValueRef vaEndFunc = LLVMAddFunction(module, "llvm.va_end.p0", vaEndType);

   // Create the variadic sum function: int sum(int count, ...)
   LLVMTypeRef sumParams[] = {int32Type};
   LLVMTypeRef sumType = LLVMFunctionType(int32Type, sumParams, 1, 1); // 1 = variadic
   LLVMValueRef sumFunc = LLVMAddFunction(module, "sum", sumType);

   LLVMValueRef countParam = LLVMGetParam(sumFunc, 0);
   LLVMSetValueName(countParam, "count");

   // Create basic blocks for the sum function
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sumFunc, "entry");
   LLVMBasicBlockRef loopHeader = LLVMAppendBasicBlock(sumFunc, "loop_header");
   LLVMBasicBlockRef loopBody = LLVMAppendBasicBlock(sumFunc, "loop_body");
   LLVMBasicBlockRef loopExit = LLVMAppendBasicBlock(sumFunc, "loop_exit");

   // Entry block
   LLVMPositionBuilderAtEnd(builder, entry);

   // va_list args; (allocate space for va_list)
   LLVMTypeRef vaListType = LLVMArrayType(int8Type, 24); // 24 bytes should be enough for most platforms
   LLVMValueRef argsAlloca = LLVMBuildAlloca(builder, vaListType, "args");

   // int total = 0;
   LLVMValueRef totalAlloca = LLVMBuildAlloca(builder, int32Type, "total");
   LLVMBuildStore(builder, LLVMConstInt(int32Type, 0, 0), totalAlloca);

   // int i = 0;
   LLVMValueRef iAlloca = LLVMBuildAlloca(builder, int32Type, "i");
   LLVMBuildStore(builder, LLVMConstInt(int32Type, 0, 0), iAlloca);

   // va_start(args, count)
   LLVMValueRef argsI8Ptr = LLVMBuildBitCast(builder, argsAlloca, int8PtrType, "args_i8");
   LLVMValueRef vaStartArgs[] = {argsI8Ptr};
   LLVMBuildCall2(builder, vaStartType, vaStartFunc, vaStartArgs, 1, "");

   LLVMBuildBr(builder, loopHeader);

   // Loop header: while (i < count)
   LLVMPositionBuilderAtEnd(builder, loopHeader);
   LLVMValueRef iVal = LLVMBuildLoad2(builder, int32Type, iAlloca, "i_val");
   LLVMValueRef cmp = LLVMBuildICmp(builder, LLVMIntSLT, iVal, countParam, "cmp");
   LLVMBuildCondBr(builder, cmp, loopBody, loopExit);

   // Loop body
   LLVMPositionBuilderAtEnd(builder, loopBody);

   // Simplified va_arg implementation: total += va_arg(args, int)
   // This is a simplified approach - real va_arg is platform-specific
   LLVMValueRef indices[] = {
       LLVMConstInt(int32Type, 0, 0),
       LLVMBuildMul(builder, iVal, LLVMConstInt(int32Type, 8, 0), "offset") // 8 bytes per arg
   };
   LLVMValueRef argPtr = LLVMBuildInBoundsGEP2(builder, vaListType, argsAlloca, indices, 2, "arg_ptr");
   LLVMValueRef argPtrCast = LLVMBuildBitCast(builder, argPtr, LLVMPointerType(int32Type, 0), "arg_ptr_cast");
   LLVMValueRef argVal = LLVMBuildLoad2(builder, int32Type, argPtrCast, "arg_val");

   LLVMValueRef totalVal = LLVMBuildLoad2(builder, int32Type, totalAlloca, "total_val");
   LLVMValueRef newTotal = LLVMBuildAdd(builder, totalVal, argVal, "new_total");
   LLVMBuildStore(builder, newTotal, totalAlloca);

   // i++
   LLVMValueRef newI = LLVMBuildAdd(builder, iVal, LLVMConstInt(int32Type, 1, 0), "new_i");
   LLVMBuildStore(builder, newI, iAlloca);

   LLVMBuildBr(builder, loopHeader);

   // Loop exit
   LLVMPositionBuilderAtEnd(builder, loopExit);

   // va_end(args)
   LLVMValueRef vaEndArgs[] = {argsI8Ptr};
   LLVMBuildCall2(builder, vaEndType, vaEndFunc, vaEndArgs, 1, "");

   // return total;
   LLVMValueRef finalTotal = LLVMBuildLoad2(builder, int32Type, totalAlloca, "final_total");
   LLVMBuildRet(builder, finalTotal);

   // Create main function: int main()
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(module, "main", mainType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // int result = sum(3, 10, 20, 30);
   LLVMValueRef sumArgs[] = {
       LLVMConstInt(int32Type, 3, 0),  // count
       LLVMConstInt(int32Type, 10, 0), // first arg
       LLVMConstInt(int32Type, 20, 0), // second arg
       LLVMConstInt(int32Type, 30, 0)  // third arg
   };
   LLVMValueRef result = LLVMBuildCall2(builder, sumType, sumFunc, sumArgs, 4, "result");

   // return result;
   LLVMBuildRet(builder, result);

   // Output LLVM IR
   LLVMPrintModuleToFile(module, "out.ll", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);

   return 0;
}