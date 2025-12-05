#include "../utils.c"

/*
main():
   chars str = "Hello world"
   return 0
*/

int main()
{
   init("module");
   Foo mainFunc = (Foo){.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc(&mainFunc, "entry");
   open_block(entry);

   ValueRef str = allocate_variable(charPtrType, "str");
   ValueRef value = create_string("hello world");
   assign(str, value);

   ret(create_int(int32Type, 0));

   finalize();
   return 0;
}
