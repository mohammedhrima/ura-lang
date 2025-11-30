#include "../utils.c"

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
   init("putchar_example");

   // Declare external function: int putchar(int c)
   Foo putcharFunc = (Foo){
      .name = "putchar", 
      .retType = int32Type, 
      .paramCount = 1, 
      .paramTypes = (LLVMTypeRef[]){int32Type}
   };
   create_function(&putcharFunc);

   // Create the main function
   Foo mainFunc = (Foo){
      .name = "main", 
      .retType = int32Type, 
      .paramCount = 0, 
      .paramTypes = NULL
   };
   create_function(&mainFunc);

   // Create and open entry basic block
   LLVMBasicBlockRef entry = create_bloc("entry", mainFunc.elem);
   open_block(entry);

   // === Entry block ===

   // Create string literal "abcd"
   LLVMValueRef str_literal = create_string("abcd");

   // Allocate local variable for str pointer
   LLVMValueRef str = allocate_variable(charPtrType, "str");
   assign(str, str_literal);

   // Load str
   LLVMValueRef str_val = load_variable(charPtrType, "str_val", str);

   // Calculate str[0] address using GEP
   LLVMValueRef elem_ptr = access(str_val, create_int(int32Type, 0), charType);

   // Load char c = str[0]
   LLVMValueRef c_val = load_variable(charType, "c", elem_ptr);

   // Allocate local variable for c
   LLVMValueRef c = allocate_variable(charType, "c");
   assign(c, c_val);

   // Load c for putchar call
   LLVMValueRef c_for_call = load_variable(charType, "c_val", c);

   // Extend char to int32 for putchar (putchar takes int)
   LLVMValueRef c_extended = LLVMBuildSExt(builder, c_for_call, int32Type, "c_ext");

   // Call putchar(c)
   LLVMValueRef putcharArgs[] = { c_extended };
   call_function(&putcharFunc, "", putcharArgs, 1);

   // Return 0
   ret(create_int(int32Type, 0));

   finalize();

   return 0;
}

/*
int main()
{
   int i = 0;
   while (i < 5)
   {
      i += 1;
   }
}
*/