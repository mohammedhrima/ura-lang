#include "llvm/Target/TargetMachine.h"
#include "llvm/MC/TargetRegistry.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>

using namespace llvm;

int main()
{
   // Initialize LLVM
   InitializeNativeTarget();
   InitializeNativeTargetAsmPrinter();

   LLVMContext Context;
   Module *TheModule = new Module("my_module", Context);
   IRBuilder<> Builder(Context);

   // Create main function
   FunctionType *FT = FunctionType::get(Type::getInt32Ty(Context), false);
   Function *MainFunc = Function::Create(FT, Function::ExternalLinkage, "main", TheModule);
   BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", MainFunc);
   Builder.SetInsertPoint(EntryBB);

   // Declare printf
   std::vector<Type *> printf_args;
   printf_args.push_back(Type::getInt8PtrTy(Context));
   FunctionType *printf_type = FunctionType::get(Builder.getInt32Ty(), printf_args, true);
   FunctionCallee PrintF = TheModule->getOrInsertFunction("printf", printf_type);

   // Create format strings
   Value *fmt_one = Builder.CreateGlobalStringPtr("one\n");
   Value *fmt_two = Builder.CreateGlobalStringPtr("two\n");
   Value *fmt_three = Builder.CreateGlobalStringPtr("three\n");

   // int a = 1;
   Value *a = Builder.getInt32(1);

   // Blocks for control flow
   BasicBlock *IfOne = BasicBlock::Create(Context, "if.one", MainFunc);
   BasicBlock *IfTwo = BasicBlock::Create(Context, "if.two", MainFunc);
   BasicBlock *IfElse = BasicBlock::Create(Context, "else", MainFunc);
   BasicBlock *After = BasicBlock::Create(Context, "after", MainFunc);

   // if (a == 1)
   Value *Cond1 = Builder.CreateICmpEQ(a, Builder.getInt32(1));
   Builder.CreateCondBr(Cond1, IfOne, IfTwo);

   // if.one:
   Builder.SetInsertPoint(IfOne);
   Builder.CreateCall(PrintF, fmt_one);
   Builder.CreateBr(After);

   // if.two:
   Builder.SetInsertPoint(IfTwo);
   Value *Cond2 = Builder.CreateICmpEQ(a, Builder.getInt32(2));
   Builder.CreateCondBr(Cond2, IfTwo = BasicBlock::Create(Context, "then.two", MainFunc), IfElse);

   // then.two:
   Builder.SetInsertPoint(IfTwo);
   Builder.CreateCall(PrintF, fmt_two);
   Builder.CreateBr(After);

   // else:
   Builder.SetInsertPoint(IfElse);
   Builder.CreateCall(PrintF, fmt_three);
   Builder.CreateBr(After);

   // after:
   Builder.SetInsertPoint(After);
   Builder.CreateRet(Builder.getInt32(0));

   // Print IR
   TheModule->print(outs(), nullptr);

   // Emit assembly
   auto TargetTriple = sys::getDefaultTargetTriple();
   std::string Error;
   const Target *Target = TargetRegistry::lookupTarget(TargetTriple, Error);

   TargetOptions opt;
   auto RM = std::optional<Reloc::Model>();
   TargetMachine *TM = Target->createTargetMachine(TargetTriple, "generic", "", opt, RM);

   TheModule->setDataLayout(TM->createDataLayout());
   TheModule->setTargetTriple(TargetTriple);

   std::error_code EC;
   raw_fd_ostream dest("output.s", EC, sys::fs::OF_None);

   legacy::PassManager pass;
   TM->addPassesToEmitFile(pass, dest, nullptr, CGFT_AssemblyFile);
   pass.run(*TheModule);
   dest.flush();

   delete TheModule;
   return 0;
}
