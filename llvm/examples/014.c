#include "../utils.c"

/*
Pseudo code to implement:
main():
   chars a = stack(2) as chars
   a[-1] = 'a'  // This should trigger bounds check error
   a[0] = 'H'   // This is valid
   a[1] = 'i'   // This is valid
   a[2] = '!'   // This should trigger bounds check error
*/

int main()
{
   init("bounds_check_example");
   
   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);
   
   BasicBlockRef entry = create_bloc("entry", &mainFunc);
   open_block(entry);
   
   // chars a = stack(2) as chars
   ValueRef a = allocate_stack(create_int(int32Type, 2), charType, "a");
   
   printf("\n╔════════════════════════════════════════╗\n");
   printf("║     Bounds Check Demo - No Parser      ║\n");
   printf("╚════════════════════════════════════════╝\n\n");
   
   printf("Creating array: chars a[2]\n\n");
   
   // Test 1: a[-1] = 'a' (OUT OF BOUNDS - negative index)
   printf("Test 1: a[-1] = 'a'\n");
   printf("  Expected: Runtime error (negative index)\n");
   ValueRef ptr1 = safe_access(a, create_int(int32Type, -1), charType, 2, 10, 5);
   assign(ptr1, create_int(charType, 'a'));
   
   // Test 2: a[0] = 'H' (VALID)
   printf("\nTest 2: a[0] = 'H'\n");
   printf("  Expected: Success\n");
   ValueRef ptr2 = safe_access(a, create_int(int32Type, 0), charType, 2, 15, 5);
   assign(ptr2, create_int(charType, 'H'));
   
   // Test 3: a[1] = 'i' (VALID)
   printf("\nTest 3: a[1] = 'i'\n");
   printf("  Expected: Success\n");
   ValueRef ptr3 = safe_access(a, create_int(int32Type, 1), charType, 2, 20, 5);
   assign(ptr3, create_int(charType, 'i'));
   
   // Test 4: a[2] = '!' (OUT OF BOUNDS - index >= size)
   printf("\nTest 4: a[2] = '!'\n");
   printf("  Expected: Runtime error (index out of bounds)\n");
   ValueRef ptr4 = safe_access(a, create_int(int32Type, 2), charType, 2, 25, 5);
   assign(ptr4, create_int(charType, '!'));
   
   ret(create_int(int32Type, 0));
   
   printf("\n════════════════════════════════════════\n");
   
   finalize();
   
   return 0;
}