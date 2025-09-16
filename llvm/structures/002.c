/*
This example creates a struct type `Point` with two i32 fields: x and y.
It creates two separate Point variables and demonstrates operations on both.
Fixed for LLVM 20+ using LLVMBuildStructGEP2 and LLVMBuildLoad2.
*/

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

/*
struct Point:
    int x
    int y

Variables created:
- point1: Point with values (10, 20)
- point2: Point with values (30, 40)
*/

int main()
{
   LLVMModuleRef mod = LLVMModuleCreateWithName("two_structs_example");
   LLVMBuilderRef builder = LLVMCreateBuilder();
   LLVMTypeRef i32 = LLVMInt32Type();

   // Define main function
   LLVMTypeRef mainType = LLVMFunctionType(i32, NULL, 0, 0);
   LLVMValueRef mainFunc = LLVMAddFunction(mod, "main", mainType);
   LLVMBasicBlockRef entry = LLVMAppendBasicBlock(mainFunc, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);

   // CREATE STRUCT TYPE
   LLVMTypeRef pointStruct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Point");
   LLVMTypeRef elements[] = {i32, i32};

   // SET STRUCT BODY
   LLVMStructSetBody(pointStruct, elements, 2, 0);

   // ===== FIRST STRUCT VARIABLE =====
   // ALLOCATE FIRST STRUCT
   LLVMValueRef point1 = LLVMBuildAlloca(builder, pointStruct, "point1");

   // SET point1.x = 10
   LLVMValueRef gep1X = LLVMBuildStructGEP2(builder, pointStruct, point1, 0, "point1_x");
   LLVMBuildStore(builder, LLVMConstInt(i32, 10, 0), gep1X);

   // SET point1.y = 20
   LLVMValueRef gep1Y = LLVMBuildStructGEP2(builder, pointStruct, point1, 1, "point1_y");
   LLVMBuildStore(builder, LLVMConstInt(i32, 20, 0), gep1Y);

   // ===== SECOND STRUCT VARIABLE =====
   // ALLOCATE SECOND STRUCT
   LLVMValueRef point2 = LLVMBuildAlloca(builder, pointStruct, "point2");

   // SET point2.x = 30
   LLVMValueRef gep2X = LLVMBuildStructGEP2(builder, pointStruct, point2, 0, "point2_x");
   LLVMBuildStore(builder, LLVMConstInt(i32, 30, 0), gep2X);

   // SET point2.y = 40
   LLVMValueRef gep2Y = LLVMBuildStructGEP2(builder, pointStruct, point2, 1, "point2_y");
   LLVMBuildStore(builder, LLVMConstInt(i32, 40, 0), gep2Y);

   // ===== DEMONSTRATE READING VALUES =====
   // Load values from point1 - Using LLVMBuildLoad2 for LLVM 20+
   LLVMValueRef point1_x_val = LLVMBuildLoad2(builder, i32, gep1X, "load_point1_x");
   LLVMValueRef point1_y_val = LLVMBuildLoad2(builder, i32, gep1Y, "load_point1_y");

   // Load values from point2 - Using LLVMBuildLoad2 for LLVM 20+
   LLVMValueRef point2_x_val = LLVMBuildLoad2(builder, i32, gep2X, "load_point2_x");
   LLVMValueRef point2_y_val = LLVMBuildLoad2(builder, i32, gep2Y, "load_point2_y");

   // ===== OPTIONAL: PERFORM OPERATIONS =====
   // Calculate sum of x coordinates: point1.x + point2.x
   LLVMValueRef sum_x = LLVMBuildAdd(builder, point1_x_val, point2_x_val, "sum_x");

   // Calculate sum of y coordinates: point1.y + point2.y
   LLVMValueRef sum_y = LLVMBuildAdd(builder, point1_y_val, point2_y_val, "sum_y");

   // Create a third point to store the result
   LLVMValueRef result_point = LLVMBuildAlloca(builder, pointStruct, "result_point");

   // Store sum_x into result_point.x
   LLVMValueRef result_x_gep = LLVMBuildStructGEP2(builder, pointStruct, result_point, 0, "result_x");
   LLVMBuildStore(builder, sum_x, result_x_gep);

   // Store sum_y into result_point.y
   LLVMValueRef result_y_gep = LLVMBuildStructGEP2(builder, pointStruct, result_point, 1, "result_y");
   LLVMBuildStore(builder, sum_y, result_y_gep);

   LLVMBuildRet(builder, LLVMConstInt(i32, 0, 0));

   // Output the generated LLVM IR
   LLVMPrintModuleToFile(mod, "out.ll", NULL);

   printf("LLVM IR generated successfully!\n");
   printf("Created:\n");
   printf("  - point1: (10, 20)\n");
   printf("  - point2: (30, 40)\n");
   printf("  - result_point: (40, 60) [sum of coordinates]\n");

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   return 0;
}