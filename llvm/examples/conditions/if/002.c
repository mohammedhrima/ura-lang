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
    LLVMValueRef value = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, right->name) : right->content;
    LLVMBuildStore(llvm.builder, value, left->content);
}

void LLVMreturn(LLVMEntity *entity)
{
    LLVMValueRef value = entity->name ? LLVMBuildLoad2(llvm.builder, int32, entity->content, "ret") : entity->content;
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
    LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(llvm.builder, int32, left->content, "") : left->content;
    LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, "") : right->content;

    LLVMIntPredicate pr;
    if (strcmp(op, ">") == 0) pr = LLVMIntSGT;
    else if (strcmp(op, "==") == 0) pr = LLVMIntEQ;
    else pr = LLVMIntNE; // fallback

    LLVMEntity res = {.type = INT, .content = LLVMBuildICmp(llvm.builder, pr, leftRef, rightRef, "cond")};
    return res;
}

LLVMEntity LLVMmath(LLVMEntity *left, char op, LLVMEntity *right)
{
    LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(llvm.builder, int32, left->content, "") : left->content;
    LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, "") : right->content;

    LLVMEntity res = {};
    switch(op)
    {
        case '+': res.content = LLVMBuildAdd(llvm.builder, leftRef, rightRef, "+"); break;
        default: break;
    }
    return res;
}

int main()
{
    init("if_else_chain");
    LLVMFunction func = new_function("main", INT);

    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const1 = new_constant(1);
    LLVMEntity const2 = new_constant(2);
    LLVMEntity const3 = new_constant(3);
    LLVMEntity const4 = new_constant(4);
    LLVMEntity const5 = new_constant(5);
    LLVMEntity const6 = new_constant(6);
    LLVMEntity const10 = new_constant(10);

    LLVMassign(&a, &const1);

    LLVMStatement if1 = {
        .cond = LLVMcompare(&a, ">", &const2),
        .start = LLVMAppendBasicBlock(func.ref, "then1"),
        .end = LLVMAppendBasicBlock(func.ref, "cond2"),
    };

    LLVMBuildCondBr(llvm.builder, if1.cond.content, if1.start, if1.end);

    LLVMPositionBuilderAtEnd(llvm.builder, if1.start);
    LLVMEntity add1 = LLVMmath(&a, '+', &const3);
    LLVMassign(&a, &add1);
    LLVMBasicBlockRef merge = LLVMAppendBasicBlock(func.ref, "merge");
    LLVMBuildBr(llvm.builder, merge);

    LLVMPositionBuilderAtEnd(llvm.builder, if1.end);
    LLVMEntity cond2 = LLVMcompare(&a, "==", &const4);
    LLVMBasicBlockRef then2 = LLVMAppendBasicBlock(func.ref, "then2");
    LLVMBasicBlockRef else2 = LLVMAppendBasicBlock(func.ref, "else");
    LLVMBuildCondBr(llvm.builder, cond2.content, then2, else2);

    LLVMPositionBuilderAtEnd(llvm.builder, then2);
    LLVMEntity add2 = LLVMmath(&a, '+', &const5);
    LLVMassign(&a, &add2);
    LLVMBuildBr(llvm.builder, merge);

    LLVMPositionBuilderAtEnd(llvm.builder, else2);
    LLVMEntity add3 = LLVMmath(&a, '+', &const6);
    LLVMassign(&a, &add3);
    LLVMBuildBr(llvm.builder, merge);

    LLVMPositionBuilderAtEnd(llvm.builder, merge);
    LLVMreturn(&a);
    LLVMsaveToFile("out.ir");

    return 0;
}
