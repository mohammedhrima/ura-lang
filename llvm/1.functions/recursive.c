#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Pseudocode to implement:
/*
main():
   char c = 'a'
   func bool isdigit(): return c >= '0' and c <= '9'
   if isdigit(): return 11
   else: return 12
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("nested_function_capture_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();

   // Basic types
   LLVMTypeRef int32Type = LLVMInt32Type();
   LLVMTypeRef int8Type = LLVMInt8Type();                  // char type
   LLVMTypeRef boolType = LLVMInt1Type();                  // bool type
   LLVMTypeRef charPtrType = LLVMPointerType(int8Type, 0); // char* for capturing variables

   // Create the nested function "isdigit" that takes a pointer to captured variables
   // For simplicity, we'll pass the address of 'c' as a parameter
   // bool isdigit(char* captured_c)
   LLVMTypeRef isdigitParams[] = {charPtrType};
   LLVMTypeRef isdigitType = LLVMFunctionType(boolType, isdigitParams, 1, 0);
   LLVMValueRef isdigitFunc = LLVMAddFunction(mod, "isdigit", isdigitType);

   // Build the isdigit function body
   LLVMBasicBlockRef isdigitEntry = LLVMAppendBasicBlock(isdigitFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, isdigitEntry);

   // Get the parameter (pointer to captured 'c')
   LLVMValueRef capturedCPtr = LLVMGetParam(isdigitFunc, 0);

   // Load the captured 'c' value
   LLVMValueRef capturedC = LLVMBuildLoad2(builder, int8Type, capturedCPtr, "captured_c");

   // Create constants for '0' and '9' (ASCII values)
   LLVMValueRef char0 = LLVMConstInt(int8Type, '0', 0); // ASCII 48
   LLVMValueRef char9 = LLVMConstInt(int8Type, '9', 0); // ASCII 57

   // c >= '0'
   LLVMValueRef cmp1 = LLVMBuildICmp(builder, LLVMIntSGE, capturedC, char0, "cmp_ge_0");
   // c <= '9'
   LLVMValueRef cmp2 = LLVMBuildICmp(builder, LLVMIntSLE, capturedC, char9, "cmp_le_9");
   // c >= '0' AND c <= '9'
   LLVMValueRef result = LLVMBuildAnd(builder, cmp1, cmp2, "is_digit");

   LLVMBuildRet(builder, result);

   // Create the main function
   LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
   LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, mainEntry);

   // char c = 'a' (allocate on stack so we can get its address)
   LLVMValueRef cVar = LLVMBuildAlloca(builder, int8Type, "c");
   LLVMValueRef charA = LLVMConstInt(int8Type, 'a', 0); // ASCII 97
   LLVMBuildStore(builder, charA, cVar);

   // Call isdigit() - pass the address of 'c' to simulate variable capture
   LLVMValueRef isdigitArgs[] = {cVar}; // Pass pointer to 'c'
   LLVMValueRef digitCheck = LLVMBuildCall2(builder, isdigitType, isdigitFunc, isdigitArgs, 1, "isdigit_result");

   // Create basic blocks for if-else
   LLVMBasicBlockRef thenBlock = LLVMAppendBasicBlock(mainFunc, "then");
   LLVMBasicBlockRef elseBlock = LLVMAppendBasicBlock(mainFunc, "else");

   // Branch based on isdigit result
   LLVMBuildCondBr(builder, digitCheck, thenBlock, elseBlock);

   // Then block: return 11
   LLVMPositionBuilderAtEnd(builder, thenBlock);
   LLVMValueRef const11 = LLVMConstInt(int32Type, 11, 0);
   LLVMBuildRet(builder, const11);

   // Else block: return 12
   LLVMPositionBuilderAtEnd(builder, elseBlock);
   LLVMValueRef const12 = LLVMConstInt(int32Type, 12, 0);
   LLVMBuildRet(builder, const12);

   // Output the LLVM IR
   printf("Generated LLVM IR with variable capture:\n");
   LLVMPrintModuleToFile(mod, "out.ll", NULL);
   char *moduleString = LLVMPrintModuleToString(mod);
   printf("%s\n", moduleString);
   LLVMDisposeMessage(moduleString);

   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);

   return 0;
}

// Alternative approach using a closure struct for multiple captured variables:
/*
If you had multiple variables to capture, you could create a struct:

struct Closure {
    char c;
    int someOtherVar;
    // ... other captured variables
};

Then pass a pointer to this struct to the nested function.
This is how many compilers implement closures internally.
*/