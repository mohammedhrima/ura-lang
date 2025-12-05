#include "../utils.c"

/*
main():
   int a = 1
   ref int b = a
   b = 2
   return b
*/

int main()
{
    init("module");

    Foo mainFunc = {.name = "main", .retType = int32Type};
    create_function(&mainFunc);

    BasicBlockRef entry = create_bloc("entry", &mainFunc);
    open_block(entry);

    ValueRef a = allocate_variable(int32Type, "a");
    assign(a, create_int(int32Type, 1));

    ValueRef b = allocate_variable(int32PtrType, "b");
    assign(b, a);

    ValueRef b_val = load_variable(int32PtrType, "b_val", b);
    assign(b_val, create_int(int32Type, 2));

    ValueRef result = load_variable(int32Type, "result", b_val);
    ret(result);

    finalize();

    return 0;
}