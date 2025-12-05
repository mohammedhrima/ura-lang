#include "../utils.c"

/*
protoFunc int putchar(char c)
main():
   char* str = "abcd"
   char c = str[0]
   putchar(c)
   return 0
*/

int main()
{
   init("module");
   Foo putcharFunc = (Foo){.name = "putchar", .retType = int32Type, .paramTypes = (TypeRef[]){int32Type}, .paramCount = 1};
   create_function(&putcharFunc);

   Foo mainFunc = (Foo){.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   open_block(entry);

   ValueRef str = allocate_variable(charPtrType, "str");
   assign(str, create_string("abcd"));

   ValueRef str_ptr = load_variable(charPtrType, "str_ptr", str);

   ValueRef c_ptr = access(str_ptr, create_int(int32Type, 0), charType);
   ValueRef c = load_variable(charType, "c", c_ptr);

   ValueRef c_int = cast_to(c, int32Type, "c_int");
   call_function(&putcharFunc, "", (ValueRef[]){c_int}, 1);

   ret(create_int(int32Type, 0));
   finalize();
   return 0;
}