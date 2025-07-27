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
typedef struct LLVMCondBlock LLVMCondBlock;
typedef struct LLVMConditional LLVMConditional;
LLVMEntity LLVMmath(LLVMEntity *left, char op, LLVMEntity *right);


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

// Single conditional block - the repeating unit
struct LLVMCondBlock
{
   LLVMEntity condition;   // The condition to test (NULL for else block)
   LLVMBasicBlockRef body; // Block where the action happens
   LLVMBasicBlockRef next; // Where to go if condition is false
   int is_else;            // Flag for else block (no condition)
};

// The unified conditional structure
struct LLVMConditional
{
   LLVMCondBlock *blocks;   // Array of conditional blocks
   int count;               // Number of blocks
   int capacity;            // Allocated capacity
   LLVMBasicBlockRef merge; // Final merge point
   int is_chained;          // 1 for if/elif chain, 0 for independent ifs
   int finalized;           // Structure is built and ready for use
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

// UNIFIED CONDITIONAL SYSTEM

LLVMConditional *create_conditional(int is_chained)
{
   LLVMConditional *cond = malloc(sizeof(LLVMConditional));
   cond->blocks = malloc(sizeof(LLVMCondBlock) * 4); // Start with 4 blocks
   cond->count = 0;
   cond->capacity = 4;
   cond->merge = NULL;
   cond->is_chained = is_chained;
   cond->finalized = 0;
   return cond;
}

void add_condition(LLVMConditional *cond, LLVMFunction *func, LLVMEntity condition, char *label)
{
   if (cond->count >= cond->capacity)
   {
      cond->capacity *= 2;
      cond->blocks = realloc(cond->blocks, sizeof(LLVMCondBlock) * cond->capacity);
   }

   char body_name[64];
   snprintf(body_name, sizeof(body_name), "%s_%d", label, cond->count);

   cond->blocks[cond->count] = (LLVMCondBlock){
       .condition = condition,
       .body = LLVMAppendBasicBlockInContext(llvm.context, func->ref, body_name),
       .next = NULL,
       .is_else = 0};
   cond->count++;
}

void add_else(LLVMConditional *cond, LLVMFunction *func, char *label)
{
   if (cond->count >= cond->capacity)
   {
      cond->capacity *= 2;
      cond->blocks = realloc(cond->blocks, sizeof(LLVMCondBlock) * cond->capacity);
   }

   char else_name[64];
   snprintf(else_name, sizeof(else_name), "else_%s", label);

   cond->blocks[cond->count] = (LLVMCondBlock){
       .condition = {0}, // No condition for else
       .body = LLVMAppendBasicBlockInContext(llvm.context, func->ref, else_name),
       .next = NULL,
       .is_else = 1};
   cond->count++;
}

void finalize_conditional(LLVMConditional *cond, LLVMFunction *func, char *label)
{
   if (cond->finalized)
      return;

   // Create merge block
   char merge_name[64];
   snprintf(merge_name, sizeof(merge_name), "merge_%s", label);
   cond->merge = LLVMAppendBasicBlockInContext(llvm.context, func->ref, merge_name);

   if (cond->is_chained)
   {
      // CHAINED: if/elif/elif/else - only one path executes
      for (int i = 0; i < cond->count; i++)
      {
         if (cond->blocks[i].is_else)
         {
            // Else block - no condition to test
            continue;
         }

         // Determine where to go if condition is false
         LLVMBasicBlockRef false_target = cond->merge;

         // Look for next condition or else block
         for (int j = i + 1; j < cond->count; j++)
         {
            if (!cond->blocks[j].is_else)
            {
               // Found next condition - create test block
               char next_name[64];
               snprintf(next_name, sizeof(next_name), "test_%s_%d", label, j);
               cond->blocks[i].next = LLVMAppendBasicBlockInContext(llvm.context, func->ref, next_name);
               false_target = cond->blocks[i].next;
               break;
            }
            else
            {
               // Found else block
               false_target = cond->blocks[j].body;
               break;
            }
         }

         // Build the conditional branch
         LLVMBuildCondBr(llvm.builder, cond->blocks[i].condition.content,
                         cond->blocks[i].body, false_target);

         // Position builder for next condition if exists
         if (cond->blocks[i].next)
         {
            LLVMPositionBuilderAtEnd(llvm.builder, cond->blocks[i].next);
         }
      }
   }
   else
   {
      // INDEPENDENT: if/if/if - each condition tested separately
      // Each condition branches independently, all false paths merge
      for (int i = 0; i < cond->count; i++)
      {
         if (cond->blocks[i].is_else)
            continue;

         LLVMBasicBlockRef false_target;
         if (i + 1 < cond->count && !cond->blocks[i + 1].is_else)
         {
            // Create block to test next condition
            char next_name[64];
            snprintf(next_name, sizeof(next_name), "test_%s_%d", label, i + 1);
            cond->blocks[i].next = LLVMAppendBasicBlockInContext(llvm.context, func->ref, next_name);
            false_target = cond->blocks[i].next;
         }
         else
         {
            // Go to merge or else block
            false_target = cond->merge;
            for (int j = 0; j < cond->count; j++)
            {
               if (cond->blocks[j].is_else)
               {
                  false_target = cond->blocks[j].body;
                  break;
               }
            }
         }

         LLVMBuildCondBr(llvm.builder, cond->blocks[i].condition.content,
                         cond->blocks[i].body, false_target);

         if (cond->blocks[i].next)
         {
            LLVMPositionBuilderAtEnd(llvm.builder, cond->blocks[i].next);
         }
      }
   }

   cond->finalized = 1;
}

void enter_condition_body(LLVMConditional *cond, int block_index)
{
   if (block_index < cond->count)
   {
      LLVMPositionBuilderAtEnd(llvm.builder, cond->blocks[block_index].body);
   }
}

void exit_condition_body(LLVMConditional *cond)
{
   LLVMBuildBr(llvm.builder, cond->merge);
}

void finish_conditional(LLVMConditional *cond)
{
   LLVMPositionBuilderAtEnd(llvm.builder, cond->merge);
}

void free_conditional(LLVMConditional *cond)
{
   free(cond->blocks);
   free(cond);
}

// EXAMPLES USING THE UNIFIED SYSTEM

// Example 1: Simple if
void example_simple_if()
{
   init("simple_if");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity a = new_variable("a", INT);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const5 = new_constant(5);
   LLVMEntity const1 = new_constant(1);

   LLVMassign(&a, &const10);

   // Create conditional structure
   LLVMConditional *cond = create_conditional(1); // chained
   LLVMEntity condition = LLVMcompare(&a, ">", &const5);
   add_condition(cond, &func, condition, "if");
   finalize_conditional(cond, &func, "simple");

   // Fill the body
   enter_condition_body(cond, 0);
   LLVMEntity result = LLVMmath(&a, '+', &const1);
   LLVMassign(&a, &result);
   exit_condition_body(cond);

   finish_conditional(cond);
   LLVMreturn(&a);

   free_conditional(cond);
}

// Example 2: If/Else
void example_if_else()
{
   init("if_else");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity a = new_variable("a", INT);
   LLVMEntity const3 = new_constant(3);
   LLVMEntity const5 = new_constant(5);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const20 = new_constant(20);

   LLVMassign(&a, &const3);

   LLVMConditional *cond = create_conditional(1); // chained
   LLVMEntity condition = LLVMcompare(&a, ">", &const5);
   add_condition(cond, &func, condition, "if");
   add_else(cond, &func, "main");
   finalize_conditional(cond, &func, "ifelse");

   // If body
   enter_condition_body(cond, 0);
   LLVMassign(&a, &const10);
   exit_condition_body(cond);

   // Else body
   enter_condition_body(cond, 1);
   LLVMassign(&a, &const20);
   exit_condition_body(cond);

   finish_conditional(cond);
   LLVMreturn(&a);

   free_conditional(cond);
}

// Example 3: If/Elif/Else Chain
void example_if_elif_else()
{
   init("if_elif_else");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity score = new_variable("score", INT);
   LLVMEntity grade = new_variable("grade", INT);
   LLVMEntity const85 = new_constant(85);
   LLVMEntity const90 = new_constant(90);
   LLVMEntity const80 = new_constant(80);
   LLVMEntity const70 = new_constant(70);
   LLVMEntity const60 = new_constant(60);

   LLVMassign(&score, &const85);

   LLVMConditional *cond = create_conditional(1); // chained

   // Add all conditions
   LLVMEntity condA = LLVMcompare(&score, ">=", &const90);
   add_condition(cond, &func, condA, "grade_A");

   LLVMEntity condB = LLVMcompare(&score, ">=", &const80);
   add_condition(cond, &func, condB, "grade_B");

   LLVMEntity condC = LLVMcompare(&score, ">=", &const70);
   add_condition(cond, &func, condC, "grade_C");

   LLVMEntity condD = LLVMcompare(&score, ">=", &const60);
   add_condition(cond, &func, condD, "grade_D");

   add_else(cond, &func, "grade");
   finalize_conditional(cond, &func, "grading");

   // Fill bodies
   LLVMEntity gradeA = new_constant(100);
   LLVMEntity gradeB = new_constant(80);
   LLVMEntity gradeC = new_constant(70);
   LLVMEntity gradeD = new_constant(60);
   LLVMEntity gradeF = new_constant(0);

   enter_condition_body(cond, 0); // A
   LLVMassign(&grade, &gradeA);
   exit_condition_body(cond);

   enter_condition_body(cond, 1); // B
   LLVMassign(&grade, &gradeB);
   exit_condition_body(cond);

   enter_condition_body(cond, 2); // C
   LLVMassign(&grade, &gradeC);
   exit_condition_body(cond);

   enter_condition_body(cond, 3); // D
   LLVMassign(&grade, &gradeD);
   exit_condition_body(cond);

   enter_condition_body(cond, 4); // F (else)
   LLVMassign(&grade, &gradeF);
   exit_condition_body(cond);

   finish_conditional(cond);
   LLVMreturn(&grade);

   free_conditional(cond);
}

// Example 4: Independent conditions (if/if/if)
void example_independent_ifs()
{
   init("independent_ifs");
   LLVMFunction func = new_function("main", INT);

   LLVMEntity x = new_variable("x", INT);
   LLVMEntity counter = new_variable("counter", INT);
   LLVMEntity const5 = new_constant(5);
   LLVMEntity const10 = new_constant(10);
   LLVMEntity const0 = new_constant(0);
   LLVMEntity const1 = new_constant(1);

   LLVMassign(&x, &const5);
   LLVMassign(&counter, &const0);

   LLVMConditional *cond = create_conditional(0); // independent

   // Add multiple independent conditions
   LLVMEntity cond1 = LLVMcompare(&x, ">", &const0);
   add_condition(cond, &func, cond1, "pos");

   LLVMEntity cond2 = LLVMcompare(&x, "<", &const10);
   add_condition(cond, &func, cond2, "small");

   LLVMEntity cond3 = LLVMcompare(&x, "==", &const5);
   add_condition(cond, &func, cond3, "five");

   finalize_conditional(cond, &func, "multi");

   // Each condition increments counter independently
   enter_condition_body(cond, 0); // x > 0
   LLVMEntity inc1 = LLVMmath(&counter, '+', &const1);
   LLVMassign(&counter, &inc1);
   exit_condition_body(cond);

   enter_condition_body(cond, 1); // x < 10
   LLVMEntity inc2 = LLVMmath(&counter, '+', &const1);
   LLVMassign(&counter, &inc2);
   exit_condition_body(cond);

   enter_condition_body(cond, 2); // x == 5
   LLVMEntity inc3 = LLVMmath(&counter, '+', &const1);
   LLVMassign(&counter, &inc3);
   exit_condition_body(cond);

   finish_conditional(cond);
   LLVMreturn(&counter); // Should return 3 (all conditions true)

   free_conditional(cond);
}

// You need to add the LLVMmath function that was missing
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
   }
   res.type = INT;
   return res;
}

int main()
{
   printf("=== Unified Conditional System Examples ===\n\n");

   printf("1. Simple If:\n");
   example_simple_if();

   // printf("2. If/Else:\n");
   // example_if_else();

   // printf("3. If/Elif/Else Chain:\n");
   // example_if_elif_else();

   // printf("4. Independent Ifs:\n");
   // example_independent_ifs();

   printf("All examples completed!\n");

   LLVMsaveToFile("out.ir");

   return 0;
}