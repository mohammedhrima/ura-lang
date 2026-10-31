// INCLUDES
#include "./lib/header.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

// LLVM STUFF
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
typedef struct Ucg Ucg;

#include "proto.h"


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
    VOID, 
    
    // BOOL, I8, I32, 
    INT,
    
    CHARS,
    
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

#define Struct(T)     \
    struct {          \
        T *arr;       \
        size_t count; \
        size_t size;  \
    }

struct Ucg {
    char *name;

    Type type;
    int shift;

    TypeRef rtype;
    Value elem;

    struct {
        bool varg;
        Ucg *ret;
        // Struct(Ucg *, params);
        struct {
            Ucg **arr;
            size_t count;
            size_t size;
        } params;
    } fn;

    struct {
        long long value;
    } num;

    struct {
        Bloc cond;
        Bloc then;
        Bloc end;
    } bloc;

    union {
        // LOAD:  left is what to load
        Ucg *left;
        // ENTRY: ptr is FN_DEC
        Ucg *ptr;

        // Ucg *fn_ptr;

        Ucg *data_type;
        Ucg *to_ret;
        Ucg *to_create;
        Ucg *to_load;
    };
    union {
        Ucg *right;
    };
};

struct {
    struct {
        TypeRef vd;
        // TypeRef i1;
        // TypeRef i8;
        // TypeRef i16;
        // TypeRef i32;
        // TypeRef i64;
        TypeRef num[255];
    } types;

    Context context;
    Module module;
    Builder builder;

    char *dir;
    char *ll_path;
    char *exe_path;

    char *error_msg;

    expand(Ucg *, insts);
} gb_ucg;


void set_exec_path(char *path) {
    gb_ucg.dir = dirname2(path);
}

void ucg_start(char *name) {
    gb_ucg.context = LLVMContextCreate();
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.context);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.context);

    gb_ucg.types.vd = LLVMVoidTypeInContext(gb_ucg.context);

    for (int i = 1; i < 255; i <<= 1)
        gb_ucg.types.num[i] = LLVMIntTypeInContext(gb_ucg.context, i);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    // LLVMInitializeNativeAsmParser(); // TODO: to be checked

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(gb_ucg.module, triple);
    Target target;
    if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
        CodeGenOptLevel level = LLVMCodeGenLevelDefault;
        RelocMode reloc = LLVMRelocDefault;
        CodeModel model = LLVMCodeModelDefault;
        TargetMachine machine;
        machine = LLVMCreateTargetMachine(target, triple, "", "", level, reloc, model);
        TargetData layout = LLVMCreateTargetDataLayout(machine);
        LLVMSetModuleDataLayout(gb_ucg.module, layout);
        LLVMDisposeTargetData(layout);
        LLVMDisposeTargetMachine(machine);
    }
    LLVMDisposeMessage(triple);

    // TODO: add asan stuff here
    // TODO: add flags stuff (Passes)
}

void ucg_end() {
    // PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
    // if (gb_ucg.flags) {
    //     Error err = LLVMRunPasses(gb_ucg.module, gb_ucg.flags, NULL, opts);
    //     if (err) {
    //         char *msg = LLVMGetErrorMessage(err);
    //         CHECK(1, "optimizer error: %s", msg);
    //         LLVMDisposeErrorMessage(msg);
    //     }
    // }
    // if (gb_ucg.debug_builder) {
    //     LLVMDIBuilderFinalize(gb_ucg.debug_builder);
    //     LLVMDisposeDIBuilder(gb_ucg.debug_builder);
    //     gb_ucg.debug_builder = NULL;
    // }
    if (LLVMVerifyModule(gb_ucg.module, LLVMReturnStatusAction, &gb_ucg.error_msg)) {
        // clang-format off
        fprintf(stderr, "%s: %s\n", 
			RED("VerifyModule error"), gb_ucg.error_msg);
        // exit(1);
        // clang-format on
    }
    // LLVMVerifyModule always allocate for char* error
    LLVMDisposeMessage(gb_ucg.error_msg);


    // LLVMDisposePassBuilderOptions(opts);
    if (gb_ucg.dir == NULL)
        gb_ucg.dir = getcwd2();
    if (gb_ucg.ll_path == NULL) {
        gb_ucg.ll_path = format("%s/build/out.ll", gb_ucg.dir);
        mkdir2(dirname2(gb_ucg.ll_path));
    }
    if (gb_ucg.exe_path == NULL) {
        gb_ucg.exe_path = format("%s/exe.out", gb_ucg.dir);
    }
    if (LLVMPrintModuleToFile(gb_ucg.module, gb_ucg.ll_path, &gb_ucg.error_msg)) {
        // clang-format off
        fprintf(stderr, "%s: %s (%s)\n", 
			RED("PrintModuleToFile error"), gb_ucg.error_msg, gb_ucg.ll_path);
        exit(1);
        // clang-format on
    }
    // LLVMVerifyModule always allocate for char* error
}

void ucg_exit() {
    if (gb_ucg.error_msg != NULL) {
        LLVMDisposeMessage(gb_ucg.error_msg);
    }
    arena_free();
}

void llvm_fn_dec(Ucg *ir) {
    // assert(ir->fn.ret_type);
    assert(ir->name);

    expand(TypeRef, types);
    zero_init(types);

    // for (int i = 0; i < ir->fn.params_count; i++) {
    //     Ucg *param = ir->fn.params[i];
    //     // param->elem = LLVMGetParam(ir->elem, i);
    //     append(types, param->rtype);
    // }

    TypeRef tmp = LLVMIntTypeInContext(gb_ucg.context, 32);

    ir->rtype = LLVMFunctionType(tmp, NULL, 0, ir->fn.varg);
    ir->elem = LLVMAddFunction(gb_ucg.module, ir->name, ir->rtype);
    // // printf("declare %s, has %zu params\n", ir->name, ir->fn.params_count);
    // for (int i = 0; i < ir->fn.params_count; i++) {
    //     Ucg *param = ir->fn.params[i];
    //     param->elem = LLVMGetParam(ir->elem, i);
    //     // append(types, param->rtype);
    // }
}

void llvm_fn_entry(Ucg *ir) {
    Value elem = ir->elem;
    Bloc bloc = LLVMAppendBasicBlockInContext(gb_ucg.context, elem, "entry");
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
}

bool is_bloc_terminated() {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    return bloc != NULL && LLVMGetBasicBlockTerminator(bloc) != NULL;
}

void llvm_ret(Ucg *ir) {
    if (is_bloc_terminated())
        return;

    if (ir->elem == NULL) {
        LLVMBuildRetVoid(gb_ucg.builder);
        return;
    }
    LLVMBuildRet(gb_ucg.builder, ir->elem);
}
/*
1    0
2    1
4    2
8    3
16   4
32   5
64   6
128  7
*/
TypeRef llvm_type(Ucg *ir) {
    switch (ir->type) { // clang-format off
    case INT:        return gb_ucg.types.num[1 << ir->shift];
    case VAL_CREATE: return llvm_type(ir->ptr);
    default: { 
        eprint("%s: %s:%d handle this case %s\n", 
            RED("error"), FILE, LINE, to_string(ir->type)); 
        exit(1);
    }
    }
    // clang-format on
    return NULL;
}

Ucg *llvm_val_create(Ucg *ir) {
    // assert(ir->rtype);
    long value = ir->ptr->num.value;

    ir->elem = LLVMConstInt(llvm_type(ir), value, 0);
    // if (ir->rtype == gb_ucg.types.i32)
    // else
    //     fprintf(stderr, "handle this case\n");
    return ir;
}

void llvm_declare_var(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    // assert(ir->rtype);
    assert(ir->name);

    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    Value basic = LLVMGetBasicBlockParent(bloc);
    Bloc entry = LLVMGetEntryBasicBlock(basic);

    Value inst = LLVMGetFirstInstruction(entry);
    while (inst && LLVMIsAAllocaInst(inst))
        inst = LLVMGetNextInstruction(inst);

    if (inst)
        LLVMPositionBuilderBefore(gb_ucg.builder, inst);
    else
        LLVMPositionBuilderAtEnd(gb_ucg.builder, entry);

    ir->elem = LLVMBuildAlloca(gb_ucg.builder, ir->rtype, ir->name);
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
}

void llvm_load(Ucg *ir) {
    Ucg *to_load = ir->to_load;
    TypeRef rtype = to_load->rtype;
    Value elem = to_load->elem;
    char *name = ir->name;
    ir->elem = LLVMBuildLoad2(gb_ucg.builder, rtype, elem, name);
}

void llvm_zero_init(Ucg *ir) {
    TypeRef rtype = ir->rtype;
    Value elem = ir->elem;
    LLVMBuildStore(gb_ucg.builder, LLVMConstNull(rtype), elem);
}

void llvm_op(Ucg *ir) {
    // Ucg *left = ir->left;
    // Ucg *right = ir->right;

    // clang-format off
    Type ops[END + 1] = {
        [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
        [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    };
    LLVMOpcode opcodes[END + 1] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub,  [MUL] = LLVMMul,
        [DIV] = LLVMSDiv, [MOD] = LLVMSRem,
        [AND] = LLVMAnd,  [OR] = LLVMOr,
    };
    LLVMIntPredicate predicates[END + 1] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
        [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
    };
    // clang-format on
    Type type = ir->type;
    Value left = ir->left->elem;
    Value right = ir->right->elem;

    Value ptr = NULL;
    if (ops[type]) {
        ptr = left;
        TypeRef rtype = LLVMGetElementType(LLVMTypeOf(ptr));
        left = LLVMBuildLoad2(gb_ucg.builder, rtype, ptr, "old");
        type = ops[type];
    }

    const char *name = to_string(ir->type); // node->token->llvm.name;
    Value value = NULL;
    if (opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, opcodes[type], left, right, name);
    else if (predicates[type])
        value = LLVMBuildICmp(gb_ucg.builder, predicates[type], left, right, name);
    else if (ir->type == ASSIGN) {
        ptr = left;
        value = right;
    } else {
        eprint("unknown operation %s\n", to_string(type));
        exit(1);
    }
    assert(value);
    // assert(ptr);
    if (ptr)
        value = LLVMBuildStore(gb_ucg.builder, value, ptr);
    ir->elem = value;
}

Ucg *ucg_new(Type type) {
    assert(type != 0);
    Ucg *new = arena_alloc(1, sizeof(Ucg));
    new->type = type;
    return new;
}

Ucg *ucg_int_type(int shift) {
    Ucg *new = ucg_new(INT);
    new->shift = log2(shift);
    return new;
}

Ucg *ucg_int_value(int shift, long long value) {
    Ucg *new = ucg_new(VAL_CREATE);

    new->to_create = ucg_int_type(shift);
    new->to_create->num.value = value;

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_load(Ucg *var) {
    Ucg *new = ucg_new(LOAD);
    new->to_load = var;
    new->name = format("load.%s", var->name ? var->name : to_string(var->type));

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_fn(char *name, Ucg **params, int params_count, Ucg *ret) {
    Ucg *new = ucg_new(FN_DEC);
    new->name = name;
    new->fn.ret = ret;
    // new->fn.params = params;

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_ret(Ucg *ret) {
    Ucg *new = ucg_new(RETURN);
    new->to_ret = ret;
    // new->right = ret;
    // new->name = name;
    // new->fn.ret = ret;
    // new->fn.params = params;

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_fn_entry(Ucg *fn) {
    Ucg *new = ucg_new(FN_ENTRY);
    new->ptr = fn;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_declare_var(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(DECLARE);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    Ucg *new = ucg_new(type);
    new->left = left;
    new->right = right;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_return() {
    Ucg *new = ucg_new(RETURN);
    append(gb_ucg.insts, new);
    return new;
}

void print_ucg(Ucg *inst, int space) {
    char *text = format("%*sinst %s", space, "", to_string(inst->type));
    if (inst->name)
        text = format("%s %s", text, inst->name);
    // if (inst->left)
    // 	print_ucg(inst->left, space + TAB);
    // if (inst->right)
    // 	print_ucg(inst->right, space + TAB);
    printf("%s\n", text);
}

void llvm_compile() {
    for (size_t i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        print_ucg(curr, 0);
        switch (curr->type) {
        case FN_DEC: {
            llvm_fn_dec(curr);
            break;
        }
        case FN_ENTRY: {
            llvm_fn_entry(curr->ptr);
            break;
        }
        case VAL_CREATE: {
            llvm_val_create(curr);
            break;
        }
        case RETURN: {
            llvm_ret(curr->ptr);
            break;
        }
        case DECLARE: {
            llvm_declare_var(curr);
            break;
        }
        case LOAD: {
            llvm_load(curr);
            break;
        }
        case ASSIGN: // clang-format off
		case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
		case DIV_ASSIGN: case MOD_ASSIGN: 
        case ADD: case SUB: case MUL: case DIV: case MOD: { // clang-format on
            llvm_op(curr);
            break;
        }
        default: { // clang-format off
            eprint("%s: %s:%d handle this case %s\n", 
                RED("error"), FILE, LINE, to_string(curr->type)); 
            // exit(1);
            break;
        } // clang-format on
        }
    }
}

int main() {
    ucg_start("test");

    Struct(Ucg *) params;
    Ucg *fn = ucg_fn("main", params.arr, params.count, ucg_new(INT));

    Ucg *entry = ucg_fn_entry(fn);

    Ucg *a = ucg_declare_var("a", ucg_int_type(32));
    Ucg *b = ucg_declare_var("b", ucg_int_type(32));

    ucg_op(ASSIGN, a, ucg_int_value(32, 10));
    ucg_op(ASSIGN, b, ucg_op(MUL, ucg_load(a), ucg_int_value(32, 2)));
    ucg_op(ASSIGN, a, ucg_op(ADD, ucg_load(a), ucg_load(b)));

    // ucg_op(ADD_ASSIGN, a, ucg_int_value(32, 5));
    // ucg_op(SUB_ASSIGN, a, ucg_int_value(32, 3));
    // ucg_op(MUL_ASSIGN, a, ucg_int_value(32, 2));
    // ucg_op(DIV_ASSIGN, a, ucg_int_value(32, 4));
    // ucg_op(MOD_ASSIGN, a, ucg_int_value(32, 5));

    // Ucg *ret = ucg_ret(ucg_int_value(32, 123));
    Ucg *ret = ucg_ret(ucg_op(MOD_ASSIGN, a, ucg_load(b)));


    llvm_compile();
    ucg_end();
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
        [VOID] = "VOID",// [BOOL] = "BOOL", [I8] = "I8", [I32] = "I32",
        [INT] = "INT",
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
        return format("UNKNOWN(%d)", type);
    return types[type];
}
