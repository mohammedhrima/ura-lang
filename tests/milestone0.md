# Milestone 0: LLVM Feature Wrapper

Specification of `src/ir.c`: a feature-level API over LLVM. Only layouts and signatures, no implementation. This file has no test entries, so the test runner skips it.

## Rules

- Only `ir.c` includes `llvm-c`. The core holds opaque pointers (`UraType *`, `UraValue *`, ...) and never names an `LLVM...Ref`.
- Immediate builder: every `ura_` call emits its LLVM instruction right away and returns a handle.
- Every function starts with `ura_` and takes the context first. Objects come from `ura_alloc`; lists use `expand` and `push_back`.
- The core passes the value names (`load.x`, `dref.p`, `ADD`); the wrapper keeps them as given.
- Reference: the recorded IR of milestones 1 to 4. LLVM 12, typed pointers.
- Checks, traps and optimisation are off by default, because the recorded IR has none.

### Names the generated IR keeps

| What | Names |
|------|-------|
| blocks | `entry`, `then`, `next` (following test or else body), `endif`, `cond`, `endwhile`, `dead` (code after a terminator) |
| variables | `x` (alloca), `load.x`, `dref.p` (through a reference), `obj.field` |
| operations | `ADD SUB MUL DIV MOD`, `EQ NQ LT GT LE GE`, `AND OR NOT`, `ADD_ASSIGN` ... |
| helpers | `str`, `str.1` (string globals), `array` (backing storage), `e`, `e1` (element addresses), `dst` (memcpy target) |
| module | `ModuleID` and `source_filename` are `ura`; struct types are named after the struct, instances `Box<i32>` |

---

## Milestone 0.1: Context, Types, Constants and Globals

### Context

```c
typedef enum { URA_DIAG_NOTE, URA_DIAG_WARNING, URA_DIAG_ERROR, URA_DIAG_FATAL } UraDiagLevel;
typedef void (*UraDiagFn)(void *user, UraDiagLevel level, const char *message);

typedef enum {
    URA_FLAG_VERIFY_MODULE,     // on by default
    URA_FLAG_KEEP_NAMES,        // on by default
    URA_FLAG_DEBUG_INFO,
    URA_FLAG_SANITIZE,
    URA_FLAG_BOUNDS_CHECKS,     // off by default
    URA_FLAG_CHECK_DIV_ZERO,    // off by default
    URA_FLAG_TRAP_NULL_DEREF,   // off by default
} UraIRFlag;

struct UraIRContext {
    LLVMContextRef       llvm_context;     // the LLVM fields are only touched by ir.c
    LLVMModuleRef        llvm_module;
    LLVMBuilderRef       llvm_builder;
    LLVMTargetDataRef    target_data;
    LLVMTargetMachineRef target_machine;
    char                *triple;
    UraTypeCache         types;            // uniqued types
    UraCursor            cursor;           // where we emit
    UraScopeStack        scopes;           // symbol tables
    expand(UraGlobal *, globals);
    expand(UraFunc *, functions);
    expand(UraStruct *, structs);
    UraOptimizer         optimizer;        // see 0.7
    UraDebugInfo        *debug;            // see 0.6
    UraDiagFn            diag;
    void                *diag_user;
    UraIRFlags           flags;
    UraRuntimeDecls      runtime;          // printf, calloc, free, write, exit, ...
    UraGlobal           *error_slot;       // try and throw, see 0.3
};

struct UraCursor {
    UraFunc  *func;          // function being emitted, NULL at global level
    UraBlock *block;         // current block
    UraBlock *entry;         // entry block: every alloca goes here
    LLVMValueRef alloca_end; // last alloca, so allocas stay together
    expand(UraLoopFrame, loops);
    size_t    scope_depth;
    // + a stack of saved cursors for nested functions
};
```

```c
UraIRContext *ura_ir_create(const char *module_name, const char *source_name, UraDiagFn diag, void *user);
void ura_ir_set_target(UraIRContext *ctx, const char *triple, const char *cpu, const char *features,
                       UraRelocMode reloc, UraCodeModel model);
void ura_ir_set_flag(UraIRContext *ctx, UraIRFlag flag, bool value);
void ura_ir_verify(UraIRContext *ctx);       // failures go to the diag callback
void ura_ir_reset(UraIRContext *ctx);
void ura_ir_dispose(UraIRContext *ctx);

void ura_cursor_push(UraIRContext *ctx);
void ura_cursor_pop(UraIRContext *ctx);
```

### Types

```c
typedef enum {
    URA_VOID, URA_BOOL,
    URA_I8, URA_I16, URA_I32, URA_I64,
    URA_U8, URA_U16, URA_U32, URA_U64,
    URA_F32, URA_F64,
    URA_PTR, URA_REF, URA_ARRAY, URA_SLICE,
    URA_STRUCT, URA_FUNC, URA_TUPLE, URA_ENUM, URA_OPAQUE,
} UraTypeKind;

struct UraType {
    UraTypeKind kind;
    char       *name;           // struct, enum
    unsigned    bits;           // integers, floats
    bool        is_signed;
    UraType    *elem;           // pointee, array or slice element
    size_t      length;         // array
    expand(UraField, fields);   // struct, tuple
    UraType    *ret;            // function
    expand(UraType *, params);
    bool        is_variadic;
    UraStruct  *struct_info;
    size_t      size, align;    // filled when the type is complete
    bool        is_complete;
    LLVMTypeRef llvm;
};

struct UraField {
    char *name; UraType *type;
    unsigned index; size_t offset;
    bool is_ref; UraValue *default_value;
};
```

```c
// types are unique: asking twice for the same type returns the same pointer
UraType *ura_type_void(UraIRContext *ctx);
UraType *ura_type_bool(UraIRContext *ctx);
UraType *ura_type_int(UraIRContext *ctx, unsigned bits, bool is_signed);
UraType *ura_type_float(UraIRContext *ctx, unsigned bits);
UraType *ura_type_ptr(UraIRContext *ctx, UraType *elem);
UraType *ura_type_ref(UraIRContext *ctx, UraType *elem);
UraType *ura_type_array(UraIRContext *ctx, UraType *elem, size_t length);
UraType *ura_type_slice(UraIRContext *ctx, UraType *elem);
UraType *ura_type_func(UraIRContext *ctx, UraType *ret, UraType **params, size_t count, bool variadic);
UraType *ura_type_tuple(UraIRContext *ctx, UraType **elems, size_t count);
UraType *ura_type_enum(UraIRContext *ctx, const char *name);

// a struct is created by name first (opaque), then given its body
UraType *ura_type_struct_declare(UraIRContext *ctx, const char *name);
bool     ura_type_struct_define(UraIRContext *ctx, UraType *type, UraType **field_types,
                                const char **field_names, size_t count);

UraTypeKind ura_type_kind(UraType *t);       UraType *ura_type_elem(UraType *t);
bool ura_type_is_integer(UraType *t);        bool ura_type_is_signed(UraType *t);
bool ura_type_is_float(UraType *t);          bool ura_type_is_complete(UraType *t);
size_t ura_sizeof(UraIRContext *ctx, UraType *t);
size_t ura_alignof(UraIRContext *ctx, UraType *t);
size_t ura_offsetof(UraIRContext *ctx, UraType *struct_type, unsigned field);
char  *ura_type_to_string(UraIRContext *ctx, UraType *t);     // "i32", "&i32", "Box<i32>"
```

Representation: `bool` is `i1` everywhere, `char` is `i8`, the untyped pointer is `i8*`, unsigned types share the LLVM integer and only change the instruction chosen. An array variable is a slot holding the pointer to element 0, the elements live in a separate backing array named `array`, and a parameter is the bare element pointer.

### Values, constants, globals, symbols

```c
struct UraValue {
    UraValueKind kind;       // CONST, ZERO, GLOBAL, PARAM, FUNC, INST, ALLOCA
    UraType     *type;
    bool         is_address; // a variable, a field or an element, not a plain value
    char        *name;
    LLVMValueRef llvm;
};

typedef enum { URA_EXTERNAL, URA_INTERNAL, URA_PRIVATE, URA_EXTERN_DECL } UraLinkage;

struct UraGlobal {
    char *name; UraType *type; UraValue *initializer;   // NULL: zero
    bool is_constant; UraLinkage linkage; UraValue *value;
    LLVMValueRef llvm;
};

struct UraSymbol { char *name; UraSymbolKind kind; UraType *type; UraValue *value; bool is_ref, is_param; size_t depth; };
struct UraScope  { UraScope *parent; expand(UraSymbol, symbols); UraScopeKind kind; expand(UraCleanup, cleanups); char *owner_name; };
```

```c
UraValue *ura_const_int(UraIRContext *ctx, UraType *type, long long value);
UraValue *ura_const_float(UraIRContext *ctx, UraType *type, double value);
UraValue *ura_const_bool(UraIRContext *ctx, bool value);
UraValue *ura_const_char(UraIRContext *ctx, int value);
UraValue *ura_const_null(UraIRContext *ctx, UraType *type);
UraValue *ura_const_zero(UraIRContext *ctx, UraType *type);        // x i32 with no value
UraValue *ura_const_enum(UraIRContext *ctx, UraType *enum_type, long long value);
UraValue *ura_const_array(UraIRContext *ctx, UraType *elem, UraValue **values, size_t count);
UraValue *ura_const_struct(UraIRContext *ctx, UraType *type, UraValue **values, size_t count);
long long ura_const_int_value(UraValue *v);
bool      ura_int_fits(UraType *type, long long value);            // for out-of-range literals
bool      ura_value_is_const(UraValue *v);   bool ura_value_is_null_const(UraValue *v);
UraType  *ura_value_type(UraValue *v);       UraType *ura_value_pointee(UraValue *v);

UraGlobal *ura_global_create(UraIRContext *ctx, const char *name, UraType *type, UraValue *init, UraLinkage linkage);
UraGlobal *ura_global_lookup(UraIRContext *ctx, const char *name);
UraValue  *ura_global_address(UraGlobal *g);
UraValue  *ura_string_literal(UraIRContext *ctx, const char *bytes, size_t length);   // private constant, i8*

UraScope  *ura_scope_push(UraIRContext *ctx, UraScopeKind kind, const char *owner_name);
void       ura_scope_pop(UraIRContext *ctx);             // runs the cleanups of the scope
UraSymbol *ura_symbol_declare(UraIRContext *ctx, const char *name, UraSymbolKind kind, UraType *type, UraValue *value);
UraSymbol *ura_symbol_lookup(UraIRContext *ctx, const char *name);          // innermost scope first
UraSymbol *ura_symbol_lookup_qualified(UraIRContext *ctx, const char **path, size_t count);

char *ura_ir_to_string(UraIRContext *ctx);   void ura_ir_free_string(char *text);
bool  ura_ir_emit_ll(UraIRContext *ctx, const char *path);
bool  ura_ir_emit_bitcode(UraIRContext *ctx, const char *path);
```

### Examples that must pass

Set-up assumed by every snippet: `ctx` is a `UraIRContext *`; `i8`, `i32`, `v` and `b` are `ura_type_int(ctx, 8, true)`, `ura_type_int(ctx, 32, true)`, `ura_type_void(ctx)` and `ura_type_bool(ctx)`; `K(n)` is `ura_const_int(ctx, i32, n)`. A `// expect:` line is what must hold once the module verifies, builds with clang and runs.

#### Create a context and read the module header

```c
UraIRContext *ctx = ura_ir_create("ura", "test.ura", on_diag, NULL);
ura_ir_verify(ctx);                       // no diagnostic
char *ir = ura_ir_to_string(ctx);
// expect: ir contains "ModuleID = 'ura'"
ura_ir_free_string(ir);
ura_ir_dispose(ctx);
```

#### Types are unique, so they compare with ==

```c
UraType *i32 = ura_type_int(ctx, 32, true);
UraType *p1 = ura_type_ptr(ctx, i32);
UraType *p2 = ura_type_ptr(ctx, i32);
// expect: p1 == p2
// expect: ura_type_array(ctx, i32, 3) == ura_type_array(ctx, i32, 3)
// expect: ura_type_array(ctx, i32, 3) != ura_type_array(ctx, i32, 4)
// expect: ura_type_ref(ctx, i32) != ura_type_ptr(ctx, i32)
// expect: strcmp(ura_type_to_string(ctx, ura_type_ref(ctx, i32)), "&i32") == 0
```

#### Sizes come from the target data

```c
// expect: ura_sizeof(ctx, ura_type_int(ctx, 8, true)) == 1
// expect: ura_sizeof(ctx, i32) == 4
// expect: ura_sizeof(ctx, ura_type_int(ctx, 64, true)) == 8
// expect: ura_sizeof(ctx, ura_type_bool(ctx)) == 1
// expect: ura_sizeof(ctx, ura_type_array(ctx, i32, 5)) == 20
```

#### Constants

```c
// expect: ura_const_int_value(ura_const_int(ctx, i32, -7)) == -7
// expect: ura_int_fits(i8, 127) and ura_int_fits(i8, -128)
// expect: !ura_int_fits(i8, 128) and !ura_int_fits(i8, -129)
// expect: ura_value_is_null_const(ura_const_null(ctx, ura_type_ptr(ctx, i8)))
```

#### A function that returns 42

```c
UraType *main_type = ura_type_func(ctx, i32, NULL, 0, false);
UraFunc *main_fn = ura_func_declare(ctx, "main", main_type, URA_EXTERNAL);
ura_func_begin(ctx, main_fn);
ura_codegen_return(ctx, ura_const_int(ctx, i32, 42));
ura_func_end(ctx);
// expect: the IR has "define i32 @main()" and "ret i32 42"
// expect: the executable exits with 42
```

#### A function with no return gets the default one

```c
ura_func_begin(ctx, main_fn);            // main returns i32
ura_func_end(ctx);                       // adds ret i32 0
// expect: the IR has "ret i32 0"; a void function gets "ret void"
```

#### A global variable

```c
UraGlobal *g = ura_global_create(ctx, "counter", i32, ura_const_int(ctx, i32, 7), URA_EXTERNAL);
UraValue *addr = ura_global_address(g);
UraValue *v = ura_load(ctx, addr, "load.counter");
ura_store(ctx, ura_build_add(ctx, v, ura_const_int(ctx, i32, 3), "ADD"), addr);
// inside main: return the loaded value again
// expect: the IR has "@counter = global i32 7"; the executable exits with 10
```

#### A string literal and a variadic call to printf

```c
UraType *printf_type = ura_type_func(ctx, i32, (UraType *[]){ ura_type_ptr(ctx, i8) }, 1, true);
UraFunc *printf_fn = ura_func_declare(ctx, "printf", printf_type, URA_EXTERNAL);
UraValue *fmt = ura_string_literal(ctx, "%d %c\n", 6);
ura_call(ctx, printf_fn, (UraValue *[]){ fmt, ura_const_int(ctx, i32, 42), ura_const_char(ctx, 'x') }, 3, "call");
// the char argument is promoted to int by ura_call, as C does
// expect: the IR has "declare i32 @printf(i8*, ...)" and a global named @str
// expect: the program prints "42 x" and a newline
```

---

## Milestone 0.2: Functions, Variables and Expressions

```c
struct UraFunc {
    char *name, *symbol;                // Ura name, name in the .ll (Type.method, mod.f)
    UraType *type;
    expand(UraValue *, params);
    expand(UraBlock *, blocks);
    UraBlock *entry;
    bool is_proto, is_defined, is_variadic;
    UraLinkage linkage;
    UraStruct *owner;                   // methods
    bool is_method, is_static, is_operator, is_destructor;
    bool can_throw;
    UraValue *value;                    // the function's address
    LLVMValueRef llvm;
};

struct UraBlock { char *name; UraFunc *func; bool ends; LLVMBasicBlockRef llvm; };

typedef enum { URA_EQ, URA_NE, URA_LT, URA_LE, URA_GT, URA_GE } UraCmp;
typedef enum { URA_OP_ADD, URA_OP_SUB, URA_OP_MUL, URA_OP_DIV, URA_OP_REM,
               URA_OP_AND, URA_OP_OR, URA_OP_XOR, URA_OP_SHL, URA_OP_SHR } UraBinOp;
typedef enum { URA_CAST_SEXT, URA_CAST_ZEXT, URA_CAST_TRUNC, URA_CAST_SITOFP, URA_CAST_UITOFP,
               URA_CAST_FPTOSI, URA_CAST_FPTOUI, URA_CAST_FPEXT, URA_CAST_FPTRUNC,
               URA_CAST_BITCAST, URA_CAST_PTRTOINT, URA_CAST_INTTOPTR } UraCast;
```

```c
// functions
UraFunc  *ura_func_declare(UraIRContext *ctx, const char *symbol, UraType *func_type, UraLinkage linkage);
UraFunc  *ura_func_lookup(UraIRContext *ctx, const char *symbol);
void      ura_func_set_param_names(UraFunc *f, const char **names);
UraValue *ura_func_param(UraFunc *f, unsigned index);
UraValue *ura_func_address(UraFunc *f);
UraFunc  *ura_func_begin(UraIRContext *ctx, UraFunc *f);   // entry block, a slot per parameter
void      ura_func_end(UraIRContext *ctx);                  // adds the default return
UraType  *ura_func_ret_type(UraIRContext *ctx);
UraFunc  *ura_func_begin_main(UraIRContext *ctx, bool takes_args);
void      ura_main_prologue(UraIRContext *ctx);             // globals, code outside functions
void      ura_main_epilogue(UraIRContext *ctx, UraValue *exit_code);

// blocks
UraBlock *ura_block_create(UraIRContext *ctx, const char *name);
UraBlock *ura_block_create_after(UraIRContext *ctx, const char *name, UraBlock *after);
void      ura_block_position(UraIRContext *ctx, UraBlock *block);
bool      ura_block_ends(UraBlock *block);
void      ura_block_open_dead(UraIRContext *ctx);

// variables: allocas always go to the entry block
UraValue *ura_var_alloca(UraIRContext *ctx, UraType *type, const char *name);
UraValue *ura_var_declare(UraIRContext *ctx, const char *name, UraType *type, UraValue *init);   // NULL: zero
UraValue *ura_var_declare_ref(UraIRContext *ctx, const char *name, UraType *elem, UraValue *target_address);
UraValue *ura_var_load(UraIRContext *ctx, const char *name);                  // load.NAME
void      ura_var_store(UraIRContext *ctx, const char *name, UraValue *value);
UraValue *ura_load(UraIRContext *ctx, UraValue *address, const char *name);
void      ura_store(UraIRContext *ctx, UraValue *value, UraValue *address);
void      ura_copy(UraIRContext *ctx, UraValue *dest, UraValue *src, UraType *type);   // memcpy for aggregates
void      ura_zero(UraIRContext *ctx, UraValue *address, UraType *type);
void      ura_ref_write(UraIRContext *ctx, UraValue *ref_slot, UraValue *value);      // p = 12
void      ura_ref_rebind(UraIRContext *ctx, UraValue *ref_slot, UraValue *new_target); // p = &y

// expressions
UraValue *ura_build_binop(UraIRContext *ctx, UraBinOp op, UraValue *l, UraValue *r, const char *name);
UraValue *ura_build_add(UraIRContext *ctx, UraValue *l, UraValue *r, const char *name);   // sub mul div rem and or xor shl shr alike
UraValue *ura_build_neg(UraIRContext *ctx, UraValue *v, const char *name);
UraValue *ura_build_bitnot(UraIRContext *ctx, UraValue *v, const char *name);
UraValue *ura_build_cmp(UraIRContext *ctx, UraCmp cmp, UraValue *l, UraValue *r, const char *name);
UraValue *ura_build_not(UraIRContext *ctx, UraValue *v, const char *name);
UraValue *ura_build_logic_and(UraIRContext *ctx, UraValue *l, UraValue *r, const char *name);   // plain and, as recorded
UraValue *ura_build_logic_or(UraIRContext *ctx, UraValue *l, UraValue *r, const char *name);
UraValue *ura_build_cast(UraIRContext *ctx, UraCast cast, UraValue *v, UraType *to, const char *name);
UraValue *ura_build_convert(UraIRContext *ctx, UraValue *v, UraType *to, const char *name);   // picks the cast

// calls
UraValue *ura_call(UraIRContext *ctx, UraFunc *f, UraValue **args, size_t count, const char *name);
UraValue *ura_call_indirect(UraIRContext *ctx, UraValue *fn, UraType *fn_type, UraValue **args, size_t count, const char *name);
void      ura_set_location(UraIRContext *ctx, int line, int column);   // for traps and debug info
```

### Examples that must pass

#### Variables and arithmetic (names as the recorded IR has them)

```c
ura_var_declare(ctx, "x", i32, ura_const_int(ctx, i32, 10));
ura_var_declare(ctx, "y", i32, ura_const_int(ctx, i32, 5));
UraValue *t1 = ura_build_add(ctx, ura_var_load(ctx, "x"), ura_var_load(ctx, "y"), "ADD");   // 15
UraValue *t2 = ura_build_mul(ctx, t1, K(2), "MUL");     // 30      (K(n) is ura_const_int(ctx, i32, n))
UraValue *t3 = ura_build_sub(ctx, t2, K(6), "SUB");      // 24
UraValue *t4 = ura_build_div(ctx, t3, K(4), "DIV");      // 6
UraValue *t5 = ura_build_rem(ctx, t4, K(5), "MOD");      // 1
ura_codegen_return(ctx, t5);
// expect: the IR has "%x = alloca i32, align 4" and "%load.x = load i32, i32* %x, align 4"
// expect: the executable exits with 1
```

#### Signed division rounds toward zero

```c
ura_var_declare(ctx, "n", i32, K(-7));
UraValue *q = ura_build_div(ctx, ura_var_load(ctx, "n"), K(2), "DIV");   // -3
UraValue *r = ura_build_rem(ctx, ura_var_load(ctx, "n"), K(2), "MOD");   // -1, the sign of the dividend
// return q * -10 + r * -1
// expect: the IR has "sdiv i32" and "srem i32"; the executable exits with 31
```

#### Comparisons and logic (plain and, no short-circuit, as recorded)

```c
UraValue *lt = ura_build_cmp(ctx, URA_LT, a, b, "LT");           // a = 3, b = 5 loaded from variables
UraValue *gt = ura_build_cmp(ctx, URA_GT, b, a, "GT");
UraValue *both = ura_build_logic_and(ctx, lt, gt, "AND");        // true
UraValue *none = ura_build_not(ctx, both, "NOT");                // false
UraValue *as_int = ura_build_convert(ctx, both, i32, "conv");    // 1: a bool becomes an int with zext
// expect: the IR has "icmp slt i32", "and i1" and "zext i1"
```

#### Casts

```c
// w is a variable holding 258
UraValue *t = ura_build_convert(ctx, ura_var_load(ctx, "w"), i8, "t");   // trunc: 2
UraValue *back = ura_build_convert(ctx, t, i32, "back");                 // sext: 2
UraValue *neg = ura_build_convert(ctx, ura_const_int(ctx, i8, -1), i32, "neg");   // sext: -1
// expect: the IR has "trunc i32"; back == 2 and neg == -1 at run time
```

#### Calling a function before its definition

```c
UraFunc *add = ura_func_declare(ctx, "add", ura_type_func(ctx, i32, (UraType *[]){ i32, i32 }, 2, false), URA_EXTERNAL);
ura_func_set_param_names(add, (const char *[]){ "a", "b" });
// 1. begin main, which calls add although add has no body yet
ura_codegen_return(ctx, ura_call(ctx, add, (UraValue *[]){ K(40), K(2) }, 2, "call"));
// 2. end main, then begin add and return a + b with ura_var_load(ctx, "a") and "b"
// expect: the IR has "define i32 @add(i32 %0, i32 %1)"; the executable exits with 42
```

#### A function used as a value

```c
UraValue *fp = ura_func_address(twice);                     // twice(x) returns x * 2
UraValue *r = ura_call_indirect(ctx, fp, twice_type, (UraValue *[]){ K(21) }, 1, "r");
// expect: the executable exits with 42
```

#### A scope hides an outer name and gives it back

```c
ura_var_declare(ctx, "x", i32, K(1));
ura_var_declare(ctx, "t", i32, K(0));
ura_scope_push(ctx, URA_SCOPE_BLOCK, NULL);
ura_var_declare(ctx, "x", i32, K(2));                    // shadows
ura_var_store(ctx, "t", ura_var_load(ctx, "x"));        // t = 2
ura_scope_pop(ctx);
// return x * 10 + t
// expect: the executable exits with 12
```

#### Main with a prologue and an epilogue

```c
ura_func_begin_main(ctx, false);
ura_main_prologue(ctx);                                    // global initialisers, code outside functions
ura_store(ctx, K(5), ura_global_address(g));
ura_main_epilogue(ctx, ura_load(ctx, ura_global_address(g), "load.g"));   // destructors of globals, then the return
ura_func_end(ctx);
// expect: the executable exits with 5
```

---

## Milestone 0.3: Control Flow

```c
typedef UraValue *(*UraEmitValueFn)(UraIRContext *ctx, void *user);   // emits an expression
typedef UraValue *(*UraEmitBodyFn)(UraIRContext *ctx, void *user);    // emits a body, may return a value

struct UraIfBranch {
    UraEmitValueFn condition;  void *condition_user;     // NULL condition: the else branch
    UraEmitBodyFn  body;       void *body_user;
    const char    *block_name;                           // default "then"
};

struct UraIfChain {
    UraIfBranch *branches;  size_t count;
    UraType     *result_type;   // NULL: a statement; otherwise the branches yield values merged by a phi
    const char  *result_name;
    bool         has_else;
};

typedef enum { URA_LOOP_WHILE, URA_LOOP_FOR, URA_LOOP_LOOP, URA_LOOP_MATCH } UraLoopKind;
struct UraLoopFrame { UraLoopKind kind; UraBlock *break_block, *continue_block; size_t scope_depth; };

typedef enum { URA_RANGE_AUTO, URA_RANGE_UP, URA_RANGE_DOWN } UraRangeDirection;
struct UraForRange {
    const char *var_name;  UraType *var_type;
    UraValue   *start, *end, *step;          // end excluded; step NULL is 1
    UraRangeDirection direction;             // AUTO goes up when start < end, down otherwise
};

struct UraForEach { const char *var_name; UraValue *array; UraType *elem_type; bool by_ref; const char *index_name; };

struct UraCase  { UraValue **values; size_t value_count; UraEmitBodyFn body; void *body_user; };
struct UraMatch { UraValue *subject; UraCase *cases; size_t case_count;
                  bool has_default; UraEmitBodyFn default_body; void *default_user; };

struct UraCleanup { UraCleanupKind kind; UraValue *value; UraType *type; UraFunc *func; };
struct UraTryFrame { UraBlock *catch_block, *end_block; size_t scope_depth; };
```

```c
// branches
void ura_br(UraIRContext *ctx, UraBlock *target);
void ura_cond_br(UraIRContext *ctx, UraValue *cond, UraBlock *then_block, UraBlock *else_block);
void ura_ret(UraIRContext *ctx, UraValue *value);   void ura_ret_void(UraIRContext *ctx);
void ura_unreachable(UraIRContext *ctx);
UraValue *ura_build_phi(UraIRContext *ctx, UraType *type, UraValue **values, UraBlock **blocks, size_t count, const char *name);
UraValue *ura_build_select(UraIRContext *ctx, UraValue *cond, UraValue *if_true, UraValue *if_false, const char *name);

// if, elif, else: blocks then, next, endif
UraValue *ura_codegen_conditional(UraIRContext *ctx, UraIfChain *chain);
//   the same, step by step:
UraIfBuilder *ura_if_begin(UraIRContext *ctx);
void      ura_if_test(UraIfBuilder *b, UraValue *cond);   void ura_if_elif(UraIfBuilder *b);
void      ura_if_else(UraIfBuilder *b);                   void ura_if_value(UraIfBuilder *b, UraValue *v);
UraValue *ura_if_end(UraIfBuilder *b, UraType *result_type, const char *name);

// loops: blocks cond, then, endwhile
void ura_codegen_while(UraIRContext *ctx, UraEmitValueFn cond, void *cond_user, UraEmitBodyFn body, void *body_user);
void ura_codegen_loop(UraIRContext *ctx, UraEmitBodyFn body, void *body_user);
void ura_codegen_for_range(UraIRContext *ctx, UraForRange *range, UraEmitBodyFn body, void *body_user);
void ura_codegen_for_each(UraIRContext *ctx, UraForEach *each, UraEmitBodyFn body, void *body_user);
void ura_codegen_match(UraIRContext *ctx, UraMatch *match);    // one switch, no fall-through

// leaving: each runs the cleanups (destructors) of the scopes it leaves
void ura_codegen_break(UraIRContext *ctx);
void ura_codegen_continue(UraIRContext *ctx);
void ura_codegen_return(UraIRContext *ctx, UraValue *value);   // value may be NULL
void ura_scope_add_cleanup(UraIRContext *ctx, UraCleanup cleanup);

// errors: a global error slot, checked after every call to a function that can throw
void      ura_try_begin(UraIRContext *ctx, UraTryFrame *frame);
void      ura_try_end_body(UraIRContext *ctx, UraTryFrame *frame);
UraValue *ura_catch_value(UraIRContext *ctx, const char *name);
void      ura_try_end(UraIRContext *ctx, UraTryFrame *frame);
void      ura_codegen_throw(UraIRContext *ctx, UraValue *error);
void      ura_codegen_trap(UraIRContext *ctx, const char *message);
void      ura_codegen_trap_if(UraIRContext *ctx, UraValue *cond, UraTrapKind kind, const char *detail);
```

A block that ends with a terminator is marked ended. A branch from an ended block is skipped, so an `if` body that returns does not jump to `endif`. Code emitted into an ended block goes to a `dead` block.

### Examples that must pass

#### If, elif, else (callbacks emit the conditions and the bodies)

```c
// cond_lt(c, limit): returns ura_build_cmp(c, URA_LT, ura_var_load(c, "n"), K(limit), "LT")
// body_ret(c, value): calls ura_codegen_return(c, K(value)) and returns NULL
UraIfBranch br[3] = {
    { .condition = cond_lt, .condition_user = (void *)10, .body = body_ret, .body_user = (void *)1 },
    { .condition = cond_lt, .condition_user = (void *)20, .body = body_ret, .body_user = (void *)2 },
    { .condition = NULL,                                  .body = body_ret, .body_user = (void *)3 },   // else
};
UraIfChain chain = { .branches = br, .count = 3, .result_type = NULL, .has_else = true };
UraValue *none = ura_codegen_conditional(ctx, &chain);     // a statement chain returns NULL
// grade(5), grade(15), grade(25) give 1, 2, 3
// expect: the IR has the labels "then:", "next:" and "endif:"; 100 * 1 + 10 * 2 + 3 = 123
```

#### An if without else, in a recursive function

```c
UraIfBranch br[1] = { { .condition = cond_n_le_1, .body = body_ret, .body_user = (void *)1 } };
UraIfChain chain = { .branches = br, .count = 1 };
ura_codegen_conditional(ctx, &chain);                      // if n <= 1: return 1
ura_codegen_return(ctx, ura_build_mul(ctx, ura_var_load(ctx, "n"),
                       ura_call(ctx, fact, (UraValue *[]){ ura_build_sub(ctx, ura_var_load(ctx, "n"), K(1), "SUB") }, 1, "call"), "MUL"));
// expect: fact(5) is 120
```

#### An if used as a value, merged by a phi

```c
UraIfBranch br[2] = {
    { .condition = cond_n_gt_0, .body = yield_const, .body_user = (void *)10 },    // yield_const returns K(10)
    { .condition = NULL,        .body = yield_const, .body_user = (void *)20 },
};
UraIfChain chain = { .branches = br, .count = 2, .result_type = i32, .result_name = "pick", .has_else = true };
UraValue *merged = ura_codegen_conditional(ctx, &chain);   // a phi of 10 and 20
ura_codegen_return(ctx, merged);
// expect: the IR has "phi i32"; pick(1) + pick(-1) is 30
```

#### While

```c
ura_var_declare(ctx, "i", i32, K(1));
ura_var_declare(ctx, "sum", i32, K(0));
ura_codegen_while(ctx, cond_i_le_10, NULL, body_add_i_and_increment, NULL);
// expect: the IR has the labels "cond:" and "endwhile:"; sum is 55
```

#### Break and continue inside the body of a loop

```c
// body: i += 1;  if i > 9: break;  if i % 2 == 0: continue;  sum += i
// the two ifs are ura_codegen_conditional calls whose bodies call ura_codegen_break(c) and ura_codegen_continue(c)
ura_codegen_while(ctx, cond_true, NULL, body_odd_sum, NULL);       // cond_true returns ura_const_bool(c, true)
ura_codegen_loop(ctx, body_odd_sum, NULL);                          // the same body in an endless loop
// expect: both give sum 1 + 3 + 5 + 7 + 9 = 25
```

#### A variable declared in a loop body is allocated in the entry block

```c
// body: ura_var_declare(c, "t", i32, K(1));  i += t
// expect: in the IR, "%t = alloca" is between "entry:" and "cond:"
```

#### For over a range, up, down and with a step

```c
UraForRange up   = { .var_name = "i", .var_type = i32, .start = K(0), .end = K(5),  .step = NULL, .direction = URA_RANGE_AUTO };
UraForRange down = { .var_name = "i", .var_type = i32, .start = K(5), .end = K(0),  .step = NULL, .direction = URA_RANGE_AUTO };
UraForRange by2  = { .var_name = "i", .var_type = i32, .start = K(0), .end = K(10), .step = K(2),  .direction = URA_RANGE_UP };
ura_codegen_for_range(ctx, &up, body_add_i_to_sum, NULL);          // 0 1 2 3 4 = 10
ura_codegen_for_range(ctx, &down, body_add_i_to_sum, NULL);        // 5 4 3 2 1 = 15, the end is excluded
ura_codegen_for_range(ctx, &by2, body_add_i_to_sum, NULL);         // 0 2 4 6 8 = 20
// expect: sum is 45
```

#### Match on integers, two values sharing a case, and a default

```c
UraValue *one[1] = { K(1) };
UraValue *two_three[2] = { K(2), K(3) };
UraCase cases[2] = {
    { .values = one,       .value_count = 1, .body = body_ret, .body_user = (void *)10 },
    { .values = two_three, .value_count = 2, .body = body_ret, .body_user = (void *)20 },
};
UraMatch m = { .subject = ura_var_load(ctx, "n"), .cases = cases, .case_count = 2, .has_default = true,
               .default_body = body_ret, .default_user = (void *)30 };
ura_codegen_match(ctx, &m);
// expect: the IR has "switch i32"; f(1) + f(3) + f(9) is 10 + 20 + 30 = 60
```

#### Match on an enum

```c
UraType *color = ura_enum_define(ctx, "Color", (const char *[]){ "RED", "GREEN", "BLUE" }, (long long[]){ 0, 1, 2 }, 3);
// expect: ura_const_int_value(ura_enum_variant(ctx, color, "BLUE")) == 2
// expect: strcmp(ura_enum_variant_name(color, 1), "GREEN") == 0
UraValue *red[1] = { ura_enum_variant(ctx, color, "RED") };   // a case on RED returns 10, GREEN returns 20, default 30
// expect: f(GREEN) + f(BLUE) is 20 + 30 = 50
```

#### Code written after a return goes to a dead block

```c
ura_codegen_return(ctx, K(3));
ura_var_declare(ctx, "z", i32, K(9));                      // nothing reaches this
ura_func_end(ctx);
// expect: the module verifies; the IR has the label "dead:"; the executable exits with 3
```

---

## Milestone 0.4: Memory, Arrays, Strings and References

```c
// addresses
UraValue *ura_build_gep_index(UraIRContext *ctx, UraValue *base, UraValue *index, const char *name);
UraValue *ura_build_gep_field(UraIRContext *ctx, UraValue *struct_address, unsigned field, const char *name);
UraValue *ura_build_gep_chain(UraIRContext *ctx, UraValue *base, unsigned *indices, size_t count, const char *name);
UraValue *ura_address_of(UraIRContext *ctx, UraValue *variable);                // &x
UraValue *ura_deref(UraIRContext *ctx, UraValue *pointer, const char *name);    // dref.NAME
UraValue *ura_build_is_null(UraIRContext *ctx, UraValue *pointer, const char *name);
UraValue *ura_build_is_not_null(UraIRContext *ctx, UraValue *pointer, const char *name);

// arrays: the literal returns the element pointer
UraValue *ura_array_literal(UraIRContext *ctx, UraType *elem, UraValue **values, size_t count, const char *name);
UraValue *ura_array_alloca(UraIRContext *ctx, UraType *elem, size_t length, const char *name);
UraValue *ura_array_alloca_dynamic(UraIRContext *ctx, UraType *elem, UraValue *length, const char *name);
UraValue *ura_array_element(UraIRContext *ctx, UraValue *array_pointer, UraValue *index, const char *name);
UraValue *ura_array_len(UraIRContext *ctx, UraValue *array, const char *name);
UraValue *ura_string_to_array(UraIRContext *ctx, UraValue *dest_array, const char *bytes, size_t length);   // memcpy

// slices, from milestone 8: a fat pointer {T *data, i64 length}
UraValue *ura_slice_make(UraIRContext *ctx, UraValue *data, UraValue *length, const char *name);
UraValue *ura_slice_from_array(UraIRContext *ctx, UraValue *array_address, const char *name);
UraValue *ura_slice_range(UraIRContext *ctx, UraValue *source, UraValue *start, UraValue *end, bool checked, const char *name);

// heap
UraValue *ura_heap_alloc(UraIRContext *ctx, UraType *type, UraValue *count, const char *name);   // calloc
UraValue *ura_heap_alloc_dynamic(UraIRContext *ctx, UraType *elem, UraValue *length, const char *name);
void      ura_heap_free(UraIRContext *ctx, UraValue *pointer);
void      ura_heap_clean(UraIRContext *ctx, UraValue *slot, UraType *type);                      // free, then store null

void ura_memcpy(UraIRContext *ctx, UraValue *dest, UraValue *src, UraValue *size, unsigned align);
void ura_memmove(UraIRContext *ctx, UraValue *dest, UraValue *src, UraValue *size, unsigned align);
void ura_memset(UraIRContext *ctx, UraValue *dest, UraValue *byte, UraValue *size, unsigned align);
```

A reference is a pointer kept in a slot, and every reference can be null (`next &Node` starts null). There is no guard operator and no fallback operator: a null check is `ura_build_is_null`, and with `URA_FLAG_TRAP_NULL_DEREF` on every dereference is checked first.

### Examples that must pass

#### An array literal, element access and write

```c
UraValue *arr = ura_array_literal(ctx, i32, (UraValue *[]){ K(4), K(5), K(6) }, 3, "array");
ura_store(ctx, K(9), ura_array_element(ctx, arr, K(1), "a[1]"));                      // a[1] = 9
UraValue *v0 = ura_load(ctx, ura_array_element(ctx, arr, K(0), "a[0]"), "load.a[0]");
UraValue *v1 = ura_load(ctx, ura_array_element(ctx, arr, K(1), "a[1]"), "load.a[1]");
UraValue *v2 = ura_load(ctx, ura_array_element(ctx, arr, K(2), "a[2]"), "load.a[2]");
// return v0 + v1 + v2
// expect: the IR has "alloca [3 x i32]"; the executable exits with 4 + 9 + 6 = 19
```

#### Heap memory is zeroed

```c
UraValue *p = ura_heap_alloc(ctx, i32, NULL, "p");           // calloc of one i32
UraValue *zero = ura_load(ctx, p, "zero");
ura_store(ctx, K(7), p);
UraValue *seven = ura_load(ctx, p, "seven");
ura_heap_free(ctx, p);
// return seven + zero
// expect: the IR calls @calloc; the executable exits with 7
```

#### A reference parameter changes the caller

```c
UraFunc *inc = ura_func_declare(ctx, "inc", ura_type_func(ctx, v, (UraType *[]){ ura_type_ref(ctx, i32) }, 1, false), URA_EXTERNAL);
ura_func_set_param_names(inc, (const char *[]){ "c" });
// body of inc:
UraValue *ptr = ura_var_load(ctx, "c");                    // the pointer kept in the parameter's slot
UraValue *old = ura_deref(ctx, ptr, "old");
ura_ref_write(ctx, ura_symbol_lookup(ctx, "c")->value, ura_build_add(ctx, old, K(1), "ADD_ASSIGN"));
// in main, three times:
ura_call(ctx, inc, (UraValue *[]){ ura_address_of(ctx, n) }, 1, NULL);        // n is a variable holding 0
// expect: the IR has "define void @inc(i32* %0)" and "%c = alloca i32*, align 8"
// expect: the executable exits with 3
```

#### Write through a reference, rebind it, write again

```c
UraValue *p = ura_var_declare_ref(ctx, "p", i32, ura_address_of(ctx, a));   // p &i32 = &a   (a = 1, b = 2)
ura_ref_write(ctx, p, K(10));                              // p = 10  writes a
ura_ref_rebind(ctx, p, ura_address_of(ctx, b));            // p = &b
ura_ref_write(ctx, p, K(20));                              // writes b
// expect: a + b is 30
```

#### A reference that is null

```c
UraValue *q = ura_var_declare_ref(ctx, "q", i32, ura_const_null(ctx, ura_type_ptr(ctx, i32)));   // q &i32 starts as null
UraValue *was_null = ura_build_convert(ctx, ura_build_is_null(ctx, ura_var_load(ctx, "q"), "isnull"), i32, "conv");
ura_ref_rebind(ctx, q, ura_address_of(ctx, a));
UraValue *now_set = ura_build_convert(ctx, ura_build_is_not_null(ctx, ura_var_load(ctx, "q"), "isset"), i32, "conv");
// return was_null * 10 + now_set
// expect: the executable exits with 11
```

---

## Milestone 0.5: Structs, Fields, Methods, Templates, Enums, Modules

```c
struct UraFieldSpec { const char *name; UraType *type; bool is_ref; UraValue *default_value; };

struct UraStruct {
    char *name;               // "Point", "Box<i32>"
    char *symbol;             // name of the LLVM type
    UraType *type;
    expand(UraField, fields);
    expand(UraMethod *, methods);
    expand(UraMethod *, operators);
    UraFunc *destructor, *copy_assign, *output_fn;
    UraStruct *template_of;   expand(UraType *, template_args);
    bool is_defined;
    size_t size, align;
    expand(unsigned, owns);   // fields with a destructor, dropped in reverse order
};

typedef enum {
    URA_OP_NONE,
    URA_OP_ADD, URA_OP_SUB, URA_OP_MUL, URA_OP_DIV, URA_OP_MOD,
    URA_OP_EQ, URA_OP_NE, URA_OP_LT, URA_OP_GT, URA_OP_LE, URA_OP_GE,
    URA_OP_ADD_ASSIGN, URA_OP_SUB_ASSIGN, URA_OP_MUL_ASSIGN, URA_OP_DIV_ASSIGN, URA_OP_MOD_ASSIGN,
    URA_OP_AND, URA_OP_OR, URA_OP_XOR, URA_OP_SHL, URA_OP_SHR,
    URA_OP_ASSIGN, URA_OP_INDEX, URA_OP_OUTPUT, URA_OP_DROP,
} UraOperatorKind;

struct UraMethod {
    char *name; UraStruct *owner; UraFunc *func;
    UraOperatorKind op;
    bool is_static;           // pub fn: no self, called Type::method()
    bool returns_ref, is_destructor;
};

struct UraTemplateKey { const char *template_name; UraType **args; size_t count; };
```

```c
// a struct in two steps, so it can be used before its declaration
UraStruct *ura_struct_declare(UraIRContext *ctx, const char *name);
bool       ura_struct_define(UraIRContext *ctx, UraStruct *s, const UraFieldSpec *fields, size_t count);
int        ura_struct_field_index(UraStruct *s, const char *field);        // -1 when missing

// fields
UraValue *ura_field_address(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, const char *name);
UraValue *ura_field_load(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, const char *name);
void      ura_field_store(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, UraValue *value);
UraValue *ura_field_chain_address(UraIRContext *ctx, UraValue *base, UraStruct *s, const char **fields, size_t count, const char *name);
void      ura_struct_zero(UraIRContext *ctx, UraValue *address, UraStruct *s);
void      ura_struct_copy(UraIRContext *ctx, UraValue *dest, UraValue *src, UraStruct *s);

// methods: the receiver is a pointer named self, added in front of func_type
UraMethod *ura_method_declare(UraIRContext *ctx, UraStruct *s, const char *name, UraType *func_type,
                              UraOperatorKind op, bool is_static);
void       ura_method_begin(UraIRContext *ctx, UraMethod *m);
void       ura_method_end(UraIRContext *ctx);
UraMethod *ura_method_lookup(UraStruct *s, const char *name, UraType **arg_types, size_t count, UraOperatorKind op);
UraValue  *ura_call_method(UraIRContext *ctx, UraValue *self_address, UraMethod *m, UraValue **args, size_t count, const char *name);
UraValue  *ura_call_static(UraIRContext *ctx, UraMethod *m, UraValue **args, size_t count, const char *name);
UraValue  *ura_call_operator(UraIRContext *ctx, UraOperatorKind op, UraValue *left_address, UraValue *right, const char *name);

// destructors
UraFunc *ura_struct_destructor(UraIRContext *ctx, UraStruct *s);   // the user one, a generated one, or NULL
void     ura_struct_register_cleanup(UraIRContext *ctx, UraValue *variable, UraStruct *s);

// templates: one instance per set of arguments
UraStruct *ura_template_instance(UraIRContext *ctx, const UraTemplateKey *key, bool *created);
UraFunc   *ura_func_instance(UraIRContext *ctx, const UraTemplateKey *key, UraType *func_type, bool *created);

// enums and modules
UraType  *ura_enum_define(UraIRContext *ctx, const char *name, const char **variants, long long *values, size_t count);
UraValue *ura_enum_variant(UraIRContext *ctx, UraType *enum_type, const char *variant);
const char *ura_enum_variant_name(UraType *enum_type, long long value);
UraScope *ura_module_open(UraIRContext *ctx, const char *name);   void ura_module_close(UraIRContext *ctx);
char     *ura_qualify(UraIRContext *ctx, const char *name);
```

As recorded, a struct passes and returns by value as an LLVM aggregate (`define i32 @bump(%Point %0)`, `define %User @make(i32 %0)`), and the receiver of a method is a pointer (`define i32 @User.get(%User* %0)`). Symbols read `Type.method`, `module.name`, `Box<i32>.method`. The destructor of a struct also drops the fields it owns, in reverse order; a reference field is borrowed and never dropped. Pass pointers to C, not structs.

### Examples that must pass

#### A struct: declare, define, zero value, fields

```c
UraStruct *pt = ura_struct_declare(ctx, "Point");
UraFieldSpec f[2] = { { .name = "x", .type = i32 }, { .name = "y", .type = i32 } };
// expect: ura_struct_define(ctx, pt, f, 2) is true; pt->size == 8; pt->align == 4
// expect: ura_struct_field_index(pt, "y") == 1 and ura_struct_field_index(pt, "nope") == -1
UraValue *p = ura_var_declare(ctx, "p", pt->type, NULL);        // zeroed: store %Point zeroinitializer
ura_field_store(ctx, p, pt, "x", K(3));
ura_field_store(ctx, p, pt, "y", K(4));
// return ura_field_load(ctx, p, pt, "x", "p.x") + ura_field_load(ctx, p, pt, "y", "p.y")
// expect: the IR has "%Point = type { i32, i32 }"; the executable exits with 7
```

#### Padding follows the target ABI

```c
UraFieldSpec f[2] = { { .name = "a", .type = i8 }, { .name = "b", .type = i32 } };
ura_struct_define(ctx, pad, f, 2);
// expect: pad->size == 8 and pad->align == 4
// expect: ura_offsetof(ctx, pad->type, 1) == 4
```

#### A struct used before its declaration, with a reference to itself

```c
UraStruct *node = ura_struct_declare(ctx, "Node");           // opaque for now
// expect: !ura_type_is_complete(node->type)
UraFieldSpec f[2] = {
    { .name = "value", .type = i32 },
    { .name = "next",  .type = ura_type_ref(ctx, node->type), .is_ref = true },   // next &Node
};
// expect: ura_struct_define(ctx, node, f, 2) is true; node->size == 2 * sizeof(void *)
```

#### A struct cannot contain itself by value

```c
UraStruct *bad = ura_struct_declare(ctx, "Bad");
UraFieldSpec f[1] = { { .name = "me", .type = bad->type } };
// expect: ura_struct_define(ctx, bad, f, 1) is false, and one diagnostic of level ERROR was reported
```

#### A field of a nested struct, in one call

```c
const char *path[2] = { "inner", "y" };
UraValue *y = ura_field_chain_address(ctx, o, outer, path, 2, "o.inner.y");
ura_store(ctx, K(9), y);
// expect: loading the same chain gives 9
```

#### Structs pass and return by value, as recorded

```c
// make(a) returns Point { a, a + 1 }:  RET(ura_load(ctx, p, "load.p"))
// sum(p Point) returns p.x + p.y, p is a parameter: ura_symbol_lookup(ctx, "p")->value is its slot
ura_codegen_return(ctx, ura_call(ctx, sum, (UraValue *[]){ ura_call(ctx, make, (UraValue *[]){ K(5) }, 1, "call") }, 1, "call"));
// expect: the IR has "define %Point @make(i32 %0)" and "define i32 @sum(%Point %0)"
// expect: the executable exits with 5 + 6 = 11
```

#### Copying a struct

```c
ura_copy(ctx, b, a, pt->type);                             // memcpy
ura_field_store(ctx, a, pt, "x", K(100));                 // b must not follow
// expect: the IR has "llvm.memcpy"; b.x + b.y is 1 + 2 = 3 when a was { 1, 2 }
```

#### Methods, declared and emitted in any order

```c
UraStruct *cn = ura_struct_declare(ctx, "Counter");   // one field: n i32
UraMethod *inc = ura_method_declare(ctx, cn, "inc", ura_type_func(ctx, v, NULL, 0, false), URA_OP_NONE, false);
UraMethod *get = ura_method_declare(ctx, cn, "get", ura_type_func(ctx, i32, NULL, 0, false), URA_OP_NONE, false);
ura_method_begin(ctx, get);                                 // get is emitted before inc
ura_codegen_return(ctx, ura_field_load(ctx, ura_var_load(ctx, "self"), cn, "n", "self.n"));
ura_method_end(ctx);
// in main, with c a Counter variable: ura_call_method(ctx, c, inc, NULL, 0, NULL) twice, then return ura_call_method(ctx, c, get, NULL, 0, "get")
// expect: the IR has "define void @Counter.inc(%Counter* %0)" and "define i32 @Counter.get(%Counter* %0)"
// expect: the executable exits with 2
```

#### A static method (pub fn)

```c
UraMethod *twice = ura_method_declare(ctx, cn, "twice", ura_type_func(ctx, i32, (UraType *[]){ i32 }, 1, false), URA_OP_NONE, true);
// body: return x * 2        (the parameter is named x with ura_func_set_param_names(twice->func, ...))
ura_codegen_return(ctx, ura_call_static(ctx, twice, (UraValue *[]){ K(21) }, 1, "r"));   // Counter::twice(21)
// expect: the IR has "define i32 @Counter.twice(i32 %0)"; the executable exits with 42
```

#### Destructors run at the end of the scope, in reverse order

```c
// struct S has one field id; its drop method does: counter = counter * 10 + self.id   (counter is a global)
UraMethod *drop = ura_method_declare(ctx, s, "drop", ura_type_func(ctx, v, NULL, 0, false), URA_OP_DROP, false);
ura_scope_push(ctx, URA_SCOPE_BLOCK, NULL);
UraValue *a = ura_var_declare(ctx, "a", s->type, NULL);  ura_field_store(ctx, a, s, "id", K(1));  ura_struct_register_cleanup(ctx, a, s);
UraValue *b = ura_var_declare(ctx, "b", s->type, NULL);  ura_field_store(ctx, b, s, "id", K(2));  ura_struct_register_cleanup(ctx, b, s);
ura_scope_pop(ctx);                                         // drops b, then a
// expect: ura_struct_destructor(ctx, s) != NULL; counter is (0 * 10 + 2) * 10 + 1 = 21
```

#### One instance per set of template arguments

```c
UraTemplateKey key = { .template_name = "Box", .args = (UraType *[]){ i32 }, .count = 1 };
bool created;
UraStruct *a = ura_template_instance(ctx, &key, &created);   // created is true
UraStruct *b = ura_template_instance(ctx, &key, &created);   // created is false
// expect: a == b and strcmp(a->name, "Box<i32>") == 0
// expect: the instance for i8 is another struct named "Box<i8>"
```

---

## Milestone 0.6: Runtime Support, Builtins, C Interop, Debug Info

```c
typedef enum { URA_TRAP_BOUNDS, URA_TRAP_NULL, URA_TRAP_DIV_ZERO, URA_TRAP_NULL_CALL,
               URA_TRAP_UNCAUGHT, URA_TRAP_OOM, URA_TRAP_CUSTOM } UraTrapKind;
typedef enum { URA_STDOUT, URA_STDERR } UraPrintTarget;

UraFunc *ura_runtime_declare(UraIRContext *ctx, const char *name);   // printf dprintf calloc free write exit strlen getenv ...
UraFunc *ura_trap_helper(UraIRContext *ctx, UraTrapKind kind);       // noreturn: message to stderr, exit 1

// output and errput: %d i32, %ld i64, %u unsigned, %f floats, %s strings, %c chars,
// True and False for bool, the variant name for an enum, structs and arrays through a helper
void     ura_codegen_print(UraIRContext *ctx, UraValue *value, UraType *type, UraPrintTarget target);
void     ura_codegen_print_string(UraIRContext *ctx, const char *text, UraPrintTarget target);
UraFunc *ura_print_helper(UraIRContext *ctx, UraType *type, UraPrintTarget target);

UraValue *ura_build_sizeof(UraIRContext *ctx, UraType *type, UraType *result_type);   // a constant
void      ura_ir_add_link(UraIRContext *ctx, const char *library);                    // link "name"

void ura_debug_enable(UraIRContext *ctx, const char *filename, const char *directory, bool full);
void ura_debug_finalize(UraIRContext *ctx);
```

A trap message reads `source:line: text`. Division by zero (floats too) traps with `URA_FLAG_CHECK_DIV_ZERO`.

### Examples that must pass

#### Output emitters

```c
ura_codegen_print(ctx, K(42), i32, URA_STDOUT);
ura_codegen_print_string(ctx, " ", URA_STDOUT);
ura_codegen_print(ctx, ura_const_bool(ctx, true), ura_type_bool(ctx), URA_STDOUT);
ura_codegen_print_string(ctx, "\n", URA_STDOUT);
// expect: the program prints "42 True" and a newline
```

#### Sizeof is a constant

```c
UraValue *size = ura_build_sizeof(ctx, pt->type, i32);       // Point is two i32
// expect: the value is 8; for an i64 it is 8 as well
```

#### Division by zero traps when the check is on

```c
ura_ir_set_flag(ctx, URA_FLAG_CHECK_DIV_ZERO, true);
ura_set_location(ctx, 3, 12);                               // line and column for the message
ura_codegen_return(ctx, ura_build_div(ctx, K(10), ura_var_load(ctx, "d"), "DIV"));   // d is a variable holding 0
// expect: the executable exits with 1 and writes a message to stderr
// with d = 4 and K(20): the executable exits with 5
```

---

## Milestone 0.7: Optimizer Hooks and Backend

```c
typedef enum { URA_O0, URA_O1, URA_O2, URA_O3, URA_OS, URA_OZ } UraOptLevel;

struct UraOptimizer {
    UraOptLevel level;
    unsigned size_level, inline_threshold;
    bool unroll_loops, vectorize_loops, vectorize_slp, verify_each;
    LLVMPassManagerRef function_pm, module_pm;          // LLVM 12: legacy pass managers
    LLVMPassManagerBuilderRef builder;
    LLVMPassBuilderOptionsRef pass_builder_options;     // LLVM 13 and later
    char *pipeline;                                     // "default<O2>"
    expand(UraPassSpec, custom_passes);
    bool initialized;
};

struct UraPassSpec { const char *name; UraPassStage stage; bool after_default; };   // mem2reg, gvn, instcombine, ...

struct UraLinkOptions {
    const char **libraries, **library_paths, **extra_flags;
    bool sanitize, debug, verbose;
};
```

```c
void ura_opt_set_level(UraIRContext *ctx, UraOptLevel level);
void ura_opt_set_option(UraIRContext *ctx, UraOptOption option, long value);
void ura_opt_add_pass(UraIRContext *ctx, const char *name, UraPassStage stage, bool after_default);
bool ura_opt_init(UraIRContext *ctx);
bool ura_opt_run_function(UraIRContext *ctx, UraFunc *f);
bool ura_opt_run(UraIRContext *ctx);       // level 0 returns at once and changes nothing

bool ura_emit_object(UraIRContext *ctx, const char *path);
bool ura_emit_assembly(UraIRContext *ctx, const char *path);
bool ura_build_executable(UraIRContext *ctx, const char *exe_path, const UraLinkOptions *options);   // NULL: defaults
int  ura_run_executable(const char *exe_path, char **args);
```

### Examples that must pass

#### The optimizer at level 0 changes nothing

```c
char *before = ura_ir_to_string(ctx);
ura_opt_run(ctx);                                           // returns true at once
char *after = ura_ir_to_string(ctx);
// expect: strcmp(before, after) == 0, and the IR still has "alloca"
```

#### The optimizer at level 2 removes the stack slots

```c
ura_opt_set_level(ctx, URA_O2);
// expect: ura_opt_run(ctx) is true; the IR no longer has "alloca"
// expect: the executable gives the same exit status as at level 0
```

#### Build and run an executable

```c
ura_ir_emit_ll(ctx, "build/out.ll");
ura_build_executable(ctx, "build/out", NULL);              // NULL options: the defaults
// expect: ura_run_executable("build/out", NULL) returns the exit status of main
```

---

## Which wrapper part each language milestone needs

| Language milestone | Wrapper |
|--------------------|---------|
| 1 programs that compute | 0.1, 0.2, `ura_codegen_conditional`, `while`, `break`, `continue`, `return`, string literals |
| 2 memory and data | arrays, references, casts, null (0.4) |
| 3 structs, 4 templates | 0.5 |
| 5 modules | `ura_module_open`, links, `os` |
| 6 output and strings | print emitters (0.6) |
| 7 enums and match | `ura_enum_define`, `ura_codegen_match` |
| 8 heap and drop | heap, cleanups, slices, bounds checks |
| 9 operators | `ura_call_operator`, `ura_call_static` |
| 10 loops | `ura_codegen_loop`, `for_range`, `for_each` |
| 11 more types | unsigned, floats, shifts, division trap |
| 12 null references and errors | `ura_build_is_null`, try and throw |
| 13 function values | `ura_func_address`, `ura_call_indirect` |

0.7 is independent. The executable build is needed from milestone 1.

## Open decisions

- D1 symbol separator: `Type.method` and `module.name` proposed. Decide before the first method is emitted.
- D2 short-circuit `and`, `or`: the recorded IR has none (plain `and`, `or`).
- D3 array parameters: bare pointers now, the fat pointer `{T*, i64}` with slices in milestone 8.
- D5 `try`: an error slot (proposed) or LLVM exception handling.
- D6 bounds and division checks: off by default, decide later whether they become the default.
- D7 integer overflow wraps; a checked mode later.
- D9 no deferred IR for now; the API would not change if it came later.

## Done when

- `grep llvm-c` finds only `src/ir.c`, and `main.c` has no identifier starting with `LLVM`.
- The IR of milestones 1 to 4 equals the recorded IR and verifies.
- `src/proto.h` lists the `ura_` functions.
