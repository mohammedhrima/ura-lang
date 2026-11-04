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
	PARAM,
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
    IF, ELIF, ELSE, THEN,
    LOOP, BRK, CNT,
    END,
    LAST,
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
        // struct {
        //     Ucg **arr;
        //     size_t count;
        //     size_t size;
        // }
        expand(Ucg *, params);
    } fn;

    struct {
        long long value;
    } num;

    struct {
        Ucg *elem;
        expand(Ucg *, items);
    } arr;

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
        Ucg *to_ref;
        Ucg *to_end; // used in ENDIF
        Ucg *to_call;
        Ucg *stmt;
        // Ucg
    };
    struct {
        Ucg *cond;
        // Ucg *then;
        Ucg *next; // next of the if statement
        // Ucg *head;
        // Ucg *last;
        Ucg *end; // ENDIF
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

void ucg_finish() {
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

    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMContextDispose(gb_ucg.context);
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

    for (int i = 0; i < ir->fn.params_count; i++) {
        Ucg *param = ir->fn.params[i];
        // param->elem = LLVMGetParam(ir->elem, i);
        assert(param);
        assert(param->type);
        // printf(CYAN("access %d -> %d\n"), i, param->type);
        param->rtype = llvm_type(param);
        append(types, param->rtype);
    }

    // TypeRef tmp = LLVMIntTypeInContext(gb_ucg.context, 32);

    ir->rtype = LLVMFunctionType( // clang-format off
							llvm_type(ir->fn.ret), 
							types, 
							types_count, 
							ir->fn.varg
						); // clang-format on

    ir->elem = LLVMAddFunction(gb_ucg.module, ir->name, ir->rtype);
    // // printf("declare %s, has %zu params\n", ir->name, ir->fn.params_count);
    for (int i = 0; i < ir->fn.params_count; i++) {
        Ucg *param = ir->fn.params[i];
        param->elem = LLVMGetParam(ir->elem, i);
        // append(types, param->rtype);
    }
}

void llvm_fn_call(Ucg *ir) {
    Ucg *fn_dec = ir->to_call;
    expand(Value, params);
    zero_init(params);

    for (int i = 0; i < ir->fn.params_count; i++) {
        append(params, ir->fn.params[i]->elem);
    }
    TypeRef rtype = fn_dec->rtype;
    Value fn_elem = fn_dec->elem;
    char *name = ir->name;
    Value elem = LLVMBuildCall2(gb_ucg.builder, // clang-format off
                                rtype, 
                                fn_elem, params, 
                                params_count, 
                                name 
                            ); // clang-format on
    ir->elem = elem;
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


TypeRef llvm_type(Ucg *ir) {
    switch (ir->type) { // clang-format off
    case INT:        	return gb_ucg.types.num[1 << ir->shift];
    case VAL_CREATE:	return llvm_type(ir->to_create);
    case DECLARE:		return llvm_type(ir->data_type);
    case PARAM:			return llvm_type(ir->data_type);
	case ARRAY: {
		Ucg* elem = ir->arr.elem;
		int count = ir->arr.items_count;
		return LLVMArrayType(llvm_type(elem), count);
	}
	case REF: {
		return LLVMPointerType(llvm_type(ir->to_ref), 0);
	}
    default: { 
        eprint("%s: %s:%d handle this case %s\n", 
            RED("error"), FILE, LINE, to_string(ir->type)); 
        exit(1);
    }
    }
    // clang-format on
    return NULL;
}

void llvm_val_create(Ucg *ir) {
    // assert(ir->rtype);
    Ucg *to_create = ir->to_create;
    switch (to_create->type) {
    case INT: {
        long value = to_create->num.value;
        ir->elem = LLVMConstInt(llvm_type(to_create), value, 0);
        break;
    }
    case ARRAY: {
        TypeRef rtype = llvm_type(to_create);
        Value agg = LLVMGetUndef(rtype);
        for (int i = 0; i < to_create->arr.items_count; i++) {
            agg = LLVMBuildInsertValue( // clang-format off
								gb_ucg.builder, agg, 
								to_create->arr.items[i]->elem, 
								i, ""); // clang-format on
        }

        Value stack = llvm_alloc(rtype, "arr");
        LLVMBuildStore(gb_ucg.builder, agg, stack);

        TypeRef i32 = gb_ucg.types.num[32];
        Value zero[] = {
            LLVMConstInt(i32, 0, 0),
            LLVMConstInt(i32, 0, 0),
        };
        ir->elem = LLVMBuildInBoundsGEP2( // clang-format off
										gb_ucg.builder, 
										rtype, stack, zero, 
										2, ""); // clang-format on

        break;
    }
    default: { // clang-format off
        eprint("%s: %s:%d handle this case %s\n", 
			RED("error"), FILE, LINE, to_string(to_create->type));
        exit(1);
    } // clang-format on
    }
}


Value llvm_alloc(TypeRef rtype, char *name) {
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

    Value elem = LLVMBuildAlloca(gb_ucg.builder, rtype, name);
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
    return elem;
}

void llvm_declare_var(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    // assert(ir->rtype);
    assert(ir->name);
    ir->elem = llvm_alloc(ir->rtype, ir->name);
}

void llvm_declare_param(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    Value elem = llvm_alloc(ir->rtype, ir->name);

    LLVMBuildStore(gb_ucg.builder, ir->elem, elem);
    ir->elem = elem;
}

void llvm_load(Ucg *ir) {
    Ucg *to_dref = ir->to_dref;
    // to_dref->rtype;
    Value elem = to_dref->elem;
    TypeRef rtype = llvm_type(to_dref);
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
    LLVMOpcode opcodes[LAST + 1] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub,  [MUL] = LLVMMul,
        [DIV] = LLVMSDiv, [MOD] = LLVMSRem,
        [AND] = LLVMAnd,  [OR] = LLVMOr,
    };
    LLVMIntPredicate predicates[LAST + 1] = {
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

Bloc llvm_bloc(Value fn, char *name) {
    return LLVMAppendBasicBlockInContext(gb_ucg.context, fn, name);
}

void llvm_set_pos(Bloc bloc) {
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
}

void llvm_jmp(Bloc bloc) {
    if (!is_bloc_terminated())
        LLVMBuildBr(gb_ucg.builder, bloc);
}

void llvm_cond(Ucg *ir) {
    assert(ir->then);

    if (ir->cond) {
        Ucg *cond = ir->cond;
        assert(cond->elem);
        Ucg *next = ir->next ? ir->next : ir->end;
        assert(next && next->bloc);
        LLVMBuildCondBr(gb_ucg.builder, cond->elem, ir->then, next->bloc);
    }
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
    // printf(">>>> %d\n", shift);
    new->shift = log2(shift);
    // printf(">>>> %d\n", new->shift);
    return new;
}

Ucg *ucg_i1_type() { // boolean
    return ucg_int_type(1);
}

Ucg *ucg_i8_type() {
    return ucg_int_type(8);
}

Ucg *ucg_char_type() {
    return ucg_i8_type();
}

Ucg *ucg_i32_type() {
    return ucg_int_type(32);
}

Ucg *ucg_ref_type(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    return new;
}

Ucg *ucg_array_type(Ucg *elem, int count) {
    Ucg *new = ucg_new(ARRAY);
    new->arr.elem = elem;
    new->arr.items_count = count;
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

Ucg *ucg_i1_value(long long value) {
    return ucg_int_value(1, value);
}

Ucg *ucg_i8_value(long long value) {
    return ucg_int_value(8, value);
}

Ucg *ucg_bool_value(long long value) {
    return ucg_i1_value(value);
}

Ucg *ucg_char_value(long long value) {
    return ucg_i8_value(value);
}

Ucg *ucg_i32_value(long long value) {
    return ucg_int_value(32, value);
}

Ucg *ucg_array_new(Ucg *elem) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ucg_new(ARRAY);
    new->to_create->arr.elem = elem;
    return new;
}

__attribute__((sentinel)) Ucg *ucg_array_value(Ucg *elem, ...) {
    Ucg *new = ucg_array_new(elem);
    Ucg *to_create = new->to_create;
    va_list ap;
    va_start(ap, elem);
    for (Ucg *v = va_arg(ap, Ucg *); v; v = va_arg(ap, Ucg *))
        append(to_create->arr.items, v);
    va_end(ap);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_value(char *str) {
    Ucg *new = ucg_array_new(ucg_i8_type());
    Ucg *to_create = new->to_create;
    for (int i = 0; i <= strlen(str); i++)
        append(to_create->arr.items, ucg_i8_value(str[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_ref(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    const char *name = to_ref->name ? to_ref->name : to_string(to_ref->type);
    new->name = format("&%s", name);

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

Ucg *ucg_fn_dec_(char *name, Ucg *ret, va_list ap) {
    Ucg *new = ucg_new(FN_DEC);
    new->name = name;
    new->fn.ret = ret;

    for (Ucg *param = va_arg(ap, Ucg *); param; param = va_arg(ap, Ucg *)) {
        if (param->type == VARIADIC)
            new->fn.varg = true;
        else
            append(new->fn.params, param);
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_fn_call_(Ucg *fn_dec, va_list ap) {
    Ucg *new = ucg_new(FN_CALL);
    new->name = format("call.%s", fn_dec->name);
    new->to_call = fn_dec;
    // new->fn.ret = ret;


    for (Ucg *param = va_arg(ap, Ucg *); param; param = va_arg(ap, Ucg *)) {
        // if (param->type == VARIADIC)
        //     new->fn.varg = true;
        // else
        append(new->fn.params, ucg_value(param));
    }
    // append(gb_ucg.insts, new);
    return new;
}

// to force variadic arg to ends with NULL
__attribute__((sentinel)) Ucg *ucg_proto(char *name, Ucg *ret, ...) {
    va_list ap;
    va_start(ap, ret);
    Ucg *new = ucg_fn_dec_(name, ret, ap);
    va_end(ap);
    return new;
}

__attribute__((sentinel)) Ucg *ucg_fn(char *name, Ucg *ret, ...) {
    va_list ap;
    va_start(ap, ret);
    Ucg *new = ucg_fn_dec_(name, ret, ap);
    va_end(ap);

    Ucg *entry = ucg_new(FN_ENTRY);
    entry->to_enter = new;
    append(gb_ucg.insts, entry);
    return new;
}

__attribute__((sentinel)) Ucg *ucg_fn_call(Ucg *fn_dec, ...) {
    va_list ap;
    va_start(ap, fn_dec);
    Ucg *new = ucg_fn_call_(fn_dec, ap);
    va_end(ap);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_varg(void) {
    return ucg_new(VARIADIC);
}

Ucg *ucg_get_param(Ucg *fn, int index) {
    return fn->fn.params[index];
}

Ucg *ucg_ret(Ucg *ret) {
    Ucg *new = ucg_new(RETURN);
    new->to_ret = ucg_value(ret);
    // new->right = ret;
    // new->name = name;
    // new->fn.ret = ret;
    // new->fn.params = params;

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

Ucg *ucg_param(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(PARAM);
    new->name = name;
    new->data_type = data_type;
    // append(gb_ucg.insts, new);
    return new;
}

bool is_locator(Ucg *ir) {
    return ir->type == DECLARE || ir->type == PARAM;
}

Ucg *ucg_value(Ucg *ir) {
    assert(ir);
    return is_locator(ir) ? ucg_dref(ir) : ir;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    Type ops[LAST + 1] = {
        [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
        [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    };

    if (type != ASSIGN && !ops[type])
        left = ucg_value(left);
    right = ucg_value(right);

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

Ucg *ucg_assign(Ucg *left, Ucg *right) {
    return ucg_op(ASSIGN, left, right);
}

Ucg *ucg_bloc(char *name) {
    Ucg *new = ucg_new(BLOC);
    new->name = name;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_branch(Type type, Ucg *prev_stmt) {
    Ucg *new = ucg_new(type);
    if (prev_stmt) {
        while (prev_stmt->next)
            prev_stmt = prev_stmt->next;
        prev_stmt->next = new;
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_loop() {
    return ucg_branch(LOOP, NULL);
}

Ucg *ucg_if(void) {
    return ucg_branch(IF, NULL);
}

Ucg *ucg_elif(Ucg *prev_stmt) {
    return ucg_branch(ELIF, prev_stmt);
}

Ucg *ucg_else(Ucg *prev_stmt) {
    return ucg_branch(ELSE, prev_stmt);
}

Ucg *ucg_then(Ucg *stmt, Ucg *cond) {
    Ucg *new = ucg_new(THEN);
    stmt->cond = cond ? ucg_value(cond) : NULL;
    new->stmt = stmt;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_end(Ucg *stmt) {
    Ucg *new = ucg_new(END);
    new->to_end = stmt;
    while (stmt) {
        stmt->end = new;
        stmt = stmt->next;
    }
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

    text = format("[%03d] %-16s", inst->inst_index, text);
    if (inst->left)
        text = format("%s [%03d]", text, inst->left->inst_index);
    if (inst->right)
        text = format("%s [%03d]", text, inst->right->inst_index);
    printf("%s%c", text, end);
}

void ucg_compile() {
    Value fn = NULL;
    int stmt_index = 0;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) {
        case FN_DEC: {
            llvm_fn_dec(curr);
            break;
        }
        case FN_ENTRY: {
            fn = curr->to_enter->elem;
            curr->bloc = llvm_bloc(fn, "entry");
            break;
        }
        case LOOP: {
            curr->bloc = llvm_bloc(fn, format("while%d", stmt_index));
            curr->then = llvm_bloc(fn, format("while%d.then", stmt_index));
            stmt_index++;
            break;
        }
        case IF: {
            curr->bloc = llvm_bloc(fn, format("if%d", stmt_index));
            curr->then = llvm_bloc(fn, format("if%d.then", stmt_index));
            stmt_index++;
            break;
        }
        case ELIF: {
            curr->bloc = llvm_bloc(fn, format("elif%d", stmt_index));
            curr->then = llvm_bloc(fn, format("elif%d.then", stmt_index));
            stmt_index++;
            break;
        }
        case ELSE: {
            curr->bloc = llvm_bloc(fn, format("else%d", stmt_index));
            curr->then = curr->bloc;
            stmt_index++;
            break;
        }
        case END: {
            Ucg *to_end = curr->to_end;
            char *name = to_end->type == LOOP ? "endloop" : "endif";
            curr->bloc = llvm_bloc(fn, name);
            // stmt_index++;
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
        // case ARRAY_LIT: {
        // 	// llvm_array_value(curr);
        // 	break;
        // }
        case FN_DEC: {
            // llvm_fn_dec(curr);
            break;
        }
        case FN_ENTRY: {
            llvm_set_pos(curr->bloc);
            Ucg *fn = curr->to_enter;
            // assert()
            for (int i = 0; i < fn->fn.params_count; i++) {
                Ucg *param = fn->fn.params[i];
                llvm_declare_param(param);
            }

            break;
        }
        case RETURN: {
            llvm_ret(curr->to_ret);
            break;
        }
        case FN_CALL: {
            llvm_fn_call(curr);
            break;
        }
        case VAL_CREATE: {
            llvm_val_create(curr);
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
		case ADD: case SUB: case MUL: case DIV: case MOD: // math
		case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite 
		case DIV_ASSIGN: case MOD_ASSIGN: 
		case LT: case GT: case EQ: case LE: case GE: case NQ: // comparision
		case AND: case OR: { // logic
            // clang-format on
            llvm_op(curr);
            break;
        }
        // case BLOC: {
        //     llvm_bloc(curr);
        //     break;
        // }
        // clang-format off
        case IF: case ELIF: case ELSE: { // clang-format on
            assert(curr->end);

            llvm_jmp(curr->type == IF ? curr->bloc : curr->end->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        case LOOP: {
            llvm_jmp(curr->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        case THEN: {
            llvm_cond(curr->stmt);
            break;
        }
        case END: {
            // position at
            Ucg *stmt = curr->to_end;
            llvm_jmp(stmt->type == LOOP ? stmt->bloc : curr->bloc);
            llvm_set_pos(curr->bloc);
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

bool run_test(int p) {
    ucg_clear();
    switch (p) {
    case 0: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        // Ucg *entry = ucg_fn_entry(fn);

        Ucg *a = ucg_var("a", ucg_i32_type());
        Ucg *b = ucg_var("b", ucg_i32_type());

        ucg_op(ASSIGN, a, ucg_i32_value(10));

        ucg_op(ASSIGN, b, ucg_op(MUL, a, ucg_i32_value(2)));
        ucg_op(ASSIGN, a, ucg_op(ADD, a, b));

        ucg_op(ADD_ASSIGN, a, ucg_i32_value(5));
        ucg_op(SUB_ASSIGN, a, ucg_i32_value(3));
        ucg_op(MUL_ASSIGN, a, ucg_i32_value(2));
        ucg_op(DIV_ASSIGN, a, ucg_i32_value(4));
        ucg_op(MOD_ASSIGN, a, ucg_i32_value(5));

        ucg_ret(ucg_op(ADD, a, b));
        break;
    };
    case 1: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *x = ucg_var("a", ucg_i32_type());
        Ucg *b = ucg_var("b", ucg_i1_type());
        Ucg *c = ucg_var("c", ucg_char_type());

        Ucg *if_stmt = ucg_if();
        ucg_then(if_stmt, // clang-format off
            ucg_op(OR, 
                ucg_op(EQ, b, ucg_i1_value(1)),
                ucg_op(NQ, c, ucg_char_value(0))
            )
        ); // clang-format on

        ucg_ret(ucg_i32_value(1));
        ucg_end(if_stmt);

        ucg_ret(x);
        break;
    }
    case 2: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *flag = ucg_var("flag", ucg_i1_type());

        Ucg *if_stmt = ucg_if();
        ucg_then(if_stmt, // clang-format off
            ucg_op(EQ, 
				flag,
				ucg_i1_value(1)
			)
        ); // clang-format on

        ucg_ret(ucg_i32_value(1));

        Ucg *else_stmt = ucg_else(if_stmt);
        ucg_then(else_stmt, NULL);
        ucg_ret(ucg_i32_value(2));

        ucg_end(if_stmt);

        ucg_ret(ucg_i32_value(0));

        break;
    }
    case 3: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *a = ucg_var("a", ucg_i32_type());
        Ucg *big = ucg_var("big", ucg_i1_type());

        ucg_op(ASSIGN, big, ucg_op(LT, a, ucg_i32_value(3)));

        Ucg *n = ucg_var("n", ucg_i32_type());

        ucg_op(ASSIGN, // clang-format off
			n,
			ucg_i32_value(0)
		); // clang-format on

        Ucg *if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(EQ, big, ucg_i1_value(1)));

        ucg_op(ADD_ASSIGN, // clang-format off
			n,
			ucg_i32_value(1)
		); // clang-format on

        ucg_end(if_stmt);

        ucg_op(ASSIGN, // clang-format off
			big,
			ucg_op(LT,
				a,
				ucg_i32_value(9)
			)
		); // clang-format on

        if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(EQ, big, ucg_i1_value(1)));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
        ucg_end(if_stmt);

        ucg_op(ASSIGN, big, ucg_i1_value(0));

        if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(EQ, big, ucg_i1_value(1)));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(100));
        ucg_end(if_stmt);

        if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(EQ, big, ucg_i1_value(0)));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(if_stmt);

        ucg_ret(n);
        break;
    }
    case 4: {
        Ucg *is_even = ucg_fn("is_even", ucg_i1_type(), // clang-format off
				ucg_param("n", ucg_i32_type()), 
				NULL
			); // clang-format on

        Ucg *n = ucg_get_param(is_even, 0);

        ucg_ret(ucg_op(EQ, // clang-format off
					ucg_op(MOD, 
						n, 
						ucg_i32_value(2)), 
					ucg_i32_value(0)
				) // clang-format on
        );

        Ucg *both = ucg_fn("both", ucg_i1_type(), // clang-format off
							ucg_param("a", ucg_i1_type()),
                        	ucg_param("b", ucg_i1_type()), 
							NULL
						); // clang-format on

        Ucg *a = ucg_get_param(both, 0);
        Ucg *b = ucg_get_param(both, 1);

        ucg_ret(ucg_op(AND, a, b));


        Ucg *main_fn = ucg_fn("main", ucg_i32_type(), NULL);
        ucg_ret(ucg_i32_value(0));

        break;
    }
    case 5: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *flag = ucg_var("flag", ucg_i1_type());

        Ucg *if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(EQ, flag, ucg_i1_value(1)));
        ucg_ret(ucg_i32_value(1));

        Ucg *elif_stmt = ucg_elif(if_stmt);
        ucg_then(elif_stmt, ucg_op(EQ, flag, ucg_i1_value(0)));
        ucg_ret(ucg_i32_value(2));

        Ucg *else_stmt = ucg_else(if_stmt);
        ucg_then(else_stmt, NULL);
        ucg_ret(ucg_i32_value(3));

        ucg_end(if_stmt);

        ucg_ret(ucg_i32_value(0));
        break;
    }
    case 6: {
        Ucg *is_even = ucg_fn("is_even", ucg_i1_type(), // clang-format off
        		ucg_param("n1", ucg_i32_type()),
        		NULL
        	); // clang-format on

        Ucg *n = ucg_get_param(is_even, 0);

        ucg_ret(ucg_op(EQ, // clang-format off
        			ucg_op(MOD,
        				n,
        				ucg_i32_value(2)),
        			ucg_i32_value(0)
        		) // clang-format on
        );

        Ucg *both = ucg_fn("both", ucg_i1_type(), // clang-format off
        					ucg_param("a", ucg_i1_type()),
                        	ucg_param("b", ucg_i1_type()),
        					NULL
        				); // clang-format on

        Ucg *a = ucg_get_param(both, 0);
        Ucg *b = ucg_get_param(both, 1);

        ucg_ret(ucg_op(AND, a, b));


        Ucg *main_fn = ucg_fn("main", ucg_i32_type(), NULL);
        Ucg *n0 = ucg_var("n0", ucg_i32_type());

        Ucg *if_stmt = ucg_if();
        ucg_then(if_stmt, // clang-format off
            ucg_op(EQ,
                ucg_fn_call(is_even, ucg_i32_value(4), NULL),
                ucg_i1_value(1)
            )
        ); // clang-format on
        ucg_op(ADD_ASSIGN, n0, ucg_i32_value(1));

        ucg_end(if_stmt);

        if_stmt = ucg_if();
        ucg_then(if_stmt, // clang-format off
            ucg_op(EQ,
                ucg_fn_call(both, 
                    ucg_fn_call(is_even, ucg_i32_value(2), NULL), 
                    ucg_fn_call(is_even, ucg_i32_value(6), NULL), 
                    NULL),
                ucg_i1_value(1)
            )
        ); // clang-format on
        ucg_op(ADD_ASSIGN, n0, ucg_i32_value(2));

        ucg_end(if_stmt);

        if_stmt = ucg_if();
        ucg_then(if_stmt, // clang-format off
            ucg_op(EQ,
                ucg_fn_call(both, 
                    ucg_fn_call(is_even, ucg_i32_value(2), NULL), 
                    ucg_fn_call(is_even, ucg_i32_value(7), NULL), 
                    NULL),
                ucg_i1_value(1)
            )
        ); // clang-format on
        ucg_op(ADD_ASSIGN, n0, ucg_i32_value(10));

        ucg_end(if_stmt);

        // Ucg *f = ucg_fn_call(foo, ucg_i32_value(3), NULL);
        ucg_ret(n0);
        break;
    }
    case 7: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL);
        Ucg *a = ucg_var("a", ucg_i1_type());
        Ucg *b = ucg_var("b", ucg_i1_type());
        Ucg *n = ucg_var("n", ucg_i32_type());

        ucg_op(ASSIGN, a, ucg_i1_value(0));
        ucg_op(ASSIGN, a, ucg_i1_value(1));
        ucg_op(ASSIGN, n, ucg_i32_value(0));

        Ucg *if_stmt = NULL;
        if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(OR, a, b));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(5));
        ucg_end(if_stmt);


        if_stmt = ucg_if();
        ucg_then(if_stmt, ucg_op(AND, a, b));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(50));
        ucg_end(if_stmt);

        ucg_ret(n);

        break;
    }
    case 8: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_op(ASSIGN, a, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        ucg_then(loop, // clang-format off
            ucg_op(LT, 
                a, 
                ucg_i32_value(5)
            )
        ); // clang-format on

        ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));

        ucg_end(loop);

        ucg_ret(a);
        break;
    }
    case 9: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_op(ASSIGN, i, ucg_i32_value(0));

        Ucg *going = ucg_var("going", ucg_i1_type());
        ucg_op(ASSIGN, going, ucg_i1_value(1));

        Ucg *loop = ucg_loop();
        ucg_then(loop, // clang-format off
            ucg_op(EQ, 
                going,
                ucg_i1_value(1)
            )
        ); // clang-format on

        ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        ucg_op(ASSIGN, going, // clang-format off
			ucg_op(
				LT,
				i,
				ucg_i32_value(4)
			)
		); // clang-format on

        ucg_end(loop);

        ucg_ret(i);
        break;
    }
    case 10: {
        Ucg *printf = ucg_proto("printf", // clang-format off
						ucg_i32_type(), 
						ucg_ref_type(ucg_i8_type()), 
						ucg_varg(),
						NULL
					); // clang-format on

        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);
        Ucg *a = ucg_var("a", ucg_char_type());
        Ucg *b = ucg_var("b", ucg_char_type());

        // Ucg *b = ucg_var('b', ucg_i8_type());

        ucg_assign(a, ucg_char_value('a'));
        ucg_assign(b, ucg_char_value('b'));

        ucg_fn_call(printf, // clang-format off
					ucg_string_value("<%c%c>\n"), 
					a, b,
                    NULL); // clang-format on

        ucg_ret(ucg_i32_value(0));
        break;
    }
    case 11: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *c = ucg_var("c", ucg_char_type());
        ucg_assign(c, ucg_char_value('x'));

        Ucg *n = ucg_var("n", ucg_i32_type());
        ucg_assign(n, ucg_i32_value(0));

        Ucg *stmt = ucg_if();
        ucg_then(stmt, ucg_op(EQ, c, ucg_char_value('x')));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(stmt);

        stmt = ucg_if();
        ucg_then(stmt, ucg_op(NQ, c, ucg_char_value('y')));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(stmt);


        stmt = ucg_if();
        ucg_then(stmt, ucg_op(EQ, c, ucg_char_value('z')));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
        ucg_end(stmt);

        ucg_ret(n);
        break;
    }
    case 12: {
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);

        Ucg *c = ucg_var("c", ucg_char_type());
        ucg_assign(c, ucg_char_value('m'));

        Ucg *n = ucg_var("n", ucg_i32_type());
        ucg_assign(n, ucg_i32_value(0));

        Ucg *stmt = ucg_if();
        ucg_then(stmt, ucg_op(GT, c, ucg_char_value('a')));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(stmt);

        stmt = ucg_if();
        ucg_then(stmt, ucg_op(LT, c, ucg_char_value('z')));
        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(stmt);

        stmt = ucg_if();
        ucg_then(stmt, // clang-format off
			ucg_op(AND,
				ucg_op(GE, c, ucg_char_value('m')),
				ucg_op(LE, c, ucg_char_value('m'))
			)); // clang-format on

        ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
        ucg_end(stmt);

        ucg_ret(n);
        break;
    }
    case 13: {
		// clang-format off
        Ucg *printf = ucg_proto("printf",
							ucg_i32_type(), 
							ucg_ref_type(ucg_char_type()),
							ucg_varg(),
							NULL); // clang-format on
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);
        Ucg *nl = ucg_var("nl", ucg_char_type());
        ucg_assign(nl, ucg_char_value('\n'));

        Ucg *tab = ucg_var("tab", ucg_char_type());
        ucg_assign(tab, ucg_char_value('\t'));

        ucg_fn_call(printf, ucg_string_value("[%c]\n"), nl, NULL);
        ucg_fn_call(printf, ucg_string_value("[%c]\n"), tab, NULL);
        ucg_fn_call(printf, // clang-format off
			ucg_string_value("[%c] [%c]\n"), 
			ucg_char_value('\''),
			ucg_char_value('\\'),
			NULL); // clang-format on


        ucg_ret(ucg_i32_value(0));
        break;
    }
    case 14: {
		// clang-format off
        Ucg *printf = ucg_proto("printf",
							ucg_i32_type(), 
							ucg_ref_type(ucg_char_type()),
							ucg_varg(),
							NULL); // clang-format on
		
		
        Ucg *fn = ucg_fn("main", ucg_i32_type(), NULL);
        Ucg *nl = ucg_var("nl", ucg_char_type());
        ucg_assign(nl, ucg_char_value('\n'));

        Ucg *tab = ucg_var("tab", ucg_char_type());
        ucg_assign(tab, ucg_char_value('\t'));

        ucg_fn_call(printf, ucg_string_value("[%c]\n"), nl, NULL);
        ucg_fn_call(printf, ucg_string_value("[%c]\n"), tab, NULL);
        ucg_fn_call(printf, // clang-format off
			ucg_string_value("[%c] [%c]\n"), 
			ucg_char_value('\''),
			ucg_char_value('\\'),
			NULL); // clang-format on

        ucg_ret(ucg_i32_value(0));
        break;
    }
    default: {
        return false;
        break;
    }
    }
    return true;
}

/*
TODO:
    + add default return
    + test it with all llvm versions
    + asm gen:
        get rid of .ll file generation when you finish
        since llvm can automaticaly generate the assembly
        that's compatible without saving it in external file
*/

bool compile_test(int p) {
    printf(CYAN("Test %d: ================================\n"), p);
    ucg_start(format("test%d", p));
    bool r = run_test(p);
    for (size_t i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        print_ucg(curr, '\n');
    }
    ucg_compile();
    gb_ucg.ll_path = format("build/out.ll");
    ucg_finish();
    return r;
}

int main() {
    mkdir2("build");

#if 1
    compile_test(14);
#else
    for (int i = 0; compile_test(i); i++)
        ;
#endif
}

// UTILS
const char *to_string(Type type) {
    char *types[LAST + 1] = {
        // clang-format off
		[ERR] = "ERR",
		[ID] = "ID", [SCOPE] = "SCOPE", [URA] = "URA",

		[VAR] = "VAR", [DECLARE] = "DECLARE", [INIT_VAR] = "INIT_VAR",
		[ARG] = "ARG", [PARAM] = "PARAM",

		[VAL_CREATE] = "VAL_CREATE",
		[TEMPLATE] = "TEMPLATE", [TEMPLATE_TYPE] = "TEMPLATE_TYPE",
		[TYPES] = "TYPES",

		[ANY] = "ANY",
		[VOID] = "VOID",
		[INT] = "INT",
		[CHARS] = "CHARS",
		[REF] = "REF",
		[ZERO] = "ZERO",

		[STRUCT] = "STRUCT", [DOT] = "DOT", [ATTR] = "ATTR",

		[DREF] = "DREF",

		[LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
		[LBRACK] = "LBRACK", [RBRACK] = "RBRACK",
		[DOTS] = "DOTS", [COMA] = "COMA",

		[ARRAY] = "ARRAY", // [ARRAY_LIT] = "ARRAY_LIT",
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
		[FN_DEC] = "FN_DEC", [ARGS] = "ARGS",
		[VARIADIC] = "VARIADIC",
		[FN_CALL] = "FN_CALL", [RETURN] = "RETURN",
		[SIZEOF] = "SIZEOF", [IS_NULLABLE] = "IS_NULLABLE",

		[IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
		[THEN] = "THEN", [END] = "END",
		[BLOC] = "BLOC",
		[LOOP] = "LOOP", [BRK] = "BRK", [CNT] = "CNT",
		[AS] = "AS",
		[LAST] = "LAST",
        // clang-format on
    };

    if (types[type] == NULL)
        return format("UNKNOWN(%d)", type);

    return types[type];
}