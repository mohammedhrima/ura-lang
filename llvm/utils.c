#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <string.h>

LLVMContextRef context;
LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMTypeRef 
   voidType,
   float32Type,
   boolType,
   charType,
   int16Type,
   int32Type,
   int64Type,
   charPtrType,
   int32PtrType;

void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   // thread safety
   voidType     = LLVMVoidTypeInContext(context);
   float32Type  = LLVMFloatTypeInContext(context);
   boolType     = LLVMInt1TypeInContext(context);
   charType     = LLVMInt8TypeInContext(context);
   int16Type    = LLVMInt16TypeInContext(context);
   int32Type    = LLVMInt32TypeInContext(context);
   int64Type    = LLVMInt64TypeInContext(context);
   charPtrType  = LLVMPointerType(charType, 0);
   int32PtrType = LLVMPointerType(int32Type, 0);
   
   // Initialize targets for code generation
   LLVMInitializeAllTargetInfos();
   LLVMInitializeAllTargets();
   LLVMInitializeAllTargetMCs();
   LLVMInitializeAllAsmParsers();
   LLVMInitializeAllAsmPrinters();
}

LLVMBasicBlockRef create_bloc(char *name, LLVMValueRef parent)
{
   return LLVMAppendBasicBlockInContext(context, parent, name);
}

void create_branch(LLVMBasicBlockRef bloc)
{
   LLVMBuildBr(builder, bloc);
}

void open_block(LLVMBasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

typedef struct {
   char *name;
   LLVMTypeRef retType;
   LLVMTypeRef *paramTypes;
   int paramCount;

   LLVMTypeRef funcType;
   LLVMValueRef elem;
} Foo;

void create_function(Foo *func)
{
   func->funcType = LLVMFunctionType(func->retType, func->paramTypes, func->paramCount, 0);
   func->elem = LLVMAddFunction(module, func->name, func->funcType);
}

LLVMValueRef call_function(Foo *func, char *name, LLVMValueRef *args, int argsCount)
{
   return LLVMBuildCall2(builder, func->funcType, func->elem, args, argsCount, name);
}

LLVMValueRef create_string(char *value)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);
   return LLVMBuildGlobalStringPtr(builder, value, name);
}

// Create a mutable string on the stack
LLVMValueRef create_mutable_string(char *value)
{
   size_t len = strlen(value) + 1; // Include null terminator
   LLVMTypeRef arrayType = LLVMArrayType(charType, len);
   LLVMValueRef str_alloc = LLVMBuildAlloca(builder, arrayType, "str_alloc");
   
   // Copy each character
   for (size_t i = 0; i < len; i++) {
      LLVMValueRef indices[] = {
         LLVMConstInt(int32Type, 0, 0),
         LLVMConstInt(int32Type, i, 0)
      };
      LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, arrayType, str_alloc, 
                                            indices, 2, "elem");
      LLVMBuildStore(builder, LLVMConstInt(charType, value[i], 0), elem_ptr);
   }
   
   // Return pointer to first element
   LLVMValueRef indices[] = {
      LLVMConstInt(int32Type, 0, 0),
      LLVMConstInt(int32Type, 0, 0)
   };
   return LLVMBuildGEP2(builder, arrayType, str_alloc, indices, 2, "str_ptr");
}

LLVMValueRef create_int(LLVMTypeRef type, int value)
{
   return LLVMConstInt(type, value, 0);
}

LLVMValueRef create_float(float value)
{
   return LLVMConstReal(float32Type, value);
}

void create_condition(LLVMValueRef cond, LLVMBasicBlockRef isTrue, LLVMBasicBlockRef isFalse)
{
   LLVMBuildCondBr(builder, cond, isTrue, isFalse);
}

LLVMValueRef load_variable(LLVMTypeRef type, char *name, LLVMValueRef source)
{
   return LLVMBuildLoad2(builder, type, source, name);
}

LLVMValueRef allocate_variable(LLVMTypeRef type, char *name)
{
   return LLVMBuildAlloca(builder, type, name);
}

LLVMValueRef assign(LLVMValueRef variable, LLVMValueRef value)
{
   LLVMBuildStore(builder, value, variable);
   return variable;
}

LLVMValueRef operation(LLVMIntPredicate op, LLVMValueRef left, LLVMValueRef right)
{
   return LLVMBuildICmp(builder, op, left, right, "cmp");
}

LLVMValueRef math(char op, LLVMValueRef left, LLVMValueRef right)
{
   switch (op)
   {
   case '+': return LLVMBuildAdd(builder, left, right, "add");
   case '-': return LLVMBuildSub(builder, left, right, "sub");
   case '*': return LLVMBuildMul(builder, left, right, "mul");
   case '/': return LLVMBuildSDiv(builder, left, right, "div");
   case '%': return LLVMBuildSRem(builder, left, right, "rem");
   default:
      fprintf(stderr, "Unknown math operation: %c\n", op);
      return NULL;
   }
}

LLVMValueRef access(LLVMValueRef source, LLVMValueRef index, LLVMTypeRef elementType)
{
   LLVMValueRef indices[] = {index};
   return LLVMBuildGEP2(builder, elementType, source, indices, 1, "access");
}

// Call a function

void ret(LLVMValueRef value)
{
   if (value)
      LLVMBuildRet(builder, value);
   else
      LLVMBuildRetVoid(builder);
}

// Verify and print module
int verify_module()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
      return 0;
   }
   return 1;
}

void finalize()
{
   if (verify_module()) {
      LLVMPrintModuleToFile(module, "out.ll", NULL);
      printf("LLVM IR written to out.ll\n");
   }
   
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}
