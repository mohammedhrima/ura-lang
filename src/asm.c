#include "./header.h"


// Add these global variables with your existing ones
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
// PTR will be handled dynamically

LLVMValueRef curr_func; // TODO: to be removed

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

LLVMTypeRef get_llvm_type(Type type)
{
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
   default: todo(1, "handle this case %s", to_string(type)); break;
   }
   return NULL;
}

// LLVMValueRef import_ref()
// {

// }

void handle_asm(Inst *inst)
{
   if(DEBUG) debug("%k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   LLVMValueRef leftRef, rightRef, ret = NULL;

   switch (curr->type)
   {
   case INT: case BOOL: case LONG: case SHORT: case CHAR:
   {
      if(curr->is_param)
      {
         ret = LLVMGetParam(curr->Param.func_ptr->llvm.element, curr->Param.index);
         LLVMSetValueName(ret, curr->name);
      }
      else if (curr->name)
      {
         if(curr->is_declare) ret = LLVMBuildAlloca(builder, get_llvm_type(curr->type), curr->name);
         else todo(1, "handle this case")
         curr->is_declare = false;
         // ret = LLVMBuildLoad2(builder, get_llvm_type(curr->type), ret, curr->name);
      }
      else
      {
         switch (curr->type)
         {
         case INT: ret = LLVMConstInt(get_llvm_type(curr->type), curr->Int.value, 0); break;
         case BOOL: ret = LLVMConstInt(get_llvm_type(curr->type), curr->Bool.value ? 1 : 0,   0); break;
         case LONG: ret = LLVMConstInt(get_llvm_type(curr->type), curr->Long.value, 0); break;
         // Assuming you store short in Int.value break;
         case SHORT: ret = LLVMConstInt(get_llvm_type(curr->type), curr->Int.value, 0); break;
         case CHAR: ret = LLVMConstInt(get_llvm_type(curr->type), curr->Char.value, 0); break;
         default:
            todo(1, "handle this literal case %s", to_string(curr->type));
            break;
         }
      }
      curr->llvm.is_set = true;
      curr->llvm.element = ret;
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
         switch (attr->type)
         {
         case INT:
            attrs[i] = int32Type;
            break;
         default:
            todo(1, "handle this case");
            break;
         }
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
      LLVMBuildStore(builder, right->llvm.element, left->llvm.element);
      break;
   }
   case ADD: case SUB: case MUL: case DIV:
   {
      if (left->name)
         leftRef = LLVMBuildLoad2(builder, get_llvm_type(left->type), left->llvm.element, left->name);
      else leftRef = left->llvm.element;
      if (right->name)
         rightRef = LLVMBuildLoad2( builder, get_llvm_type(left->type), right->llvm.element, right->name);
      else rightRef = right->llvm.element;

      Type op = curr->type;
      switch (curr->type)
      {
      case ADD: ret = LLVMBuildAdd(builder, leftRef, rightRef, to_string(op)); break;
      case SUB: ret = LLVMBuildSub(builder, leftRef, rightRef, to_string(op)); break;
      case MUL: ret = LLVMBuildMul(builder, leftRef, rightRef, to_string(op)); break;
      case DIV: ret = LLVMBuildSDiv(builder, leftRef, rightRef, to_string(op)); break;
      default: todo(1, "handle this")
      }
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case AND: case OR:
   {
      if (check(!left->llvm.is_set, "left is not set\n")) break;
      if (check(!right->llvm.is_set, "right is not set\n")) break;

      leftRef = left->llvm.element;
      rightRef = right->llvm.element;

      ret = curr->type == AND ?
            LLVMBuildAnd(builder, leftRef, rightRef, "and") :
            LLVMBuildOr(builder, leftRef, rightRef, "or");
      curr->llvm.element = ret;
      curr->llvm.is_set = true;

      break;
   }
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   {
      leftRef = left->llvm.element;
      rightRef = right->llvm.element;

      Type op = curr->type;
      switch (curr->type)
      {
      case LESS:
         ret = LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, to_string(op));
         break;
      case LESS_EQUAL:
         ret = LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, to_string(op));
         break;
      case MORE:
         ret = LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, to_string(op));
         break;
      case MORE_EQUAL:
         ret = LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, to_string(op));
         break;
      case EQUAL:
         ret = LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, to_string(op));
         break;
      case NOT_EQUAL:
         ret = LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, to_string(op));
         break;
      default: todo(1, "handle this")
      }
      // printf(BLUE"set is_set %p, %s\n"RESET, curr, to_string(curr->type));
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
            args[i] = curr->Fcall.args[i]->llvm.element;
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
            args[i] = get_llvm_type(curr->Fdec.args[i]->type);
         curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr->retType), args, curr->Fdec.pos, 0);
         free(args);
      }
      else curr->llvm.funcType = LLVMFunctionType(get_llvm_type(curr->retType), NULL, 0, 0);

      curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);
      LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element, "entry");
      LLVMPositionBuilderAtEnd(builder, funcEntry);

      // if (strcmp(curr->name, "main") == 0)
      {
         LLVMPositionBuilderAtEnd(builder, funcEntry);
         curr_func = curr->llvm.element; // TODO: to be removed
      }
      curr->llvm.is_set = true;
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
            // Load variable and return it
            ret = LLVMBuildRet(builder, left->llvm.element);
         }
         else
         {
            // Return literal constant
            LLVMValueRef constant = NULL;
            switch (left->type)
            {
            case INT: constant = LLVMConstInt(get_llvm_type(left->type), left->Int.value, 0); break;
            case BOOL: constant = LLVMConstInt(get_llvm_type(left->type), left->Bool.value ? 1 : 0, 0); break;
            case LONG: constant = LLVMConstInt(get_llvm_type(left->type), left->Long.value, 0); break;
            // Assuming SHORT stored in Int.value
            case SHORT: constant = LLVMConstInt(get_llvm_type(left->type), left->Int.value, 0); break;
            case CHAR: constant = LLVMConstInt(get_llvm_type(left->type), left->Char.value, 0); break;
            case FLOAT: constant = LLVMConstReal(get_llvm_type(left->type), left->Float.value); break;
            default: todo(1, "unhandled constant type in return"); break;
            }
            ret = LLVMBuildRet(builder, constant);
         }
         break;
      }
      case VOID:
      {
         // Void return - no value
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
   case END_BLOC: break;
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

   for (i = 0; insts[i]; ) {
      handle_asm(insts[i]);
      i++;
   }

   // debug("module name: [%s]\n", moduleName);
   strcpy(moduleName + strlen(moduleName) - 2, "ll");
   LLVMPrintModuleToFile(mod, moduleName, NULL);

   // Cleanup
   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(mod);
   LLVMContextDispose(context); // Don't forget to dispose the context
   free(moduleName);
#endif
}
