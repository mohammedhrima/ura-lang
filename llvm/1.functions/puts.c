#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   // Initialize LLVM
   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();

   // Create a new LLVM context
   LLVMContextRef context = LLVMContextCreate();

   // Create a module
   LLVMModuleRef module = LLVMModuleCreateWithNameInContext("main_module", context);

   // Create a builder
   LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

   // Define types
   LLVMTypeRef int32_type = LLVMInt32TypeInContext(context);
   LLVMTypeRef int8_type = LLVMInt8TypeInContext(context);
   LLVMTypeRef int8_ptr_type = LLVMPointerType(int8_type, 0);

   // Create the string constant "abc\0"
   LLVMValueRef str_constant = LLVMConstStringInContext(context, "abc", 3, 0); // 0 means null-terminate

   // Create a global variable for the string
   LLVMValueRef global_str = LLVMAddGlobal(module, LLVMTypeOf(str_constant), "str_literal");
   LLVMSetInitializer(global_str, str_constant);
   LLVMSetLinkage(global_str, LLVMPrivateLinkage);
   LLVMSetGlobalConstant(global_str, 1);

   // Declare puts function
   LLVMTypeRef puts_param_types[] = {int8_ptr_type};
   LLVMTypeRef puts_function_type = LLVMFunctionType(int32_type, puts_param_types, 1, 0);
   LLVMValueRef puts_function = LLVMAddFunction(module, "puts", puts_function_type);

   // Create main function
   LLVMTypeRef main_function_type = LLVMFunctionType(int32_type, NULL, 0, 0);
   LLVMValueRef main_function = LLVMAddFunction(module, "main", main_function_type);

   // Create basic block for main function
   LLVMBasicBlockRef entry_block = LLVMAppendBasicBlockInContext(context, main_function, "entry");
   LLVMPositionBuilderAtEnd(builder, entry_block);

   // Create alloca for str pointer
   LLVMValueRef str_alloca = LLVMBuildAlloca(builder, int8_ptr_type, "str");

   // Get pointer to the global string (equivalent to getting address of "abc")
   LLVMValueRef indices[] = {
       LLVMConstInt(int32_type, 0, 0), // First index: 0 (to get the array)
       LLVMConstInt(int32_type, 0, 0)  // Second index: 0 (to get first element)
   };
   LLVMValueRef str_ptr = LLVMBuildInBoundsGEP2(builder, LLVMTypeOf(str_constant),
                                                global_str, indices, 2, "str_ptr");

   // Store the string pointer
   LLVMBuildStore(builder, str_ptr, str_alloca);

   // Load the string pointer for puts call
   LLVMValueRef loaded_str = LLVMBuildLoad2(builder, int8_ptr_type, str_alloca, "loaded_str");

   // Call puts function
   LLVMValueRef puts_args[] = {loaded_str};
   LLVMBuildCall2(builder, puts_function_type, puts_function, puts_args, 1, "puts_call");

   // Return 0
   LLVMBuildRet(builder, LLVMConstInt(int32_type, 0, 0));

   // Verify the module
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMAbortProcessAction, &error))
   {
      fprintf(stderr, "Error: %s\n", error);
      LLVMDisposeMessage(error);
      return 1;
   }

   // Print the generated LLVM IR
   char *ir_string = LLVMPrintModuleToString(module);
   printf("%s\n", ir_string);
   LLVMDisposeMessage(ir_string);

   // Optional: Write bitcode to file
    LLVMPrintModuleToFile(module, "out.ll", NULL);
   // Clean up
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);

   return 0;
}