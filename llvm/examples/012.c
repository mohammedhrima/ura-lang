#include "../utils.c"
#include <llvm-c/ExecutionEngine.h>

/*
This demonstrates multiple error handling approaches in LLVM:
1. Manual NULL pointer checking (safest for your use case)
2. Signal handling (SIGSEGV detection)
3. Return code checking
4. Guard pages / bounds checking

Note: LLVM doesn't have try/catch like C++/Java, but we can simulate
error handling using conditional checks and error handlers.
*/

// Approach 1: Manual NULL Pointer Check with Error Handler
/*
main():
   char *str = NULL
   if (str == NULL) goto error_handler
   str[0] = 'a'
   goto success
error_handler:
   printf("Error: NULL pointer access!\n")
   return 1
success:
   printf("Success!\n")
   return 0
*/

void example1_manual_null_check()
{
   init("manual_null_check");

   // Declare printf function
   TypeRef printfParams[] = {charPtrType};
   Foo printfFunc = {
      .name = "printf",
      .retType = int32Type,
      .paramTypes = printfParams,
      .paramCount = 1,
      .isVariadic = true
   };
   create_function(&printfFunc);

   // Create main function
   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   // Create basic blocks
   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   BasicBlockRef error_handler = create_bloc("error_handler", &mainFunc);
   BasicBlockRef access_block = create_bloc("access_block", &mainFunc);
   BasicBlockRef success_block = create_bloc("success_block", &mainFunc);

   // Entry block
   open_block(entry);
   
   // char *str = NULL
   ValueRef str = allocate_variable(charPtrType, "str");
   ValueRef nullPtr = LLVMConstPointerNull(charPtrType);
   assign(str, nullPtr);

   // Load str value for checking
   ValueRef strValue = load_variable(charPtrType, "str_val", str);

   // Check if str == NULL
   ValueRef isNull = operation(strValue, "==", nullPtr);
   create_condition(isNull, error_handler, access_block);

   // Access block (would try to access str[0])
   open_block(access_block);
   branch(success_block);

   // Error handler block
   open_block(error_handler);
   ValueRef errorMsg = create_string("Error: NULL pointer access detected!\n");
   ValueRef errorArgs[] = {errorMsg};
   call_function(&printfFunc, "", errorArgs, 1);
   ret(create_int(int32Type, 1));

   // Success block
   open_block(success_block);
   ValueRef successMsg = create_string("Success: Pointer was valid!\n");
   ValueRef successArgs[] = {successMsg};
   call_function(&printfFunc, "", successArgs, 1);
   ret(create_int(int32Type, 0));

   finalize();
   printf("\n=== Example 1: Manual NULL Check ===\n");
   printf("Generated: out.ll\n\n");
}

// Approach 2: Array Bounds Checking
/*
main():
   int arr[10]
   int index = 15
   if (index < 0 || index >= 10) goto bounds_error
   arr[index] = 42
   goto success
bounds_error:
   printf("Error: Array index out of bounds!\n")
   return 1
success:
   printf("Array access successful!\n")
   return 0
*/

void example2_bounds_checking()
{
   init("bounds_checking");

   // Declare printf
   TypeRef printfParams[] = {charPtrType};
   Foo printfFunc = {
      .name = "printf",
      .retType = int32Type,
      .paramTypes = printfParams,
      .paramCount = 1,
      .isVariadic = true
   };
   create_function(&printfFunc);

   // Create main
   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   BasicBlockRef check_upper = create_bloc("check_upper", &mainFunc);
   BasicBlockRef bounds_error = create_bloc("bounds_error", &mainFunc);
   BasicBlockRef safe_access = create_bloc("safe_access", &mainFunc);
   BasicBlockRef success = create_bloc("success", &mainFunc);

   open_block(entry);

   // int arr[10]
   ValueRef arr = allocate_stack(create_int(int32Type, 10), int32Type, "arr");

   // int index = 15 (out of bounds)
   ValueRef index = allocate_variable(int32Type, "index");
   assign(index, create_int(int32Type, 15));
   ValueRef indexVal = load_variable(int32Type, "index_val", index);

   // Check if index < 0
   ValueRef zero = create_int(int32Type, 0);
   ValueRef isNegative = operation(indexVal, "<", zero);
   create_condition(isNegative, bounds_error, check_upper);

   // Check if index >= 10
   open_block(check_upper);
   ValueRef arraySize = create_int(int32Type, 10);
   ValueRef isOutOfBounds = operation(indexVal, ">=", arraySize);
   create_condition(isOutOfBounds, bounds_error, safe_access);

   // Bounds error handler
   open_block(bounds_error);
   ValueRef errorMsg = create_string("Error: Array index %d out of bounds [0-9]!\n");
   ValueRef errorArgs[] = {errorMsg, indexVal};
   call_function(&printfFunc, "", errorArgs, 2);
   ret(create_int(int32Type, 1));

   // Safe access
   open_block(safe_access);
   ValueRef ptr = access(arr, indexVal, int32Type);
   assign(ptr, create_int(int32Type, 42));
   branch(success);

   // Success
   open_block(success);
   ValueRef successMsg = create_string("Array access successful!\n");
   ValueRef successArgs[] = {successMsg};
   call_function(&printfFunc, "", successArgs, 1);
   ret(create_int(int32Type, 0));

   finalize();
   printf("\n=== Example 2: Bounds Checking ===\n");
   printf("Generated: out.ll\n\n");
}

// Approach 3: Division by Zero Check
/*
main():
   int a = 10
   int b = 0
   int result
   if (b == 0) goto div_error
   result = a / b
   goto success
div_error:
   printf("Error: Division by zero!\n")
   return 1
success:
   printf("Result: %d\n", result)
   return 0
*/

void example3_division_by_zero()
{
   init("division_check");

   // Declare printf
   TypeRef printfParams[] = {charPtrType};
   Foo printfFunc = {
      .name = "printf",
      .retType = int32Type,
      .paramTypes = printfParams,
      .paramCount = 1,
      .isVariadic = true
   };
   create_function(&printfFunc);

   // Create main
   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   BasicBlockRef div_error = create_bloc("div_error", &mainFunc);
   BasicBlockRef safe_div = create_bloc("safe_div", &mainFunc);
   BasicBlockRef success = create_bloc("success", &mainFunc);

   open_block(entry);

   // int a = 10, b = 0
   ValueRef a = allocate_variable(int32Type, "a");
   ValueRef b = allocate_variable(int32Type, "b");
   assign(a, create_int(int32Type, 10));
   assign(b, create_int(int32Type, 0));

   ValueRef bVal = load_variable(int32Type, "b_val", b);
   ValueRef zero = create_int(int32Type, 0);

   // Check if b == 0
   ValueRef isZero = operation(bVal, "==", zero);
   create_condition(isZero, div_error, safe_div);

   // Division error
   open_block(div_error);
   ValueRef errorMsg = create_string("Error: Division by zero detected!\n");
   ValueRef errorArgs[] = {errorMsg};
   call_function(&printfFunc, "", errorArgs, 1);
   ret(create_int(int32Type, 1));

   // Safe division
   open_block(safe_div);
   ValueRef aVal = load_variable(int32Type, "a_val", a);
   ValueRef result = operation(aVal, "/", bVal);
   ValueRef resultVar = allocate_variable(int32Type, "result");
   assign(resultVar, result);
   branch(success);

   // Success
   open_block(success);
   ValueRef resultVal = load_variable(int32Type, "result_val", resultVar);
   ValueRef successMsg = create_string("Result: %d\n");
   ValueRef successArgs[] = {successMsg, resultVal};
   call_function(&printfFunc, "", successArgs, 2);
   ret(create_int(int32Type, 0));

   finalize();
   printf("\n=== Example 3: Division by Zero Check ===\n");
   printf("Generated: out.ll\n\n");
}

// Approach 4: Multiple Error Checks (Combined Example)
/*
main():
   char *str = NULL
   int arr[5]
   int index = -1
   
   // Check 1: NULL pointer
   if (str == NULL) goto null_error
   str[0] = 'a'
   goto check_bounds
   
null_error:
   printf("Error 1: NULL pointer!\n")
   return 1
   
check_bounds:
   // Check 2: Array bounds
   if (index < 0 || index >= 5) goto bounds_error
   arr[index] = 100
   goto success
   
bounds_error:
   printf("Error 2: Index out of bounds!\n")
   return 2
   
success:
   printf("All checks passed!\n")
   return 0
*/

void example4_multiple_errors()
{
   init("multiple_errors");

   // Declare printf
   TypeRef printfParams[] = {charPtrType};
   Foo printfFunc = {
      .name = "printf",
      .retType = int32Type,
      .paramTypes = printfParams,
      .paramCount = 1,
      .isVariadic = true
   };
   create_function(&printfFunc);

   // Create main
   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   BasicBlockRef null_error = create_bloc("null_error", &mainFunc);
   BasicBlockRef check_bounds = create_bloc("check_bounds", &mainFunc);
   BasicBlockRef check_upper = create_bloc("check_upper", &mainFunc);
   BasicBlockRef bounds_error = create_bloc("bounds_error", &mainFunc);
   BasicBlockRef success = create_bloc("success", &mainFunc);

   open_block(entry);

   // char *str = NULL
   ValueRef str = allocate_variable(charPtrType, "str");
   ValueRef nullPtr = LLVMConstPointerNull(charPtrType);
   assign(str, nullPtr);

   // int arr[5]
   ValueRef arr = allocate_stack(create_int(int32Type, 5), int32Type, "arr");

   // int index = -1
   ValueRef index = allocate_variable(int32Type, "index");
   assign(index, create_int(int32Type, -1));

   // Check 1: NULL pointer
   ValueRef strVal = load_variable(charPtrType, "str_val", str);
   ValueRef isNull = operation(strVal, "==", nullPtr);
   create_condition(isNull, null_error, check_bounds);

   // NULL error
   open_block(null_error);
   ValueRef nullErrorMsg = create_string("Error 1: NULL pointer access!\n");
   ValueRef nullErrorArgs[] = {nullErrorMsg};
   call_function(&printfFunc, "", nullErrorArgs, 1);
   ret(create_int(int32Type, 1));

   // Check bounds
   open_block(check_bounds);
   ValueRef indexVal = load_variable(int32Type, "index_val", index);
   ValueRef zero = create_int(int32Type, 0);
   ValueRef isNegative = operation(indexVal, "<", zero);
   create_condition(isNegative, bounds_error, check_upper);

   open_block(check_upper);
   ValueRef arraySize = create_int(int32Type, 5);
   ValueRef isOutOfBounds = operation(indexVal, ">=", arraySize);
   create_condition(isOutOfBounds, bounds_error, success);

   // Bounds error
   open_block(bounds_error);
   ValueRef boundsErrorMsg = create_string("Error 2: Array index out of bounds!\n");
   ValueRef boundsErrorArgs[] = {boundsErrorMsg};
   call_function(&printfFunc, "", boundsErrorArgs, 1);
   ret(create_int(int32Type, 2));

   // Success
   open_block(success);
   ValueRef successMsg = create_string("All checks passed!\n");
   ValueRef successArgs[] = {successMsg};
   call_function(&printfFunc, "", successArgs, 1);
   ret(create_int(int32Type, 0));

   finalize();
   printf("\n=== Example 4: Multiple Error Checks ===\n");
   printf("Generated: out.ll\n\n");
}

int main()
{
   printf("=================================================\n");
   printf("    LLVM Error Handling Examples\n");
   printf("=================================================\n");
   printf("\nNote: LLVM doesn't have built-in try/catch.\n");
   printf("These examples show manual error checking patterns.\n");
   printf("=================================================\n\n");

   // Run all examples
   example1_manual_null_check();
   example2_bounds_checking();
   example3_division_by_zero();
   example4_multiple_errors();

   printf("\n=================================================\n");
   printf("All examples generated successfully!\n");
   printf("=================================================\n");

   return 0;
}