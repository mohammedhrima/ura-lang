#include "../utils.c"

/*
int main()
{
   char *str = "abc";
   return 0;
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

   LLVMValueRef str = allocate_variable(charPtrType, "str");
   LLVMValueRef value = create_string("hello world");
   assign(str, value);

   ret(create_int(int32Type, 0));

   finalize();
   return 0;
}
