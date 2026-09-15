#include "header.h"

void asm_init(char *name) {
    ura.context = LLVMContextCreate();
    ura.module = LLVMModuleCreateWithNameInContext(name, ura.context);
    ura.builder = LLVMCreateBuilderInContext(ura.context);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    // LLVMInitializeNativeAsmParser(); // TODO: to be checked

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(ura.module, triple);
    Target target;
    if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
        TargetMachine machine =
            LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault,
                                    LLVMRelocDefault, LLVMCodeModelDefault);
        TargetData layout = LLVMCreateTargetDataLayout(machine);
        LLVMSetModuleDataLayout(ura.module, layout);
        LLVMDisposeTargetData(layout);
        LLVMDisposeTargetMachine(machine);
    }
    LLVMDisposeMessage(triple);

    // TODO: add asan stuff here
    // TODO: add flags stuff (Passes)
}

void asm_finalize(char *ll_path) {
    char *error = NULL;
    // PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
    // if (ura.flags) {
    //     Error err = LLVMRunPasses(ura.module, ura.flags, NULL, opts);
    //     if (err) {
    //         char *msg = LLVMGetErrorMessage(err);
    //         CHECK(1, "optimizer error: %s", msg);
    //         LLVMDisposeErrorMessage(msg);
    //     }
    // }
    // if (ura.debug_builder) {
    //     LLVMDIBuilderFinalize(ura.debug_builder);
    //     LLVMDisposeDIBuilder(ura.debug_builder);
    //     ura.debug_builder = NULL;
    // }
    if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
        eprint("module verification failed:\n%s\n", error);
    LLVMDisposeMessage(error);
    // LLVMDisposePassBuilderOptions(opts);
    LLVMPrintModuleToFile(ura.module, ll_path, NULL);
}

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
    case BOOL:
        return LLVMInt1TypeInContext(ura.context);
    default:
        eprint("handle this case %t\n", type);
        break;
    }
    return NULL;
};

Value create_variable(Node *node) {
    switch (node->token->type) {
        // clang-format off
    case I32: case BOOL:
        // clang-format on
        return LLVMBuildAlloca(ura.builder, get_llvm_type(node->token->type), node->token->name);
    default:
        eprint("handle this case %t\n", node->token->type);
        break;
    }
    return NULL;
}

Value create_value(Token *token) {
    switch (token->type) {
    case I32:
        return LLVMConstInt(get_llvm_type(token->type), token->i32.value, 0);
    case BOOL:
        return LLVMConstInt(get_llvm_type(token->type), token->b1.value, 0);
    default:
        eprint("handle this case %t\n", token->type);
        break;
    }
    return NULL;
}

Value create_load(Token *token) {
    return LLVMBuildLoad2(ura.builder, get_llvm_type(token->type), token->llvm.elem, token->name);
}

Value create_math_op(Token *left, Token *op_token, Token *right) {
    // TODO: handle unsigned types
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

Value create_comparision_op(Token *left, Token *op_token, Token *right) {
    // TODO: handle unsigned types
    LLVMIntPredicate ops[] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE, [LE] = LLVMIntSLE,
        [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,  [END] = 0,
    };

    LLVMIntPredicate op = ops[op_token->type];
    if (op == 0) {
        eprint("unknown operation\n");
        exit(1);
    }

    switch (left->type) {
    case I32:
        return LLVMBuildICmp(ura.builder, op, left->llvm.elem, right->llvm.elem,
                             to_string(op_token->type));
    default:
        eprint("handle this case %t", left->type);
        exit(1);
        break;
    }
    return NULL;
}

Value create_assign(Token *left, Token *right) {
    return LLVMBuildStore(ura.builder, right->llvm.elem, left->llvm.elem);
}

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

Value create_param(Token *fn, Token *param, size_t pos) {
    return LLVMBuildStore(ura.builder, LLVMGetParam(fn->llvm.elem, pos), param->llvm.elem);
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

Value create_return(Token *token) {
    return LLVMBuildRet(ura.builder, token->llvm.elem);
}


Value get_parent_bloc() {
    return LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
}

Bloc create_label(char *name) {
    Value parent = get_parent_bloc();
    return LLVMAppendBasicBlockInContext(ura.context, parent, name);
}

void create_jmp_condition(Value cond, Bloc then, Bloc next) {
    LLVMBuildCondBr(ura.builder, cond, then, next);
    LLVMPositionBuilderAtEnd(ura.builder, then);
}

bool is_bloc_terminated() {
    return LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)) != NULL;
}

void create_jmp_out(Bloc bloc) {
    LLVMBuildBr(ura.builder, bloc);
}

void create_jmp(Bloc bloc) {
    if (!is_bloc_terminated())
        LLVMBuildBr(ura.builder, bloc);
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

void create_at(Bloc bloc) {
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

void create_last_label(Bloc bloc) {
    LLVMMoveBasicBlockAfter(bloc, LLVMGetLastBasicBlock(get_parent_bloc()));
}