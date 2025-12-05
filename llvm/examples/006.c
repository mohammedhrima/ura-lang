#include "../utils.c"

/*
main():
    char c = 'a'
    func bool isdigit(): return (c >= '0' && c <= '9')
    if isdigit(): return 11
    else: return 12

Compiles to:
    func bool isdigit(char* ptr): return (*ptr >= '0' && *ptr <= '9')
    main():
        char c = 'a'
        if isdigit(&c): return 11
        else: return 12
*/
int main()
{
    init("nested_function_capture_example");

    Foo isdigit_func = {.name = "isdigit", .retType = boolType, .paramTypes = (TypeRef[]){charPtrType}, .paramCount = 1};
    create_function(&isdigit_func);

    BasicBlockRef isdigit_entry = create_bloc("entry", &isdigit_func);
    open_block(isdigit_entry);

    ValueRef captured_ptr = get_param(&isdigit_func, 0, "ptr");
    ValueRef captured_c = load_variable(charType, "captured_c", captured_ptr);

    ValueRef char0 = create_int(charType, '0');
    ValueRef cmp_ge_0 = operation(captured_c, ">=", char0);

    ValueRef char9 = create_int(charType, '9');
    ValueRef cmp_le_9 = operation(captured_c, "<=", char9);

    ValueRef both = operation(cmp_ge_0, "and", cmp_le_9);

    ret(both);

    Foo main_func = {.name = "main", .retType = int32Type};
    create_function(&main_func);

    BasicBlockRef main_entry = create_bloc("entry", &main_func);
    open_block(main_entry);

    ValueRef c = allocate_variable(charType, "c");
    assign(c, create_int(charType, 'a'));

    ValueRef isdigit_args[] = {c};
    ValueRef digit_check = call_function(&isdigit_func, "isdigit_res", isdigit_args, 1);

    BasicBlockRef then_block = create_bloc("then", &main_func);
    BasicBlockRef else_block = create_bloc("else", &main_func);

    create_condition(digit_check, then_block, else_block);

    open_block(then_block);
    ret(create_int(int32Type, 11));

    open_block(else_block);
    ret(create_int(int32Type, 12));

    finalize();

    return 0;
}