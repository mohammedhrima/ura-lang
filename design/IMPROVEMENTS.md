# Roadmap: 5 architectural improvements for Ura

## Context

After today's seven-gap fixes the compiler is at ~70/100. Five workstreams separate it from ~85. Each is independent — pick in any order.

---

## ① Error Recovery — show many errors per run

```
ERROR RECOVERY
│
├── PROBLEM
│   ├── Today: first error → STOP
│   │   └── example:
│   │       file with 6 errors → 6 recompiles
│   │
│   └── Goal: see all 6 errors in one run
│
├── STEPS
│   │
│   ├── (1) Counter instead of a flag
│   │   └── example:
│   │       // src/utils.c:3
│   │       bool found_error;          ← before
│   │       int  error_count = 0;      ← after
│   │
│   ├── (2) Define "sync points" (recovery anchors)
│   │   └── example — places where the parser can resume cleanly:
│   │       • NEWLINE at the same indent
│   │       • dedent to a parent scope
│   │       • top-level keywords: fn / struct / enum / mod / pub / use
│   │
│   ├── (3) parser_recover() helper
│   │   └── example:
│   │       // src/parse.c
│   │       void parser_recover(int target_indent) {
│   │           while (!found_sync_point()) exe_count++;
│   │       }
│   │
│   ├── (4) Stop short-circuiting on error
│   │   └── example:
│   │       // before
│   │       if (!fname) {
│   │           parse_error(...);
│   │           return syntax_error();   ← halts everything
│   │       }
│   │       // after
│   │       if (!fname) {
│   │           parse_error(...);
│   │           parser_recover(token->space);
│   │           return syntax_error();   ← just the sentinel, parsing continues
│   │       }
│   │
│   ├── (5) SYNTAX_ERROR node = silent poison
│   │   └── example:
│   │       void ir_gen(Node *node) {
│   │           if (node->token->type == SYNTAX_ERROR) return;  ← skip
│   │           ...
│   │       }
│   │
│   └── (6) Cap reported errors (avoid noise)
│       └── example:
│           if (error_count >= 25) {
│               fprintf(stderr, "... and %d more errors\n", remaining);
│               exit(1);
│           }
│
├── FILES
│   ├── src/utils.c       (counter)
│   ├── src/parse.c       (recover + ~30 error sites)
│   ├── src/gen.c         (poison pass-through)
│   └── src/main.c        (top-level loop + exit code)
│
└── EFFORT: ~1 week, mechanical but spread across many files
```

---

## ② Exhaustive Semantic Checks

```
SEMANTIC CHECKS
│
├── PROBLEM
│   ├── Reactive pattern: a user trips on a bug → we add the check
│   └── Industry: list every type rule UP FRONT, then implement them
│
├── CHECKLIST TO COVER
│   │
│   ├── CONDITIONS
│   │   └── example:
│   │       if 5: ...           ← should be bool, not int
│   │       while "hello": ...  ← same
│   │
│   ├── ARRAY INDEX
│   │   └── example:
│   │       arr["key"]          ← non-int key → error
│   │
│   ├── INFINITE-SIZE TYPE
│   │   └── example:
│   │       struct A:
│   │           x A             ← infinite size → error
│   │       (`x A ref` is fine — ref is a pointer)
│   │
│   ├── OVERLOADED OPERATOR SIGNATURE
│   │   └── example:
│   │       struct Vec:
│   │           operator + () Vec:    ← `+` requires one parameter
│   │               return self
│   │
│   ├── TUPLE ARITY
│   │   └── example:
│   │       fn pair() (int, int):
│   │           return 1            ← 1 value where 2 required
│   │
│   ├── USING THE RETURN OF A VOID FN
│   │   └── example:
│   │       fn print() void: ...
│   │       x int = print()         ← void has no value
│   │
│   ├── BREAK / CONTINUE OUTSIDE A LOOP
│   │   └── example:
│   │       fn foo() void:
│   │           break               ← not in a loop
│   │
│   ├── SELF OUTSIDE A METHOD
│   │   └── example:
│   │       fn bar() void:
│   │           self.x = 1          ← `self` doesn't exist here
│   │
│   ├── PUB FN USING SELF
│   │   └── example:
│   │       struct Foo:
│   │           pub fn make() Foo:
│   │               self.x = 1      ← pub fn has no self
│   │
│   ├── UNINITIALIZED READ
│   │   └── example:
│   │       x int
│   │       output(x)               ← read before assign
│   │
│   ├── UNREACHABLE CODE (warning)
│   │   └── example:
│   │       fn f() int:
│   │           return 1
│   │           output("never")     ← unreachable
│   │
│   └── UNUSED VARIABLE (warning)
│       └── example:
│           x int = 5               ← never read
│
├── APPROACH
│   └── 1 check per session → also add 1 fixture in
│       src/tests/errors/<category>/NNN.ura for each
│
└── EFFORT: ongoing, ~20–40h total
```

---

## ③ Multi-Pass Architecture

```
MULTI-PASS — separate the responsibilities
│
├── EXPLAINING THE PROBLEM
│   │
│   ├── Today your ir_assign() does 3 things at once:
│   │   │
│   │   ├── (a) NAME RESOLUTION
│   │   │   └── example: get_variable("x") → finds the declaration
│   │   │
│   │   ├── (b) TYPE CHECKING
│   │   │   └── example: compatible(left, right) → verifies types
│   │   │
│   │   └── (c) CODEGEN PREP
│   │       └── example:
│   │           node->token->ret_type = ...
│   │           node->token->Fcall.ptr = func
│   │           these fields get read later by asm_gen
│   │
│   └── All three mixed together = pain:
│       │
│       ├── Risky: adding a check can break the prep
│       │   └── example:
│       │       I add a new check inside ir_assign,
│       │       accidentally I return before setting ret_type,
│       │       asm_gen later segfaults because ret_type=0
│       │
│       ├── No "type-check only" mode
│       │   └── example:
│       │       a Language Server for your VSCode extension
│       │       does NOT need to emit LLVM,
│       │       it only wants "are there type errors?"
│       │       Today: impossible without doing everything
│       │
│       └── Hard to reason about
│           └── example:
│               "where is the type INT decided for x+y ?"
│               answer: maybe ir_id, maybe ir_binary_op,
│               maybe set_ret_type. No single answer.
│
├── EXPLAINING THE SOLUTION
│   │
│   ├── Three separate passes, each with ONE job:
│   │
│   │   parse() → AST
│   │       ↓
│   │   ┌─────────────────────────────────────────────┐
│   │   │ PASS A : resolve()                          │
│   │   │  - every ID has a pointer to its decl       │
│   │   │  - every fcall knows which function it hits │
│   │   │  - NO type checking                         │
│   │   │  - NO codegen state                         │
│   │   └─────────────────────────────────────────────┘
│   │       ↓
│   │   ┌─────────────────────────────────────────────┐
│   │   │ PASS B : typecheck()                        │
│   │   │  - every node has a known type              │
│   │   │  - assign / call / return verified          │
│   │   │  - errors emitted HERE                      │
│   │   │  - NO codegen state                         │
│   │   └─────────────────────────────────────────────┘
│   │       ↓
│   │   ┌─────────────────────────────────────────────┐
│   │   │ PASS C : codegen()                          │
│   │   │  - emits LLVM IR                            │
│   │   │  - NO checks                                │
│   │   │  - NO name lookups                          │
│   │   │  - assumes the AST is well-typed            │
│   │   └─────────────────────────────────────────────┘
│   │
│   └── Each pass has a clear CONTRACT
│       └── example:
│           "After resolve(), every ID has a non-NULL Decl ptr"
│           "After typecheck(), every node has node->resolved_type"
│
├── HOW TO MIGRATE (without breaking the existing build)
│   │
│   ├── (1) Add a Type field on Node
│   │   └── example:
│   │       // src/header.h - struct Node
│   │       Type resolved_type;       ← canonical after typecheck
│   │
│   ├── (2) Create resolve_pass() before ir_gen
│   │   └── example:
│   │       // src/resolve.c (new)
│   │       void resolve(Node *node) {
│   │           if (node->token->type == ID)
│   │               node->resolved_decl = get_variable(node->token->name);
│   │           ...
│   │       }
│   │
│   ├── (3) In ir_gen, read the pointer instead of looking up
│   │   └── example:
│   │       // before
│   │       Token *find = get_variable(node->token->name);
│   │       // after
│   │       Token *find = node->resolved_decl;  ← already resolved
│   │
│   ├── (4) Create typecheck_pass() after resolve
│   │   └── move compatible() / count checks / etc. here
│   │
│   └── (5) ir_gen progressively empties out
│       └── eventually fold into asm_gen
│
├── PAYOFF
│   ├── -typecheck-only flag (fast, no LLVM)
│   ├── solid base for a Language Server
│   └── code easier to understand and modify
│
└── EFFORT: ~4–6 weeks (the biggest internal refactor)
```

---

## ④ Language Spec

```
LANGUAGE SPEC
│
├── EXPLAINING THE PROBLEM
│   │
│   ├── Question: what are Ura's exact RULES?
│   │   ├── Operator precedence?
│   │   ├── Is `s.method()` legal on a temporary?
│   │   ├── When does `operator delete` fire?
│   │   └── Lifetime of a `ref`?
│   │
│   ├── Today: the only way to answer = read src/parse.c
│   │
│   └── Rules exist (encoded in C) but are not written down anywhere
│
├── MINIMUM DELIVERABLES (not a textbook, just the essentials)
│   │
│   ├── (a) GRAMMAR IN EBNF
│   │   └── example — docs/grammar.ebnf:
│   │       program     ::= top_level*
│   │       top_level   ::= fn_decl | struct_def | enum_def | global_var
│   │       fn_decl     ::= "pub"? "fn" IDENT "(" params? ")" ret_type ":"
│   │                       INDENT stmt+ DEDENT
│   │       params      ::= param ("," param)*
│   │       param       ::= IDENT type "ref"?
│   │       expr        ::= assign
│   │       assign      ::= or_expr (("=" | "+=" | ...) assign)?
│   │       binary      ::= unary (BIN_OP unary)*
│   │       primary     ::= INT | FLOAT | STRING | IDENT | "(" expr ")"
│   │       ...
│   │
│   ├── (b) LANGUAGE TOUR (5–10 pages of markdown)
│   │   └── example — docs/tour.md:
│   │       # Variables
│   │       x int = 5
│   │       y chars = "hello"
│   │
│   │       # Structs
│   │       struct Point:
│   │           x int
│   │           y int
│   │           fn dist() float: ...
│   │           pub fn origin() Point: ...
│   │
│   │       # Methods: `.` for instance, `::` for pub fn
│   │       p Point = Point::origin()
│   │       d float = p.dist()
│   │       ...
│   │
│   └── (c) STDLIB REFERENCE (generated from doc-comments)
│       └── example — docs/stdlib.md:
│           ## io
│           ### output(...args) void
│           Prints arguments joined to stdout.
│           ### read_line() chars
│           Reads a line from stdin.
│
├── PAYOFF (not always obvious)
│   │
│   ├── Forces you to confront edge cases
│   │   └── example:
│   │       writing the rule for `ref`, you realize
│   │       you never decided if `ref` can be reassigned.
│   │       Better to decide BEFORE it bites you.
│   │
│   ├── Allows another implementation
│   │   └── example:
│   │       someone wants to write a Ura interpreter in Python?
│   │       Without a spec → impossible. With a spec → doable.
│   │
│   └── Makes the language learnable
│       └── example:
│           today, learning Ura = read fixtures in tests/
│           with a spec, read docs/tour.md
│
└── EFFORT: ~30h, mostly writing prose
    └── Start with the grammar (1 weekend), fill in the rest per feature
```

---

## ⑤ Incremental Compilation

```
INCREMENTAL COMPILATION
│
├── EXPLAINING THE PROBLEM
│   │
│   ├── Today, every `ura myfile.ura` does EVERYTHING from scratch:
│   │   │
│   │   └── example — you change a comma in myfile.ura:
│   │       1. tokenize(myfile.ura)
│   │       2. tokenize(@/header) ← pulls io.ura, math.ura, ...
│   │       3. tokenize(@/io) ← 200 lines
│   │       4. tokenize(@/math) ← 150 lines
│   │       5. tokenize(@/string) ← 300 lines
│   │       6. ... (all the others)
│   │       7. parse everything
│   │       8. type-check everything
│   │       9. emit LLVM for everything
│   │       10. clang link
│   │
│   │       → 1700 lines reprocessed FOR ONE COMMA
│   │
│   └── At your scale (5K LoC) → near-instant, fine
│       At larger scale (100K LoC) → multi-minute builds, painful
│
├── EXPLAINING THE SOLUTION
│   │
│   ├── Idea: save the work already done, reuse it
│   │
│   ├── example inspired by C:
│   │   • foo.c → foo.o (compiled object, cached)
│   │   • bar.c → bar.o
│   │   • main.c → main.o
│   │   • build: if only main.c changed, only recompile main.c
│   │   • then link foo.o + bar.o + main.o
│   │
│   └── For Ura, the equivalent: .ura → .uracache
│
├── STEPS (simplest to most complete)
│   │
│   ├── (1) Cache tokens
│   │   └── example:
│   │       // after tokenize(io.ura):
│   │       save build/cache/io.ura.tokens
│   │       on next build:
│   │       hash(io.ura) == cached_hash ? load tokens : re-tokenize
│   │
│   ├── (2) Cache the AST per file
│   │   └── example:
│   │       build/cache/io.ura.ast (serialization of Node*)
│   │       difficulty: pointers between nodes → need IDs
│   │
│   ├── (3) Track dependencies
│   │   └── example:
│   │       myfile.ura.cache contains:
│   │         ast: ...
│   │         deps: ["io.ura@hash1", "string.ura@hash2"]
│   │       on build: if any dep's hash changed, invalidate the cache
│   │
│   └── (4) Real separate compilation
│       └── example:
│           every .ura → its own .ll
│           clang links every .ll
│           only the modified .ura re-emits its .ll
│
├── FORESEEABLE DIFFICULTIES
│   │
│   ├── AST serialization (pointers everywhere)
│   │   └── example:
│   │       node->left, node->right, token->Fcall.ptr, ...
│   │       you need an ID system to recreate them
│   │
│   ├── Correct invalidation
│   │   └── example:
│   │       string.ura changes → all its dependents must invalidate
│   │       otherwise → very subtle prod bugs
│   │
│   └── Cross-file type identity
│       └── example:
│           myfile cache points to the String struct
│           you load the cache → you have to RECONNECT the pointer
│           to the freshly loaded copy of String from string.ura
│
└── EFFORT: large, defer
    └── Advice: do NOT do this until you have a program big enough
        to feel the slowness. At your current scale, gain = zero.
```

---

## Suggested order

```
PRIORITY
│
├── ① Error Recovery
│   └── biggest user-visible win, mechanical
│
├── ② Semantic Checks
│   └── interleave, 1 per session
│
├── ④ Language Spec
│   └── relatively short, clarifies everything else
│       (do this BEFORE ③ to define the contracts)
│
├── ③ Multi-Pass
│   └── biggest internal refactor, unlocks tooling and future work
│
└── ⑤ Incremental
    └── defer — not useful at your current scale
```

---

## Verification (per item)

```
DONE WHEN…
│
├── ① a file with 10 errors → 10 messages in 1 run, non-zero exit, no crash
├── ② every test in tests/errors/ passes + 30 new ones added and stable
├── ③ asm_gen has no get_variable / get_function calls; -typecheck-only flag works
├── ④ docs/grammar.ebnf and docs/tour.md exist; a newcomer can write 30 lines
└── ⑤ ura on a stdlib-heavy project: warm build 5–10× faster than cold
```

The full roadmap = ~1 year of part-time work. Each item is independently shippable.
