// Milestone 0 tests: the LLVM feature wrapper (src/ir.c), as specified in
// tests/milestone0.md.
//
// Each test builds a small program through the ura_ API only (this file never
// includes an llvm-c header, which is part of what it checks), writes the IR,
// builds an executable, runs it, and compares the exit status and the output.
// A few tests also look for names the recorded IR of milestones 1 to 4 uses.
//
// Build and run from the repo root, with the task shell loaded (source
// src/config.sh): irtests
//
// By hand (LLVM 12, adjust llvm-config):
//   clang -g3 -fsanitize=address $(llvm-config --cflags) tests/milestone0.c \
//         $(llvm-config --ldflags --libs core native bitwriter passes ipo analysis) \
//         -lstdc++ -o build/milestone0 && ./build/milestone0
//
// Every test must pass with no failure line. The exit status of this program
// is the number of failed checks (0 means everything passed).

#define _POSIX_C_SOURCE 200809L   // popen, pclose

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>

// ---------------------------------------------------------------------------
// What ir.c expects from the compiler core: the arena and the list macros.
// The real ones are in main.c; these are the same interface, over calloc.
// ---------------------------------------------------------------------------
#define expand(type, name) \
    type *name;            \
    size_t name##_count;   \
    size_t name##_size;

void *ura_alloc(size_t count, size_t size) {
    void *p = calloc(count, size);
    if (!p) {
        fprintf(stderr, "ura_alloc: out of memory\n");
        exit(2);
    }
    return p;
}

#define push_back(parent, child)                                          \
    {                                                                     \
        if (parent##_size == 0)                                           \
            parent = ura_alloc((parent##_size = 10), sizeof(*parent));    \
        else if (parent##_count + 2 == parent##_size) {                   \
            void *tmp = ura_alloc((parent##_size *= 2), sizeof(*parent)); \
            memcpy(tmp, parent, (parent##_count * sizeof(*parent)));      \
            parent = tmp;                                                 \
        }                                                                 \
        parent[parent##_count++] = child;                                 \
    }

#include "../src/ir.c"

// ---------------------------------------------------------------------------
// A tiny test harness
// ---------------------------------------------------------------------------
static int checks, failures;
static const char *current;

#define CHECK(cond)                                                              \
    do {                                                                         \
        checks++;                                                                \
        if (!(cond)) {                                                           \
            failures++;                                                          \
            fprintf(stderr, "  FAIL %s:%d [%s] %s\n", __FILE__, __LINE__, current, #cond); \
        }                                                                        \
    } while (0)

typedef struct {
    UraIRContext *ctx;
    UraType *i8, *i32, *i64, *b, *v, *p8;
    int diag_errors;
    char diag_last[512];
} Env;
static Env E;

static void on_diag(void *user, UraDiagLevel level, const char *message) {
    (void)user;
    if (level >= URA_DIAG_ERROR) {
        E.diag_errors++;
        snprintf(E.diag_last, sizeof(E.diag_last), "%s", message);
    }
}

static void begin(const char *name) {
    current = name;
    memset(&E, 0, sizeof(E));
    E.ctx = ura_ir_create("ura", "test.ura", on_diag, NULL);
    E.i8 = ura_type_int(E.ctx, 8, true);
    E.i32 = ura_type_int(E.ctx, 32, true);
    E.i64 = ura_type_int(E.ctx, 64, true);
    E.b = ura_type_bool(E.ctx);
    E.v = ura_type_void(E.ctx);
    E.p8 = ura_type_ptr(E.ctx, E.i8);
}

static void finish(void) {
    if (E.diag_errors)
        fprintf(stderr, "  diagnostic: %s\n", E.diag_last);
    ura_ir_dispose(E.ctx);
}

// Verifies the module, writes the IR, builds the executable, runs it.
// Returns the exit status; the program's output (stdout, and stderr when
// merge is true) is copied into out.
static int build_and_run(const char *label, char *out, size_t cap, bool merge) {
    char ll[256], exe[256], cmd[600];
    mkdir("build", 0755);
    snprintf(ll, sizeof(ll), "build/ir_%s.ll", label);
    snprintf(exe, sizeof(exe), "build/ir_%s", label);
    ura_ir_verify(E.ctx);
    CHECK(E.diag_errors == 0);
    CHECK(ura_ir_emit_ll(E.ctx, ll));
    CHECK(ura_build_executable(E.ctx, exe, NULL));
    snprintf(cmd, sizeof(cmd), "./%s%s", exe, merge ? " 2>&1" : "");
    if (out && cap) out[0] = 0;
    FILE *f = popen(cmd, "r");
    if (!f) {
        CHECK(!"popen");
        return -1;
    }
    size_t n = 0;
    if (out && cap) {
        n = fread(out, 1, cap - 1, f);
        out[n] = 0;
    } else {
        char sink[256];
        while (fread(sink, 1, sizeof(sink), f) > 0) {}
    }
    int status = pclose(f);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

// build_and_run, then compare the exit status and (when not NULL) the stdout
static void run_expect(const char *label, int code, const char *stdout_text) {
    char out[4096];
    int got = build_and_run(label, out, sizeof(out), false);
    if (got != code) {
        failures++;
        checks++;
        fprintf(stderr, "  FAIL [%s] exit status %d, expected %d\n", current, got, code);
    } else
        checks++;
    if (stdout_text) {
        checks++;
        if (strcmp(out, stdout_text) != 0) {
            failures++;
            fprintf(stderr, "  FAIL [%s] stdout \"%s\", expected \"%s\"\n", current, out, stdout_text);
        }
    }
}

// the textual IR contains text
static bool ir_has(const char *text) {
    char *ir = ura_ir_to_string(E.ctx);
    bool found = ir && strstr(ir, text) != NULL;
    if (!found) fprintf(stderr, "  IR of [%s] has no \"%s\"\n", current, text);
    ura_ir_free_string(ir);
    return found;
}
#define IR_HAS(text) CHECK(ir_has(text))

// ---------------------------------------------------------------------------
// Short names for the calls every test makes
// ---------------------------------------------------------------------------
static UraValue *K(long long v) { return ura_const_int(E.ctx, E.i32, v); }
static UraValue *LD(const char *name) { return ura_var_load(E.ctx, name); }
static void ST(const char *name, UraValue *v) { ura_var_store(E.ctx, name, v); }
static UraValue *ADD(UraValue *a, UraValue *b) { return ura_build_add(E.ctx, a, b, "ADD"); }
static UraValue *SUB(UraValue *a, UraValue *b) { return ura_build_sub(E.ctx, a, b, "SUB"); }
static UraValue *MUL(UraValue *a, UraValue *b) { return ura_build_mul(E.ctx, a, b, "MUL"); }
static UraValue *DIV(UraValue *a, UraValue *b) { return ura_build_div(E.ctx, a, b, "DIV"); }
static UraValue *REM(UraValue *a, UraValue *b) { return ura_build_rem(E.ctx, a, b, "MOD"); }
static UraValue *CMP(UraCmp c, UraValue *a, UraValue *b) { return ura_build_cmp(E.ctx, c, a, b, "CMP"); }
static UraValue *TOINT(UraValue *v) { return ura_build_convert(E.ctx, v, E.i32, "conv"); }
static void RET(UraValue *v) { ura_codegen_return(E.ctx, v); }

static UraType *fn_type(UraType *ret, UraType **params, size_t n) {
    return ura_type_func(E.ctx, ret, params, n, false);
}
static UraFunc *declare(const char *name, UraType *ret, UraType **params, size_t n, const char **names) {
    UraFunc *f = ura_func_declare(E.ctx, name, fn_type(ret, params, n), URA_EXTERNAL);
    if (names) ura_func_set_param_names(f, names);
    return f;
}
static UraFunc *begin_main(void) {
    UraFunc *m = declare("main", E.i32, NULL, 0, NULL);
    ura_func_begin(E.ctx, m);
    return m;
}
static UraFunc *declare_printf(void) {
    return ura_func_declare(E.ctx, "printf", ura_type_func(E.ctx, E.i32, (UraType *[]){ E.p8 }, 1, true), URA_EXTERNAL);
}
static UraValue *CALL(UraFunc *f, UraValue **args, size_t n) { return ura_call(E.ctx, f, args, n, "call"); }
static UraValue *CALL1(UraFunc *f, UraValue *a) { return CALL(f, (UraValue *[]){ a }, 1); }
static UraValue *VAR(const char *name, UraValue *init) {
    return ura_var_declare(E.ctx, name, E.i32, init);
}
static UraValue *SLOT(const char *name) { return ura_symbol_lookup(E.ctx, name)->value; }

// ===========================================================================
// Milestone 0.1: context, types, constants, globals
// ===========================================================================

static void test_context_lifecycle(void) {
    begin("context lifecycle");
    CHECK(E.ctx != NULL);
    ura_ir_verify(E.ctx);
    CHECK(E.diag_errors == 0);
    char *ir = ura_ir_to_string(E.ctx);
    CHECK(ir != NULL && strstr(ir, "ModuleID = 'ura'") != NULL);
    ura_ir_free_string(ir);
    finish();
}

static void test_types_are_unique(void) {
    begin("types are unique");
    UraIRContext *c = E.ctx;
    CHECK(ura_type_int(c, 32, true) == E.i32);
    CHECK(ura_type_int(c, 8, true) != E.i32);
    CHECK(ura_type_ptr(c, E.i32) == ura_type_ptr(c, E.i32));
    CHECK(ura_type_ref(c, E.i32) == ura_type_ref(c, E.i32));
    CHECK(ura_type_ref(c, E.i32) != ura_type_ptr(c, E.i32));
    CHECK(ura_type_array(c, E.i32, 3) == ura_type_array(c, E.i32, 3));
    CHECK(ura_type_array(c, E.i32, 3) != ura_type_array(c, E.i32, 4));
    CHECK(ura_type_bool(c) == E.b && ura_type_void(c) == E.v);
    UraType *params[2] = { E.i32, E.i32 };
    CHECK(ura_type_func(c, E.i32, params, 2, false) == ura_type_func(c, E.i32, params, 2, false));
    CHECK(ura_type_func(c, E.i32, params, 2, false) != ura_type_func(c, E.i32, params, 2, true));
    CHECK(ura_type_kind(E.i32) == URA_I32);
    CHECK(ura_type_kind(E.b) == URA_BOOL);
    CHECK(ura_type_kind(ura_type_ref(c, E.i32)) == URA_REF);
    CHECK(ura_type_elem(ura_type_ref(c, E.i32)) == E.i32);
    CHECK(strcmp(ura_type_to_string(c, E.i32), "i32") == 0);
    CHECK(strcmp(ura_type_to_string(c, ura_type_ref(c, E.i32)), "&i32") == 0);
    finish();
}

static void test_type_sizes(void) {
    begin("type sizes");
    UraIRContext *c = E.ctx;
    CHECK(ura_sizeof(c, E.i8) == 1);
    CHECK(ura_sizeof(c, E.i32) == 4);
    CHECK(ura_sizeof(c, E.i64) == 8);
    CHECK(ura_sizeof(c, E.b) == 1);
    CHECK(ura_sizeof(c, ura_type_float(c, 32)) == 4);
    CHECK(ura_sizeof(c, ura_type_float(c, 64)) == 8);
    CHECK(ura_sizeof(c, ura_type_ptr(c, E.i32)) == sizeof(void *));
    CHECK(ura_sizeof(c, ura_type_array(c, E.i32, 5)) == 20);
    CHECK(ura_alignof(c, E.i64) == 8);
    finish();
}

static void test_constants(void) {
    begin("constants");
    UraIRContext *c = E.ctx;
    CHECK(ura_value_is_const(K(5)));
    CHECK(ura_const_int_value(K(42)) == 42);
    CHECK(ura_const_int_value(K(-7)) == -7);
    CHECK(ura_value_type(K(1)) == E.i32);
    CHECK(ura_value_type(ura_const_bool(c, true)) == E.b);
    CHECK(ura_const_int_value(ura_const_zero(c, E.i32)) == 0);
    CHECK(ura_value_is_null_const(ura_const_null(c, ura_type_ptr(c, E.i8))));
    CHECK(ura_int_fits(E.i8, 127));
    CHECK(!ura_int_fits(E.i8, 128));
    CHECK(ura_int_fits(E.i8, -128));
    CHECK(!ura_int_fits(E.i8, -129));
    CHECK(ura_int_fits(E.i32, 2147483647LL));
    CHECK(!ura_int_fits(E.i32, 2147483648LL));
    finish();
}

static void test_return_42(void) {
    begin("return 42");
    begin_main();
    RET(K(42));
    ura_func_end(E.ctx);
    IR_HAS("define i32 @main()");
    IR_HAS("ret i32 42");
    IR_HAS("target triple");
    run_expect("return_42", 42, NULL);
    finish();
}

static void test_default_return(void) {
    begin("default return");
    begin_main();          // no return statement at all
    ura_func_end(E.ctx);   // adds ret i32 0
    IR_HAS("ret i32 0");
    UraFunc *nothing = declare("nothing", E.v, NULL, 0, NULL);
    ura_func_begin(E.ctx, nothing);
    ura_func_end(E.ctx);
    IR_HAS("ret void");
    run_expect("default_return", 0, NULL);
    finish();
}

static void test_global_variable(void) {
    begin("global variable");
    UraGlobal *g = ura_global_create(E.ctx, "counter", E.i32, K(7), URA_EXTERNAL);
    CHECK(g != NULL);
    CHECK(ura_global_lookup(E.ctx, "counter") == g);
    begin_main();
    UraValue *addr = ura_global_address(g);
    UraValue *v = ura_load(E.ctx, addr, "load.counter");
    ura_store(E.ctx, ADD(v, K(3)), addr);
    RET(ura_load(E.ctx, addr, "load.counter"));
    ura_func_end(E.ctx);
    IR_HAS("@counter = global i32 7");
    run_expect("global_variable", 10, NULL);
    finish();
}

static void test_string_literal_and_printf(void) {
    begin("string literal and printf");
    UraFunc *pf = declare_printf();
    begin_main();
    UraValue *s = ura_string_literal(E.ctx, "hello\n", 6);
    CALL1(pf, s);
    RET(K(0));
    ura_func_end(E.ctx);
    IR_HAS("declare i32 @printf(i8*, ...)");
    IR_HAS("@str");
    IR_HAS("c\"hello\\0A\\00\"");
    run_expect("string_printf", 0, "hello\n");
    finish();
}

static void test_variadic_promotion(void) {
    begin("variadic promotion");
    UraFunc *pf = declare_printf();
    begin_main();
    UraValue *fmt = ura_string_literal(E.ctx, "%d %c\n", 6);
    // a char argument reaches printf promoted to int
    CALL(pf, (UraValue *[]){ fmt, K(42), ura_const_char(E.ctx, 'x') }, 3);
    RET(K(0));
    ura_func_end(E.ctx);
    run_expect("variadic_promotion", 0, "42 x\n");
    finish();
}

// ===========================================================================
// Milestone 0.2: functions, variables, expressions
// ===========================================================================

static void test_variables_and_arithmetic(void) {
    begin("variables and arithmetic");
    begin_main();
    VAR("x", K(10));
    VAR("y", K(5));
    UraValue *t1 = ADD(LD("x"), LD("y"));   // 15
    UraValue *t2 = MUL(t1, K(2));           // 30
    UraValue *t3 = SUB(t2, K(6));           // 24
    UraValue *t4 = DIV(t3, K(4));           // 6
    UraValue *t5 = REM(t4, K(5));           // 1
    RET(t5);
    ura_func_end(E.ctx);
    IR_HAS("%load.x = load i32, i32* %x, align 4");
    IR_HAS("%x = alloca i32, align 4");
    run_expect("arith", 1, NULL);
    finish();
}

static void test_signed_division(void) {
    begin("signed division");
    begin_main();
    VAR("n", K(-7));
    UraValue *q = DIV(LD("n"), K(2));   // -3, rounds toward zero
    UraValue *r = REM(LD("n"), K(2));   // -1, same sign as the dividend
    // 31 = (-3 * -10) + (-1 * -1)
    RET(ADD(MUL(q, K(-10)), MUL(r, K(-1))));
    ura_func_end(E.ctx);
    IR_HAS("sdiv i32");
    IR_HAS("srem i32");
    run_expect("signed_division", 31, NULL);
    finish();
}

static void test_compare_and_logic(void) {
    begin("compare and logic");
    begin_main();
    VAR("a", K(3));
    VAR("b", K(5));
    UraValue *lt = CMP(URA_LT, LD("a"), LD("b"));
    UraValue *gt = CMP(URA_GT, LD("b"), LD("a"));
    UraValue *both = ura_build_logic_and(E.ctx, lt, gt, "AND");
    UraValue *ne = CMP(URA_NE, LD("a"), LD("b"));
    UraValue *none = ura_build_not(E.ctx, both, "NOT");
    // 1 + 1 * 2 + 0 * 4
    RET(ADD(ADD(TOINT(both), MUL(TOINT(ne), K(2))), MUL(TOINT(none), K(4))));
    ura_func_end(E.ctx);
    IR_HAS("icmp slt i32");
    IR_HAS("icmp sgt i32");
    IR_HAS("and i1");
    IR_HAS("zext i1");
    run_expect("compare_logic", 3, NULL);
    finish();
}

static void test_casts(void) {
    begin("casts");
    begin_main();
    VAR("w", K(258));
    UraValue *t = ura_build_convert(E.ctx, LD("w"), E.i8, "t");      // trunc: 258 -> 2
    UraValue *back = ura_build_convert(E.ctx, t, E.i32, "back");     // sext: 2
    UraValue *m = ura_const_int(E.ctx, E.i8, -1);
    UraValue *neg = ura_build_convert(E.ctx, m, E.i32, "neg");       // sext: -1
    // 2 * 10 + (-1 + 1) + a true bool as 1
    UraValue *flag = TOINT(CMP(URA_EQ, LD("w"), K(258)));
    RET(ADD(ADD(MUL(back, K(10)), ADD(neg, K(1))), flag));
    ura_func_end(E.ctx);
    IR_HAS("trunc i32");
    IR_HAS("zext i1");
    run_expect("casts", 21, NULL);
    finish();
}

static void test_calls_before_declaration(void) {
    begin("call before the definition");
    UraType *two[2] = { E.i32, E.i32 };
    UraFunc *add = declare("add", E.i32, two, 2, (const char *[]){ "a", "b" });
    UraFunc *m = declare("main", E.i32, NULL, 0, NULL);
    ura_func_begin(E.ctx, m);              // main is emitted first, add has no body yet
    RET(CALL(add, (UraValue *[]){ K(40), K(2) }, 2));
    ura_func_end(E.ctx);
    ura_func_begin(E.ctx, add);
    RET(ADD(LD("a"), LD("b")));
    ura_func_end(E.ctx);
    IR_HAS("define i32 @add(i32 %0, i32 %1)");
    IR_HAS("call i32 @add(i32 40, i32 2)");
    run_expect("call_before_definition", 42, NULL);
    finish();
}

static void test_function_value(void) {
    begin("function value");
    UraType *one[1] = { E.i32 };
    UraFunc *twice = declare("twice", E.i32, one, 1, (const char *[]){ "x" });
    ura_func_begin(E.ctx, twice);
    RET(MUL(LD("x"), K(2)));
    ura_func_end(E.ctx);
    begin_main();
    UraValue *fp = ura_func_address(twice);
    UraValue *r = ura_call_indirect(E.ctx, fp, fn_type(E.i32, one, 1), (UraValue *[]){ K(21) }, 1, "r");
    RET(r);
    ura_func_end(E.ctx);
    run_expect("function_value", 42, NULL);
    finish();
}

static void test_scopes_and_shadowing(void) {
    begin("scopes and shadowing");
    begin_main();
    VAR("x", K(1));
    VAR("t", K(0));
    ura_scope_push(E.ctx, URA_SCOPE_BLOCK, NULL);
    VAR("x", K(2));             // shadows the outer x inside this scope
    ST("t", LD("x"));
    ura_scope_pop(E.ctx);
    RET(ADD(MUL(LD("x"), K(10)), LD("t")));   // outer x again: 1 * 10 + 2
    ura_func_end(E.ctx);
    run_expect("scopes", 12, NULL);
    finish();
}

static void test_main_prologue_and_epilogue(void) {
    begin("main prologue and epilogue");
    UraGlobal *g = ura_global_create(E.ctx, "g", E.i32, NULL, URA_EXTERNAL);
    ura_func_begin_main(E.ctx, false);
    ura_main_prologue(E.ctx);
    ura_store(E.ctx, K(5), ura_global_address(g));
    ura_main_epilogue(E.ctx, ura_load(E.ctx, ura_global_address(g), "load.g"));
    ura_func_end(E.ctx);
    IR_HAS("@g = global i32 0");
    run_expect("main_epilogue", 5, NULL);
    finish();
}

// ===========================================================================
// Milestone 0.3: control flow
// ===========================================================================

// -- callbacks used by several tests
static UraValue *cb_n_lt(UraIRContext *c, void *user) {
    (void)c;
    return CMP(URA_LT, LD("n"), K((long long)(intptr_t)user));
}
static UraValue *cb_n_le_1(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_LE, LD("n"), K(1));
}
static UraValue *cb_n_gt_0(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_GT, LD("n"), K(0));
}
static UraValue *cb_return_const(UraIRContext *c, void *user) {
    (void)c;
    RET(K((long long)(intptr_t)user));
    return NULL;
}
static UraValue *cb_yield_const(UraIRContext *c, void *user) {
    (void)c;
    return K((long long)(intptr_t)user);
}

static void test_if_elif_else(void) {
    begin("if elif else");
    UraType *one[1] = { E.i32 };
    UraFunc *grade = declare("grade", E.i32, one, 1, (const char *[]){ "n" });
    ura_func_begin(E.ctx, grade);
    UraIfBranch br[3] = {
        { .condition = cb_n_lt, .condition_user = (void *)(intptr_t)10, .body = cb_return_const, .body_user = (void *)(intptr_t)1 },
        { .condition = cb_n_lt, .condition_user = (void *)(intptr_t)20, .body = cb_return_const, .body_user = (void *)(intptr_t)2 },
        { .condition = NULL, .body = cb_return_const, .body_user = (void *)(intptr_t)3 },
    };
    UraIfChain chain = { .branches = br, .count = 3, .result_type = NULL, .has_else = true };
    CHECK(ura_codegen_conditional(E.ctx, &chain) == NULL);
    ura_func_end(E.ctx);
    begin_main();
    UraValue *a = CALL1(grade, K(5));
    UraValue *b = CALL1(grade, K(15));
    UraValue *d = CALL1(grade, K(25));
    RET(ADD(ADD(MUL(a, K(100)), MUL(b, K(10))), d));
    ura_func_end(E.ctx);
    IR_HAS("then:");
    IR_HAS("next:");
    IR_HAS("endif:");
    IR_HAS("icmp slt i32");
    run_expect("if_elif_else", 123, NULL);
    finish();
}

static void test_if_without_else(void) {
    begin("if without else");
    UraType *one[1] = { E.i32 };
    UraFunc *fact = declare("fact", E.i32, one, 1, (const char *[]){ "n" });
    ura_func_begin(E.ctx, fact);
    UraIfBranch br[1] = { { .condition = cb_n_le_1, .body = cb_return_const, .body_user = (void *)(intptr_t)1 } };
    UraIfChain chain = { .branches = br, .count = 1 };
    ura_codegen_conditional(E.ctx, &chain);
    RET(MUL(LD("n"), CALL1(fact, SUB(LD("n"), K(1)))));
    ura_func_end(E.ctx);
    begin_main();
    RET(CALL1(fact, K(5)));
    ura_func_end(E.ctx);
    run_expect("factorial", 120, NULL);
    finish();
}

static void test_if_value_with_phi(void) {
    begin("if as a value, merged by a phi");
    UraType *one[1] = { E.i32 };
    UraFunc *pick = declare("pick", E.i32, one, 1, (const char *[]){ "n" });
    ura_func_begin(E.ctx, pick);
    UraIfBranch br[2] = {
        { .condition = cb_n_gt_0, .body = cb_yield_const, .body_user = (void *)(intptr_t)10 },
        { .condition = NULL, .body = cb_yield_const, .body_user = (void *)(intptr_t)20 },
    };
    UraIfChain chain = { .branches = br, .count = 2, .result_type = E.i32, .result_name = "pick", .has_else = true };
    UraValue *merged = ura_codegen_conditional(E.ctx, &chain);
    CHECK(merged != NULL);
    RET(merged);
    ura_func_end(E.ctx);
    begin_main();
    RET(ADD(CALL1(pick, K(1)), CALL1(pick, K(-1))));   // 10 + 20
    ura_func_end(E.ctx);
    IR_HAS("phi i32");
    run_expect("if_phi", 30, NULL);
    finish();
}

static UraValue *cb_i_le_10(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_LE, LD("i"), K(10));
}
static UraValue *cb_sum_body(UraIRContext *c, void *user) {
    (void)c; (void)user;
    ST("sum", ADD(LD("sum"), LD("i")));
    ST("i", ADD(LD("i"), K(1)));
    return NULL;
}

static void test_while_loop(void) {
    begin("while loop");
    begin_main();
    VAR("i", K(1));
    VAR("sum", K(0));
    ura_codegen_while(E.ctx, cb_i_le_10, NULL, cb_sum_body, NULL);
    RET(LD("sum"));
    ura_func_end(E.ctx);
    IR_HAS("cond:");
    IR_HAS("endwhile:");
    run_expect("while", 55, NULL);
    finish();
}

static UraValue *cb_true(UraIRContext *c, void *user) {
    (void)user;
    return ura_const_bool(c, true);
}
static UraValue *cb_i_gt_9(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_GT, LD("i"), K(9));
}
static UraValue *cb_i_even(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_EQ, REM(LD("i"), K(2)), K(0));
}
static UraValue *cb_break(UraIRContext *c, void *user) {
    (void)user;
    ura_codegen_break(c);
    return NULL;
}
static UraValue *cb_continue(UraIRContext *c, void *user) {
    (void)user;
    ura_codegen_continue(c);
    return NULL;
}
static UraValue *cb_odd_sum_body(UraIRContext *c, void *user) {
    (void)user;
    ST("i", ADD(LD("i"), K(1)));
    UraIfBranch stop[1] = { { .condition = cb_i_gt_9, .body = cb_break } };
    UraIfChain c1 = { .branches = stop, .count = 1 };
    ura_codegen_conditional(c, &c1);
    UraIfBranch skip[1] = { { .condition = cb_i_even, .body = cb_continue } };
    UraIfChain c2 = { .branches = skip, .count = 1 };
    ura_codegen_conditional(c, &c2);
    ST("sum", ADD(LD("sum"), LD("i")));
    return NULL;
}

static void test_break_and_continue(void) {
    begin("break and continue");
    begin_main();
    VAR("i", K(0));
    VAR("sum", K(0));
    ura_codegen_while(E.ctx, cb_true, NULL, cb_odd_sum_body, NULL);
    RET(LD("sum"));          // 1 + 3 + 5 + 7 + 9
    ura_func_end(E.ctx);
    run_expect("break_continue", 25, NULL);
    finish();
}

static UraValue *cb_declare_t_and_count(UraIRContext *c, void *user) {
    (void)c; (void)user;
    VAR("t", K(1));
    ST("i", ADD(LD("i"), LD("t")));
    return NULL;
}
static UraValue *cb_i_lt_3(UraIRContext *c, void *user) {
    (void)c; (void)user;
    return CMP(URA_LT, LD("i"), K(3));
}

static void test_alloca_goes_to_the_entry_block(void) {
    begin("alloca goes to the entry block");
    begin_main();
    VAR("i", K(0));
    ura_codegen_while(E.ctx, cb_i_lt_3, NULL, cb_declare_t_and_count, NULL);
    RET(LD("i"));
    ura_func_end(E.ctx);
    char *ir = ura_ir_to_string(E.ctx);
    const char *entry = strstr(ir, "entry:");
    const char *cond = strstr(ir, "cond:");
    const char *alloca_t = strstr(ir, "%t = alloca");
    CHECK(entry && cond && alloca_t && alloca_t > entry && alloca_t < cond);   // declared in the loop body, allocated in entry
    ura_ir_free_string(ir);
    run_expect("alloca_entry", 3, NULL);
    finish();
}

static UraValue *cb_add_i_to_sum(UraIRContext *c, void *user) {
    (void)c; (void)user;
    ST("sum", ADD(LD("sum"), LD("i")));
    return NULL;
}

static void test_for_range(void) {
    begin("for range");
    begin_main();
    VAR("sum", K(0));
    UraForRange up = { .var_name = "i", .var_type = E.i32, .start = K(0), .end = K(5), .step = NULL, .direction = URA_RANGE_AUTO };
    ura_codegen_for_range(E.ctx, &up, cb_add_i_to_sum, NULL);       // 0 1 2 3 4 = 10
    UraForRange down = { .var_name = "i", .var_type = E.i32, .start = K(5), .end = K(0), .step = NULL, .direction = URA_RANGE_AUTO };
    ura_codegen_for_range(E.ctx, &down, cb_add_i_to_sum, NULL);     // 5 4 3 2 1 = 15, the end is excluded
    UraForRange stepped = { .var_name = "i", .var_type = E.i32, .start = K(0), .end = K(10), .step = K(2), .direction = URA_RANGE_UP };
    ura_codegen_for_range(E.ctx, &stepped, cb_add_i_to_sum, NULL);  // 0 2 4 6 8 = 20
    RET(LD("sum"));
    ura_func_end(E.ctx);
    run_expect("for_range", 45, NULL);
    finish();
}

static void test_loop_forever_with_break(void) {
    begin("loop with break");
    begin_main();
    VAR("i", K(0));
    VAR("sum", K(0));
    ura_codegen_loop(E.ctx, cb_odd_sum_body, NULL);
    RET(LD("sum"));
    ura_func_end(E.ctx);
    run_expect("loop", 25, NULL);
    finish();
}

static void test_match_on_integers(void) {
    begin("match on integers");
    UraType *one[1] = { E.i32 };
    UraFunc *f = declare("f", E.i32, one, 1, (const char *[]){ "n" });
    ura_func_begin(E.ctx, f);
    UraValue *one_v[1] = { K(1) };
    UraValue *two_three[2] = { K(2), K(3) };    // two values share one body
    UraCase cases[2] = {
        { .values = one_v, .value_count = 1, .body = cb_return_const, .body_user = (void *)(intptr_t)10 },
        { .values = two_three, .value_count = 2, .body = cb_return_const, .body_user = (void *)(intptr_t)20 },
    };
    UraMatch m = { .subject = LD("n"), .cases = cases, .case_count = 2, .has_default = true,
                   .default_body = cb_return_const, .default_user = (void *)(intptr_t)30 };
    ura_codegen_match(E.ctx, &m);
    ura_func_end(E.ctx);
    begin_main();
    RET(ADD(ADD(CALL1(f, K(1)), CALL1(f, K(3))), CALL1(f, K(9))));    // 10 + 20 + 30
    ura_func_end(E.ctx);
    IR_HAS("switch i32");
    run_expect("match_int", 60, NULL);
    finish();
}

static void test_match_on_an_enum(void) {
    begin("match on an enum");
    UraType *color = ura_enum_define(E.ctx, "Color", (const char *[]){ "RED", "GREEN", "BLUE" }, (long long[]){ 0, 1, 2 }, 3);
    CHECK(color != NULL);
    CHECK(ura_const_int_value(ura_enum_variant(E.ctx, color, "BLUE")) == 2);
    CHECK(strcmp(ura_enum_variant_name(color, 1), "GREEN") == 0);
    UraFunc *f = declare("f", E.i32, (UraType *[]){ color }, 1, (const char *[]){ "c" });
    ura_func_begin(E.ctx, f);
    UraValue *red[1] = { ura_enum_variant(E.ctx, color, "RED") };
    UraValue *green[1] = { ura_enum_variant(E.ctx, color, "GREEN") };
    UraCase cases[2] = {
        { .values = red, .value_count = 1, .body = cb_return_const, .body_user = (void *)(intptr_t)10 },
        { .values = green, .value_count = 1, .body = cb_return_const, .body_user = (void *)(intptr_t)20 },
    };
    UraMatch m = { .subject = LD("c"), .cases = cases, .case_count = 2, .has_default = true,
                   .default_body = cb_return_const, .default_user = (void *)(intptr_t)30 };
    ura_codegen_match(E.ctx, &m);
    ura_func_end(E.ctx);
    begin_main();
    RET(ADD(CALL1(f, ura_enum_variant(E.ctx, color, "GREEN")), CALL1(f, ura_enum_variant(E.ctx, color, "BLUE"))));   // 20 + 30
    ura_func_end(E.ctx);
    run_expect("match_enum", 50, NULL);
    finish();
}

static void test_code_after_a_return(void) {
    begin("code after a return");
    begin_main();
    RET(K(3));
    VAR("z", K(9));        // emitted into a block nothing reaches
    ura_func_end(E.ctx);
    IR_HAS("dead:");
    run_expect("after_return", 3, NULL);
    finish();
}

// ===========================================================================
// Milestone 0.4: memory, arrays, references
// ===========================================================================

static void test_array_literal(void) {
    begin("array literal");
    begin_main();
    UraValue *arr = ura_array_literal(E.ctx, E.i32, (UraValue *[]){ K(4), K(5), K(6) }, 3, "array");
    UraValue *e1 = ura_array_element(E.ctx, arr, K(1), "a[1]");
    ura_store(E.ctx, K(9), e1);
    UraValue *v0 = ura_load(E.ctx, ura_array_element(E.ctx, arr, K(0), "a[0]"), "load.a[0]");
    UraValue *v1 = ura_load(E.ctx, ura_array_element(E.ctx, arr, K(1), "a[1]"), "load.a[1]");
    UraValue *v2 = ura_load(E.ctx, ura_array_element(E.ctx, arr, K(2), "a[2]"), "load.a[2]");
    RET(ADD(ADD(v0, v1), v2));        // 4 + 9 + 6
    ura_func_end(E.ctx);
    IR_HAS("alloca [3 x i32]");
    run_expect("array_literal", 19, NULL);
    finish();
}

static void test_heap_allocation(void) {
    begin("heap allocation");
    begin_main();
    UraValue *p = ura_heap_alloc(E.ctx, E.i32, NULL, "p");
    UraValue *zero = ura_load(E.ctx, p, "zero");          // calloc: zeroed
    ura_store(E.ctx, K(7), p);
    UraValue *seven = ura_load(E.ctx, p, "seven");
    ura_heap_free(E.ctx, p);
    RET(ADD(seven, zero));
    ura_func_end(E.ctx);
    IR_HAS("@calloc");
    run_expect("heap", 7, NULL);
    finish();
}

static void test_reference_parameter(void) {
    begin("reference parameter");
    UraFunc *inc = declare("inc", E.v, (UraType *[]){ ura_type_ref(E.ctx, E.i32) }, 1, (const char *[]){ "c" });
    ura_func_begin(E.ctx, inc);
    UraValue *ptr = LD("c");                               // the pointer held by the parameter's slot
    UraValue *old = ura_deref(E.ctx, ptr, "old");
    ura_ref_write(E.ctx, SLOT("c"), ADD(old, K(1)));
    ura_func_end(E.ctx);
    begin_main();
    UraValue *n = VAR("n", K(0));
    for (int i = 0; i < 3; i++)
        CALL1(inc, ura_address_of(E.ctx, n));
    RET(LD("n"));
    ura_func_end(E.ctx);
    IR_HAS("define void @inc(i32* %0)");
    IR_HAS("%c = alloca i32*, align 8");
    IR_HAS("call void @inc(i32* %n)");
    run_expect("ref_param", 3, NULL);
    finish();
}

static void test_reference_write_and_rebind(void) {
    begin("reference write and rebind");
    begin_main();
    UraValue *a = VAR("a", K(1));
    UraValue *b = VAR("b", K(2));
    UraValue *p = ura_var_declare_ref(E.ctx, "p", E.i32, ura_address_of(E.ctx, a));
    ura_ref_write(E.ctx, p, K(10));                         // p = 10 writes a
    ura_ref_rebind(E.ctx, p, ura_address_of(E.ctx, b));     // p = &b
    ura_ref_write(E.ctx, p, K(20));                         // writes b
    RET(ADD(LD("a"), LD("b")));                             // 10 + 20
    ura_func_end(E.ctx);
    run_expect("ref_rebind", 30, NULL);
    finish();
}

static void test_null_reference(void) {
    begin("null reference");
    begin_main();
    UraValue *a = VAR("a", K(1));
    UraValue *q = ura_var_declare_ref(E.ctx, "q", E.i32, ura_const_null(E.ctx, ura_type_ptr(E.ctx, E.i32)));   // q &i32 starts as null
    UraValue *was_null = TOINT(ura_build_is_null(E.ctx, LD("q"), "isnull"));
    ura_ref_rebind(E.ctx, q, ura_address_of(E.ctx, a));
    UraValue *now_set = TOINT(ura_build_is_not_null(E.ctx, LD("q"), "isset"));
    RET(ADD(MUL(was_null, K(10)), now_set));
    ura_func_end(E.ctx);
    run_expect("null_ref", 11, NULL);
    finish();
}

// ===========================================================================
// Milestone 0.5: structs, fields, methods
// ===========================================================================

static UraStruct *make_point(const char *name) {
    UraStruct *s = ura_struct_declare(E.ctx, name);
    UraFieldSpec f[2] = { { .name = "x", .type = E.i32 }, { .name = "y", .type = E.i32 } };
    CHECK(ura_struct_define(E.ctx, s, f, 2));
    return s;
}

static void test_struct_layout_and_fields(void) {
    begin("struct fields");
    UraStruct *pt = make_point("Point");
    CHECK(pt->size == 8 && pt->align == 4);
    CHECK(ura_struct_field_index(pt, "y") == 1);
    CHECK(ura_struct_field_index(pt, "nope") == -1);
    begin_main();
    UraValue *p = ura_var_declare(E.ctx, "p", pt->type, NULL);       // zero-initialised
    ura_field_store(E.ctx, p, pt, "x", K(3));
    ura_field_store(E.ctx, p, pt, "y", K(4));
    RET(ADD(ura_field_load(E.ctx, p, pt, "x", "p.x"), ura_field_load(E.ctx, p, pt, "y", "p.y")));
    ura_func_end(E.ctx);
    IR_HAS("%Point = type { i32, i32 }");
    IR_HAS("zeroinitializer");
    run_expect("struct_fields", 7, NULL);
    finish();
}

static void test_struct_padding(void) {
    begin("struct padding");
    UraStruct *s = ura_struct_declare(E.ctx, "Pad");
    UraFieldSpec f[2] = { { .name = "a", .type = E.i8 }, { .name = "b", .type = E.i32 } };
    CHECK(ura_struct_define(E.ctx, s, f, 2));
    CHECK(s->size == 8);
    CHECK(s->align == 4);
    CHECK(ura_offsetof(E.ctx, s->type, 1) == 4);
    CHECK(ura_sizeof(E.ctx, s->type) == 8);
    finish();
}

static void test_struct_used_before_its_declaration(void) {
    begin("struct used before its declaration");
    UraStruct *node = ura_struct_declare(E.ctx, "Node");     // opaque for now
    CHECK(!ura_type_is_complete(node->type));
    UraFieldSpec f[2] = {
        { .name = "value", .type = E.i32 },
        { .name = "next", .type = ura_type_ref(E.ctx, node->type), .is_ref = true },   // next &Node
    };
    CHECK(ura_struct_define(E.ctx, node, f, 2));
    CHECK(ura_type_is_complete(node->type));
    CHECK(node->size == 2 * sizeof(void *));
    finish();
}

static void test_struct_cannot_contain_itself(void) {
    begin("struct cannot contain itself");
    UraStruct *bad = ura_struct_declare(E.ctx, "Bad");
    UraFieldSpec f[1] = { { .name = "me", .type = bad->type } };
    CHECK(!ura_struct_define(E.ctx, bad, f, 1));
    CHECK(E.diag_errors > 0);
    E.diag_errors = 0;       // the diagnostic was the expected result
    finish();
}

static void test_nested_struct_chain(void) {
    begin("nested struct chain");
    UraStruct *inner = ura_struct_declare(E.ctx, "Inner");
    CHECK(ura_struct_define(E.ctx, inner, (UraFieldSpec[]){ { .name = "y", .type = E.i32 } }, 1));
    UraStruct *outer = ura_struct_declare(E.ctx, "Outer");
    CHECK(ura_struct_define(E.ctx, outer, (UraFieldSpec[]){ { .name = "inner", .type = inner->type }, { .name = "n", .type = E.i32 } }, 2));
    begin_main();
    UraValue *o = ura_var_declare(E.ctx, "o", outer->type, NULL);
    UraValue *y = ura_field_chain_address(E.ctx, o, outer, (const char *[]){ "inner", "y" }, 2, "o.inner.y");
    ura_store(E.ctx, K(9), y);
    RET(ura_load(E.ctx, ura_field_chain_address(E.ctx, o, outer, (const char *[]){ "inner", "y" }, 2, "o.inner.y"), "load.o.inner.y"));
    ura_func_end(E.ctx);
    run_expect("struct_nested", 9, NULL);
    finish();
}

static void test_struct_by_value(void) {
    begin("struct by value");
    UraStruct *pt = make_point("Point");
    UraFunc *make = declare("make", pt->type, (UraType *[]){ E.i32 }, 1, (const char *[]){ "a" });
    ura_func_begin(E.ctx, make);
    UraValue *p = ura_var_declare(E.ctx, "p", pt->type, NULL);
    ura_field_store(E.ctx, p, pt, "x", LD("a"));
    ura_field_store(E.ctx, p, pt, "y", ADD(LD("a"), K(1)));
    RET(ura_load(E.ctx, p, "load.p"));
    ura_func_end(E.ctx);
    UraFunc *sum = declare("sum", E.i32, (UraType *[]){ pt->type }, 1, (const char *[]){ "p" });
    ura_func_begin(E.ctx, sum);
    UraValue *sp = SLOT("p");
    RET(ADD(ura_field_load(E.ctx, sp, pt, "x", "p.x"), ura_field_load(E.ctx, sp, pt, "y", "p.y")));
    ura_func_end(E.ctx);
    begin_main();
    RET(CALL1(sum, CALL1(make, K(5))));        // 5 + 6
    ura_func_end(E.ctx);
    IR_HAS("define %Point @make(i32 %0)");
    IR_HAS("define i32 @sum(%Point %0)");
    run_expect("struct_by_value", 11, NULL);
    finish();
}

static void test_struct_copy(void) {
    begin("struct copy");
    UraStruct *pt = make_point("Point");
    begin_main();
    UraValue *a = ura_var_declare(E.ctx, "a", pt->type, NULL);
    UraValue *b = ura_var_declare(E.ctx, "b", pt->type, NULL);
    ura_field_store(E.ctx, a, pt, "x", K(1));
    ura_field_store(E.ctx, a, pt, "y", K(2));
    ura_copy(E.ctx, b, a, pt->type);
    ura_field_store(E.ctx, a, pt, "x", K(100));         // b must not follow
    RET(ADD(ura_field_load(E.ctx, b, pt, "x", "b.x"), ura_field_load(E.ctx, b, pt, "y", "b.y")));
    ura_func_end(E.ctx);
    IR_HAS("llvm.memcpy");
    run_expect("struct_copy", 3, NULL);
    finish();
}

static void test_methods(void) {
    begin("methods");
    UraStruct *cn = ura_struct_declare(E.ctx, "Counter");
    CHECK(ura_struct_define(E.ctx, cn, (UraFieldSpec[]){ { .name = "n", .type = E.i32 } }, 1));
    UraMethod *inc = ura_method_declare(E.ctx, cn, "inc", fn_type(E.v, NULL, 0), URA_OP_NONE, false);
    UraMethod *get = ura_method_declare(E.ctx, cn, "get", fn_type(E.i32, NULL, 0), URA_OP_NONE, false);
    CHECK(inc && get);
    ura_method_begin(E.ctx, get);          // get is emitted before inc: methods can be called in any order
    RET(ura_field_load(E.ctx, LD("self"), cn, "n", "self.n"));
    ura_method_end(E.ctx);
    ura_method_begin(E.ctx, inc);
    UraValue *n = ura_field_address(E.ctx, LD("self"), cn, "n", "self.n");
    ura_store(E.ctx, ADD(ura_load(E.ctx, n, "old"), K(1)), n);
    ura_method_end(E.ctx);
    begin_main();
    UraValue *c = ura_var_declare(E.ctx, "c", cn->type, NULL);
    ura_call_method(E.ctx, c, inc, NULL, 0, NULL);
    ura_call_method(E.ctx, c, inc, NULL, 0, NULL);
    RET(ura_call_method(E.ctx, c, get, NULL, 0, "get"));
    ura_func_end(E.ctx);
    IR_HAS("define void @Counter.inc(%Counter* %0)");
    IR_HAS("define i32 @Counter.get(%Counter* %0)");
    run_expect("methods", 2, NULL);
    finish();
}

static void test_static_method(void) {
    begin("static method");
    UraStruct *cn = ura_struct_declare(E.ctx, "Counter");
    CHECK(ura_struct_define(E.ctx, cn, (UraFieldSpec[]){ { .name = "n", .type = E.i32 } }, 1));
    UraMethod *twice = ura_method_declare(E.ctx, cn, "twice", fn_type(E.i32, (UraType *[]){ E.i32 }, 1), URA_OP_NONE, true);
    ura_func_set_param_names(twice->func, (const char *[]){ "x" });
    ura_method_begin(E.ctx, twice);
    RET(MUL(LD("x"), K(2)));
    ura_method_end(E.ctx);
    begin_main();
    RET(ura_call_static(E.ctx, twice, (UraValue *[]){ K(21) }, 1, "r"));    // Counter::twice(21)
    ura_func_end(E.ctx);
    IR_HAS("define i32 @Counter.twice(i32 %0)");
    run_expect("static_method", 42, NULL);
    finish();
}

static void test_destructors_run_in_reverse_order(void) {
    begin("destructors run in reverse order");
    UraGlobal *counter = ura_global_create(E.ctx, "counter", E.i32, NULL, URA_EXTERNAL);
    UraStruct *s = ura_struct_declare(E.ctx, "S");
    CHECK(ura_struct_define(E.ctx, s, (UraFieldSpec[]){ { .name = "id", .type = E.i32 } }, 1));
    UraMethod *drop = ura_method_declare(E.ctx, s, "drop", fn_type(E.v, NULL, 0), URA_OP_DROP, false);
    ura_method_begin(E.ctx, drop);
    UraValue *ca = ura_global_address(counter);
    UraValue *id = ura_field_load(E.ctx, LD("self"), s, "id", "self.id");
    ura_store(E.ctx, ADD(MUL(ura_load(E.ctx, ca, "load.counter"), K(10)), id), ca);
    ura_method_end(E.ctx);
    CHECK(ura_struct_destructor(E.ctx, s) != NULL);
    begin_main();
    ura_scope_push(E.ctx, URA_SCOPE_BLOCK, NULL);
    UraValue *a = ura_var_declare(E.ctx, "a", s->type, NULL);
    ura_field_store(E.ctx, a, s, "id", K(1));
    ura_struct_register_cleanup(E.ctx, a, s);
    UraValue *b = ura_var_declare(E.ctx, "b", s->type, NULL);
    ura_field_store(E.ctx, b, s, "id", K(2));
    ura_struct_register_cleanup(E.ctx, b, s);
    ura_scope_pop(E.ctx);                       // drops b, then a: counter = (0 * 10 + 2) * 10 + 1
    RET(ura_load(E.ctx, ura_global_address(counter), "load.counter"));
    ura_func_end(E.ctx);
    run_expect("destructors", 21, NULL);
    finish();
}

static void test_template_instances(void) {
    begin("template instances");
    UraTemplateKey key = { .template_name = "Box", .args = (UraType *[]){ E.i32 }, .count = 1 };
    bool created = false;
    UraStruct *a = ura_template_instance(E.ctx, &key, &created);
    CHECK(a != NULL && created);
    CHECK(strcmp(a->name, "Box<i32>") == 0);
    UraStruct *again = ura_template_instance(E.ctx, &key, &created);
    CHECK(again == a && !created);              // one instance per set of arguments
    UraTemplateKey other = { .template_name = "Box", .args = (UraType *[]){ E.i8 }, .count = 1 };
    UraStruct *b = ura_template_instance(E.ctx, &other, &created);
    CHECK(b != a && created);
    CHECK(strcmp(b->name, "Box<i8>") == 0);
    finish();
}

// ===========================================================================
// Milestone 0.6: runtime support, builtins
// ===========================================================================

static void test_print_emitters(void) {
    begin("print emitters");
    begin_main();
    ura_codegen_print(E.ctx, K(42), E.i32, URA_STDOUT);
    ura_codegen_print_string(E.ctx, " ", URA_STDOUT);
    ura_codegen_print(E.ctx, ura_const_bool(E.ctx, true), E.b, URA_STDOUT);
    ura_codegen_print_string(E.ctx, "\n", URA_STDOUT);
    RET(K(0));
    ura_func_end(E.ctx);
    run_expect("print", 0, "42 True\n");
    finish();
}

static void test_sizeof_builtin(void) {
    begin("sizeof builtin");
    UraStruct *pt = make_point("Point");
    begin_main();
    RET(ADD(ura_build_sizeof(E.ctx, pt->type, E.i32), ura_build_sizeof(E.ctx, E.i64, E.i32)));   // 8 + 8
    ura_func_end(E.ctx);
    run_expect("sizeof", 16, NULL);
    finish();
}

static void test_division_by_zero_traps(void) {
    begin("division by zero traps");
    ura_ir_set_flag(E.ctx, URA_FLAG_CHECK_DIV_ZERO, true);
    begin_main();
    VAR("d", K(0));
    ura_set_location(E.ctx, 3, 12);
    RET(DIV(K(10), LD("d")));
    ura_func_end(E.ctx);
    char out[1024];
    int status = build_and_run("div_zero", out, sizeof(out), true);
    CHECK(status == 1);
    CHECK(strlen(out) > 0);      // the trap says what happened
    finish();
}

static void test_division_check_lets_good_divisions_through(void) {
    begin("division check lets good divisions through");
    ura_ir_set_flag(E.ctx, URA_FLAG_CHECK_DIV_ZERO, true);
    begin_main();
    VAR("d", K(4));
    RET(DIV(K(20), LD("d")));
    ura_func_end(E.ctx);
    run_expect("div_ok", 5, NULL);
    finish();
}

// ===========================================================================
// Milestone 0.7: optimizer and backend
// ===========================================================================

static void build_constant_function(void) {
    begin_main();
    VAR("x", K(2));
    VAR("y", K(0));
    ST("y", MUL(LD("x"), K(21)));
    RET(LD("y"));
    ura_func_end(E.ctx);
}

static void test_optimizer_off_changes_nothing(void) {
    begin("optimizer off changes nothing");
    build_constant_function();
    char *before = ura_ir_to_string(E.ctx);
    CHECK(ura_opt_run(E.ctx));                       // level 0: returns at once
    char *after = ura_ir_to_string(E.ctx);
    CHECK(strcmp(before, after) == 0);
    CHECK(strstr(after, "alloca") != NULL);
    ura_ir_free_string(before);
    ura_ir_free_string(after);
    run_expect("opt_off", 42, NULL);
    finish();
}

static void test_optimizer_level_2(void) {
    begin("optimizer level 2");
    build_constant_function();
    ura_opt_set_level(E.ctx, URA_O2);
    CHECK(ura_opt_run(E.ctx));
    char *ir = ura_ir_to_string(E.ctx);
    CHECK(strstr(ir, "alloca") == NULL);              // mem2reg and friends removed the stack slots
    ura_ir_free_string(ir);
    run_expect("opt_o2", 42, NULL);                   // same answer
    finish();
}

static void test_run_executable(void) {
    begin("run executable");
    begin_main();
    RET(K(7));
    ura_func_end(E.ctx);
    ura_ir_verify(E.ctx);
    mkdir("build", 0755);
    CHECK(ura_ir_emit_ll(E.ctx, "build/ir_runexe.ll"));
    CHECK(ura_build_executable(E.ctx, "build/ir_runexe", NULL));
    CHECK(ura_run_executable("build/ir_runexe", NULL) == 7);
    finish();
}

// ---------------------------------------------------------------------------
int main(void) {
    void (*tests[])(void) = {
        // 0.1
        test_context_lifecycle, test_types_are_unique, test_type_sizes, test_constants,
        test_return_42, test_default_return, test_global_variable,
        test_string_literal_and_printf, test_variadic_promotion,
        // 0.2
        test_variables_and_arithmetic, test_signed_division, test_compare_and_logic, test_casts,
        test_calls_before_declaration, test_function_value, test_scopes_and_shadowing,
        test_main_prologue_and_epilogue,
        // 0.3
        test_if_elif_else, test_if_without_else, test_if_value_with_phi, test_while_loop,
        test_break_and_continue, test_alloca_goes_to_the_entry_block, test_for_range,
        test_loop_forever_with_break, test_match_on_integers, test_match_on_an_enum,
        test_code_after_a_return,
        // 0.4
        test_array_literal, test_heap_allocation, test_reference_parameter,
        test_reference_write_and_rebind, test_null_reference,
        // 0.5
        test_struct_layout_and_fields, test_struct_padding, test_struct_used_before_its_declaration,
        test_struct_cannot_contain_itself, test_nested_struct_chain, test_struct_by_value,
        test_struct_copy, test_methods, test_static_method, test_destructors_run_in_reverse_order,
        test_template_instances,
        // 0.6
        test_print_emitters, test_sizeof_builtin, test_division_by_zero_traps,
        test_division_check_lets_good_divisions_through,
        // 0.7
        test_optimizer_off_changes_nothing, test_optimizer_level_2, test_run_executable,
    };
    size_t count = sizeof(tests) / sizeof(*tests);
    for (size_t i = 0; i < count; i++) {
        int before = failures;
        tests[i]();
        printf("%s %s\n", failures == before ? "ok  " : "FAIL", current);
    }
    printf("\n%zu tests, %d checks, %d failed\n", count, checks, failures);
    return failures > 255 ? 255 : failures;
}
