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
   char merge_name[64];
   snprintf(merge_name, sizeof(merge_name), "merge_%s", label);
   chain->merge_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, merge_name);

   if (has_else)
   {
      char else_name[64];
      snprintf(else_name, sizeof(else_name), "else_%s", label);
      chain->else_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, else_name);
   }

   // Build the chain properly - each condition needs to be tested in sequence
   for (int i = 0; i < chain->branch_count; i++)
   {
      LLVMBasicBlockRef false_target;

      if (i + 1 < chain->branch_count)
      {
         // Not the last condition, create next block for the next condition test
         char next_name[64];
         snprintf(next_name, sizeof(next_name), "next_%s_%d", label, i);
         chain->branches[i].next_block = LLVMAppendBasicBlockInContext(llvm.context, func->ref, next_name);
         false_target = chain->branches[i].next_block;
      }
      else
      {
         // Last condition, false goes to else block (if exists) or merge block
         false_target = has_else ? chain->else_block : chain->merge_block;
         chain->branches[i].next_block = false_target; // For consistency
      }

      // Build the conditional branch from current position
      LLVMBuildCondBr(llvm.builder, chain->branches[i].condition.content,
                      chain->branches[i].then_block, false_target);

      // If there's a next condition to test, position builder at that block
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

/*
PSEUDOCODE for multiple elif chain (grading system):
BEGIN
   score = 85
   grade = 0
   IF score >= 90 THEN
      grade = 100  // A grade
   ELIF score >= 80 THEN
      grade = 80   // B grade
   ELIF score >= 70 THEN
      grade = 70   // C grade
   ELIF score >= 60 THEN
      grade = 60   // D grade
   ELSE
      grade = 0    // F grade
   END IF
   RETURN grade
END
Expected result: 80 (B grade, since 85 >= 80 but < 90)
*/

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

   // score = 85
   LLVMassign(&score, &const85);

   // Create the if/elif/else chain
   LLVMIfChain *grading = create_if_chain(4);

   // Add all conditions first
   LLVMEntity condA = LLVMcompare(&score, ">=", &const90);
   add_if_branch(grading, &func, condA, "grade");

   LLVMEntity condB = LLVMcompare(&score, ">=", &const80);
   add_if_branch(grading, &func, condB, "grade");

   LLVMEntity condC = LLVMcompare(&score, ">=", &const70);
   add_if_branch(grading, &func, condC, "grade");

   LLVMEntity condD = LLVMcompare(&score, ">=", &const60);
   add_if_branch(grading, &func, condD, "grade");

   // Finalize the chain structure (this builds the control flow)
   finalize_if_chain(grading, &func, "grade", 1);

   // Now fill in the branch contents
   // A grade (score >= 90)
   enter_chain_branch(grading, 0);
   LLVMassign(&grade, &const100);
   exit_chain_branch(grading);

   // B grade (score >= 80) - this will execute for score=85
   enter_chain_branch(grading, 1);
   LLVMassign(&grade, &constB);
   exit_chain_branch(grading);

   // C grade (score >= 70)
   enter_chain_branch(grading, 2);
   LLVMassign(&grade, &constC);
   exit_chain_branch(grading);

   // D grade (score >= 60)
   enter_chain_branch(grading, 3);
   LLVMassign(&grade, &constD);
   exit_chain_branch(grading);

   // F grade (else - score < 60)
   enter_chain_else(grading);
   LLVMassign(&grade, &constF);
   exit_chain_branch(grading);

   // Continue after the if/elif/else chain
   finish_if_chain(grading);
   LLVMreturn(&grade);

   free_if_chain(grading);
   LLVMsaveToFile("out.ir");
}

int main()
{
   printf("=== Example 4: Multiple Elif Chain (Grading System) ===\n");
   printf("Pseudocode:\n");
   printf("  score = 85\n");
   printf("  if (score >= 90) then\n");
   printf("    grade = 100  // A grade\n");
   printf("  elif (score >= 80) then\n");
   printf("    grade = 80   // B grade\n");
   printf("  elif (score >= 70) then\n");
   printf("    grade = 70   // C grade\n");
   printf("  elif (score >= 60) then\n");
   printf("    grade = 60   // D grade\n");
   printf("  else\n");
   printf("    grade = 0    // F grade\n");
   printf("  end if\n");
   printf("  return grade\n");
   printf("Expected result: 80 (B grade)\n\n");

   example_multiple_elif();
   return 0;
}