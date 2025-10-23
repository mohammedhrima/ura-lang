#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

/*
int main()
{
   char* str = "abcd";
   char c = str[0];
   putchar(c);
}
*/

int main()
{
   // Create a new LLVM module
   LLVMModuleRef mod = LLVMModuleCreateWithName("calloc_example");

   // Create a builder for generating instructions
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Define types
   LLVMTypeRef int32Type = LLVMInt32Type();                // int
   LLVMTypeRef charType = LLVMInt8Type();                  // char (i8)
   LLVMTypeRef charPtrType = LLVMPointerType(charType, 0); // char*
   LLVMTypeRef voidType = LLVMVoidType();                  // void
   LLVMTypeRef size_tType = LLVMInt64Type();               // size_t (typically i64)

   // Declare external functions
   
   // void* calloc(size_t nmemb, size_t size)
   LLVMTypeRef callocParamTypes[] = { size_tType, size_tType };
   LLVMTypeRef callocType = LLVMFunctionType(charPtrType, callocParamTypes, 2, 0);
   LLVMValueRef callocFunc = LLVMAddFunction(mod, "calloc", callocType);

   // char* strcpy(char* dest, const char* src)
   LLVMTypeRef strcpyParamTypes[] = { charPtrType, charPtrType };
   LLVMTypeRef strcpyType = LLVMFunctionType(charPtrType, strcpyParamTypes, 2, 0);
   LLVMValueRef strcpyFunc = LLVMAddFunction(mod, "strcpy", strcpyType);

   // int puts(const char* s)
   LLVMTypeRef putsParamTypes[] = { charPtrType };
   LLVMTypeRef putsType = LLVMFunctionType(int32Type, putsParamTypes, 1, 0);
   LLVMValueRef putsFunc = LLVMAddFunction(mod, "puts", putsType);

   // void free(void* ptr)
   LLVMTypeRef freeParamTypes[] = { charPtrType };
   LLVMTypeRef freeType = LLVMFunctionType(voidType, freeParamTypes, 1, 0);
   LLVMValueRef freeFunc = LLVMAddFunction(mod, "free", freeType);

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

   // Call calloc(10, 1)
   LLVMValueRef callocArgs[] = {
       LLVMConstInt(size_tType, 10, 0),
       LLVMConstInt(size_tType, 1, 0)
   };
   LLVMValueRef str_ptr = LLVMBuildCall2(builder, callocType, callocFunc, 
                                          callocArgs, 2, "str_ptr");

   // Allocate local variable for str
   LLVMValueRef str = LLVMBuildAlloca(builder, charPtrType, "str");
   LLVMBuildStore(builder, str_ptr, str);

   // Create string literal "abcdef"
   LLVMValueRef src_literal = LLVMBuildGlobalStringPtr(builder, "abcdef", "src_literal");

   // Call strcpy(str, "abcdef")
   LLVMValueRef str_val = LLVMBuildLoad2(builder, charPtrType, str, "str_val");
   LLVMValueRef strcpyArgs[] = { str_val, src_literal };
   LLVMBuildCall2(builder, strcpyType, strcpyFunc, strcpyArgs, 2, "");

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

   // Conditional branch
   LLVMBuildCondBr(builder, cmp, whileBody, whileEnd);

   // === While body block ===
   LLVMPositionBuilderAtEnd(builder, whileBody);

   // Load str
   LLVMValueRef str_val2 = LLVMBuildLoad2(builder, charPtrType, str, "str_val2");

   // Load i
   LLVMValueRef i_val2 = LLVMBuildLoad2(builder, int32Type, i, "i_val2");

   // Calculate str[i] address using GEP
   LLVMValueRef indices[] = { i_val2 };
   LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, charType, str_val2, 
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

   // Call puts(str)
   LLVMValueRef str_val3 = LLVMBuildLoad2(builder, charPtrType, str, "str_val3");
   LLVMValueRef putsArgs[] = { str_val3 };
   LLVMBuildCall2(builder, putsType, putsFunc, putsArgs, 1, "");

   // Call free(str)
   LLVMValueRef str_val4 = LLVMBuildLoad2(builder, charPtrType, str, "str_val4");
   LLVMValueRef freeArgs[] = { str_val4 };
   LLVMBuildCall2(builder, freeType, freeFunc, freeArgs, 1, "");

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32Type, 0, 0));

   // Write the generated IR to a file
   LLVMPrintModuleToFile(mod, "out.ll", NULL);


   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}