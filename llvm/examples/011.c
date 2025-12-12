#include "../utils.c"

/*
main():
   chars a = stack(10) as chars
   return 0
*/

int main()
{
   init("example");

   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   open_block(entry);

   // chars a = stack(10) as chars
   ValueRef a = allocate_stack(create_int(int32Type, 10), charType, "a");
   
   // a[0] = 'H'
   ValueRef ptr = access(a, create_int(int32Type, 0), charType);
   assign(ptr, create_int(charType, 'H'));

   ret(create_int(int32Type, 0));

   finalize();

   return 0;
}