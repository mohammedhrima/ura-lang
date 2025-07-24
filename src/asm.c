#include "./include/header.h"

void generate_asm(char *name)
{
    if (found_error) return;
    
    // Create LLVM context, module, and IR builder
    LLVMModuleRef mod = LLVMModuleCreateWithName(name);
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32Type();

    copy_insts();
    for (int i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
        switch (curr->type)
        {
        case INT:
        {
            if(curr->name) curr->llvm.element = LLVMBuildAlloca(builder, int32Type, curr->name);
            else curr->llvm.element = LLVMConstInt(int32Type, curr->Int.value, 0);
            break;
        }
        case ASSIGN:
        {
            LLVMBuildStore(builder, right->llvm.element, left->llvm.element);
            break;
        }
        case ADD: case SUB: case MUL: case DIV:
        {
            LLVMValueRef leftRef = left->name ? LLVMBuildLoad2(builder, int32Type, left->llvm.element, left->name) : left->llvm.element;
            LLVMValueRef rightRef = right->name ? LLVMBuildLoad2(builder, int32Type, right->llvm.element, right->name) : right->llvm.element;
            LLVMValueRef ret;
            Type op = curr->type;
            switch(curr->type) 
            {
                case ADD: ret = LLVMBuildAdd(builder, leftRef, rightRef, to_string(op)); break;
                case SUB: ret = LLVMBuildSub(builder, leftRef, rightRef, to_string(op)); break;
                case MUL: ret = LLVMBuildMul(builder, leftRef, rightRef, to_string(op)); break;
                case DIV: ret = LLVMBuildSDiv(builder, leftRef, rightRef, to_string(op)); break;
                default: todo(1, "handle this")
            }
            curr->llvm.element = ret;
            break;
        }
        case FCALL:
        {
            curr->llvm.element = LLVMBuildCall2(builder, 
                curr->Fcall.ptr->llvm.funcType, 
                curr->Fcall.ptr->llvm.element, 
                NULL, 0, "");
            break;
        }
        case FDEC:
        {
            // debug("FDEC: ", curr->name);
            curr->llvm.funcType = LLVMFunctionType(int32Type, NULL, 0, 0);
            curr->llvm.element = LLVMAddFunction(mod, curr->name, curr->llvm.funcType);
            LLVMBasicBlockRef funcEntry = LLVMAppendBasicBlock(curr->llvm.element, "entry");
            LLVMPositionBuilderAtEnd(builder, funcEntry);

            if(strcmp(curr->name, "main") == 0) LLVMPositionBuilderAtEnd(builder, funcEntry);
            break;
        }
        case RETURN:
        {
            LLVMBuildRet(builder, left->llvm.element);
            break;
        }
        case END_BLOC:
        {
            break;
        }
        default: todo(1, "handle this case (%s)\n", to_string(curr->type)); break;
        }
    }
    char *outfile = calloc(strlen(name) + 2, 1);
    strcpy(outfile, name);
    strcpy(outfile + strlen(outfile) - 1, "ll");

    LLVMPrintModuleToFile(mod, outfile, NULL);
    // Cleanup
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    free(outfile);
}