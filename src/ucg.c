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
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

// LLVM STUFF
#include <llvm/Config/llvm-config.h>
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
	PARAM, GLOBAL,
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
    // CHAR, BOOL,
    INT, FLOAT,
    
    CHARS,
    
    REF, POINT_TO,
    ZERO,

    STRUCT, DOT, ATTR, ENUM,

    //OWN,

    LPARENT, RPARENT, DOTS, DOUBLE_DOTS, COMA,
    OPERATOR, PUB,
    LBRACK, RBRACK,
    LCBRACK, RCBRACK,
    ARRAY, ARRAY_LIT, ACCESS,

    ASSIGN,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN,
    ADD, SUB, MUL, DIV, MOD,
    BAND, BOR, BXOR, LSHIFT, RSHIFT,

    GT, LT, GE, LE, EQ, NQ,

    AND, OR, NOT, NEG,

    PROTO, 
    FN_DEC, ARGS, VARIADIC, FN_CALL, RETURN, WRITE, READ,
    LINK, USE, MACRO_IF, MACRO_ELIF, MACRO_ELSE, MACRO_ERROR,
	FN_ENTRY,
    SIZEOF, TYPEOF, DROP, COPY, IS_NULLABLE,
    AS,
	BLOC,
    IF, ELIF, ELSE, THEN,
    LOOP, BRK, CNT, MATCH, CASE, DEFAULT,
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
    bool is_set;
    bool is_uint;
    int used;

    struct { // llvm stuff
        TypeRef rtype;
        Value elem;
        Bloc bloc;
        Bloc then;
        Bloc caller_bloc;
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
        double real;
    } num;

    struct {
        Ucg *elem;
        expand(Ucg *, items);
    } arr;

    struct {
        expand(Ucg *, attrs);
        int index;
    } obj;

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
        Ucg *base;
        Ucg *to_point;
        Ucg *to_size;
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
        Ucg *to_type;
        Ucg *attr;
        Ucg *index;
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
    TargetMachine machine;

    char *dir;
    char *ll_path;
    char *asm_path;
    char *obj_path;
    char *exe_path;
    expand(char *, libs);

    char *error_msg;

    expand(Ucg *, insts);
    Ucg *read_fn;
    Ucg *write_fn;
} gb_ucg;

void ucg_start(char *name) {
    // atexit(ucg_finish);

    gb_ucg.context = LLVMContextCreate();
    gb_ucg.read_fn = NULL;
    gb_ucg.write_fn = NULL;
    zero_init(gb_ucg.libs);
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.context);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.context);

    gb_ucg.types.vd = LLVMVoidTypeInContext(gb_ucg.context);

    for (int i = 1; i < 255; i <<= 1)
        gb_ucg.types.num[i] = LLVMIntTypeInContext(gb_ucg.context, i);

    // Initialize native target.
    if (LLVMInitializeNativeTarget()) {
        eprint("%s:%d Failed to initialize native target\n", FILE, LINE);
        exit(1);
    }

    if (LLVMInitializeNativeAsmPrinter()) {
        eprint("%s:%d Failed to initialize native asm printer\n", FILE, LINE);
        exit(1);
    }

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(gb_ucg.module, triple);

    Target target;
    char *error = NULL;

    if (LLVMGetTargetFromTriple(triple, &target, &error)) {
        eprint("%s:%d GetTargetFromTriple error: %s\n", FILE, LINE, error);
        exit(1);
    }

    gb_ucg.machine = LLVMCreateTargetMachine(
        target,
        triple,
        "", // CPU
        "", // Features
        LLVMCodeGenLevelDefault,
        LLVMRelocPIC,
        LLVMCodeModelDefault
    );

    if (!gb_ucg.machine) {
        eprint("%s:%d Failed to create target machine\n", FILE, LINE);
        LLVMDisposeMessage(triple);
        exit(1);
    }

    TargetData layout = LLVMCreateTargetDataLayout(gb_ucg.machine);
    LLVMSetModuleDataLayout(gb_ucg.module, layout);

    LLVMDisposeTargetData(layout);
    LLVMDisposeMessage(triple);

    // TODO: add ASAN stuff here
    // TODO: add flags stuff (Passes)
}

void ucg_finish(void) {
    // TODO: make sure to use ucg.error_msg
    if (gb_ucg.error_msg != NULL) {
        print("there is an error %s\n", gb_ucg.error_msg);
        // LLVMDisposeMessage(gb_ucg.error_msg);
        eprint("%s: <%s>\n", RED("error"), gb_ucg.error_msg);
    }
    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMDisposeTargetMachine(gb_ucg.machine);
    LLVMContextDispose(gb_ucg.context);
    // arena_free();
}

void ucg_gen_ll(void) {
    mkdir2(dirname2(gb_ucg.ll_path));

    char *error = NULL;

    if (LLVMPrintModuleToFile(gb_ucg.module, gb_ucg.ll_path, &error)) {
        eprint("%s:%d LLVM IR error: \n%s\n", FILE, LINE, error);
        LLVMDisposeMessage(error);
    }
}

bool llvm_verify(void) {
    char *error = NULL;
    bool failed = LLVMVerifyModule(gb_ucg.module, LLVMReturnStatusAction, &error);
    if (failed)
        eprint("%s:%d LLVM verification failed:\n%s\n", FILE, LINE, error);
    LLVMDisposeMessage(error);
    return !failed;
}

void llvm_emit(char *path, LLVMCodeGenFileType type) {
    mkdir2(dirname2(path));

    char *error = NULL;

    assert(gb_ucg.machine);
    assert(gb_ucg.module);

    if (LLVMTargetMachineEmitToFile(
            gb_ucg.machine, gb_ucg.module, path, type, &error
        )) {
        eprint("LLVM codegen error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(1);
    }
}

void ucg_gen_bin(void) {
    char *obj_path = gb_ucg.obj_path;
    if (obj_path == NULL) {
        obj_path = format("%s.o", gb_ucg.exe_path);
        llvm_emit(obj_path, LLVMObjectFile);
    }

    char **argv = arena_alloc(gb_ucg.libs_count + 5, sizeof(char *));
    int argc = 0;
    argv[argc++] = "clang";
    argv[argc++] = obj_path;
    argv[argc++] = "-o";
    argv[argc++] = gb_ucg.exe_path;
    for (int i = 0; i < gb_ucg.libs_count; i++)
        argv[argc++] = gb_ucg.libs[i];

    pid_t pid = fork();
    if (pid == 0) {
        execvp("clang", argv);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        eprint("Linking failed\n");
        exit(1);
    }
    if (gb_ucg.obj_path == NULL)
        remove(obj_path);
}

void ucg_gen(void) {
    bool valid = llvm_verify();
    if (gb_ucg.ll_path)
        ucg_gen_ll();
    if (!valid)
        exit(1);
    if (gb_ucg.asm_path)
        llvm_emit(gb_ucg.asm_path, LLVMAssemblyFile);
    if (gb_ucg.obj_path)
        llvm_emit(gb_ucg.obj_path, LLVMObjectFile);
    if (gb_ucg.exe_path)
        ucg_gen_bin();
}

int ucg_run_bin(void) {
    pid_t pid = fork();

    if (pid == 0) {
        execl(gb_ucg.exe_path, gb_ucg.exe_path, NULL);
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        print("Exit code: %d\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }
    print("Terminated by signal: %d\n", WTERMSIG(status));
    return 128 + WTERMSIG(status);
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

    ir->rtype =
        LLVMFunctionType(llvm_type(ir->fn.ret), types, types_count, ir->fn.varg);

    ir->elem = LLVMGetNamedFunction(gb_ucg.module, ir->name);
    if (ir->elem == NULL || LLVMGlobalGetValueType(ir->elem) != ir->rtype)
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
        Value value = ir->fn.params[i]->elem;
        bool is_extra = i >= fn_dec->fn.params_count; // printf passed f32 as f64
        if (is_extra && llvm_is_float(value) && llvm_bits(value) == 32)
            value = LLVMBuildFPExt(gb_ucg.builder, value, llvm_float_type(64), "");
        append(params, value);
    }
    TypeRef rtype = fn_dec->rtype;
    Value fn_elem = fn_dec->elem;
    char *name = fn_dec->fn.ret->type != VOID ? ir->name : "";
    Value elem =
        LLVMBuildCall2(gb_ucg.builder, rtype, fn_elem, params, params_count, name);
    ir->elem = elem;
}

bool is_bloc_terminated() {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    return bloc != NULL && LLVMGetBasicBlockTerminator(bloc) != NULL;
}

void llvm_end_fn(Ucg *fn) {
    if (!is_bloc_terminated()) {
        if (fn->fn.ret->type == VOID)
            LLVMBuildRetVoid(gb_ucg.builder);
        else {
            // TODO: struct type
            TypeRef rtype = llvm_type(fn->fn.ret);
            LLVMBuildRet(gb_ucg.builder, LLVMConstNull(rtype));
        }
    }
    if (fn->caller_bloc)
        llvm_set_pos(fn->caller_bloc);
}

void llvm_ret(Ucg *ir) {
    if (is_bloc_terminated())
        return;

    if (ir == NULL) {
        LLVMBuildRetVoid(gb_ucg.builder);
        return;
    }
    LLVMBuildRet(gb_ucg.builder, ir->elem);
}


TypeRef llvm_float_type(int bits) {
    if (bits == 32)
        return LLVMFloatTypeInContext(gb_ucg.context);
    return LLVMDoubleTypeInContext(gb_ucg.context);
}

TypeRef llvm_type(Ucg *ir) {
    switch (ir->type) { // clang-format off
    case INT:        	return gb_ucg.types.num[1 << ir->shift];
    case FLOAT:			return llvm_float_type(1 << ir->shift);
    case STRUCT:		return ir->rtype;
    case POINT_TO:		return llvm_type(ucg_type_of(ir));
    case ATTR:			return llvm_type(ir->data_type);
    case DOT:			return llvm_type(ir->attr);
    case ACCESS:		return llvm_type(ucg_type_of(ir));
	case VOID:			return gb_ucg.types.vd;
    case VAL_CREATE:	return llvm_type(ir->to_create);
    case DECLARE:		return llvm_type(ir->data_type);
    case PARAM:			return llvm_type(ir->data_type);
    case GLOBAL:		return llvm_type(ir->data_type);
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
    case FLOAT: {
        ir->elem = LLVMConstReal(llvm_type(to_create), to_create->num.real);
        break;
    }
    case REF: {
        ir->elem = LLVMConstNull(llvm_type(to_create));
        break;
    }
    case STRUCT: {
        Value agg = LLVMConstNull(llvm_type(to_create));
        for (int i = 0; i < ir->arr.items_count; i++) {
            Ucg *item = ir->arr.items[i];
            if (item)
                agg = LLVMBuildInsertValue(gb_ucg.builder, agg, item->elem, i, "");
        }
        ir->elem = agg;
        break;
    }
    case ARRAY: {
        TypeRef rtype = llvm_type(to_create);
        Value agg = LLVMGetUndef(rtype);
        for (int i = 0; i < to_create->arr.items_count; i++) {
            agg = LLVMBuildInsertValue(
                gb_ucg.builder, agg, to_create->arr.items[i]->elem, i, ""
            );
        }

        Value stack = llvm_alloc(rtype, "arr");
        LLVMBuildStore(gb_ucg.builder, agg, stack);

        TypeRef i32 = gb_ucg.types.num[32];
        Value zero[] = {
            LLVMConstInt(i32, 0, 0),
            LLVMConstInt(i32, 0, 0),
        };
        ir->elem = LLVMBuildInBoundsGEP2(gb_ucg.builder, rtype, stack, zero, 2, "");

        break;
    }
    default: {
        eprint(
            "%s: %s:%d handle this case %s\n",
            RED("error"),
            FILE,
            LINE,
            to_string(to_create->type)
        );
        exit(1);
    }
    }
}


Value llvm_alloc(TypeRef rtype, char *name) {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    assert(bloc);
    Value basic = LLVMGetBasicBlockParent(bloc);
    assert(basic);
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
    if (!ir->is_set)
        llvm_zero_init(ir);
}

void llvm_struct_create(Ucg *ir) {
    ir->rtype = LLVMStructCreateNamed(gb_ucg.context, ir->name);
}

void llvm_struct_body(Ucg *ir) {
    int count = ir->obj.attrs_count;
    TypeRef *types = arena_alloc(count + 1, sizeof(TypeRef));
    for (int i = 0; i < count; i++)
        types[i] = llvm_type(ir->obj.attrs[i]);
    LLVMStructSetBody(ir->rtype, types, count, 0);
}

char *ucg_string_text(Ucg *value) {
    if (value->type != VAL_CREATE || value->to_create->type != ARRAY)
        return NULL;
    Ucg *arr = value->to_create;
    char *text = arena_alloc(arr->arr.items_count * 2 + 1, sizeof(char));
    int len = 0;
    for (int i = 0; i < arr->arr.items_count; i++) {
        char c = arr->arr.items[i]->to_create->num.value;
        if (c == '\0')
            break;
        if (c == '%')
            text[len++] = '%';
        text[len++] = c;
    }
    return text;
}

void llvm_ref(Ucg *ir) {
    ir->elem = ir->to_ref->elem;
}

void llvm_point_to(Ucg *ir) {
    Ucg *ref = ir->to_point;
    ir->elem = LLVMBuildLoad2(gb_ucg.builder, llvm_type(ref), ref->elem, ir->name);
}

void llvm_get_attr(Ucg *ir) {
    Ucg *base = ir->base;
    TypeRef rtype = llvm_type(base);
    int index = ir->attr->obj.index;
    ir->elem = LLVMBuildStructGEP2(gb_ucg.builder, rtype, base->elem, index, ir->name);
}

void llvm_access(Ucg *ir) {
    TypeRef rtype = llvm_type(ucg_type_of(ir));
    Value index = ir->index->elem;
    ir->elem = LLVMBuildInBoundsGEP2(
        gb_ucg.builder, rtype, ir->base->elem, &index, 1, ir->name
    );
}

void llvm_sizeof(Ucg *ir) {
    Value size = LLVMSizeOf(llvm_type(ir->to_size));
    ir->elem =
        LLVMBuildIntCast2(gb_ucg.builder, size, gb_ucg.types.num[32], 0, "SIZEOF");
}

void llvm_typeof(Ucg *ir) {
    char *name = ucg_type_name(ir->to_type);
    ir->elem = LLVMBuildGlobalStringPtr(gb_ucg.builder, name, "TYPEOF");
}

void llvm_declare_global(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    assert(ir->name);
    ir->elem = LLVMAddGlobal(gb_ucg.module, ir->rtype, ir->name);
    LLVMSetInitializer(ir->elem, LLVMConstNull(ir->rtype));
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
        [BAND] = LLVMAnd, [BOR] = LLVMOr,   [BXOR] = LLVMXor,
        [LSHIFT] = LLVMShl, [RSHIFT] = LLVMAShr,
    };
    LLVMIntPredicate predicates[LAST + 1] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
        [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
    };
    LLVMOpcode float_opcodes[LAST + 1] = {
        [ADD] = LLVMFAdd, [SUB] = LLVMFSub, [MUL] = LLVMFMul,
        [DIV] = LLVMFDiv, [MOD] = LLVMFRem,
    };
    LLVMRealPredicate float_predicates[LAST + 1] = {
        [GT] = LLVMRealOGT, [LT] = LLVMRealOLT, [GE] = LLVMRealOGE,
        [LE] = LLVMRealOLE, [EQ] = LLVMRealOEQ, [NQ] = LLVMRealUNE,
    };
    // clang-format on
    if (ir->is_uint) {
        opcodes[DIV] = LLVMUDiv;
        opcodes[MOD] = LLVMURem;
        opcodes[RSHIFT] = LLVMLShr;
        predicates[GT] = LLVMIntUGT;
        predicates[LT] = LLVMIntULT;
        predicates[GE] = LLVMIntUGE;
        predicates[LE] = LLVMIntULE;
    }
    Type type = ir->type;
    Value left = ir->left->elem;
    Value right = ir->right->elem;
    bool is_float = llvm_is_float(right);

    const char *name = to_string(ir->type); // node->token->llvm.name;
    Value value = NULL;
    if (is_float && float_opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, float_opcodes[type], left, right, name);
    else if (is_float && float_predicates[type])
        value =
            LLVMBuildFCmp(gb_ucg.builder, float_predicates[type], left, right, name);
    else if (!is_float && opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, opcodes[type], left, right, name);
    else if (!is_float && predicates[type])
        value = LLVMBuildICmp(gb_ucg.builder, predicates[type], left, right, name);
    else if (ir->type == ASSIGN) {
        value = LLVMBuildStore(gb_ucg.builder, right, left);
    } else {
        eprint("unknown operation %s\n", to_string(type));
        exit(1);
    }
    ir->elem = value;
}

bool llvm_is_float(Value value) {
    TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(value));
    return kind == LLVMFloatTypeKind || kind == LLVMDoubleTypeKind;
}

int llvm_bits(Value value) {
    TypeRef type = LLVMTypeOf(value);
    switch (LLVMGetTypeKind(type)) {
    case LLVMFloatTypeKind:
        return 32;
    case LLVMDoubleTypeKind:
        return 64;
    default:
        return LLVMGetIntTypeWidth(type);
    }
}

// TODO: review this one
void llvm_cast(Ucg *ir) {
    Value value = ir->left->elem;
    Ucg *to = ir->to_type;
    if (to->type == REF) {
        ir->elem = LLVMBuildPointerCast(gb_ucg.builder, value, llvm_type(to), "AS");
        return;
    }
    bool from_float = llvm_is_float(value);
    bool to_float = to->type == FLOAT;
    int from_bits = llvm_bits(value);
    int to_bits = 1 << to->shift;

    if (from_float == to_float && from_bits == to_bits) {
        ir->elem = value;
        return;
    }
    bool from_uint = from_bits == 1 || ir->is_uint;
    LLVMOpcode op;
    if (from_float && to_float)
        op = to_bits > from_bits ? LLVMFPExt : LLVMFPTrunc;
    else if (from_float)
        op = to->is_uint ? LLVMFPToUI : LLVMFPToSI;
    else if (to_float)
        op = from_uint ? LLVMUIToFP : LLVMSIToFP;
    else if (to_bits > from_bits)
        op = from_uint ? LLVMZExt : LLVMSExt;
    else
        op = LLVMTrunc;
    ir->elem = LLVMBuildCast(gb_ucg.builder, op, value, llvm_type(to), "AS");
}

void llvm_neg(Ucg *ir) {
    Value value = ir->right->elem;
    if (llvm_is_float(value))
        ir->elem = LLVMBuildFNeg(gb_ucg.builder, value, "NEG");
    else
        ir->elem = LLVMBuildNeg(gb_ucg.builder, value, "NEG");
}

void llvm_not(Ucg *ir) {
    ir->elem = LLVMBuildNot(gb_ucg.builder, ir->right->elem, "NOT");
}

Value llvm_parent_bloc() {
    assert(gb_ucg.builder);
    assert(LLVMGetInsertBlock(gb_ucg.builder));
    return LLVMGetBasicBlockParent(LLVMGetInsertBlock(gb_ucg.builder));
}

Bloc llvm_bloc(Value fn, char *name) {
    return LLVMAppendBasicBlockInContext(gb_ucg.context, fn, name);
}

Bloc llvm_get_pos(void) {
    return LLVMGetInsertBlock(gb_ucg.builder);
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
    memset(gb_ucg.insts, 0, gb_ucg.insts_count * sizeof(Ucg *));
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

Ucg *ucg_uint_type(int shift) {
    Ucg *new = ucg_int_type(shift);
    new->is_uint = true;
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

Ucg *ucg_bool_type() {
    return ucg_i1_type();
}

Ucg *ucg_void_type() {
    return ucg_new(VOID);
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

Ucg *ucg_float_type(int shift) {
    Ucg *new = ucg_new(FLOAT);
    new->shift = log2(shift);
    return new;
}

Ucg *ucg_float_value(int shift, double value) {
    Ucg *new = ucg_new(VAL_CREATE);

    new->to_create = ucg_float_type(shift);
    new->to_create->num.real = value;

    new->to_create->is_val = true;
    append(gb_ucg.insts, new->to_create);

    append(gb_ucg.insts, new);
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

Ucg *ucg_string_type(void) {
    return ucg_ref_type(ucg_char_type());
}

Ucg *ucg_ref(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    const char *name = to_ref->name ? to_ref->name : to_string(to_ref->type);
    new->name = format("&%s", name);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_point_to(Ucg *ref) {
    Ucg *new = ucg_new(POINT_TO);
    new->to_point = ref;
    const char *name = ref->name ? ref->name : to_string(ref->type);
    new->name = format("*%s", name);
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

Ucg *ucg_proto(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_new(FN_DEC);
    new->name = name;
    new->fn.ret = ret;

    for (int i = 0; i < params_count; i++) {
        Ucg *param = params[i];
        if (param->type == VARIADIC)
            new->fn.varg = true;
        else
            append(new->fn.params, param);
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_entry(Ucg *fn) {
    Ucg *new = ucg_new(FN_ENTRY);
    new->to_enter = fn;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_fn(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_proto(name, ret, params, params_count);
    ucg_entry(new);
    return new;
}

Ucg *ucg_fn_call(Ucg *fn_dec, Ucg **args, int args_count) {
    Ucg *new = ucg_new(FN_CALL);
    new->name = format("call.%s", fn_dec->name);
    new->to_call = fn_dec;
    fn_dec->used++;
    for (int i = 0; i < args_count; i++)
        append(new->fn.params, ucg_value(args[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_varg_type(void) {
    return ucg_new(VARIADIC);
}

Ucg *ucg_get_param(Ucg *fn, int index) {
    return fn->fn.params[index];
}

Ucg *ucg_add_param(Ucg *fn, Ucg *param) {
    append(fn->fn.params, param);
    return param;
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

Ucg *ucg_global(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(GLOBAL);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_null(Ucg *ref_type) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ref_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_struct(char *name) {
    Ucg *new = ucg_new(STRUCT);
    new->name = name;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_attr(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(ATTR);
    new->name = name;
    new->data_type = data_type;
    return new;
}

void ucg_attrs(Ucg *st, Ucg **attrs, int attrs_count) {
    for (int i = 0; i < attrs_count; i++) {
        attrs[i]->obj.index = st->obj.attrs_count;
        append(st->obj.attrs, attrs[i]);
    }
}

Ucg *ucg_type_of(Ucg *place) {
    if (place->type == DOT)
        return place->attr->data_type;
    if (place->type == POINT_TO)
        return ucg_type_of(place->to_point)->to_ref;
    if (place->type == ACCESS)
        return ucg_value_type(place->base)->to_ref;
    return place->data_type;
}

Ucg *ucg_value_type(Ucg *value) {
    switch (value->type) { // clang-format off
    case DREF:       return ucg_type_of(value->to_dref);
    case VAL_CREATE: return value->to_create;
    case FN_CALL:    return value->to_call->fn.ret;
    case AS:         return value->to_type;
    case SIZEOF:     return ucg_i32_type();
    case TYPEOF:     return ucg_string_type();
    case NEG:        return ucg_value_type(value->right);
    case LT: case GT: case EQ: case LE: case GE: case NQ: case NOT:
                     return ucg_i1_type();
    default:         return ucg_value_type(value->left); // clang-format on
    }
}

Ucg *ucg_write(Ucg *fd, Ucg **values, int values_count) {
    if (gb_ucg.write_fn == NULL)
        gb_ucg.write_fn = ucg_proto(
            "dprintf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fd", ucg_i32_type()),
                ucg_param("format", ucg_string_type()),
                ucg_varg_type(),
            },
            3
        );
    Ucg **args = arena_alloc(values_count + 3, sizeof(Ucg *));
    int args_count = 2;
    char *layout = "";
    for (int i = 0; i < values_count; i++) {
        Ucg *value = ucg_value(values[i]);
        char *text = ucg_string_text(value);
        if (text) {
            layout = format("%s%s", layout, text);
            continue;
        }
        Ucg *type = ucg_value_type(value);
        int bits = 1 << type->shift;
        char *spec = "%s";
        if (type->type == FLOAT) {
            spec = "%f";
            value = ucg_cast(value, ucg_float_type(64));
        } else if (type->type == INT && bits == 1) {
            Ucg *text_var = ucg_var("bool.text", ucg_string_type());
            Ucg *stmt = ucg_if();
            ucg_then(stmt, value);
            ucg_assign(text_var, ucg_string_value("True"));
            ucg_then(ucg_else(stmt), NULL);
            ucg_assign(text_var, ucg_string_value("False"));
            ucg_end(stmt);
            value = text_var;
        } else if (type->type == INT && type->is_uint && bits == 64) {
            spec = "%llu";
        } else if (type->type == INT && type->is_uint) {
            spec = "%u";
            value = ucg_cast(value, ucg_uint_type(32));
        } else if (type->type == INT && bits == 8) {
            spec = "%c";
            value = ucg_cast(value, ucg_i32_type());
        } else if (type->type == INT && bits == 64) {
            spec = "%lld";
        } else if (type->type == INT) {
            spec = "%d";
            value = ucg_cast(value, ucg_i32_type());
        }
        layout = format("%s%s", layout, spec);
        args[args_count++] = value;
    }
    args[0] = fd;
    args[1] = ucg_string_value(layout);
    return ucg_fn_call(gb_ucg.write_fn, args, args_count);
}

Ucg *ucg_read(Ucg *fd) {
    if (gb_ucg.read_fn == NULL) {
        Ucg *str = ucg_string_type();
        Ucg *i32 = ucg_i32_type();
        Ucg *i64 = ucg_int_type(64);
        Ucg *getline = ucg_proto(
            "getline",
            i64,
            (Ucg *[]){
                ucg_param("line", ucg_ref_type(str)),
                ucg_param("cap", ucg_ref_type(i64)),
                ucg_param("file", str),
            },
            3
        );
        Ucg *find = ucg_proto(
            "strchr",
            str,
            (Ucg *[]){
                ucg_param("s", str),
                ucg_param("c", i32),
            },
            2
        );
        Ucg *copy = ucg_proto(
            "strdup",
            str,
            (Ucg *[]){
                ucg_param("s", str),
            },
            1
        );
        Ucg *open = ucg_proto(
            "fdopen",
            str,
            (Ucg *[]){
                ucg_param("fd", i32),
                ucg_param("mode", str),
            },
            2
        );
        Ucg *file = ucg_global("ura.read_file", str);
        Ucg *file_fd = ucg_global("ura.read_fd", i32);

        Ucg *fn = ucg_fn(
            "ura.read",
            str,
            (Ucg *[]){
                ucg_param("fd", i32),
            },
            1
        );
        Ucg *fd_param = ucg_get_param(fn, 0);
        Ucg *reopen = ucg_if();
        {
            ucg_then(
                reopen,
                ucg_op(
                    OR, ucg_op(EQ, file, ucg_null(str)), ucg_op(NQ, fd_param, file_fd)
                )
            );
            ucg_assign(
                file,
                ucg_fn_call(
                    open,
                    (Ucg *[]){
                        fd_param,
                        ucg_string_value("r"),
                    },
                    2
                )
            );
            ucg_assign(file_fd, fd_param);
        }
        ucg_end(reopen);

        Ucg *line = ucg_var("line", str);
        Ucg *cap = ucg_var("cap", i64);
        Ucg *len = ucg_var("len", i64);
        ucg_assign(
            len,
            ucg_fn_call(
                getline,
                (Ucg *[]){
                    ucg_ref(line),
                    ucg_ref(cap),
                    file,
                },
                3
            )
        );

        Ucg *eof = ucg_if();
        {
            ucg_then(eof, ucg_op(LT, len, ucg_int_value(64, 1)));
            ucg_ret(ucg_fn_call(
                copy,
                (Ucg *[]){
                    ucg_string_value(""),
                },
                1
            ));
        }
        ucg_end(eof);

        Ucg *newline = ucg_var("newline", str);
        ucg_assign(
            newline,
            ucg_fn_call(
                find,
                (Ucg *[]){
                    line,
                    ucg_i32_value('\n'),
                },
                2
            )
        );
        Ucg *found = ucg_if();
        {
            ucg_then(found, ucg_op(NQ, newline, ucg_null(str)));
            ucg_assign(ucg_point_to(newline), ucg_char_value(0));
        }
        ucg_end(found);
        ucg_ret(line);
        ucg_end(fn);
        gb_ucg.read_fn = fn;
    }
    return ucg_fn_call(
        gb_ucg.read_fn,
        (Ucg *[]){
            fd,
        },
        1
    );
}

Ucg *ucg_struct_value(Ucg *st, Ucg **values, int values_count) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = st;
    for (int i = 0; i < st->obj.attrs_count; i++) {
        bool has_value = i < values_count && values[i];
        append(new->arr.items, has_value ? ucg_value(values[i]) : NULL);
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_get_attr(Ucg *base, int index) {
    Ucg *attr = ucg_type_of(base)->obj.attrs[index];
    Ucg *new = ucg_new(DOT);
    new->base = base;
    new->attr = attr;
    const char *name = base->name ? base->name : to_string(base->type);
    new->name = format("%s.%s", name, attr->name);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_access(Ucg *base, Ucg *index) {
    Ucg *new = ucg_new(ACCESS);
    new->base = ucg_value(base);
    new->index = ucg_value(index);
    const char *name = base->name ? base->name : to_string(base->type);
    new->name = format("%s[]", name);
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
    bool is_variable = ir->type == DECLARE || ir->type == PARAM || ir->type == GLOBAL;
    bool is_part_of_variable =
        ir->type == DOT || ir->type == POINT_TO || ir->type == ACCESS;
    return is_variable || is_part_of_variable;
}

Ucg *ucg_value(Ucg *ir) {
    assert(ir);
    return is_locator(ir) ? ucg_dref(ir) : ir;
}

bool is_first_write(Ucg *var) {
    if (var->type != DECLARE)
        return false;
    for (int i = gb_ucg.insts_count - 1; i >= 0 && gb_ucg.insts[i] != var; i--) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) { // clang-format off
        case IF: case ELIF: case ELSE: case THEN: case LOOP: case END:
        case BRK: case CNT: case RETURN: case FN_ENTRY:
            return false;
        case DREF: if (curr->to_dref == var) return false; break;
        case DOT:  if (curr->base == var) return false; break;
        case REF:  if (curr->to_ref == var) return false; break;
        case POINT_TO: if (curr->to_point == var) return false; break;
        default:   break; // clang-format on
        }
    }
    return true;
}

bool ucg_is_uint(Ucg *value) {
    Ucg *type = is_locator(value) ? ucg_type_of(value) : ucg_value_type(value);
    return type->type == INT && type->is_uint;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    assert(left);
    assert(right);
    bool is_uint = type != ASSIGN && (ucg_is_uint(left) || ucg_is_uint(right));
    Type ops[LAST + 1] = {
        [ADD_ASSIGN] = ADD,       [SUB_ASSIGN] = SUB,   [MUL_ASSIGN] = MUL,
        [DIV_ASSIGN] = DIV,       [MOD_ASSIGN] = MOD,   [BAND_ASSIGN] = BAND,
        [BOR_ASSIGN] = BOR,       [BXOR_ASSIGN] = BXOR, [LSHIFT_ASSIGN] = LSHIFT,
        [RSHIFT_ASSIGN] = RSHIFT,
    };

    if (type != ASSIGN && !ops[type])
        left = ucg_value(left);
    right = ucg_value(right);

    if (ops[type]) {
        type = ops[type];
        Ucg *binary_op = ucg_new(type);
        binary_op->left = ucg_dref(left);
        binary_op->right = right;
        binary_op->is_uint = is_uint;
        right = binary_op;
        append(gb_ucg.insts, binary_op);
        type = ASSIGN;
    }
    if (type == ASSIGN && is_first_write(left))
        left->is_set = true;
    Ucg *new = ucg_new(type);
    new->left = left;
    new->right = right;
    new->is_uint = is_uint;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_assign(Ucg *left, Ucg *right) {
    return ucg_op(ASSIGN, left, right);
}

Ucg *ucg_cast(Ucg *value, Ucg *type) {
    Ucg *new = ucg_new(AS);
    new->left = ucg_value(value);
    new->to_type = type;
    new->is_uint = ucg_is_uint(new->left);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_sizeof(Ucg *type) {
    Ucg *new = ucg_new(SIZEOF);
    new->to_size = type;
    append(gb_ucg.insts, new);
    return new;
}

char *ucg_type_name(Ucg *type) {
    switch (type->type) {
    case INT: {
        int bits = 1 << type->shift;
        if (bits == 1)
            return "bool";
        return format("%c%d", type->is_uint ? 'u' : 'i', bits);
    }
    case FLOAT:
        return format("f%d", 1 << type->shift);
    case REF:
        return format("%s[]", ucg_type_name(type->to_ref));
    case VOID:
        return "void";
    default:
        return type->name;
    }
}

Ucg *ucg_typeof(Ucg *type) {
    Ucg *new = ucg_new(TYPEOF);
    new->to_type = type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_neg(Ucg *value) {
    Ucg *new = ucg_new(NEG);
    new->right = ucg_value(value);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_not(Ucg *value) {
    Ucg *new = ucg_new(NOT);
    new->right = ucg_value(value);
    append(gb_ucg.insts, new);
    return new;
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

Ucg *ucg_brk(Ucg *loop) {
    Ucg *new = ucg_new(BRK);
    new->stmt = loop;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_cnt(Ucg *loop) {
    Ucg *new = ucg_new(CNT);
    new->stmt = loop;
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
        case FLOAT: {
            text = format("%s(%f)", text, inst->num.real);
            break;
        }
        default: {
            eprint(
                "%s: %s:%d handle this case %s\n",
                RED("error"),
                FILE,
                LINE,
                to_string(inst->type)
            );
            exit(1);
        }
        }
    }

    text = format("[%03d] %-16s", inst->inst_index, text);
    if (inst->left)
        text = format("%s [%03d]", text, inst->left->inst_index);
    if (inst->right)
        text = format("%s [%03d]", text, inst->right->inst_index);
    print("%s%c", text, end);
}

void ucg_optimize(void) {
    bool removed = true;
    while (removed) {
        removed = false;
        for (int i = 0; i < gb_ucg.insts_count; i++) {
            Ucg *fn = gb_ucg.insts[i];
            bool is_unused = fn->type == FN_DEC && fn->used == 0;
            if (!is_unused || strcmp2(fn->name, "main"))
                continue;
            fn->used = -1;
            removed = true;
            bool is_inside = false;
            for (int j = i + 1; j < gb_ucg.insts_count; j++) {
                Ucg *inst = gb_ucg.insts[j];
                if (inst->type == FN_ENTRY && inst->to_enter == fn)
                    is_inside = true;
                if (!is_inside)
                    continue;
                if (inst->type == END && inst->to_end == fn)
                    break;
                if (inst->type == FN_CALL)
                    inst->to_call->used--;
            }
        }
    }
    int count = 0;
    Ucg *removed_fn = NULL;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        bool is_removed = inst->type == FN_DEC && inst->used < 0;
        if (inst->type == FN_ENTRY && inst->to_enter->used < 0 && !removed_fn)
            removed_fn = inst->to_enter;
        bool is_body_end =
            removed_fn && inst->type == END && inst->to_end == removed_fn;
        if (!removed_fn && !is_removed)
            gb_ucg.insts[count++] = inst;
        if (is_body_end)
            removed_fn = NULL;
    }
    gb_ucg.insts_count = count;
}

void ucg_compile() {
    ucg_optimize();
    Value fn = NULL;
    int stmt_index = 0;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        if (curr->type == STRUCT)
            llvm_struct_create(curr);
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        if (curr->type == STRUCT)
            llvm_struct_body(curr);
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) {
        case FN_DEC: {
            llvm_fn_dec(curr);
            break;
        }
        case GLOBAL: {
            llvm_declare_global(curr);
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
            if (to_end->type == FN_DEC)
                break;
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
        case FLOAT:
            break;
        // case ARRAY_LIT: {
        // 	// llvm_array_value(curr);
        // 	break;
        // }
        case FN_DEC: {
            // llvm_fn_dec(curr);
            break;
        }
        case GLOBAL: {
            break;
        }
        case FN_ENTRY: {
            curr->to_enter->caller_bloc = llvm_get_pos();
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
		case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: // bitwise
		case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite
		case DIV_ASSIGN: case MOD_ASSIGN:
		case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
		case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
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
            if (stmt->type == FN_DEC) {
                llvm_end_fn(stmt);
                break;
            }
            llvm_jmp(stmt->type == LOOP ? stmt->bloc : curr->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        case BRK: {
            llvm_jmp(curr->stmt->end->bloc);
            break;
        }
        case NEG: {
            llvm_neg(curr);
            break;
        }
        case NOT: {
            llvm_not(curr);
            break;
        }
        case AS: {
            llvm_cast(curr);
            break;
        }
        case STRUCT: {
            break;
        }
        case DOT: {
            llvm_get_attr(curr);
            break;
        }
        case ACCESS: {
            llvm_access(curr);
            break;
        }
        case SIZEOF: {
            llvm_sizeof(curr);
            break;
        }
        case TYPEOF: {
            llvm_typeof(curr);
            break;
        }
        case REF: {
            llvm_ref(curr);
            break;
        }
        case POINT_TO: {
            llvm_point_to(curr);
            break;
        }
        case CNT: {
            llvm_jmp(curr->stmt->bloc);
            break;
        }
        default: {
            eprint(
                "%s: %s:%d handle this case %s\n",
                RED("error"),
                FILE,
                LINE,
                to_string(curr->type)
            );
            // exit(1);
            break;
        }
        }
    }
}

bool run_test(int p) {
    ucg_clear();
    switch (p) {
    case 0: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            Ucg *b = ucg_var("b", ucg_i32_type());

            ucg_assign(a, ucg_i32_value(10));

            ucg_assign(b, ucg_op(MUL, a, ucg_i32_value(2)));
            ucg_assign(a, ucg_op(ADD, a, b));

            ucg_op(ADD_ASSIGN, a, ucg_i32_value(5));
            ucg_op(SUB_ASSIGN, a, ucg_i32_value(3));
            ucg_op(MUL_ASSIGN, a, ucg_i32_value(2));
            ucg_op(DIV_ASSIGN, a, ucg_i32_value(4));
            ucg_op(MOD_ASSIGN, a, ucg_i32_value(5));

            ucg_ret(ucg_op(ADD, a, b));
        }
        ucg_end(fn_main);
        break;
    }
    case 1: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *x = ucg_var("a", ucg_i32_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *c = ucg_var("c", ucg_char_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    OR,
                    ucg_op(EQ, b, ucg_bool_value(1)),
                    ucg_op(NQ, c, ucg_char_value(0))
                )
            );
            {
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(x);
        }
        ucg_end(fn_main);
        break;
    }
    case 2: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 3: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            Ucg *big = ucg_var("big", ucg_bool_type());

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(3)));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(9)));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_bool_value(0));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(100));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(0)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 4: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 5: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *elif_stmt = ucg_elif(if_stmt);
            ucg_then(elif_stmt, ucg_op(EQ, flag, ucg_bool_value(0)));
            {
                ucg_ret(ucg_i32_value(2));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(3));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 6: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n1", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *n0 = ucg_var("n0", ucg_i32_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        is_even,
                        (Ucg *[]){
                            ucg_i32_value(4),
                        },
                        1
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(6) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(7) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n0);
        }
        ucg_end(fn_main);
        break;
    }
    case 7: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_bool_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *n = ucg_var("n", ucg_i32_type());

            ucg_assign(a, ucg_bool_value(0));
            ucg_assign(a, ucg_bool_value(1));
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(OR, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(5));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(AND, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(50));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 8: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            ucg_assign(a, ucg_i32_value(0));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(5)));
            {
                ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
            }
            ucg_end(loop);

            ucg_ret(a);
        }
        ucg_end(fn_main);
        break;
    }
    case 9: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *i = ucg_var("i", ucg_i32_type());
            ucg_assign(i, ucg_i32_value(0));

            Ucg *going = ucg_var("going", ucg_bool_type());
            ucg_assign(going, ucg_bool_value(1));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(EQ, going, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
                ucg_assign(going, ucg_op(LT, i, ucg_i32_value(4)));
            }
            ucg_end(loop);

            ucg_ret(i);
        }
        ucg_end(fn_main);
        break;
    }
    case 10: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_char_type());
            Ucg *b = ucg_var("b", ucg_char_type());

            ucg_assign(a, ucg_char_value('a'));
            ucg_assign(b, ucg_char_value('b'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("<%c%c>\n"),
                    a,
                    b,
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 11: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('x'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('x')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(NQ, c, ucg_char_value('y')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 12: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('m'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(GT, c, ucg_char_value('a')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(LT, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    AND,
                    ucg_op(GE, c, ucg_char_value('m')),
                    ucg_op(LE, c, ucg_char_value('m'))
                )
            );
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 13: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 14: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 15: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *answer = ucg_fn(
            "answer",
            ucg_char_type(),
            (Ucg *[]){
                ucg_param("yes", ucg_bool_type()),
            },
            1
        );
        {
            Ucg *yes = ucg_get_param(answer, 0);
            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, yes, ucg_bool_value(1)));
            {
                ucg_ret(ucg_char_value('Y'));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_char_value('N'));
        }
        ucg_end(answer);

        Ucg *show = ucg_fn(
            "show",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("c", ucg_char_type()),
            },
            1
        );
        {
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("%c\n"),
                    ucg_get_param(show, 0),
                },
                2
            );
        }
        ucg_end(show);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(1) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(0) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
        }
        ucg_end(fn_main);
        break;
    }
    case 16: {
        Ucg *fact = ucg_fn(
            "fact",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(LE, ucg_get_param(fact, 0), ucg_i32_value(1)));
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(stmt);
            ucg_ret(ucg_op(
                MUL,
                ucg_get_param(fact, 0),
                ucg_fn_call(
                    fact,
                    (Ucg *[]){
                        ucg_op(SUB, ucg_get_param(fact, 0), ucg_i32_value(1)),
                    },
                    1
                )
            ));
        }
        ucg_end(fact);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);

        Ucg *call = ucg_fn_call(
            fact,
            (Ucg *[]){
                ucg_i32_value(5),
            },
            1
        );
        ucg_ret(call);
        ucg_end(fn_main);
        break;
    }
    case 17: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(10)));
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(EQ, a, ucg_i32_value(3)));
                ucg_brk(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(a);
        ucg_end(fn_main);
        break;
    }
    case 18: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        Ucg *sum = ucg_var("sum", ucg_i32_type());
        ucg_assign(i, ucg_i32_value(0));
        ucg_assign(sum, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(10)));
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
            Ucg *stmt = ucg_if();
            {
                ucg_then(
                    stmt,
                    ucg_op(EQ, ucg_op(MOD, i, ucg_i32_value(2)), ucg_i32_value(0))
                );
                ucg_cnt(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, sum, i);
        }
        ucg_end(loop);

        ucg_ret(sum);
        ucg_end(fn_main);
        break;
    }
    case 19: {
        Ucg *count = ucg_global("count", ucg_i32_type());

        Ucg *tick = ucg_fn("tick", ucg_void_type(), NULL, 0);
        ucg_op(ADD_ASSIGN, count, ucg_i32_value(1));
        ucg_end(tick);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_assign(count, ucg_i32_value(0));
        ucg_assign(i, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(4)));
            ucg_fn_call(tick, NULL, 0);
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(count);
        ucg_end(fn_main);
        break;
    }
    case 20: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(5));
        ucg_ret(ucg_op(ADD, ucg_neg(a), ucg_i32_value(12)));
        ucg_end(fn_main);
        break;
    }
    case 21: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(3));

        Ucg *help = ucg_fn("help", ucg_i32_type(), NULL, 0);
        ucg_ret(ucg_i32_value(4));
        ucg_end(help);

        ucg_ret(ucg_op(ADD, a, ucg_fn_call(help, NULL, 0)));
        ucg_end(fn_main);
        break;
    }
    case 22: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_float_type(32));
        Ucg *b = ucg_var("b", ucg_float_type(64));
        ucg_assign(a, ucg_float_value(32, 1.5));
        ucg_assign(b, ucg_float_value(64, 2.25));
        ucg_assign(b, ucg_op(MUL, b, ucg_cast(a, ucg_float_type(64))));
        ucg_fn_call(
            printf,
            (Ucg *[]){
                ucg_string_value("%f %f\n"),
                a,
                b,
            },
            3
        );

        Ucg *stmt = ucg_if();
        {
            ucg_then(stmt, ucg_op(GT, b, ucg_float_value(64, 3.0)));
            ucg_ret(ucg_cast(ucg_neg(b), ucg_i32_type()));
        }
        ucg_end(stmt);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 23: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *rect = ucg_struct("Rect");
        ucg_attrs(
            rect,
            (Ucg *[]){
                ucg_attr("a", point),
                ucg_attr("b", point),
            },
            2
        );

        Ucg *make = ucg_fn(
            "make",
            point,
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        Ucg *p = ucg_var("p", point);
        ucg_assign(ucg_get_attr(p, 0), ucg_get_param(make, 0));
        ucg_ret(p);
        ucg_end(make);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *r = ucg_var("r", rect);
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 0), 0), ucg_i32_value(1));
        ucg_assign(
            ucg_get_attr(r, 1),
            ucg_fn_call(
                make,
                (Ucg *[]){
                    ucg_i32_value(5),
                },
                1
            )
        );
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 1), 1), ucg_i32_value(2));
        Ucg *diff = ucg_op(
            SUB,
            ucg_get_attr(ucg_get_attr(r, 1), 0),
            ucg_get_attr(ucg_get_attr(r, 0), 0)
        );
        ucg_ret(ucg_op(ADD, diff, ucg_get_attr(ucg_get_attr(r, 1), 1)));
        ucg_end(fn_main);
        break;
    }
    case 24: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        Ucg *n = ucg_var("n", ucg_i32_type());
        Ucg *sum = ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1));
        ucg_ret(ucg_op(ADD, sum, n));
        ucg_end(fn_main);
        break;
    }
    case 25: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        ucg_assign(
            p,
            ucg_struct_value(
                point,
                (Ucg *[]){
                    ucg_i32_value(10),
                },
                1
            )
        );
        ucg_ret(ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1)));
        ucg_end(fn_main);
        break;
    }
    case 26: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *swap = ucg_fn(
            "swap",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("x", ucg_ref_type(ucg_i32_type())),
                ucg_param("y", ucg_ref_type(ucg_i32_type())),
            },
            2
        );
        Ucg *x = ucg_get_param(swap, 0);
        Ucg *y = ucg_get_param(swap, 1);
        Ucg *t = ucg_var("t", ucg_i32_type());
        ucg_assign(t, ucg_point_to(x));
        ucg_assign(ucg_point_to(x), ucg_point_to(y));
        ucg_assign(ucg_point_to(y), t);
        ucg_end(swap);

        Ucg *move = ucg_fn(
            "move",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("p", ucg_ref_type(point)),
            },
            1
        );
        ucg_op(
            ADD_ASSIGN,
            ucg_get_attr(ucg_point_to(ucg_get_param(move, 0)), 0),
            ucg_i32_value(5)
        );
        ucg_end(move);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        Ucg *b = ucg_var("b", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(1));
        ucg_assign(b, ucg_i32_value(2));
        ucg_fn_call(
            swap,
            (Ucg *[]){
                ucg_ref(a),
                ucg_ref(b),
            },
            2
        );

        Ucg *p = ucg_var("p", point);
        ucg_fn_call(
            move,
            (Ucg *[]){
                ucg_ref(p),
            },
            1
        );

        Ucg *r = ucg_var("r", ucg_ref_type(ucg_i32_type()));
        ucg_assign(r, ucg_ref(a));
        ucg_assign(ucg_point_to(r), ucg_i32_value(7));

        Ucg *sum = ucg_op(ADD, ucg_op(MUL, a, ucg_i32_value(10)), b);
        ucg_ret(ucg_op(ADD, sum, ucg_get_attr(p, 0)));
        ucg_end(fn_main);
        break;
    }
    case 27: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *hp = ucg_var("hp", ucg_i32_type());
        Ucg *speed = ucg_var("speed", ucg_float_type(32));
        Ucg *alive = ucg_var("alive", ucg_i1_type());
        Ucg *grade = ucg_var("grade", ucg_char_type());
        ucg_assign(hp, ucg_i32_value(100));
        ucg_assign(speed, ucg_float_value(32, 1.5));
        ucg_assign(alive, ucg_i1_value(1));
        ucg_assign(grade, ucg_char_value('S'));
        Ucg *values[] = {
            ucg_string_value("hp: "),     hp,    ucg_string_value(" speed: "), speed,
            ucg_string_value(" alive: "), alive, ucg_string_value(" grade: "), grade,
            ucg_string_value(" 100%\n"),
        };
        ucg_write(ucg_i32_value(1), values, 9);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 28: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *first = ucg_var("first", ucg_string_type());
        Ucg *second = ucg_var("second", ucg_string_type());
        ucg_assign(first, ucg_read(ucg_i32_value(0)));
        ucg_assign(second, ucg_read(ucg_i32_value(0)));
        Ucg *values[] = {
            ucg_string_value("["),   first, ucg_string_value("] ["), second,
            ucg_string_value("]\n"),
        };
        ucg_write(ucg_i32_value(1), values, 5);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 29: {
        Ucg *i32 = ucg_i32_type();
        Ucg *malloc = ucg_proto(
            "malloc",
            ucg_string_type(),
            (Ucg *[]){
                ucg_param("size", i32),
            },
            1
        );
        Ucg *fn_main = ucg_fn("main", i32, NULL, 0);
        Ucg *arr = ucg_var("arr", ucg_ref_type(i32));
        Ucg *size = ucg_op(MUL, ucg_sizeof(i32), ucg_i32_value(4));
        ucg_assign(
            arr,
            ucg_cast(
                ucg_fn_call(
                    malloc,
                    (Ucg *[]){
                        size,
                    },
                    1
                ),
                ucg_ref_type(i32)
            )
        );
        ucg_assign(ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(7));
        ucg_assign(
            ucg_access(arr, ucg_i32_value(0)),
            ucg_op(ADD, ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(1))
        );
        Ucg *values[] = {
            ucg_typeof(ucg_ref_type(i32)),
            ucg_string_value(" "),
            ucg_access(arr, ucg_i32_value(0)),
            ucg_string_value(" "),
            ucg_sizeof(i32),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 6);
        ucg_ret(ucg_access(arr, ucg_i32_value(0)));
        ucg_end(fn_main);
        break;
    }
    case 30: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *small = ucg_var("small", ucg_uint_type(8));
        ucg_assign(small, ucg_int_value(8, 250));
        ucg_op(ADD_ASSIGN, small, ucg_int_value(8, 10));
        Ucg *big = ucg_var("big", ucg_uint_type(32));
        ucg_assign(big, ucg_int_value(32, 4000000000));
        Ucg *half = ucg_var("half", ucg_uint_type(32));
        ucg_assign(half, ucg_op(DIV, big, ucg_int_value(32, 2)));
        Ucg *is_big = ucg_var("is_big", ucg_bool_type());
        ucg_assign(is_big, ucg_op(GT, big, ucg_int_value(32, 100)));
        Ucg *values[] = {
            ucg_typeof(ucg_uint_type(8)),
            ucg_string_value(" "),
            small,
            ucg_string_value(" "),
            big,
            ucg_string_value(" "),
            half,
            ucg_string_value(" "),
            is_big,
            ucg_string_value(" "),
            ucg_cast(big, ucg_float_type(64)),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 12);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 31: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *x = ucg_var("x", ucg_i32_type());
        ucg_assign(x, ucg_i32_value(5));
        Ucg *small = ucg_var("small", ucg_bool_type());
        ucg_assign(small, ucg_not(ucg_op(GT, x, ucg_i32_value(3))));
        Ucg *values[] = {
            small,
            ucg_string_value(" "),
            ucg_not(small),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 4);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    default: {
        return false;
    }
    }
    return true;
}

/*
TODO:
    + test it with all llvm versions
    + asm gen:
        get rid of .ll file generation when you finish
        since llvm can automaticaly generate the assembly
        that's compatible without saving it in external file
*/

void print_ucgs(void) {
    for (size_t i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        print_ucg(curr, '\n');
    }
}

void ucg_path(char *path) {
    gb_ucg.ll_path = path;
    gb_ucg.dir = dirname2(path);
}

void ucg_bin_path(char *path) {
    gb_ucg.exe_path = path;
}

void ucg_asm_path(char *path) {
    gb_ucg.asm_path = path;
}

void ucg_obj_path(char *path) {
    gb_ucg.obj_path = path;
}

char *ucg_find_lib_in(char *dir, char *name) {
    DIR *d = dir ? opendir(dir) : NULL;
    if (d == NULL)
        return NULL;
    char *prefix = format("lib%s", name);
    size_t len = strlen(prefix);
    char *found = NULL;
    for (struct dirent *entry = readdir(d); entry && !found; entry = readdir(d)) {
        char *file = entry->d_name;
        if (!strncmp2(file, prefix, len))
            continue;
        if (file[len] != '.' && file[len] != '-')
            continue;
        if (!strstr(file, ".so") && !strstr(file, ".dylib"))
            continue;
        found = format("%s/%s", dir, file);
    }
    closedir(d);
    return found;
}

char *ucg_find_lib(char *name) {
    char *dirs[] = {
        getenv("URA_LIB_PATH"),
        "/usr/local/lib",
        "/opt/homebrew/lib",
        "/usr/lib/x86_64-linux-gnu",
        "/usr/lib/aarch64-linux-gnu",
        "/usr/lib64",
        "/usr/lib",
    };
    for (size_t i = 0; i < sizeof(dirs) / sizeof(*dirs); i++) {
        char *path = ucg_find_lib_in(dirs[i], name);
        if (path)
            return path;
    }
    return NULL;
}

void ucg_link(char *path) {
    append(gb_ucg.libs, path);
}

bool compile_test(int p) {
    print(CYAN("Test %d: ================================\n"), p);
    ucg_start(format("test%d", p));
    bool r = run_test(p);
    print_ucgs();
    ucg_compile();
    ucg_path("build/out.ll");
    ucg_bin_path(r ? "build/exe.out" : NULL);
    ucg_gen();
    if (r)
        ucg_run_bin();
    ucg_finish();
    return r;
}
#if UCG
int main(int ac, char **av) {
    mkdir2("build");

    if (ac > 1)
        compile_test(atoi(av[1]));
    else
        for (int i = 0; compile_test(i); i++)
            ;
}
#endif

// UTILS
const char *to_string(Type type) {
    char *types[LAST + 1] = {
        // clang-format off
		[ERR] = "ERR",
		[ID] = "ID", [SCOPE] = "SCOPE", [URA] = "URA",

		[VAR] = "VAR", [DECLARE] = "DECLARE", [INIT_VAR] = "INIT_VAR",
		[ARG] = "ARG", [PARAM] = "PARAM", [GLOBAL] = "GLOBAL",

		[VAL_CREATE] = "VAL_CREATE",
		[TEMPLATE] = "TEMPLATE", [TEMPLATE_TYPE] = "TEMPLATE_TYPE",
		[TYPES] = "TYPES",

		[ANY] = "ANY",
		[VOID] = "VOID",
		[INT] = "INT", [FLOAT] = "FLOAT",
		[CHARS] = "CHARS",
		[REF] = "REF", [POINT_TO] = "POINT_TO",
		[ZERO] = "ZERO",

		[STRUCT] = "STRUCT", [DOT] = "DOT", [ATTR] = "ATTR", [ENUM] = "ENUM",

		[DREF] = "DREF",

		[LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
		[LBRACK] = "LBRACK", [RBRACK] = "RBRACK",
		[LCBRACK] = "LCBRACK", [RCBRACK] = "RCBRACK",
		[DOTS] = "DOTS", [DOUBLE_DOTS] = "DOUBLE_DOTS", [COMA] = "COMA",
		[OPERATOR] = "OPERATOR", [PUB] = "PUB",

		[ARRAY] = "ARRAY", // [ARRAY_LIT] = "ARRAY_LIT",
		[ACCESS] = "ACCESS",

		[ASSIGN] = "ASSIGN",
		[ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
		[MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
		[MOD_ASSIGN] = "MOD_ASSIGN",
		[BAND_ASSIGN] = "BAND_ASSIGN", [BOR_ASSIGN] = "BOR_ASSIGN",
		[BXOR_ASSIGN] = "BXOR_ASSIGN", [LSHIFT_ASSIGN] = "LSHIFT_ASSIGN",
		[RSHIFT_ASSIGN] = "RSHIFT_ASSIGN",

		[ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
		[DIV] = "DIV", [MOD] = "MOD",
		[BAND] = "BAND", [BOR] = "BOR", [BXOR] = "BXOR",
		[LSHIFT] = "LSHIFT", [RSHIFT] = "RSHIFT",

		[GT] = "GT", [LT] = "LT", [GE] = "GE",
		[LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

		[AND] = "AND", [OR] = "OR", [NOT] = "NOT", [NEG] = "NEG",

		[PROTO] = "PROTO", [FN_ENTRY] = "FN_ENTRY",
		[FN_DEC] = "FN_DEC", [ARGS] = "ARGS",
		[VARIADIC] = "VARIADIC",
		[FN_CALL] = "FN_CALL", [RETURN] = "RETURN", [WRITE] = "WRITE", [READ] = "READ", [LINK] = "LINK", [USE] = "USE",
		[MACRO_IF] = "MACRO_IF", [MACRO_ELIF] = "MACRO_ELIF", [MACRO_ELSE] = "MACRO_ELSE", [MACRO_ERROR] = "MACRO_ERROR",
		[SIZEOF] = "SIZEOF", [TYPEOF] = "TYPEOF", [DROP] = "DROP", [COPY] = "COPY",
		[IS_NULLABLE] = "IS_NULLABLE",

		[IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
		[THEN] = "THEN", [END] = "END",
		[BLOC] = "BLOC",
		[LOOP] = "LOOP", [BRK] = "BRK", [CNT] = "CNT",
		[MATCH] = "MATCH", [CASE] = "CASE", [DEFAULT] = "DEFAULT",
		[AS] = "AS",
		[LAST] = "LAST",
        // clang-format on
    };

    if (types[type] == NULL)
        return format("UNKNOWN(%d)", type);

    return types[type];
}
