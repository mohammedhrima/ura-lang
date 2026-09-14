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
        eprint("handle this case %t\n", type);
        break;
    }
    return NULL;
};

void create_function(Node *node) {
    Token *token = node->token;
    // set return type
    TypeRef ret = get_llvm_type(token->ret_type);
    // set params signature
    // TODO: add them
    TypeRef *args = NULL;
    size_t args_count = node->left->children_count;
    if (args_count) {
        args = ura_alloc(node->left->children_count, sizeof(TypeRef));
        for (size_t i = 0; i < args_count; i++) {
            Token *child = node->left->children[i]->left->token;
            args[i] = get_llvm_type(child->type);
        }
    }
    // TODO: set args count, set if function is variadic or not
    token->llvm.func_type = LLVMFunctionType(ret, args, args_count, false);
    token->llvm.elem = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
    free(args);
}

void create_entry(Token *token) {
    Bloc bloc = LLVMAppendBasicBlockInContext(ura.context, token->llvm.elem, "entry");
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

Value create_return(Token *token) {
    return LLVMBuildRet(ura.builder, token->llvm.elem);
}

Value create_function_call(Node *node) {
    Token *fdec = node->right->token;
    Value *args = NULL;
    size_t args_count = node->left->children_count;
    if (args_count) {
        args = ura_alloc(args_count, sizeof(Value));
        for (size_t i = 0; i < args_count; i++) {
            args[i] = node->left->children[i]->token->llvm.elem;
        }
    }
    Value res = LLVMBuildCall2(ura.builder, fdec->llvm.func_type, fdec->llvm.elem, args, args_count,
                               node->token->name);
    free(args);
    return res;
}

Value create_value(Token *token) {
    switch (token->type) {
    case I32:
        return LLVMConstInt(get_llvm_type(token->type), token->i32.value, 0);
    default:
        eprint("handle this case %t\n", token->type);
        break;
    }
    return NULL;
}

Value create_variable(Node *node) {
    switch (node->token->type) {
    case I32:
        return LLVMBuildAlloca(ura.builder, get_llvm_type(node->token->type), node->token->name);
    default:
        eprint("handle this case %t\n", node->token->type);
        break;
    }
    return NULL;
}

Value create_load(Token *token) {
    return LLVMBuildLoad2(ura.builder, get_llvm_type(token->type), token->llvm.elem, token->name);
}

Value create_assign(Token *left, Token *right) {
    return LLVMBuildStore(ura.builder, right->llvm.elem, left->llvm.elem);
}

Value create_param(Token *fn, Token *param, size_t pos) {
    return LLVMBuildStore(ura.builder, LLVMGetParam(fn->llvm.elem, pos), param->llvm.elem);
}

Value create_math_op(Token *left, Token *op_token, Token *right) {
    LLVMOpcode ops[] = {
        [ADD] = LLVMAdd,
        [SUB] = LLVMSub,
        [MUL] = LLVMMul,
        [DIV] = LLVMSDiv,
        // [DIV] = LLVMUDiv, unsigned div
        [MOD] = LLVMSRem,
        // [MOD] = LLVMSRem, unsigned Mod
        [END] = 0,
    };

    LLVMOpcode op = ops[op_token->type];
    if (op == 0) {
        eprint("unknown operation\n");
        exit(1);
    }

    switch (left->type) {
    case I32:
        return LLVMBuildBinOp(ura.builder, op, left->llvm.elem, right->llvm.elem,
                              to_string(op_token->type));
    default:
        eprint("handle this case %t", left->type);
        exit(1);
        break;
    }
    return NULL;
}