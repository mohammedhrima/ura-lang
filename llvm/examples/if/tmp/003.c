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
typedef struct LLVMBranch LLVMBranch;
typedef struct LLVMIfChain LLVMIfChain;

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

struct LLVMBranch
{
    LLVMEntity condition;
    LLVMBasicBlockRef then_block;
    LLVMBasicBlockRef next_block;
};

struct LLVMIfChain
{
    LLVMBranch *branches;
    int branch_count;
    int capacity;
    
    LLVMBasicBlockRef else_block;
    LLVMBasicBlockRef merge_block;
    int finalized;
};

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

// Fixed control flow functions
LLVMIfChain *create_if_chain(int initial_capacity)
{
    LLVMIfChain *chain = malloc(sizeof(LLVMIfChain));
    chain->branches = malloc(sizeof(LLVMBranch) * initial_capacity);
    chain->branch_count = 0;
    chain->capacity = initial_capacity;
    chain->else_block = NULL;
    chain->merge_block = NULL;
    chain->finalized = 0;
    return chain;
}

void add_if_branch(LLVMIfChain *chain, LLVMFunction *func, LLVMEntity condition, char *label)
{
    if (chain->branch_count >= chain->capacity)
    {
        chain->capacity *= 2;
        chain->branches = realloc(chain->branches, sizeof(LLVMBranch) * chain->capacity);
    }

    char then_name[64];
    snprintf(then_name, sizeof(then_name), "then_%s_%d", label, chain->branch_count);

    LLVMBranch branch = {
        .condition = condition,
        .then_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, then_name),
        .next_block = NULL // Will be set during finalization
    };

    chain->branches[chain->branch_count++] = branch;
}

void finalize_if_chain(LLVMIfChain *chain, LLVMFunction *func, char *label, int has_else)
{
    if (chain->finalized)
        return;

    char merge_name[64];
    snprintf(merge_name, sizeof(merge_name), "merge_%s", label);
    chain->merge_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, merge_name);

    if (has_else)
    {
        char else_name[64];
        snprintf(else_name, sizeof(else_name), "else_%s", label);
        chain->else_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, else_name);
    }

    // Create next blocks for the chain and build conditional branches
    for (int i = 0; i < chain->branch_count; i++)
    {
        LLVMBasicBlockRef false_target;

        if (i + 1 < chain->branch_count)
        {
            // Not the last condition, create a next block for the next condition
            char next_name[64];
            snprintf(next_name, sizeof(next_name), "next_%s_%d", label, i);
            chain->branches[i].next_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, next_name);
            false_target = chain->branches[i].next_block;
        }
        else
        {
            // Last condition, false goes to else block or merge block
            false_target = has_else ? chain->else_block : chain->merge_block;
        }

        // Build the conditional branch
        LLVMBuildCondBr(llvm.builder, chain->branches[i].condition.content,
                        chain->branches[i].then_block, false_target);

        // If there's a next block, position builder there for the next condition
        if (i + 1 < chain->branch_count)
        {
            LLVMPositionBuilderAtEnd(llvm.builder, chain->branches[i].next_block);
        }
    }

    chain->finalized = 1;
}

void enter_chain_branch(LLVMIfChain *chain, int branch_index)
{
    if (branch_index < chain->branch_count)
    {
        LLVMPositionBuilderAtEnd(llvm.builder, chain->branches[branch_index].then_block);
    }
}

void enter_chain_else(LLVMIfChain *chain)
{
    if (chain->else_block)
    {
        LLVMPositionBuilderAtEnd(llvm.builder, chain->else_block);
    }
}

void exit_chain_branch(LLVMIfChain *chain)
{
    LLVMBuildBr(llvm.builder, chain->merge_block);
}

void finish_if_chain(LLVMIfChain *chain)
{
    LLVMPositionBuilderAtEnd(llvm.builder, chain->merge_block);
}

void free_if_chain(LLVMIfChain *chain)
{
    free(chain->branches);
    free(chain);
}

/*
PSEUDOCODE for if/elif/else chain:
BEGIN
    a = 1
    IF a > 1 THEN
        a = 10
    ELIF a == 1 THEN
        a = 20
    ELSE
        a = 30
    END IF
    RETURN a
END
Expected result: 20 (since a == 1 is true, second condition matches)
*/

void example_if_elif_else()
{
    init("if_elif_else_example");
    LLVMFunction func = new_function("main", INT);

    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const1 = new_constant(1);
    LLVMEntity const10 = new_constant(10);
    LLVMEntity const20 = new_constant(20);
    LLVMEntity const30 = new_constant(30);

    // a = 1
    LLVMassign(&a, &const1);

    // Create if/elif/else chain
    LLVMIfChain *chain = create_if_chain(3);

    // Add conditions but don't build branches yet
    LLVMEntity cond1 = LLVMcompare(&a, ">", &const1);
    add_if_branch(chain, &func, cond1, "chain");

    LLVMEntity cond2 = LLVMcompare(&a, "==", &const1);
    add_if_branch(chain, &func, cond2, "chain");

    // Finalize the chain structure
    finalize_if_chain(chain, &func, "chain", 1);

    // Now fill in the branch contents

    // First branch: if (a > 1) then a = 10
    enter_chain_branch(chain, 0);
    LLVMassign(&a, &const10);
    exit_chain_branch(chain);

    // Second branch: elif (a == 1) then a = 20
    enter_chain_branch(chain, 1);
    LLVMassign(&a, &const20);
    exit_chain_branch(chain);

    // Else branch: else a = 30
    enter_chain_else(chain);
    LLVMassign(&a, &const30);
    exit_chain_branch(chain);

    // Continue after the if/elif/else chain
    finish_if_chain(chain);
    LLVMreturn(&a);

    free_if_chain(chain);
    LLVMsaveToFile("out.ir");
}

int main()
{
    printf("=== Example 3: If/Elif/Else Chain ===\n");
    printf("Pseudocode:\n");
    printf("  a = 1\n");
    printf("  if (a > 1) then\n");
    printf("    a = 10\n");
    printf("  elif (a == 1) then\n");
    printf("    a = 20\n");
    printf("  else\n");
    printf("    a = 30\n");
    printf("  end if\n");
    printf("  return a\n");
    printf("Expected result: 20\n\n");

    example_if_elif_else();
    return 0;
}