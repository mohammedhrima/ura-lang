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
#include <signal.h> // SIGSEGV

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
    DREF,
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
	BLOC,
    IF, ELIF, ELSE, ENDIF,
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
    int inst_index;

    char *name;

    Type type;
    int shift;
    bool is_val;

    struct { // llvm stuff
        TypeRef rtype;
        Value elem;
        Bloc bloc;
        Bloc then;
    };

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

    // struct {
    //     Bloc cond;
    //     Bloc then;
    //     Bloc end;
    // } bloc;

    union {
        Ucg *left;

        Ucg *data_type;

        Ucg *to_enter; // entry
        Ucg *to_ret;
        Ucg *to_create;
        Ucg *to_dref;
        Ucg *to_end;
    };
    struct {
        Ucg *cond;
        // Ucg *then;
        Ucg *next; // next of the if statement
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
    case VAL_CREATE: return llvm_type(ir->to_create);
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
    long value = ir->to_create->num.value;

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
    Ucg *to_dref = ir->to_dref;
    // to_dref->rtype;
    Value elem = to_dref->elem;
    TypeRef rtype = LLVMGetElementType(LLVMTypeOf(elem));
    char *name = ir->name;

    assert(elem);
    assert(name);
    // assert(rtype);
    ir->elem = LLVMBuildLoad2(gb_ucg.builder, rtype, elem, name);
    // ir->rtype = rtype;
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
    // Type ops[END + 1] = {
    //     [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
    //     [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    // };
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

    const char *name = to_string(ir->type); // node->token->llvm.name;
    Value value = NULL;
    if (opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, opcodes[type], left, right, name);
    else if (predicates[type])
        value = LLVMBuildICmp(gb_ucg.builder, predicates[type], left, right, name);
    else if (ir->type == ASSIGN) {
        value = LLVMBuildStore(gb_ucg.builder, right, left);
    } else {
        eprint("unknown operation %s\n", to_string(type));
        exit(1);
    }
    ir->elem = value;
}

Value llvm_parent_bloc() {
    assert(gb_ucg.builder);
    assert(LLVMGetInsertBlock(gb_ucg.builder));
    return LLVMGetBasicBlockParent(LLVMGetInsertBlock(gb_ucg.builder));
}

Bloc llvm_bloc(char *name) {
    Value parent = llvm_parent_bloc();
    return LLVMAppendBasicBlockInContext(gb_ucg.context, parent, name);
}

void llvm_set_pos(Bloc bloc) {
    if (!is_bloc_terminated())
        LLVMBuildBr(gb_ucg.builder, bloc);
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
}

void llvm_cond(Ucg *ir) {
    Ucg *cond = ir->cond;
    // Ucg *then = ir->then;
    Ucg *next = ir->next;
    assert(cond);
    assert(cond->elem);
    // assert(then);
    assert(ir->then);
    assert(next);
    assert(next->bloc);

    LLVMBuildCondBr(gb_ucg.builder, cond->elem, ir->then, next->bloc);
    llvm_set_pos(ir->then);
}

void ucg_clear(void) {
    memset(gb_ucg.insts, 0, gb_ucg.insts_size * sizeof(Ucg *));
    gb_ucg.insts_count = 0;
}

Ucg *ucg_new(Type type) {
    assert(type != 0);
    Ucg *new = arena_alloc(1, sizeof(Ucg));
    new->type = type;
    static int inst_index = 0;
    new->inst_index = inst_index;
    inst_index++;
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

    new->to_create->is_val = true;
    append(gb_ucg.insts, new->to_create);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_dref(Ucg *var) {
    Ucg *new = ucg_new(DREF);
    new->to_dref = var;
    new->name = format("$%s", var->name ? var->name : to_string(var->type));

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
    new->to_enter = fn;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_return() {
    Ucg *new = ucg_new(RETURN);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_var(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(DECLARE);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    Type ops[END + 1] = {
        [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
        [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    };

    if (ops[type]) {
        type = ops[type];
        Ucg *binary_op = ucg_new(type);
        binary_op->left = ucg_dref(left);
        binary_op->right = right;
        right = binary_op;
        append(gb_ucg.insts, binary_op);
        type = ASSIGN;
    }
    Ucg *new = ucg_new(type);
    new->left = left;
    new->right = right;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_bloc(char *name) {
    Ucg *new = ucg_new(BLOC);
    new->name = name;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_if(Ucg *cond) {
    Ucg *new = ucg_new(IF);
    new->cond = cond;
    // new->then = ucg_bloc("then");
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_endif(Ucg *if_stmt) {
    Ucg *new = ucg_new(ENDIF);
    new->to_end = if_stmt;
    if_stmt->next = new;
    append(gb_ucg.insts, new);
    return new;
}


void print_ucg(Ucg *inst, char end) {
    char *text = "";

    text = format("%s%s%s", text, to_string(inst->type), " ");
    if (inst->name)
        text = format("%s(%s)", text, inst->name ? inst->name : "");
    else if (inst->is_val) {
        switch (inst->type) {
        case INT: {
            text = format("%s(%lld)", text, inst->num.value);
            break;
        }
        default: {
            eprint("%s: %s:%d handle this case %s\n", RED("error"), FILE, LINE,
                   to_string(inst->type));
            exit(1);
        }
        }
    }

    text = format("[%03d] <%-16s>", inst->inst_index, text);
    if (inst->left)
        text = format("%s [%03d]", text, inst->left->inst_index);
    if (inst->right)
        text = format("%s [%03d]", text, inst->right->inst_index);
    printf("%s%c", text, end);
}

void ucg_compile() {
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) {
        case FN_DEC: {
            llvm_fn_dec(curr);
            break;
        }
        case FN_ENTRY: {
            llvm_fn_entry(curr->to_enter);
            break;
        }
        case IF: {
            curr->then = llvm_bloc("then");
            break;
        }
        case ENDIF: {
            curr->bloc = llvm_bloc("endif");
            break;
        }
        default: {
            break;
        }
        }
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        // print_ucg(curr, '\n');
        switch (curr->type) {
        case INT:
            break;
        case FN_DEC: {
            // llvm_fn_dec(curr);
            break;
        }
        case FN_ENTRY: {
            // llvm_fn_entry(curr->to_enter);
            break;
        }
        case VAL_CREATE: {
            llvm_val_create(curr);
            break;
        }
        case RETURN: {
            llvm_ret(curr->to_ret);
            break;
        }
        case DECLARE: {
            llvm_declare_var(curr);
            break;
        }
        case DREF: {
            llvm_load(curr);
            break;
        } // clang-format off
        case ASSIGN: 
        // math ops
		case ADD: case SUB: case MUL: case DIV: case MOD:
		// composite math ops
		case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
		case DIV_ASSIGN: case MOD_ASSIGN: 
		// comparision
		case LT: case GT: case EQ: case LE: case GE: case NQ:
		// logic 
		case AND: case OR: { // clang-format on
            llvm_op(curr);
            break;
        }
        // case BLOC: {
        //     llvm_bloc(curr);
        //     break;
        // }
        case ENDIF: {
            // position at
            llvm_set_pos(curr->bloc);
            break;
        }
        case IF: {
            llvm_cond(curr);
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

#define TESTS_SIZE 3
void run_test(int p) {
    ucg_clear();
    switch (p) {
    case 0: {
        Struct(Ucg *) params = {};
        Ucg *fn = ucg_fn("main", params.arr, params.count, ucg_int_type(32));

        Ucg *entry = ucg_fn_entry(fn);

        Ucg *a = ucg_var("a", ucg_int_type(32));
        Ucg *b = ucg_var("b", ucg_int_type(32));

        ucg_op(ASSIGN, a, ucg_int_value(32, 10));

        ucg_op(ASSIGN, b, ucg_op(MUL, ucg_dref(a), ucg_int_value(32, 2)));
        ucg_op(ASSIGN, a, ucg_op(ADD, ucg_dref(a), ucg_dref(b)));

        ucg_op(ADD_ASSIGN, a, ucg_int_value(32, 5));
        ucg_op(SUB_ASSIGN, a, ucg_int_value(32, 3));
        ucg_op(MUL_ASSIGN, a, ucg_int_value(32, 2));
        ucg_op(DIV_ASSIGN, a, ucg_int_value(32, 4));
        ucg_op(MOD_ASSIGN, a, ucg_int_value(32, 5));

        ucg_ret(ucg_op(ADD, ucg_dref(a), ucg_dref(b)));
        break;
    };
    case 1: {
        Struct(Ucg *) params = {};
        Ucg *fn = ucg_fn("main", params.arr, params.count, ucg_int_type(32));
        Ucg *entry = ucg_fn_entry(fn);

        Ucg *x = ucg_var("a", ucg_int_type(32));
        Ucg *b = ucg_var("b", ucg_int_type(1));
        Ucg *c = ucg_var("c", ucg_int_type(4));

        Ucg *if_stmt = ucg_if( // clang-format off
			ucg_op(OR, 
				ucg_op(EQ, ucg_dref(b), ucg_int_value(1, 1)),
				ucg_op(NQ, ucg_dref(c), ucg_int_value(4, 0))
			)
		); // clang-format on
        ucg_ret(ucg_int_value(32, 1));
        ucg_endif(if_stmt);

        ucg_ret(ucg_dref(x));
        break;
    }
    case 2: {
        Struct(Ucg *) params = {};
        Ucg *fn = ucg_fn("main", params.arr, params.count, ucg_int_type(32));
        Ucg *entry = ucg_fn_entry(fn);

        Ucg *flag = ucg_var("flag", ucg_int_type(1));

        Ucg *if_stmt = ucg_if( // clang-format off
			ucg_op(EQ, 
				ucg_dref(flag),
				ucg_int_value(1, 1)
			)
		); // clang-format on
        ucg_ret(ucg_int_value(32, 1));
        ucg_endif(if_stmt);

        ucg_ret(ucg_int_value(32, 0));

        break;
    }
    case 3: {
        Struct(Ucg *) params = {};
        Ucg *fn = ucg_fn("main", params.arr, params.count, ucg_int_type(32));
        Ucg *entry = ucg_fn_entry(fn);

        Ucg *a = ucg_var("a", ucg_int_type(32));
        Ucg *big = ucg_var("big", ucg_int_type(1));

        ucg_op(ASSIGN, // clang-format off
			big, 
			ucg_op(LT, 
				ucg_dref(a), 
				ucg_int_value(32, 3)
			)
		); // clang-format on

		Ucg *n = ucg_var("n", ucg_int_type(32));

		ucg_op(ASSIGN, // clang-format off
			n,
			ucg_int_value(32, 0)
		);  // clang-format on

		Ucg *if_stmt = ucg_if( // clang-format off
			ucg_op(EQ,
				ucg_dref(big),
				ucg_int_value(1, 1)
			)
		); // clang-format on

		ucg_op(ADD_ASSIGN, // clang-format off
			n,
			ucg_int_value(32, 1)
		); // clang-format on

		ucg_endif(if_stmt);

		ucg_op(ASSIGN, // clang-format off
			big,
			ucg_op(LT,
				ucg_dref(a),
				ucg_int_value(32, 9)
			)
		); // clang-format on

		if_stmt = ucg_if( // clang-format off
			ucg_op(EQ,
				ucg_dref(big),
				ucg_int_value(1, 1)
			)
		); // clang-format on

		ucg_op(ADD_ASSIGN, 
			n,
			ucg_int_value(32, 10)
		);

		ucg_endif(if_stmt);

		ucg_ret(ucg_int_value(32, 0));
        break;
    }
    default: {
        eprint("exceeded numbers of tests");
        exit(1);
        break;
    }
    }
}

/*
TODO:
    + add default return
*/

int main() {
    ucg_start("test");

#if 0
    for (int i = 0; i <= TESTS_SIZE; i++) {
		printf(CYAN("Test %d: ================================\n"), i);
        run_test(i);
        for (size_t i = 0; i < gb_ucg.insts_count; i++) {
            Ucg *curr = gb_ucg.insts[i];
            print_ucg(curr, '\n');
        }
    }
#else
    run_test(TESTS_SIZE);
    for (size_t i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        print_ucg(curr, '\n');
    }
#endif

    ucg_compile();

    ucg_end();
}

// UTILS
const char *to_string(Type type) {
    char *types[END + 1] = {
        // clang-format off
        [ERR] = "ERR",
        [ID] = "ID",
        [SCOPE] = "SCOPE", [URA] = "URA",

        [VAR] = "VAR", [DECLARE] = "DECL", [INIT_VAR] = "INIT",
        [ARG] = "ARG",

        [VAL_CREATE] = "VCREATE",
        [TEMPLATE] = "TMPL",
        [TEMPLATE_TYPE] = "TMPLT",
        [TYPES] = "TYPES",

        [ANY] = "ANY",
        [VOID] = "VOID",
        [INT] = "INT",
        [CHARS] = "STR",
        [REF] = "REF",
        [ZERO] = "ZERO",

        [STRUCT] = "STRU", [DOT] = "DOT", [ATTR] = "ATTR",

        [DREF] = "DREF",

        [LPARENT] = "LPAR", [RPARENT] = "RPAR",
        [LBRACK] = "LBRK", [RBRACK] = "RBRK",
        [DOTS] = "DOTS", [COMA] = "COMA",

        [ARRAY] = "ARRAY", [ARRAY_LIT] = "ALIT",
        [ACCESS] = "ACCESS",

        [ASSIGN] = "ASSIGN",
        [ADD_ASSIGN] = "ADDA", [SUB_ASSIGN] = "SUBA",
        [MUL_ASSIGN] = "MULA", [DIV_ASSIGN] = "DIVA",
        [MOD_ASSIGN] = "MODA",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE",
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [AND] = "AND", [OR] = "OR", [NOT] = "NOT",

        [PROTO] = "PROTO", [FN_ENTRY] = "FENT",
        [FN_DEC] = "FDEC", [ARGS] = "ARGS", [VARIADIC] = "VARA",
        [FN_CALL] = "FCAL", [RETURN] = "RET",
        [SIZEOF] = "SIZE", [IS_NULLABLE] = "NULL",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
		[ENDIF] = "ENDIF",
		[BLOC] = "BLOC",
        [WHILE] = "WHILE", [BRK] = "BRK", [CNT] = "CNT",
        [AS] = "AS",
        [END] = "END",
        // clang-format on
    };

    if (types[type] == NULL)
        return format("UNKNOWN(%d)", type);

    return types[type];
}