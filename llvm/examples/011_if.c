// headers
#include <llvm-c/Core.h>
#include <llvm/IR/...>

// pseudo code
// int a = 10;
// if (a > 5) {
//    a = a + 1;
// }

int main() {
    LLVMContext context;
    Module module("if_stmt", context);
    IRBuilder<> builder(context);

    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), false);
    Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);

    BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    AllocaInst *aVar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "a");
    builder.CreateStore(builder.getInt32(10), aVar);

    Value *aVal = builder.CreateLoad(builder.getInt32Ty(), aVar, "a.load");
    Value *cond = builder.CreateICmpSGT(aVal, builder.getInt32(5), "cond");

    BasicBlock *thenBB = BasicBlock::Create(context, "then", mainFunc);
    BasicBlock *mergeBB = BasicBlock::Create(context, "merge", mainFunc);
    builder.CreateCondBr(cond, thenBB, mergeBB);

    builder.SetInsertPoint(thenBB);
    aVal = builder.CreateLoad(builder.getInt32Ty(), aVar, "a.load");
    Value *inc = builder.CreateAdd(aVal, builder.getInt32(1), "a.inc");
    builder.CreateStore(inc, aVar);
    builder.CreateBr(mergeBB);

    builder.SetInsertPoint(mergeBB);
    builder.CreateRet(builder.getInt32(0));

    module.print(outs(), nullptr);
    return 0;
}
