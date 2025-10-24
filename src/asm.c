#include "./header.h"

// Global variables
LLVMModuleRef mod;
LLVMBuilderRef builder;
LLVMContextRef context;

// Type declarations
LLVMTypeRef voidType;
LLVMTypeRef int32Type;
LLVMTypeRef floatType;
LLVMTypeRef int64Type;    // long
LLVMTypeRef int16Type;    // short
LLVMTypeRef boolType;     // bool (i1)
LLVMTypeRef charType;     // char (i8)
LLVMTypeRef charsType;    // char* (pointer to i8)

LLVMValueRef func_arr[100];
LLVMValueRef curr_func; // TODO: to be removed

static int func_pos = 0;
void enter_func(LLVMValueRef func)
{
   func_arr[func_pos] = func;
   curr_func = func_arr[func_pos];
   func_pos++;
}

void exit_func()
{
   func_pos--;
   curr_func = func_arr[func_pos];
}

int i = 0;

// Initialize types in generate_asm function
void init_llvm_types() {
   voidType = LLVMVoidTypeInContext(context);
   int32Type = LLVMInt32TypeInContext(context);
   floatType = LLVMFloatTypeInContext(context);
   int64Type = LLVMInt64TypeInContext(context);
   int16Type = LLVMInt16TypeInContext(context);
   boolType = LLVMInt1TypeInContext(context);     // 1-bit integer for bool
   charType = LLVMInt8TypeInContext(context);     // 8-bit integer for char
   charsType = LLVMPointerType(charType, 0);      // char* for strings
}

LLVMTypeRef get_llvm_type(Token *token)
{
   Type type = includes(token->type, FDEC, FCALL, ACCESS, 0) ? token->retType : token->type;
   switch (type)
   {
   case VOID: return voidType;
   case INT: return int32Type;
   case FLOAT: return floatType;
   case LONG: return int64Type;
   case SHORT: return int16Type;
   case BOOL: return boolType;
   case CHAR: return charType;
   case CHARS: return charsType;
   case PTR:
      // For PTR, you'll need additional context about what it points to
      // This is a placeholder - you might need to pass the pointed-to type
      return LLVMPointerType(int32Type, 0); // Default to int* for now
   default:
      todo(1, "handle this case %s", to_string(type)); break;
      seg();
   }
   return NULL;
}

LLVMValueRef get_value(Token *token)
{
   LLVMTypeRef llvmType = get_llvm_type(token);
   switch (token->type)
   {
   case INT: return LLVMConstInt(llvmType, token->Int.value, 0);
   case FLOAT: return LLVMConstReal(llvmType, token->Float.value);
   case BOOL: return LLVMConstInt(llvmType, token->Bool.value ? 1 : 0, 0);
   case LONG: return LLVMConstInt(llvmType, token->Long.value, 0);
   case SHORT: return LLVMConstInt(llvmType, token->Short.value, 0);
   case CHAR: return LLVMConstInt(llvmType, token->Char.value, 0);
   case CHARS:
   {
      LLVMValueRef str_constant = LLVMConstStringInContext(context, token->Chars.value,
                                  strlen(token->Chars.value), 0);
      // 0 means null-terminate
      LLVMValueRef global_str = LLVMAddGlobal(mod, LLVMTypeOf(str_constant), "str_literal");
      LLVMSetInitializer(global_str, str_constant);
      LLVMSetLinkage(global_str, LLVMPrivateLinkage);
      LLVMSetGlobalConstant(global_str, 1);
      return global_str;
   }
   default: todo(1, "handle this literal case %s", to_string(token->type));
   }
   return (LLVMValueRef) {};
}

LLVMValueRef get_llvm_op(Token *token, Token* left, Token* right)
{
   LLVMValueRef leftRef = NULL, rightRef = NULL;
   if (left->name && !left->is_param && left->type != FCALL)
      leftRef = LLVMBuildLoad2(builder, get_llvm_type(left), left->llvm.element, left->name);
   else leftRef = left->llvm.element;

   if (right->name && !right->is_param && right->type != FCALL)
      rightRef = LLVMBuildLoad2(builder, get_llvm_type(left), right->llvm.element, right->name);
   else rightRef = right->llvm.element;

   char* op = to_string(token->type);
   Type type = token->type == FCALL ? token->retType : token->type;
   switch (type)
   {
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, op); break;
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, op); break;
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, op); break;
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, op); break;
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, op); break;
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, op); break;
   case ADD: return LLVMBuildAdd(builder, leftRef, rightRef, op); break;
   case SUB: return LLVMBuildSub(builder, leftRef, rightRef, op); break;
   case MUL: return LLVMBuildMul(builder, leftRef, rightRef, op); break;
   case DIV: return LLVMBuildSDiv(builder, leftRef, rightRef, op); break;
   case AND: return LLVMBuildAnd(builder, leftRef, rightRef, op); break;
   case OR: return LLVMBuildOr(builder, leftRef, rightRef, op); break;
   default: todo(1, "handle this %s", op);
   }
   return (LLVMValueRef) {};
}

void handle_asm(Inst *inst)
{
   if (DEBUG) debug("%k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   LLVMValueRef ret = NULL;

   switch (curr->type)
   {
   case INT: case BOOL: case LONG: case SHORT: case CHAR: case CHARS: case PTR:
   {
      if (curr->is_param)
      {
         ret = LLVMGetParam(curr->Param.func_ptr->llvm.element, curr->Param.index);
         LLVMSetValueName(ret, curr->name);
      }
      else if (curr->name)
      {
         if (curr->is_declare) ret = LLVMBuildAlloca(builder, get_llvm_type(curr), curr->name);
         else todo(1, "handle this case");
         curr->is_declare = false;
         curr->is_param = false;
      }
      else ret = get_value(curr);

      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case STRUCT_DEF:
   {
      curr->llvm.is_set = true;
      // CREATE STRUCT TYPE
      // SET STRUCT BODY
      curr->llvm.structType = LLVMStructCreateNamed(LLVMGetGlobalContext(), curr->Struct.name);
      int pos = curr->Struct.pos;
      LLVMTypeRef *attrs = allocate(pos, sizeof(LLVMTypeRef));
      for (int i = 0; i < pos; i++)
      {
         Token *attr = curr->Struct.attrs[i];
         stop(!attr, "attribite is NULL\n");
         attrs[i] = get_llvm_type(attr);

      }
      // SET STRUCT BODY
      LLVMStructSetBody(curr->llvm.structType, attrs, pos, 0);
      free(attrs);
      break;
   }
   case STRUCT_CALL:
   {
      curr->llvm.element = LLVMBuildAlloca(builder, curr->Struct.ptr->llvm.structType, curr->name);
      curr->llvm.is_set = true;
      break;
   }
   case DOT:
   {
      LLVMValueRef st_call = left->llvm.element;
      LLVMTypeRef st_type = left->Struct.ptr->llvm.structType;
      int index = right->Struct.attr_index; // attribute position
      curr->llvm.element = LLVMBuildStructGEP2(builder, st_type, st_call, index, right->name);
      curr->llvm.is_set = true;
      curr->type = right->type;
      curr->name = strdup(right->name);
      break;
   }
   case ASSIGN:
   {
      check(!left->llvm.is_set, "assign, left is not set");
      check(!right->llvm.is_set, "assign, right is not set");
      
      // FIXED: Check if left side is an ACCESS operation (has ptr set)
      // Use the pointer for ACCESS, otherwise use element
      LLVMValueRef target = left->llvm.ptr ? left->llvm.ptr : left->llvm.element;
      LLVMBuildStore(builder, right->llvm.element, target);
      break;
   }
   case ADD: case SUB: case MUL: case DIV:
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   case AND: case OR:
   {
      if (check(!left->llvm.is_set, "left is not set\n")) break;
      if (check(!right->llvm.is_set, "right is not set\n")) break;
      ret = get_llvm_op(curr, left, right);
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case FCALL:
   {
      LLvm srcFunc = curr->Fcall.ptr->llvm;
      LLVMValueRef *args = NULL;
      if (curr->Fcall.pos)
      {
         args = allocate(curr->Fcall.pos, sizeof(LLVMValueRef));
         for (int i = 0; i < curr->Fcall.pos; i++)
         {
            Token *arg = curr->Fcall.args[i];
            check(!arg->llvm.is_set, "llvm is not set");
            
            // Load the value if it's a variable, otherwise use the element directly
            if (arg->name && !arg->is_param && arg->type != FCALL)
               args[i] = LLVMBuildLoad2(builder, get_llvm_type(arg), arg->llvm.element, arg->name);
            else
               args[i] = arg->llvm.element;
         }
         curr->llvm.element = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.element, args,
                                             curr->Fcall.pos, curr->name);
         free(args);
      }
      else
         curr->llvm.element = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.element,
                                             NULL, 0, curr->name);

      curr->llvm.is_set = true;
      break;
   }
   case FDEC:
   {
      // debug("FDEC: ", curr->name);
      LLVMTypeRef *args = NULL;

      if (curr->Fdec.pos)
      {
         args = allocate(curr->Fdec.pos + 1, sizeof(LLVMTypeRef));
         for (int i = 0; i < curr->Fdec.pos; i++)
            args[i] = get_llvm_type(curr->Fdec.args[i]);
         curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), args, curr->Fdec.pos, 0);
         free(args);
      }
      else curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr), NULL, 0, 0);

      curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);

      if (!curr->is_proto)
      {
         LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element, "entry");
         LLVMPositionBuilderAtEnd(builder, funcEntry);
         LLVMPositionBuilderAtEnd(builder, funcEntry);
      }

      enter_func(curr->llvm.element);
      curr->llvm.is_set = true;
      break;
   }
   case END_BLOC:
   {
      exit_func();
      break;
   }
   case RETURN:
   {
      if (check(!left->llvm.is_set, "return result is not set\n")) break;
      switch (left->type)
      {
      case FCALL:
      {
         ret = LLVMBuildRet(builder, left->llvm.element);
         break;
      }
      case LESS: case LESS_EQUAL: case MORE:
      case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
      case AND: case OR:
      {
         ret = LLVMBuildRet(builder, left->llvm.element);
         break;
      }
      case INT: case BOOL: case LONG: case SHORT: case CHAR: case FLOAT:
      {
         if (left->name || left->is_attr)
         {
            // FIXED: Load the value before returning
            LLVMValueRef loaded = LLVMBuildLoad2(builder, get_llvm_type(left), 
                                                 left->llvm.element, left->name);
            ret = LLVMBuildRet(builder, loaded);
         }
         else
            ret = LLVMBuildRet(builder, get_value(left));
         break;
      }
      case VOID:
      {
         ret = LLVMBuildRetVoid(builder);
         break;
      }
      default:
         todo(1, "handle this case %s\n", to_string(left->type));
         break;
      }
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case APPEND_BLOC:
   {
      check(!left->name, "APPEND BLOC require a name");
      left->llvm.bloc = LLVMAppendBasicBlockInContext(context, curr_func, left->name);
      curr->llvm.is_set = true;
      break;
   }
   case BUILD_COND:
   {
      LLVMValueRef cond = curr->Statement.ptr->llvm.element;
      LLVMBasicBlockRef start = left->llvm.bloc;
      LLVMBasicBlockRef end = right->llvm.bloc;
      curr->llvm.element = LLVMBuildCondBr(builder, cond, start, end);
      curr->llvm.is_set = true;
      break;
   }
   case ACCESS:
   {
      check(!left->llvm.is_set, "left is not set");
      check(!right->llvm.is_set, "right is not set");

      LLVMValueRef leftRef = NULL, rightRef = NULL;
      if (left->name && !left->is_param && left->type != FCALL)
         leftRef = LLVMBuildLoad2(builder, get_llvm_type(left), left->llvm.element, left->name);
      else leftRef = left->llvm.element;

      if (right->name && !right->is_param && right->type != FCALL)
         rightRef = LLVMBuildLoad2(builder, get_llvm_type(right), right->llvm.element, right->name);
      else rightRef = right->llvm.element;

      LLVMValueRef indices[] = { rightRef };
      LLVMValueRef elem_ptr = LLVMBuildGEP2(builder, charType, leftRef, indices, 1, "access");
      
      // FIXED: Store the pointer separately for use in assignments
      curr->llvm.ptr = elem_ptr;
      
      // Load the value for use in expressions
      curr->llvm.element = LLVMBuildLoad2(builder, charType, elem_ptr, "access_val");
      curr->llvm.is_set = true;
      curr->type = CHAR;
      curr->name = NULL;  // Clear name to prevent double-loading
      break;
   }
   case SET_POS:
   {
      LLVMPositionBuilderAtEnd(builder, left->llvm.bloc);
      break;
   }
   case BUILD_BR:
   {
      check(!left->name, "BUILD BR require a name");
      LLVMBuildBr(builder, left->llvm.bloc);
      break;
   }
   default: todo(1, "handle this case (%s)\n", to_string(curr->type)); break;
   }
}

void generate_asm(char *name)
{
#if ASM
   if (found_error) return;

   // Create LLVM context, module, and IR builder
   char *moduleName = resolve_path(name);
   context = LLVMContextCreate();
   mod = LLVMModuleCreateWithName(moduleName);
   builder = LLVMCreateBuilder();

   // Initialize all types
   init_llvm_types();

   for (i = 0; insts[i]; i++) handle_asm(insts[i]);

   // debug("module name: [%s]\n", moduleName);
   int len = strlen(moduleName);
   strcpy(moduleName + len - 3, "ll");
   moduleName[len - 1] = '\0';
   LLVMPrintModuleToFile(mod, moduleName, NULL);

   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   LLVMContextDispose(context); // Don't forget to dispose the context
   free(moduleName);
#endif
}