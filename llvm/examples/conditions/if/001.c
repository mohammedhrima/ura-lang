#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

typedef enum Type Type;
typedef struct LLVM LLVM;
typedef struct LLVMFunction LLVMFunction;
typedef struct LLVMVariable LLVMVariable;
typedef struct LLVMConst LLVMConst;

LLVMTypeRef int32;

enum Type
{
    INT = 1,
};

struct LLVM
{
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMContextRef context;
};

struct LLVMFunction
{
    LLVMTypeRef retType;
    LLVMTypeRef setup;
    LLVMValueRef ref;
    LLVMBasicBlockRef bloc;
};

struct LLVMVariable
{
    LLVMValueRef ptr;
};

struct LLVMConst
{
    LLVMValueRef value;
};

LLVM init(char *name)
{
    LLVM res = {};
    res.module = LLVMModuleCreateWithName(name);
    res.builder = LLVMCreateBuilder();
    res.context = LLVMGetGlobalContext();
    return res;
}

LLVMFunction new_function(LLVM *llvm, char *name, Type ret)
{
    LLVMFunction res = {};

    switch (ret)
    {
    case INT:
        res.setup = LLVMFunctionType(int32, NULL, 0, 0);
        break;
    default:
        break;
    }
    res.ref = LLVMAddFunction(llvm->module, name, res.setup);
    res.bloc = LLVMAppendBasicBlock(res.ref, "bloc");
    LLVMPositionBuilderAtEnd(llvm->builder, res.bloc);

    return res;
}

LLVMVariable new_variable(LLVM *llvm, char *name, Type type)
{
    LLVMVariable res;
    switch (type)
    {
    case INT:
        res.ptr = LLVMBuildAlloca(llvm->builder, int32, name);
    default:
        break;
    }
    return res;
}

LLVMConst new_constant(int value)
{
    LLVMConst res = {.value = LLVMConstInt(int32, value, 0)};
    return res;
}

void LLVMassign(LLVM *llvm, LLVMVariable *var, LLVMConst *val)
{
    LLVMBuildStore(llvm->builder, val->value, var->ptr);
}

void LLVMreturnFromVariable(LLVM *llvm, LLVMVariable *v)
{
    LLVMValueRef ret_val = LLVMBuildLoad2(llvm->builder, int32, v->ptr, "ret_val");
    LLVMBuildRet(llvm->builder, ret_val);
}

void LLVMreturnFromConst(LLVM *llvm, LLVMConst *val)
{
    LLVMBuildRet(llvm->builder, val->value);
}

void LLVMsaveToFile(LLVM *llvm, char *outfile)
{
    LLVMPrintModuleToFile(llvm->module, outfile, NULL);

    LLVMDisposeBuilder(llvm->builder);
    LLVMDisposeModule(llvm->module);
}

// int a = 10;
// if a > 5: a = a + 1
int main()
{
    LLVM llvm = init("module");
    int32 = LLVMInt32Type();

    // define int main()
    LLVMFunction func = new_function(&llvm, "main", INT);
    LLVMVariable var1 = new_variable(&llvm, "a", INT);

    LLVMConst const1 = new_constant(1);
    LLVMConst const2 = new_constant(2);
    LLVMConst const3 = new_constant(3);

    LLVMassign(&llvm, &var1, &const3);

    // LLVMreturnFromConst(&llvm, &const1);
    LLVMreturnFromVariable(&llvm, &var1);

    LLVMsaveToFile(&llvm, "out.ir");
    return 0;
}
