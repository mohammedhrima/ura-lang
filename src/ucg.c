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
typedef LLVMOpcode OpCode;

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

    AND, OR, NOT, NEG, BNOT,

    PROTO, 
    FN_DEC, ARGS, VARIADIC, FN_CALL, RETURN, FORMAT,
    LINK, USE, MACRO_IF, MACRO_ELIF, MACRO_ELSE, MACRO_ERROR,
    MACRO_NAME, MACRO_VERSION, MACRO_ENTRY, MACRO_SCRIPT, MACRO_DEPENDENCY,
	FN_ENTRY,
    SIZEOF, TYPEOF, DROP, COPY, IS_ZERO, ALLOC, MEMCPY, FREE,
    AS, FROM, AND_LIT, OR_LIT, SELF, FOR, IN, MODULE, ARGC, ARGV,
    LINE_NUMBER, FILE_NAME, FN_NAME,
	BLOC,
    IF, ELIF, ELSE, THEN,
    LOOP, BRK, CNT, MATCH, CASE, DEFAULT, CLEAN,
    TRY, CATCH, THROW, CHECK,
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
    bool is_throwing;
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
    expand(Ucg *, cleans);
};

typedef struct {
    File *file;
    int s;
    int e;
    char *msg;
    char *help;
} UcgError;

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
    expand(Ucg *, runtime);

    expand(Ucg *, blocks);
    int clean_start;
    Ucg *clean_var;

    bool is_cleaning;
    bool throws;
    expand(Ucg *, tries);
    Ucg *thrown;
    Ucg *error;
    Ucg *error_type;
    expand(Ucg *, error_types);
    expand(Ucg *, error_ids);

    expand(UcgError, errors);
} gb_ucg;

void ucg_add_error(File *file, int s, int e, char *msg, char *help) {
    UcgError error = { .file = file, .s = s, .e = e, .msg = msg, .help = help };
    append(gb_ucg.errors, error);
}

bool ucg_has_errors(void) {
    return gb_ucg.errors_count > 0;
}

void ucg_check_errors(void) {
    if (gb_ucg.errors_count == 0)
        return;
    for (int i = 0; i < gb_ucg.errors_count; i++) {
        UcgError *error = &gb_ucg.errors[i];
        char *buff = error->file->buff;
        char *path = relative_path(error->file->path);
        int line = 1;
        for (int j = 0; j < error->s; j++)
            line += buff[j] == '\n';
        int start = error->s;
        while (start > 0 && buff[start - 1] != '\n')
            start--;
        int end = error->s;
        while (buff[end] && buff[end] != '\n')
            end++;
        int col = error->s - start;
        int width = snprintf(NULL, 0, "%d", line);

        fprintf(
            stderr, "%s: %s:%d:%d %s\n", RED("error"), path, line, col + 1, error->msg
        );
        fprintf(stderr, "%*s |\n", width, "");
        fprintf(stderr, "%d | %.*s\n", line, end - start, buff + start);
        fprintf(stderr, "%*s | %*s", width, "", col, "");
        for (int j = error->s; j < error->e || j == error->s; j++)
            fprintf(stderr, "^");
        fprintf(stderr, "\n");
        if (error->help)
            fprintf(stderr, "help: %s\n", error->help);
    }
    fprintf(
        stderr,
        "%s: aborting due to %d error%s\n",
        RED("error"),
        (int)gb_ucg.errors_count,
        gb_ucg.errors_count > 1 ? "s" : ""
    );
    exit(1);
}

void ucg_start(char *name) {
    // atexit(ucg_finish);

    gb_ucg.context = LLVMContextCreate();
    zero_init(gb_ucg.runtime);
    zero_init(gb_ucg.libs);
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.context);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.context);

    gb_ucg.types.vd = LLVMVoidTypeInContext(gb_ucg.context);

    for (int i = 1; i < 255; i++)
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

}

void ucg_finish(void) {
    if (gb_ucg.error_msg != NULL) {
        print("there is an error %s\n", gb_ucg.error_msg);
        // LLVMDisposeMessage(gb_ucg.error_msg);
        eprint("%s: <%s>\n", RED("error"), gb_ucg.error_msg);
    }
    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMDisposeTargetMachine(gb_ucg.machine);
    LLVMContextDispose(gb_ucg.context);
    gb_ucg.throws = false;
    gb_ucg.thrown = NULL;
    gb_ucg.error = NULL;
    gb_ucg.error_type = NULL;
    gb_ucg.error_types_count = 0;
    gb_ucg.error_ids_count = 0;
    gb_ucg.tries_count = 0;
    gb_ucg.blocks_count = 0;
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
        TypeRef type = is_extra ? NULL : fn_dec->fn.params[i]->rtype;
        bool is_pointer = LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind;
        if (type && is_pointer && LLVMTypeOf(value) != type)
            value = LLVMBuildBitCast(gb_ucg.builder, value, type, "");
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
    case INT:        	return gb_ucg.types.num[ir->shift];
    case FLOAT:			return llvm_float_type(ir->shift);
    case CHARS:			return LLVMPointerType(gb_ucg.types.num[8], 0);
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
    TypeRef rtype = llvm_type(to_create);
    switch (to_create->type) { // clang-format off
    case INT:   ir->elem = LLVMConstInt(rtype, to_create->num.value, 0); break;
    case FLOAT: ir->elem = LLVMConstReal(rtype, to_create->num.real); break;
    case REF:   ir->elem = LLVMConstNull(rtype); break; // clang-format on
    case STRUCT: {
        Value agg = LLVMConstNull(rtype);
        for (int i = 0; i < ir->arr.items_count; i++) {
            Ucg *item = ir->arr.items[i];
            if (item)
                agg = LLVMBuildInsertValue(gb_ucg.builder, agg, item->elem, i, "");
        }
        ir->elem = agg;
        break;
    }
    case CHARS: {
        if (ir->used < 0)
            break;
        int count = to_create->arr.items_count;
        char *text = arena_alloc(count + 1, sizeof(char));
        for (int i = 0; i < count; i++)
            text[i] = to_create->arr.items[i]->to_create->num.value;
        Value global = LLVMBuildGlobalString(gb_ucg.builder, text, "str");
        ir->elem = LLVMBuildPointerCast(gb_ucg.builder, global, rtype, "");
        break;
    }
    case ARRAY: {
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
    if (value->type != VAL_CREATE || value->to_create->type != CHARS)
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
    // Type ops[LAST + 1] = {
    //     [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
    //     [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    // };
    OpCode opcodes[LAST + 1] = {
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
    OpCode float_opcodes[LAST + 1] = {
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

void llvm_cast(Ucg *ir) {
    Value value = ir->left->elem;
    Ucg *to = ir->to_type;
    bool from_pointer = LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind;
    if (to->type == REF) {
        OpCode op = from_pointer ? LLVMBitCast : LLVMIntToPtr;
        ir->elem = LLVMBuildCast(gb_ucg.builder, op, value, llvm_type(to), "AS");
        return;
    }
    if (from_pointer) {
        ir->elem = LLVMBuildPtrToInt(gb_ucg.builder, value, llvm_type(to), "AS");
        return;
    }
    bool from_float = llvm_is_float(value);
    bool to_float = to->type == FLOAT;
    int from_bits = llvm_bits(value);
    int to_bits = to->shift;
    if (to->type == INT && to_bits == 1 && from_bits != 1) {
        Value zero = LLVMConstNull(LLVMTypeOf(value));
        if (from_float)
            ir->elem = LLVMBuildFCmp(gb_ucg.builder, LLVMRealUNE, value, zero, "AS");
        else
            ir->elem = LLVMBuildICmp(gb_ucg.builder, LLVMIntNE, value, zero, "AS");
        return;
    }

    if (from_float == to_float && from_bits == to_bits) {
        ir->elem = value;
        return;
    }
    bool from_uint = from_bits == 1 || ir->is_uint;
    OpCode op;
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
    new->shift = shift;
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
    new->shift = shift;
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

Ucg *ucg_array_from(Ucg *elem, Ucg **items, int count) {
    Ucg *new = ucg_array_new(elem);
    for (int i = 0; i < count; i++)
        append(new->to_create->arr.items, ucg_value(items[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_value(char *str) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ucg_new(CHARS);
    Ucg *to_create = new->to_create;
    for (int i = 0; i <= strlen(str); i++)
        append(to_create->arr.items, ucg_i8_value(str[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_array(char *str) {
    Ucg *new = ucg_array_new(ucg_char_type());
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

void ucg_clean(Ucg *var) {
    gb_ucg.clean_start = gb_ucg.insts_count;
    gb_ucg.clean_var = var;
    gb_ucg.is_cleaning = true;
}

void ucg_clean_end(void) {
    Ucg *clean = ucg_new(CLEAN);
    clean->left = gb_ucg.clean_var;
    for (int i = gb_ucg.clean_start; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type == FN_CALL)
            inst->to_call->used--;
        append(clean->cleans, inst);
    }
    gb_ucg.insts_count = gb_ucg.clean_start;
    gb_ucg.is_cleaning = false;
    Ucg *block = gb_ucg.blocks[gb_ucg.blocks_count - 1];
    append(block->cleans, clean);
}

void ucg_run_cleans(Ucg *block, Ucg *moved) {
    for (int i = block->cleans_count - 1; i >= 0; i--) {
        Ucg *clean = block->cleans[i];
        if (moved && clean->left == moved)
            continue;
        for (int j = 0; j < clean->cleans_count; j++) {
            Ucg *inst = clean->cleans[j];
            if (inst->type == FN_CALL)
                inst->to_call->used++;
            append(gb_ucg.insts, inst);
        }
    }
}

bool ucg_ends_with_jump(void) {
    Ucg *last = gb_ucg.insts[gb_ucg.insts_count - 1];
    return last->type == RETURN || last->type == BRK || last->type == CNT;
}

void ucg_close_block(void) {
    Ucg *block = gb_ucg.blocks[gb_ucg.blocks_count - 1];
    gb_ucg.blocks_count--;
    if (!ucg_ends_with_jump())
        ucg_run_cleans(block, NULL);
}

bool ucg_is_block_of(Ucg *block, Ucg *target) {
    if (target == NULL)
        return block->type == FN_DEC;
    for (Ucg *stmt = target; stmt; stmt = stmt->next)
        if (stmt == block)
            return true;
    return false;
}

void ucg_leave_blocks(Ucg *target, Ucg *moved) {
    for (int i = gb_ucg.blocks_count - 1; i >= 0; i--) {
        Ucg *block = gb_ucg.blocks[i];
        ucg_run_cleans(block, moved);
        if (ucg_is_block_of(block, target))
            return;
    }
}

Ucg *ucg_entry(Ucg *fn) {
    Ucg *new = ucg_new(FN_ENTRY);
    new->to_enter = fn;
    append(gb_ucg.insts, new);
    append(gb_ucg.blocks, fn);
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
    if (gb_ucg.throws && !gb_ucg.is_cleaning)
        ucg_check(fn_dec);
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

    ucg_leave_blocks(NULL, ret);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_return() {
    Ucg *new = ucg_new(RETURN);
    ucg_leave_blocks(NULL, NULL);
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

Ucg *ucg_runtime(char *name) {
    for (int i = 0; i < gb_ucg.runtime_count; i++) {
        if (strcmp2(gb_ucg.runtime[i]->name, name))
            return gb_ucg.runtime[i];
    }
    return NULL;
}

Ucg *ucg_extern(char *name, Ucg *ret, int count, ...) {
    Ucg *fn = ucg_runtime(name);
    if (fn)
        return fn;
    Ucg **params = arena_alloc(count + 1, sizeof(Ucg *));
    va_list ap;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        Ucg *type = va_arg(ap, Ucg *);
        params[i] = type->type == VARIADIC ? type : ucg_param("arg", type);
    }
    va_end(ap);
    fn = ucg_proto(name, ret, params, count);
    append(gb_ucg.runtime, fn);
    return fn;
}

Ucg *ucg_call(Ucg *fn, int count, ...) {
    Ucg **args = arena_alloc(count + 1, sizeof(Ucg *));
    va_list ap;
    va_start(ap, count);
    for (int i = 0; i < count; i++)
        args[i] = va_arg(ap, Ucg *);
    va_end(ap);
    return ucg_fn_call(fn, args, count);
}

Ucg *ucg_main_arg(char *name, Ucg *type) {
    Ucg *global = ucg_runtime(name);
    if (global == NULL) {
        global = ucg_new(GLOBAL);
        global->name = name;
        global->data_type = type;
        append(gb_ucg.runtime, global);
    }
    return ucg_dref(global);
}

Ucg *ucg_argc(void) {
    return ucg_main_arg("ucg.argc", ucg_i32_type());
}

Ucg *ucg_argv(void) {
    return ucg_main_arg("ucg.argv", ucg_ref_type(ucg_string_type()));
}

void ucg_main_args(void) {
    Ucg *argc = ucg_runtime("ucg.argc");
    Ucg *argv = ucg_runtime("ucg.argv");
    Ucg *main = NULL;
    bool is_used = false;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        bool is_arg = inst->to_dref == argc || inst->to_dref == argv;
        is_used = is_used || (inst->type == DREF && is_arg);
        if (inst->type == FN_DEC && strcmp2(inst->name, "main"))
            main = inst;
    }
    if (!is_used || main == NULL)
        return;
    if (argc)
        append(gb_ucg.insts, argc);
    if (argv)
        append(gb_ucg.insts, argv);
    if (main->fn.params_count != 0)
        return;
    append(main->fn.params, ucg_param("argc", ucg_i32_type()));
    append(main->fn.params, ucg_param("argv", ucg_ref_type(ucg_string_type())));
}

void llvm_main_args(Ucg *fn) {
    Ucg *args[] = { ucg_runtime("ucg.argc"), ucg_runtime("ucg.argv") };
    bool is_main = strcmp2(fn->name, "main") && fn->fn.params_count >= 2;
    for (int i = 0; is_main && i < 2; i++) {
        if (args[i] && args[i]->elem)
            LLVMBuildStore(gb_ucg.builder, LLVMGetParam(fn->elem, i), args[i]->elem);
    }
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
    bool is_array = value->type == VAL_CREATE && value->to_create->type == ARRAY;
    if (is_array)
        return ucg_ref_type(value->to_create->arr.elem);
    switch (value->type) { // clang-format off
    case DREF:       return ucg_type_of(value->to_dref);
    case VAL_CREATE: return value->to_create;
    case REF:        return ucg_ref_type(ucg_type_of(value->to_ref));
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

int ucg_layout(Ucg **values, int values_count, Ucg **args, int at) {
    int args_count = at;
    char *layout = "";
    for (int i = 0; i < values_count; i++) {
        Ucg *value = ucg_value(values[i]);
        char *text = ucg_string_text(value);
        if (text) {
            layout = format("%s%s", layout, text);
            value->used = -1;
            continue;
        }
        Ucg *type = ucg_value_type(value);
        int bits = type->shift;
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
        } else if (type->type == INT && bits == 21) {
            value = ucg_unic_text(value);
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
    args[at - 1] = ucg_string_value(layout);
    return args_count;
}

Ucg *ucg_write(Ucg *fd, Ucg **values, int values_count) {
    Ucg *i32 = ucg_i32_type();
    Ucg *str = ucg_string_type();
    Ucg *dprintf = ucg_extern("dprintf", i32, 3, i32, str, ucg_varg_type());
    Ucg **args = arena_alloc(values_count + 3, sizeof(Ucg *));
    int args_count = ucg_layout(values, values_count, args, 2);
    args[0] = fd;
    return ucg_fn_call(dprintf, args, args_count);
}

Ucg *ucg_format(Ucg *size, Ucg **values, int values_count) {
    Ucg *i32 = ucg_i32_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *str = ucg_string_type();
    Ucg *snprintf = ucg_extern("snprintf", i32, 4, str, i64, str, ucg_varg_type());
    Ucg **args = arena_alloc(values_count + 4, sizeof(Ucg *));
    int args_count = ucg_layout(values, values_count, args, 3);
    args[0] = ucg_null(str);
    args[1] = ucg_int_value(64, 0);
    ucg_assign(size, ucg_fn_call(snprintf, args, args_count));

    Ucg *cap = ucg_cast(ucg_op(ADD, size, ucg_i32_value(1)), i64);
    Ucg *text = ucg_var("format.text", str);
    ucg_assign(text, ucg_alloc(ucg_i8_type(), cap));
    Ucg **fill = arena_alloc(args_count + 1, sizeof(Ucg *));
    memcpy(fill, args, args_count * sizeof(Ucg *));
    fill[0] = text;
    fill[1] = cap;
    ucg_fn_call(snprintf, fill, args_count);
    return text;
}

Ucg *ucg_read_fn(void) {
    Ucg *str = ucg_string_type();
    Ucg *i32 = ucg_i32_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *line_ref = ucg_ref_type(str);
    Ucg *getline = ucg_extern("getline", i64, 3, line_ref, ucg_ref_type(i64), str);
    Ucg *find = ucg_extern("strchr", str, 2, str, i32);
    Ucg *copy = ucg_extern("strdup", str, 1, str);
    Ucg *open = ucg_extern("fdopen", str, 2, i32, str);
    Ucg *file = ucg_global("ura.read_file", str);
    Ucg *file_fd = ucg_global("ura.read_fd", i32);
    Ucg *fn = ucg_fn("ura.read", str, (Ucg *[]){ ucg_param("fd", i32) }, 1);
    append(gb_ucg.runtime, fn);
    Ucg *fd = ucg_get_param(fn, 0);

    Ucg *reopen = ucg_if();
    Ucg *is_closed = ucg_op(EQ, file, ucg_null(str));
    ucg_then(reopen, ucg_op(OR, is_closed, ucg_op(NQ, fd, file_fd)));
    ucg_assign(file, ucg_call(open, 2, fd, ucg_string_value("r")));
    ucg_assign(file_fd, fd);
    ucg_end(reopen);

    Ucg *line = ucg_var("line", str);
    Ucg *cap = ucg_var("cap", i64);
    Ucg *len = ucg_var("len", i64);
    ucg_assign(len, ucg_call(getline, 3, ucg_ref(line), ucg_ref(cap), file));

    Ucg *eof = ucg_if();
    ucg_then(eof, ucg_op(LT, len, ucg_int_value(64, 1)));
    ucg_ret(ucg_call(copy, 1, ucg_string_value("")));
    ucg_end(eof);

    Ucg *newline = ucg_var("newline", str);
    ucg_assign(newline, ucg_call(find, 2, line, ucg_i32_value('\n')));
    Ucg *found = ucg_if();
    ucg_then(found, ucg_op(NQ, newline, ucg_null(str)));
    ucg_assign(ucg_point_to(newline), ucg_char_value(0));
    ucg_end(found);
    ucg_ret(line);
    ucg_end(fn);
    return fn;
}

Ucg *ucg_read(Ucg *fd) {
    Ucg *fn = ucg_runtime("ura.read");
    return ucg_call(fn ? fn : ucg_read_fn(), 1, fd);
}

Ucg *ucg_unic_fn(void) {
    Ucg *i32 = ucg_i32_type();
    Ucg *str = ucg_string_type();
    Ucg *params[] = { ucg_param("code", i32), ucg_param("text", str) };
    Ucg *fn = ucg_fn("ura.unic", str, params, 2);
    append(gb_ucg.runtime, fn);
    Ucg *code = ucg_get_param(fn, 0);
    Ucg *text = ucg_get_param(fn, 1);

    // code < 128     -> [code]
    // code < 2048    -> [192 + code / 64,     128 + code % 64]
    // code < 65536   -> [224 + code / 4096,   128 + code / 64 % 64,   128 + code % 64]
    // code < 1114112 -> [240 + code / 262144, 128 + code / 4096 % 64, ...]
    // 233 (é)        -> [192 + 3, 128 + 41] = [C3, A9]
    int below[] = { 128, 2048, 65536, 1114112 };
    int marker[] = { 0, 192, 224, 240 };
    Ucg *stmt = ucg_if();
    for (int count = 1; count <= 4; count++) {
        Ucg *branch = count == 1 ? stmt : ucg_elif(stmt);
        ucg_then(branch, ucg_op(LT, code, ucg_i32_value(below[count - 1])));
        int place = 1;
        for (int i = count - 1; i >= 0; i--) {
            Ucg *digit = ucg_op(DIV, code, ucg_i32_value(place));
            Ucg *first = ucg_op(ADD, digit, ucg_i32_value(marker[count - 1]));
            Ucg *next = ucg_op(MOD, digit, ucg_i32_value(64));
            next = ucg_op(ADD, next, ucg_i32_value(128));
            Ucg *byte = ucg_cast(i == 0 ? first : next, ucg_char_type());
            ucg_assign(ucg_access(text, ucg_i32_value(i)), byte);
            place *= 64;
        }
        ucg_assign(ucg_access(text, ucg_i32_value(count)), ucg_char_value(0));
    }
    ucg_end(stmt);
    ucg_ret(text);
    ucg_end(fn);
    return fn;
}

Ucg *ucg_unic_text(Ucg *code) {
    Ucg *fn = ucg_runtime("ura.unic");
    if (fn == NULL)
        fn = ucg_unic_fn();
    Ucg *zero = ucg_char_value(0);
    Ucg *text = ucg_array_value(ucg_char_type(), zero, zero, zero, zero, zero, NULL);
    return ucg_call(fn, 2, ucg_cast(code, ucg_i32_type()), text);
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
    bool is_uint = ucg_is_uint(index);
    new->index = is_uint ? ucg_cast(index, ucg_uint_type(64)) : ucg_value(index);
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

Ucg *ucg_alloc(Ucg *type, Ucg *count) {
    Ucg *bytes = ucg_string_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *calloc = ucg_extern("calloc", bytes, 2, i64, i64);
    Ucg *size = ucg_cast(ucg_sizeof(type), i64);
    Ucg *memory = ucg_call(calloc, 2, ucg_cast(count, i64), size);
    return ucg_cast(memory, ucg_ref_type(type));
}

Ucg *ucg_copy(Ucg *dst, Ucg *src, Ucg *count) {
    Ucg *bytes = ucg_string_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *memcpy = ucg_extern("memcpy", bytes, 3, bytes, bytes, i64);
    dst = ucg_value(dst);
    Ucg *size = ucg_cast(ucg_sizeof(ucg_value_type(dst)->to_ref), i64);
    Ucg *to = ucg_cast(dst, bytes);
    Ucg *from = ucg_cast(src, bytes);
    return ucg_call(memcpy, 3, to, from, ucg_op(MUL, ucg_cast(count, i64), size));
}

Ucg *ucg_free(Ucg *memory) {
    Ucg *bytes = ucg_string_type();
    Ucg *free = ucg_extern("free", ucg_void_type(), 1, bytes);
    return ucg_call(free, 1, ucg_cast(memory, bytes));
}

char *ucg_type_name(Ucg *type) {
    switch (type->type) {
    case INT: {
        int bits = type->shift;
        if (bits == 1)
            return "bool";
        return format("%c%d", type->is_uint ? 'u' : 'i', bits);
    }
    case FLOAT:
        return format("f%d", type->shift);
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
    ucg_close_block();
    return ucg_branch(ELIF, prev_stmt);
}

Ucg *ucg_else(Ucg *prev_stmt) {
    ucg_close_block();
    return ucg_branch(ELSE, prev_stmt);
}

Ucg *ucg_then(Ucg *stmt, Ucg *cond) {
    Ucg *new = ucg_new(THEN);
    stmt->cond = cond ? ucg_value(cond) : NULL;
    new->stmt = stmt;
    append(gb_ucg.insts, new);
    append(gb_ucg.blocks, stmt);
    return new;
}

Ucg *ucg_end(Ucg *stmt) {
    ucg_close_block();
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
    ucg_leave_blocks(loop, NULL);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_cnt(Ucg *loop) {
    ucg_leave_blocks(loop, NULL);
    Ucg *new = ucg_new(CNT);
    new->stmt = loop;
    append(gb_ucg.insts, new);
    return new;
}

void ucg_throws(void) {
    gb_ucg.throws = true;
    gb_ucg.thrown = ucg_global("ucg.thrown", ucg_bool_type());
}

Ucg *ucg_thrown(void) {
    return gb_ucg.thrown;
}

Ucg *ucg_error_type(void) {
    if (gb_ucg.error_type == NULL)
        gb_ucg.error_type = ucg_global("ucg.error_type", ucg_i32_type());
    return gb_ucg.error_type;
}

Ucg *ucg_type_id(Ucg *type) {
    Ucg *id = ucg_i32_value(0);
    append(gb_ucg.error_types, type);
    append(gb_ucg.error_ids, id);
    return id;
}

Ucg *ucg_error(Ucg *type) {
    if (gb_ucg.error == NULL)
        gb_ucg.error = ucg_global("ucg.error", ucg_int_type(64));
    Ucg *at = ucg_var("error.at", ucg_ref_type(type));
    ucg_assign(at, ucg_cast(ucg_ref(gb_ucg.error), ucg_ref_type(type)));
    return ucg_point_to(at);
}

Ucg *ucg_find_try(void) {
    for (int i = gb_ucg.blocks_count - 1; i >= 0; i--) {
        Ucg *block = gb_ucg.blocks[i];
        if (block->type == FN_DEC)
            return NULL;
        for (int j = 0; j < gb_ucg.tries_count; j++)
            if (gb_ucg.tries[j] == block)
                return block;
    }
    return NULL;
}

Ucg *ucg_goto_catch(void) {
    Ucg *try_stmt = ucg_find_try();
    Ucg *fn = NULL;
    for (int i = gb_ucg.blocks_count - 1; i >= 0 && fn == NULL; i--)
        if (gb_ucg.blocks[i]->type == FN_DEC)
            fn = gb_ucg.blocks[i];
    ucg_assign(ucg_thrown(), ucg_bool_value(0));
    ucg_leave_blocks(try_stmt, NULL);
    ucg_assign(ucg_thrown(), ucg_bool_value(1));
    Ucg *new = ucg_new(try_stmt ? BRK : RETURN);
    if (try_stmt)
        new->stmt = try_stmt;
    else if (fn->fn.ret->type != VOID)
        new->to_ret = ucg_value(ucg_null(fn->fn.ret));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_throw(Ucg *value) {
    value = ucg_value(value);
    Ucg *type = ucg_value_type(value);
    ucg_assign(ucg_error(type), value);
    ucg_assign(ucg_error_type(), ucg_type_id(type));
    return ucg_goto_catch();
}

void ucg_check(Ucg *fn) {
    if (gb_ucg.blocks_count == 0)
        return;
    int start = gb_ucg.insts_count;
    Ucg *stmt = ucg_if();
    stmt->name = "thrown";
    ucg_then(stmt, ucg_thrown());
    ucg_goto_catch();
    ucg_end(stmt);
    Ucg *check = ucg_new(CHECK);
    check->to_call = fn;
    for (int i = start; i < gb_ucg.insts_count; i++)
        append(check->cleans, gb_ucg.insts[i]);
    gb_ucg.insts_count = start;
    append(gb_ucg.insts, check);
}

Ucg *ucg_try(void) {
    Ucg *stmt = ucg_if();
    stmt->name = "try";
    ucg_then(stmt, ucg_bool_value(1));
    append(gb_ucg.tries, stmt);
    return stmt;
}

Ucg *ucg_end_try(Ucg *try_stmt) {
    gb_ucg.tries_count--;
    return ucg_end(try_stmt);
}

Ucg *ucg_catch(Ucg *stmt, Ucg *type) {
    bool is_first = gb_ucg.tries_count && gb_ucg.tries[gb_ucg.tries_count - 1] == stmt;
    Ucg *catch_stmt = NULL;
    if (is_first) {
        ucg_end_try(stmt);
        catch_stmt = ucg_if();
    } else
        catch_stmt = ucg_elif(stmt);
    catch_stmt->name = "catch";
    Ucg *is_type = ucg_op(EQ, ucg_error_type(), ucg_type_id(type));
    ucg_then(catch_stmt, ucg_op(AND, ucg_thrown(), is_type));
    ucg_assign(ucg_thrown(), ucg_bool_value(0));
    return catch_stmt;
}

Ucg *ucg_end_catch(Ucg *catch_stmt) {
    Ucg *end = ucg_end(catch_stmt);
    Ucg *uncaught = ucg_if();
    uncaught->name = "rethrow";
    ucg_then(uncaught, ucg_thrown());
    ucg_goto_catch();
    ucg_end(uncaught);
    return end;
}

void print_ucg(Ucg *inst, char end) {
    char *text = "";

    text = format("%s%s%s", text, to_string(inst->type), " ");
    if (inst->name)
        text = format("%s(%s)", text, inst->name ? inst->name : "");
    else if (inst->is_val) {
        switch (inst->type) { // clang-format off
        case INT:   text = format("%s(%lld)", text, inst->num.value); break;
        case FLOAT: text = format("%s(%f)", text, inst->num.real); break;
        // clang-format on
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

bool ucg_can_throw(Ucg *fn) {
    bool is_inside = false;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type == FN_ENTRY && inst->to_enter == fn)
            is_inside = true;
        if (!is_inside)
            continue;
        if (inst->type == END && inst->to_end == fn)
            return false;
        bool is_error = gb_ucg.error_type && inst->left == gb_ucg.error_type;
        bool is_throw = inst->type == ASSIGN && is_error;
        bool is_call = inst->type == CHECK && inst->to_call->is_throwing;
        if (is_throw || is_call)
            return true;
    }
    return false;
}

void ucg_find_throwing(void) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < gb_ucg.insts_count; i++) {
            Ucg *fn = gb_ucg.insts[i];
            if (fn->type != FN_DEC || fn->is_throwing || !ucg_can_throw(fn))
                continue;
            fn->is_throwing = true;
            changed = true;
        }
    }
}

void ucg_splice_checks(void) {
    int total = gb_ucg.insts_count;
    for (int i = 0; i < gb_ucg.insts_count; i++)
        total += gb_ucg.insts[i]->cleans_count;
    Ucg **insts = arena_alloc(total + 2, sizeof(Ucg *));
    int count = 0;
    ucg_find_throwing();
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type != CHECK) {
            insts[count++] = inst;
            continue;
        }
        bool is_throwing = inst->to_call->is_throwing;
        for (int j = 0; j < inst->cleans_count; j++) {
            Ucg *part = inst->cleans[j];
            if (is_throwing)
                insts[count++] = part;
            else if (part->type == FN_CALL)
                part->to_call->used--;
        }
    }
    gb_ucg.insts = insts;
    gb_ucg.insts_count = count;
    gb_ucg.insts_size = total + 2;
}

void ucg_number_errors(void) {
    if (gb_ucg.error == NULL)
        return;
    TargetData layout = LLVMCreateTargetDataLayout(gb_ucg.machine);
    TypeRef *seen = arena_alloc(gb_ucg.error_types_count + 1, sizeof(TypeRef));
    int seen_count = 0;
    unsigned long long size = 8;
    for (int i = 0; i < gb_ucg.error_types_count; i++) {
        TypeRef type = llvm_type(gb_ucg.error_types[i]);
        int id = 0;
        while (id < seen_count && seen[id] != type)
            id++;
        if (id == seen_count)
            seen[seen_count++] = type;
        gb_ucg.error_ids[i]->to_create->num.value = id + 1;
        unsigned long long type_size = LLVMABISizeOfType(layout, type);
        if (type_size > size)
            size = type_size;
    }
    gb_ucg.error->data_type = ucg_array_type(ucg_int_type(64), (size + 7) / 8);
    LLVMDisposeTargetData(layout);
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
        bool is_module_level =
            inst->type == FN_DEC || inst->type == GLOBAL || inst->type == STRUCT;
        if ((!removed_fn || is_module_level) && !is_removed)
            gb_ucg.insts[count++] = inst;
        if (is_body_end)
            removed_fn = NULL;
    }
    gb_ucg.insts_count = count;
}

void ucg_compile() {
    ucg_splice_checks();
    ucg_optimize();
    ucg_main_args();
    Value fn = NULL;
    expand(Value, outer_fns);
    zero_init(outer_fns);
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
    ucg_number_errors();
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) { // clang-format off
        case FN_DEC: llvm_fn_dec(curr); break;
        case GLOBAL: llvm_declare_global(curr); break; // clang-format on
        case FN_ENTRY: {
            append(outer_fns, fn);
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
            char *kind = curr->name ? curr->name : "if";
            curr->bloc = llvm_bloc(fn, format("%s%d", kind, stmt_index));
            curr->then = llvm_bloc(fn, format("%s%d.then", kind, stmt_index));
            stmt_index++;
            break;
        }
        case ELIF: {
            char *kind = curr->name ? curr->name : "elif";
            curr->bloc = llvm_bloc(fn, format("%s%d", kind, stmt_index));
            curr->then = llvm_bloc(fn, format("%s%d.then", kind, stmt_index));
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
            if (to_end->type == FN_DEC) {
                fn = outer_fns[--outer_fns_count];
                break;
            }
            char *name = to_end->type == LOOP ? "endloop" : "endif";
            if (to_end->name)
                name = format("end%s", to_end->name);
            curr->bloc = llvm_bloc(fn, name);
            // stmt_index++;
            break;
        }
        default: break;
        }
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        // print_ucg(curr, '\n');
        switch (curr->type) { // clang-format off
        case INT:
        case FLOAT:      break;
        case FN_DEC:     break;
        case GLOBAL:     break;
        case STRUCT:     break;
        case RETURN:     llvm_ret(curr->to_ret); break;
        case FN_CALL:    llvm_fn_call(curr); break;
        case VAL_CREATE: llvm_val_create(curr); break;
        case DECLARE:    llvm_declare_var(curr); break;
        case DREF:       llvm_load(curr); break;
        case THEN:       llvm_cond(curr->stmt); break;
        case BRK:        llvm_jmp(curr->stmt->end->bloc); break;
        case CNT:        llvm_jmp(curr->stmt->bloc); break;
        case NEG:        llvm_neg(curr); break;
        case NOT:        llvm_not(curr); break;
        case AS:         llvm_cast(curr); break;
        case DOT:        llvm_get_attr(curr); break;
        case ACCESS:     llvm_access(curr); break;
        case SIZEOF:     llvm_sizeof(curr); break;
        case TYPEOF:     llvm_typeof(curr); break;
        case REF:        llvm_ref(curr); break;
        case POINT_TO:   llvm_point_to(curr); break;
        case ASSIGN:
        case ADD: case SUB: case MUL: case DIV: case MOD:         // math
        case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: // bitwise
        case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:        // composite
        case DIV_ASSIGN: case MOD_ASSIGN:
        case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
        case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
        case LT: case GT: case EQ: case LE: case GE: case NQ:     // comparison
        case AND: case OR:                                        // logic
            llvm_op(curr);
            break;
        // clang-format on
        case FN_ENTRY: {
            curr->to_enter->caller_bloc = llvm_get_pos();
            llvm_set_pos(curr->bloc);
            Ucg *fn = curr->to_enter;
            // assert()
            for (int i = 0; i < fn->fn.params_count; i++) {
                Ucg *param = fn->fn.params[i];
                llvm_declare_param(param);
            }
            llvm_main_args(fn);

            break;
        } // clang-format off
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
        getenv("URA_LIBC"),
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


// UTILS
const char *to_string(Type type) {
#define NAME(t) [t] = #t
    char *types[LAST + 1] = { // clang-format off
        NAME(ERR),
        NAME(ID),            NAME(SCOPE),         NAME(URA),

        NAME(VAR),           NAME(DECLARE),       NAME(INIT_VAR),
        NAME(ARG),           NAME(PARAM),         NAME(GLOBAL),

        NAME(VAL_CREATE),    NAME(TEMPLATE),      NAME(TEMPLATE_TYPE),
        NAME(TYPES),

        NAME(ANY),           NAME(VOID),          NAME(INT),
        NAME(FLOAT),         NAME(CHARS),         NAME(REF),
        NAME(POINT_TO),      NAME(ZERO),

        NAME(STRUCT),        NAME(DOT),           NAME(ATTR),
        NAME(ENUM),          NAME(DREF),

        NAME(LPARENT),       NAME(RPARENT),       NAME(LBRACK),
        NAME(RBRACK),        NAME(LCBRACK),       NAME(RCBRACK),
        NAME(DOTS),          NAME(DOUBLE_DOTS),   NAME(COMA),
        NAME(OPERATOR),      NAME(PUB),

        NAME(ARRAY),         NAME(ARRAY_LIT),     NAME(ACCESS),

        NAME(ASSIGN),        NAME(ADD_ASSIGN),    NAME(SUB_ASSIGN),
        NAME(MUL_ASSIGN),    NAME(DIV_ASSIGN),    NAME(MOD_ASSIGN),
        NAME(BAND_ASSIGN),   NAME(BOR_ASSIGN),    NAME(BXOR_ASSIGN),
        NAME(LSHIFT_ASSIGN), NAME(RSHIFT_ASSIGN),

        NAME(ADD),           NAME(SUB),           NAME(MUL),
        NAME(DIV),           NAME(MOD),
        NAME(BAND),          NAME(BOR),           NAME(BXOR),
        NAME(LSHIFT),        NAME(RSHIFT),

        NAME(GT),            NAME(LT),            NAME(GE),
        NAME(LE),            NAME(EQ),            NAME(NQ),

        NAME(AND),           NAME(OR),            NAME(NOT),
        NAME(NEG),           NAME(BNOT),

        NAME(PROTO),         NAME(FN_ENTRY),      NAME(FN_DEC),
        NAME(ARGS),          NAME(VARIADIC),      NAME(FN_CALL),
        NAME(RETURN),        NAME(FORMAT),
        NAME(LINK),          NAME(USE),
        NAME(MACRO_IF),      NAME(MACRO_ELIF),    NAME(MACRO_ELSE),
        NAME(MACRO_ERROR),   NAME(MACRO_NAME),    NAME(MACRO_VERSION),
        NAME(MACRO_ENTRY),   NAME(MACRO_SCRIPT),  NAME(MACRO_DEPENDENCY),
        NAME(SIZEOF),        NAME(TYPEOF),        NAME(DROP),
        NAME(COPY),          NAME(ALLOC),         NAME(MEMCPY),
        NAME(FREE),          NAME(IS_ZERO),

        NAME(IF),            NAME(ELIF),          NAME(ELSE),
        NAME(THEN),          NAME(END),           NAME(BLOC),
        NAME(LOOP),          NAME(BRK),           NAME(CNT),
        NAME(MATCH),         NAME(CASE),          NAME(DEFAULT),
        NAME(CLEAN),
        NAME(TRY),           NAME(CATCH),         NAME(THROW),
        NAME(CHECK),

        NAME(AS),            NAME(FROM),          NAME(AND_LIT),
        NAME(OR_LIT),        NAME(SELF),          NAME(FOR),
        NAME(IN),            NAME(MODULE),        NAME(ARGC),
        NAME(ARGV),          NAME(LINE_NUMBER),   NAME(FILE_NAME),
        NAME(FN_NAME),
        NAME(LAST),
    }; // clang-format on
#undef NAME

    if (types[type] == NULL)
        return format("UNKNOWN(%d)", type);

    return types[type];
}
