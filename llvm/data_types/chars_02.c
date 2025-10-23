#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   char *str = "abcdef";
   int i = 0;
   while(i < 2)
   {
      str[i] = 'a';
      i += 1;
   }
   return 0;
}
*/

int main()
{
   // Create a new LLVM module
   LLVMModuleRef mod = LLVMModuleCreateWithName("while_loop_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define types
   LLVMTypeRef int32Type = LLVMInt32Type();                // int
   LLVMTypeRef charType = LLVMInt8Type();                  // char (i8)
   LLVMTypeRef charPtrType = LLVMPointerType(charType, 0); // char*

   // Create the function type for main: `int main()`
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);

   // Add the `main` function to the module
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);

   // Create basic blocks
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMBasicBlockRef whileCond = LLVMAppendBasicBlock(mainFunc, "while.cond");
   LLVMBasicBlockRef whileBody = LLVMAppendBasicBlock(mainFunc, "while.body");
   LLVMBasicBlockRef whileEnd = LLVMAppendBasicBlock(mainFunc, "while.end");

   // === Entry block ===
   LLVMPositionBuilderAtEnd(builder, entry);

   // Create string literal "abcdef"
   LLVMValueRef str_literal = LLVMBuildGlobalStringPtr(builder, "abcdef", "str_literal");

   // Declare char* str variable and initialize it
   LLVMValueRef str = LLVMBuildAlloca(builder, charPtrType, "str");
   LLVMBuildStore(builder, str_literal, str);

   // Declare int i variable and initialize to 0
   LLVMValueRef i = LLVMBuildAlloca(builder, int32Type, "i");
   LLVMBuildStore(builder, LLVMConstInt(int32Type, 0, 0), i);

   // Branch to while condition
   LLVMBuildBr(builder, whileCond);

   // === While condition block ===
   LLVMPositionBuilderAtEnd(builder, whileCond);

   // Load i
   LLVMValueRef i_val = LLVMBuildLoad2(builder, int32Type, i, "i_val");

   // Compare i < 2
   LLVMValueRef cmp = LLVMBuildICmp(builder, LLVMIntSLT, i_val,
                                    LLVMConstInt(int32Type, 2, 0), "cmp");

   // Conditional branch: if true goto while.body, else goto while.end
   LLVMBuildCondBr(builder, cmp, whileBody, whileEnd);

   // === While body block ===
   LLVMPositionBuilderAtEnd(builder, whileBody);

   // Load str
   LLVMValueRef str_val = LLVMBuildLoad2(builder, charPtrType, str, "str_val");

   // Load i again
   LLVMValueRef i_val2 = LLVMBuildLoad2(builder, int32Type, i, "i_val2");

   // Calculate str[i] address using GEP
   LLVMValueRef indices[] = {i_val2};
   LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, charType, str_val,
                                         indices, 1, "elem_ptr");

   // Store 'a' into str[i]
   LLVMBuildStore(builder, LLVMConstInt(charType, 'a', 0), elem_ptr);

   // i += 1
   LLVMValueRef i_plus_1 = LLVMBuildAdd(builder, i_val2,
                                        LLVMConstInt(int32Type, 1, 0), "i_plus_1");
   LLVMBuildStore(builder, i_plus_1, i);

   // Branch back to condition
   LLVMBuildBr(builder, whileCond);

   // === While end block ===
   LLVMPositionBuilderAtEnd(builder, whileEnd);

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);


   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}