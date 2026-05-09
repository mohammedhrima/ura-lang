# Ura — language tour

**Status:** planned. To be written under improvement ④. See [IMPROVEMENTS.md](IMPROVEMENTS.md).

This file will be a ~10-section walkthrough of every Ura feature with working code examples. Sections will mirror the categories in [ROADMAP.md](ROADMAP.md):

1. Hello world & `main()`
2. Variables and primitive types
3. Control flow (`if` / `elif` / `else`, `while`, `break` / `continue`)
4. Functions (declarations, ref params, variadic, `proto`)
5. Structs and methods (`pub fn`, `operator delete`, operator overloading)
6. Modules (`mod name:`, `::` access)
7. Arrays and lists (`stack[T](n)`, `heap[T](n)`, `list[T]`)
8. Refs (C++-style; pending redesign — see decisions log)
9. Enums
10. Casting (`as`)
11. Tuples
12. C interop (`proto fn`, `link`)
13. Memory (auto-destructor, scope-end cleanup)

Every example will be testable as a fixture under [src/tests/](../src/tests/).
