// headers
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/BitWriter.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
using namespace llvm;

// pseudo code
// int i = 0;
// while(i < 5) {
//    i = i + 1;
// }

int main() {
    LLVMContext context;
    Module module("while_loop", context);
    IRBuilder<> builder(context);

    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), false);
    Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);

    BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    AllocaInst *iVar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "i");
    builder.CreateStore(builder.getInt32(0), iVar);

    BasicBlock *loopCond = BasicBlock::Create(context, "loop.cond", mainFunc);
    BasicBlock *loopBody = BasicBlock::Create(context, "loop.body", mainFunc);
    BasicBlock *loopEnd = BasicBlock::Create(context, "loop.end", mainFunc);
    builder.CreateBr(loopCond);

    builder.SetInsertPoint(loopCond);
    Value *iVal = builder.CreateLoad(builder.getInt32Ty(), iVar, "i.load");
    Value *cond = builder.CreateICmpSLT(iVal, builder.getInt32(5), "cond");
    builder.CreateCondBr(cond, loopBody, loopEnd);

    builder.SetInsertPoint(loopBody);
    Value *inc = builder.CreateAdd(iVal, builder.getInt32(1), "i.add");
    builder.CreateStore(inc, iVar);
    builder.CreateBr(loopCond);

    builder.SetInsertPoint(loopEnd);
    builder.CreateRet(builder.getInt32(0));

    module.print(outs(), nullptr);
    return 0;
}
