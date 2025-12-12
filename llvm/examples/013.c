#include "../utils.c"

/*
main():
   chars a = stack(2) as chars
   a[-1] = 'a'
*/

int main()
{
   init("ura_runtime_test");

   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   open_block(entry);

   printf("\n\033[1;34m=== Creating Test Array ===\033[0m\n");
   
   ValueRef array = allocate_stack(create_int(int32Type, 5), int32Type, "numbers");
   
   printf("\033[1;32m✓ Array created with size 5\033[0m\n");
   
   printf("\n\033[1;34m=== Initializing Array (Safe) ===\033[0m\n");
   for (int i = 0; i < 5; i++) {
      ValueRef ptr = safe_access(array, create_int(int32Type, i), int32Type, 5, 50 + i, 10);
      assign(ptr, create_int(int32Type, i * 100));
      printf("\033[1;32m✓ array[%d] = %d\033[0m\n", i, i * 100);
   }
   
   printf("\n\033[1;34m=== Test 1: Valid Access ===\033[0m\n");
   printf("Accessing array[2]...\n");
   ValueRef ptr1 = safe_access(array, create_int(int32Type, 2), int32Type, 5, 65, 15);
   (void)ptr1; // Mark as intentionally unused
   printf("\033[1;32m✓ Successfully accessed array[2]\033[0m\n");
   
   printf("\n\033[1;34m=== Test 2: Out of Bounds Access ===\033[0m\n");
   printf("Attempting to access array[10]...\n");
   ValueRef ptr2 = safe_access(array, create_int(int32Type, 10), int32Type, 5, 72, 15);
   (void)ptr2; // Mark as intentionally unused
   
   printf("\033[1;32m✓ This shouldn't print\033[0m\n");

   ret(create_int(int32Type, 0));

   printf("\n\033[1;34m=== Creating Helper Function ===\033[0m\n");
   
   TypeRef getElemParams[] = {int32PtrType, int32Type, int32Type};
   Foo getElemFunc = {
      .name = "get_element",
      .retType = int32Type,
      .paramTypes = getElemParams,
      .paramCount = 3,
      .isVariadic = false
   };
   create_function(&getElemFunc);
   
   BasicBlockRef funcEntry = create_bloc("entry", &getElemFunc);
   open_block(funcEntry);
   
   ValueRef arrParam = get_param(&getElemFunc, 0, "arr");
   ValueRef idxParam = get_param(&getElemFunc, 1, "idx");
   // sizeParam not needed for safe_access since we hardcode the size
   
   ValueRef ptrDyn = safe_access(arrParam, idxParam, int32Type, 5, 100, 20);
   ValueRef valDyn = load_variable(int32Type, "result", ptrDyn);
   
   ret(valDyn);
   
   printf("\033[1;32m✓ Helper function created\033[0m\n");

   finalize();
   
   printf("\n\033[1;35m=== Compilation ===\033[0m\n");
   int compileResult = system("clang out.ll -o program 2>&1");
   
   if (compileResult == 0) {
      printf("\033[1;32m✓ Compilation successful\033[0m\n");
      printf("\n\033[1;35m=== Running Program ===\033[0m\n");
      printf("\033[1;33mExpected: Runtime error for out of bounds access\033[0m\n\n");
      system("./program");
      printf("\n\033[1;31m✗ Program should have exited with error\033[0m\n");
   } else {
      printf("\033[1;31m✗ Compilation failed\033[0m\n");
   }

   return 0;
}