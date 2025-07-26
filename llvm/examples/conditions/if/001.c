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
    llvm.context = LLVMContextCreate();
    llvm.module = LLVMModuleCreateWithNameInContext(name, llvm.context);
    llvm.builder = LLVMCreateBuilderInContext(llvm.context);
    int32 = LLVMInt32TypeInContext(llvm.context);
}

LLVMFunction new_function(char *name, Type ret)
{
    LLVMFunction res = {};
    if (ret == INT)
        res.setup = LLVMFunctionType(int32, NULL, 0, 0);
    res.ref = LLVMAddFunction(llvm.module, name, res.setup);
    res.bloc = LLVMAppendBasicBlockInContext(llvm.context, res.ref, "entry");
    LLVMPositionBuilderAtEnd(llvm.builder, res.bloc);
    return res;
}

LLVMEntity new_variable(char *name, Type type)
{
    LLVMEntity res = {.type = type, .name = name};
    if (type == INT)
        res.content = LLVMBuildAlloca(llvm.builder, int32, name);
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
    LLVMContextDispose(llvm.context);
}

LLVMEntity LLVMcompare(LLVMEntity *left, char *op, LLVMEntity *right)
{
    LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(llvm.builder, int32, left->content, "") : left->content;
    LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, "") : right->content;

    LLVMIntPredicate pr;
    if (strcmp(op, ">") == 0)
        pr = LLVMIntSGT;
    else if (strcmp(op, ">=") == 0)
        pr = LLVMIntSGE;
    else if (strcmp(op, "<") == 0)
        pr = LLVMIntSLT;
    else if (strcmp(op, "<=") == 0)
        pr = LLVMIntSLE;
    else if (strcmp(op, "==") == 0)
        pr = LLVMIntEQ;
    else if (strcmp(op, "!=") == 0)
        pr = LLVMIntNE;
    else
        pr = LLVMIntNE;

    LLVMEntity res = {.type = INT, .content = LLVMBuildICmp(llvm.builder, pr, leftRef, rightRef, "cond")};
    return res;
}

LLVMEntity LLVMmath(LLVMEntity *left, char op, LLVMEntity *right)
{
    LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(llvm.builder, int32, left->content, "") : left->content;
    LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, "") : right->content;

    LLVMEntity res = {};
    switch (op)
    {
    case '+':
        res.content = LLVMBuildAdd(llvm.builder, leftRef, rightRef, "+");
        break;
    case '-':
        res.content = LLVMBuildSub(llvm.builder, leftRef, rightRef, "-");
        break;
    case '*':
        res.content = LLVMBuildMul(llvm.builder, leftRef, rightRef, "*");
        break;
    case '/':
        res.content = LLVMBuildSDiv(llvm.builder, leftRef, rightRef, "/");
        break;
    default:
        break;
    }
    return res;
}

// Simple if statement abstraction
LLVMStatement create_simple_if(LLVMFunction *func, LLVMEntity condition, char *label)
{
    char then_name[64], end_name[64];
    snprintf(then_name, sizeof(then_name), "then_%s", label);
    snprintf(end_name, sizeof(end_name), "end_%s", label);

    LLVMStatement stmt = {
        .cond = condition,
        .start = LLVMAppendBasicBlockInContext(llvm.context, func->ref, then_name),
        .end = LLVMAppendBasicBlockInContext(llvm.context, func->ref, end_name)};

    LLVMBuildCondBr(llvm.builder, condition.content, stmt.start, stmt.end);
    return stmt;
}

void enter_if_then(LLVMStatement *stmt)
{
    LLVMPositionBuilderAtEnd(llvm.builder, stmt->start);
}

void exit_if_then(LLVMStatement *stmt)
{
    LLVMBuildBr(llvm.builder, stmt->end);
    LLVMPositionBuilderAtEnd(llvm.builder, stmt->end);
}

/*
Simple If Example:
main():
    int a = 10
    if (a > 5) {
        a = a + 1  // 10 + 1 = 11
    }
    return a  // returns 11
*/

int main()
{
    init("simple_if_example");
    LLVMFunction func = new_function("main", INT);

    // Variables and constants
    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const10 = new_constant(10);
    LLVMEntity const5 = new_constant(5);
    LLVMEntity const1 = new_constant(1);

    // a = 10
    LLVMassign(&a, &const10);

    // if (a > 5) { a = a + 1 }
    LLVMEntity condition = LLVMcompare(&a, ">", &const5);
    LLVMStatement if_stmt = create_simple_if(&func, condition, "simple");

    enter_if_then(&if_stmt);
    LLVMEntity result = LLVMmath(&a, '+', &const1);
    LLVMassign(&a, &result);
    exit_if_then(&if_stmt);

    // return a
    LLVMreturn(&a);
    LLVMsaveToFile("out.ir");

    printf("Simple if statement IR generated: simple_if.ir\n");
    printf("Expected result: 11 (10 > 5 is true, so a = 10 + 1 = 11)\n");

    return 0;
}