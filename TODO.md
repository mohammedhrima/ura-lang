+ phase 2: language features (now)
	+ sized arrays: postponed to phase 4 (tests use Array<T> meanwhile)
	+ unic: a new 32-bit Unicode code point type (like Rust's char); char stays i8 (C's char)
		+ c unic = '🦀'     output(c) prints its UTF-8 bytes
		+ for u in s.unics():   // a String's code points (String stays UTF-8 bytes)
		+ nothing existing changes: char, char[], String, libc protos stay as they are
	+ ura::line, ura::filename, ura::function   (like __LINE__, __FILE__, __func__ in C)
		+ io::err(ura::filename, ":", ura::line, " in ", ura::function, "\n")
	+ variadic functions written in ura
		+ fn sum(count i32, ...) i32:   // read the extra arguments inside
	+ tuples
		+ fn divmod(a i32, b i32) (i32, i32):
		+ q, r = divmod(7, 2)
	+ function values (callbacks)
		+ fn apply(f fn(i32) i32, x i32) i32:
		+ apply(square, 3)
		+ milestone13 010-013, 015, 019, 039 test it (fn-typed values, parameters, a zeroed fn value traps)
	+ ** power operator (like Python)
		+ x i32 = 2 ** 10      f f64 = 2.0 ** 0.5      x **= 2
		+ binds tighter than * and is right-associative: 2 ** 3 ** 2 == 2 ** 9
	+ i128 / u128
		+ ucg: 128-bit int types (LLVM i128, / and % through __divti3), and printing them
		+ ura: i128 and u128 keywords
	+ big integers, provided by ucg so any compiler built on it gets them
		+ ucg: a bigint type (limbs) with + - * / % and compare, and printing
		+ ura: n bigint = 2 ** 200
	+ any (not decided)
		+ fn show(x any): ...   // works like a template

+ phase 3: fmt and io
	+ fmt as a parameter type
		+ fn out(text fmt) i32: return write(1, text.data, text.count)
		+ io::out("x = ", x, "\n")      // arguments formatted at compile time
	+ f strings, sugar over fmt
		+ io::out(f"hp: {hp}, mp: {mp}\n")      // same as io::out("hp: ", hp, ", mp: ", mp, "\n")
	+ operator fmt (a struct prints itself)
		+ operator fmt() String: return fmt("(", self.x, ", ", self.y, ")")
	+ colours as globals in io: io::out(io::red, "failed", io::reset, "\n")
		+ then rewrite milestone6 005, 007, 010, 011 (old errput colour tests) for io::err
	+ io in core, output / errput builtins removed
		+ buffered output instead of one dprintf per call
		+ io::out(...)  io::err(...)
		+ name String = io::input("Your name: ")
	+ alias + Stream (files, sockets)
		+ alias print = io::out
		+ f Stream = io::open("a.txt")
		+ f.write("hi ", n, "\n")   line String = f.getline()
	+ scan (reverse of fmt)
		+ scan("12 3.5", &a, &b)
	+ organize the std repos (ura-posix, ura-math, ura-graphics) like ura-core: a folder per area, one struct with methods each
		+ io/ (Stream, files), net/ (Socket), math/, ...
		+ root .ura files use their folder, like core.ura uses core/

+ phase 4: memory and safety
	+ bitmap arena allocator, default for every ura program
		+ 1 bit per 16-byte block: 1001 = blocks 1, 4 used, 2, 3 free
		+ sits behind alloc / free, so Array, String work unchanged
	+ stack arrays and heap arrays (later, maybe as Array instances made at compile time)
		+ rooms Room[] = Room[2]      // zeroed, on the stack; the size can be a runtime value
		+ grid Room[][] = Room[2][3]
		+ a i32[] = new<i32>(5)       // heap
		+ no .len on raw arrays; Array<T> has size()
	+ new before a constructor: copies the stack value to the heap
		+ p &Vec3 = new Vec3::create(1, 2, 3)
		+ only before Type::create(...): new Vec3{x: 1} and new 5 are errors
		+ operator drop cleans fields only, the same for stack and heap values
	+ arena decorator on a fn (like Zig's arena allocator, design later)
		+ @arena above a fn: every heap allocation in it is freed when the fn exits
	+ assignment the C++ way: operator = runs in place on x = y, a move operator for temporaries
		+ then operator drop never sees a never-set (zeroed) value
		+ today: operator drop must be safe on zeros (milestone8 033 guards with if self.id != 0)
	+ optimization flags: ura main.ura -O2   (-O0 .. -O3, -Os, -Oz)
		+ in ucg_start: LLVM passes for -O, ASAN setup for -san
	+ later: function annotations, like Python decorators
		+ @cache above a fn: remembers results by arguments (Python's @lru_cache)
		+ @vector / @gpu above a fn: the compiler runs its math as SIMD vectors or on the GPU (for graphics)
	+ -san that reports the ura file:line of the error
		+ needs debug info (DWARF) from ucg

+ phase 5: tooling
	+ done: ura-config (ura syntax: @name @version @main @script, @if works)
		+ @dependency "name" "version" "https://github.com/user/repo" (or a folder): always 3 strings
		+ ura install / install <name> <version> <where> / remove / list / <script>; clones go to <project>/dependencies/
		+ ura-lib split into repos ura-core, ura-posix, ura-math, ura-graphics (all at the dev tag 0.1.0); ura-lib deleted
		+ left: @flags "-O2 -san" once -O and -san exist (phase 4); a distinct icon for ura-config in the extension
	+ ura watch   (rebuild and rerun on every save, like nodemon)
	+ ura main.ura -tree   (print the AST as a tree)
	+ Makefile instead of config.sh, make ucg builds only ucg (maybe its own repo)
		+ test ucg with LLVM 12, 15, 21
		+ emit assembly / objects straight from LLVM, without writing the .ll file
	+ installing ura also installs ura-core (today: `ura init` adds it, `ura install` clones it)

+ phase 6: projects
	+ rewrite the raytracer with the new features (match, try, fmt, io)
	+ rewrite ura-tcp-server in the new syntax
	+ rewrite milestone13's projects on today's model (new<T>/free<T>, values own, refs borrow):
		+ 025 calc, 026 libft, 027 net server, 028 net client, 032 dungeon boss, 033 calculator, 037 038 040 libft lists
	+ later: http lib, backend framework, ura init SDL2

+ phase 7: release (only when you say)
	+ merge refactor -> dev -> main, tag the std repos for the release
	+ rewrite git history, delete files that no longer exist (smaller repo)

+ fix all remaining TODOS in thecodebase
+ either put ucg in derent repo, or make it a standlone build, so it can be reusable
+ rename place to osmehting readble
+ format the codebase
+ remove unecessary files
+ move any code that can be moved to lib/header.h
+ how ALLOC and MEMECPY .. are handled in ucg ?


