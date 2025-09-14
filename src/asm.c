#include "./header.h"

LLVMModuleRef mod;
LLVMBuilderRef builder;
LLVMTypeRef int32Type;
LLVMContextRef context;

LLVMValueRef main_func; // TODO: to be removed

int i = 0;

void handle_asm(Inst *inst)
{
   debug("%k\n", inst->token);
   Token *curr = inst->token;
   Token *left = inst->left;
   Token *right = inst->right;
   LLVMValueRef leftRef, rightRef, ret = NULL;

   switch (curr->type)
   {
   case INT:
   {
      if (curr->name) 
         curr->llvm.element = LLVMBuildAlloca(builder, int32Type, curr->name);
      else 
         curr->llvm.element = LLVMConstInt(int32Type, curr->Int.value, 0);
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
         if(attr) debug(">>> %k\n", attr);
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
      curr->llvm.element = LLVMBuildAlloca(builder,
      curr->Struct.ptr->llvm.structType, curr->name);
      curr->llvm.is_set = true;
      break;
   }
   case ACCESS:
   {
      todo(1, "access");
      // LLVMValueRef gep1X = LLVMBuildStructGEP(builder, point1, 0, "point1_x");
      LLVMValueRef structRef = left->llvm.element;
      int index = right->Struct.attr_index; // attribute position
      curr->llvm.element = LLVMBuildStructGEP(builder, structRef, index, "access");
      break;
   }
   case ASSIGN:
   {
      LLVMBuildStore(builder, right->llvm.element, left->llvm.element);
      break;
   }
   case ADD: case SUB: case MUL: case DIV:
   {
      if (left->name) 
         leftRef = LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name);
      else leftRef = left->llvm.element;
      if (right->name) 
         rightRef = LLVMBuildLoad2( builder, int32Type, right->llvm.element, right->name);
      else rightRef = right->llvm.element;

      Type op = curr->type;
      switch (curr->type)
      {
      case ADD:
         ret = LLVMBuildAdd(builder, leftRef, rightRef, to_string(op));
         break;
      case SUB:
         ret = LLVMBuildSub(builder, leftRef, rightRef, to_string(op));
         break;
      case MUL:
         ret = LLVMBuildMul(builder, leftRef, rightRef, to_string(op));
         break;
      case DIV:
         ret = LLVMBuildSDiv(builder, leftRef, rightRef, to_string(op));
         break;
      default: todo(1, "handle this")
      }
      curr->llvm.element = ret;
      curr->llvm.is_set = true;
      break;
   }
   case LESS: case LESS_EQUAL: case MORE:
   case MORE_EQUAL: case EQUAL: case NOT_EQUAL:
   {
      if (left->name) leftRef = LLVMBuildLoad2(builder, int32Type,
                                   left->llvm.element, left->name);
      else leftRef = left->llvm.element;
      if (right->name) rightRef = LLVMBuildLoad2(builder, int32Type,
                                     right->llvm.element, right->name);
      else rightRef = right->llvm.element;

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
      if(curr->Fcall.pos)
      {
         args = allocate(curr->Fcall.pos, sizeof(LLVMValueRef));
         for(int i = 0; i < curr->Fcall.pos; i++)
         {
            Token *arg = curr->Fcall.args[i];
            debug(">> [%k]\n", arg);
            check(!arg->llvm.is_set, "llvm is not set");
            args[i] = curr->Fcall.args[i]->llvm.element;
         }
         curr->llvm.element = LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.element,
            args, curr->Fcall.pos, curr->name);
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
      
      if(curr->Fdec.pos)
      {
         args = allocate(curr->Fdec.pos + 1, sizeof(LLVMTypeRef));
         for(int i = 0; i < curr->Fdec.pos; i++)
         {
            switch (curr->Fdec.args[i]->type)
            {
            case INT:
               args[i] = int32Type;
               break;
            default:
               check(1, "handle this case");
               break;
            }
         }
         curr->llvm.funcType = LLVMFunctionType(int32Type, args, curr->Fdec.pos, 0);
         free(args);
      }
      else curr->llvm.funcType = LLVMFunctionType(int32Type, NULL, 0, 0);

      curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);
      LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element,
                                    "entry");
      LLVMPositionBuilderAtEnd(builder, funcEntry);

      if (strcmp(curr->name, "main") == 0)
      {
         LLVMPositionBuilderAtEnd(builder, funcEntry);
         main_func = curr->llvm.element; // TODO: to be removed
      }
      curr->llvm.is_set = true;
      break;
   }
   case RETURN:
   {
      switch (left->type)
      {
      case FCALL:
      {
         ret = LLVMBuildRet(builder, left->llvm.element);
         break;
      }
      case INT:
      {
         // debug(">>>> %k\n\n", left);
         // if(!left->llvm.is_set)
         // {
         //     debug(RED"variable is not set\n"RESET);
         //     // exit(1);
         // }
         // stop(!left->llvm.is_set, "helloooooooooo\n", "");
         // if(!left->llvm.is_set)
         //     check_error(FILE, FUNC, LINE, true, "found error"); exit(1);

         // debug(RED"return %k\n\n"RESET, left);
         if (left->name)
         {
            ret = LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name);
            ret = LLVMBuildRet(builder, ret);
         }
         else
            ret = LLVMBuildRet(builder, LLVMConstInt(int32Type, left->Int.value, 0));
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
      left->llvm.bloc =
         LLVMAppendBasicBlockInContext(context, main_func, left->name);
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
   // context = LLVMGetGlobalContext();
   context = LLVMContextCreate();
   mod = LLVMModuleCreateWithName(moduleName);
   builder = LLVMCreateBuilder();
   int32Type = LLVMInt32Type();

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
   free(moduleName);
#endif
}
