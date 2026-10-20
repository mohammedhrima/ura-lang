# Milestone 0: LLVM Feature Wrapper

This file is a specification, not a test file. It has no numbered entries, so the test runner finds nothing to run in it. It describes the wrapper that lives in src/ir.c, and it is meant to be read before writing that file. Nothing in it is implementation code: it gives the data structure layouts and the function signatures, and the C bodies are yours to write.

Where this file and the "IR layer" section of the README differ, this file wins. The README draft described a deferred IR (lists of instructions lowered at the end). This file describes an immediate builder, see "Design decision" below.

## Overview

### Goal

+ Give the compiler core a feature-level API over LLVM: the core says "an if with two elif branches and an else", "a struct with these fields", "a while loop", and ir.c turns that into basic blocks, GEPs, branches and phi nodes.
+ Remove every direct LLVM call from main.c. In 003 the core used about 60 distinct LLVM functions, 17 LLVM typedefs (Value, TypeRef, Bloc, and so on) and stored LLVM handles inside tokens (the Asm struct in Token). All of that moves into ir.c.
+ Keep the generated .ll identical to what the old compiler produced, so the recorded IR of milestones 1 to 4 is the regression test of the wrapper.
+ Make later work possible without touching the frontend: optimisation passes, debug info, sanitizers, another LLVM version, even another backend.

### Non-goals

+ The wrapper does not parse, tokenize or type-check Ura. It trusts the core: the core has already reported every user error before the first ura_ call.
+ The wrapper has no knowledge of Token, Node, Type (the core enum) or the arena macros of the core, except ura_alloc.
+ No optimisation by default. Optimisation is off until the core asks for it (-O1 and up).
+ No garbage collector, no runtime library in C. Runtime behaviour (traps, printing) is generated as LLVM code that calls libc.

### Rules of the boundary

+ Only src/ir.c includes the llvm-c headers and llvm/Config/llvm-config.h. main.c includes ir.c after its own core types, so there is still one translation unit.
+ The core never names LLVMValueRef, LLVMTypeRef, LLVMBasicBlockRef, LLVMBuilderRef, LLVMModuleRef or LLVMContextRef. It only holds pointers to the wrapper's own structs: UraType, UraValue, UraBlock, UraFunc, UraGlobal, UraStruct, UraIRContext.
+ A grep for llvm-c outside src/ir.c must find nothing. This is a verification step of the milestone.
+ Every wrapper struct has one hidden member that holds the LLVM handle. The core never reads it. Putting the definitions in ir.c and only forward declarations before it is the way to enforce that, if the include order allows it.
+ Every function of the wrapper starts with the prefix ura_ and takes the UraIRContext pointer as its first argument. The few exceptions are pure queries on a type or a value, which take only that object.
+ All wrapper objects are allocated with the arena (ura_alloc). Wrapper lists use the core's expand and push_back macros. The wrapper frees nothing one by one. The only disposals are the LLVM objects (context, module, builder, target machine, pass managers), done in ura_ir_dispose.

### Design decision: immediate builder, not a deferred IR

+ The README draft planned a second IR (Type, Value, Instruction, Block, Function, Module as plain structs) and a lowering pass at the end. This milestone chooses the simpler path: each ura_ call emits its LLVM instruction right away, and returns a wrapper handle.
+ Why: it is how 003 already worked, so the recorded IR keeps the same order of instructions and names. LLVM verifies the module for free. There is no second tree to keep in sync.
+ What it costs: the order of the calls is the order of the instructions, so the core must call in the right order (for example, a function must be started before its body is emitted). Structs used before their declaration are solved with named opaque struct types, and functions called before their declaration with ura_func_declare, both described below.
+ If a deferred IR is wanted later, the API in this file does not change: the handles stay opaque, only ir.c changes.

### Conventions

+ Return values: functions that create something return a pointer, and NULL means an internal failure that was already reported through the diagnostic callback. Functions that only emit return void.
+ Names: every function that creates an LLVM value takes a name (const char *). The core chooses the name (load.x, dref.p, ADD, and so on), the wrapper passes it on unchanged. LLVM makes names unique and prints them.
+ Nullable arguments: when an argument may be NULL the signature comment says so. A NULL name means an unnamed temporary.
+ Booleans: bool is the C bool of the core. Ura's bool type is i1 everywhere, see the type section.
+ Integer widths are in bits. Sizes, offsets and lengths use size_t on the C side and i64 in the generated code.
+ Errors the wrapper can find itself (a failed module verification, an unsupported type combination) are reported through the callback UraDiagFn stored in the context. User errors never reach the wrapper.
+ The wrapper assumes LLVM 12 (typed pointers: i32*, no opaque ptr). Every place that differs in LLVM 13 or later is marked with the macro LLVM_VERSION_MAJOR in the optimizer section, which is the only part that differs.

### Names the generated IR must keep

These come from the recorded IR in milestones 1 to 4. The core passes them, but the wrapper creates some blocks itself, so it must use the same names.

+ Basic blocks: entry (first block of a function, receives every alloca), then (every body of an if chain, numbered then1, then2 by LLVM when repeated), next (the block that holds the following test of an if chain; when the chain ends with an else, the last next block holds the else body), endif (the merge block of an if chain, created even when nothing reaches it, in which case the function end gives it the default return, as in the recorded IR where it prints as endif with no predecessors and ret i32 0), cond, then and endwhile (a while loop: the test, the body, the exit).
+ Block order is creation order and the recorded IR depends on it: the exit block endwhile is created together with cond and then, so it prints before the blocks of a nested if; the endif of a chain with an else prints after all its bodies. Check the nested entries of milestone1.md (if and while) when fixing the exact moment each block is created.
+ Values: a variable's alloca takes the variable's name (a, hp, self). A load of a variable is load.NAME. A load through a reference is dref.NAME. A field address is NAME.FIELD (for example b.value). Results of binary operations are named after the operator: ADD, SUB, MUL, DIV, MOD, EQ, NQ, LT, GT, LE, GE, AND, OR, NOT, and the compound forms ADD_ASSIGN, SUB_ASSIGN and so on.
+ Struct types are named after the struct (User, Point). Template instances are named Box<i32>, quoted in the .ll as the output already does.
+ Helper names: str, str.1, str.2 for string literal globals; array for the backing storage of an array literal or a string variable; e, e1, e2 for the element addresses an array literal stores through; dst for the destination of the memcpy that fills a string. The array variable itself is a slot holding a pointer to its elements, see 0.4.
+ Module header: ModuleID is ura and source_filename is ura (milestones 1 to 3; the older recording of milestone 4 says ura-module and is recorded again when the new compiler runs), then the data layout and the target triple of the host.
+ Struct copies and string literals use the llvm.memcpy.p0i8.p0i8.i64 intrinsic. Integer comparisons are signed (icmp slt, sgt, sle, sge, eq, ne). Casts are sext, zext and trunc.
+ When one of these names turns out to be wrong against the recorded IR, the recorded IR is right and this list is fixed.

### Sub-milestones and order

+ Milestone 0.1: Context, types, constants and globals.
+ Milestone 0.2: Functions, variables and expressions.
+ Milestone 0.3: Control flow (conditionals, loops, match, break, continue, return, try).
+ Milestone 0.4: Memory, arrays, strings, references and nullable values.
+ Milestone 0.5: Structs, fields, methods, templates, enums and modules.
+ Milestone 0.6: Runtime support, builtins, C interop and debug info.
+ Milestone 0.7: Optimizer hooks and backend.
+ Which language milestone needs which part: language milestone 1 needs 0.1, 0.2, 0.3 (without match and try) and the string part of 0.4. Language milestone 2 needs the rest of 0.4. Language milestone 3 and 4 need 0.5. After that: 0.6 and the match and try parts of 0.3 come with their language milestones, and 0.7 can be done at any time after 0.2, since it is off by default.
+ Appendix A maps every language milestone to the wrapper functions it uses.

## Milestone 0.1: Context, Types, Constants and Globals

### UraIRContext layout

The context is one struct, created once per compilation (the core has a single global Ura, so one global context is the normal case, but nothing in the wrapper may rely on that).

+ struct UraIRContext, members:
    - llvm_context: LLVMContextRef. Created with LLVMContextCreate. Every type and constant is created in this context, never in the global one.
    - llvm_module: LLVMModuleRef. One module per compilation. All files pulled in by use end up in the same module.
    - llvm_builder: LLVMBuilderRef. The only builder. The current position is part of the context state, see the cursor below.
    - target_data: LLVMTargetDataRef. Created from the module's data layout. Used for sizeof, alignment, struct offsets and padding.
    - target_machine: LLVMTargetMachineRef. Created from the host triple. NULL until ura_ir_set_target is called.
    - triple: char *. The target triple string, default is the host's.
    - module_name: char *. Default ura.
    - source_name: char *. Used for the source_filename line and for trap messages.
    - types: UraTypeCache. Table of uniqued types, described below.
    - cursor: UraCursor. The current function, current block, entry block and loop stack. Described in 0.2 and 0.3.
    - globals: array of UraGlobal pointers in creation order, with count and size (the expand macro).
    - functions: array of UraFunc pointers in creation order.
    - structs: array of UraStruct pointers in creation order.
    - scopes: UraScopeStack. The symbol tables, described below.
    - optimizer: UraOptimizer. Pass managers and settings, see 0.7.
    - debug: UraDebugInfo pointer. NULL unless debug info is on, see 0.6.
    - diag: UraDiagFn. Callback for internal errors and warnings.
    - diag_user: void pointer handed back to the callback.
    - flags: UraIRFlags. Bits: verify_module (default on), keep_value_names (default on, needed to match the recorded IR), emit_debug_info, sanitize, bounds_checks (default off: the recorded IR of milestones 1 to 3 has none), check_div_zero (default off until language milestone 11, same reason), trap_on_null_deref (default off, the explicit guard x? always checks).
    - counters: unsigned counters for generated names (string literals, anonymous blocks, helper functions).
    - runtime: UraRuntimeDecls. Cached handles of the libc functions the wrapper itself calls: printf, dprintf, calloc, free, write, exit, strlen, memcpy, memset, getenv, abort.
    - error_slot: UraGlobal pointer for the try and catch machinery, see 0.3. NULL until used.

### Diagnostics callback

+ UraDiagLevel: enum with URA_DIAG_NOTE, URA_DIAG_WARNING, URA_DIAG_ERROR, URA_DIAG_FATAL.
+ UraDiagFn: function pointer type, void (*)(void *user, UraDiagLevel level, const char *message). The wrapper never prints. The core decides how to show the message.
+ The wrapper calls it for: a failed LLVMVerifyModule (message from LLVM, level FATAL), a type that cannot be built (a struct that contains itself by value, a zero-size array element), a call whose argument count does not match, a name used before it was created.
+ A FATAL message means the module is not usable. The core stops, and ura_ir_emit_ll is still allowed so the broken IR can be inspected with the -ll flag.

### Lifecycle functions

+ UraIRContext *ura_ir_create(const char *module_name, const char *source_name, UraDiagFn diag, void *user): creates the LLVM context, module and builder, initialises the native target (LLVMInitializeNativeTarget, LLVMInitializeNativeAsmPrinter, LLVMInitializeNativeAsmParser), sets the triple and the data layout, creates the type cache and the empty tables.
+ void ura_ir_set_target(UraIRContext *ctx, const char *triple, const char *cpu, const char *features, UraRelocMode reloc, UraCodeModel model): creates the target machine, sets the module's triple and data layout from it. Default is the host triple, generic CPU, no features, default reloc and model.
+ void ura_ir_set_flag(UraIRContext *ctx, UraIRFlag flag, bool value): sets one flag.
+ void ura_ir_dispose(UraIRContext *ctx): disposes the builder, pass managers, target machine, target data, module and context, in that order.
+ void ura_ir_verify(UraIRContext *ctx): runs LLVMVerifyModule with the return status action and reports the message as a FATAL diagnostic. Called by ura_ir_emit_ll unless verify_module is off.
+ void ura_ir_reset(UraIRContext *ctx): empties the module so the same context can compile another file. Needed for the test runner if it ever runs in-process.

### The cursor

The cursor is the "where am I emitting" state. It lives inside the context. The core never edits it directly.

+ struct UraCursor, members:
    - func: UraFunc pointer, the function being emitted, NULL at global level.
    - block: UraBlock pointer, the block the builder is positioned in.
    - entry: UraBlock pointer, the entry block of func. Allocas always go here.
    - alloca_end: the last alloca instruction inserted in entry, so the next one goes right after it and the entry block keeps allocas first.
    - loops: array of UraLoopFrame, the stack of open loops and matches (see 0.3).
    - scope_depth: size_t, the number of open scopes.
    - match_depth: size_t.
    - saved: a stack of cursors, for emitting a nested function or a helper in the middle of another.
+ void ura_cursor_push(UraIRContext *ctx): saves the cursor.
+ void ura_cursor_pop(UraIRContext *ctx): restores the saved cursor and the builder position.
+ UraBlock *ura_current_block(UraIRContext *ctx).
+ UraFunc *ura_current_func(UraIRContext *ctx).
+ bool ura_block_ends(UraBlock *block): true when the block already has a terminator. This replaces LLVMGetBasicBlockTerminator in the core.

### Type system: UraTypeKind

+ enum UraTypeKind:
    - URA_VOID
    - URA_BOOL (i1, in registers and in memory alike)
    - URA_I8 (also char)
    - URA_I16
    - URA_I32
    - URA_I64
    - URA_U8, URA_U16, URA_U32, URA_U64 (same LLVM integers as the signed ones; the kind tells the wrapper to use unsigned division, remainder, shifts, comparisons and extensions)
    - URA_F32
    - URA_F64
    - URA_PTR (a pointer to some type; the untyped pointer of the language is a pointer to i8)
    - URA_REF (a reference: a pointer in memory, but dereferenced automatically, see 0.4)
    - URA_ARRAY (fixed length, a known number of elements)
    - URA_SLICE (a view: a fat pointer holding the data pointer and a length, see 0.4)
    - URA_STRUCT (named, with fields)
    - URA_FUNC (function type, used for functions and for function values)
    - URA_TUPLE (an anonymous struct, for multiple return values)
    - URA_ENUM (an i32 with a name; kept as its own kind so printing can use the variant names)
    - URA_OPTIONAL (a nullable value, see 0.4)
    - URA_OPAQUE (a named struct whose body is not set yet)

### Type layout: UraType

+ struct UraType, members:
    - kind: UraTypeKind.
    - name: char *. For structs, enums and opaque types. NULL otherwise.
    - bits: unsigned. For integers and floats, the width.
    - is_signed: bool. For integers.
    - elem: UraType pointer. Pointee of a pointer or reference, element of an array, slice or optional.
    - length: size_t. Element count of an array. Zero for a slice or an array of unknown size.
    - fields: array of UraField (see below), with count and size. For structs and tuples.
    - ret: UraType pointer. Function result.
    - params: array of UraType pointers, with count and size. Function parameters.
    - is_variadic: bool. Function types.
    - struct_info: UraStruct pointer. For struct types, the full description of 0.5.
    - template_args: array of UraType pointers. Set on a template instance, empty otherwise.
    - size, align: size_t. ABI size and alignment, computed from the target data when the type is complete. Zero while the type is opaque.
    - is_complete: bool. False for an opaque struct.
    - llvm: LLVMTypeRef. Hidden.
+ struct UraField, members: name (char *), type (UraType pointer), index (unsigned, position in the LLVM struct), offset (size_t, from the target data), is_ref (a reference field), default_value (UraValue pointer or NULL).

### Type constructors and queries

Types are uniqued in the cache: asking twice for the pointer to i32 returns the same pointer, so the core compares types with ==.

+ UraType *ura_type_void(UraIRContext *ctx)
+ UraType *ura_type_bool(UraIRContext *ctx)
+ UraType *ura_type_int(UraIRContext *ctx, unsigned bits, bool is_signed): 8, 16, 32 and 64.
+ UraType *ura_type_float(UraIRContext *ctx, unsigned bits): 32 or 64.
+ UraType *ura_type_ptr(UraIRContext *ctx, UraType *elem): the plain pointer. ura_type_ptr(ctx, ura_type_int(ctx, 8, true)) is the pointer type of the language.
+ UraType *ura_type_ref(UraIRContext *ctx, UraType *elem)
+ UraType *ura_type_array(UraIRContext *ctx, UraType *elem, size_t length)
+ UraType *ura_type_slice(UraIRContext *ctx, UraType *elem)
+ UraType *ura_type_func(UraIRContext *ctx, UraType *ret, UraType **params, size_t count, bool variadic)
+ UraType *ura_type_tuple(UraIRContext *ctx, UraType **elems, size_t count)
+ UraType *ura_type_optional(UraIRContext *ctx, UraType *elem)
+ UraType *ura_type_enum(UraIRContext *ctx, const char *name)
+ UraType *ura_type_struct_declare(UraIRContext *ctx, const char *name): creates the named opaque struct and returns it. Calling it twice with the same name returns the same type. This is what lets a struct be used before its declaration, and lets a struct hold a reference to itself.
+ bool ura_type_struct_define(UraIRContext *ctx, UraType *type, UraType **field_types, const char **field_names, size_t count): sets the body. Returns false and reports a diagnostic when a field is the struct itself by value (infinite size) or is a type that is still opaque and not behind a pointer.
+ Queries, all pure: ura_type_kind, ura_type_is_integer, ura_type_is_signed, ura_type_is_float, ura_type_is_pointer, ura_type_is_aggregate (array, struct, tuple), ura_type_elem, ura_type_bits, ura_type_name, ura_type_field_count, ura_type_field(type, index), ura_type_param_count, ura_type_param(type, index), ura_type_ret, ura_type_is_complete.
+ bool ura_type_equal(UraType *a, UraType *b): same as ==, written out for clarity in the core.
+ bool ura_type_can_convert(UraType *from, UraType *to, UraConvertKind *how): tells whether an implicit conversion exists (a literal fitting its target, a non-null value widening to an optional) and which cast it needs. Explicit casts (as) are always allowed between scalars.
+ size_t ura_sizeof(UraIRContext *ctx, UraType *type): ABI size from the target data, with struct padding. Used by the sizeof builtin.
+ size_t ura_alignof(UraIRContext *ctx, UraType *type).
+ size_t ura_offsetof(UraIRContext *ctx, UraType *struct_type, unsigned field_index).
+ char *ura_type_to_string(UraIRContext *ctx, UraType *type): the Ura spelling (i32, i32[], Box<i32>, &i32), used by the core in error messages and by typeof.

### Representation decisions for types

+ bool: i1 everywhere, in registers and in memory (a bool variable is alloca i1, a bool array is an array of i1, a bool field is an i1 member), as the recorded IR shows. There is no widening on store and no compare on load. A bool converted to an integer is zext.
+ char is i8 and prints as a character. i8 as a number prints as a number: the distinction is kept by the core, the wrapper only has i8.
+ The untyped pointer of the language and string literals are i8*.
+ Arrays: the language type i32[] has no size in its type (an error to write one). In the recorded IR (milestone 2) an array variable is a slot holding a pointer to its first element (alloca i32*), the elements live in a separate backing array (alloca [3 x i32] named array), and a parameter is the bare element pointer (define i32 @count_zero(i32* %0, i32 %1), with the length passed by the caller as a separate argument). The wrapper follows that. The fat pointer (data pointer plus length, URA_SLICE) is the later form for slices, .len and bounds checks (language milestone 8).
+ Optional values: a pointer-like type (reference, pointer, array, slice) is nullable by being null. A scalar optional would need a flag and is not part of this milestone; ura_type_optional on a scalar reports a diagnostic.
+ Enums are i32. Their variants are constants created by ura_const_enum.
+ Unsigned types share the LLVM integer type with the signed one. A function that emits an operation takes the UraType of its operand to know which instruction to choose.
+ Floats: f32 is float, f64 is double. Comparisons are ordered (fcmp oeq, olt, and so on); the exception is != which is unordered (une).

### Constants

+ UraValue *ura_const_int(UraIRContext *ctx, UraType *type, long long value): an integer, a bool or a char. Sign-extends the value for signed types.
+ UraValue *ura_const_uint(UraIRContext *ctx, UraType *type, unsigned long long value).
+ UraValue *ura_const_float(UraIRContext *ctx, UraType *type, double value).
+ UraValue *ura_const_bool(UraIRContext *ctx, bool value): True and False.
+ UraValue *ura_const_char(UraIRContext *ctx, int value): an i8.
+ UraValue *ura_const_null(UraIRContext *ctx, UraType *type): the zero of a pointer type.
+ UraValue *ura_const_zero(UraIRContext *ctx, UraType *type): the zero of any type: 0, False, 0.0, null, a zeroed struct, a zeroed array. This is the value of a declaration without an initialiser (x i32).
+ UraValue *ura_const_enum(UraIRContext *ctx, UraType *enum_type, long long value).
+ UraValue *ura_const_array(UraIRContext *ctx, UraType *elem, UraValue **values, size_t count): an array literal whose elements are all constants.
+ UraValue *ura_const_struct(UraIRContext *ctx, UraType *type, UraValue **values, size_t count).
+ bool ura_int_fits(UraType *type, long long value) and the unsigned form: tells whether a literal fits its target type, for the core's out-of-range errors.
+ Queries: ura_value_type(UraValue *), ura_value_is_const, ura_value_is_null_const, ura_const_int_value (get the number back, for array sizes and case labels).

### Values: UraValue

+ struct UraValue, members:
    - kind: UraValueKind, one of URA_V_CONST, URA_V_ZERO, URA_V_GLOBAL, URA_V_PARAM, URA_V_FUNC, URA_V_INST (the result of an instruction), URA_V_ALLOCA (storage of a variable).
    - type: UraType pointer. For storage values (alloca, global, field address) this is the type of the thing stored, and is_address is true, so the core can tell a value from the place that holds it.
    - is_address: bool. True when the value is a pointer to storage that was named after a variable, a field or an element.
    - name: char *.
    - llvm: LLVMValueRef. Hidden.
+ UraType *ura_value_type(UraValue *value): replaces LLVMTypeOf.
+ UraType *ura_value_pointee(UraValue *value): the type behind an address. Replaces LLVMGetElementType.

### Globals and string literals

+ struct UraGlobal, members: name, type, initializer (UraValue pointer or NULL for zero), is_constant, linkage (UraLinkage), alignment, section (char * or NULL), is_thread_local (reserved), value (UraValue pointer for the address), llvm (hidden).
+ enum UraLinkage: URA_EXTERNAL (visible to other files, the default for program globals), URA_INTERNAL (static), URA_PRIVATE (string constants), URA_EXTERN_DECL (declared here, defined elsewhere).
+ UraGlobal *ura_global_create(UraIRContext *ctx, const char *name, UraType *type, UraValue *init, UraLinkage linkage): a global variable. A NULL init means zero. Global variables of the language use this.
+ void ura_global_set_constant(UraGlobal *global, bool is_constant).
+ UraValue *ura_global_address(UraGlobal *global).
+ UraGlobal *ura_global_lookup(UraIRContext *ctx, const char *name).
+ UraValue *ura_string_literal(UraIRContext *ctx, const char *bytes, size_t length): creates a private constant array of i8 with a trailing zero (named from a counter), and returns its address as i8*. Two equal literals may share one global.
+ UraValue *ura_string_to_array(UraIRContext *ctx, UraValue *dest_array, const char *bytes, size_t length): fills a writable i8 array from a literal with the memcpy intrinsic, as the recorded IR does. A string in the language is a writable i8 array, so this is how a string declaration works.
+ Globals with a dynamic initialiser (an initial value computed by a call) are not allowed in the module. The core emits the initialisation code at the start of main, see 0.6.

### Module output

+ char *ura_ir_to_string(UraIRContext *ctx): the textual IR, for the -tree and -ll flags. Freed with LLVMDisposeMessage inside the wrapper (ura_ir_free_string).
+ bool ura_ir_emit_ll(UraIRContext *ctx, const char *path): verifies, then writes the .ll file with LLVMPrintModuleToFile. The core passes build/out.ll.
+ bool ura_ir_emit_bitcode(UraIRContext *ctx, const char *path): LLVMWriteBitcodeToFile (the BitWriter header is already included in 003).
+ The build of an executable is in 0.7.

### Symbol tables

The core already resolves names in analyze. The wrapper keeps its own small tables only to map a name to the storage the wrapper created, so a later ura_load_var can find the alloca.

+ struct UraSymbol, members: name (char *), kind (URA_SYM_VAR, URA_SYM_PARAM, URA_SYM_GLOBAL, URA_SYM_FUNC, URA_SYM_STRUCT, URA_SYM_ENUM, URA_SYM_MODULE), type (UraType pointer), value (UraValue pointer, the storage), is_ref (the variable is a reference: loads go through it), is_param, depth (scope depth where it was declared), owner (UraSymbol pointer or NULL, for members).
+ struct UraScope, members: parent (UraScope pointer), symbols (array of UraSymbol), kind (URA_SCOPE_GLOBAL, URA_SCOPE_MODULE, URA_SCOPE_FUNC, URA_SCOPE_BLOCK, URA_SCOPE_LOOP, URA_SCOPE_STRUCT), cleanups (array of UraCleanup, see 0.3), owner_name (the module or struct name, for qualified names).
+ struct UraScopeStack, members: current (UraScope pointer), global (UraScope pointer), depth.
+ UraScope *ura_scope_push(UraIRContext *ctx, UraScopeKind kind, const char *owner_name).
+ void ura_scope_pop(UraIRContext *ctx): emits the cleanups of the scope (destructors) if the current block is not terminated, then closes it. See 0.3.
+ UraSymbol *ura_symbol_declare(UraIRContext *ctx, const char *name, UraSymbolKind kind, UraType *type, UraValue *value): adds to the current scope. A later declaration of the same name in an inner scope shadows the outer one. In the same scope it is an internal error, since the core has already refused it.
+ UraSymbol *ura_symbol_lookup(UraIRContext *ctx, const char *name): searches from the innermost scope outward.
+ UraSymbol *ura_symbol_lookup_qualified(UraIRContext *ctx, const char **path, size_t count): for module::member and Type::method.
+ A struct declared inside a function or block is a symbol of that scope, so the innermost same-named struct wins. The LLVM type gets a unique name when two scopes use the same struct name (the second becomes Name.1, and the printed name stays the Ura one).

## Milestone 0.2: Functions, Variables and Expressions

### Functions

+ struct UraFunc, members: name (the Ura name), symbol (the mangled name in the .ll, see 0.5), type (UraType pointer, a URA_FUNC), params (array of UraValue pointers), param_names (array of char *), blocks (array of UraBlock pointers), entry (UraBlock pointer), is_proto (declared, no body), is_defined, is_variadic, linkage, calling_convention, attributes (UraFuncAttrs), owner (UraStruct pointer or NULL, for methods), is_method, is_static, is_operator, is_destructor, can_throw (set when the body contains a throw or calls something that can), ret_slot (UraValue pointer, the hidden return variable when the function has cleanups to run), value (UraValue pointer, the address of the function), llvm (hidden).
+ enum UraFuncAttrs (bit flags): URA_ATTR_NORETURN, URA_ATTR_NOUNWIND (the default for every Ura function, as there are no C++ exceptions), URA_ATTR_ALWAYSINLINE, URA_ATTR_NOINLINE, URA_ATTR_COLD (trap helpers).
+ UraFunc *ura_func_declare(UraIRContext *ctx, const char *symbol, UraType *type, UraLinkage linkage): creates the function in the module without a body. The core calls it for every function before emitting any body, so a call before the declaration works. A proto statement is exactly this with external linkage.
+ UraFunc *ura_func_lookup(UraIRContext *ctx, const char *symbol).
+ void ura_func_set_param_names(UraFunc *func, const char **names): names the LLVM parameters.
+ UraFunc *ura_func_begin(UraIRContext *ctx, UraFunc *declared): starts the body: creates the entry block, positions the builder in it, opens a function scope, and for each parameter creates an alloca named after it and stores the incoming value (a by-value parameter is a copy, so changing it never changes the caller). A reference parameter is not copied: its pointer is used as the storage. Returns the function.
+ void ura_func_end(UraIRContext *ctx): closes the function. If the current block does not end, it adds the default return: ret void for a void function, and for others a return of the zero of the result type (the recorded IR has this default return). Pops the function scope, verifies the function (LLVMVerifyFunction), and restores the cursor.
+ UraValue *ura_func_param(UraFunc *func, unsigned index): the incoming value.
+ UraType *ura_func_ret_type(UraIRContext *ctx): the result type of the function being emitted (replaces the default-return lookup of 003).
+ Functions inside functions: a function nested in another is emitted as a separate top-level function with a mangled name (outer.inner). It cannot capture locals: the core reports that error, the wrapper only needs ura_cursor_push and ura_cursor_pop to emit the inner body in the middle of the outer one.
+ Function values: ura_func_address(UraFunc *func) gives the address as a value of the function pointer type, so a function can be stored in a variable, passed, and called indirectly.
+ Script mode: UraFunc *ura_func_main_for_script(UraIRContext *ctx): creates i32 main() for a file that has none, and the code outside functions is emitted into it. The global scope code of a file that also has a main is emitted at the start of main by ura_func_begin_main_prologue, see 0.6.
+ Variadic functions: declared with the variadic flag in the type, called with extra arguments after applying C's default promotions (a float argument becomes double, a char or a bool becomes i32). ura_call does the promotion for the extra arguments.

### Blocks

+ struct UraBlock, members: name (char *), func (UraFunc pointer), ends (bool, set by any branch, return, unreachable or switch), is_entry, llvm (hidden).
+ UraBlock *ura_block_create(UraIRContext *ctx, const char *name): appends a block at the end of the current function.
+ UraBlock *ura_block_create_after(UraIRContext *ctx, const char *name, UraBlock *after): puts the new block right after another one (LLVMMoveBasicBlockAfter in 003). The recorded IR shows plain creation order for if and while, so the core should not need it; it is kept for later constructs (match, for) that want a readable order.
+ void ura_block_position(UraIRContext *ctx, UraBlock *block): moves the builder to the end of the block.
+ void ura_block_position_before_terminator(UraIRContext *ctx, UraBlock *block): used for the cleanups before a return.
+ bool ura_block_ends(UraBlock *block).
+ void ura_block_open_dead(UraIRContext *ctx): creates and enters a block named dead. It is used only when the core emits an instruction into a block that already ended (code written after a return, break, continue or throw in the same Ura block). The recorded IR of milestones 1 to 3 shows no such block after a break or a return, so the wrapper must not open one on its own at that point: it just marks the block as ended, and the next instruction (if any) triggers the dead block.

### Variables

+ UraValue *ura_var_alloca(UraIRContext *ctx, UraType *type, const char *name): reserves stack space in the entry block of the current function (after the other allocas), whatever block the builder is in, and returns its address. This is the only way the core creates a local variable.
+ UraValue *ura_var_declare(UraIRContext *ctx, const char *name, UraType *type, UraValue *init): alloca, store of the initial value, and symbol declaration. When init is NULL the slot is zeroed (the language rule) by a store of the zero constant, for scalars and for structs alike (store %User zeroinitializer, as in the recorded IR).
+ UraValue *ura_var_declare_ref(UraIRContext *ctx, const char *name, UraType *elem, UraValue *target_address): a reference variable. The slot holds the pointer. Loading the variable gives the pointer, then dereferencing loads the value (dref.NAME in the recorded IR).
+ UraValue *ura_load(UraIRContext *ctx, UraValue *address, const char *name): a load with the natural alignment of the type. The name is load.NAME or dref.NAME as the core chooses.
+ void ura_store(UraIRContext *ctx, UraValue *value, UraValue *address): a store with the natural alignment. Storing an aggregate value (a struct passed or returned by value) is a plain store of the whole value, as the recorded IR does.
+ UraValue *ura_var_load(UraIRContext *ctx, const char *name): finds the symbol, loads, names it load.NAME. A convenience over ura_symbol_lookup and ura_load.
+ void ura_var_store(UraIRContext *ctx, const char *name, UraValue *value).
+ void ura_ref_rebind(UraIRContext *ctx, UraValue *ref_slot, UraValue *new_target): p = &y. Stores the new pointer in the slot of the reference.
+ void ura_ref_write(UraIRContext *ctx, UraValue *ref_slot, UraValue *value): p = 12. Loads the pointer from the slot, then stores through it.
+ void ura_copy(UraIRContext *ctx, UraValue *dest, UraValue *src, UraType *type): copies an aggregate with the llvm.memcpy intrinsic (size from ura_sizeof, alignment from ura_alignof), as the recorded struct-copy IR does. Scalars use load and store.
+ void ura_zero(UraIRContext *ctx, UraValue *address, UraType *type): zeroes storage: a store of the zero constant when the size is known, memset when it is only known at run time.
+ Globals are read and written with the same ura_load and ura_store on the global's address.

### Arithmetic

All take the operand type from the values, and a name.

+ UraValue *ura_build_add(UraIRContext *ctx, UraValue *left, UraValue *right, const char *name) for integers (add) and floats (fadd).
+ ura_build_sub, ura_build_mul with the same signature.
+ ura_build_div: sdiv for signed, udiv for unsigned, fdiv for floats.
+ ura_build_rem: srem, urem, frem.
+ UraValue *ura_build_neg(UraIRContext *ctx, UraValue *value, const char *name): unary minus (sub from zero, or fneg).
+ Bitwise and shifts: ura_build_and, ura_build_or, ura_build_xor, ura_build_shl, ura_build_shr (arithmetic for signed, logical for unsigned), ura_build_bitnot.
+ Division and remainder by zero: when the flag check_div_zero is on (language milestone 11 turns it on), ura_build_div and ura_build_rem first call the trap helper (see 0.6) if the divisor is zero. The message names the file and line. A division of a float by zero also traps in Ura (no infinity), so the check applies to floats too. The core passes the line through ura_set_location. Until then the divisions are plain sdiv and srem, as recorded.
+ ura_build_binop(UraIRContext *ctx, UraBinOp op, UraValue *left, UraValue *right, const char *name): one entry point with an enum (URA_OP_ADD ... URA_OP_SHR) so the core's switch is a single call. The functions above are thin wrappers.
+ Overflow: wrapping, like C unsigned. No nsw or nuw flags yet. A checked-arithmetic mode is a roadmap item.

### Comparison and logic

+ enum UraCmp: URA_EQ, URA_NE, URA_LT, URA_LE, URA_GT, URA_GE.
+ UraValue *ura_build_cmp(UraIRContext *ctx, UraCmp cmp, UraValue *left, UraValue *right, const char *name): icmp slt, sle, sgt, sge, eq, ne for signed integers and chars; icmp ult and so on for unsigned and for pointers; fcmp o-forms for floats. The result is a bool (i1).
+ UraValue *ura_build_not(UraIRContext *ctx, UraValue *value, const char *name): logical not of a bool (xor with true).
+ UraValue *ura_build_is_null(UraIRContext *ctx, UraValue *pointer, const char *name): icmp eq with null. The isNullable builtin and comparisons with null use it. Also ura_build_is_not_null.
+ and, or: UraValue *ura_build_logic_and(UraIRContext *ctx, UraValue *left, UraValue *right, const char *name) and ura_build_logic_or emit a plain and or or on two bools, named AND and OR. The recorded IR does exactly this (and i1 %GT, %LT): both sides are evaluated, there is no short-circuit, so this is the default.
+ Short-circuit form, for later (not in the recorded IR): UraValue *ura_build_and_then(UraIRContext *ctx, UraEmitValueFn left, UraEmitValueFn right, void *user, const char *name) and ura_build_or_else with the same shape. The right operand is emitted in its own block, only reached when the left does not decide the result, and the two are merged with a phi of type bool. UraEmitValueFn is a function pointer type: UraValue *(*)(UraIRContext *ctx, void *user). Adopting it changes the IR of every test that uses and or or, so it needs a decision (D2) and a new recording.

### Casts

+ enum UraCast: URA_CAST_SEXT, URA_CAST_ZEXT, URA_CAST_TRUNC, URA_CAST_SITOFP, URA_CAST_UITOFP, URA_CAST_FPTOSI, URA_CAST_FPTOUI, URA_CAST_FPEXT, URA_CAST_FPTRUNC, URA_CAST_BITCAST, URA_CAST_PTRTOINT, URA_CAST_INTTOPTR.
+ UraValue *ura_build_cast(UraIRContext *ctx, UraCast cast, UraValue *value, UraType *to, const char *name).
+ UraValue *ura_build_convert(UraIRContext *ctx, UraValue *value, UraType *to, const char *name): picks the right cast from the two types, as the as keyword needs: a smaller integer to a bigger one is sext for signed and zext for unsigned (a bool or char to i32 is zext, as the recorded IR shows for the bool case), bigger to smaller is trunc, int to float is sitofp or uitofp, float to int is fptosi or fptoui, f32 to f64 is fpext, f64 to f32 is fptrunc, a pointer to a pointer is bitcast, a pointer to a 64-bit integer is ptrtoint. The same type returns the value unchanged. A struct to or from a scalar is not convertible: the core has already refused it.
+ UraValue *ura_build_bool_to_int and ura_build_int_to_bool: a bool as i32 and a number as a bool (compare with zero).

### Calls

+ UraValue *ura_call(UraIRContext *ctx, UraFunc *func, UraValue **args, size_t count, const char *name): a direct call. Checks the argument count against the type (variadic allowed), applies the C promotions to the variadic extras, and names the result (a void call has no name).
+ UraValue *ura_call_indirect(UraIRContext *ctx, UraValue *func_value, UraType *func_type, UraValue **args, size_t count, const char *name): a call through a function value. A null function value traps before the call (see 0.6).
+ UraValue *ura_call_method(UraIRContext *ctx, UraValue *self_address, UraFunc *method, UraValue **args, size_t count, const char *name): the receiver is passed first, as a pointer. Defined with the methods in 0.5.
+ After a call to a function that can throw, the wrapper emits the error check, see the try part of 0.3.
+ A call whose result is unused still gets a name of NULL.

### Source locations

+ void ura_set_location(UraIRContext *ctx, int line, int column): the line the core is currently emitting. Used for trap messages and, when debug info is on, for the debug locations. It costs nothing when both are off.

## Milestone 0.3: Control Flow

### Terminator rules

+ A block ends with exactly one terminator. After any branch, return, switch or unreachable the wrapper marks the block as ended.
+ Any ura_ function that would emit an instruction into an ended block first opens a dead block (ura_block_open_dead), so no instruction ever follows a terminator and code after a return is still well-formed. Nothing is opened until such an instruction actually comes.
+ A branch to a block from an ended block is skipped, which is how an if whose body ends with a return avoids a useless jump to endif.

### Branches

+ void ura_br(UraIRContext *ctx, UraBlock *target): unconditional branch, if the current block is not ended.
+ void ura_cond_br(UraIRContext *ctx, UraValue *condition, UraBlock *then_block, UraBlock *else_block).
+ void ura_ret(UraIRContext *ctx, UraValue *value): return, without cleanups. The core uses ura_codegen_return below, which runs the cleanups first.
+ void ura_ret_void(UraIRContext *ctx).
+ void ura_unreachable(UraIRContext *ctx).
+ UraValue *ura_build_phi(UraIRContext *ctx, UraType *type, UraValue **values, UraBlock **blocks, size_t count, const char *name): a phi node with its incoming values. LLVMAddIncoming is called inside.
+ UraValue *ura_build_select(UraIRContext *ctx, UraValue *condition, UraValue *if_true, UraValue *if_false, const char *name): a value choice without branching, used by the optional default operator when both sides are cheap.

### Conditionals: ura_codegen_conditional

The core hands over a whole if, elif ... else chain at once. The conditions and bodies are emitted by callbacks, because the wrapper has to decide in which block each one is emitted.

+ typedef: UraEmitValueFn is UraValue *(*)(UraIRContext *ctx, void *user). It emits an expression in the current block and returns its value.
+ typedef: UraEmitBodyFn is UraValue *(*)(UraIRContext *ctx, void *user). It emits a body in the current block. It returns the value the branch produces, or NULL when the branch is a statement list that produces nothing.
+ struct UraIfBranch, members:
    - condition: UraEmitValueFn. Emitted when the branch is tried. NULL for the else branch.
    - condition_user: void pointer handed to the condition callback.
    - body: UraEmitBodyFn.
    - body_user: void pointer.
    - block_name: const char * for the body block (then for if and elif; NULL means then).
    - scope_kind: the kind of scope opened around the body (always URA_SCOPE_BLOCK; kept so a future branch kind can differ).
+ struct UraIfChain, members: branches (array of UraIfBranch), count, result_type (UraType pointer, or NULL when the chain is a statement), result_name (const char *, name of the phi), has_else (derived: the last branch has no condition).
+ UraValue *ura_codegen_conditional(UraIRContext *ctx, UraIfChain *chain): emits the whole chain and returns the merged value, or NULL for a statement chain.
+ What it does, in order:
    - Remember the current block. The merge block endif is created after the last body when the chain has an else, and together with the last then block when it has none (the last failed test branches to it). The recorded nested entries decide the exact moment.
    - For each branch with a condition: emit the condition callback in the current block; create the body block (then) and the block of the following test or of the else body (next); emit a conditional branch to the body block or the next test block; position in the body block; open a block scope; run the body callback; pop the scope (cleanups run); if the block is not ended, branch to endif and remember the (value, block) pair for the phi.
    - For the else branch: position in the last next block, open a scope, run the body, pop, branch to endif if not ended, remember the pair.
    - If there is no else, the last failed test branches straight to endif.
    - Position in the merge block (it is already last in creation order, no move is needed).
    - If a result type is wanted, every branch that reaches endif must have produced a value and there must be an else: build a phi of those values, and return it. A missing else or a branch with no value is an internal error (the core must have refused it).
    - If every branch ended (all return), the end block would be unreachable: the wrapper still creates it, as the recorded IR does, and leaves it empty. ura_func_end gives it the default return (ret i32 0 in the recorded grade function), since an empty block would not verify.
+ Block names used: then for every body, next for the block of the following test or the else body, endif for the merge. The recorded IR (milestone 1, the if tests, for example the if / elif / else chain entry) is the reference for nested chains.
+ An incremental form, for a core that prefers to drive the emission itself: ura_if_begin(UraIRContext *ctx) returns a UraIfBuilder pointer; ura_if_test(UraIfBuilder *b, UraValue *condition) opens the then block; ura_if_elif(UraIfBuilder *b) closes the current body and opens the next test block; ura_if_else(UraIfBuilder *b); ura_if_value(UraIfBuilder *b, UraValue *value) records a produced value; ura_if_end(UraIfBuilder *b, UraType *result_type, const char *name) closes, merges and returns the phi or NULL. ura_codegen_conditional is built on these.
+ Conditions that are constants (if True) are still emitted as branches. Folding is the optimizer's job.

### Loops

+ enum UraLoopKind: URA_LOOP_WHILE, URA_LOOP_FOR, URA_LOOP_LOOP, URA_LOOP_MATCH.
+ struct UraLoopFrame, members: kind, break_block (UraBlock pointer, where break goes), continue_block (UraBlock pointer, where continue goes; NULL for a match), scope_depth (the scope depth at the loop's start, so break and continue know which cleanups to run), label (const char *, reserved for named loops), try_depth (the open try blocks, so break out of a try can restore the error state).
+ void ura_loop_push(UraIRContext *ctx, UraLoopFrame frame) and void ura_loop_pop(UraIRContext *ctx).
+ UraLoopFrame *ura_loop_current(UraIRContext *ctx): NULL when there is no loop. The core uses it to know whether break is legal, though it has already checked.
+ Ura loops use these block names: cond for the test block, then for the body, endwhile for the exit. A for loop adds step and a loop uses body.
+ void ura_codegen_while(UraIRContext *ctx, UraEmitValueFn condition, void *condition_user, UraEmitBodyFn body, void *body_user): creates cond, then and endwhile; branches to cond; emits the condition there; conditional branch to then or endwhile; pushes a loop frame (continue goes to cond, break to endwhile); opens a scope; runs the body; pops the scope; branches back to cond if the block is not ended; pops the frame; positions in endwhile.
+ void ura_codegen_loop(UraIRContext *ctx, UraEmitBodyFn body, void *body_user): the infinite loop. A body block that branches back to itself, an exit block for break.
+ struct UraForRange, members: var_name (const char *), var_type (UraType pointer, an integer type), start (UraValue pointer), end (UraValue pointer), step (UraValue pointer or NULL for 1), by_ref (reserved), direction (URA_RANGE_AUTO, URA_RANGE_UP, URA_RANGE_DOWN). With AUTO the wrapper compares start and end at run time and picks the direction (up when start is smaller, down when larger), the end is excluded.
+ void ura_codegen_for_range(UraIRContext *ctx, UraForRange *range, UraEmitBodyFn body, void *body_user): evaluates start, end and step once before the loop, creates the loop variable in a new scope (alloca in entry), emits the test, body, and the step block (continue goes to the step block, not to the test), and the exit.
+ struct UraForEach, members: var_name, array (UraValue pointer, an array address or a slice), elem_type, by_ref (bool, for ref x in arr: the variable is a reference to the element and writes change the array), index_name (const char * or NULL).
+ void ura_codegen_for_each(UraIRContext *ctx, UraForEach *each, UraEmitBodyFn body, void *body_user): a hidden index runs from 0 to the length (known for a fixed array, read from the slice otherwise), the element is loaded or bound by reference.
+ void ura_codegen_break(UraIRContext *ctx): runs the cleanups of every scope opened since the loop started, then branches to the loop's break block and marks the block ended (no block is opened until code follows). Inside a match, break leaves the match, not the loop around it.
+ void ura_codegen_continue(UraIRContext *ctx): same, branching to the continue block of the nearest real loop (it skips match frames).
+ void ura_codegen_return(UraIRContext *ctx, UraValue *value): the value may be NULL. If the function has cleanups in open scopes, the value is first stored in the hidden return slot (so a returned local survives its own destructor), all open scopes' cleanups run from the innermost to the outermost, then the slot is loaded and returned. Without cleanups it is a plain ret. The block is marked ended.

### Scope cleanups (destructors)

+ struct UraCleanup, members: kind (URA_CLEANUP_DROP for operator drop, URA_CLEANUP_FREE for a heap value owned by a variable, URA_CLEANUP_CUSTOM), value (UraValue pointer, the storage to clean), type (UraType pointer), func (UraFunc pointer, the destructor to call), flag (UraValue pointer or NULL, an i1 slot that says the object is still alive, so a moved or already cleaned object is not cleaned twice).
+ void ura_scope_add_cleanup(UraIRContext *ctx, UraCleanup cleanup): the core calls it right after declaring a variable whose struct type has a destructor. The wrapper does not search for destructors by itself.
+ void ura_scope_run_cleanups(UraIRContext *ctx, UraScope *scope): emits the calls in reverse order of declaration. Called by ura_scope_pop (when the block is not ended), by break and continue (for the scopes they leave) and by return (for all scopes).
+ The structs a struct owns are cleaned by the destructor-generating helper in 0.5: the destructor of the outer struct first runs its own body, then drops its owned fields in reverse order.
+ Globals with destructors are cleaned at the end of main, before the return (also on an early return).

### Match (switch)

+ struct UraCase, members: values (array of UraValue pointers, all constant integers or enum constants; several values share a body), body (UraEmitBodyFn), body_user (void pointer).
+ struct UraMatch, members: subject (UraValue pointer), cases (array of UraCase), has_default (bool), default_body (UraEmitBodyFn), default_user (void pointer).
+ void ura_codegen_match(UraIRContext *ctx, UraMatch *match): emits an LLVM switch instruction (LLVMBuildSwitch, LLVMAddCase), one block per case named case, a default block (or the end block when there is no default), and an end block named endmatch. There is no fall-through: each body branches to the end block. Pushes a match frame so that break leaves the match and continue is passed on to the enclosing loop.
+ Matching on non-integer subjects (strings) is not part of this milestone. If it is wanted, it is a chain of ura_codegen_conditional calls built by the core.

### Errors: throw, try and catch

Two designs are possible. The first is the default here.

+ Design A, error slot (what 001 did): a module global holds an error flag (i1) and an error value (a pointer to the Error struct). throw stores the value, sets the flag, and returns from the function after its cleanups. After every call to a function that can throw (UraFunc.can_throw), the wrapper loads the flag and, if set, either jumps to the nearest catch block of the current function or returns to its own caller (propagation). It needs no personality function, works with plain C interop, and the cost is one load and branch after each throwing call.
+ Design B, LLVM exception handling: invoke instead of call, a landing pad, a personality function, and resume. Heavier, needs unwinding tables and a runtime. Not chosen, listed so the decision is on record.
+ struct UraTryFrame, members: catch_block (UraBlock pointer), end_block (UraBlock pointer), scope_depth, loop_depth. The frames are kept in a stack in the cursor.
+ void ura_try_begin(UraIRContext *ctx, UraTryFrame *frame): clears the flag, creates the catch and end blocks, pushes the frame.
+ void ura_try_end_body(UraIRContext *ctx, UraTryFrame *frame): branches to the end block if the block is not ended (a plain fall-through), and positions in the catch block.
+ UraValue *ura_catch_value(UraIRContext *ctx, const char *name): in the catch block, loads the error value and clears the flag.
+ void ura_try_end(UraIRContext *ctx, UraTryFrame *frame): branches to the end block, positions there, pops the frame.
+ void ura_codegen_throw(UraIRContext *ctx, UraValue *error_value): stores the value and sets the flag; if a try frame is open in this function, branches to its catch block (after running the cleanups of the scopes inside the try); otherwise runs all cleanups and returns (the zero of the result type). In main, an unhandled throw is checked before the return and prints the message and exits with 1 (an uncaught throw prints its message and exits).
+ void ura_check_error_after_call(UraIRContext *ctx, UraFunc *callee): called by ura_call for throwing callees. Emits the load, the branch to the catch block or the propagation return.
+ return, break and continue inside a try must leave the try state clean: a return inside a try returns from the function (flag unchanged), a break inside a try leaves the loop. Their cleanup path just has to not leave a catch frame half open: ura_codegen_break and ura_codegen_return pop nothing from the try stack (the frames belong to the emission order, not to the run time), they only emit the jumps.
+ Code after a throw in the same block goes to a dead block (the mechanism of 0.2), which nothing reaches.

### Traps

+ void ura_codegen_trap(UraIRContext *ctx, const char *message): writes the message to stderr with write (file:line when the location is known), then calls exit(1), then unreachable. Implemented by a shared helper function per message kind so the generated code stays small, see 0.6.

## Milestone 0.4: Memory, Arrays, Strings, References and Nullable Values

### Pointers and addresses

+ UraValue *ura_build_gep_index(UraIRContext *ctx, UraValue *base, UraValue *index, const char *name): address of element index of a pointer or array (getelementptr inbounds).
+ UraValue *ura_build_gep_field(UraIRContext *ctx, UraValue *struct_address, unsigned field_index, const char *name): address of a struct field. The name is NAME.FIELD in the recorded IR.
+ UraValue *ura_build_gep_chain(UraIRContext *ctx, UraValue *base, unsigned *indices, size_t count, const char *name): several steps at once for nested access (a.b.c).
+ UraValue *ura_address_of(UraIRContext *ctx, UraValue *variable): the address of a variable's storage. The & operator. The core has already checked that the operand is a variable.
+ UraValue *ura_deref(UraIRContext *ctx, UraValue *pointer, const char *name): loads through a pointer (the dref.NAME values).
+ UraValue *ura_ptr_add(UraIRContext *ctx, UraValue *pointer, UraValue *offset, const char *name): pointer arithmetic for the low-level builtins, byte or element based.

### References

+ A reference variable is stored as a pointer in an alloca. A read of the variable loads the pointer then the value; a write-through loads the pointer then stores into it; a rebind stores a new pointer. The functions are in 0.2 (ura_var_declare_ref, ura_ref_write, ura_ref_rebind).
+ A reference parameter receives the caller's address and is used directly: no alloca, no copy. The caller passes ura_address_of(variable).
+ A reference return (fn f() ref T) returns the pointer. The caller binds it to a reference or dereferences it.
+ A nullable reference (ref?) can be null. Reading it through the guard (m?) checks for null first and traps if it is, see below. A raw read would segfault, which is the language's rule.
+ Reference fields in structs are stored as pointers. They are bound, not assigned: ura_ref_rebind on the field address.
+ Taking a reference to a field or an element is the same as taking the address of the GEP.

### Nullable values

+ ura_build_is_null and ura_build_is_not_null (0.2) cover null checks and the isNullable builtin.
+ UraValue *ura_build_guard(UraIRContext *ctx, UraValue *pointer, const char *what, const char *name): the ? operator. Emits the null check; the null branch calls the trap helper with a message that names the expression; the other branch continues. Returns the pointer, now known to be non-null.
+ UraValue *ura_build_fallback(UraIRContext *ctx, UraValue *optional, UraEmitValueFn fallback, void *user, const char *name): the ?? operator. If the optional is not null, its value; otherwise the fallback is emitted in its own block, and a phi merges the two. A chain a ?? b ?? c is right-associative and is built by nesting the calls.
+ A non-null value widens into an optional with no instruction (the same pointer).
+ null needs a target type: ura_const_null takes it.

### Arrays and slices

+ Representation, as recorded for language milestone 2: an array variable is a pointer slot. Its backing storage is a fixed LLVM array ([N x T]) in a separate alloca named array (or a global, or heap memory). Declaring the array stores the address of element 0 into the slot. Indexing loads the pointer from the slot and takes an element GEP on it (getelementptr inbounds i32, i32* %load.a, i32 0). A parameter is the element pointer. Language milestone 8 adds the fat pointer for slices and .len: a struct of the data pointer (T*) and the length (i64). The array attribute len is then the second member, or the constant N for a fixed array. Until then .len on a bare pointer does not exist.
+ UraValue *ura_array_alloca(UraIRContext *ctx, UraType *elem, size_t length, const char *name): the backing storage of a fixed array on the stack, in the entry block, named array. Returns the element pointer slot value: it also stores the address of element 0 into the variable's slot when a variable name is given (ura_array_bind_slot).
+ UraValue *ura_array_alloca_dynamic(UraIRContext *ctx, UraType *elem, UraValue *length, const char *name): a runtime-sized stack array (alloca with a count).
+ UraValue *ura_array_literal(UraIRContext *ctx, UraType *elem, UraValue **values, size_t count, const char *name): allocates the backing array, then stores each element through its own address (named e, e1, e2 ...), as recorded. A literal of constants as one memcpy from a private constant is an optimisation for later.
+ UraValue *ura_array_element(UraIRContext *ctx, UraValue *array, UraValue *index, const char *name): the address of an element: a GEP on the loaded element pointer. The bounds check is added only when the flag bounds_checks is on (it is off in the recorded milestones 1 to 3). The guarded form a[i]? always checks, whatever the flag. Out of bounds calls the trap helper with the index and length in the message.
+ UraValue *ura_array_len(UraIRContext *ctx, UraValue *array, const char *name): the .len attribute, as an i64 or i32 as the core wants (ura_array_len_as with a type).
+ UraValue *ura_slice_make(UraIRContext *ctx, UraValue *data, UraValue *length, const char *name): builds a fat pointer.
+ UraValue *ura_slice_from_array(UraIRContext *ctx, UraValue *array_address, const char *name): decay of a fixed array to a slice.
+ UraValue *ura_slice_range(UraIRContext *ctx, UraValue *source, UraValue *start, UraValue *end, bool checked, const char *name): the view a[1..4]; shares storage, the end is excluded. With checking, a range outside the source traps.
+ UraValue *ura_slice_data(UraIRContext *ctx, UraValue *slice, const char *name) and ura_slice_length for the two members.
+ Multi-dimensional arrays: T[N][M] is an array of arrays. Access is two ura_array_element calls. A sized heap matrix is an array of slices, allocated row by row, and cleaned row by row.
+ Arrays of structs: the element type is the struct type; ura_array_element gives the address of one struct, and fields are reached with ura_build_gep_field on it.
+ Array equality and copy: ura_copy with the array type (memcpy). No element-wise operator.

### Strings

+ A string literal is a private constant i8 array (0.1). Using it as a char pointer (a printf argument) takes its address.
+ A string variable is a writable i8 array: the wrapper allocates the array in the stack and copies the literal in (ura_string_to_array). This is the memcpy seen in the recorded IR.
+ The length of a literal includes the trailing zero in the stored array and excludes it in .len.
+ Escape sequences are decoded by the tokenizer. The wrapper receives raw bytes and a length, so embedded zeros survive.
+ String comparison and concatenation are not wrapper features: the String struct in the standard library does them (language milestone 6).

### Heap

+ UraValue *ura_heap_alloc(UraIRContext *ctx, UraType *type, UraValue *count, const char *name): calloc of count elements of the type (count NULL means one), cast to a pointer of the type. Zeroed, as the language says. Uses the runtime declaration of calloc. The new keyword.
+ UraValue *ura_heap_alloc_dynamic(UraIRContext *ctx, UraType *elem, UraValue *length, const char *name): new T[n], returns a slice.
+ void ura_heap_free(UraIRContext *ctx, UraValue *pointer): calls free, skipping null.
+ void ura_heap_clean(UraIRContext *ctx, UraValue *slot, UraType *type): the clean keyword: frees the value the slot points to (for a multi-dimensional array, each row first, recursively, using a generated helper function) and stores null back into the slot so a later guarded read traps and a second clean does nothing.
+ The default declarations come from the standard library file common.ura (language milestone 6), not from the compiler, but until that exists the wrapper declares calloc, free and malloc itself through ura_runtime_declare (0.6). The recorded IR of milestone 8 will show which.
+ Memory intrinsics: ura_memcpy(UraIRContext *ctx, UraValue *dest, UraValue *src, UraValue *size, unsigned align), ura_memmove, ura_memset(UraIRContext *ctx, UraValue *dest, UraValue *byte, UraValue *size, unsigned align). They declare the llvm.memcpy, llvm.memmove and llvm.memset intrinsics on demand.

### Stack lifetime

+ No lifetime markers (llvm.lifetime.start and end) in this milestone. The entry-block alloca rule already stops stack growth in loops.

## Milestone 0.5: Structs, Fields, Methods, Templates, Enums and Modules

### Struct description: UraStruct

+ struct UraStruct, members:
    - name: char *. The Ura name (Point, Box<i32>).
    - symbol: char *. The LLVM type name (unique even when two scopes declare the same name).
    - type: UraType pointer, the URA_STRUCT type.
    - fields: array of UraField, with count and size (see 0.1: name, type, index, offset, is_ref, default_value).
    - methods: array of UraMethod pointers (below).
    - operators: array of UraMethod pointers, the operator overloads, found by kind.
    - destructor: UraFunc pointer or NULL.
    - copy_assign: UraFunc pointer or NULL (operator =, the deep copy).
    - output_fn: UraFunc pointer or NULL (operator output, or the generated dump function).
    - owner_module: UraScope pointer or NULL.
    - template_of: UraStruct pointer or NULL, the template this is an instance of.
    - template_args: array of UraType pointers.
    - declared_in: UraScope pointer, the scope that owns the name.
    - is_defined: bool. False between ura_struct_declare and ura_struct_define.
    - size, align: size_t, filled when defined.
    - owns: array of field indexes whose type has a destructor, filled by ura_struct_define so the generated destructor knows what to drop.
    - has_ref_fields: bool.

### ura_codegen_struct

+ UraStruct *ura_struct_declare(UraIRContext *ctx, const char *name): the first step. Creates the named opaque LLVM struct type and registers the struct in the current scope. The core calls this for every struct of a scope before it emits any code that mentions one, which is how a struct is usable before its declaration. Declaring a name that already exists in the same scope is an internal error.
+ bool ura_struct_define(UraIRContext *ctx, UraStruct *s, const UraFieldSpec *fields, size_t count): the second step. UraFieldSpec holds the name, the type, the is_ref flag and an optional default. Sets the LLVM body (LLVMStructSetBody) in field order with natural alignment (not packed), then reads back the offsets, the size and the alignment from the target data, and fills the field table. Returns false if a field makes the struct contain itself by value, directly or through other structs.
+ UraStruct *ura_codegen_struct(UraIRContext *ctx, const char *name, const UraFieldSpec *fields, size_t count, const UraStructBindings *bindings): the one-call form for a struct whose field types are all known. It runs ura_struct_declare (or reuses the declared one), ura_struct_define, then binds the methods listed in bindings. UraStructBindings is a struct of: methods (array of UraMethodSpec), method_count, operators (array of UraMethodSpec), operator_count, destructor (UraMethodSpec pointer or NULL). Each UraMethodSpec carries the Ura name, the function type, the flags (static, operator kind), and a callback UraEmitBodyFn that emits the body. The wrapper declares all the methods first (so methods can call each other in any order), then emits the bodies one by one, with self as the first parameter.
+ The unused-struct rule: a struct no code refers to produces no type in the .ll, because LLVM only prints types that are used. The wrapper does nothing special for that.
+ Local structs: a struct declared inside a function or block gets a type name that is unique across the module; its methods are emitted as top-level functions with mangled names, using the cursor push and pop.
+ Struct layout queries: ura_struct_field_index(UraStruct *s, const char *name) returns the index or -1 (the core has already checked). ura_struct_field_type, ura_struct_field_offset, ura_struct_size, ura_struct_align, ura_struct_field_count.
+ Padding: fields follow the target's ABI alignment, so sizeof gives the padded size. The wrapper never reorders fields.

### Fields

+ UraValue *ura_field_address(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, const char *name): GEP to the field. The name is object.field when the core does not pass one.
+ UraValue *ura_field_load(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, const char *name).
+ void ura_field_store(UraIRContext *ctx, UraValue *struct_address, UraStruct *s, const char *field, UraValue *value).
+ UraValue *ura_field_chain_address(UraIRContext *ctx, UraValue *base, UraStruct *s, const char **fields, size_t count, const char *name): a.b.c in one call, following nested structs, and following reference fields (a reference field is loaded and then used as the base of the next step).
+ Struct values: ura_struct_zero(UraIRContext *ctx, UraValue *address, UraStruct *s) zeroes a struct (the declaration without value); with default values on fields, it stores them after the zeroing. ura_struct_copy(ctx, dest, src, s) copies with memcpy, or calls copy_assign if the struct has operator =, which is the deep copy case.
+ Passing structs, as recorded: by value passes the LLVM aggregate itself (define i32 @bump(%Point %0)) and the callee stores it into an alloca named after the parameter. By reference passes the address (%User*). The receiver of a method is always a pointer (define i32 @User.get(%User* %0)); an operator's right operand is by value (define %Vec @Vec.add(%Vec* %0, %Vec %1)).
+ Returning structs, as recorded: the function returns the aggregate (define %User @make(i32 %0), ret %User %load.u). No hidden out-parameter. Decision D4 is therefore settled for the C-level ABI question only partly: aggregates returned to C code would not follow the platform ABI, so protos should use pointers.
+ A copy of a struct inside a function (assignment, p1 = p2) is the llvm.memcpy intrinsic, as recorded; a struct moving across a call is a whole-value load or store.
+ Comparing and ordering structs is never built-in; it goes through an operator overload.

### Methods and member functions

+ enum UraOperatorKind: URA_OP_NONE (an ordinary method), URA_OP_ADD, URA_OP_SUB, URA_OP_MUL, URA_OP_DIV, URA_OP_MOD, URA_OP_EQ, URA_OP_NE, URA_OP_LT, URA_OP_GT, URA_OP_LE, URA_OP_GE, URA_OP_ADD_ASSIGN, URA_OP_SUB_ASSIGN, URA_OP_MUL_ASSIGN, URA_OP_DIV_ASSIGN, URA_OP_MOD_ASSIGN, URA_OP_AND, URA_OP_OR, URA_OP_XOR, URA_OP_SHL, URA_OP_SHR, URA_OP_ASSIGN, URA_OP_INDEX, URA_OP_OUTPUT, URA_OP_DROP.
+ struct UraMethod, members: name (the Ura name), owner (UraStruct pointer), func (UraFunc pointer), op (UraOperatorKind), is_static (pub fn: no self, called with ::), self_by_ref (the receiver is a pointer; always true for instance methods), param_types (array), ret_type (UraType pointer), returns_ref (bool), is_destructor.
+ UraMethod *ura_method_declare(UraIRContext *ctx, UraStruct *s, const char *name, UraType *func_type, UraOperatorKind op, bool is_static): creates the function with the mangled symbol and the receiver as first parameter named self (when not static), registers it in the struct. The function type passed in does not include the receiver; the wrapper adds a pointer to the struct.
+ void ura_method_begin(UraIRContext *ctx, UraMethod *method) and void ura_method_end(UraIRContext *ctx): like ura_func_begin and ura_func_end. In the body, self is a reference-like parameter: reading an attribute goes through dref.self and then the field address (the recorded IR has dref.self and load.dref.self.a names).
+ Overloading: several methods may share a name if their parameter types differ. UraMethod *ura_method_lookup(UraStruct *s, const char *name, UraType **arg_types, size_t count, UraOperatorKind op): finds the exact match. The core does the language-level checking and error messages ("no method X, the struct has ..."); the wrapper lookup is only for choosing the function to call.
+ UraValue *ura_call_method(UraIRContext *ctx, UraValue *self_address, UraMethod *method, UraValue **args, size_t count, const char *name): passes the receiver first. A chain like a.get().next.get() is the core calling this and ura_field_address in sequence; a method on a returned reference passes the loaded pointer.
+ UraValue *ura_call_static(UraIRContext *ctx, UraMethod *method, UraValue **args, size_t count, const char *name): Type::method().
+ UraValue *ura_call_operator(UraIRContext *ctx, UraOperatorKind op, UraValue *left_address, UraValue *right, const char *name): calls the overload found for the left operand's struct and the right operand's type.
+ Method on array element or nested field: the core passes the element address or the field address as the receiver. Nothing special.
+ Symbol mangling: decision D1 in Appendix D. The proposed scheme keeps the .ll readable: a method is Type.method (Vec.drop, Counter.get), a module member is module.name (nested: a.b.f), a template instance is Box<i32>.method, an operator is Type.operator+ (or a letter code if '+' is not accepted in names; LLVM accepts any name when it is quoted), an overload adds the parameter types after the name only when needed to tell two apart, a nested function is outer.inner. The $ idea in the old TODO is an alternative for the separator.

### Destructors and ownership

+ A struct has a destructor when it declares operator drop, or when one of the fields it owns has one (then the wrapper generates a destructor that drops the owned fields in reverse order).
+ UraFunc *ura_struct_destructor(UraIRContext *ctx, UraStruct *s): returns the user destructor, the generated one, or NULL if the struct needs none.
+ void ura_struct_register_cleanup(UraIRContext *ctx, UraValue *variable, UraStruct *s): adds a cleanup to the current scope if the struct has a destructor. The core calls it after each declaration of a struct variable, which keeps drop order = reverse declaration order.
+ A destructor must take no parameter and return void: the core reports that, the wrapper assumes it.
+ A reference field is borrowed, not owned: it is never dropped by the outer struct.
+ clean does not run a destructor and a destructor does not free the struct: the language keeps them separate (the legacy tests say clean on a struct with operator drop does not destroy it).

### Templates (generic structs)

+ struct UraTemplateKey, members: template_name (const char *), args (array of UraType pointers), count.
+ UraStruct *ura_template_instance(UraIRContext *ctx, const UraTemplateKey *key, bool *created): returns the existing instance for these arguments, or creates a new declared (opaque) struct named Name<arg1,arg2> and sets created to true. The core, seeing created, substitutes the type parameters in the template's fields and methods and calls ura_struct_define and ura_method_declare for the instance. Two uses of Box<i32> share one struct.
+ Instances inside templates (a template used inside itself, nested templates such as Box<Box<i32>>) are created on first use, and the opaque declaration breaks the cycle.
+ Type parameters never reach the wrapper: only instantiated types do. sizeof of a type parameter is sizeof of the instance's argument, resolved by the core.
+ Template functions (a function called with type arguments) follow the same scheme with ura_func_instance(UraIRContext *ctx, const UraTemplateKey *key, UraType *func_type, bool *created).
+ Instances are emitted at their first use; their names follow the mangling rules.

### Enums

+ UraType *ura_enum_define(UraIRContext *ctx, const char *name, const char **variants, long long *values, size_t count): creates the enum type and the variant constants. Values are given resolved by the core (auto-numbering and explicit values reset the counter in the core).
+ UraValue *ura_enum_variant(UraIRContext *ctx, UraType *enum_type, const char *variant).
+ const char *ura_enum_variant_name(UraType *enum_type, long long value): used to generate printing code.
+ A variant used before its enum is declared works because the core declares enums in its first pass.
+ Match on an enum uses ura_codegen_match (0.3).

### Modules

+ A module (mod name:) is a scope kind that prefixes the symbol of everything declared in it.
+ UraScope *ura_module_open(UraIRContext *ctx, const char *name): opens (or reopens, for a module declared in two places) a module scope, nested modules prefix with the parent's name. A module can be referenced before its declaration because the core declares module scopes in its first pass.
+ void ura_module_close(UraIRContext *ctx).
+ char *ura_qualify(UraIRContext *ctx, const char *name): the full symbol name with the module prefix, for function and struct symbols.
+ A function inside a module calls its siblings without the prefix: ura_symbol_lookup searches the module scope before the global one.
+ Files loaded by use are not separate wrapper modules: they are emitted into the same LLVM module, the first time only (the core tracks which files were loaded; the wrapper only ever sees each declaration once).

## Milestone 0.6: Runtime Support, Builtins, C Interop and Debug Info

### Runtime declarations

+ struct UraRuntimeDecls, members: one UraFunc pointer per libc function the wrapper uses: printf, dprintf, calloc, malloc, free, write, exit, abort, strlen, memcmp, getenv, plus the memory intrinsics (llvm.memcpy, llvm.memmove, llvm.memset), filled on demand.
+ UraFunc *ura_runtime_declare(UraIRContext *ctx, const char *name): declares (once) the libc function with its known signature and returns it. If the program already declared it with proto, the existing declaration is reused: identical declarations collapse into one, and a different signature is a diagnostic.
+ The goal is that the compiler does not hardcode these, the standard library file does (language milestone 6). Until then the wrapper's declarations are the fallback.

### Traps and runtime errors

+ A trap is: write a message to file descriptor 2, then exit with status 1. The message has the form file:line: text, built from the source name and the location.
+ UraFunc *ura_trap_helper(UraIRContext *ctx, UraTrapKind kind): returns (creating once) a small noreturn function for each kind: URA_TRAP_BOUNDS (index out of range), URA_TRAP_NULL (null reference or pointer used), URA_TRAP_DIV_ZERO (division by zero), URA_TRAP_NULL_CALL (a null function value called), URA_TRAP_UNCAUGHT (a throw nobody caught), URA_TRAP_OOM (calloc returned null), URA_TRAP_CUSTOM (message passed as an argument). Each helper takes the line number and, for bounds, the index and the length.
+ void ura_codegen_trap_if(UraIRContext *ctx, UraValue *condition, UraTrapKind kind, const char *detail): emits a conditional branch to a cold block that calls the helper and ends with unreachable, and continues in the other block. The branch blocks are named trap and ok.
+ Stack overflow from infinite recursion is left to the operating system (a segmentation fault), the language only promises that it does not run forever. With the sanitizer on, the sanitizer reports it.
+ Every trap kind can be switched off by a flag for release builds, except division by zero and explicit guards.

### Output builtins

+ output(a, b, c) prints each argument by its type with no format string. The core does the walk over the arguments; the wrapper offers the emitters.
+ void ura_codegen_print(UraIRContext *ctx, UraValue *value, UraType *type, UraPrintTarget target): target is URA_STDOUT or URA_STDERR (errput). Emits the right printf or dprintf call: %d for i8-as-number, i16, i32, %ld for i64, %u and %lu for unsigned, %f for floats (f32 promoted to double), %s for strings, %c for chars, True and False for a bool (a select between two string constants), the pointer as %p, an enum as the variant name (a generated lookup switch or a table of private strings).
+ void ura_codegen_print_string(UraIRContext *ctx, const char *text, UraPrintTarget target): a literal piece (a separator, a newline).
+ UraFunc *ura_print_helper(UraIRContext *ctx, UraType *type, UraPrintTarget target): for aggregates, a generated function that prints the value: a struct as Name(field: value, ...) or the layout the old output used, an array as [a, b, c] (two dimensions nested), a reference field by following it and printing null if unbound, a cycle as [Circular] (the helper carries a visited marker). A struct with operator output uses it instead. Generated once per type and target.
+ The colour pair for errput and the exact formats are given by the language milestone 6 tests; the wrapper only provides the emitters.

### sizeof and typeof

+ sizeof is a constant: UraValue *ura_build_sizeof(UraIRContext *ctx, UraType *type, UraType *result_type) uses ura_sizeof and makes an integer constant. sizeof of a value is sizeof of its type, with array types giving the total.
+ typeof is resolved by the core into a string (ura_type_to_string) and a string literal.

### Program entry

+ UraFunc *ura_func_begin_main(UraIRContext *ctx, bool takes_args): begins i32 main() or, when the program reads os.argc and os.argv, i32 main(i32 argc, i8** argv), storing them in the os global. For a file with no main it is created by script mode (0.2).
+ void ura_main_prologue(UraIRContext *ctx): emits the code that initialises globals with computed values and runs the code written outside of functions, in file order. The core calls it once at the start of main.
+ void ura_main_epilogue(UraIRContext *ctx, UraValue *exit_code): runs the destructors of globals in reverse order, checks the uncaught-error flag, and returns the code. Script mode returns zero.
+ The os global: a global struct (argc, argv) filled by the prologue when used. Its get method is getenv.

### C interop

+ A proto is ura_func_declare with external linkage and, when it is variadic, the variadic flag. Types map directly: i8 is char, i32 is int, i64 is long, pointers are pointers. A struct passed by value to C does not follow the platform ABI (the recorded form is a plain LLVM aggregate, see D4); pass pointers to C.
+ Linking: link "name" adds a library to the list kept in the context (ura_ir_add_link(UraIRContext *ctx, const char *library)); the list becomes -l flags for the link step in 0.7. URA_LINK_name environment variables give the path.
+ Calling conventions: C only. ura_func_set_calling_convention is reserved.
+ Global variables from C (extern) use URA_EXTERN_DECL.
+ Callbacks to Ura functions from C: a function value has the plain C function pointer type, so it can be passed to a proto that wants a callback.
+ Variadic calls follow 0.2.
+ Platform conditionals (@if) are resolved by the core before the wrapper sees anything.

### Debug info

+ struct UraDebugInfo, members: builder (LLVMDIBuilderRef), compile_unit (LLVMMetadataRef), file (LLVMMetadataRef), scopes (a stack of LLVMMetadataRef, one per function and block), type_cache (a map from UraType to its metadata), enabled, line_tables_only (a cheaper mode that records only line numbers).
+ void ura_debug_enable(UraIRContext *ctx, const char *filename, const char *directory, bool full): creates the DIBuilder, the compile unit and the file. Called before the first function.
+ Per function: ura_func_begin creates the subprogram and sets it on the function when debug is on. Per variable: ura_var_declare creates a local variable entry and an llvm.dbg.declare call. Per statement: ura_set_location sets the current debug location.
+ void ura_debug_finalize(UraIRContext *ctx): LLVMDIBuilderFinalize, once, before verification and printing.
+ Debug info is off by default so the recorded IR is unchanged. The -san flag turns on line tables and keeps the frame pointer.

## Milestone 0.7: Optimizer Hooks and Backend

### Goal

+ Optimisation can be switched on later without touching the frontend: the core sets a level on the context and calls one function before output. Level 0 does nothing and leaves the IR exactly as emitted.

### Settings

+ enum UraOptLevel: URA_O0, URA_O1, URA_O2, URA_O3, URA_OS, URA_OZ.
+ struct UraOptimizer, members:
    - level: UraOptLevel.
    - size_level: unsigned (0 for speed, 1 for -Os, 2 for -Oz).
    - inline_threshold: unsigned (0 means the level's default).
    - unroll_loops, vectorize_loops, vectorize_slp: bool.
    - verify_each: bool (verify after each pass, for debugging a pass).
    - function_pm: LLVMPassManagerRef. The per-function pass manager (used with LLVM 12 and earlier through the legacy interface).
    - module_pm: LLVMPassManagerRef. The module-level manager (inlining, global optimisation, dead code removal).
    - builder: LLVMPassManagerBuilderRef (legacy interface in LLVM 12).
    - pass_builder_options: LLVMPassBuilderOptionsRef (new pass manager, LLVM 13 and later).
    - pipeline: char *. The textual pipeline for the new pass manager (default<O2> and so on, or a user string).
    - custom_passes: array of UraPassSpec, passes added on top of the level.
    - initialized: bool.
+ struct UraPassSpec, members: name (const char *), stage (URA_PASS_FUNCTION or URA_PASS_MODULE), after_default (bool, run after the standard pipeline or before it).
+ Known pass names to support through custom_passes: mem2reg, instcombine, simplifycfg, reassociate, gvn, sroa, early-cse, dce, adce, licm, loop-unroll, loop-vectorize, inline, globaldce, constprop, sccp, tailcallelim, jump-threading, deadargelim.

### Functions

+ void ura_opt_set_level(UraIRContext *ctx, UraOptLevel level): sets the level and the size level, from the -O0 ... -O3, -Os, -Oz flags. Marks the optimizer as not initialised.
+ void ura_opt_set_option(UraIRContext *ctx, UraOptOption option, long value): inline threshold, unroll, vectorize, verify_each.
+ void ura_opt_add_pass(UraIRContext *ctx, const char *name, UraPassStage stage, bool after_default): registers an extra pass.
+ bool ura_opt_init(UraIRContext *ctx): creates the managers. With LLVM 12 and earlier: LLVMCreateFunctionPassManagerForModule, LLVMCreatePassManager, LLVMPassManagerBuilderCreate, LLVMPassManagerBuilderSetOptLevel, LLVMPassManagerBuilderSetSizeLevel, LLVMPassManagerBuilderUseInlinerWithThreshold, LLVMPassManagerBuilderPopulateFunctionPassManager, LLVMPassManagerBuilderPopulateModulePassManager, then adds the custom passes, then LLVMInitializeFunctionPassManager. With LLVM 13 and later (selected by the LLVM_VERSION_MAJOR macro, as 003 already does for its includes): LLVMCreatePassBuilderOptions and the pipeline string.
+ bool ura_opt_run_function(UraIRContext *ctx, UraFunc *func): runs the function pass manager on one function (LLVMRunFunctionPassManager). Can be called right after ura_func_end for per-function streaming.
+ bool ura_opt_run(UraIRContext *ctx): the one call the core makes before output. At level 0 it returns at once. Otherwise: initialise if needed, run the function passes over every function with a body, run the module passes (LLVMRunPassManager, or LLVMRunPasses with the target machine on LLVM 13 and later), finalise the function manager (LLVMFinalizeFunctionPassManager), and verify the module.
+ void ura_opt_dispose(UraIRContext *ctx): LLVMDisposePassManager on both, LLVMPassManagerBuilderDispose, LLVMDisposePassBuilderOptions.
+ Where it is called: ura_ir_emit_ll calls ura_opt_run first when the level is not 0, so the printed IR is the optimised one. A second entry, ura_ir_emit_ll_unoptimized, writes the IR before the passes, for comparing.
+ The recorded tests are always compiled at -O0. A test for an optimisation level is a different file and is not part of milestones 1 to 13.

### Backend: object code and executable

+ bool ura_emit_object(UraIRContext *ctx, const char *path): uses the target machine to write an object file (LLVMTargetMachineEmitToFile with LLVMObjectFile). Needs ura_ir_set_target.
+ bool ura_emit_assembly(UraIRContext *ctx, const char *path): the same with LLVMAssemblyFile.
+ bool ura_build_executable(UraIRContext *ctx, const char *exe_path, const UraLinkOptions *options): writes the .ll (or an object) and runs clang to produce the executable, as gen_bin did: clang file.ll -o exe. UraLinkOptions holds: libraries (array of const char *, -l), library_paths (-L), extra_flags (array), sanitize (adds -fsanitize=address,undefined), debug (adds -g and -fno-omit-frame-pointer), opt_level (passed to clang only when the optimizer did not already run), verbose.
+ int ura_run_executable(const char *exe_path, char **args): for the -exec flag. Forks, runs, waits, returns the exit status. Lives in ir.c because the build step lives there, but it uses no LLVM.
+ The paths follow the task shell contract: the IR goes to build/out.ll below the directory the compiler runs in, and the executable path comes from -o.
+ Cross compilation: ura_ir_set_target with another triple; the link step then needs a matching clang target. Not tested in this milestone.

## Appendix A: Which wrapper parts each language milestone uses

+ Language milestone 1 (programs that compute): 0.1 (contexts, i8, i32, bool, constants, globals, string literals, symbols, output), 0.2 (functions, protos, variables, arithmetic, comparisons, casts excluded, calls, variadic calls, script main), 0.3 (branches, ura_codegen_conditional, while, break, continue, return, dead blocks), the string literal part of 0.4.
+ Language milestone 2 (memory and data): the rest of 0.4 (arrays, slices as parameters, references, null, nullable guard), casts of 0.2.
+ Language milestone 3 (structs): 0.5 without templates, enums and modules; the struct copy of 0.2.
+ Language milestone 4 (templates): ura_template_instance and ura_func_instance.
+ Language milestone 5 (modules and imports): ura_module_open, ura_qualify, ura_ir_add_link, runtime declarations of 0.6, the os global and program entry parameters.
+ Language milestone 6 (output and strings): the print emitters and helpers of 0.6.
+ Language milestone 7 (enums and match): ura_enum_define and ura_codegen_match.
+ Language milestone 8 (heap, drop, array extras): heap functions and the cleanups of 0.3, slices and bounds checks of 0.4.
+ Language milestone 9 (operator overloading, static methods): UraMethod operators and ura_call_operator, ura_call_static.
+ Language milestone 10 (loops): ura_codegen_loop, ura_codegen_for_range, ura_codegen_for_each.
+ Language milestone 11 (more types and operators): the unsigned and float paths of 0.2, the other integer widths in 0.1, shifts, float traps.
+ Language milestone 12 (nullable and errors): ura_build_fallback, optional types, the try and throw machinery.
+ Language milestone 13 (function values and directives): ura_func_address, ura_call_indirect, null function trap. Directives are the core's.
+ 0.7 (optimizer, backend) is independent. The executable building part is needed from the first milestone, since the test runner builds programs.

## Appendix B: LLVM calls of 003 and where they went

This is the list of what the core did directly in 003. Each group is now inside one wrapper function family.

+ Context, module and builder setup (LLVMContextCreate, LLVMModuleCreateWithNameInContext, LLVMCreateBuilderInContext): ura_ir_create.
+ Target setup (LLVMInitializeNative*, LLVMGetDefaultTargetTriple, LLVMGetTargetFromTriple, LLVMCreateTargetMachine, LLVMCreateTargetDataLayout, LLVMSetModuleDataLayout, LLVMSetTarget): ura_ir_create and ura_ir_set_target.
+ Types (LLVMVoidTypeInContext, LLVMIntTypeInContext, LLVMPointerType, LLVMArrayType, LLVMFunctionType, LLVMStructCreateNamed, LLVMStructSetBody, LLVMGetElementType, LLVMGetIntTypeWidth, LLVMTypeOf): the ura_type_ family and ura_value_type, ura_value_pointee.
+ Constants and globals (LLVMConstInt, LLVMConstNull, LLVMConstStringInContext, LLVMAddGlobal, LLVMSetInitializer, LLVMSetGlobalConstant, LLVMSetLinkage): the ura_const_ family, ura_global_create and ura_string_literal.
+ Functions (LLVMAddFunction, LLVMGetParam): ura_func_declare, ura_func_begin, ura_func_param.
+ Blocks (LLVMAppendBasicBlockInContext, LLVMPositionBuilderAtEnd, LLVMPositionBuilderBefore, LLVMMoveBasicBlockAfter, LLVMGetInsertBlock, LLVMGetBasicBlockParent, LLVMGetEntryBasicBlock, LLVMGetLastBasicBlock, LLVMGetFirstInstruction, LLVMGetNextInstruction, LLVMIsAAllocaInst, LLVMGetBasicBlockTerminator): the ura_block_ family and the cursor, including the alloca-in-entry rule (ura_var_alloca replaces the walk over entry instructions).
+ Instructions (LLVMBuildAlloca, LLVMBuildLoad, LLVMBuildStore, LLVMBuildBinOp, LLVMBuildICmp, LLVMBuildNot, LLVMBuildIsNull, LLVMBuildMemCpy, LLVMBuildBr, LLVMBuildCondBr, LLVMBuildRet, LLVMBuildRetVoid, LLVMBuildCall, LLVMBuildStructGEP, LLVMBuildGEP, the cast builders): the ura_build_ family, ura_load, ura_store, ura_copy, ura_call and the control flow functions of 0.3.
+ Verification and output (LLVMVerifyModule, LLVMPrintModuleToFile, LLVMDisposeMessage, LLVMWriteBitcodeToFile): ura_ir_verify, ura_ir_emit_ll, ura_ir_emit_bitcode.
+ Optimisation (LLVMCreatePassBuilderOptions, LLVMRunPasses, LLVMDisposePassBuilderOptions, LLVMGetErrorMessage, LLVMDisposeErrorMessage): 0.7.
+ Debug info (the DIBuilder calls and LLVMDisposeDIBuilder): 0.6.
+ Disposal (LLVMDisposeTargetMachine, LLVMDisposeTargetData): ura_ir_dispose.
+ The Asm struct inside Token (elem, bloc, type, cond, then, end for the while loop) disappears: loop blocks live in UraLoopFrame, variable storage lives in UraSymbol, types live in UraType.

## Appendix C: Implementation order and checkpoints

+ Step 1: UraIRContext, create and dispose, the type cache for void, bool, i8, i32, pointers, constants, ura_ir_emit_ll. Checkpoint: build a module with one function that returns 42 by calling only ura_ functions, compile it with clang, run it, exit status 42.
+ Step 2: functions, entry block and the alloca rule, variables, load and store, arithmetic, comparisons. Checkpoint: the variables and operators entries of milestone1.md produce the same IR as recorded.
+ Step 3: branches, ura_codegen_conditional, ura_codegen_while, break, continue, return with the ended-block rule. Checkpoint: the if and while entries.
+ Step 4: calls, protos, variadic promotion, string literals, globals, script main. Checkpoint: all of milestone1.md passes with the wrapper under the new core.
+ Step 5: arrays, slices, references, nullable guard, casts. Checkpoint: milestone2.md.
+ Step 6: structs, fields, methods, copy. Checkpoint: milestone3.md.
+ Step 7: templates. Checkpoint: milestone4.md.
+ Step 8: the rest, in the order of the language milestones 5 to 13, adding the parts of Appendix A as each one starts.
+ Step 9: the optimizer and debug info, whenever there is time. Checkpoint: the same programs at -O2 give the same exit codes and output (compared by running, not by IR).
+ At every step the verification is the same: tests, then a grep for llvm-c outside ir.c finds nothing, and the module verifier passes.

## Appendix D: Open decisions

+ D1, symbol mangling: Type.method, module.name, Box<i32>.method as proposed in 0.5, or a $ separator. Decide before the first method is emitted, since it appears in the recorded IR.
+ D2, short-circuit and and or: check the recorded IR in milestone1.md and match it (see 0.2).
+ D3, array parameters: the recorded IR passes a bare element pointer and a separate length argument, so that is the form for language milestones 1 to 7. The fat pointer (0.4) arrives with slices and .len in milestone 8; decide then whether older tests are recorded again or whether bare pointers stay valid next to slices.
+ D4, structs by value across calls and returns: settled by the recorded IR of milestone 3, which uses true LLVM aggregate values for parameters and results. What stays open is the C ABI: a proto that takes or returns a struct by value will not match C on every platform, so the rule is to pass pointers to C.
+ D5, error handling: error slot (Design A) or LLVM exception handling (Design B), see 0.3. The proposal is A.
+ D6, bounds checks and division checks: the recorded IR of milestones 1 to 3 has none, so they are off by default. They turn on with the first language milestone that records them (division by zero in milestone 11, the explicit guard x? and the slice checks in milestone 8). Decide whether they become the default after that, and whether release builds can switch them off.
+ D7, integer overflow: wrapping now, checked arithmetic later (a flag), see 0.2.
+ D8, optional scalars (i32?): a pair of a flag and a value, or not supported. Proposed: not supported until a milestone needs them.
+ D9, a deferred IR: not now; the API allows it later (see the design decision).
+ D10, one context per compilation or per file: one per compilation, with ura_ir_reset available.
+ D11, multi-threaded compilation: not supported. The context is not thread safe.
+ D12, strings as language-level values: only through the String struct of the standard library. The wrapper knows only i8 arrays.
+ D13, 128-bit integers, vectors (SIMD), atomics, threads: reserved. The type kinds can be added without changing existing signatures.

## Appendix E: Checklist for the finished milestone

+ A grep for llvm-c finds it only in src/ir.c.
+ main.c contains no identifier starting with LLVM.
+ No Token member holds a wrapper or LLVM handle.
+ The generated .ll for every entry of milestones 1 to 4 equals the recorded IR.
+ ura_ir_verify passes on all of them.
+ The optimizer at level 0 changes nothing, at level 2 the programs still produce the same output.
+ src/proto.h lists the ura_ functions (the ctags line of config.sh scans ir.c).
+ This file has been updated for every decision of Appendix D that was taken.
