#include "../utils.c"

/*
func int add_multiply(int a, int b, int c):
   int sum = a + b
   int result = sum * c
   return result

main():
   int x = add_multiply(5, 3, 2)
   return x
*/

int main()
{
   init("module");

   Foo addMultiplyFunc = (Foo){.name = "add_multiply", .retType = int32Type, .paramCount = 3, .paramTypes = (TypeRef[]){int32Type, int32Type, int32Type}};
   create_function(&addMultiplyFunc);

   BasicBlockRef addMultiplyEntry = create_bloc("entry", &addMultiplyFunc);
   open_block(addMultiplyEntry);

   ValueRef paramA = get_param(&addMultiplyFunc, 0, "a");
   ValueRef paramB = get_param(&addMultiplyFunc, 1, "b");
   ValueRef paramC = get_param(&addMultiplyFunc, 2, "c");

   ValueRef aAlloca = allocate_variable(int32Type, "a");
   ValueRef bAlloca = allocate_variable(int32Type, "b");
   ValueRef cAlloca = allocate_variable(int32Type, "c");
   ValueRef sumAlloca = allocate_variable(int32Type, "sum");
   ValueRef resultAlloca = allocate_variable(int32Type, "result");

   assign(aAlloca, paramA);
   assign(bAlloca, paramB);
   assign(cAlloca, paramC);

   ValueRef aVal = load_variable(int32Type, "a_val", aAlloca);
   ValueRef bVal = load_variable(int32Type, "b_val", bAlloca);
   ValueRef cVal = load_variable(int32Type, "c_val", cAlloca);

   ValueRef sum = operation(aVal, "+", bVal);
   assign(sumAlloca, sum);

   ValueRef sumVal = load_variable(int32Type, "sum_val", sumAlloca);
   ValueRef result = operation(sumVal, "*", cVal);
   assign(resultAlloca, result);

   ValueRef returnVal = load_variable(int32Type, "return_val", resultAlloca);
   ret(returnVal);

   Foo mainFunc = (Foo){.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef mainEntry = create_bloc("entry", &mainFunc);
   open_block(mainEntry);

   ValueRef xAlloca = allocate_variable(int32Type, "x");

   ValueRef args[] = {create_int(int32Type, 5), create_int(int32Type, 3), create_int(int32Type, 2)};
   ValueRef callResult = call_function(&addMultiplyFunc, "call_result", args, 3);

   assign(xAlloca, callResult);

   ValueRef xVal = load_variable(int32Type, "x_val", xAlloca);
   ret(xVal);

   finalize();

   return 0;
}