// headers
#include <llvm-c/Core.h>
#include <llvm/IR/...>

// pseudo code
// int x = 2;
// switch(x) {
//   case 1: break;
//   case 2: break;
//   default: break;
// }

int main() {
    LLVMContext context;
    Module module("switch_case", context);
    IRBuilder<> builder(context);

    FunctionType *funcType = FunctionType::get(builder.getInt32Ty(), false);
    Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);

    BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    AllocaInst *xVar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "x");
    builder.CreateStore(builder.getInt32(2), xVar);
    Value *xVal = builder.CreateLoad(builder.getInt32Ty(), xVar, "x.load");

    BasicBlock *case1 = BasicBlock::Create(context, "case.1", mainFunc);
    BasicBlock *case2 = BasicBlock::Create(context, "case.2", mainFunc);
    BasicBlock *defaultCase = BasicBlock::Create(context, "default", mainFunc);
    BasicBlock *endSwitch = BasicBlock::Create(context, "end", mainFunc);

    SwitchInst *switchInst = builder.CreateSwitch(xVal, defaultCase, 2);
    switchInst->addCase(builder.getInt32(1), case1);
    switchInst->addCase(builder.getInt32(2), case2);

    builder.SetInsertPoint(case1);
    builder.CreateBr(endSwitch);

    builder.SetInsertPoint(case2);
    builder.CreateBr(endSwitch);

    builder.SetInsertPoint(defaultCase);
    builder.CreateBr(endSwitch);

    builder.SetInsertPoint(endSwitch);
    builder.CreateRet(builder.getInt32(0));

    module.print(outs(), nullptr);
    return 0;
}
