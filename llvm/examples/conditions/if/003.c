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

LLVMTypeRef int32;
LLVM llvm;

enum Type {
    INT = 1
};

struct LLVM {
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMContextRef context;
};

struct LLVMFunction {
    LLVMTypeRef retType;
    LLVMTypeRef setup;
    LLVMValueRef ref;
    LLVMBasicBlockRef bloc;
};

struct LLVMEntity {
    Type type;
    char *name;
    LLVMValueRef content;
};

void init(char *name) {
    llvm.context = LLVMContextCreate();
    llvm.module = LLVMModuleCreateWithNameInContext(name, llvm.context);
    llvm.builder = LLVMCreateBuilderInContext(llvm.context);
    int32 = LLVMInt32TypeInContext(llvm.context);
}

LLVMFunction new_function(char *name, Type ret) {
    LLVMFunction res = {};
    if (ret == INT) res.setup = LLVMFunctionType(int32, NULL, 0, 0);
    res.ref = LLVMAddFunction(llvm.module, name, res.setup);
    res.bloc = LLVMAppendBasicBlockInContext(llvm.context, res.ref, "entry");
    LLVMPositionBuilderAtEnd(llvm.builder, res.bloc);
    return res;
}

LLVMEntity new_variable(char *name, Type type) {
    LLVMEntity res = {.type = type, .name = name};
    if (type == INT) res.content = LLVMBuildAlloca(llvm.builder, int32, name);
    return res;
}

LLVMEntity new_constant(int value) {
    LLVMEntity res = {.type = INT, .content = LLVMConstInt(int32, value, 0)};
    return res;
}

void LLVMassign(LLVMEntity *left, LLVMEntity *right) {
    LLVMValueRef value = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, right->name) : right->content;
    LLVMBuildStore(llvm.builder, value, left->content);
}

void LLVMreturn(LLVMEntity *entity) {
    LLVMValueRef value = entity->name ? LLVMBuildLoad2(llvm.builder, int32, entity->content, "ret") : entity->content;
    LLVMBuildRet(llvm.builder, value);
}

void LLVMsaveToFile(char *outfile) {
    LLVMVerifyModule(llvm.module, LLVMAbortProcessAction, NULL);
    LLVMPrintModuleToFile(llvm.module, outfile, NULL);
    LLVMDisposeBuilder(llvm.builder);
    LLVMDisposeModule(llvm.module);
    LLVMContextDispose(llvm.context);
}

LLVMEntity LLVMcompare(LLVMEntity *left, char *op, LLVMEntity *right) {
    LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(llvm.builder, int32, left->content, "") : left->content;
    LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(llvm.builder, int32, right->content, "") : right->content;

    LLVMIntPredicate pr;
    if (strcmp(op, ">") == 0) pr = LLVMIntSGT;
    else if (strcmp(op, ">=") == 0) pr = LLVMIntSGE;
    else if (strcmp(op, "<") == 0) pr = LLVMIntSLT;
    else if (strcmp(op, "<=") == 0) pr = LLVMIntSLE;
    else if (strcmp(op, "==") == 0) pr = LLVMIntEQ;
    else if (strcmp(op, "!=") == 0) pr = LLVMIntNE;
    else pr = LLVMIntNE;

    LLVMEntity res = {.type = INT, .content = LLVMBuildICmp(llvm.builder, pr, leftRef, rightRef, "cond")};
    return res;
}

/*
If/Elif/Else Chain Example:
main():
    int a = 1
    if (a > 1) {
        a = a + 11     // 1 > 1 = false, won't execute
    } elif (a == 1) {
        a = a + 22     // 1 == 1 = true, will execute: a = 1 + 22 = 23
    } else {
        a = a + 33     // won't execute
    }
    return a  // returns 23
*/

int main() {
    init("if_elif_else_example");
    LLVMFunction func = new_function("main", INT);

    // Variables and constants
    LLVMEntity a = new_variable("a", INT);
    LLVMEntity const1 = new_constant(1);
    LLVMEntity const11 = new_constant(11);
    LLVMEntity const22 = new_constant(22);
    LLVMEntity const33 = new_constant(33);

    // a = 1
    LLVMassign(&a, &const1);

    // Create all basic blocks first
    LLVMBasicBlockRef then1 = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "then1");
    LLVMBasicBlockRef elif_check = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "elif_check");
    LLVMBasicBlockRef then2 = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "then2");
    LLVMBasicBlockRef else_block = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "else");
    LLVMBasicBlockRef merge = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "merge");

    // First condition: if (a > 1)
    LLVMEntity cond1 = LLVMcompare(&a, ">", &const1);
    LLVMBuildCondBr(llvm.builder, cond1.content, then1, elif_check);

    // First then block: a = a + 11
    LLVMPositionBuilderAtEnd(llvm.builder, then1);
    LLVMEntity add1_left = {.type = INT, .name = "a", .content = a.content};
    LLVMValueRef add1_left_val = LLVMBuildLoad2(llvm.builder, int32, add1_left.content, "");
    LLVMValueRef add1_result = LLVMBuildAdd(llvm.builder, add1_left_val, const11.content, "+");
    LLVMBuildStore(llvm.builder, add1_result, a.content);
    LLVMBuildBr(llvm.builder, merge);

    // Elif condition check: elif (a == 1)
    LLVMPositionBuilderAtEnd(llvm.builder, elif_check);
    LLVMEntity cond2 = LLVMcompare(&a, "==", &const1);
    LLVMBuildCondBr(llvm.builder, cond2.content, then2, else_block);

    // Second then block: a = a + 22
    LLVMPositionBuilderAtEnd(llvm.builder, then2);
    LLVMEntity add2_left = {.type = INT, .name = "a", .content = a.content};
    LLVMValueRef add2_left_val = LLVMBuildLoad2(llvm.builder, int32, add2_left.content, "");
    LLVMValueRef add2_result = LLVMBuildAdd(llvm.builder, add2_left_val, const22.content, "+");
    LLVMBuildStore(llvm.builder, add2_result, a.content);
    LLVMBuildBr(llvm.builder, merge);

    // Else block: a = a + 33
    LLVMPositionBuilderAtEnd(llvm.builder, else_block);
    LLVMEntity add3_left = {.type = INT, .name = "a", .content = a.content};
    LLVMValueRef add3_left_val = LLVMBuildLoad2(llvm.builder, int32, add3_left.content, "");
    LLVMValueRef add3_result = LLVMBuildAdd(llvm.builder, add3_left_val, const33.content, "+");
    LLVMBuildStore(llvm.builder, add3_result, a.content);
    LLVMBuildBr(llvm.builder, merge);

    // Merge point and return
    LLVMPositionBuilderAtEnd(llvm.builder, merge);
    LLVMreturn(&a);
    
    LLVMsaveToFile("if_elif_else.ir");
    
    printf("If/elif/else chain IR generated: if_elif_else.ir\n");
    printf("Expected result: 23 (a=1, first condition false, second condition true: 1 + 22 = 23)\n");
    
    return 0;
}