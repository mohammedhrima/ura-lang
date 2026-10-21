# Ura: onboarding for Claude Code on the personal laptop

This file is for Claude Code when it works on Ura from the user's personal laptop. It carries over what Claude learned in the sessions on the 1337 workstation: how the user works, the state of the code, the workflow, the decisions already made, and what is left to do. Claude's memory of those sessions does not move to the laptop. This file replaces it.

+ Written on 2026-10-21 on the 1337 workstation, at commit 240a072 of the refactor branch.
+ The code changes fast. Check a fact here against the code and git log before relying on it.
+ SPECS.md has the measured details of the workstation, every LLVM version from 12 to 23, and every platform Ura must run on. This file points to it instead of repeating it.
+ Claude Code reads CLAUDE.md by itself, not this file. To load this file in every session, put a CLAUDE.md at the repo root that contains the line @ONBOARDING.md

## 1. How to work with this user

### Talking

+ The user writes fast, informal English. Read for intent. Ask only when the intent is really unclear.
+ Keep replies short. One step per reply, then wait for the user. Never send a long answer with many parts at once: the user has asked for this many times.
+ Lead with the answer or the fix. Explain only when asked, and briefly.
+ When the user asks how to do something, show the code to write. When the user asks for the fix, give only the fix.
+ The user often debugs alone for a long time before asking. Find the real cause in their current code, not a generic answer.

### Code

+ The user writes the compiler. Do not edit src/main.c, src/ir.h or src/lib/header.h unless the user explicitly asks for it in that request, for example "fix it for me", "handle all of them" or "apply it". Otherwise show the change: the file, the line, and the code to put there.
+ "Don't edit code" means: explain or show, change nothing.
+ Read the current code very carefully before suggesting anything. The user renames things often: STRUCT_DEC became STRUCT, TEMPLATE_DEC became TEMPLATE. Never reuse a name from an older snippet, or from this file, without checking it.
+ No hardcoding. A fix must work for every input of that kind, not only for the example. Name the edge cases when they matter.
+ No over-engineering. The user rejects designs that add layers ("naaah i don't like this approach is over engineered"). Prefer the smallest change that fits their code: a plain enum and a switch, not a new struct type.
+ Match their style:
    - 4 spaces, braces attached (K and R), the .clang-format at the repo root, the pointer star on the name (int *p);
    - short names, few comments;
    - VS Code formats C files with clang-format on save.
+ Verify before claiming anything. Test a proposed change on a scratch copy (section 6), run the tests there, and report the real numbers. Say so when something was not tested.
+ When the user's code has a bug, point to the line and say what happens. Do not rewrite the function around it.

### Files and editor

+ The user keeps files open in VS Code. When Claude edits an open file, the next save from the editor can overwrite Claude's edit with the old buffer. After an edit, tell the user to reload the file before saving. If an edit seems lost, read the file again before redoing it.
+ The user sometimes builds or runs tests at the same time as Claude. A "compile: bin/ura not found" in the middle of a run can come from that. Run again before drawing a conclusion.
+ Documentation that Claude writes in this repo (.md files such as this one and SPECS.md):
    - structure only with # headings and + or - lists, nested lists indented by 4 spaces;
    - no code blocks, no backticks, no tables, no bold, no emojis;
    - commands and code names are plain text.
+ Exceptions to that rule:
    - the test files (tests/milestoneN.md and src/tmp.md) need their code fences, because the test runner reads them;
    - files the user wrote in their own style stay as they are: README.md has a table, tests/milestone0.md has code blocks. Do not reformat them.
+ Never use emojis in a README, in any repo. Side repos such as arena_allocator may use badges, tables, diagrams and code blocks: only the ura-lang docs follow the +, - and # rule.
+ Code blocks are fine in chat replies.

### Git

+ Commit or push only when the user asks. Force push only when the user explicitly asks for it.
+ Commits carry only the user's identity: mohammedhrima, mohammedxhrima@gmail.com.
+ Never add a Co-Authored-By line for Claude, an Anthropic email address, or a "Generated with Claude Code" line, in commits, tags or pull requests. This rule overrides Claude Code's default attribution. The user once had such a commit removed from the history of the refactor branch.
+ The user's commit messages are short and lowercase, for example "milestone 0: readable spec, real C declarations and code snippets".
+ The "autosync on logout" commits come from the workstation's logout script. Leave them as they are unless the user asks.
+ Do not rewrite history (amend, rebase, reset) unless the user asks.

### Memory on the laptop

+ When the user gives a lasting rule on the laptop, save it in Claude Code's memory there, and offer to add it to this file so both machines share it.

## 2. What Ura is

+ A compiled, statically typed language with Python's indentation and C's speed. No braces, no semicolons.
+ The compiler is written in C. It builds LLVM IR through the LLVM C API, writes it to build/out.ll, and calls clang to turn it into an executable.
+ Syntax, as the tests use it now:
    - fn main() i32: then an indented body that ends with return 0. A function without a result: fn f():
    - parameters: fn add(a i32, b i32) i32:
    - variables: x i32 = 5. Without a value, a variable starts at zero: x i32
    - types so far: i32, i8, char (the same as i8), bool (True and False); arrays such as i8[] in protos;
    - indentation is exactly 4 spaces, anything else is an error; a body can sit on the same line: if x > 1: n += 1
    - control flow: if, elif, else, while, break, continue;
    - operators: + - * / % (signed), the comparisons, and, or, not, unary minus, and the compound assignments;
    - C functions: proto printf(fmt i8[], ...) i32
    - references: r &i32 = &x. A reference that can be null: next &Node
    - structs: struct Point: with indented fields. Templates: struct Box\<T\>: used as Box\<i32\>
    - modules: use "file" (the design is open, see section 8);
    - casts with as; line comments with //, and block comments.
+ Code outside functions runs at the start of main. A file with no main gets one, like a Python script.
+ README.md is the full feature list, milestone by milestone. The old compilers' syntax differs in places. The ? syntax for things that can be null is removed.

## 3. The rewrite and its history

+ The compiler is being rewritten from scratch, one milestone at a time. README.md lists every milestone, and tags each feature with the old compiler that had it.
+ src/old holds the three previous compilers, for reference only. The build never compiles them.
    - 001.c, 7467 lines: the biggest, with the best error messages and the most features (warnings, optimisation levels, -tree, -exec and others).
    - 002.c, 3690 lines.
    - 003.c, 3503 lines: the latest. It is the compiler of the workstation sessions, moved to src/old when the rewrite started (commit 7f4da5c, "fresh starts"). The recorded IR and error output of milestones 1 to 4 are its output.
+ 003.c implements every design of section 8. Where to find them in it:
    - tokenize at line 519, find_struct_type at 741, get_default_return at 858, parse_structs at 926;
    - parse_expr at 1302, find_variable at 1379, find_function at 1402, analyze at 1716;
    - codegen_global at 2268, codegen at 2412, handle_global_scope at 2686, main at 2733;
    - check_errors at 3187, print_errors at 3192, error at 3226, fix_line at 3350, edit_distance at 3384, did_you_mean at 3408.
+ When the user asks how to do something in the rewrite, look at how 003.c did it first, then adapt it to the new code. The names and the structs differ.
+ The git tag legacy points to 6039042, "remove the vscode extension (snapshot before cleanup)". It is the repo before the cleanup, with the old docs, src/random, src/tests and the VS Code extension. Example: git show 6039042:src/random/docs/ROADMAP.md

## 4. Repository

+ Remote: git@github.com:mohammedhrima/ura-lang.git
+ Branches: main (the default), dev, and refactor, where the work happens.
+ Every file:
    - README.md: the milestone plan and the feature list. The user's file.
    - SPECS.md: machines, LLVM versions, platforms. Not committed yet when this file was written.
    - ONBOARDING.md: this file. Not committed yet when it was written.
    - LICENSE (MIT), .gitignore, .clang-format.
    - .vscode/settings.json: clang-format on save, C/C++ error squiggles off, some files hidden from the explorer.
    - .vscode/c_cpp_properties.json: IntelliSense reads src/compile_commands.json (written by build), with /usr/lib/llvm-12/include and /usr/bin/clang as fallbacks. Change them on a machine where LLVM is somewhere else.
    - src/main.c: the core of the new compiler. Only the tokenizer for now.
    - src/ir.h: the LLVM wrapper, named uir.
    - src/lib/header.h: the shared tools library (arena, lists, strings, files, format).
    - src/proto.h: generated by build. Never edit it by hand.
    - src/config.sh: the task shell (build, tests, record, copy).
    - src/old/001.c, 002.c, 003.c: the old compilers.
    - tests/milestone0.md: the specification of the LLVM wrapper. It has no test entries.
    - tests/milestone1.md to tests/milestone13.md: the tests, one file per milestone.
    - ura-lib: the standard library, proto declarations over the C library written in Ura, still in the old syntax. common.ura is loaded into every program; use "@/header" loads the common set; the compiler finds the library next to its binary, or through the URA_LIB variable. ura-lib/README.md documents it.
+ Ignored by git, so never on the other machine: build/, bin/, src/compile_commands.json, *.out, src/tmp.md, src/main.ura.
+ README.md is behind the code in two places: it says src/main.c is empty, and it calls the wrapper src/ir.c, "not written yet". The wrapper is src/ir.h. Ask the user before changing README.md.

## 5. State of the code on 2026-10-21

### src/main.c (638 lines)

+ Structs: Token, Node, uraError (one error), NodePrint (for printing trees), and the global struct ura: the tree, the tokens, the scopes and the current scope, the files, the errors, and more.
+ Tables: keywords (bool, char, i8, i32, True, False, struct and the others) and specials (the punctuation, with the longer ones such as += and ... before the single characters).
+ Functions: new_token, parse_token, tokenize, main, the printing helpers (_print_type, _print_char, _print_token, _print_node_helper, is_open, _print_node, _print, _fprint), check_errors, includes, parse_args, parse_escaped.
+ print, help and eprint are macros over _print. It understands the printf conversions plus Ura's own, for example %k for a token and %n for a tree.
+ includes(type, ...) checks a type against a list of types that ends with 0.
+ parse_args takes .ura files and -o name, and exits with a usage message on anything else.
+ Stages are switched by macros. TOKENIZE defaults to 1. AST, ANALYZE and ASM are not defined yet, so only the tokenizer runs. main prints every token, then "did allocate N" (the bytes taken from the arena).
+ The AST, ANALYZE and ASM blocks in main already call functions that the rewrite does not have yet: parse_structs, parse_expr, analyze, codegen_beg, codegen, codegen_finish, gen_bin and others. That is how 003.c was organised.
+ The uraError struct and the ura.errors list exist, but nothing adds errors to the list yet. check_errors exits with status 1 when the list is not empty. Nothing prints errors yet.
+ Observed on 2026-10-21: in a scratch build, bin/ura a.ura -o e on a two-line program did not finish within 5 seconds. Not investigated.
+ The tests cannot pass right now: the runner reads build/out.ll, which the new main.c does not write yet. That is expected during the rewrite.

### src/ir.h (517 lines)

+ It includes lib/header.h and the LLVM-C headers. With LLVM 13 and newer it uses the new pass manager (PassBuilder.h), with LLVM 12 the legacy one (PassManagerBuilder.h).
+ Short names for the LLVM types: TypeRef, Context, Module, Builder, Bloc, Value, TargetData, Target, TargetMachine and others.
+ enum Type: every kind of token, node and value in one enum (NONE, ERR, SCOPE, URA, ID, VAR, DECLARE, INIT_VAR, LOAD, ARG and the rest). to_string gives a Type's name.
+ The uir global: the LLVM context, module and builder; the basic types (vd, i1, i8, i16, i32, i64); the output directory, the .ll path and the executable path; the list of IR objects.
+ It includes proto.h after its declarations, so every function of main.c and ir.h can call every other one.
+ Functions:
    - set_exec_path, uir_start, uir_end, uir_exit;
    - the llvm_ functions turn IR objects into LLVM: llvm_fn_dec, llvm_fn_entry, is_bloc_terminated, llvm_return, llvm_val_create, llvm_declare, llvm_zero_init, llvm_assign, llvm_load, llvm_compile;
    - the ir_ functions build uraIR objects: new_ir, ir_fn, ir_entry, ir_add_param, ir_num, ir_variable, ir_return, ir_load;
    - to_string.

### src/lib/header.h (279 lines)

+ The shared tools library, meant for Ura and for other projects.
+ Arena: new_arena, arena_reserve, arena_alloc(count, size), arena_free, arena_size. Everything is allocated from the arena, and freed once, at exit.
+ Lists: expand(T, name) declares name, name_count and name_size. append(name, item) grows the list inside the arena. zero_init(name) empties it.
+ Strings: strdup2, strndup2, strcmp2, strncmp2, strchr2. Watch out: strcmp2 and strncmp2 return true when the strings are equal, the opposite of strcmp.
+ Files and paths: getcwd2, dirname2, mkdir2, new_file, open_file, close_file.
+ format(fmt, ...) prints into a new string through the vfprint macro, which is vfprintf unless it was defined before the include.
+ Colors: GREEN, RED, CYAN, DIM, BOLD, RESET.
+ FILE, LINE and FUNC are macros for the current source file, line and function. Because FILE is taken, the stdio file type is written ioFile.
+ The comment at the top is the URA file header made by psi-header (section 10).

### src/proto.h

+ build regenerates it from main.c and ir.h with Universal Ctags: every function, without main.
+ It is committed, so a build without ctags still works. But then new functions get no prototype.

## 6. Build and test

### The task shell

+ Load it from src, in bash or zsh: cd src, then source config.sh
+ It puts bin/ on PATH, adds (ura-lang) to the prompt, and defines these commands:
    - build: compile the compiler into bin/ura, at the repo root;
    - tests: rebuild, then run every test file. tests milestone1 runs one file. tests milestone1 4 runs only its entry 004;
    - record: rebuild, then rewrite the recorded IR (same arguments as tests). It only rewrites llvm ir blocks: an entry that does not compile, an error test included, is left as it was;
    - copy file.ura milestone1 "title": add a program as a new entry, with its recorded IR, or its stderr and status when it does not compile. An entry with the same title is replaced in place;
    - update: source config.sh again after editing it;
    - extension: out of date. It packages ../0.ura-vscode-extension, which no longer exists (the extension was removed in 6039042).

### What build does

+ CC is clang, with -fsanitize=address -g3 -Werror. Every warning breaks the build. AddressSanitizer, with its leak checker, runs in every compile.
+ It picks the newest llvm-config it can find: on PATH, versioned names such as llvm-config-12, /usr/lib/llvm-N, the Homebrew prefixes, and llvm folders in the home directory. LLVM_CONFIG=/path/to/llvm-config overrides the choice. It prints "using LLVM version (path)".
+ It links llvm-config --libs core native bitwriter passes ipo, with an rpath to LLVM's library folder.
    - A static LLVM also gets the C++ runtime.
    - A system library that only exists as a versioned runtime (such as libzstd.so.1) is linked through a temporary symlink.
+ It regenerates src/proto.h (needs Universal Ctags) and src/compile_commands.json.
+ It compiles only src/main.c. main.c includes ir.h, which includes lib/header.h and proto.h.

### How the compiler is called

+ ura file.ura -o exe
+ It writes the LLVM IR to build/out.ll under the current directory, then links it with clang. clang must be on PATH when the compiler runs.

### How tests works

+ Each entry is compiled alone, as test.ura in its own temporary directory, so messages read test.ura:4:15.
+ A compile gets 10 seconds where the timeout command exists. macOS does not have it.
+ An entry with a "### status" section expects the compile to fail, with exactly that stderr and that exit status. Every other entry must compile, and its IR must match its "### llvm ir" block exactly.
+ Colors are removed from stderr. An internal message such as "main.c:1165 find_function message" becomes "find_function: message", so moving lines in main.c does not break tests.
+ The output has one line per entry: green when it passes, red when it fails, with a file:line link that opens the entry in VS Code with ctrl+click. A failure shows a short diff under its line. The totals come last.
+ The programs are never run. The expect and stdout comments are checked by hand.
+ record changes what the tests expect. Use it only when the output changes on purpose, or when a milestone starts. Look at the diff before keeping it.

### Test file format

+ It applies to every tests/milestoneN.md and to src/tmp.md.
+ The first line is # milestoneN. Then ## index, with one line per entry: "- 001 — title", with an em dash. The runner rewrites the index itself.
+ Entries are separated by a line of three dashes.
+ An entry is "## 001 — title", a blank line, then a code fence tagged ura that holds the program.
+ For a program that compiles: "### llvm ir", then a code fence tagged llvm with the recorded IR.
+ For a compile error: "### stderr" with a plain code fence that holds the exact stderr, then "### status" with a plain code fence that holds the exit status (1).
+ Comments at the top of a program say what it must do:
    - // expect: N, the exit code;
    - // stdout: one comment per printed line;
    - // stderr: a runtime message;
    - // error: and // help: for compile errors.
+ An error looks like this, line by line:
    - error: test.ura:4:15 unknown character '$'
    - the source line, with its number and a | gutter;
    - a ^ under the faulty part;
    - help: remove it

### Test status

+ Recorded entries, from README.md: milestone1 176 of 241, milestone2 80 of 127, milestone3 87 of 136, milestone4 13 of 13.
+ Milestones 5 to 13 are not recorded yet. Fix the syntax of a milestone file when that milestone starts, then run record on it.
+ Each milestone includes the features of the earlier ones, and tests runs every file, so the old milestones guard against regressions.
+ src/tmp.md holds draft entries, in the test format, before they move into a milestone file. src/main.ura is a hand test program. Both are ignored by git, so they do not travel between machines.

### Testing a change without touching the user's tree

+ Make a scratch directory with mktemp -d, and copy src and tests into it.
+ In the copy's src: source config.sh, apply the change there, build, then run tests.
+ Report the totals, and each failure with its cause.

## 7. Toolchain and platforms

+ The reference toolchain is the workstation's: Ubuntu 22.04 on x86_64, clang and LLVM 12.0.1 (llvm-config-12), Universal Ctags 5.9.0. SPECS.md section 2 has every detail.
+ Every recorded IR depends on it: the target datalayout line, the triple x86_64-pc-linux-gnu, and typed pointers (i32*, i8*). With another LLVM version or another CPU, tests fail even when the compiler is right.
+ First session on the laptop:
    - run the checks of SPECS.md section 4, "On the personal laptop", and write the results into SPECS.md as a new section;
    - go through the checklist of SPECS.md section 9 before trusting a test run;
    - if the laptop's LLVM is newer than 12: install LLVM 12 (on Ubuntu 22.04: sudo apt install clang-12 llvm-12-dev) and pin it with LLVM_CONFIG=/usr/bin/llvm-config-12, or use the Docker image of SPECS.md section 7;
    - install Universal Ctags (the universal-ctags package), or proto.h stops being updated;
    - update .vscode/c_cpp_properties.json if LLVM is not in /usr/lib/llvm-12.
+ The goal: Ura must work on Linux, on Windows 10 and newer, and on every macOS machine (Intel and Apple Silicon), at Rust's level of portability. SPECS.md sections 3, 5, 6 and 8 hold the plan.
+ Rules for any LLVM code written now:
    - never ask LLVM what a pointer points to: Ura's own IR keeps the element types;
    - use only the 2 builders (LLVMBuildLoad2, LLVMBuildGEP2, LLVMBuildInBoundsGEP2, LLVMBuildStructGEP2, LLVMBuildCall2) and the InContext versions of the type and constant functions;
    - never build constant expressions with the LLVMConst operations: build instructions, and LLVM folds them;
    - keep every LLVM version check in one place, ir.h today;
    - from LLVM 19, use LLVMConstStringInContext2.
+ Portability problems already in the code, known and not fixed:
    - lib/header.h defines ioFile only on Linux, so the build fails on macOS until it gets a definition there;
    - open_memstream, libgen.h, unistd.h and a mkdir with a mode argument do not exist on Windows (SPECS.md section 6);
    - config.sh needs bash or zsh, and macOS has no timeout command.

## 8. Design decisions already made

These were decided with the user on the workstation, and 003.c implements most of them. Do not reopen them unless the user does.

### Pipeline and errors

+ The stages: tokenize, parse, analyze, IR, LLVM, clang. Each stage collects all its errors, and the compiler stops after the first stage that has errors.
+ Every error shows file:line:col, the source line, the faulty part underlined with ^, and a help line when there is a likely fix. In 003.c, fix_line builds the corrected line for the help.
+ An unknown name gets a "did you mean" help, from edit_distance (did_you_mean in 003.c).
+ Errors are collected while a stage runs, and printed at exit by a function registered with atexit. atexit runs the last registered function first, so register the printer after the arena cleanup: the printer then runs before the cleanup.
+ check_errors runs after each stage and exits with 1 when there are errors.
+ A user must never see an internal message such as "main.c:1180 parse_prime handle this case DOTS". Every case a program can reach, valid or not, gets a real error.
+ An empty char literal '' is not an error. The user decided it.

### Structs and global scope

+ Structs can be used before their declaration. parse_structs declares the structs of a scope before that scope's body is parsed. It works scope by scope, recursively. It does not move every struct to the global scope, because the user wants structs that exist only inside a scope.
+ The global scope works like a Python script (handle_global_scope):
    - declarations outside functions (variables, functions, protos, structs) stay at file level;
    - those variables become LLVM globals (codegen_global, with LLVMAddGlobal) that start at zero;
    - every other statement outside functions moves to the start of main, in order, and runs like main's own code, so a global with a value is assigned at the start of main;
    - a file without main gets one, which returns 0;
    - with several files, only one of them has main.
+ A declaration without a value starts at zero (INIT_VAR, llvm_zero_init).

### Names in the generated IR

+ The names are part of the recorded tests. Keep them exactly:
    - an alloca has the variable's name (x), a load is load.x, a load through a reference is dref.p, a field is obj.field;
    - an operation is named after its opcode (ADD, EQ and the others);
    - blocks are entry, then, next, endif, cond, endwhile, and dead for code after a return;
    - string globals are str, str.1 and so on.
+ tests/milestone0.md has the full list.

### Operators and references

+ Casts use as. Negation is not, or !.
+ The ? syntax is gone. A reference that can be null is declared like next &Node, and the null checks follow the isNullable logic of 003.c.

### Templates

+ struct Box\<T\>: follows this design.
+ find_struct_type parses a type name with its \<...\> part. The user wrote it on purpose to handle many edge cases: keep it.
+ The template itself is parsed, but never analyzed or compiled.
+ parse_structs saves on the template the position of the token after its colon: where the body starts.
+ The first use of Box\<i32\> creates an ordinary struct named Box\<i32\>, next to the template and linked to it, with i32 bound to T. Its body is parsed again from the template's tokens: save the token position, jump to the body start, parse the body inside the instance's scope, then restore the position.
+ Another use of Box\<i32\> returns the same instance, so it is one struct and one LLVM type.
+ Type lookup goes through the scopes from the inside out, and an instance's scope maps T to its concrete type. Nested templates then need nothing more.
+ Errors inside an instance point to the template's lines, and say which use caused them, for example "in Box\<i32\>".
+ A template that contains itself by value would be instantiated forever, so it needs a check. Through a reference it is fine.
+ Why parse again instead of copying the tree: copying needs a case for every node type, and a missing case is a silent bug.

### Modules (use): not decided

+ Ask the user before implementing anything here.
+ Before, use was textual inclusion: the tokenizer pasted the other file's tokens. With that model, the user decided that a use cycle is an error, and drafted tests in which use is transitive (main sees the names of a file used by a file it uses).
+ Later, Claude proposed Python's model:
    - each file is loaded once, keyed by its real path, with its own tokens and its own tree;
    - first every module's structs, functions and globals are declared, then the bodies are analyzed;
    - use makes a module's names visible only in the block where it is written, and is not transitive;
    - cycles are allowed;
    - a used module's top-level code becomes its init function, run once at program start, in dependency order;
    - LLVM names get the module's name as a prefix (file1.foo), except main and the protos;
    - when two used modules define the same name, the bare name is an error and file1.foo always works; your own name wins over an imported one;
    - use "lib/util" as lutil renames a module; a path is relative to the file that has the use.
+ The two models conflict on transitivity and on cycles. The user has not chosen yet.

### The LLVM wrapper (uir)

+ The core never handles LLVM types directly. ir.h is the only place that talks to LLVM.
+ Types: one plain enum Type, and a TypeRef llvm_type(Type) function with a switch when a mapping is needed. The user rejected a UirType struct as over-engineered.
+ Constructors take the type as a parameter (as ir_num does), not one function per type such as create_i32 and create_i64. They return a pointer allocated with arena_alloc.
+ tests/milestone0.md specifies the wrapper with ura_ names, as an immediate builder in src/ir.c. The code uses the uir_, ir_ and llvm_ prefixes, in src/ir.h. Ask the user which one wins before aligning them.
+ Settled LLVM details: dispose the message of LLVMVerifyModule even when it succeeds; pass the address of the error string to LLVMPrintModuleToFile.

### Printing and escapes

+ _print and _fprint are Ura's printf. format() in lib/header.h prints through the vfprint macro, so it can use Ura's printer. For that, the program must define vfprint before it includes lib/header.h, directly or through ir.h. Otherwise lib/header.h falls back to vfprintf.
+ The escaped table in main.c (char *escaped[256]) maps both ways: the letter after a backslash to its character (n to a newline), and a control character to its escape text (a newline to backslash n).
+ So printing must use it only for control characters, with iscntrl and an unsigned char index. Otherwise the letter n prints as a newline. _print_char in main.c does this.

### Arena and library rules

+ Everything is allocated from the arena. Nothing is freed one by one.
+ arena_free resets its head and current pointers, so the arena can be used again after it.
+ Helper names never match libc names: strdup2, not strdup. libLLVM calls libc's strdup and the other string functions, and a strdup defined by the program replaces libc's inside LLVM too. That caused a heap-use-after-free under ASan. That is why the helpers end in 2.
+ atexit needs a function that takes nothing and returns nothing. Wrap a cleanup that has another signature.
+ Prefer tables over long switches, like the keywords and specials tables in main.c. Use X-macros when an enum and its table of names must stay in sync.

## 9. Known issues in the current code

Seen while writing this file. The user decides when and how to fix them.

+ main.c defines vfprint as _fprint after it includes ir.h, so format(), compiled earlier inside lib/header.h, still uses vfprintf: Ura's conversions (%k, %n) do not work in format(). Declaring _fprint and defining vfprint above the include fixes it.
+ parse_escaped indexes escaped with a plain char (input[s + 1]). A byte above 127 gives a negative index. Cast it to unsigned char, as _print_char does.
+ bin/ura did not finish on a two-line program in a scratch run (section 5).
+ lib/header.h has no ioFile on macOS (section 7).
+ README.md is behind the code (section 4), and the extension command of config.sh points to a removed folder (section 6).

## 10. Laptop setup

+ Git: clone git@github.com:mohammedhrima/ura-lang.git and check out refactor. Set user.name to mohammedhrima and user.email to mohammedxhrima@gmail.com.
+ Packages on Ubuntu: clang-12 and llvm-12-dev (see section 7), universal-ctags, make, git, zsh. For debugging: gdb and valgrind.
+ VS Code extensions used on the workstation: anthropic.claude-code, ms-vscode.cpptools with its extension pack, psioniq.psi-header, sunshaoce.llvmir (LLVM IR highlighting), pkief.material-icon-theme, zhuangtongfa.material-theme.
+ Claude Code on the workstation had the caveman plugin active (very short replies). Install it on the laptop too if the user wants the same style.

### The URA file header (psi-header)

+ Its settings live in VS Code's user settings on the workstation, not in the repo. Recreate them on the laptop, in the user settings:
    - psi-header.config: author "Mohammed Hrima", authorEmail "mohammedxhrima@gmail.com";
    - psi-header.lang-config for c: begin and end are the 80-column line of stars of the header in src/lib/header.h; prefix "/*", suffix "*/", lineLength 80; cpp maps to c;
    - psi-header.templates for c: a blank line; the URA ASCII art exactly as in the header of src/lib/header.h, with the placeholders filename, author, authoremail and filecreated('YYYY/MM/DD HH:mm:ss') where that header shows their values; a blank line; updated: followed by the placeholder dateformat('YYYY/MM/DD HH:mm:ss'); a blank line. Every placeholder is written between double angle brackets. cpp maps to c;
    - psi-header.changes-tracking: isActive true, modDate "updated:", modDateFormat "YYYY/MM/DD HH:mm:ss".
+ Claude can rebuild these settings from this description and from the header of src/lib/header.h.

## 11. Side projects and names

+ arena_allocator, at git@github.com:mohammedhrima/arena_allocator.git, branch master:
    - the arena of lib/header.h as its own single-header library (header.h);
    - a word counter example in main.c;
    - a README with badges, tables and diagrams, and no emojis;
    - a .gitignore for *.out;
    - the last commit is 571e081, authored by mohammedhrima, mohammedxhrima@gmail.com;
    - it is no longer inside this repo.
+ Names chosen for the parts: uir for the LLVM wrapper, in use (the uir global, uir_start). dogu was proposed for the tools library of lib/header.h, and is not used in the code yet.

## 12. What comes next

+ Rewrite main.c stage by stage, following milestone 1 of README.md: tokenizer errors, then the parser (AST), the analyzer (ANALYZE), and codegen through uir (ASM), until tests milestone1 passes against the recorded IR again. Then milestones 2, 3 and 4, which are recorded too.
+ For milestones 5 to 13: fix each file's syntax when its milestone starts, then record it.
+ Decide the module model (section 8) before milestone 5. Rewrite ura-lib in the new syntax at milestones 5 and 6: string.ura relies on the removed ?? operator.
+ Decide the wrapper's names and file: ir.h with the uir_, ir_ and llvm_ prefixes, or ir.c with ura_ names as tests/milestone0.md says.
+ Measure the laptop and add it to SPECS.md. Build the Docker image of SPECS.md section 7 if the laptop has no LLVM 12.
+ Later: the portability roadmap of SPECS.md section 8.
+ When the user asks: commit SPECS.md and ONBOARDING.md, and add the CLAUDE.md that loads this file.
