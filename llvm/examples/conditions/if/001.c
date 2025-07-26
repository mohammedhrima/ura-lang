#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Type Type;
typedef struct LLVM LLVM;
typedef struct LLVMFunction LLVMFunction;
typedef struct LLVMEntity LLVMEntity;
typedef struct LLVMStatement LLVMStatement;

LLVMTypeRef int32;
LLVM llvm;

enum Type
{
    INT = 1
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

struct LLVMEntity
{
    Type type;
    char *name;
    LLVMValueRef content;
};

struct LLVMStatement
{
    LLVMBasicBlockRef start;
    LLVMBasicBlockRef end;
    LLVMEntity cond;
};

void init(char *name)
{
    llvm.module = LLVMModuleCreateWithName(name);
    llvm.builder = LLVMCreateBuilder();
    llvm.context = LLVMGetGlobalContext();

    int32 = LLVMInt32Type();
}

LLVMFunction new_function(char *name, Type ret)
{
    LLVMFunction res = {};
    if (ret == INT) res.setup = LLVMFunctionType(int32, NULL, 0, 0);
    res.ref = LLVMAddFunction(llvm.module, name, res.setup);
    res.bloc = LLVMAppendBasicBlock(res.ref, "entry");
    LLVMPositionBuilderAtEnd(llvm.builder, res.bloc);
    return res;
}

LLVMEntity new_variable(char *name, Type type)
{
    LLVMEntity res = {.type = type, .name = name};
    if (type == INT) res.content = LLVMBuildAlloca(llvm.builder, int32, name);
    return res;
}

LLVMEntity new_constant(int value)
{
    LLVMEntity res = {.type = INT, .content = LLVMConstInt(int32, value, 0)};
    return res;
}

void LLVMassign(LLVMEntity *left, LLVMEntity *right)
{
    LLVMValueRef value;

    if (right->name) value = LLVMBuildLoad2(llvm.builder, int32, right->content, right->name);
    else value = right->content;
    LLVMBuildStore(llvm.builder, value, left->content);
}

void LLVMreturn(LLVMEntity *entity)
{
    LLVMValueRef value;

    if (entity->name) value = LLVMBuildLoad2(llvm.builder, int32, entity->content, "ret");
    else value = entity->content;
    LLVMBuildRet(llvm.builder, value);
}

void LLVMsaveToFile(char *outfile)
{
    LLVMVerifyModule(llvm.module, LLVMAbortProcessAction, NULL);
    LLVMPrintModuleToFile(llvm.module, outfile, NULL);
    LLVMDisposeBuilder(llvm.builder);
    LLVMDisposeModule(llvm.module);
}

LLVMEntity LLVMcompare(LLVMEntity *left, char *op, LLVMEntity *right)
{
    LLVMValueRef leftRef, rightRef;
    LLVMEntity res = {};

    if (left->name) leftRef = LLVMBuildLoad2(llvm.builder, int32, left->content, "");
    else leftRef = left->content;

    if (right->name) rightRef = LLVMBuildLoad2(llvm.builder, int32, right->content, "");
    else rightRef = right->content;

    LLVMIntPredicate pr;
    struct { LLVMIntPredicate key; char *value; } arr[] = {
        {LLVMIntEQ, "=="}, {LLVMIntNE, "!="}, {LLVMIntUGT, ">"},
        {LLVMIntUGE, ">="}, {LLVMIntULT, "<"}, {LLVMIntULE, "<="},
        {0, NULL},
    };
    for(int i = 0; arr[i].value; i++)
    {
        if(strcmp(arr[i].value, op) == 0)
        {
            res.content = LLVMBuildICmp(llvm.builder, LLVMIntSGT, leftRef, rightRef, "cond");
            return res;
        }
    }
    perror("in compare");
    exit(1);
    return res;
}

LLVMEntity LLVMmath(LLVMEntity *left, char op, LLVMEntity *right)
{
    LLVMValueRef leftRef, rightRef;
    LLVMEntity res = {};

    if (left->name) leftRef = LLVMBuildLoad2(llvm.builder, int32, left->content, "");
    else leftRef = left->content;

    if (right->name) rightRef = LLVMBuildLoad2(llvm.builder, int32, right->content, "");
    else rightRef = right->content;
    
    switch(op)
    {
        case '+': res.content = LLVMBuildAdd(llvm.builder, leftRef, rightRef, "+"); break;
        default: break;
    }
    return res;
}

// =====================================================
/*
main():
    int a = 10
    if a > 5: a = a + 1
    return a
*/
// =====================================================
int main()
{
    init("module");

    LLVMFunction func = new_function("main", INT);

    // int a = 10;
    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const1 = new_constant(1);
    LLVMEntity const2 = new_constant(2);
    LLVMEntity const3 = new_constant(3);
    LLVMEntity const4 = new_constant(4);
    LLVMEntity const5 = new_constant(5);
    LLVMEntity const10 = new_constant(10);
    
    LLVMassign(&a, &const10);

    // Build: if (a > 5)
    LLVMStatement state = {
        .cond = LLVMcompare(&a, ">", &const5),
        .start = LLVMAppendBasicBlock(func.ref, "then"),
        .end = LLVMAppendBasicBlock(func.ref, "merge"),
    };

    LLVMBuildCondBr(llvm.builder, state.cond.content, state.start, state.end);

    // start block
    LLVMPositionBuilderAtEnd(llvm.builder, state.start);
    
    // a = a + 1
    LLVMEntity add = LLVMmath(&a, '+', &const1);
    LLVMassign(&a, &add);
    
    // end block
    LLVMBuildBr(llvm.builder, state.end);

    LLVMPositionBuilderAtEnd(llvm.builder, state.end);
    
    // return a
    LLVMreturn(&a);

    // Save IR
    LLVMsaveToFile("out.ir");

    return 0;
}
