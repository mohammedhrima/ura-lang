
#include "../utils.c"
/*
int main()
{
   char str[] = "abcdef";
   int i = 0;
   while(i < 2)
   {
      str[i] = 'x';
      i += 1;
   }
   return i;
}
*/


int main()
{
   init("module");
   Foo mainFunc = (Foo){
       .name = "main",
       .retType = int32Type,
       .paramTypes = NULL,
       .paramCount = 0,
   };
   create_function(&mainFunc);
   LLVMBasicBlockRef entry = create_bloc("entry", mainFunc.elem);
   open_block(entry);
   
   LLVMBasicBlockRef whileCond = create_bloc("while.cond", mainFunc.elem);
   LLVMBasicBlockRef whileBody = create_bloc("while.body", mainFunc.elem);
   LLVMBasicBlockRef whileEnd = create_bloc("while.end", mainFunc.elem);

   // Create mutable string array
   LLVMValueRef str = create_mutable_string("abcdef");

   LLVMValueRef i = allocate_variable(int32Type, "i");
   assign(i, create_int(int32Type, 0));

   create_branch(whileCond);
   open_block(whileCond);

   LLVMValueRef i_val = load_variable(int32Type, "i_val", i);

   // Compare i < 2
   LLVMValueRef cmp = operation(LLVMIntSLT, i_val, create_int(int32Type, 2));

   create_condition(cmp, whileBody, whileEnd);
   
   open_block(whileBody);

   LLVMValueRef i_val2 = load_variable(int32Type, "i_val2", i);

   // Calculate str[i] address using GEP
   LLVMValueRef elem_ptr = access(str, i_val2, charType);

   // Store 'x' into str[i]
   assign(elem_ptr, create_int(charType, 'x'));

   // i += 1
   LLVMValueRef i_plus_1 = math('+', i_val2, create_int(int32Type, 1));
   assign(i, i_plus_1);

   // Branch back to condition
   create_branch(whileCond);

   // === While end block ===
   open_block(whileEnd);

   ret(load_variable(int32Type, "load_i", i));

   finalize();

   return 0;
}
