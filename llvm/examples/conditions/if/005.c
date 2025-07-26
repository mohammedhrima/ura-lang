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
typedef struct LLVMBranch LLVMBranch;
typedef struct LLVMIfChain LLVMIfChain;

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

// Enhanced structure for handling branches
struct LLVMBranch
{
   LLVMEntity condition;
   LLVMBasicBlockRef then_block;
   LLVMBasicBlockRef next_block; // For chaining elif/else
};

// Structure for if/elif/else chains
struct LLVMIfChain
{
   LLVMBranch *branches;
   int branch_count;
   int capacity;
   LLVMBasicBlockRef else_block;
   LLVMBasicBlockRef merge_block;
};

// Original structure - still useful for simple cases
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

// ==== CONTROL FLOW ABSTRACTIONS ====

// 1. Simple if statement using original struct
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

// 2. If/Else using enhanced approach
typedef struct
{
   LLVMBasicBlockRef then_block;
   LLVMBasicBlockRef else_block;
   LLVMBasicBlockRef merge_block;
} LLVMIfElse;

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

// 3. If/Elif/Else chain
LLVMIfChain *create_if_chain(int initial_capacity)
{
   LLVMIfChain *chain = malloc(sizeof(LLVMIfChain));
   chain->branches = malloc(sizeof(LLVMBranch) * initial_capacity);
   chain->branch_count = 0;
   chain->capacity = initial_capacity;
   chain->else_block = NULL;
   chain->merge_block = NULL;
   return chain;
}

void add_if_branch(LLVMIfChain *chain, LLVMFunction *func, LLVMEntity condition, char *label)
{
   if (chain->branch_count >= chain->capacity)
   {
      chain->capacity *= 2;
      chain->branches = realloc(chain->branches, sizeof(LLVMBranch) * chain->capacity);
   }

   char then_name[64], next_name[64];
   snprintf(then_name, sizeof(then_name), "then_%s_%d", label, chain->branch_count);
   snprintf(next_name, sizeof(next_name), "next_%s_%d", label, chain->branch_count);

   LLVMBranch branch = {
       .condition = condition,
       .then_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, then_name),
       .next_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, next_name)};

   chain->branches[chain->branch_count++] = branch;
}

void finalize_if_chain(LLVMIfChain *chain, LLVMFunction *func, char *label, int has_else)
{
   char merge_name[64];
   snprintf(merge_name, sizeof(merge_name), "merge_%s", label);
   chain->merge_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, merge_name);

   if (has_else)
   {
      char else_name[64];
      snprintf(else_name, sizeof(else_name), "else_%s", label);
      chain->else_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, else_name);
   }

   // Build the chain
   for (int i = 0; i < chain->branch_count; i++)
   {
      LLVMBasicBlockRef next = (i + 1 < chain->branch_count) ? chain->branches[i].next_block : (has_else ? chain->else_block : chain->merge_block);

      LLVMBuildCondBr(llvm.builder, chain->branches[i].condition.content,
                      chain->branches[i].then_block, next);

      if (i + 1 < chain->branch_count)
      {
         LLVMPositionBuilderAtEnd(llvm.builder, chain->branches[i].next_block);
      }
   }
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

// ==== EXAMPLES ====

// Example 1: Simple If Statement
void example_simple_if()
{
   init("simple_if_example");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity a = new_variable("a", INT);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const5 = new_constant(5);
   LLVMEntity const1 = new_constant(1);

   LLVMassign(&a, &const10);

   // if (a > 5) { a = a + 1 }
   LLVMEntity condition = LLVMcompare(&a, ">", &const5);
   LLVMStatement if_stmt = create_simple_if(&func, condition, "simple");

   enter_if_then(&if_stmt);
   LLVMEntity result = LLVMmath(&a, '+', &const1);
   LLVMassign(&a, &result);
   exit_if_then(&if_stmt);

   LLVMreturn(&a);
   LLVMsaveToFile("simple_if.ir");
}

// Example 2: If/Else Statement
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
   LLVMsaveToFile("if_else.ir");
}

// Example 3: If/Elif/Else Chain
void example_if_elif_else()
{
   init("if_elif_else_example");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity a = new_variable("a", INT);
   LLVMEntity const1 = new_constant(1);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const20 = new_constant(20);
   LLVMEntity const30 = new_constant(30);
   LLVMEntity const40 = new_constant(40);

   LLVMassign(&a, &const1);

   // Create if/elif/else chain
   LLVMIfChain *chain = create_if_chain(3);

   // if (a > 1)
   LLVMEntity cond1 = LLVMcompare(&a, ">", &const1);
   add_if_branch(chain, &func, cond1, "chain");

   // elif (a == 1)
   LLVMEntity cond2 = LLVMcompare(&a, "==", &const1);
   add_if_branch(chain, &func, cond2, "chain");

   finalize_if_chain(chain, &func, "chain", 1);

   // Execute first condition (a > 1) - false
   enter_chain_branch(chain, 0);
   LLVMassign(&a, &const10);
   exit_chain_branch(chain);

   // Execute second condition (a == 1) - true
   enter_chain_branch(chain, 1);
   LLVMassign(&a, &const20);
   exit_chain_branch(chain);

   // Else branch
   enter_chain_else(chain);
   LLVMassign(&a, &const30);
   exit_chain_branch(chain);

   finish_if_chain(chain);
   LLVMreturn(&a);

   free_if_chain(chain);
   LLVMsaveToFile("if_elif_else.ir");
}

// Example 4: Multiple Elif Chain
void example_multiple_elif()
{
   init("multiple_elif_example");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity score = new_variable("score", INT);
   LLVMEntity grade = new_variable("grade", INT);
   LLVMEntity const85 = new_constant(85);
   LLVMEntity const90 = new_constant(90);
   LLVMEntity const80 = new_constant(80);
   LLVMEntity const70 = new_constant(70);
   LLVMEntity const60 = new_constant(60);
   LLVMEntity const100 = new_constant(100);
   LLVMEntity constB = new_constant(80); // B grade
   LLVMEntity constC = new_constant(70); // C grade
   LLVMEntity constD = new_constant(60); // D grade
   LLVMEntity constF = new_constant(0);  // F grade

   LLVMassign(&score, &const85);

   LLVMIfChain *grading = create_if_chain(4);

   // if (score >= 90) grade = 100 (A)
   LLVMEntity condA = LLVMcompare(&score, ">=", &const90);
   add_if_branch(grading, &func, condA, "grade");

   // elif (score >= 80) grade = 80 (B)
   LLVMEntity condB = LLVMcompare(&score, ">=", &const80);
   add_if_branch(grading, &func, condB, "grade");

   // elif (score >= 70) grade = 70 (C)
   LLVMEntity condC = LLVMcompare(&score, ">=", &const70);
   add_if_branch(grading, &func, condC, "grade");

   // elif (score >= 60) grade = 60 (D)
   LLVMEntity condD = LLVMcompare(&score, ">=", &const60);
   add_if_branch(grading, &func, condD, "grade");

   finalize_if_chain(grading, &func, "grade", 1);

   // A grade
   enter_chain_branch(grading, 0);
   LLVMassign(&grade, &const100);
   exit_chain_branch(grading);

   // B grade (this will execute for score=85)
   enter_chain_branch(grading, 1);
   LLVMassign(&grade, &constB);
   exit_chain_branch(grading);

   // C grade
   enter_chain_branch(grading, 2);
   LLVMassign(&grade, &constC);
   exit_chain_branch(grading);

   // D grade
   enter_chain_branch(grading, 3);
   LLVMassign(&grade, &constD);
   exit_chain_branch(grading);

   // F grade (else)
   enter_chain_else(grading);
   LLVMassign(&grade, &constF);
   exit_chain_branch(grading);

   finish_if_chain(grading);
   LLVMreturn(&grade);

   free_if_chain(grading);
   LLVMsaveToFile("multiple_elif.ir");
}

// Example 5: Nested If Statements
void example_nested_if()
{
   init("nested_if_example");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity x = new_variable("x", INT);
   LLVMEntity y = new_variable("y", INT);
   LLVMEntity result = new_variable("result", INT);

   LLVMEntity const5 = new_constant(5);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const15 = new_constant(15);
   LLVMEntity const0 = new_constant(0);
   LLVMEntity const1 = new_constant(1);
   LLVMEntity const2 = new_constant(2);
   LLVMEntity const3 = new_constant(3);
   LLVMEntity const4 = new_constant(4);

   LLVMassign(&x, &const15);
   LLVMassign(&y, &const5);
   LLVMassign(&result, &const0);

   // Outer if: if (x > 10)
   LLVMEntity outer_cond = LLVMcompare(&x, ">", &const10);
   LLVMStatement outer_if = create_simple_if(&func, outer_cond, "outer");

   enter_if_then(&outer_if);
   {
      // Inner if: if (y > 0)
      LLVMEntity inner_cond1 = LLVMcompare(&y, ">", &const0);
      LLVMIfElse inner_if_else = create_if_else(&func, inner_cond1, "inner");

      // Inner then: if (y < 10)
      enter_if_then_else(&inner_if_else, 0);
      {
         LLVMEntity deep_cond = LLVMcompare(&y, "<", &const10);
         LLVMStatement deep_if = create_simple_if(&func, deep_cond, "deep");

         enter_if_then(&deep_if);
         LLVMassign(&result, &const1);
         exit_if_then(&deep_if);
      }
      exit_if_else_branch(&inner_if_else);

      // Inner else
      enter_if_then_else(&inner_if_else, 1);
      LLVMassign(&result, &const2);
      exit_if_else_branch(&inner_if_else);

      finalize_if_else(&inner_if_else);
   }
   exit_if_then(&outer_if);

   LLVMreturn(&result);
   LLVMsaveToFile("nested_if.ir");
}

int main()
{
   printf("Generating LLVM IR examples...\n");

   example_simple_if();
   printf("1. Simple if statement -> simple_if.ir\n");

   example_if_else();
   printf("2. If/else statement -> if_else.ir\n");

   example_if_elif_else();
   printf("3. If/elif/else chain -> if_elif_else.ir\n");

   example_multiple_elif();
   printf("4. Multiple elif chain -> multiple_elif.ir\n");

   example_nested_if();
   printf("5. Nested if statements -> nested_if.ir\n");

   printf("All examples generated successfully!\n");
   return 0;
}