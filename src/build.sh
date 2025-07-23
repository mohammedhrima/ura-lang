#!/bin/bash

san_flags="-fsanitize=address,null"
debug_flags="-g3"
warn_flags="-Wall -Wextra -Werror=incompatible-pointer-types -Werror=int-conversion"

flags="$san_flags $debug_flags $warn_flags"
files="main.c utils.c asm.c"
llvm_flags="$(llvm-config --cflags --ldflags --libs core)"

build() {
    clang $files $llvm_flags $flags -o wcc
}

compile() {
    ./wcc file.w && rm -rf wcc
}

gen_asm() {
    llc file.ll -o file.s
}

build && compile && gen_asm
