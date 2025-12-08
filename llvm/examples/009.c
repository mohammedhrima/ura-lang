#include "../utils.c"

/*
main():
   int a = 1
   char b = (char)a
   b = 'e'
   return 0
*/

int main()
{
   init("example");

   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   open_block(entry);

   ValueRef a = allocate_variable(int32Type, "a");
   assign(a, create_int(int32Type, 1));

   ValueRef b = allocate_variable(charType, "b");
   ValueRef a_val = load_variable(int32Type, "a_val", a);
   ValueRef a_truncated = cast_to(a_val, charType, "a_truncated");
   assign(b, a_truncated);

   assign(b, create_int(charType, 'e'));

   ret(create_int(int32Type, 0));

   finalize();

   return 0;
}