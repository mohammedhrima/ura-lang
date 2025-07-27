#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type definitions
typedef enum Type Type;
typedef struct LLVM LLVM;
typedef struct LLVMFunction LLVMFunction;
typedef struct LLVMEntity LLVMEntity;

// Global variables
LLVMTypeRef int32;
LLVM llvm;

// Enums and structures
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

typedef struct
{
    LLVMBasicBlockRef then_block;
    LLVMBasicBlockRef else_block;
    LLVMBasicBlockRef merge_block;
} LLVMIfElse;

// Utility functions
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
    if (strcmp(op, ">") == 0)       pr = LLVMIntSGT;
    else if (strcmp(op, ">=") == 0) pr = LLVMIntSGE;
    else if (strcmp(op, "<") == 0)  pr = LLVMIntSLT;
    else if (strcmp(op, "<=") == 0) pr = LLVMIntSLE;
    else if (strcmp(op, "==") == 0) pr = LLVMIntEQ;
    else if (strcmp(op, "!=") == 0) pr = LLVMIntNE;
    else pr = LLVMIntNE;

    LLVMEntity res = {.type = INT, .content = LLVMBuildICmp(llvm.builder, pr, leftRef, rightRef, "cond")};
    return res;
}

// Control flow functions needed for this example
LLVMIfElse create_if_else(LLVMFunction *func, LLVMEntity condition, char *label)
{
    char then_name[64], else_name[64], merge_name[64];
    snprintf(then_name, sizeof(then_name), "then_%s", label);
    snprintf(else_name, sizeof(else_name), "else_%s", label);
    snprintf(merge_name, sizeof(merge_name), "merge_%s", label);

    LLVMIfElse if_else = {
        .then_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, then_name),
        .else_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, else_name),
        .merge_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, merge_name)};

    LLVMBuildCondBr(llvm.builder, condition.content, if_else.then_block, if_else.else_block);
    return if_else;
}

void enter_if_then_else(LLVMIfElse *if_else, int is_else)
{
    LLVMPositionBuilderAtEnd(llvm.builder, is_else ? if_else->else_block : if_else->then_block);
}

void exit_if_else_branch(LLVMIfElse *if_else)
{
    LLVMBuildBr(llvm.builder, if_else->merge_block);
}

void finalize_if_else(LLVMIfElse *if_else)
{
    LLVMPositionBuilderAtEnd(llvm.builder, if_else->merge_block);
}

/*
PSEUDOCODE for if/else statement:
BEGIN
    a = 3
    IF a > 5 THEN
        a = 10
    ELSE
        a = 20
    END IF
    RETURN a
END
Expected result: 20 (since 3 is not > 5, else branch executes)
*/

void example_if_else()
{
    init("if_else_example");
    LLVMFunction func = new_function("main", INT);

    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const3 = new_constant(3);
    LLVMEntity const5 = new_constant(5);
    LLVMEntity const10 = new_constant(10);
    LLVMEntity const20 = new_constant(20);

    LLVMassign(&a, &const3);

    // if (a > 5) { a = 10 } else { a = 20 }
    LLVMEntity condition = LLVMcompare(&a, ">", &const5);
    LLVMIfElse if_else = create_if_else(&func, condition, "main");

    // Then branch
    enter_if_then_else(&if_else, 0);
    LLVMassign(&a, &const10);
    exit_if_else_branch(&if_else);

    // Else branch
    enter_if_then_else(&if_else, 1);
    LLVMassign(&a, &const20);
    exit_if_else_branch(&if_else);

    finalize_if_else(&if_else);
    LLVMreturn(&a);
    LLVMsaveToFile("out.ir");
}

int main()
{
    printf("=== Example 2: If/Else Statement ===\n");
    printf("Pseudocode:\n");
    printf("  a = 3\n");
    printf("  if (a > 5) then\n");
    printf("    a = 10\n");
    printf("  else\n");
    printf("    a = 20\n");
    printf("  end if\n");
    printf("  return a\n");
    printf("Expected result: 20\n\n");

    example_if_else();
    return 0;
}