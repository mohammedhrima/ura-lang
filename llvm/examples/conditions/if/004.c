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
      pr = LLVMIntNE; // fallback

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

/*
Nested If Example - Grade Calculator:
main():
   int score = 85
   int grade = 0

   if (score >= 60) {        // Passing grade
      if (score >= 90) {     // A grade
         if (score >= 95) {
            grade = 100      // A+
         } else {
            grade = 90       // A
         }
      } else if (score >= 80) {
         grade = 80          // B
      } else if (score >= 70) {
         grade = 70          // C
      } else {
         grade = 60          // D
      }
   } else {                  // Failing grade
      if (score >= 50) {
         grade = 50          // F+
      } else {
         grade = 0           // F
      }
   }

   return grade

Expected flow with score = 85:
- score >= 60: true  → enter outer if
- score >= 90: false → check elif
- score >= 80: true  → grade = 80
- return 80
*/

int main()
{
   init("nested_if_grade_calculator");
   LLVMFunction func = new_function("main", INT);

   // Variables
   LLVMEntity score = new_variable("score", INT);
   LLVMEntity grade = new_variable("grade", INT);

   // Constants
   LLVMEntity const0 = new_constant(0);
   LLVMEntity const50 = new_constant(50);
   LLVMEntity const60 = new_constant(60);
   LLVMEntity const70 = new_constant(70);
   LLVMEntity const80 = new_constant(80);
   LLVMEntity const85 = new_constant(85);
   LLVMEntity const90 = new_constant(90);
   LLVMEntity const95 = new_constant(95);
   LLVMEntity const100 = new_constant(100);

   // Initialize: score = 85, grade = 0
   LLVMassign(&score, &const85);
   LLVMassign(&grade, &const0);

   // Create all basic blocks for nested structure
   LLVMBasicBlockRef outer_if = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "outer_if");
   LLVMBasicBlockRef outer_else = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "outer_else");
   LLVMBasicBlockRef final_block = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "final");

   // Inner blocks for the passing grade path
   LLVMBasicBlockRef inner_a_check = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_a_check");
   LLVMBasicBlockRef inner_a_plus = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_a_plus");
   LLVMBasicBlockRef inner_a = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_a");
   LLVMBasicBlockRef inner_b_check = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_b_check");
   LLVMBasicBlockRef inner_b = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_b");
   LLVMBasicBlockRef inner_c_check = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_c_check");
   LLVMBasicBlockRef inner_c = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_c");
   LLVMBasicBlockRef inner_d = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "inner_d");

   // Inner blocks for the failing grade path
   LLVMBasicBlockRef fail_check = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "fail_check");
   LLVMBasicBlockRef fail_plus = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "fail_plus");
   LLVMBasicBlockRef fail_zero = LLVMAppendBasicBlockInContext(llvm.context, func.ref, "fail_zero");

   // OUTER IF: Check if score >= 60 (passing)
   LLVMEntity outer_cond = LLVMcompare(&score, ">=", &const60);
   LLVMBuildCondBr(llvm.builder, outer_cond.content, outer_if, outer_else);

   // === OUTER IF BLOCK (Passing grades) ===
   LLVMPositionBuilderAtEnd(llvm.builder, outer_if);

   // Check if score >= 90 (A grade range)
   LLVMEntity a_cond = LLVMcompare(&score, ">=", &const90);
   LLVMBuildCondBr(llvm.builder, a_cond.content, inner_a_check, inner_b_check);

   // NESTED: A grade check (score >= 95 for A+)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_a_check);
   LLVMEntity a_plus_cond = LLVMcompare(&score, ">=", &const95);
   LLVMBuildCondBr(llvm.builder, a_plus_cond.content, inner_a_plus, inner_a);

   // A+ grade (95-100)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_a_plus);
   LLVMassign(&grade, &const100);
   LLVMBuildBr(llvm.builder, final_block);

   // A grade (90-94)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_a);
   LLVMassign(&grade, &const90);
   LLVMBuildBr(llvm.builder, final_block);

   // NESTED: B grade check (score >= 80)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_b_check);
   LLVMEntity b_cond = LLVMcompare(&score, ">=", &const80);
   LLVMBuildCondBr(llvm.builder, b_cond.content, inner_b, inner_c_check);

   // B grade (80-89)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_b);
   LLVMassign(&grade, &const80);
   LLVMBuildBr(llvm.builder, final_block);

   // NESTED: C grade check (score >= 70)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_c_check);
   LLVMEntity c_cond = LLVMcompare(&score, ">=", &const70);
   LLVMBuildCondBr(llvm.builder, c_cond.content, inner_c, inner_d);

   // C grade (70-79)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_c);
   LLVMassign(&grade, &const70);
   LLVMBuildBr(llvm.builder, final_block);

   // D grade (60-69)
   LLVMPositionBuilderAtEnd(llvm.builder, inner_d);
   LLVMassign(&grade, &const60);
   LLVMBuildBr(llvm.builder, final_block);

   // === OUTER ELSE BLOCK (Failing grades) ===
   LLVMPositionBuilderAtEnd(llvm.builder, outer_else);

   // NESTED: Check if score >= 50 (F+ vs F)
   LLVMEntity fail_cond = LLVMcompare(&score, ">=", &const50);
   LLVMBuildCondBr(llvm.builder, fail_cond.content, fail_plus, fail_zero);

   // F+ grade (50-59)
   LLVMPositionBuilderAtEnd(llvm.builder, fail_plus);
   LLVMassign(&grade, &const50);
   LLVMBuildBr(llvm.builder, final_block);

   // F grade (0-49)
   LLVMPositionBuilderAtEnd(llvm.builder, fail_zero);
   LLVMassign(&grade, &const0);
   LLVMBuildBr(llvm.builder, final_block);

   // === FINAL BLOCK ===
   LLVMPositionBuilderAtEnd(llvm.builder, final_block);
   LLVMreturn(&grade);

   // Save to file
   LLVMsaveToFile("nested_if.ir");
   return 0;
}