#include "../utils.c"

/*
int sum(int count, ...)
{
   va_list args;
   va_start(args, count);

   int total = 0;
   int i = 0;
   while (i < count)
   {
      total += va_arg(args, int);
      i++;
   }
   va_end(args);
   return total;
}

int main()
{
   int result = sum(3, 10, 20, 30);
   return result;
}
*/

int main()
{
   init("module");

   Foo vaStart = {.name = "llvm.va_start.p0", .retType = voidType, .paramTypes = (TypeRef[]){charPtrType}, .paramCount = 1};
   create_function(&vaStart);

   Foo vaEnd = {.name = "llvm.va_end.p0", .retType = voidType, .paramTypes = (TypeRef[]){charPtrType}, .paramCount = 1};
   create_function(&vaEnd);

   Foo sumFunc = {.name = "sum", .retType = int32Type, .paramTypes = (TypeRef[]){int32Type}, .paramCount = 1, .isVariadic = true};
   create_function(&sumFunc);

   ValueRef countParam = get_param(&sumFunc, 0, "count");

   BasicBlockRef entry = create_bloc("entry", &sumFunc);
   BasicBlockRef loopHeader = create_bloc("loop_header", &sumFunc);
   BasicBlockRef loopBody = create_bloc("loop_body", &sumFunc);
   BasicBlockRef loopExit = create_bloc("loop_exit", &sumFunc);

   open_block(entry);

   TypeRef vaListType = LLVMArrayType(charType, 24);
   ValueRef argsAlloca = allocate_variable(vaListType, "args");

   ValueRef totalAlloca = allocate_variable(int32Type, "total");
   assign(totalAlloca, create_int(int32Type, 0));

   ValueRef iAlloca = allocate_variable(int32Type, "i");
   assign(iAlloca, create_int(int32Type, 0));

   ValueRef argsI8Ptr = LLVMBuildBitCast(builder, argsAlloca, charPtrType, "args_i8");
   ValueRef vaStartArgs[] = {argsI8Ptr};
   call_function(&vaStart, "", vaStartArgs, 1);

   branch(loopHeader);

   open_block(loopHeader);
   ValueRef iVal = load_variable(int32Type, "i_val", iAlloca);
   ValueRef cmp = operation(iVal, "<", countParam);
   create_condition(cmp, loopBody, loopExit);

   open_block(loopBody);

   ValueRef indices[] = {
       create_int(int32Type, 0),
       operation(iVal, "*", create_int(int32Type, 8))
   };
   ValueRef argPtr = LLVMBuildInBoundsGEP2(builder, vaListType, argsAlloca, indices, 2, "arg_ptr");
   ValueRef argPtrCast = LLVMBuildBitCast(builder, argPtr, int32PtrType, "arg_ptr_cast");
   ValueRef argVal = load_variable(int32Type, "arg_val", argPtrCast);

   ValueRef totalVal = load_variable(int32Type, "total_val", totalAlloca);
   ValueRef newTotal = operation(totalVal, "+", argVal);
   assign(totalAlloca, newTotal);

   ValueRef newI = operation(iVal, "+", create_int(int32Type, 1));
   assign(iAlloca, newI);

   branch(loopHeader);

   open_block(loopExit);

   ValueRef vaEndArgs[] = {argsI8Ptr};
   call_function(&vaEnd, "", vaEndArgs, 1);

   ValueRef finalTotal = load_variable(int32Type, "final_total", totalAlloca);
   ret(finalTotal);

   Foo mainFunc = {.name = "main", .retType = int32Type};
   create_function(&mainFunc);

   BasicBlockRef mainEntry = create_bloc("entry", &mainFunc);
   open_block(mainEntry);

   ValueRef sumArgs[] = {
      create_int(int32Type, 3),
      create_int(int32Type, 10),
      create_int(int32Type, 20),
      create_int(int32Type, 30)
   };
   ValueRef result = call_function(&sumFunc, "result", sumArgs, 4);

   ret(result);

   finalize();

   return 0;
}