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
        CodeGenOptLevel level = CodeGenLevelDefault;
        RelocMode reloc = RelocDefault;
        CodeModel model = CodeModelDefault;
        TargetMachine machine;
        machine = LLVMCreateTargetMachine(target, triple, "", "", level, reloc, model);
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
    // assert(type);
    // assert(type->token);
    if (type->token->type == CHARS)
        return LLVMPointerType(get_llvm_type(I8), 0);
    if (type->token->type == REF)
        return LLVMPointerType(get_data_type(type->left), 0);
    if (type->token->type == STRUCT_DEC)
        return type->token->llvm.type;
    return get_llvm_type(type->token->type);
}

// node is VAR_DEC: its left is the VAR, the VAR's left is the type
Value create_variable(Node *node) {
    Token *var = node->left->token;
    Node *type = node->left->left;
    return LLVMBuildAlloca(ura.builder, get_data_type(type), var->name);
}

Value create_value(Node *node) {
    Token *token = node->token;
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

// node is VAR_LOAD: its left is the VAR that own the storage
Value create_load(Node *node) {
    Value ptr = address_of(node);
    TypeRef type = LLVMGetElementType(LLVMTypeOf(ptr));
    Token *token = node->token;
    char *name = token->asm_name ? token->asm_name : token->name;
    return LLVMBuildLoad2(ura.builder, type, ptr, name ? name : "");
}

void create_struct(Node *node) {
    Token *token = node->token;
    token->llvm.type = LLVMStructCreateNamed(ura.context, token->name);
    assert(token->llvm.type != NULL);
    // TODO: protect when struct has no attributes
    TypeRef *attrs = ura_alloc(node->children_count, sizeof(TypeRef));
    size_t count = 0;
    for (size_t i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (!includes(child->token->type, VAR_DEC, 0))
            continue;
        attrs[count++] = get_data_type(child->left->left);
    }
    LLVMStructSetBody(token->llvm.type, attrs, count, 0);
}


// TODO: add a flag to define if it's float or unsigned or something
Value create_bin_op(Node *node) {
    // TODO: handle unsigned types
    // clang-format off
    LLVMOpcode opcodes[END + 1] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub, [MUL] = LLVMMul, [DIV] = LLVMSDiv,
        [MOD] = LLVMSRem, [AND] = LLVMAnd, [OR] = LLVMOr,
    };
    LLVMIntPredicate predicates[END + 1] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
        [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
    };
    // clang-format on

    Type type = node->token->type;
    const char *name = to_string(type);
    Value left = node->left->token->llvm.elem;
    Value right = node->right->token->llvm.elem;
    if (opcodes[type])
        return LLVMBuildBinOp(ura.builder, opcodes[type], left, right, name);
    if (predicates[type])
        return LLVMBuildICmp(ura.builder, predicates[type], left, right, name);
    eprint("unknown operation %t\n", type);
    exit(1);
}


Value address_of(Node *node) {
    switch (node->token->type) { // clang-format off
    case VAR:      return node->token->llvm.elem;
    case VAR_LOAD: return node->left->token->llvm.elem;
    case DOT: {
        Value ptr = address_of(node->left); // address of struct, not its value
        TypeRef type = LLVMGetElementType(LLVMTypeOf(ptr));
        Token *attr = node->right->token;
        unsigned index = attr->i32.value;
        char *name = node->token->asm_name;
        return LLVMBuildStructGEP2(ura.builder, type, ptr, index, name);
    }
    case DREF: {
        code_gen(node->left); // TODO: to be checked, I don't like it here
        return node->left->token->llvm.elem;
    }
    default: {
        eprint("can't assign to %t\n", node->token->type);
        exit(1);
        break;
    }
    } // clang-format on
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
    token->llvm.type = LLVMFunctionType(ret, args, args_count, token->is_variadic);
    char *name = token->asm_name ? token->asm_name : token->name;
    token->llvm.elem = LLVMAddFunction(ura.module, name, token->llvm.type);
}

void create_entry(Node *node) {
    Value fn = node->token->llvm.elem;
    Bloc bloc = LLVMAppendBasicBlockInContext(ura.context, fn, "entry");
    LLVMPositionBuilderAtEnd(ura.builder, bloc);
}

// fn is FN_DEC, param is VAR_DEC: store the incoming argument into its slot
Value create_param(Node *fn, Node *param_node, size_t pos) {
    assert(fn->token->llvm.elem);
    assert(param_node->left->token->llvm.elem);

    Value param = LLVMGetParam(fn->token->llvm.elem, pos);
    Value value = param_node->left->token->llvm.elem;

    return LLVMBuildStore(ura.builder, param, value);
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
    TypeRef type = fdec->llvm.type;
    Value fn = fdec->llvm.elem;
    char *name = node->right->right ? node->token->name : "";
    Value res = LLVMBuildCall2(ura.builder, type, fn, args, args_count, name);
    return res;
}

Value create_return(Node *node) {
    if (!node)
        return LLVMBuildRetVoid(ura.builder);
    return LLVMBuildRet(ura.builder, node->token->llvm.elem);
}

void create_default_return(Node *node) {
    if (is_bloc_terminated())
        return;
    if (!node->right) {
        LLVMBuildRetVoid(ura.builder);
        return;
    }
    LLVMBuildRet(ura.builder, create_value(node->right));
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