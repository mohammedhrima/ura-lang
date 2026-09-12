#include "header.h"

TypeRef get_llvm_type(Type type) {
    // ura.vd = LLVMVoidTypeInContext(ura.context);
    // ura.i1 = LLVMInt1TypeInContext(ura.context);
    // ura.i8 = LLVMInt8TypeInContext(ura.context);
    // ura.i16 = LLVMInt16TypeInContext(ura.context);
    // ura.i32 = LLVMInt32TypeInContext(ura.context);
    // ura.i64 = LLVMInt64TypeInContext(ura.context);
    // ura.f32 = LLVMFloatTypeInContext(ura.context);
    // ura.f64 = LLVMDoubleTypeInContext(ura.context);
    switch (type) {
    case I32:
        return LLVMInt32TypeInContext(ura.context);
    default:
        eprint("%s:%d handle this case %t\n", FILE, LINE, type);
        break;
    }
    return NULL;
};

void create_function(Token *token) {
    // set return type
    TypeRef ret = get_llvm_type(token->ret_type);
    // set params signature
    // TODO: add them
    TypeRef *params = NULL;
    // TODO: set params count, set if function is variadic or not
    token->llvm.func_type = LLVMFunctionType(ret, params, 0, false);
    token->llvm.elem = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
}

void create_entry(Token *token) {
    Bloc bloc = LLVMAppendBasicBlockInContext(ura.context, token->llvm.elem, "entry");
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

Value create_return(Token *token) {
    return LLVMBuildRet(ura.builder, token->llvm.elem);
}

Value create_value(Token *token) {
    switch (token->type) {
    case I32:
        return LLVMConstInt(get_llvm_type(token->type), token->i32.value, 0);
    default:
        eprint("%s:%d handle this case %t", FILE, LINE, token->type);
        break;
    }
    return NULL;
}

Value create_variable(Token *token) {
    switch (token->type) {
    case I32:
        return LLVMBuildAlloca(ura.builder, get_llvm_type(token->type), token->name);
    default:
        eprint("%s:%d handle this case %t", FILE, LINE, token->type);
        break;
    }
    return NULL;
}

Value create_assign(Token *left, Token *right) {
    return LLVMBuildStore(ura.builder, right->llvm.elem, left->llvm.elem);
}


Value create_math_op(Token *left, Token *op_token, Token *right) {
    LLVMOpcode op = NULL;

    switch (left->type) {
    case I32:
        return LLVMBuildBinOp(ura.builder, LLVMAdd, left->llvm.elem, right->llvm.elem, "add");
    default:
        eprint("%s:%d handle this case %t", FILE, LINE, left->type);
        break;
    }
}