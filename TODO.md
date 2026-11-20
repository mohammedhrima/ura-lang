# ura plan

## milestone 1: test cleanup (done)

## milestone 2: language features (now)

### sized arrays
postponed to milestone 4 (tests use `Array<T>` meanwhile)

### unic: 32-bit Unicode code point (like Rust's char)
```ura
c unic = '🦀'
output(c)                  // prints its UTF-8 bytes
for u in s.unics():        // a String's code points (String stays UTF-8 bytes)
    output(u)
// nothing existing changes: char stays i8 (C's char), char[], String, libc protos stay as they are
```

### ura::line, ura::filename, ura::function (like __LINE__, __FILE__, __func__ in C)
```ura
io::err(ura::filename, ":", ura::line, " in ", ura::function, "\n")
```

### later: throw records where it happened
```ura
throw Error::create("no file")         // fills line, filename, function by itself
catch e Error:
    output(e.filename, ":", e.line, " in ", e.function, "\n")   // main.ura:12 in load
// an uncaught error prints it too: error: no file (main.ura:12 in load)
```

### variadic functions written in ura
```ura
fn sum(count i32, ...) i32:   // read the extra arguments inside
```

### tuples
```ura
fn divmod(a i32, b i32) (i32, i32):
    return a / b, a % b

q, r = divmod(7, 2)
```

### function values (callbacks)
```ura
fn apply(f fn(i32) i32, x i32) i32:
    return f(x)

apply(square, 3)
// tests/milestone13.md 010-013, 015, 019, 039 test it
// (fn-typed values, parameters, a zeroed fn value traps)
```

### ** power operator (like Python)
```ura
x i32 = 2 ** 10
f f64 = 2.0 ** 0.5
x **= 2
2 ** 3 ** 2 == 2 ** 9      // binds tighter than *, right-associative
```

### i128 / u128
```ura
big i128 = 170141183460469231731687303715884105727
// ucg: 128-bit int types (LLVM i128, / and % through __divti3), and printing them
// ura: i128 and u128 keywords
```

### big integers, provided by ucg (so any compiler built on it gets them)
```ura
n bigint = 2 ** 200
// ucg: a bigint type (limbs) with + - * / % and compare, and printing
```

### any (not decided)
```ura
fn show(x any): ...        // works like a template
```

## milestone 3: fmt and io

### fmt as a parameter type
```ura
fn out(text fmt) i32:
    return write(1, text.data, text.count)

io::out("x = ", x, "\n")   // arguments formatted at compile time
```

### f strings, sugar over fmt
```ura
io::out(f"hp: {hp}, mp: {mp}\n")      // same as io::out("hp: ", hp, ", mp: ", mp, "\n")
```
then change output calls like this one (the starter's src/shapes.ura, in template_files in src/project.c) to f strings:
```ura
output(
    f" at ($(self.origin.x as i32) ",
    f"$(self.origin.y as i32) area ",
    f"area as i32\n"
)
```

### operator fmt (a struct prints itself)
```ura
operator fmt() String:
    return fmt("(", self.x, ", ", self.y, ")")
```

### colours as globals in io
```ura
io::out(io::red, "failed", io::reset, "\n")
// then rewrite tests/milestone6.md 005, 007, 010, 011 (old errput colour tests) for io::err
```

### io in core, output / errput builtins removed
```ura
io::out(...)
io::err(...)
name String = io::input("Your name: ")
// buffered output instead of one dprintf per call
```

### alias + Stream (files, sockets)
```ura
alias print = io::out
f Stream = io::open("a.txt")
f.write("hi ", n, "\n")
line String = f.getline()
```

### scan (reverse of fmt)
```ura
scan("12 3.5", &a, &b)
```

### organize the std repos (ura-posix, ura-math, ura-graphics) like ura-core
a folder per area, one struct with methods each; root .ura files use their folder, like core.ura uses core/
```
io/      Stream, files
net/     Socket
math/
...
```

## milestone 4: memory and safety

### bitmap arena allocator, default for every ura program
```
1 bit per 16-byte block:  1001 = blocks 1, 4 used, 2, 3 free
sits behind alloc / free, so Array, String work unchanged
```

### stack arrays and heap arrays (later, maybe as Array instances made at compile time)
```ura
rooms Room[] = Room[2]        // zeroed, on the stack; the size can be a runtime value
grid Room[][] = Room[2][3]
a i32[] = new<i32>(5)         // heap
// no .len on raw arrays; Array<T> has size()
```

### memory builtins renamed
```ura
memory_alloc(W * H) as char[]   ->   new<char[]>(W * H)
memory_free                     ->   clean(arr)
```

### new before a constructor: copies the stack value to the heap
```ura
p &Vec3 = new Vec3::create(1, 2, 3)
new Vec3{x: 1}                // error: only before Type::create(...)
new 5                         // error
// operator drop cleans fields only, the same for stack and heap values
```

### arena decorator on a fn (like Zig's arena allocator, design later)
```ura
@arena
fn frame():                   // every heap allocation in it is freed when the fn exits
```

### assignment the C++ way
```ura
x = y                         // operator = runs in place; a move operator for temporaries
// then operator drop never sees a never-set (zeroed) value
// today: operator drop must be safe on zeros (tests/milestone8.md 033 guards with if self.id != 0)
```

### optimization flags
```
ura main.ura -O2              -O0 .. -O3, -Os, -Oz
in ucg_start: LLVM passes for -O, ASAN setup for -san
```

### later: function annotations, like Python decorators
```ura
@cache                        // remembers results by arguments (Python's @lru_cache)
fn fib(n i32) i64:

@vector                       // the compiler runs its math as SIMD vectors (for graphics)
@gpu                          // or on the GPU
fn shade(...):
```

### -san that reports the ura file:line of the error
needs debug info (DWARF) from ucg

## milestone 5: tooling

### done: ura.conf (ura syntax, @if works)
```ura
@name "raytracer"
@version "0.1.0"
@main "src/main.ura"
@script "test" "ura tests/main.ura -o build/t && ./build/t"
@dependency "name" "version" "https://github.com/user/repo"   // or a folder: always 3 strings
```
```
ura install                              clones go to <project>/dependencies/
ura install <name> <version> <where>
ura remove / list / <script>
```
- ura-lib split into repos ura-core, ura-posix, ura-math, ura-graphics (all at the dev tag 0.1.0); ura-lib deleted
- left: `@flags "-O2 -san"` once -O and -san exist (milestone 4); a distinct icon for ura.conf in the extension

### dependency without a url: found in the ura-computing organization
```ura
@dependency "ura-math" "0.1.0"                                   // clones github.com/ura-computing/ura-math
@dependency "geo" "0.2.0" "https://github.com/user/geo"          // clones the url given
```

### ura-core by default
- after careful thinking: ura adds ura-core to ura.conf by itself
- installing ura also installs ura-core (today: `ura init` doesn't add it, `ura install` clones it)

### ura install: a global package cache, copied into each project
```
ura install
   for each @dependency "ura-core" "0.1.0" "https://github.com/ura-computing/ura-core"
   +-- ~/.ura/packages/ura-core@0.1.0 exists?
   |       no  -> git clone --branch 0.1.0 <url> ~/.ura/packages/ura-core@0.1.0
   +-- copy ~/.ura/packages/ura-core@0.1.0  ->  <project>/dependencies/ura-core@0.1.0
```
- global folder: `~/.ura/packages/` (Linux, macOS), `%USERPROFILE%\.ura\packages\` (Windows)
- versions are numeric `a.b.c` only: `"latest"`, `"main"`, `"1.0"`, `"^1.2"` are errors
- the repo has no tag for that version: use the biggest version already in `~/.ura/packages/`;
  none there: clone the repo's biggest version tag
- `./folder` dependencies are copied straight into `dependencies/`, never cached
- already in `dependencies/`: skipped

### ura init: a basic, simple, beautiful starter
- today's spinning donut starter is overkill: replace it with a small clean program
- the donut moves to its own repo (milestone 6)

### ura watch (rebuild and rerun on every save, like nodemon)
```
ura watch
```

### ura doc: built-in docs in the terminal (design later)
```
ura doc Array          // the docs of a type, a fn or a library, read from the sources
ura doc SDL2
```

### print the AST as a tree
```
ura main.ura -tree
```

### Makefile instead of config.sh
```
make ucg                      builds only ucg
```
- either put ucg in a different repo, or make it a standalone build, so it can be reused
- test ucg with LLVM 12, 15, 21
- emit assembly / objects straight from LLVM, without writing the .ll file

## milestone 6: projects

### rewrite the raytracer with the new features (match, try, fmt, io)
done so far: modern syntax, one folder per struct (like vec3), own repo github.com/ura-computing/ura-raytracer

### ura-donut: the spinning donut demo in its own repo
- github.com/ura-computing/ura-donut, the donut that `ura init` makes today

### rewrite ura-tcp-server in the new syntax

### rewrite tests/milestone13.md's projects on today's model (new<T>/free<T>, values own, refs borrow)
025 calc, 026 libft, 027 net server, 028 net client, 032 dungeon boss, 033 calculator, 037 038 040 libft lists

### fix the READMEs of ura-libft and ura-tcp-server (before graphics)
they still say: clone ura-lib, download build/ura

### graphics
```
ura-sdl2   ura-raylib   ura-opengl   ura-wayland
     \          |           |          /
               ura-graphic on top of them
                 /                \
           uRaytracer          uRasterizer
```
- implement multi threading

### later
http lib, backend framework, ura init SDL2
test the ocmpiler in deftrent architectur using an emulator or something
write LLVM ABI in ura-lang and rewrite ucg and ura-lang using ura-lang

## milestone 7: cleanup
- fix all remaining TODOs in the codebase
- rename `place` to something readable
- format the codebase
- remove unnecessary files
- move any code that can be moved to lib/header.h
- how are ALLOC and MEMCPY .. handled in ucg ?
- make sure all github repos have proper descriptions and READMEs

## milestone 8: release (only when you say)
- merge refactor -> dev -> main, tag the std repos for the release
- rewrite git history, delete files that no longer exist (smaller repo)
- all repos live in the GitHub organization github.com/ura-computing

### ura syntax highlighting on GitHub
push the ura grammar to github.com/github-linguist/linguist, so GitHub colours .ura files and counts ura in the language bar

### VS Code extension in the store
publish _extension to the VS Code Marketplace (`vsce publish`), so anyone installs it from the Extensions view
the vs cod extions keep showing th erro even though i installed the depedecies

+ 30 push dsrl

## at the end: Makefile replaces config.sh, and Windows
- remove src/config.sh, a Makefile does build / tests / record / ucg
- the Makefile checks the build dependencies first and installs the missing ones (LLVM, clang, git, ctags, ...)
- src/lib/header.h on Windows: `fork` / `waitpid` (run_command), `open_memstream` (format), `realpath`, `dirname` have no Windows path yet
- `symlink2` already has the Windows path (`CreateSymbolicLinkA`), but it needs Developer Mode without admin rights:
  fall back to a directory junction, which works without it
