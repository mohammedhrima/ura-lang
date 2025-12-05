#include "../utils.c"

/*
main():
   char* str = "abcd"
   char c = str[0]
   putchar(c)
   return 0
*/

int main()
{
   init("module");
   Foo mainFunc = (Foo){.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", mainFunc.elem);
   open_block(entry);

   ValueRef i = allocate_variable(int32Type, "i");
   assign(i, create_int(int32Type, 0));

   BasicBlockRef whileCond = create_bloc("whileCond", mainFunc.elem);
   BasicBlockRef whileBody = create_bloc("whileBody", mainFunc.elem);
   BasicBlockRef whileEnd = create_bloc("whileEnd", mainFunc.elem);

   branch(whileCond);
   open_block(whileCond);
   
   ValueRef i_val = load_variable(int32Type, "i_val", i);
   ValueRef cond = operation(i_val, "<", create_int(int32Type, 5));   
   create_condition(cond, whileBody, whileEnd);

   open_block(whileBody);
   // i = i + 1
   ValueRef i_loaded = load_variable(int32Type, "i_loaded", i);
   ValueRef i_inc = operation(i_loaded, "+", create_int(int32Type, 1));
   assign(i, i_inc);
   branch(whileCond);

   open_block(whileEnd);
   
   ValueRef ret_val = load_variable(int32Type, "ret", i);
   ret(ret_val);

   finalize();
   return 0;
}