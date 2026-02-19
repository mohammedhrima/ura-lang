
+ there is a bug when accessing struct attributes via printf


+ code need to be refactored to remove garbage code

+ see if it's possible to share the llvm struct:
    + for functions (FDEC, FCALL)
    + for structs

+ protect free unallocated memery, double free
    + use malloc_error_break
