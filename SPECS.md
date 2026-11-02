# Ura specs: machines, LLVM versions, platforms

What Ura depends on, measured on the reference machine, plus what changes on every other machine, LLVM version and platform Ura should run on.

+ Machine facts were measured on 2026-10-21 with read-only commands.
+ LLVM facts come from the official release notes (releases.llvm.org) and the release tags of github.com/llvm/llvm-project.
+ Anything this file could not measure is marked "check there".

## 1. Summary

+ The reference machine is Ubuntu 22.04 on x86_64, with clang and LLVM 12.0.1.
+ The recorded IR tests are only valid on that toolchain: they contain its data layout, its target triple and typed pointers.
+ Three things decide whether Ura works on a machine:
    - which LLVM version it is built against;
    - which target it compiles for: triple, data layout, C type sizes, object format, linker;
    - which OS the compiler itself runs on: POSIX calls, paths, processes, terminal.
+ Rust handles all three:
    - rustc ships its own pinned LLVM, so the LLVM on the machine never matters;
    - it knows every target's facts internally;
    - it hides each OS behind one standard library interface.
+ Section 8 is the roadmap to do the same for Ura.

## 2. Reference machine: 1337 workstation

### System

+ OS: Ubuntu 22.04.2 LTS (Jammy Jellyfish), codename jammy.
+ Kernel: Linux 5.15.0-191-generic, x86_64, little endian.
+ CPU: 12th Gen Intel Core i7-12700, 1 socket, 12 cores, 20 threads.
+ RAM: 15 GiB.
+ C library: glibc 2.35 (Ubuntu GLIBC 2.35-0ubuntu3.15).
+ Locale: en_US.UTF-8. Login shell: /bin/zsh.
+ Storage:
    - /goinfre is local ext4, 59 G. The repo lives in /goinfre/mhrima/workspace/ura-lang.
    - /home/mhrima is xfs, 4.7 G.
    - The seat changes between sessions (c3r7p3, c4r9p10, c4r7p9 were all used). Anything kept only on one seat's local disk may not follow you to another one.
+ Containers: Docker 27.3.1 is installed. Podman is not.

### Compiler and linker

+ clang: Ubuntu clang version 12.0.1-19ubuntu3.
    - /usr/bin/clang is /usr/lib/llvm-12/bin/clang.
    - Default target x86_64-pc-linux-gnu, thread model posix.
    - It uses the GCC 12 installation (/usr/lib/gcc/x86_64-linux-gnu/12) for crt files and libgcc.
+ gcc: the gcc command is gcc 10.5.0 (x86_64-linux-gnu-gcc-10). gcc-11 11.4.0 and gcc-12 12.3.0 are also installed.
+ Linker: GNU ld (binutils) 2.38. There is no lld.

### LLVM

+ Only LLVM 12.0.1 is usable.
+ Packages installed, all at version 1:12.0.1-19ubuntu3:
    - llvm-12, llvm-12-dev, libllvm12, llvm-12-tools, llvm-12-runtime, llvm-12-linker-tools;
    - clang-12, clang-tools-12, clang-tidy-12, lldb-12;
    - libclang-common-12-dev, libclang-cpp12, libclang1-12.
+ libllvm15 15.0.7 is installed as a runtime library only: there are no LLVM 15 headers and no llvm-config-15.
+ llvm-config: only llvm-config-12 (/usr/bin/llvm-config-12). There is no plain llvm-config.
    - version 12.0.1, prefix /usr/lib/llvm-12;
    - includedir /usr/lib/llvm-12/include, libdir /usr/lib/llvm-12/lib;
    - host target x86_64-pc-linux-gnu;
    - targets built: AArch64 AMDGPU ARM AVR BPF Hexagon Lanai Mips MSP430 NVPTX PowerPC RISCV Sparc SystemZ WebAssembly X86 XCore;
    - build mode RelWithDebInfo, assertions off, shared mode shared, RTTI on;
    - cflags: -I/usr/lib/llvm-12/include -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
    - ldflags: -L/usr/lib/llvm-12/lib
    - libs: -lLLVM-12
    - system libs: none.
+ libLLVM: /usr/lib/x86_64-linux-gnu/libLLVM-12.so.1 (SONAME libLLVM-12.so.1). /usr/lib/llvm-12/lib links to it.
+ LLVM-C headers: Analysis, BitWriter, Core, DebugInfo, Target, TargetMachine.
    - Transforms has the legacy headers: AggressiveInstCombine, Coroutines, InstCombine, IPO, PassManagerBuilder, Scalar, Utils, Vectorize.
    - There is no Transforms/PassBuilder.h. That is why ir.h uses the legacy pass manager below LLVM 13.
+ Sanitizer runtimes (/usr/lib/llvm-12/lib/clang/12.0.1/lib/linux): asan (static and shared), lsan (static), ubsan_standalone (static and shared), all for x86_64.

### Tools

+ Universal Ctags 5.9.0 (package universal-ctags 5.9.20210829.0-1): build uses it to regenerate src/proto.h.
+ GNU coreutils 8.32: timeout, mktemp and seq are used by config.sh.
+ GNU Make 4.3, CMake 3.22.1, git 2.34.1.
+ bash 5.1.16 and zsh 5.8.1: config.sh works in both.
+ Debuggers and checkers: gdb 12.1, lldb-12, valgrind 3.18.1.
+ Python 3.13.1 from pyenv (/opt/pyenv). The build doesn't need it.

### How the build uses all this (src/config.sh)

+ CC is clang. Compile flags: -fsanitize=address -g3 -Werror.
+ LLVM is found automatically:
    - it looks for llvm-config, llvm-config-3 to llvm-config-40 on PATH, /usr/lib/llvm-*/bin/llvm-config, the Homebrew prefixes and $HOME/llvm*;
    - it picks the highest version found;
    - setting LLVM_CONFIG skips the search.
+ Link flags:
    - llvm-config --ldflags;
    - an rpath to the LLVM libdir;
    - llvm-config --libs core native bitwriter passes ipo;
    - -lstdc++ (or -lc++ on macOS) when LLVM is a static library;
    - a temporary symlink when a system library only exists in its versioned form.
+ At run time, the Ura compiler calls clang to link build/out.ll into an executable, so clang must be on PATH wherever Ura runs.

### What the recorded tests assume

+ Every recorded IR snapshot in tests/ starts with this machine's:
    - data layout: e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128
    - target triple: x86_64-pc-linux-gnu
+ The snapshots use typed pointers: i32*, i8* and so on, 643 times in tests/milestone1.md.
+ So the snapshots only match LLVM 12 to 14 on x86_64 Linux. Section 3 explains why.

## 3. LLVM versions 12 to 23: what each one changes for Ura

### Releases

+ Every major from 12 to 23 exists. The latest point release of each:
    - 12.0.1, 13.0.1, 14.0.6, 15.0.7, 16.0.6, 17.0.6
    - 18.1.8, 19.1.7, 20.1.8, 21.1.8, 22.1.8, 23.1.4
+ From 18 on, the first release of a major is N.1.0 (18.1.0, 19.1.0, and so on).

### Changes, version by version

Only the changes that touch a compiler using the LLVM C API, like Ura, are listed.

+ LLVM 12, the reference:
    - typed pointers: every pointer type names what it points to (i32*);
    - only the legacy pass manager in the C API (PassManagerBuilder);
    - the explicit-type builders (LLVMBuildLoad2, LLVMBuildGEP2, LLVMBuildCall2 and the others) already exist.
+ LLVM 13:
    - the new pass manager reaches the C API: LLVMRunPasses, with pass builder options (Transforms/PassBuilder.h);
    - ptr, the opaque pointer type, is introduced as experimental;
    - LLVMGetInlineAsm takes a ninth argument (CanThrow);
    - LLVMIntrinsicCopyOverloadedName is deprecated in favour of LLVMIntrinsicCopyOverloadedName2;
    - using the legacy pass manager for the optimization pipeline is deprecated.
+ LLVM 14:
    - the legacy optimization pipeline is deprecated, to be removed after 14;
    - LLVMSetInstDebugLocation is deprecated in favour of LLVMAddMetadataToInst.
+ LLVM 15:
    - LLVM uses opaque pointers: every pointer type is ptr, and the IR prints ptr instead of i32*;
    - the functions that relied on typed pointers are deprecated in favour of the 2 variants (LLVMBuildLoad for LLVMBuildLoad2, LLVMBuildCall for LLVMBuildCall2, and so on);
    - many constant expression functions are removed (LLVMConstExtractValue, LLVMConstSDiv, LLVMConstFAdd and others): build instructions instead, and LLVM folds constants;
    - LLVMGetAggregateElement is added; LLVMGetElementAsConstant is deprecated;
    - LLVMGetCastOpcode and LLVMDeleteInstruction are added.
+ LLVM 16: the deprecated typed-pointer functions are removed.
    - LLVMBuildLoad, LLVMBuildCall, LLVMBuildInvoke, LLVMBuildGEP, LLVMBuildInBoundsGEP, LLVMBuildStructGEP, LLVMBuildPtrDiff;
    - LLVMConstGEP, LLVMConstInBoundsGEP, LLVMAddAlias;
    - also LLVMConstFNeg.
+ LLVM 17:
    - typed pointers no longer exist at all;
    - LLVMGetElementType no longer returns what a pointer points to;
    - LLVMContextSetOpaquePointers, the switch back to typed pointers, is removed;
    - the legacy pass manager is gone: LLVMPassManagerBuilderRef and its functions, the functions that add legacy passes, the pass initializers, and LLVMPassRegistryRef;
    - LLVMConstSelect is removed.
+ LLVM 18:
    - more constant expressions are removed: LLVMConstAnd, LLVMConstOr, LLVMConstLShr, LLVMConstAShr, LLVMConstZExt, LLVMConstSExt, LLVMConstZExtOrBitCast, LLVMConstSExtOrBitCast, LLVMConstIntCast, and the floating point casts;
    - LLVMCreateTargetMachineWithOptions is added, with a way to set the target ABI;
    - on X86, i128 now matches __int128 in GCC and clang, so the data layout string changes;
    - flag getters and setters are added: LLVMGetNNeg and LLVMSetNNeg, LLVMGetIsDisjoint and LLVMSetIsDisjoint, the fast-math flags, and operand bundles.
+ LLVM 19:
    - debug info moves from intrinsics to debug records: LLVMDIBuilderInsertDeclareBefore and AtEnd, and LLVMDIBuilderInsertDbgValueBefore and AtEnd, are removed and replaced by their Record variants;
    - LLVMConstICmp, LLVMConstFCmp and LLVMConstShl are removed;
    - LLVMConstStringInContext is deprecated in favour of LLVMConstStringInContext2;
    - LLVMConstNUWNeg and LLVMBuildNUWNeg are deprecated.
+ LLVM 20:
    - the x86_mmx type is removed (LLVMX86MMXTypeInContext and its type kind);
    - LLVMRunPassesOnFunction and LLVMGetBuilderContext are added;
    - LLVMSetInitializer accepts NULL to clear an initializer;
    - building LLVM itself on Windows needs Visual Studio 2019 16.8 or newer.
+ LLVM 21:
    - LLVMConstMul, LLVMConstNUWMul and LLVMConstNSWMul are removed;
    - LLVMConstDataArray is added.
+ LLVM 22:
    - every function that works on the global context is deprecated: LLVMInt32Type, LLVMVoidType, LLVMStructType, LLVMCreateBuilder, LLVMAppendBasicBlock, LLVMConstString, LLVMConstStruct and the others;
    - use LLVMContextCreate and the InContext versions instead;
    - LLVMGetOrInsertFunction is added.
+ LLVM 23:
    - the LLVMBr opcode is replaced by LLVMUncondBr and LLVMCondBr;
    - the operands of a conditional branch are reordered to match its successors, which breaks code that reads successors with LLVMGetOperand;
    - data layout pointer specs gain the flags z (null is all zeros) and o (null is all ones).

### Rules that make one codebase work from LLVM 12 to 23

+ Never ask LLVM what a pointer points to. Ura's own IR must keep the element type of every pointer, array and struct field.
    - This single rule is what lets the same code work with typed pointers (12 to 16) and opaque pointers (15 and up).
    - old/003.c breaks it 5 times with LLVMGetElementType(LLVMTypeOf(...)).
+ Always use the explicit-type builders: LLVMBuildLoad2, LLVMBuildGEP2, LLVMBuildInBoundsGEP2, LLVMBuildStructGEP2, LLVMBuildCall2. They exist in 12 and are the only ones left from 16.
+ Always use a context you created (LLVMContextCreate) and the InContext versions of every type, constant, block and builder function.
+ Never build constant expressions with LLVMConst operations (add, mul, casts, comparisons). Build the instruction: LLVM folds it into a constant when it can.
+ Pass manager: the legacy one below 13, the new one from 13. From 17, only the new one exists. ir.h already switches on LLVM_VERSION_MAJOR.
+ String constants: LLVMConstStringInContext below 19, LLVMConstStringInContext2 from 19.
+ Debug info, when Ura gets it: intrinsics below 19, debug records from 19.
+ Put every version check in one file, src/ir.c, so the rest of the compiler never sees an LLVM version.
+ Text IR is never comparable between versions (i32* or ptr, data layouts), so IR snapshot tests belong to one pinned version.

## 4. Where each platform gets LLVM

+ Ubuntu 22.04 (jammy):
    - apt has LLVM 12, 13, 14 and 15; this machine uses 12;
    - install with: sudo apt install clang-12 llvm-12-dev
+ Ubuntu 24.04 and newer:
    - apt has no LLVM 12, only newer versions;
    - check there with: apt-cache search llvm-config
    - for the reference toolchain, use the Docker image of section 7.
+ apt.llvm.org provides recent LLVM versions for Debian and Ubuntu.
+ macOS (Intel and Apple Silicon):
    - Apple clang from Xcode has no llvm-config and no LLVM C libraries, so Ura needs Homebrew's llvm, which is a current version with opaque pointers;
    - check there for older versions with: brew search llvm
    - config.sh already looks in /opt/homebrew/opt/llvm* (Apple Silicon) and /usr/local/opt/llvm* (Intel).
+ Windows 10 and newer:
    - MSYS2, clang64 environment: LLVM with llvm-config, headers and libLLVM, current version only, target x86_64-w64-windows-gnu;
    - official LLVM Windows releases: target x86_64-pc-windows-msvc, and linking needs Visual Studio Build Tools and the Windows SDK. Check there that the package has the LLVM-C headers and import library; expect no llvm-config;
    - WSL2 with Ubuntu 22.04: the same as the reference machine, the easiest way to develop on Windows.
+ Every platform: the LLVM GitHub releases publish prebuilt clang+llvm archives. Check there which platforms each version has.

### On the personal laptop

+ Run these and write the results in a new section of this file:
    - cat /etc/os-release and uname -a
    - lscpu and ldd --version
    - clang --version and clang -v
    - ls /usr/bin/llvm-config*, then --version, --cflags, --ldflags, --libs and --shared-mode for each
    - dpkg -l | grep -E 'llvm|clang|ctags'
    - ctags --version
+ Watch out: if the laptop has an LLVM newer than 12, build picks it automatically, and the IR tests fail even though the compiler is right. Set LLVM_CONFIG to an LLVM 12 llvm-config, or use the Docker image.

## 5. Targets: what changes per platform

+ Target triples:
    - Linux: x86_64-pc-linux-gnu, aarch64-unknown-linux-gnu;
    - macOS: arm64-apple-darwin (Apple Silicon), x86_64-apple-darwin (Intel);
    - Windows: x86_64-pc-windows-msvc (Visual Studio ABI), x86_64-w64-windows-gnu (MinGW).
+ Object and executable formats: ELF on Linux, Mach-O on macOS, PE/COFF on Windows. Windows executables need a .exe name.
+ C type sizes (they matter for proto declarations and ura-lib):
    - Linux and macOS are LP64: int is 32 bits, long and pointers are 64 bits;
    - Windows is LLP64: int and long are 32 bits, long long and pointers are 64 bits;
    - char is signed on x86 and on Apple arm64, but unsigned on aarch64 Linux;
    - wchar_t is 32 bits on Linux and macOS, 16 bits on Windows.
+ Calling conventions:
    - LLVM handles scalar arguments for every target;
    - passing or returning a struct by value to a C function needs ABI lowering in the compiler, per target. LLVM does not do it. Rust implements it for each target.
+ Data layout: take it from the target machine, as ir.h already does. It differs per target and per LLVM version.
+ Linking:
    - today, clang is used as the linker driver, so clang must be installed wherever Ura runs;
    - Rust can ship its own linker (rust-lld) so that a system linker is not always needed.

## 6. The compiler itself on each OS

+ POSIX calls the compiler uses (src/lib/header.h, src/ir.h, src/old/003.c):
    - open_memstream, getcwd, mkdir, realpath, dirname;
    - fork, execvp and waitpid, to run clang;
    - unistd.h and libgen.h.
+ macOS has all of them; open_memstream exists since macOS 10.13.
+ Windows with MSVC has none of them. Replacements:
    - open_memstream: a growing buffer of your own;
    - fork and execvp: _spawnvp, or CreateProcess;
    - realpath: _fullpath; getcwd: _getcwd;
    - mkdir: _mkdir, which takes no mode;
    - dirname: a small function of your own.
    + MinGW provides some of them (getcwd, mkdir, libgen.h), but not open_memstream or fork.
+ Terminal colors: Windows 10 consoles show ANSI escape codes only after virtual terminal processing is turned on. Windows Terminal handles them.
+ Line endings: a source file with CRLF line endings leaves a carriage return at the end of every line. Add a .gitattributes that keeps .ura files in LF.
+ Paths: Windows uses backslashes and needs the wide-character APIs for non-ASCII paths.
+ config.sh:
    - needs bash or zsh: on Windows use WSL, Git Bash or MSYS2; on macOS, /bin/bash is 3.2, but zsh is the default and works;
    - timeout does not exist on macOS: Homebrew's coreutils installs it as gtimeout;
    - ldconfig does not exist on macOS (config.sh already tolerates that).
+ ura-lib wraps POSIX headers (dirent, fcntl, errno, and others). They need separate Windows versions.
+ Sanitizers:
    - AddressSanitizer works on Linux, on macOS, and for the MSVC target on Windows;
    - LeakSanitizer is not available on macOS by default;
    - sanitizer support for MinGW is limited.

## 7. Docker image of the reference machine

+ Base image: ubuntu:22.04.
+ On an Apple Silicon Mac, run the image as linux/amd64 (the platform option of docker build and docker run). That keeps the triple and data layout of the recorded tests.
+ Packages to install with apt:
    - clang-12, llvm-12, llvm-12-dev, libllvm12;
    - universal-ctags, make, git, zsh;
    - gcc-12 and libc6-dev;
    - optional: gdb, valgrind, lldb-12.
+ Pin the toolchain inside the image:
    - LLVM_CONFIG=/usr/bin/llvm-config-12 in the environment;
    - make the clang command point to clang-12.
+ Use:
    - mount the repo into the container;
    - run it with your user id, so files created in the mount belong to you;
    - in src: source config.sh, then build and tests.
+ Risks:
    - AddressSanitizer may not run under amd64 emulation on Apple Silicon. If it fails there, build without -fsanitize=address inside the container only;
    - the image must not pick another LLVM: check that build prints "using LLVM 12.0.1".

## 8. Reaching Rust's level: the roadmap

+ Ship a pinned LLVM:
    - link one LLVM version statically into Ura's release builds, the way Rust and Zig do;
    - users never install LLVM, and the IR snapshot tests mean the same thing everywhere.
+ Keep development builds working from LLVM 12 to 23 with the rules of section 3. Test the oldest and the newest supported version in CI.
+ Targets:
    - a --target option, defaulting to the host;
    - a built-in table of every target's facts: pointer width, C type sizes, char signedness, object format, executable suffix, linker.
+ Conditional compilation per platform in the language, like Rust's cfg. The old compiler had @if linux, @if macos and @if windows.
+ One OS layer for the compiler: processes, paths, files, buffers, colors. It needs POSIX and Windows implementations, in the shared library (src/lib/header.h).
+ C ABI lowering per target, for structs passed to and returned from C functions.
+ Tests that travel:
    - IR snapshot tests compiled with an explicit --target, on the pinned LLVM;
    - runtime tests, from the expect and stdout comments, run on every platform.
+ A CI matrix: Linux x86_64 and arm64, macOS on Apple Silicon and Intel, Windows with MSVC and with MinGW.

## 9. Checklist before trusting a test run on a new machine

+ uname -m prints x86_64.
+ build prints "using LLVM 12.0.1".
+ ctags --version says Universal Ctags.
+ clang is on PATH, because the Ura compiler calls it at run time.
+ tests passes on a clean checkout.
