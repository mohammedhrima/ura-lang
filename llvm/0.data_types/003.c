#include "../utils.c"

/*
int main()
{
   char* str = "abcd";
   char c = str[0];
   putchar(c);
}
*/

int main()
{
   init("calloc_example");

   Foo callocFunc = (Foo){.name = "calloc", .retType = charPtrType, .paramCount = 2, .paramTypes = (LLVMTypeRef[]){int64Type, int64Type}};
   create_function(&callocFunc);

   Foo strcpyFunc = (Foo){.name = "strcpy", .retType = charPtrType, .paramCount = 2, .paramTypes = (LLVMTypeRef[]){charPtrType, charPtrType}};
   create_function(&strcpyFunc);

   Foo putsFunc = (Foo){.name = "puts", .retType = int32Type, .paramCount = 1, .paramTypes = (LLVMTypeRef[]){charPtrType}};
   create_function(&putsFunc);

   Foo freeFunc = (Foo){.name = "free", .retType = voidType, .paramCount = 1, .paramTypes = (LLVMTypeRef[]){charPtrType}};
   create_function(&freeFunc);

   Foo mainFunc = (Foo){.name = "main", .retType = int32Type, .paramCount = 0, .paramTypes = NULL};
   create_function(&mainFunc);

   LLVMBasicBlockRef entry = create_bloc("entry", mainFunc.elem);
   open_block(entry);

   LLVMBasicBlockRef whileCond = create_bloc("while.cond", mainFunc.elem);
   LLVMBasicBlockRef whileBody = create_bloc("while.body", mainFunc.elem);
   LLVMBasicBlockRef whileEnd = create_bloc("while.end", mainFunc.elem);

   // === Entry block (already positioned by create_function_no_params) ===

   LLVMValueRef *args = (LLVMValueRef[]){create_int(int64Type, 10), create_int(int64Type, 1)};
   LLVMValueRef str_ptr = call_function(&callocFunc, "str_ptr", args, 2);
   LLVMValueRef str = allocate_variable(charPtrType, "str");
   assign(str, str_ptr);

   args = (LLVMValueRef[]){load_variable(charPtrType, "str_val", str), create_string("abcdef")};
   call_function(&strcpyFunc, "", args, 2);

   LLVMValueRef i = allocate_variable(int32Type, "i");
   assign(i, create_int(int32Type, 0));

   // Branch to while condition
   create_branch(whileCond);

   // === While condition block ===
   open_block(whileCond);

   // Load i
   LLVMValueRef i_val = load_variable(int32Type, "i_val", i);

   // Compare i < 2
   LLVMValueRef cmp = operation(LLVMIntSLT, i_val, create_int(int32Type, 2));

   // Conditional branch
   create_condition(cmp, whileBody, whileEnd);

   // === While body block ===
   open_block(whileBody);

   // Load str
   LLVMValueRef str_val2 = load_variable(charPtrType, "str_val2", str);

   // Load i
   LLVMValueRef i_val2 = load_variable(int32Type, "i_val2", i);

   // Calculate str[i] address using GEP
   LLVMValueRef elem_ptr = access(str_val2, i_val2, charType);

   // Store 'a' into str[i]
   assign(elem_ptr, create_int(charType, 'a'));

   // i += 1
   LLVMValueRef i_plus_1 = math('+', i_val2, create_int(int32Type, 1));
   assign(i, i_plus_1);

   // Branch back to condition
   create_branch(whileCond);

   // === While end block ===
   open_block(whileEnd);

   // Call puts(str)
   call_function(&putsFunc, "", (LLVMValueRef[]){
                                    load_variable(charPtrType, "str_val3", str),
                                },
                 1);

   // Call free(str)
   call_function(&freeFunc, "", (LLVMValueRef[]){
                                    load_variable(charPtrType, "str_val4", str),
                                },
                 1);

   // Return 0
   ret(create_int(int32Type, 0));

   finalize();

   return 0;
}