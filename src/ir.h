#pragma once
// INCLUDES
#include "./lib/header.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>

// STRUCTS
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/DebugInfo.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm/Config/llvm-config.h>

#if LLVM_VERSION_MAJOR >= 13
#    undef LLVM_C_STRICT_PROTOTYPES_BEGIN
#    undef LLVM_C_STRICT_PROTOTYPES_END
#    define LLVM_C_STRICT_PROTOTYPES_BEGIN
#    define LLVM_C_STRICT_PROTOTYPES_END
#    include <llvm-c/Transforms/PassBuilder.h>
typedef LLVMErrorRef ErrorRef;
typedef LLVMPassBuilderOptionsRef PassBuilderOptions;
#else
#    include <llvm-c/Transforms/PassManagerBuilder.h>
#endif

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Bloc;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTargetRef Target;
typedef LLVMTargetMachineRef TargetMachine;
typedef LLVMCodeGenOptLevel CodeGenOptLevel;
typedef LLVMRelocMode RelocMode;
typedef LLVMCodeModel CodeModel;
typedef LLVMTypeKind TypeKind;
typedef LLVMAttributeRef AttributeRef;
typedef LLVMMetadataRef MetadataRef;

typedef enum Type Type;
enum Type {
    // clang-format off
    NONE, ERR, SCOPE, URA,
    
    // ID become VAR, after getting declared
    ID, VAR, 

    DECLARE,
    INIT_VAR,
    LOAD,
    ARG,

    VAL_CREATE,

    TEMPLATE,
    TYPES, TEMPLATE_TYPE, 
    ANY,
    VOID, BOOL, I8, I32, CHARS,
    REF,
    ZERO,

    STRUCT, DOT, ATTR,

    //OWN,
    DREF,

    LPARENT, RPARENT, DOTS, COMA,
    LBRACK, RBRACK,
    ARRAY, ARRAY_LIT, ACCESS,

    ASSIGN,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    ADD, SUB, MUL, DIV, MOD,

    GT, LT, GE, LE, EQ, NQ,

    AND, OR, NOT,

    PROTO, 
    FN_DEC, ARGS, VARIADIC, FN_CALL, RETURN,
	FN_ENTRY,
    SIZEOF, IS_NULLABLE,
    AS,
    IF, ELIF, ELSE,
    WHILE, BRK, CNT,

    END,
    // clang-format on
};

typedef struct uraIR uraIR;
struct uraIR {
    char *name;

    Type type;
    TypeRef rtype;
    Value elem;

    struct {
        bool variadic;
        TypeRef ret_type;
        expand(uraIR *, params);
    } fn;
    struct {
        long value;
    } num;
    struct {
        Bloc cond;
        Bloc then;
        Bloc end;
    } bloc;

    union {
        // LOAD:  left is what to load
        uraIR *left;
        // ENTRY: ptr is FN_DEC
        uraIR *ptr;
    };
    union {
        uraIR *right;
    };
};

struct {
    struct {
        TypeRef vd;
        TypeRef i1;
        TypeRef i8;
        TypeRef i16;
        TypeRef i32;
        TypeRef i64;
    } types;

    Context context;
    Module module;
    Builder builder;

    char *dir;
    char *ll_path;
    char *exe_path;

    char *error_msg;

    expand(uraIR *, irs);
} uir;

#include "proto.h"

void set_exec_path(char *path) {
    uir.dir = dirname2(path);
}

void uir_start(char *name) {
    uir.context = LLVMContextCreate();
    uir.module = LLVMModuleCreateWithNameInContext(name, uir.context);
    uir.builder = LLVMCreateBuilderInContext(uir.context);

    uir.types.vd = LLVMVoidTypeInContext(uir.context);
    uir.types.i1 = LLVMInt1TypeInContext(uir.context);
    uir.types.i8 = LLVMInt8TypeInContext(uir.context);
    uir.types.i16 = LLVMInt16TypeInContext(uir.context);
    uir.types.i32 = LLVMInt32TypeInContext(uir.context);
    uir.types.i64 = LLVMInt64TypeInContext(uir.context);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    // LLVMInitializeNativeAsmParser(); // TODO: to be checked

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(uir.module, triple);
    Target target;
    if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
        CodeGenOptLevel level = LLVMCodeGenLevelDefault;
        RelocMode reloc = LLVMRelocDefault;
        CodeModel model = LLVMCodeModelDefault;
        TargetMachine machine;
        machine = LLVMCreateTargetMachine(target, triple, "", "", level, reloc, model);
        TargetData layout = LLVMCreateTargetDataLayout(machine);
        LLVMSetModuleDataLayout(uir.module, layout);
        LLVMDisposeTargetData(layout);
        LLVMDisposeTargetMachine(machine);
    }
    LLVMDisposeMessage(triple);

    // TODO: add asan stuff here
    // TODO: add flags stuff (Passes)
}

void uir_end() {
    // PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
    // if (uir.flags) {
    //     Error err = LLVMRunPasses(uir.module, uir.flags, NULL, opts);
    //     if (err) {
    //         char *msg = LLVMGetErrorMessage(err);
    //         CHECK(1, "optimizer error: %s", msg);
    //         LLVMDisposeErrorMessage(msg);
    //     }
    // }
    // if (uir.debug_builder) {
    //     LLVMDIBuilderFinalize(uir.debug_builder);
    //     LLVMDisposeDIBuilder(uir.debug_builder);
    //     uir.debug_builder = NULL;
    // }
    if (LLVMVerifyModule(uir.module, LLVMReturnStatusAction, &uir.error_msg)) {
        // clang-format off
        fprintf(stderr, "%s: %s\n", 
			RED("VerifyModule error"), uir.error_msg);
        exit(1);
        // clang-format on
    }
    // LLVMVerifyModule always allocate for char* error
    LLVMDisposeMessage(uir.error_msg);


    // LLVMDisposePassBuilderOptions(opts);
    if (uir.dir == NULL)
        uir.dir = getcwd2();
    if (uir.ll_path == NULL) {
        uir.ll_path = format("%s/build/out.ll", uir.dir);
        mkdir2(dirname2(uir.ll_path));
    }
    if (uir.exe_path == NULL) {
        uir.exe_path = format("%s/exe.out", uir.dir);
    }
    if (LLVMPrintModuleToFile(uir.module, uir.ll_path, &uir.error_msg)) {
        // clang-format off
        fprintf(stderr, "%s: %s (%s)\n", 
			RED("PrintModuleToFile error"), uir.error_msg, uir.ll_path);
        exit(1);
        // clang-format on
    }
    // LLVMVerifyModule always allocate for char* error
}

void uir_exit() {
    if (uir.error_msg != NULL) {
        LLVMDisposeMessage(uir.error_msg);
    }
    arena_free();
}

void llvm_fn_dec(uraIR *ir) {
    assert(ir->fn.ret_type);
    assert(ir->name);

    expand(TypeRef, types);
    zero_init(types);

    for (int i = 0; i < ir->fn.params_count; i++) {
        uraIR *param = ir->fn.params[i];
        // param->elem = LLVMGetParam(ir->elem, i);
        append(types, param->rtype);
    }

    ir->rtype = LLVMFunctionType(ir->fn.ret_type, types, types_count, ir->fn.variadic);
    ir->elem = LLVMAddFunction(uir.module, ir->name, ir->rtype);
    // printf("declare %s, has %zu params\n", ir->name, ir->fn.params_count);
    for (int i = 0; i < ir->fn.params_count; i++) {
        uraIR *param = ir->fn.params[i];
        param->elem = LLVMGetParam(ir->elem, i);
        // append(types, param->rtype);
    }
}

void llvm_fn_entry(uraIR *ir) {
    Value elem = ir->elem;
    Bloc bloc = LLVMAppendBasicBlockInContext(uir.context, elem, "entry");
    LLVMPositionBuilderAtEnd(uir.builder, bloc);
}

bool is_bloc_terminated() {
    Bloc bloc = LLVMGetInsertBlock(uir.builder);
    return bloc != NULL && LLVMGetBasicBlockTerminator(bloc) != NULL;
}

void llvm_return(uraIR *ir) {
    if (is_bloc_terminated())
        return;
    if (ir->elem == NULL) {
        LLVMBuildRetVoid(uir.builder);
        return;
    }
    LLVMBuildRet(uir.builder, ir->elem);
}

uraIR *llvm_val_create(uraIR *ir) {
    assert(ir->rtype);
    long value = ir->num.value;

    if (ir->rtype == uir.types.i32)
        ir->elem = LLVMConstInt(ir->rtype, value, 0);
    else
        fprintf(stderr, "handle this case\n");
    return ir;
}

void llvm_declare(uraIR *ir) {
    assert(ir->rtype);
    assert(ir->name);

    Bloc bloc = LLVMGetInsertBlock(uir.builder);
    Value basic = LLVMGetBasicBlockParent(bloc);
    Bloc entry = LLVMGetEntryBasicBlock(basic);

    Value inst = LLVMGetFirstInstruction(entry);
    while (inst && LLVMIsAAllocaInst(inst))
        inst = LLVMGetNextInstruction(inst);

    if (inst)
        LLVMPositionBuilderBefore(uir.builder, inst);
    else
        LLVMPositionBuilderAtEnd(uir.builder, entry);

    ir->elem = LLVMBuildAlloca(uir.builder, ir->rtype, ir->name);
    LLVMPositionBuilderAtEnd(uir.builder, bloc);
}

void llvm_zero_init(uraIR *ir) {
    TypeRef rtype = ir->rtype;
    Value elem = ir->elem;
    LLVMBuildStore(uir.builder, LLVMConstNull(rtype), elem);
}

void llvm_assign(uraIR *left, uraIR *right) {
    LLVMBuildStore(uir.builder, right->elem, left->elem);
}

uraIR *new_ir(Type type) {
    assert(type != 0);
    uraIR *new = arena_alloc(1, sizeof(uraIR));
    new->type = type;
    return new;
}

uraIR *ir_fn(char *name, TypeRef ret_type) {
    uraIR *new = new_ir(FN_DEC);
    new->name = name;
    new->fn.ret_type = ret_type;

    append(uir.irs, new);
    return new;
}

uraIR *ir_entry() {
    uraIR *new = new_ir(FN_ENTRY);

    append(uir.irs, new);
    return new;
}

uraIR *ir_add_param(uraIR *fn, char *name, TypeRef rtype) {
    assert(fn != NULL);
    uraIR *param = ir_variable(rtype, format("arg.%s", name));

    append(fn->fn.params, param);
    return param;
}

uraIR *ir_num(TypeRef rtype, long value) {
    uraIR *new = new_ir(VAL_CREATE);
    new->rtype = rtype;
    new->num.value = value;

    append(uir.irs, new);
    return new;
}

// uraIR *ir_dec_variable() {
//     uraIR *new = new_ir(DECLARE);

//     append(uir.irs, new);
//     return new;
// }

uraIR *ir_variable(TypeRef rtype, char *name) {
    uraIR *new = arena_alloc(1, sizeof(uraIR));
    new->rtype = rtype;
    new->name = name;
    new->type = DECLARE;

    append(uir.irs, new);
    return new;
}

uraIR *ir_return() {
    uraIR *new = new_ir(RETURN);
    append(uir.irs, new);
    return new;
}

uraIR *ir_load() {
    uraIR *new = new_ir(LOAD);
    // new->load.ptr = ir;
    append(uir.irs, new);
    return new;
}

void llvm_load(uraIR *ir) {
    char *name = ir->name;

    // TypeRef rtype = ir->load.ptr->rtype;
    // Value elem = ir->load.ptr->elem;
    // ir->elem = LLVMBuildLoad2(uir.builder, rtype, elem, name);
}
/*
TODO:
+ add assertion everywhere (replace it later on with clear error)
*/

void llvm_compile() {
    for (size_t i = 0; i < uir.irs_count; i++) {
        uraIR *curr = uir.irs[i];
        print("%s\n", to_string(curr->type));
        // switch (curr->type) {
        // case FN_DEC: {
        //     llvm_fn_dec(curr);
        //     break;
        // }
        // case FN_ENTRY: {
        //     uraIR *fn = uir.irs[i - 1];
        //     llvm_fn_entry(fn);
        //     break;
        // }
        // case VAL_CREATE: {
        //     llvm_val_create(curr);
        //     break;
        // }
        // case RETURN: {
        //     uraIR *r = uir.irs[i - 1];
        //     llvm_return(r);
        //     break;
        // }
        // case DECLARE: {
        //     uraIR *var = uir.irs[i];
        //     llvm_declare(var);
        //     break;
        // }
        // default: {
        //     eprint("handle this case %s\n", to_string(curr->type));
        //     exit(1);
        //     break;
        // }
        // }
    }
}

// UTILS
const char *to_string(Type type) {
    char *types[END + 1] = {
        // clang-format off
        [ERR] = "ERR",
        [ID] = "ID",
        [SCOPE] = "SCOPE", [URA] = "URA",

        [VAR] = "VAR", [DECLARE] = "DECLARE", [INIT_VAR] = "INIT_VAR",
        [LOAD] = "LOAD", [ARG] = "ARG",

        [VAL_CREATE] = "VAL_CREATE",
        [TEMPLATE] = "TEMPLATE",
        [TEMPLATE_TYPE] = "TEMPLATE_TYPE",
        [TYPES] = "TYPES",

        [ANY] = "ANY",
        [VOID] = "VOID", [BOOL] = "BOOL", [I8] = "I8", [I32] = "I32",
        [CHARS] = "CHARS",
        [REF] = "REF",
        [ZERO] = "ZERO",

        [STRUCT] = "STRUCT", [DOT] = "DOT", [ATTR] = "ATTR",

        // [OWN] = "OWN",
        [DREF] = "DREF",

        [LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
        [LBRACK] = "LBRACK", [RBRACK] = "RBRACK", 
        [DOTS] = "DOTS", [COMA] = "COMA",
        
        [ARRAY] = "ARRAY", [ARRAY_LIT] = "ARRAY_LIT",
        [ACCESS] = "ACCESS",
        
        [ASSIGN] = "ASSIGN",
        [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
        [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
        [MOD_ASSIGN] = "MOD_ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE",
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [AND] = "AND", [OR] = "OR", [NOT] = "NOT",

        [PROTO] = "PROTO", [FN_ENTRY] = "FN_ENTRY",
        [FN_DEC] = "FN_DEC", [ARGS] = "ARGS", [VARIADIC] = "VARIADIC",
        [FN_CALL] = "FN_CALL", [RETURN] = "RETURN",
        [SIZEOF] = "SIZEOF", [IS_NULLABLE] = "IS_NULLABLE",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
        [WHILE] = "WHILE", [BRK] = "BRK", [CNT] = "CNT",
        [AS] = "AS",
        [END] = "END",
        // clang-format on
    };
    if (types[type] == NULL)
        return "UNKNOWN";
    return types[type];
}
