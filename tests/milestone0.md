# Milestone 0: LLVM Feature Wrapper

Specification of src/ir.c. No test entries (the runner skips this file). Layouts and signatures only, no implementation.

## Rules

+ Only ir.c includes llvm-c. The core holds opaque pointers: UraIRContext, UraType, UraValue, UraBlock, UraFunc, UraGlobal, UraStruct.
+ Immediate builder: every ura_ call emits its LLVM instruction at once and returns a handle.
+ Every function starts with ura_ and takes the context first. Objects come from ura_alloc, lists from expand and push_back.
+ The core passes the value names (load.x, dref.p, ADD); the wrapper keeps them as given.
+ The recorded IR of milestones 1 to 4 is the reference. LLVM 12 (typed pointers).
+ Checks, traps and optimisation are off by default, because the recorded IR has none.

### Names the generated IR keeps

+ Blocks: entry, then, next (following test or else body), endif, cond, endwhile, dead (code after a terminator).
+ Values: NAME (variable), load.NAME, dref.NAME, OBJ.FIELD, ADD SUB MUL DIV MOD EQ NQ LT GT LE GE AND OR NOT, ADD_ASSIGN ...
+ Helpers: str, str.1 (string globals), array (backing storage), e, e1 (element addresses), dst (memcpy destination).
+ Module: ModuleID and source_filename are ura. Structs are named after the struct, instances Box<i32>.

## Milestone 0.1: Context, Types, Constants and Globals

### Layout

+ struct UraIRContext: llvm_context, llvm_module, llvm_builder, target_data, target_machine, triple, types, cursor, scopes, globals, functions, structs, optimizer, debug, diag, diag_user, flags, runtime, error_slot
+ enum UraDiagLevel: URA_DIAG_NOTE, URA_DIAG_WARNING, URA_DIAG_ERROR, URA_DIAG_FATAL
+ typedef void (*UraDiagFn)(void *user, UraDiagLevel level, const char *message)
+ enum UraIRFlag: URA_FLAG_VERIFY_MODULE, URA_FLAG_KEEP_NAMES, URA_FLAG_DEBUG_INFO, URA_FLAG_SANITIZE, URA_FLAG_BOUNDS_CHECKS, URA_FLAG_CHECK_DIV_ZERO, URA_FLAG_TRAP_NULL_DEREF
+ struct UraCursor: func, block, entry, alloca_end, loops, scope_depth, saved
+ enum UraTypeKind: URA_VOID, URA_BOOL, URA_I8, URA_I16, URA_I32, URA_I64, URA_U8, URA_U16, URA_U32, URA_U64, URA_F32, URA_F64, URA_PTR, URA_REF, URA_ARRAY, URA_SLICE, URA_STRUCT, URA_FUNC, URA_TUPLE, URA_ENUM, URA_OPAQUE
+ struct UraType: kind, name, bits, is_signed, elem, length, fields, ret, params, is_variadic, struct_info, template_args, size, align, is_complete, llvm
+ struct UraField: name, type, index, offset, is_ref, default_value
+ struct UraValue: kind (CONST ZERO GLOBAL PARAM FUNC INST ALLOCA), type, is_address, name, llvm
+ struct UraGlobal: name, type, initializer, is_constant, linkage, alignment, value, llvm
+ enum UraLinkage: URA_EXTERNAL, URA_INTERNAL, URA_PRIVATE, URA_EXTERN_DECL
+ struct UraSymbol: name, kind, type, value, is_ref, is_param, depth, owner
+ struct UraScope: parent, symbols, kind (GLOBAL MODULE FUNC BLOCK LOOP STRUCT), cleanups, owner_name

### Signatures

+ UraIRContext *ura_ir_create(const char *module_name, const char *source_name, UraDiagFn diag, void *user)
+ void ura_ir_set_target(UraIRContext *ctx, const char *triple, const char *cpu, const char *features, UraRelocMode reloc, UraCodeModel model)
+ void ura_ir_set_flag(UraIRContext *ctx, UraIRFlag flag, bool value)
+ void ura_ir_verify(UraIRContext *ctx);  void ura_ir_dispose(UraIRContext *ctx);  void ura_ir_reset(UraIRContext *ctx)
+ void ura_cursor_push(UraIRContext *ctx);  void ura_cursor_pop(UraIRContext *ctx)
+ UraType *ura_type_void(ctx);  ura_type_bool(ctx);  ura_type_int(ctx, unsigned bits, bool is_signed);  ura_type_float(ctx, unsigned bits)
+ UraType *ura_type_ptr(ctx, UraType *elem);  ura_type_ref(ctx, elem);  ura_type_array(ctx, elem, size_t length);  ura_type_slice(ctx, elem)
+ UraType *ura_type_func(ctx, UraType *ret, UraType **params, size_t count, bool variadic);  ura_type_tuple(ctx, elems, count);  ura_type_enum(ctx, name)
+ UraType *ura_type_struct_declare(ctx, name);  bool ura_type_struct_define(ctx, type, UraType **field_types, const char **field_names, size_t count)
+ queries: ura_type_kind, ura_type_elem, ura_type_bits, ura_type_is_integer, ura_type_is_signed, ura_type_is_float, ura_type_is_complete, ura_type_field(type, i), ura_type_param(type, i), ura_type_ret
+ size_t ura_sizeof(ctx, type);  ura_alignof(ctx, type);  ura_offsetof(ctx, struct_type, unsigned field);  char *ura_type_to_string(ctx, type)
+ UraValue *ura_const_int(ctx, type, long long);  ura_const_uint;  ura_const_float(ctx, type, double);  ura_const_bool(ctx, bool);  ura_const_char(ctx, int)
+ UraValue *ura_const_null(ctx, type);  ura_const_zero(ctx, type);  ura_const_enum(ctx, enum_type, value);  ura_const_array(ctx, elem, values, count);  ura_const_struct(ctx, type, values, count)
+ long long ura_const_int_value(UraValue *);  bool ura_int_fits(UraType *, long long);  bool ura_value_is_const(UraValue *);  bool ura_value_is_null_const(UraValue *)
+ UraType *ura_value_type(UraValue *);  UraType *ura_value_pointee(UraValue *)
+ UraGlobal *ura_global_create(ctx, name, type, UraValue *init, UraLinkage);  ura_global_lookup(ctx, name);  UraValue *ura_global_address(UraGlobal *)
+ UraValue *ura_string_literal(ctx, const char *bytes, size_t length)
+ char *ura_ir_to_string(ctx);  void ura_ir_free_string(char *);  bool ura_ir_emit_ll(ctx, path);  bool ura_ir_emit_bitcode(ctx, path)
+ UraScope *ura_scope_push(ctx, UraScopeKind, const char *owner_name);  void ura_scope_pop(ctx)
+ UraSymbol *ura_symbol_declare(ctx, name, UraSymbolKind, type, value);  ura_symbol_lookup(ctx, name);  ura_symbol_lookup_qualified(ctx, const char **path, size_t count)

### Representation

+ bool is i1 everywhere, char is i8, the untyped pointer is i8*, unsigned types share the LLVM integer and only change the instruction chosen.
+ An array variable is a slot holding the pointer to element 0; the elements live in a separate backing array named array. A parameter is the bare element pointer.
+ Struct types are created by name first (opaque), so a struct can be used before its declaration and can hold &itself.

### Examples that must pass

+ Example: create a context and read the module header
    - UraIRContext *ctx = ura_ir_create("ura", "test.ura", on_diag, NULL);
    - ura_ir_verify(ctx);                       // no diagnostic
    - char *ir = ura_ir_to_string(ctx);
    - // expect: ir contains "ModuleID = 'ura'"
    - ura_ir_free_string(ir);
    - ura_ir_dispose(ctx);

+ Example: types are unique, so they compare with ==
    - UraType *i32 = ura_type_int(ctx, 32, true);
    - UraType *p1 = ura_type_ptr(ctx, i32);
    - UraType *p2 = ura_type_ptr(ctx, i32);
    - // expect: p1 == p2
    - // expect: ura_type_array(ctx, i32, 3) == ura_type_array(ctx, i32, 3)
    - // expect: ura_type_array(ctx, i32, 3) != ura_type_array(ctx, i32, 4)
    - // expect: ura_type_ref(ctx, i32) != ura_type_ptr(ctx, i32)
    - // expect: strcmp(ura_type_to_string(ctx, ura_type_ref(ctx, i32)), "&i32") == 0

+ Example: sizes come from the target data
    - // expect: ura_sizeof(ctx, ura_type_int(ctx, 8, true)) == 1
    - // expect: ura_sizeof(ctx, i32) == 4
    - // expect: ura_sizeof(ctx, ura_type_int(ctx, 64, true)) == 8
    - // expect: ura_sizeof(ctx, ura_type_bool(ctx)) == 1
    - // expect: ura_sizeof(ctx, ura_type_array(ctx, i32, 5)) == 20

+ Example: constants
    - // expect: ura_const_int_value(ura_const_int(ctx, i32, -7)) == -7
    - // expect: ura_int_fits(i8, 127) and ura_int_fits(i8, -128)
    - // expect: !ura_int_fits(i8, 128) and !ura_int_fits(i8, -129)
    - // expect: ura_value_is_null_const(ura_const_null(ctx, ura_type_ptr(ctx, i8)))

+ Example: a function that returns 42
    - UraType *main_type = ura_type_func(ctx, i32, NULL, 0, false);
    - UraFunc *main_fn = ura_func_declare(ctx, "main", main_type, URA_EXTERNAL);
    - ura_func_begin(ctx, main_fn);
    - ura_codegen_return(ctx, ura_const_int(ctx, i32, 42));
    - ura_func_end(ctx);
    - // expect: the IR has "define i32 @main()" and "ret i32 42"
    - // expect: the executable exits with 42

+ Example: a function with no return gets the default one
    - ura_func_begin(ctx, main_fn);            // main returns i32
    - ura_func_end(ctx);                       // adds ret i32 0
    - // expect: the IR has "ret i32 0"; a void function gets "ret void"

+ Example: a global variable
    - UraGlobal *g = ura_global_create(ctx, "counter", i32, ura_const_int(ctx, i32, 7), URA_EXTERNAL);
    - UraValue *addr = ura_global_address(g);
    - UraValue *v = ura_load(ctx, addr, "load.counter");
    - ura_store(ctx, ura_build_add(ctx, v, ura_const_int(ctx, i32, 3), "ADD"), addr);
    - // inside main: return the loaded value again
    - // expect: the IR has "@counter = global i32 7"; the executable exits with 10

+ Example: a string literal and a variadic call to printf
    - UraType *printf_type = ura_type_func(ctx, i32, (UraType *[]){ ura_type_ptr(ctx, i8) }, 1, true);
    - UraFunc *printf_fn = ura_func_declare(ctx, "printf", printf_type, URA_EXTERNAL);
    - UraValue *fmt = ura_string_literal(ctx, "%d %c\n", 6);
    - ura_call(ctx, printf_fn, (UraValue *[]){ fmt, ura_const_int(ctx, i32, 42), ura_const_char(ctx, 'x') }, 3, "call");
    - // the char argument is promoted to int by ura_call, as C does
    - // expect: the IR has "declare i32 @printf(i8*, ...)" and a global named @str
    - // expect: the program prints "42 x" and a newline


## Milestone 0.2: Functions, Variables and Expressions

### Layout

+ struct UraFunc: name, symbol, type, params, param_names, blocks, entry, is_proto, is_defined, is_variadic, linkage, attributes, owner, is_method, is_static, is_operator, is_destructor, can_throw, ret_slot, value, llvm
+ struct UraBlock: name, func, ends, is_entry, llvm
+ enum UraBinOp: URA_OP_ADD, URA_OP_SUB, URA_OP_MUL, URA_OP_DIV, URA_OP_REM, URA_OP_AND, URA_OP_OR, URA_OP_XOR, URA_OP_SHL, URA_OP_SHR
+ enum UraCmp: URA_EQ, URA_NE, URA_LT, URA_LE, URA_GT, URA_GE
+ enum UraCast: URA_CAST_SEXT, ZEXT, TRUNC, SITOFP, UITOFP, FPTOSI, FPTOUI, FPEXT, FPTRUNC, BITCAST, PTRTOINT, INTTOPTR

### Signatures

+ UraFunc *ura_func_declare(ctx, const char *symbol, UraType *func_type, UraLinkage);  ura_func_lookup(ctx, symbol)
+ void ura_func_set_param_names(UraFunc *, const char **names);  UraValue *ura_func_param(UraFunc *, unsigned i);  UraValue *ura_func_address(UraFunc *)
+ UraFunc *ura_func_begin(ctx, UraFunc *);  void ura_func_end(ctx)   // end adds the default return (ret void, or the zero of the result)
+ UraFunc *ura_func_begin_main(ctx, bool takes_args);  void ura_main_prologue(ctx);  void ura_main_epilogue(ctx, UraValue *exit_code)
+ UraType *ura_func_ret_type(ctx)   // result type of the function being emitted
+ UraBlock *ura_block_create(ctx, name);  ura_block_create_after(ctx, name, UraBlock *after);  void ura_block_position(ctx, block);  bool ura_block_ends(UraBlock *);  void ura_block_open_dead(ctx)
+ UraValue *ura_var_alloca(ctx, type, name)   // always in the entry block
+ UraValue *ura_var_declare(ctx, name, type, UraValue *init)   // init NULL: zero
+ UraValue *ura_var_declare_ref(ctx, name, UraType *elem, UraValue *target_address)
+ UraValue *ura_var_load(ctx, name);  void ura_var_store(ctx, name, value)
+ UraValue *ura_load(ctx, address, name);  void ura_store(ctx, value, address);  void ura_copy(ctx, dest, src, type);  void ura_zero(ctx, address, type)
+ void ura_ref_write(ctx, ref_slot, value);  void ura_ref_rebind(ctx, ref_slot, new_target)
+ UraValue *ura_build_add / sub / mul / div / rem / and / or / xor / shl / shr (ctx, left, right, name);  ura_build_neg(ctx, v, name);  ura_build_bitnot(ctx, v, name);  ura_build_binop(ctx, UraBinOp, l, r, name)
+ UraValue *ura_build_cmp(ctx, UraCmp, left, right, name);  ura_build_not(ctx, v, name);  ura_build_logic_and(ctx, l, r, name);  ura_build_logic_or(ctx, l, r, name)
+ UraValue *ura_build_cast(ctx, UraCast, value, UraType *to, name);  ura_build_convert(ctx, value, to, name)   // convert picks the cast from the two types
+ UraValue *ura_call(ctx, UraFunc *, UraValue **args, size_t count, name);  ura_call_indirect(ctx, func_value, func_type, args, count, name)
+ void ura_set_location(ctx, int line, int column)

### Examples that must pass

+ Example: variables and arithmetic (names as the recorded IR has them)
    - ura_var_declare(ctx, "x", i32, ura_const_int(ctx, i32, 10));
    - ura_var_declare(ctx, "y", i32, ura_const_int(ctx, i32, 5));
    - UraValue *t1 = ura_build_add(ctx, ura_var_load(ctx, "x"), ura_var_load(ctx, "y"), "ADD");   // 15
    - UraValue *t2 = ura_build_mul(ctx, t1, K(2), "MUL");     // 30      (K(n) is ura_const_int(ctx, i32, n))
    - UraValue *t3 = ura_build_sub(ctx, t2, K(6), "SUB");      // 24
    - UraValue *t4 = ura_build_div(ctx, t3, K(4), "DIV");      // 6
    - UraValue *t5 = ura_build_rem(ctx, t4, K(5), "MOD");      // 1
    - ura_codegen_return(ctx, t5);
    - // expect: the IR has "%x = alloca i32, align 4" and "%load.x = load i32, i32* %x, align 4"
    - // expect: the executable exits with 1

+ Example: signed division rounds toward zero
    - ura_var_declare(ctx, "n", i32, K(-7));
    - UraValue *q = ura_build_div(ctx, ura_var_load(ctx, "n"), K(2), "DIV");   // -3
    - UraValue *r = ura_build_rem(ctx, ura_var_load(ctx, "n"), K(2), "MOD");   // -1, the sign of the dividend
    - // return q * -10 + r * -1
    - // expect: the IR has "sdiv i32" and "srem i32"; the executable exits with 31

+ Example: comparisons and logic (plain and, no short-circuit, as recorded)
    - UraValue *lt = ura_build_cmp(ctx, URA_LT, a, b, "LT");           // a = 3, b = 5 loaded from variables
    - UraValue *gt = ura_build_cmp(ctx, URA_GT, b, a, "GT");
    - UraValue *both = ura_build_logic_and(ctx, lt, gt, "AND");        // true
    - UraValue *none = ura_build_not(ctx, both, "NOT");                // false
    - UraValue *as_int = ura_build_convert(ctx, both, i32, "conv");    // 1: a bool becomes an int with zext
    - // expect: the IR has "icmp slt i32", "and i1" and "zext i1"

+ Example: casts
    - // w is a variable holding 258
    - UraValue *t = ura_build_convert(ctx, ura_var_load(ctx, "w"), i8, "t");   // trunc: 2
    - UraValue *back = ura_build_convert(ctx, t, i32, "back");                 // sext: 2
    - UraValue *neg = ura_build_convert(ctx, ura_const_int(ctx, i8, -1), i32, "neg");   // sext: -1
    - // expect: the IR has "trunc i32"; back == 2 and neg == -1 at run time

+ Example: calling a function before its definition
    - UraFunc *add = ura_func_declare(ctx, "add", ura_type_func(ctx, i32, (UraType *[]){ i32, i32 }, 2, false), URA_EXTERNAL);
    - ura_func_set_param_names(add, (const char *[]){ "a", "b" });
    - // 1. begin main, which calls add although add has no body yet
    - ura_codegen_return(ctx, ura_call(ctx, add, (UraValue *[]){ K(40), K(2) }, 2, "call"));
    - // 2. end main, then begin add and return a + b with ura_var_load(ctx, "a") and "b"
    - // expect: the IR has "define i32 @add(i32 %0, i32 %1)"; the executable exits with 42

+ Example: a function used as a value
    - UraValue *fp = ura_func_address(twice);                     // twice(x) returns x * 2
    - UraValue *r = ura_call_indirect(ctx, fp, twice_type, (UraValue *[]){ K(21) }, 1, "r");
    - // expect: the executable exits with 42

+ Example: a scope hides an outer name and gives it back
    - ura_var_declare(ctx, "x", i32, K(1));
    - ura_var_declare(ctx, "t", i32, K(0));
    - ura_scope_push(ctx, URA_SCOPE_BLOCK, NULL);
    - ura_var_declare(ctx, "x", i32, K(2));                    // shadows
    - ura_var_store(ctx, "t", ura_var_load(ctx, "x"));        // t = 2
    - ura_scope_pop(ctx);
    - // return x * 10 + t
    - // expect: the executable exits with 12

+ Example: main with a prologue and an epilogue
    - ura_func_begin_main(ctx, false);
    - ura_main_prologue(ctx);                                    // global initialisers, code outside functions
    - ura_store(ctx, K(5), ura_global_address(g));
    - ura_main_epilogue(ctx, ura_load(ctx, ura_global_address(g), "load.g"));   // destructors of globals, then the return
    - ura_func_end(ctx);
    - // expect: the executable exits with 5


## Milestone 0.3: Control Flow

### Layout

+ typedef UraValue *(*UraEmitValueFn)(UraIRContext *ctx, void *user);  UraValue *(*UraEmitBodyFn)(UraIRContext *ctx, void *user)
+ struct UraIfBranch: condition (NULL for else), condition_user, body, body_user, block_name, scope_kind
+ struct UraIfChain: branches, count, result_type (NULL for a statement), result_name, has_else
+ struct UraLoopFrame: kind (WHILE FOR LOOP MATCH), break_block, continue_block, scope_depth, label, try_depth
+ struct UraForRange: var_name, var_type, start, end, step (NULL is 1), by_ref, direction (URA_RANGE_AUTO, UP, DOWN)
+ struct UraForEach: var_name, array, elem_type, by_ref, index_name
+ struct UraCase: values, value_count, body, body_user
+ struct UraMatch: subject, cases, case_count, has_default, default_body, default_user
+ struct UraCleanup: kind (DROP FREE CUSTOM), value, type, func, flag
+ struct UraTryFrame: catch_block, end_block, scope_depth, loop_depth

### Signatures

+ void ura_br(ctx, block);  ura_cond_br(ctx, cond, then_block, else_block);  ura_ret(ctx, value);  ura_ret_void(ctx);  ura_unreachable(ctx)
+ UraValue *ura_build_phi(ctx, type, UraValue **values, UraBlock **blocks, size_t count, name);  ura_build_select(ctx, cond, if_true, if_false, name)
+ UraValue *ura_codegen_conditional(ctx, UraIfChain *chain)   // then, next, endif blocks; a phi when result_type is set
+ incremental form: UraIfBuilder *ura_if_begin(ctx);  ura_if_test(b, cond);  ura_if_elif(b);  ura_if_else(b);  ura_if_value(b, value);  UraValue *ura_if_end(b, result_type, name)
+ void ura_codegen_while(ctx, UraEmitValueFn cond, void *cond_user, UraEmitBodyFn body, void *body_user)
+ void ura_codegen_loop(ctx, UraEmitBodyFn body, void *body_user)
+ void ura_codegen_for_range(ctx, UraForRange *, UraEmitBodyFn body, void *body_user)
+ void ura_codegen_for_each(ctx, UraForEach *, UraEmitBodyFn body, void *body_user)
+ void ura_codegen_match(ctx, UraMatch *)   // one switch, no fall-through, break leaves the match
+ void ura_codegen_break(ctx);  ura_codegen_continue(ctx);  ura_codegen_return(ctx, UraValue *value)   // all three run the cleanups of the scopes they leave
+ void ura_loop_push(ctx, UraLoopFrame);  ura_loop_pop(ctx);  UraLoopFrame *ura_loop_current(ctx)
+ void ura_scope_add_cleanup(ctx, UraCleanup);  ura_scope_run_cleanups(ctx, UraScope *)
+ try (error slot, a global flag plus value, checked after calls to throwing functions): void ura_try_begin(ctx, UraTryFrame *);  ura_try_end_body(ctx, frame);  UraValue *ura_catch_value(ctx, name);  ura_try_end(ctx, frame);  ura_codegen_throw(ctx, UraValue *error);  ura_check_error_after_call(ctx, UraFunc *callee)
+ void ura_codegen_trap(ctx, const char *message);  ura_codegen_trap_if(ctx, UraValue *condition, UraTrapKind, const char *detail)

### Rules

+ A block with a terminator is ended. Nothing is emitted after it until the core emits code there; then a dead block opens.
+ A branch from an ended block is skipped (an if body that returns does not jump to endif).

### Examples that must pass

+ Example: if, elif, else (callbacks emit the conditions and the bodies)
    - // cond_lt(c, limit): returns ura_build_cmp(c, URA_LT, ura_var_load(c, "n"), K(limit), "LT")
    - // body_ret(c, value): calls ura_codegen_return(c, K(value)) and returns NULL
    - UraIfBranch br[3] = {
    -     { .condition = cond_lt, .condition_user = (void *)10, .body = body_ret, .body_user = (void *)1 },
    -     { .condition = cond_lt, .condition_user = (void *)20, .body = body_ret, .body_user = (void *)2 },
    -     { .condition = NULL,                                  .body = body_ret, .body_user = (void *)3 },   // else
    - };
    - UraIfChain chain = { .branches = br, .count = 3, .result_type = NULL, .has_else = true };
    - UraValue *none = ura_codegen_conditional(ctx, &chain);     // a statement chain returns NULL
    - // grade(5), grade(15), grade(25) give 1, 2, 3
    - // expect: the IR has the labels "then:", "next:" and "endif:"; 100 * 1 + 10 * 2 + 3 = 123

+ Example: an if without else, in a recursive function
    - UraIfBranch br[1] = { { .condition = cond_n_le_1, .body = body_ret, .body_user = (void *)1 } };
    - UraIfChain chain = { .branches = br, .count = 1 };
    - ura_codegen_conditional(ctx, &chain);                      // if n <= 1: return 1
    - ura_codegen_return(ctx, ura_build_mul(ctx, ura_var_load(ctx, "n"),
    -                        ura_call(ctx, fact, (UraValue *[]){ ura_build_sub(ctx, ura_var_load(ctx, "n"), K(1), "SUB") }, 1, "call"), "MUL"));
    - // expect: fact(5) is 120

+ Example: an if used as a value, merged by a phi
    - UraIfBranch br[2] = {
    -     { .condition = cond_n_gt_0, .body = yield_const, .body_user = (void *)10 },    // yield_const returns K(10)
    -     { .condition = NULL,        .body = yield_const, .body_user = (void *)20 },
    - };
    - UraIfChain chain = { .branches = br, .count = 2, .result_type = i32, .result_name = "pick", .has_else = true };
    - UraValue *merged = ura_codegen_conditional(ctx, &chain);   // a phi of 10 and 20
    - ura_codegen_return(ctx, merged);
    - // expect: the IR has "phi i32"; pick(1) + pick(-1) is 30

+ Example: while
    - ura_var_declare(ctx, "i", i32, K(1));
    - ura_var_declare(ctx, "sum", i32, K(0));
    - ura_codegen_while(ctx, cond_i_le_10, NULL, body_add_i_and_increment, NULL);
    - // expect: the IR has the labels "cond:" and "endwhile:"; sum is 55

+ Example: break and continue inside the body of a loop
    - // body: i += 1;  if i > 9: break;  if i % 2 == 0: continue;  sum += i
    - // the two ifs are ura_codegen_conditional calls whose bodies call ura_codegen_break(c) and ura_codegen_continue(c)
    - ura_codegen_while(ctx, cond_true, NULL, body_odd_sum, NULL);       // cond_true returns ura_const_bool(c, true)
    - ura_codegen_loop(ctx, body_odd_sum, NULL);                          // the same body in an endless loop
    - // expect: both give sum 1 + 3 + 5 + 7 + 9 = 25

+ Example: a variable declared in a loop body is allocated in the entry block
    - // body: ura_var_declare(c, "t", i32, K(1));  i += t
    - // expect: in the IR, "%t = alloca" is between "entry:" and "cond:"

+ Example: for over a range, up, down and with a step
    - UraForRange up   = { .var_name = "i", .var_type = i32, .start = K(0), .end = K(5),  .step = NULL, .direction = URA_RANGE_AUTO };
    - UraForRange down = { .var_name = "i", .var_type = i32, .start = K(5), .end = K(0),  .step = NULL, .direction = URA_RANGE_AUTO };
    - UraForRange by2  = { .var_name = "i", .var_type = i32, .start = K(0), .end = K(10), .step = K(2),  .direction = URA_RANGE_UP };
    - ura_codegen_for_range(ctx, &up, body_add_i_to_sum, NULL);          // 0 1 2 3 4 = 10
    - ura_codegen_for_range(ctx, &down, body_add_i_to_sum, NULL);        // 5 4 3 2 1 = 15, the end is excluded
    - ura_codegen_for_range(ctx, &by2, body_add_i_to_sum, NULL);         // 0 2 4 6 8 = 20
    - // expect: sum is 45

+ Example: match on integers, two values sharing a case, and a default
    - UraValue *one[1] = { K(1) };
    - UraValue *two_three[2] = { K(2), K(3) };
    - UraCase cases[2] = {
    -     { .values = one,       .value_count = 1, .body = body_ret, .body_user = (void *)10 },
    -     { .values = two_three, .value_count = 2, .body = body_ret, .body_user = (void *)20 },
    - };
    - UraMatch m = { .subject = ura_var_load(ctx, "n"), .cases = cases, .case_count = 2, .has_default = true,
    -                .default_body = body_ret, .default_user = (void *)30 };
    - ura_codegen_match(ctx, &m);
    - // expect: the IR has "switch i32"; f(1) + f(3) + f(9) is 10 + 20 + 30 = 60

+ Example: match on an enum
    - UraType *color = ura_enum_define(ctx, "Color", (const char *[]){ "RED", "GREEN", "BLUE" }, (long long[]){ 0, 1, 2 }, 3);
    - // expect: ura_const_int_value(ura_enum_variant(ctx, color, "BLUE")) == 2
    - // expect: strcmp(ura_enum_variant_name(color, 1), "GREEN") == 0
    - UraValue *red[1] = { ura_enum_variant(ctx, color, "RED") };   // a case on RED returns 10, GREEN returns 20, default 30
    - // expect: f(GREEN) + f(BLUE) is 20 + 30 = 50

+ Example: code written after a return goes to a dead block
    - ura_codegen_return(ctx, K(3));
    - ura_var_declare(ctx, "z", i32, K(9));                      // nothing reaches this
    - ura_func_end(ctx);
    - // expect: the module verifies; the IR has the label "dead:"; the executable exits with 3


## Milestone 0.4: Memory, Arrays, Strings and References

### Signatures

+ UraValue *ura_build_gep_index(ctx, base, index, name);  ura_build_gep_field(ctx, struct_address, unsigned field, name);  ura_build_gep_chain(ctx, base, unsigned *indices, size_t count, name)
+ UraValue *ura_address_of(ctx, variable);  ura_deref(ctx, pointer, name);  ura_ptr_add(ctx, pointer, offset, name)
+ UraValue *ura_build_is_null(ctx, pointer, name);  ura_build_is_not_null(ctx, pointer, name)
+ UraValue *ura_array_literal(ctx, UraType *elem, UraValue **values, size_t count, name)   // returns the element pointer
+ UraValue *ura_array_alloca(ctx, elem, size_t length, name);  ura_array_alloca_dynamic(ctx, elem, UraValue *length, name)
+ UraValue *ura_array_element(ctx, array_pointer, index, name);  ura_array_len(ctx, array, name)
+ UraValue *ura_slice_make(ctx, data, length, name);  ura_slice_from_array(ctx, array_address, name);  ura_slice_range(ctx, source, start, end, bool checked, name)
+ UraValue *ura_string_to_array(ctx, dest_array, const char *bytes, size_t length)   // memcpy from the literal
+ UraValue *ura_heap_alloc(ctx, UraType *type, UraValue *count, name);  ura_heap_alloc_dynamic(ctx, elem, length, name)
+ void ura_heap_free(ctx, pointer);  ura_heap_clean(ctx, slot, type)   // clean frees and stores null
+ void ura_memcpy(ctx, dest, src, size, unsigned align);  ura_memmove(...);  ura_memset(ctx, dest, byte, size, align)

### Rules

+ A reference is a pointer kept in a slot (alloca of the pointer type); reads go through dref.NAME. Every reference can be null; next &Node starts null.
+ There is no guard operator and no fallback operator any more. A null check is ura_build_is_null; with URA_FLAG_TRAP_NULL_DEREF on, every dereference is checked first.
+ Bounds checks only with URA_FLAG_BOUNDS_CHECKS. A slice is {T*, i64} and arrives with milestone 8 (.len).

### Examples that must pass

+ Example: an array literal, element access and write
    - UraValue *arr = ura_array_literal(ctx, i32, (UraValue *[]){ K(4), K(5), K(6) }, 3, "array");
    - ura_store(ctx, K(9), ura_array_element(ctx, arr, K(1), "a[1]"));                      // a[1] = 9
    - UraValue *v0 = ura_load(ctx, ura_array_element(ctx, arr, K(0), "a[0]"), "load.a[0]");
    - UraValue *v1 = ura_load(ctx, ura_array_element(ctx, arr, K(1), "a[1]"), "load.a[1]");
    - UraValue *v2 = ura_load(ctx, ura_array_element(ctx, arr, K(2), "a[2]"), "load.a[2]");
    - // return v0 + v1 + v2
    - // expect: the IR has "alloca [3 x i32]"; the executable exits with 4 + 9 + 6 = 19

+ Example: heap memory is zeroed
    - UraValue *p = ura_heap_alloc(ctx, i32, NULL, "p");           // calloc of one i32
    - UraValue *zero = ura_load(ctx, p, "zero");
    - ura_store(ctx, K(7), p);
    - UraValue *seven = ura_load(ctx, p, "seven");
    - ura_heap_free(ctx, p);
    - // return seven + zero
    - // expect: the IR calls @calloc; the executable exits with 7

+ Example: a reference parameter changes the caller
    - UraFunc *inc = ura_func_declare(ctx, "inc", ura_type_func(ctx, v, (UraType *[]){ ura_type_ref(ctx, i32) }, 1, false), URA_EXTERNAL);
    - ura_func_set_param_names(inc, (const char *[]){ "c" });
    - // body of inc:
    - UraValue *ptr = ura_var_load(ctx, "c");                    // the pointer kept in the parameter's slot
    - UraValue *old = ura_deref(ctx, ptr, "old");
    - ura_ref_write(ctx, ura_symbol_lookup(ctx, "c")->value, ura_build_add(ctx, old, K(1), "ADD_ASSIGN"));
    - // in main, three times:
    - ura_call(ctx, inc, (UraValue *[]){ ura_address_of(ctx, n) }, 1, NULL);        // n is a variable holding 0
    - // expect: the IR has "define void @inc(i32* %0)" and "%c = alloca i32*, align 8"
    - // expect: the executable exits with 3

+ Example: write through a reference, rebind it, write again
    - UraValue *p = ura_var_declare_ref(ctx, "p", i32, ura_address_of(ctx, a));   // p &i32 = &a   (a = 1, b = 2)
    - ura_ref_write(ctx, p, K(10));                              // p = 10  writes a
    - ura_ref_rebind(ctx, p, ura_address_of(ctx, b));            // p = &b
    - ura_ref_write(ctx, p, K(20));                              // writes b
    - // expect: a + b is 30

+ Example: a reference that is null
    - UraValue *q = ura_var_declare_ref(ctx, "q", i32, ura_const_null(ctx, ura_type_ptr(ctx, i32)));   // q &i32 starts as null
    - UraValue *was_null = ura_build_convert(ctx, ura_build_is_null(ctx, ura_var_load(ctx, "q"), "isnull"), i32, "conv");
    - ura_ref_rebind(ctx, q, ura_address_of(ctx, a));
    - UraValue *now_set = ura_build_convert(ctx, ura_build_is_not_null(ctx, ura_var_load(ctx, "q"), "isset"), i32, "conv");
    - // return was_null * 10 + now_set
    - // expect: the executable exits with 11


## Milestone 0.5: Structs, Fields, Methods, Templates, Enums, Modules

### Layout

+ struct UraFieldSpec: name, type, is_ref, default_value
+ struct UraStruct: name, symbol, type, fields, methods, operators, destructor, copy_assign, output_fn, owner_module, template_of, template_args, declared_in, is_defined, size, align, owns, has_ref_fields
+ struct UraMethod: name, owner, func, op, is_static, self_by_ref, param_types, ret_type, returns_ref, is_destructor
+ enum UraOperatorKind: URA_OP_NONE, ADD, SUB, MUL, DIV, MOD, EQ, NE, LT, GT, LE, GE, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, AND, OR, XOR, SHL, SHR, ASSIGN, INDEX, OUTPUT, DROP
+ struct UraTemplateKey: template_name, args, count
+ struct UraMethodSpec: name, func_type, op, is_static, emit_body (UraEmitBodyFn), user;  struct UraStructBindings: methods, method_count, operators, operator_count, destructor

### Signatures

+ UraStruct *ura_struct_declare(ctx, name)   // step 1: opaque type, registered in the scope
+ bool ura_struct_define(ctx, UraStruct *, const UraFieldSpec *fields, size_t count)   // step 2: body, offsets, size; false if it contains itself by value
+ UraStruct *ura_codegen_struct(ctx, name, const UraFieldSpec *fields, size_t count, const UraStructBindings *bindings)   // both steps plus the methods
+ int ura_struct_field_index(UraStruct *, name);  ura_struct_field_count;  ura_struct_size;  ura_struct_align
+ UraValue *ura_field_address(ctx, struct_address, UraStruct *, field, name);  ura_field_load(ctx, struct_address, s, field, name);  void ura_field_store(ctx, struct_address, s, field, value)
+ UraValue *ura_field_chain_address(ctx, base, UraStruct *, const char **fields, size_t count, name)
+ void ura_struct_zero(ctx, address, s);  ura_struct_copy(ctx, dest, src, s)
+ UraMethod *ura_method_declare(ctx, UraStruct *, name, UraType *func_type, UraOperatorKind, bool is_static)   // func_type excludes the receiver
+ void ura_method_begin(ctx, UraMethod *);  ura_method_end(ctx);  UraMethod *ura_method_lookup(UraStruct *, name, UraType **arg_types, size_t count, UraOperatorKind)
+ UraValue *ura_call_method(ctx, self_address, UraMethod *, args, count, name);  ura_call_static(ctx, UraMethod *, args, count, name);  ura_call_operator(ctx, UraOperatorKind, left_address, right, name)
+ UraFunc *ura_struct_destructor(ctx, UraStruct *);  void ura_struct_register_cleanup(ctx, UraValue *variable, UraStruct *)
+ UraStruct *ura_template_instance(ctx, const UraTemplateKey *, bool *created);  UraFunc *ura_func_instance(ctx, const UraTemplateKey *, UraType *func_type, bool *created)
+ UraType *ura_enum_define(ctx, name, const char **variants, long long *values, size_t count);  UraValue *ura_enum_variant(ctx, enum_type, variant);  const char *ura_enum_variant_name(enum_type, long long value)
+ UraScope *ura_module_open(ctx, name);  void ura_module_close(ctx);  char *ura_qualify(ctx, name)

### Rules

+ By value: structs travel as LLVM aggregates (define i32 @bump(%Point %0), define %User @make(i32 %0)). The receiver of a method is a pointer (define i32 @User.get(%User* %0)).
+ Symbols: Type.method, module.name, Box<i32>.method, outer.inner. Pass pointers, not structs, to C.
+ The destructor of a struct also drops the fields it owns, in reverse order; a reference field is borrowed and never dropped.

### Examples that must pass

+ Example: a struct: declare, define, zero value, fields
    - UraStruct *pt = ura_struct_declare(ctx, "Point");
    - UraFieldSpec f[2] = { { .name = "x", .type = i32 }, { .name = "y", .type = i32 } };
    - // expect: ura_struct_define(ctx, pt, f, 2) is true; pt->size == 8; pt->align == 4
    - // expect: ura_struct_field_index(pt, "y") == 1 and ura_struct_field_index(pt, "nope") == -1
    - UraValue *p = ura_var_declare(ctx, "p", pt->type, NULL);        // zeroed: store %Point zeroinitializer
    - ura_field_store(ctx, p, pt, "x", K(3));
    - ura_field_store(ctx, p, pt, "y", K(4));
    - // return ura_field_load(ctx, p, pt, "x", "p.x") + ura_field_load(ctx, p, pt, "y", "p.y")
    - // expect: the IR has "%Point = type { i32, i32 }"; the executable exits with 7

+ Example: padding follows the target ABI
    - UraFieldSpec f[2] = { { .name = "a", .type = i8 }, { .name = "b", .type = i32 } };
    - ura_struct_define(ctx, pad, f, 2);
    - // expect: pad->size == 8 and pad->align == 4
    - // expect: ura_offsetof(ctx, pad->type, 1) == 4

+ Example: a struct used before its declaration, with a reference to itself
    - UraStruct *node = ura_struct_declare(ctx, "Node");           // opaque for now
    - // expect: !ura_type_is_complete(node->type)
    - UraFieldSpec f[2] = {
    -     { .name = "value", .type = i32 },
    -     { .name = "next",  .type = ura_type_ref(ctx, node->type), .is_ref = true },   // next &Node
    - };
    - // expect: ura_struct_define(ctx, node, f, 2) is true; node->size == 2 * sizeof(void *)

+ Example: a struct cannot contain itself by value
    - UraStruct *bad = ura_struct_declare(ctx, "Bad");
    - UraFieldSpec f[1] = { { .name = "me", .type = bad->type } };
    - // expect: ura_struct_define(ctx, bad, f, 1) is false, and one diagnostic of level ERROR was reported

+ Example: a field of a nested struct, in one call
    - const char *path[2] = { "inner", "y" };
    - UraValue *y = ura_field_chain_address(ctx, o, outer, path, 2, "o.inner.y");
    - ura_store(ctx, K(9), y);
    - // expect: loading the same chain gives 9

+ Example: structs pass and return by value, as recorded
    - // make(a) returns Point { a, a + 1 }:  RET(ura_load(ctx, p, "load.p"))
    - // sum(p Point) returns p.x + p.y, p is a parameter: ura_symbol_lookup(ctx, "p")->value is its slot
    - ura_codegen_return(ctx, ura_call(ctx, sum, (UraValue *[]){ ura_call(ctx, make, (UraValue *[]){ K(5) }, 1, "call") }, 1, "call"));
    - // expect: the IR has "define %Point @make(i32 %0)" and "define i32 @sum(%Point %0)"
    - // expect: the executable exits with 5 + 6 = 11

+ Example: copying a struct
    - ura_copy(ctx, b, a, pt->type);                             // memcpy
    - ura_field_store(ctx, a, pt, "x", K(100));                 // b must not follow
    - // expect: the IR has "llvm.memcpy"; b.x + b.y is 1 + 2 = 3 when a was { 1, 2 }

+ Example: methods, declared and emitted in any order
    - UraStruct *cn = ura_struct_declare(ctx, "Counter");   // one field: n i32
    - UraMethod *inc = ura_method_declare(ctx, cn, "inc", ura_type_func(ctx, v, NULL, 0, false), URA_OP_NONE, false);
    - UraMethod *get = ura_method_declare(ctx, cn, "get", ura_type_func(ctx, i32, NULL, 0, false), URA_OP_NONE, false);
    - ura_method_begin(ctx, get);                                 // get is emitted before inc
    - ura_codegen_return(ctx, ura_field_load(ctx, ura_var_load(ctx, "self"), cn, "n", "self.n"));
    - ura_method_end(ctx);
    - // in main, with c a Counter variable: ura_call_method(ctx, c, inc, NULL, 0, NULL) twice, then return ura_call_method(ctx, c, get, NULL, 0, "get")
    - // expect: the IR has "define void @Counter.inc(%Counter* %0)" and "define i32 @Counter.get(%Counter* %0)"
    - // expect: the executable exits with 2

+ Example: a static method (pub fn)
    - UraMethod *twice = ura_method_declare(ctx, cn, "twice", ura_type_func(ctx, i32, (UraType *[]){ i32 }, 1, false), URA_OP_NONE, true);
    - // body: return x * 2        (the parameter is named x with ura_func_set_param_names(twice->func, ...))
    - ura_codegen_return(ctx, ura_call_static(ctx, twice, (UraValue *[]){ K(21) }, 1, "r"));   // Counter::twice(21)
    - // expect: the IR has "define i32 @Counter.twice(i32 %0)"; the executable exits with 42

+ Example: destructors run at the end of the scope, in reverse order
    - // struct S has one field id; its drop method does: counter = counter * 10 + self.id   (counter is a global)
    - UraMethod *drop = ura_method_declare(ctx, s, "drop", ura_type_func(ctx, v, NULL, 0, false), URA_OP_DROP, false);
    - ura_scope_push(ctx, URA_SCOPE_BLOCK, NULL);
    - UraValue *a = ura_var_declare(ctx, "a", s->type, NULL);  ura_field_store(ctx, a, s, "id", K(1));  ura_struct_register_cleanup(ctx, a, s);
    - UraValue *b = ura_var_declare(ctx, "b", s->type, NULL);  ura_field_store(ctx, b, s, "id", K(2));  ura_struct_register_cleanup(ctx, b, s);
    - ura_scope_pop(ctx);                                         // drops b, then a
    - // expect: ura_struct_destructor(ctx, s) != NULL; counter is (0 * 10 + 2) * 10 + 1 = 21

+ Example: one instance per set of template arguments
    - UraTemplateKey key = { .template_name = "Box", .args = (UraType *[]){ i32 }, .count = 1 };
    - bool created;
    - UraStruct *a = ura_template_instance(ctx, &key, &created);   // created is true
    - UraStruct *b = ura_template_instance(ctx, &key, &created);   // created is false
    - // expect: a == b and strcmp(a->name, "Box<i32>") == 0
    - // expect: the instance for i8 is another struct named "Box<i8>"


## Milestone 0.6: Runtime Support, Builtins, C Interop, Debug Info

### Signatures

+ enum UraTrapKind: URA_TRAP_BOUNDS, NULL, DIV_ZERO, NULL_CALL, UNCAUGHT, OOM, CUSTOM
+ UraFunc *ura_runtime_declare(ctx, name)   // printf dprintf calloc malloc free write exit abort strlen memcmp getenv
+ UraFunc *ura_trap_helper(ctx, UraTrapKind)   // noreturn: write to stderr, exit 1
+ enum UraPrintTarget: URA_STDOUT, URA_STDERR
+ void ura_codegen_print(ctx, UraValue *, UraType *, UraPrintTarget);  ura_codegen_print_string(ctx, text, target);  UraFunc *ura_print_helper(ctx, type, target)
+ UraValue *ura_build_sizeof(ctx, UraType *type, UraType *result_type)
+ void ura_ir_add_link(ctx, const char *library)
+ debug: void ura_debug_enable(ctx, filename, directory, bool full);  ura_debug_finalize(ctx)

### Rules

+ Printing: %d for ints, %ld for i64, %u for unsigned, %f for floats, %s for strings, %c for chars, True and False for bool, the variant name for an enum.
+ A trap message is source:line: text. Division by zero (also floats) traps with URA_FLAG_CHECK_DIV_ZERO.
+ Pass pointers to C; a struct by value does not follow the C ABI.

### Examples that must pass

+ Example: output emitters
    - ura_codegen_print(ctx, K(42), i32, URA_STDOUT);
    - ura_codegen_print_string(ctx, " ", URA_STDOUT);
    - ura_codegen_print(ctx, ura_const_bool(ctx, true), ura_type_bool(ctx), URA_STDOUT);
    - ura_codegen_print_string(ctx, "\n", URA_STDOUT);
    - // expect: the program prints "42 True" and a newline

+ Example: sizeof is a constant
    - UraValue *size = ura_build_sizeof(ctx, pt->type, i32);       // Point is two i32
    - // expect: the value is 8; for an i64 it is 8 as well

+ Example: division by zero traps when the check is on
    - ura_ir_set_flag(ctx, URA_FLAG_CHECK_DIV_ZERO, true);
    - ura_set_location(ctx, 3, 12);                               // line and column for the message
    - ura_codegen_return(ctx, ura_build_div(ctx, K(10), ura_var_load(ctx, "d"), "DIV"));   // d is a variable holding 0
    - // expect: the executable exits with 1 and writes a message to stderr
    - // with d = 4 and K(20): the executable exits with 5


## Milestone 0.7: Optimizer Hooks and Backend

### Layout

+ enum UraOptLevel: URA_O0, URA_O1, URA_O2, URA_O3, URA_OS, URA_OZ
+ struct UraOptimizer: level, size_level, inline_threshold, unroll_loops, vectorize_loops, vectorize_slp, verify_each, function_pm, module_pm, builder, pass_builder_options, pipeline, custom_passes, initialized
+ struct UraPassSpec: name, stage (URA_PASS_FUNCTION or URA_PASS_MODULE), after_default
+ struct UraLinkOptions: libraries, library_paths, extra_flags, sanitize, debug, opt_level, verbose

### Signatures

+ void ura_opt_set_level(ctx, UraOptLevel);  ura_opt_set_option(ctx, UraOptOption, long value);  ura_opt_add_pass(ctx, name, UraPassStage, bool after_default)
+ bool ura_opt_init(ctx);  ura_opt_run_function(ctx, UraFunc *);  ura_opt_run(ctx)   // level 0 returns at once
+ void ura_opt_dispose(ctx)   // LLVM 12: legacy pass managers; LLVM 13 and later: pass builder and a pipeline string
+ bool ura_emit_object(ctx, path);  ura_emit_assembly(ctx, path)
+ bool ura_build_executable(ctx, exe_path, const UraLinkOptions *options)   // NULL: defaults
+ int ura_run_executable(const char *exe_path, char **args)

### Examples that must pass

+ Example: the optimizer at level 0 changes nothing
    - char *before = ura_ir_to_string(ctx);
    - ura_opt_run(ctx);                                           // returns true at once
    - char *after = ura_ir_to_string(ctx);
    - // expect: strcmp(before, after) == 0, and the IR still has "alloca"

+ Example: the optimizer at level 2 removes the stack slots
    - ura_opt_set_level(ctx, URA_O2);
    - // expect: ura_opt_run(ctx) is true; the IR no longer has "alloca"
    - // expect: the executable gives the same exit status as at level 0

+ Example: build and run an executable
    - ura_ir_emit_ll(ctx, "build/out.ll");
    - ura_build_executable(ctx, "build/out", NULL);              // NULL options: the defaults
    - // expect: ura_run_executable("build/out", NULL) returns the exit status of main


## Which wrapper part each language milestone needs

+ 1: 0.1, 0.2, 0.3 (conditional, while, break, continue, return), string literals.
+ 2: arrays, references, casts, null of 0.4.
+ 3: 0.5 structs, fields, methods, copy.   4: templates.   5: modules, links, os.   6: print emitters.
+ 7: enums, match.   8: heap, destructors, slices.   9: operators, static methods.   10: loops.
+ 11: unsigned, floats, shifts, division trap.   12: null references, try and throw.   13: function values.
+ 0.7 is independent; the executable build is needed from milestone 1.

## Open decisions

+ D1: symbol separator. Proposed: Type.method, module.name. Decide before the first method is emitted.
+ D2: short-circuit and, or. The recorded IR has none (plain and, or).
+ D3: array parameters are bare pointers now; the fat pointer {T*, i64} arrives with slices in milestone 8.
+ D5: try with an error slot (proposed) or LLVM exception handling.
+ D6: bounds and division checks are off by default; decide later whether they become the default.
+ D7: integer overflow wraps; a checked mode later.
+ D9: no deferred IR for now; the API would not change if it came later.

## Done when

+ grep llvm-c finds only src/ir.c. main.c has no identifier starting with LLVM.
+ The IR of milestones 1 to 4 equals the recorded IR and verifies.
+ src/proto.h lists the ura_ functions.
