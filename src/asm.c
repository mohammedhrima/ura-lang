#include "./include/header.h"

LLVMModuleRef mod;
LLVMBuilderRef builder;
LLVMTypeRef int32Type;
LLVMContextRef context;

LLVMValueRef main_func; // TODO: to be removed

int i = 0;
void generate_if(Token *token)
{
    check(!token->ifCond.ptr->llvm.is_set, "condition is not set");
    
    LLVMValueRef cond = token->ifCond.ptr->llvm.element;
    
    LLVMBasicBlockRef if_beg = LLVMAppendBasicBlockInContext(
        context,
         main_func,
          "start_if");
    LLVMBasicBlockRef if_end = LLVMAppendBasicBlockInContext(context, main_func, "end_if");

    LLVMBuildCondBr(builder, cond, if_beg, if_end);

    LLVMPositionBuilderAtEnd(builder, if_beg);
    i++;
    while(insts[i] && insts[i]->token->type != END_IF)
    {
        handle_ir(insts[i]);
        i++;
    }
    check(insts[i]->token->type != END_IF, "handling if statement");
    i++;
    LLVMBuildBr(builder, if_end);
    LLVMPositionBuilderAtEnd(builder, if_end);
    debug(RED"current inst: %k\n"RESET, insts[i]->token);
}

void handle_ir(Inst *inst)
{
    debug("handle ir: %k\n", inst->token);
    Token *curr = inst->token;
    Token *left = inst->left;
    Token *right = inst->right;
    switch (curr->type)
    {
    case INT:
    {
        if (curr->name) curr->llvm.element = LLVMBuildAlloca(builder, int32Type, curr->name);
        else
        {
            curr->llvm.element = LLVMConstInt(int32Type, curr->Int.value, 0);
        } 
        curr->llvm.is_set = true;
        break;
    }
    case ASSIGN:
    {
        LLVMBuildStore(builder, right->llvm.element, left->llvm.element);
        break;
    }
    case ADD: case SUB: case MUL: case DIV:
    {
        LLVMValueRef leftRef = left->name ?
                               LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name) : left->llvm.element;
        LLVMValueRef rightRef = right->name ?
                                LLVMBuildLoad2(builder, int32Type, right->llvm.element, right->name) : right->llvm.element;
        LLVMValueRef ret;
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
    case LESS: case LESS_EQUAL: case MORE: case MORE_EQUAL:
    case EQUAL: case NOT_EQUAL:
    {
        LLVMValueRef leftRef = left->name ?
                               LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name) : left->llvm.element;
        LLVMValueRef rightRef = right->name ?
                                LLVMBuildLoad2(builder, int32Type, right->llvm.element, right->name) : right->llvm.element;
        LLVMValueRef ret;
        Type op = curr->type;
        switch (curr->type)
        {
        case LESS:       ret = LLVMBuildICmp(builder, LLVMIntSLT, leftRef, rightRef, to_string(op)); break;
        case LESS_EQUAL: ret = LLVMBuildICmp(builder, LLVMIntSLE, leftRef, rightRef, to_string(op)); break;
        case MORE:       ret = LLVMBuildICmp(builder, LLVMIntSGT, leftRef, rightRef, to_string(op)); break;
        case MORE_EQUAL: ret = LLVMBuildICmp(builder, LLVMIntSGE, leftRef, rightRef, to_string(op)); break;
        case EQUAL:      ret = LLVMBuildICmp(builder, LLVMIntEQ,  leftRef, rightRef, to_string(op)); break;
        case NOT_EQUAL:  ret = LLVMBuildICmp(builder, LLVMIntNE,  leftRef, rightRef, to_string(op)); break;
        default: todo(1, "handle this")
        }
        curr->llvm.element = ret;
        curr->llvm.is_set = true;
        break;
    }
    case FCALL:
    {
        LLvm fcall = curr->Fcall.ptr->llvm;
        curr->llvm.element = LLVMBuildCall2(builder, fcall.funcType, fcall.element, NULL, 0, "");
        curr->llvm.is_set = true;
        break;
    }
    case FDEC:
    {
        // debug("FDEC: ", curr->name);
        curr->llvm.funcType = LLVMFunctionType(int32Type, NULL, 0, 0);
        curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);
        LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element, "entry");
        LLVMPositionBuilderAtEnd(builder, funcEntry);

        if (strcmp(curr->name, "main") == 0)
        {
            LLVMPositionBuilderAtEnd(builder, funcEntry);
            main_func = curr->llvm.element; // TODO: to be removed
        }
        curr->llvm.is_set = true;
        break;
    }
    case IF:
    {
        generate_if(curr);
        break;
    }
    case RETURN:
    {
        LLVMValueRef ret;
        switch (left->type)
        {
        case FCALL:
        {
            ret = LLVMBuildRet(builder, left->llvm.element);
            break;
        }
        case INT:
        {
            debug("%k\n", left);
            todo(!left->llvm.is_set, "need to be set\n");
            
            debug(RED"return %k\n"RESET, left);
            if (left->name)
            {
                ret = LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name);
                ret = LLVMBuildRet(builder, ret);
            }
            else ret = LLVMBuildRet(builder, LLVMConstInt(int32Type, left->Int.value, 0));
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
    int32Type = LLVMInt32Type();

    for ( ; insts[i]; i++) handle_ir(insts[i]);
    strcpy(moduleName + strlen(moduleName) - 1, "ir");

    LLVMPrintModuleToFile(mod, moduleName, NULL);
    // Cleanup
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    free(moduleName);
#endif
}