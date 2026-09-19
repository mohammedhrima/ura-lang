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
    // clang-format off
    switch (type) {
    case VOID:  return LLVMVoidTypeInContext(ura.context);
    case BOOL:  return LLVMInt1TypeInContext(ura.context);
    case I8:    return LLVMInt8TypeInContext(ura.context);
    case I32:   return LLVMInt32TypeInContext(ura.context);
    default:
        eprint("handle this case %t\n", type);
        exit(1);
        break;
    }
    // clang-format on
    return NULL;
};

TypeRef get_data_type(Node *type) {
    if (type->token->type == CHARS)
        return LLVMPointerType(get_llvm_type(I8), 0);
    if (type->token->type == REF)
        return LLVMPointerType(get_data_type(type->left), 0);
    return get_llvm_type(type->token->type);
}

Value create_variable(Token *var, Node *type) {
    return LLVMBuildAlloca(ura.builder, get_data_type(type), var->name);
}

Value create_value(Token *token) {
    if (token->type == CHARS)
        return LLVMBuildGlobalStringPtr(ura.builder, token->chars.value, "str");
    TypeRef llvm_type = get_llvm_type(token->type);
    // if (token->is_ref)
    //     llvm_type = LLVMPointerType(llvm_type, 0);
    // clang-format off
    switch (token->type) { 
    case BOOL:  return LLVMConstInt(llvm_type, token->b1.value, 0);
    case I8:    return LLVMConstInt(llvm_type, token->i8.value, 0);
    case I32:   return LLVMConstInt(llvm_type, token->i32.value, 0);
    default:
        eprint("handle this case %t\n", token->type);
        exit(1);
        break;
    }
    // clang-format on
    return NULL;
}

Value create_load(Token *var, Node *type) {
    return LLVMBuildLoad2(ura.builder, get_data_type(type), var->llvm.elem, var->name);
}

Value create_dref(Value ptr, Node *type) {
    return LLVMBuildLoad2(ura.builder, get_data_type(type), ptr, "dref");
}

// TODO: add a flag to define if it's float or unsigned or something
Value create_math_op(Token *left, Token *op_token, Token *right) {
    // TODO: handle unsigned types
    LLVMOpcode ops[] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub,  [MUL] = LLVMMul,
        [DIV] = LLVMSDiv, [MOD] = LLVMSRem, [END] = 0,
        // [DIV] = LLVMUDiv, unsigned div
        // [MOD] = LLVMSRem, unsigned Mod
    };

    LLVMOpcode op = ops[op_token->type];
    if (op == 0) {
        eprint("unknown operation\n");
        exit(1);
    }

    const char *name = to_string(op_token->type);
    return LLVMBuildBinOp(ura.builder, op, left->llvm.elem, right->llvm.elem, name);
}

Value create_logic_op(Token *left, Token *op_token, Token *right) {
    LLVMOpcode ops[] = { [AND] = LLVMAnd, [OR] = LLVMOr };

    LLVMOpcode op = ops[op_token->type];
    if (op == 0) {
        eprint("unknown operation\n");
        exit(1);
    }

    const char *name = to_string(op_token->type);
    return LLVMBuildBinOp(ura.builder, op, left->llvm.elem, right->llvm.elem, name);
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

    const char *name = to_string(op_token->type);
    return LLVMBuildICmp(ura.builder, op, left->llvm.elem, right->llvm.elem, name);
}

Value address_of(Node *node) {
    switch (node->token->type) {
    case VAR:
        return node->token->llvm.elem;
    case LOAD_VAR:
        return node->left->token->llvm.elem;
    case DREF:
        code_gen(node->left); // TODO: to be checked, I don't like it here
        return node->left->token->llvm.elem;
    default: {
        eprint("can't assign to %t\n", node->token->type);
        exit(1);
        break;
    }
    }
    return NULL;
}

Value create_assign(Node *left, Node *right) {
    Value left_elem = address_of(left);
    Value right_elem = right->token->llvm.elem;
    return LLVMBuildStore(ura.builder, right_elem, left_elem);
}

void create_function(Node *node) {
    Token *token = node->token;
    // set return type
    TypeRef ret = node->right ? get_data_type(node->right) : get_llvm_type(VOID);
    // set params signature
    // TODO: add them
    TypeRef *args = NULL;
    size_t args_count = node->left->children_count;
    if (args_count) {
        args = ura_alloc(node->left->children_count, sizeof(TypeRef));
        for (size_t i = 0; i < args_count; i++) {
            Node *type = node->left->children[i]->left->left;
            args[i] = get_data_type(type);
        }
    }
    // TODO: set args count, set if function is variadic or not
    token->llvm.func_type = LLVMFunctionType(ret, args, args_count, token->is_variadic);
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
                               node->right->right ? node->token->name : "");
    free(args);
    return res;
}

Value create_return(Token *token) {
    return LLVMBuildRet(ura.builder, token->llvm.elem);
}

void create_default_return(Node *node) {
    if (is_bloc_terminated())
        return;
    if (!node->right) {
        LLVMBuildRetVoid(ura.builder);
        return;
    }
    LLVMBuildRet(ura.builder, create_value(node->right->token));
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