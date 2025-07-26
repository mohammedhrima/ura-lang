#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

LLVMValueRef create_condition(LLVMBuilderRef builder, LLVMValueRef a, int value)
{
   LLVMValueRef cmpVal = LLVMConstInt(LLVMTypeOf(a), value, 0);
   return LLVMBuildICmp(builder, LLVMIntEQ, a, cmpVal, "cond");
}

int main()
{
   LLVMModuleRef module = LLVMModuleCreateWithName("if_else_chain");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMContextRef context = LLVMGetGlobalContext();

   LLVMTypeRef funcType = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
   LLVMValueRef func = LLVMAddFunction(module, "main", funcType);

   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   LLVMValueRef a = LLVMBuildAlloca(builder, LLVMInt32Type(), "a");
   LLVMValueRef aVal = LLVMBuildLoad2(builder, LLVMInt32Type(), a, "a_val");
   LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), 10, 0), aVal);
 
#if 0
   // ========================================================
   // if a == 1: do_something()
   LLVMBasicBlockRef ifCond = LLVMAppendBasicBlock(func, "ifcond");
   LLVMBasicBlockRef ifBody = LLVMAppendBasicBlock(func, "ifbody");
   LLVMBasicBlockRef endBlock = LLVMAppendBasicBlock(func, "end");

   LLVMBuildBr(builder, ifCond);
   LLVMPositionBuilderAtEnd(builder, ifCond);
   LLVMValueRef cond = create_condition(builder, a, 1);
   LLVMBuildCondBr(builder, cond, ifBody, endBlock);

   LLVMPositionBuilderAtEnd(builder, ifBody);
   // do_something()
   LLVMBuildBr(builder, endBlock);

   LLVMPositionBuilderAtEnd(builder, endBlock);
   // ========================================================
#elif 0
   // ========================================================
   // if a == 1: do1()
   // else: do2()
   LLVMBasicBlockRef ifCond = LLVMAppendBasicBlock(func, "ifcond");
   LLVMBasicBlockRef ifTrue = LLVMAppendBasicBlock(func, "iftrue");
   LLVMBasicBlockRef ifFalse = LLVMAppendBasicBlock(func, "iffalse");
   LLVMBasicBlockRef end = LLVMAppendBasicBlock(func, "end");

   LLVMBuildBr(builder, ifCond);
   LLVMPositionBuilderAtEnd(builder, ifCond);
   LLVMValueRef cond = create_condition(builder, a, 1);
   LLVMBuildCondBr(builder, cond, ifTrue, ifFalse);

   LLVMPositionBuilderAtEnd(builder, ifTrue);
   // do1()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, ifFalse);
   // do2()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, end);
   // ========================================================
#elif 0
   // ========================================================
   // if a == 1:   do1()
   // elif a == 2: do2()

   LLVMBasicBlockRef cond1 = LLVMAppendBasicBlock(func, "cond1");
   LLVMBasicBlockRef body1 = LLVMAppendBasicBlock(func, "body1");
   LLVMBasicBlockRef cond2 = LLVMAppendBasicBlock(func, "cond2");
   LLVMBasicBlockRef body2 = LLVMAppendBasicBlock(func, "body2");
   LLVMBasicBlockRef end = LLVMAppendBasicBlock(func, "end");

   LLVMBuildBr(builder, cond1);

   // First condition
   LLVMPositionBuilderAtEnd(builder, cond1);
   LLVMValueRef c1 = create_condition(builder, a, 1);
   LLVMBuildCondBr(builder, c1, body1, cond2);

   // Second condition
   LLVMPositionBuilderAtEnd(builder, cond2);
   LLVMValueRef c2 = create_condition(builder, a, 2);
   LLVMBuildCondBr(builder, c2, body2, end);

   LLVMPositionBuilderAtEnd(builder, body1);
   // do1()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, body2);
   // do2()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, end);
   // ========================================================
#elif 0
   // ========================================================
   // if a == 1:   do1()
   // elif a == 2: do2()
   // else: do3()
   LLVMBasicBlockRef cond1 = LLVMAppendBasicBlock(func, "cond1");
   LLVMBasicBlockRef body1 = LLVMAppendBasicBlock(func, "body1");
   LLVMBasicBlockRef cond2 = LLVMAppendBasicBlock(func, "cond2");
   LLVMBasicBlockRef body2 = LLVMAppendBasicBlock(func, "body2");
   LLVMBasicBlockRef elseBody = LLVMAppendBasicBlock(func, "else");
   LLVMBasicBlockRef end = LLVMAppendBasicBlock(func, "end");

   LLVMBuildBr(builder, cond1);

   LLVMPositionBuilderAtEnd(builder, cond1);
   LLVMValueRef c1 = create_condition(builder, a, 1);
   LLVMBuildCondBr(builder, c1, body1, cond2);

   LLVMPositionBuilderAtEnd(builder, cond2);
   LLVMValueRef c2 = create_condition(builder, a, 2);
   LLVMBuildCondBr(builder, c2, body2, elseBody);

   LLVMPositionBuilderAtEnd(builder, body1);
   // do1()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, body2);
   // do2()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, elseBody);
   // do3()
   LLVMBuildBr(builder, end);

   LLVMPositionBuilderAtEnd(builder, end);
   // ========================================================
#elif 1
   // ========================================================
   // if a == 1: do1()
   // elif a == 2: do2()
   // elif a == 3: do3()
   // else: do4()
   LLVMBasicBlockRef cond1 = LLVMAppendBasicBlock(func, "cond1");
   LLVMBasicBlockRef body1 = LLVMAppendBasicBlock(func, "body1");

   LLVMBasicBlockRef cond2 = LLVMAppendBasicBlock(func, "cond2");
   LLVMBasicBlockRef body2 = LLVMAppendBasicBlock(func, "body2");

   LLVMBasicBlockRef cond3 = LLVMAppendBasicBlock(func, "cond3");
   LLVMBasicBlockRef body3 = LLVMAppendBasicBlock(func, "body3");

   LLVMBasicBlockRef elseBody = LLVMAppendBasicBlock(func, "else");
   LLVMBasicBlockRef end = LLVMAppendBasicBlock(func, "end");

   // cond1
   LLVMBuildBr(builder, cond1);

   // cond1: if (a == 1) -> body1 else -> cond2
   LLVMPositionBuilderAtEnd(builder, cond1);
   LLVMValueRef c1 = create_condition(builder, a, 1);
   LLVMBuildCondBr(builder, c1, body1, cond2);

   // cond2: if (a == 2) -> body2 else -> cond3
   LLVMPositionBuilderAtEnd(builder, cond2);
   LLVMValueRef c2 = create_condition(builder, a, 2);
   LLVMBuildCondBr(builder, c2, body2, cond3);

   // cond3: if (a == 3) -> body3 else -> elseBody
   LLVMPositionBuilderAtEnd(builder, cond3);
   LLVMValueRef c3 = create_condition(builder, a, 3);
   LLVMBuildCondBr(builder, c3, body3, elseBody);

   // body1: do1(); -> end
   LLVMPositionBuilderAtEnd(builder, body1);
   // insert do1()
   LLVMBuildBr(builder, end);

   // body2: do2(); -> end
   LLVMPositionBuilderAtEnd(builder, body2);
   // insert do2()
   LLVMBuildBr(builder, end);

   // body3: do3(); -> end
   LLVMPositionBuilderAtEnd(builder, body3);
   // insert do3()
   LLVMBuildBr(builder, end);

   // else: do4(); -> end
   LLVMPositionBuilderAtEnd(builder, elseBody);
   // insert do4()
   LLVMBuildBr(builder, end);

   // continue from end
   // LLVMPositionBuilderAtEnd(builder, end);
   // ========================================================
#endif
   // ========================================================

   // ========================================================

   LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(module, "out.ir", NULL);

   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
}