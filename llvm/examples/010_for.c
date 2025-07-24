// headers
#include <llvm-c/Core.h>
#include <llvm/IR/...> // same as above

// pseudo code
// for(int i = 0; i < 5; i++) {
//    // do something
// }

int main() {
    LLVMContext context;
    Module module("for_loop", context);
    IRBuilder<> builder(context);

    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), false);
    Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);

    BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    AllocaInst *iVar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "i");
    builder.CreateStore(builder.getInt32(0), iVar);

    BasicBlock *condBlock = BasicBlock::Create(context, "for.cond", mainFunc);
    BasicBlock *bodyBlock = BasicBlock::Create(context, "for.body", mainFunc);
    BasicBlock *incBlock = BasicBlock::Create(context, "for.inc", mainFunc);
    BasicBlock *endBlock = BasicBlock::Create(context, "for.end", mainFunc);

    builder.CreateBr(condBlock);

    builder.SetInsertPoint(condBlock);
    Value *iVal = builder.CreateLoad(builder.getInt32Ty(), iVar, "i.load");
    Value *cond = builder.CreateICmpSLT(iVal, builder.getInt32(5), "cond");
    builder.CreateCondBr(cond, bodyBlock, endBlock);

    builder.SetInsertPoint(bodyBlock);
    // (no-op body)
    builder.CreateBr(incBlock);

    builder.SetInsertPoint(incBlock);
    iVal = builder.CreateLoad(builder.getInt32Ty(), iVar, "i.load");
    Value *inc = builder.CreateAdd(iVal, builder.getInt32(1), "i.inc");
    builder.CreateStore(inc, iVar);
    builder.CreateBr(condBlock);

    builder.SetInsertPoint(endBlock);
    builder.CreateRet(builder.getInt32(0));

    module.print(outs(), nullptr);
    return 0;
}
