+ phase 1: test cleanup (now)
	+ milestone1:
		+ finish old syntax tests (proto fn ... pointer -> proto name(...))
		+ better error messages, for example:
			+ "did you mean 'count'?"
			+ "'x' is used before its declaration, on line 5"
			+ "can't assign to a value"   (1 = a, f() = 3)
	+ milestone2, 3, 6, 8, 10, 11, 13: same work
		+ missing feature -> build it (phase 2)
		+ dropped feature -> delete the test
		+ good new message / right output -> record
	+ compare src/old/*.c with main.c, list the missing features
	+ go through the TODO comments in main.c and ucg.c

+ phase 2: language features
	+ array literals
		+ a i32[] = [1, 2, 3]
		+ rooms Room[] = Room[2]      // zeroed, on the stack; the size can be a runtime value
		+ grid Room[][] = Room[2][3]
		+ rooms.len
	+ for loop through operator in
		+ for i in Range::create(0, 10):
		+ for c in "hello":
		+ struct Range:
			+ operator in(x &i32) bool:   // writes the next value in x, False when done
		+ then turn milestone3 104 back into a for loop (rewritten with while for now)
		+ rewrite milestone10 (old loop, a..b, by, for ref) for for-in
	+ isZero(x) replaces isNullable(x) (rename it in the tests)
	+ null (only the literal, for libc)
		+ r &i32 with no value starts as null: rewrite milestone2 111 (expects an error today)
		+ if file == null:
	+ mod
		+ mod math:
			+ fn square(x i32) i32: ...
		+ math::square(3)
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
		+ io::out(...)  io::err(...)
		+ name String = io::input("Your name: ")
	+ alias + Stream (files, sockets)
		+ alias print = io::out
		+ f Stream = io::open("a.txt")
		+ f.write("hi ", n, "\n")   line String = f.getline()
	+ scan (reverse of fmt)
	+ organize all of ura-lib like core: a folder per area, one struct with methods each
		+ io/ (Stream, files), net/ (Socket), math/, ...
		+ root .ura files use their folder, like core.ura uses core/
		+ scan("12 3.5", &a, &b)

+ phase 4: memory and safety
	+ bitmap arena allocator, default for every ura program
		+ 1 bit per 16-byte block: 1001 = blocks 1, 4 used, 2, 3 free
		+ sits behind alloc / free, so Array, String work unchanged
	+ assignment the C++ way: operator = runs in place on x = y, a move operator for temporaries
		+ then operator drop never sees a never-set (zeroed) value
		+ today: operator drop must be safe on zeros (milestone8 033 guards with if self.id != 0)
	+ optimization flags: ura main.ura -O2   (-O0 .. -O3, -Os, -Oz)
	+ later: function annotations, like Python decorators
		+ @cache above a fn: remembers results by arguments (Python's @lru_cache)
		+ @vector / @gpu above a fn: the compiler runs its math as SIMD vectors or on the GPU (for graphics)
	+ -san that reports the ura file:line of the error
		+ needs debug info (DWARF) from ucg

+ phase 5: tooling
	+ ura.project in yaml
		+ name: raytracer
		+ flags: -O2 -san
		+ packages:  sdl2, math@1.0      (ura install with no name installs them)
		+ commands:  run: ..., test: ...   (ura test runs it)
	+ ura watch   (rebuild and rerun on every save, like nodemon)
	+ ura main.ura -tree   (print the AST as a tree)
	+ Makefile instead of config.sh, make ucg builds only ucg (maybe its own repo)
	+ installing ura also installs ura-lib

+ phase 6: projects
	+ rewrite the raytracer with the new features (match, try, fmt, io)
	+ rewrite ura-tcp-server in the new syntax
	+ rewrite milestone13's projects on today's model (new<T>/free<T>, values own, refs borrow):
		+ 025 calc, 026 libft, 027 net server, 028 net client, 032 dungeon boss, 033 calculator, 037 038 040 libft lists
	+ later: http lib, backend framework, ura init SDL2

+ phase 7: release (only when you say)
	+ merge refactor -> dev -> main, push ura-lib
	+ rewrite git history, delete files that no longer exist (smaller repo)

+ fix all remaining TODOS in thecodebase