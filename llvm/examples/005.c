#include "../utils.c"

/*
protoFunc int printf(chars fmt, ...)
main():
   int result = 10
   printf("func1 returned: %d\n", result)
   return 0
*/

int main()
{
   init("simple_printf");

   Foo printfFunc = (Foo){.name = "printf", .retType = int32Type, .paramCount = 1, .paramTypes = (LLVMTypeRef[]){charPtrType}, .isVariadic = true, };
   create_function(&printfFunc);

   Foo mainFunc = (Foo){.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef mainEntry = create_bloc("entry", &mainFunc);
   open_block(mainEntry);

   ValueRef result_alloca = allocate_variable(int32Type, "result");
   assign(result_alloca, create_int(int32Type, 10));

   ValueRef result_val = load_variable(int32Type, "result_val", result_alloca);

   ValueRef format_str = create_string("func1 returned: %d\n");
   call_function(&printfFunc, "", (ValueRef[]){format_str, result_val}, 2);

   ret(create_int(int32Type, 0));

   finalize();
   return 0;
}